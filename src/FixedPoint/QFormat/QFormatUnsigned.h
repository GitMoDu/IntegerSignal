#ifndef _INTEGER_SIGNAL_Q_FORMAT_UNSIGNED_h
#define _INTEGER_SIGNAL_Q_FORMAT_UNSIGNED_h

#include <stdint.h>
#include "Base/Operation.h"
#include "Base/TypeTraits.h"
#include "Base/BitSize.h"


namespace IntegerSignal
{
	namespace QFormatUnsigned
	{
		using namespace TypeTraits::TypeLimits;
		using namespace TypeTraits::TypeNext;
		using namespace TypeTraits::TypeSign;
		using namespace TypeTraits::TypeConditional;
		using namespace BitSize;

		/// <summary>
		/// Unsigned Q-format fixed-point value.
		/// Provides scaling, interpolation, and arithmetic for unsigned fixed-point math.
		/// 
		/// The Q-format scalar is always a power-of-2 value, specifically (max_value >> 1) + 1 for the underlying type.
		/// This means only certain fractional ranges are representable, and the scaling is not fully general.
		/// 
		/// Performance: This restriction enables all scaling and interpolation operations to use fast bit-shifting instead of division or floating-point math.
		/// As a result, all arithmetic is highly efficient and suitable for real-time and embedded applications.
		/// </summary>
		/// <typeparam name="unsigned_scalar_t">Underlying unsigned integer type.</typeparam>
		template<typename unsigned_scalar_t>
		class TemplateFormat
		{
		public:
			/// <summary>
			/// Underlying unsigned integer type.
			/// </summary>
			using scalar_t = unsigned_scalar_t;

		public:
			/// <summary>
			/// Maximum scalar value for unit in the Q-format.
			/// </summary>
			static constexpr scalar_t SCALAR_UNIT = (type_limits<scalar_t>::Max() >> 1) + 1;

		private:
			/// <summary>
			/// Number of bits to shift for scaling operations.
			/// </summary>
			static constexpr uint8_t BIT_SHIFTS = bit_count<SCALAR_UNIT>::value - 1;

			// Compile-time validation.
			static_assert((SCALAR_UNIT& (SCALAR_UNIT - 1)) == 0,
				"TemplateUnsignedQFormat: SCALAR_UNIT must be a power of two.");
			static_assert(BIT_SHIFTS == IntegerSignal::GetBitShifts(static_cast<uint64_t>(SCALAR_UNIT)),
				"TemplateUnsignedQFormat: BIT_SHIFTS must equal log2(SCALAR_UNIT).");
			static_assert(static_cast<uint64_t>(SCALAR_UNIT) == (uint64_t(1) << BIT_SHIFTS),
				"TemplateUnsignedQFormat: SCALAR_UNIT != (1 << BIT_SHIFTS).");

		private:
			/// <summary>
			/// Fractions an unsigned value by the Q-format scalar.
			/// </summary>
			template<typename T>
			static constexpr T Fraction(const scalar_t scalar, const T value, TypeTraits::TypeDispatch::TrueType)
			{
				using larger_t = typename larger_type<T, scalar_t>::type; // Larger of unsigned value type and unsigned scalar type.
				using intermediate_t = typename next_uint_type<larger_t>::type; // Next larger unsigned type for intermediate calculations.

				return static_cast<T>((static_cast<intermediate_t>(value) * scalar) >> BIT_SHIFTS);
			}

			/// <summary>
			/// Fractions a signed value by the Q-format scalar.
			/// </summary>
			template<typename T>
			static constexpr T Fraction(const scalar_t scalar, const T value, TypeTraits::TypeDispatch::FalseType)
			{
				using larger_t = typename larger_type<T, scalar_t>::type; // Larger of signed value type and unsigned scalar type.
				using intermediate_t = typename next_int_type<larger_t>::type; // Next larger signed type for intermediate calculations.

				return static_cast<T>((static_cast<intermediate_t>(value) * scalar) >> BIT_SHIFTS);
			}

		public:
			/// <summary>
			/// Checks if a scalar value is valid.
			/// </summary>
			static constexpr bool Valid(const scalar_t scalar)
			{
				return scalar <= SCALAR_UNIT;
			}

			/// <summary>
			/// Calculates a Q-format scalar from a ratio.
			/// </summary>
			template<typename T>
			static constexpr scalar_t GetScalar(const T numerator, const T denominator)
			{
				using larger_t = typename larger_type<T, scalar_t>::type; // Wider of input and scalar type.
				using intermediate_t = typename next_uint_type<larger_t>::type; // Next larger unsigned type for intermediate calculations.

				return numerator < 0 ? scalar_t(0) : denominator <= 0 ? SCALAR_UNIT : numerator > denominator ? SCALAR_UNIT :
					scalar_t((static_cast<intermediate_t>(numerator) << BIT_SHIFTS) / denominator);
			}

			/// <summary>
			/// Fractions a value by a given Q-format scalar.
			/// </summary>
			template<typename T>
			static constexpr T Fraction(const scalar_t scalar, const T value)
			{
				return Fraction<T>(scalar, value, typename IsUnsignedType<T>::type());
			}

			/// <summary>
			/// Fractions a value by a given Q-format scalar.
			/// </summary>
			template<typename T,
				scalar_t scalar>
			static constexpr T Fraction(const T value)
			{
				return Fraction<T>(scalar, value, typename IsUnsignedType<T>::type());
			}

			/// <summary>
			/// Interpolates between two values using the Q-format scalar.
			/// </summary>
			template<typename T>
			static constexpr T Interpolate(const scalar_t scalar, const T from, const T to)
			{
				using larger_t = typename larger_type<T, scalar_t>::type; // Larger of value type and unsigned scalar type.
				using intermediate_t = typename next_uint_type<larger_t>::type; // Next larger unsigned type for intermediate calculations.

				return static_cast<T>(
					((static_cast<intermediate_t>(to) * scalar) +
						(static_cast<intermediate_t>(from) * (SCALAR_UNIT - scalar))) >> BIT_SHIFTS);
			}
		};

		namespace Scalar
		{
			namespace Implementation
			{
				/// <summary>
				/// Reference implementation of GetScalar for unsigned types, constexpr-capable but not necessarily optimized for all platforms.
				/// </summary>
				namespace Constexpr
				{
					static constexpr uint8_t GetScalarU8(const uint8_t numerator, const uint8_t denominator)
					{
						return TemplateFormat<uint8_t>::GetScalar(numerator, denominator);
					}

					static constexpr uint16_t GetScalarU16(const uint16_t numerator, const uint16_t denominator)
					{
						return TemplateFormat<uint16_t>::GetScalar(numerator, denominator);
					}

					static constexpr uint32_t GetScalarU32(const uint32_t numerator, const uint32_t denominator)
					{
						return TemplateFormat<uint32_t>::GetScalar(numerator, denominator);
					}
				}

				/// <summary>
				/// Runtime-only implementation of GetScalar for unsigned types, optimized for platforms without fast division or where the denominator is small.
				/// </summary>
				namespace Runtime
				{
					inline uint8_t GetScalarU8(const uint8_t numerator, const uint8_t denominator)
					{
#if defined(INTEGER_SIGNAL_DISABLE_ACCELERATION)
						return Constexpr::GetScalarU8(numerator, denominator);
#else
						if (numerator >= denominator)
							return TemplateFormat<uint8_t>::SCALAR_UNIT;

						const uint16_t denominator16 = denominator;
						uint16_t remainder = numerator;
						uint8_t scalar = 0;

						remainder <<= 1;
						if (remainder >= denominator16)
						{
							remainder -= denominator16;
							scalar |= TemplateFormat<uint8_t>::SCALAR_UNIT >> 1;
						}

						remainder <<= 1;
						if (remainder >= denominator16)
						{
							remainder -= denominator16;
							scalar |= TemplateFormat<uint8_t>::SCALAR_UNIT >> 2;
						}

						remainder <<= 1;
						if (remainder >= denominator16)
						{
							remainder -= denominator16;
							scalar |= TemplateFormat<uint8_t>::SCALAR_UNIT >> 3;
						}

						remainder <<= 1;
						if (remainder >= denominator16)
						{
							remainder -= denominator16;
							scalar |= TemplateFormat<uint8_t>::SCALAR_UNIT >> 4;
						}

						remainder <<= 1;
						if (remainder >= denominator16)
						{
							remainder -= denominator16;
							scalar |= TemplateFormat<uint8_t>::SCALAR_UNIT >> 5;
						}

						remainder <<= 1;
						if (remainder >= denominator16)
						{
							remainder -= denominator16;
							scalar |= TemplateFormat<uint8_t>::SCALAR_UNIT >> 6;
						}

						remainder <<= 1;
						if (remainder >= denominator16)
						{
							scalar |= TemplateFormat<uint8_t>::SCALAR_UNIT >> 7;
						}

						return scalar;
#endif
					}

					inline uint16_t GetScalarU16(const uint16_t numerator, const uint16_t denominator)
					{
#if defined(INTEGER_SIGNAL_DISABLE_ACCELERATION)
						return Constexpr::GetScalarU16(numerator, denominator);
#else
						if (denominator == 0 || numerator >= denominator)
							return TemplateFormat<uint16_t>::SCALAR_UNIT;

						uint16_t scalar = 0;
						uint32_t remainder = numerator;

						for (uint16_t bit = TemplateFormat<uint16_t>::SCALAR_UNIT >> 1; bit != 0; bit >>= 1)
						{
							remainder <<= 1;
							if (remainder >= denominator)
							{
								remainder -= denominator;
								scalar |= bit;
							}
						}

						return scalar;
#endif
					}

					inline uint32_t GetScalarU32(const uint32_t numerator, const uint32_t denominator)
					{
#if defined(INTEGER_SIGNAL_DISABLE_ACCELERATION)
						return Constexpr::GetScalarU32(numerator, denominator);
#else
						static constexpr uint8_t ScalarBits = IntegerSignal::GetBitShifts(TemplateFormat<uint32_t>::SCALAR_UNIT);
						static constexpr uint8_t ScalarLoBits = sizeof(uint16_t) * 8;
						static constexpr uint8_t ScalarHiBits = ScalarBits - ScalarLoBits;

						if (denominator > UINT16_MAX)
						{
							return Constexpr::GetScalarU32(numerator, denominator);
						}
						else
						{
							if (denominator == 0u || numerator >= denominator)
								return TemplateFormat<uint32_t>::SCALAR_UNIT;

							const uint32_t scalarHi = (numerator << ScalarHiBits) / denominator;
							const uint32_t remainder = (numerator << ScalarHiBits) - (scalarHi * denominator);
							const uint32_t scalarLo = (remainder << ScalarLoBits) / denominator;

							return (scalarHi << ScalarLoBits) | scalarLo;
						}
#endif
					}
				}

				/// <summary>
				/// Policy-based selection of the appropriate GetScalar implementation for unsigned types.
				/// Chooses between runtime-optimized and constexpr implementations based on platform and configuration.
				/// AVR covers 8-bit MCUs basically, we use the fast runtime version for 8-bit and 16-bit scalars.
				/// For ARM, RISC-V and Extensa, 32-bit MCUs use only the 32 runtime-optimized scalar.
				/// </summary>
				namespace Policy
				{
					inline uint8_t GetScalarU8(const uint8_t numerator, const uint8_t denominator)
					{
						// 8-bit MCUs benefit runtime-optimized version for 8-bit scalars.
#if defined(__AVR__)
						return Runtime::GetScalarU8(numerator, denominator);
#else
						return Constexpr::GetScalarU8(numerator, denominator);
#endif
					}

					inline uint16_t GetScalarU16(const uint16_t numerator, const uint16_t denominator)
					{
						// 8-bit and 16-bit MCUs benefit runtime-optimized version for 16-bit scalars.
#if defined(__AVR__)
						return Runtime::GetScalarU16(numerator, denominator);
#else
						return Constexpr::GetScalarU16(numerator, denominator);
#endif
					}

					inline uint32_t GetScalarU32(const uint32_t numerator, const uint32_t denominator)
					{
						// 32-bit MCUs benefit from the runtime-optimized version for 32-bit scalars.
#if (INTPTR_MAX == INT32_MAX)
						return Runtime::GetScalarU32(numerator, denominator);
#else
						return Constexpr::GetScalarU32(numerator, denominator);
#endif
					}
				}
			}

			/// <summary>
			/// Explicit runtime-optimized versions of GetScalar for specific types and platforms.
			/// </summary>
			namespace Runtime
			{
				inline uint8_t GetScalarU8(const uint8_t numerator, const uint8_t denominator)
				{
					return Implementation::Runtime::GetScalarU8(numerator, denominator);
				}

				inline uint16_t GetScalarU16(const uint16_t numerator, const uint16_t denominator)
				{
					return Implementation::Runtime::GetScalarU16(numerator, denominator);
				}

				inline uint32_t GetScalarU32(const uint32_t numerator, const uint32_t denominator)
				{
					return Implementation::Runtime::GetScalarU32(numerator, denominator);
				}
			}

			/// <summary>
			/// Explicit constexpr versions of GetScalar for compile-time evaluation.
			/// </summary>
			namespace Constexpr
			{
				static constexpr uint8_t GetScalarU8(const uint8_t numerator, const uint8_t denominator)
				{
					return Implementation::Constexpr::GetScalarU8(numerator, denominator);
				}

				static constexpr uint16_t GetScalarU16(const uint16_t numerator, const uint16_t denominator)
				{
					return Implementation::Constexpr::GetScalarU16(numerator, denominator);
				}

				static constexpr uint32_t GetScalarU32(const uint32_t numerator, const uint32_t denominator)
				{
					return Implementation::Constexpr::GetScalarU32(numerator, denominator);
				}
			}

			inline uint8_t GetScalarU8(const uint8_t numerator, const uint8_t denominator)
			{
				return Implementation::Policy::GetScalarU8(numerator, denominator);
			}

			inline uint16_t GetScalarU16(const uint16_t numerator, const uint16_t denominator)
			{
				return Implementation::Policy::GetScalarU16(numerator, denominator);
			}

			inline uint32_t GetScalarU32(const uint32_t numerator, const uint32_t denominator)
			{
				return Implementation::Policy::GetScalarU32(numerator, denominator);
			}
		}
	}
}
#endif