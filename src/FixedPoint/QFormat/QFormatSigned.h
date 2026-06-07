#ifndef _INTEGER_SIGNAL_Q_FORMAT_SIGNED_h
#define _INTEGER_SIGNAL_Q_FORMAT_SIGNED_h

#include <stdint.h>
#include "Base/Operation.h"
#include "Base/TypeTraits.h"
#include "Base/BitSize.h"

namespace IntegerSignal
{
	namespace QFormatSigned
	{
		using namespace TypeTraits::TypeLimits;
		using namespace TypeTraits::TypeNext;
		using namespace TypeTraits::TypeConditional;
		using namespace BitSize;

		/// <summary>
		/// Signed Q-format fixed-point value.
		/// Provides scaling, interpolation, and arithmetic for signed fixed-point math.
		/// 
		/// Format restriction:</b> The Q-format scalar is always a power-of-2 value, specifically (max_value >> 1) for the underlying type.
		/// This means only certain fractional ranges are representable, and the scaling is not fully general.
		/// 
		/// Performance: This restriction enables all scaling and interpolation operations to use fast bit-shifting instead of division or floating-point math.
		/// As a result, all arithmetic is highly efficient and suitable for real-time and embedded applications.
		/// </summary>
		/// <typeparam name="signed_scalar_t">Underlying signed integer type.</typeparam>
		template<typename signed_scalar_t>
		class TemplateFormat
		{
		public:
			/// <summary>
			/// Underlying signed integer type.
			/// </summary>
			using scalar_t = signed_scalar_t;

		public:
			/// <summary>
			/// Maximum positive scalar value for unit in the Q-format.
			/// </summary>
			static constexpr scalar_t SCALAR_UNIT = SignedRightShift(type_limits<scalar_t>::Max(), 1) + 1;

			/// <summary>
			/// Maximum negative scalar value for unit in the Q-format.
			/// </summary>
			static constexpr scalar_t SCALAR_UNIT_NEGATIVE = -SCALAR_UNIT;

		private:
			/// <summary>
			/// Number of bits to shift for scaling operations.
			/// </summary>
			static constexpr uint8_t BIT_SHIFTS = bit_count<SCALAR_UNIT>::value - 1;

			// Compile-time validation.
			static_assert((SCALAR_UNIT& (SCALAR_UNIT - 1)) == 0,
				"TemplateSignedQFormat: SCALAR_UNIT must be a power of two.");
			static_assert(BIT_SHIFTS == IntegerSignal::GetBitShifts(static_cast<int64_t>(SCALAR_UNIT)),
				"TemplateSignedQFormat: BIT_SHIFTS must equal log2(SCALAR_UNIT).");

		public:
			/// <summary>
			/// Checks if a scalar value is valid.
			/// </summary>
			static constexpr bool Valid(const scalar_t scalarValue)
			{
				return scalarValue >= SCALAR_UNIT_NEGATIVE && scalarValue <= SCALAR_UNIT;
			}

			/// <summary>
			/// Calculates a Q-format scalar from a ratio.
			/// Uses multiplication by SCALAR_UNIT to match the representable signed range [-SCALAR_UNIT; +SCALAR_UNIT].
			/// Promotes to a wide intermediate to avoid overflow for 32-bit formats.
			/// </summary>
			template<typename ArgT>
			static constexpr scalar_t GetScalar(const ArgT numerator, const ArgT denominator)
			{
				using wider_value_t = typename larger_type<ArgT, scalar_t>::type;
				using intermediate_t = typename next_int_type<wider_value_t>::type;

				return (denominator == 0) ? (numerator >= 0 ? SCALAR_UNIT : SCALAR_UNIT_NEGATIVE)
					: LimitValue<intermediate_t, intermediate_t(SCALAR_UNIT_NEGATIVE), intermediate_t(SCALAR_UNIT)>((static_cast<intermediate_t>(numerator) << BIT_SHIFTS) / denominator);
			}

			/// <summary>
			/// Fractions a value by a given Q-format scalar.
			/// </summary>
			template<typename T>
			inline static constexpr T Fraction(const scalar_t scalarValue, const T value)
			{
				using larger_t = typename larger_type<T, scalar_t>::type;
				using intermediate_t = typename next_int_type<larger_t>::type;

				return static_cast<T>(SignedRightShift(static_cast<intermediate_t>(value) * scalarValue, BIT_SHIFTS));
			}
		};

		namespace Scalar
		{
			namespace Implementation
			{
				/// <summary>
				/// Q-format owns signed scalar-generation dispatch, including the fixed-width constexpr, runtime, and policy-selected entry points.
				/// Public facades such as FixedPoint::ScalarFraction forward here so behavior and explicit path selection remain centralized.
				/// </summary>
				namespace Constexpr
				{
					template<typename T>
					static constexpr int8_t GetScalarS8(const T numerator, const T denominator)
					{
						return TemplateFormat<int8_t>::GetScalar(numerator, denominator);
					}

					template<typename T>
					static constexpr int16_t GetScalarS16(const T numerator, const T denominator)
					{
						return TemplateFormat<int16_t>::GetScalar(numerator, denominator);
					}

					template<typename T>
					static constexpr int32_t GetScalarS32(const T numerator, const T denominator)
					{
						return TemplateFormat<int32_t>::GetScalar(numerator, denominator);
					}
				}

				/// <summary>
				/// Runtime-only implementation of GetScalar for signed types, optimized for platforms without fast division or where the denominator is small.
				/// </summary>
				namespace Runtime
				{
					inline int8_t GetScalarS8(const int8_t numerator, const int8_t denominator)
					{
#if defined(INTEGER_SIGNAL_DISABLE_ACCELERATION)
						return Constexpr::GetScalarS8(numerator, denominator);
#else
						if (numerator >= denominator)
							return TemplateFormat<int8_t>::SCALAR_UNIT;

						uint8_t scalar = 0;
						uint16_t remainder = static_cast<uint8_t>(numerator);
						const uint16_t denominator16 = static_cast<uint8_t>(denominator);

						remainder <<= 1;
						if (remainder >= denominator16)
						{
							remainder -= denominator16;
							scalar |= 0x20;
						}

						remainder <<= 1;
						if (remainder >= denominator16)
						{
							remainder -= denominator16;
							scalar |= 0x10;
						}

						remainder <<= 1;
						if (remainder >= denominator16)
						{
							remainder -= denominator16;
							scalar |= 0x08;
						}

						remainder <<= 1;
						if (remainder >= denominator16)
						{
							remainder -= denominator16;
							scalar |= 0x04;
						}

						remainder <<= 1;
						if (remainder >= denominator16)
						{
							remainder -= denominator16;
							scalar |= 0x02;
						}

						remainder <<= 1;
						if (remainder >= denominator16)
						{
							scalar |= 0x01;
						}

						return static_cast<int8_t>(scalar);
#endif
					}

					template<typename T>
					inline int8_t GetScalarS8(const T numerator, const T denominator)
					{
#if defined(INTEGER_SIGNAL_DISABLE_ACCELERATION)
						return Constexpr::GetScalarS8(numerator, denominator);
#else
						return denominator == 0 ? (numerator >= 0 ? TemplateFormat<int8_t>::SCALAR_UNIT : TemplateFormat<int8_t>::SCALAR_UNIT_NEGATIVE)
							: numerator <= -denominator ? TemplateFormat<int8_t>::SCALAR_UNIT_NEGATIVE
							: numerator >= denominator ? TemplateFormat<int8_t>::SCALAR_UNIT
							: !FitsIn<int8_t>(denominator) ? TemplateFormat<int8_t>::GetScalar(numerator, denominator)
							: !FitsIn<int8_t>(numerator) ? TemplateFormat<int8_t>::GetScalar(numerator, denominator)
							: GetScalarS8(static_cast<int8_t>(numerator), static_cast<int8_t>(denominator));
#endif
					}

					inline int16_t GetScalarS16(const int16_t numerator, const int16_t denominator)
					{
#if defined(INTEGER_SIGNAL_DISABLE_ACCELERATION)
						return Constexpr::GetScalarS16(numerator, denominator);
#else
						int16_t scalar = 0;
						int32_t remainder = numerator;

						for (int16_t bit = TemplateFormat<int16_t>::SCALAR_UNIT >> 1; bit != 0; bit >>= 1)
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

					template<typename T>
					inline int16_t GetScalarS16(const T numerator, const T denominator)
					{
#if defined(INTEGER_SIGNAL_DISABLE_ACCELERATION)
						return Constexpr::GetScalarS16(numerator, denominator);
#else
						return denominator == 0 ? (numerator >= 0 ? TemplateFormat<int16_t>::SCALAR_UNIT : TemplateFormat<int16_t>::SCALAR_UNIT_NEGATIVE)
							: numerator <= -denominator ? TemplateFormat<int16_t>::SCALAR_UNIT_NEGATIVE
							: numerator >= denominator ? TemplateFormat<int16_t>::SCALAR_UNIT
							: !FitsIn<int16_t>(denominator) ? TemplateFormat<int16_t>::GetScalar(numerator, denominator)
							: !FitsIn<int16_t>(numerator) ? TemplateFormat<int16_t>::GetScalar(numerator, denominator)
							: GetScalarS16(static_cast<int16_t>(numerator), static_cast<int16_t>(denominator));
#endif
					}

					template<typename T>
					inline int32_t GetScalarS32(const T numerator, const T denominator)
					{
#if defined(INTEGER_SIGNAL_DISABLE_ACCELERATION)
						return Constexpr::GetScalarS32(numerator, denominator);
#else
						static constexpr uint8_t ScalarBits = IntegerSignal::GetBitShifts(TemplateFormat<int32_t>::SCALAR_UNIT);
						static constexpr uint8_t ScalarLoBits = sizeof(uint16_t) * 8;
						static constexpr uint8_t ScalarHiBits = ScalarBits - ScalarLoBits;

						if ((denominator <= 0 || denominator > INT16_MAX)
							|| (numerator <= -denominator || numerator >= denominator))
						{
							return Constexpr::GetScalarS32(numerator, denominator);
						}
						else
						{
							const bool isNegative = numerator < 0;
							const uint32_t absNumerator = isNegative ? static_cast<uint32_t>(-numerator) : static_cast<uint32_t>(numerator);
							const uint32_t absDenominator = static_cast<uint32_t>(denominator);
							const uint32_t scalarHi = (absNumerator << ScalarHiBits) / absDenominator;
							const uint32_t remainder = (absNumerator << ScalarHiBits) - (scalarHi * absDenominator);
							const uint32_t scalarLo = (remainder << ScalarLoBits) / absDenominator;
							const int32_t scalar = static_cast<int32_t>((scalarHi << ScalarLoBits) | scalarLo);

							return isNegative ? -scalar : scalar;
						}
#endif
					}
				}

				/// <summary>
				/// Policy-based selection of the appropriate GetScalar implementation for signed types.
				/// Chooses between runtime-optimized and constexpr implementations based on platform and configuration.
				/// AVR covers 8-bit MCUs basically, we use the fast runtime version for 8-bit and 16-bit scalars.
				/// For ARM, RISC-V and Extensa, 32-bit MCUs use only the 32 runtime-optimized scalar.
				/// </summary>
				namespace Policy
				{
					template<typename T>
					inline int8_t GetScalarS8(const T numerator, const T denominator)
					{
#if defined(__AVR__)
						return (denominator > 0 && numerator >= 0
							&& TypeTraits::TypeLimits::FitsIn<int8_t>(denominator)
							&& TypeTraits::TypeLimits::FitsIn<int8_t>(numerator))
							? Runtime::GetScalarS8(numerator, denominator)
							: Constexpr::GetScalarS8(numerator, denominator);
#else
						return Constexpr::GetScalarS8(numerator, denominator);
#endif
					}

					template<typename T>
					inline int16_t GetScalarS16(const T numerator, const T denominator)
					{
#if defined(__AVR__)
						return (denominator > 0 && numerator >= 0
							&& TypeTraits::TypeLimits::FitsIn<int16_t>(denominator)
							&& TypeTraits::TypeLimits::FitsIn<int16_t>(numerator))
							? Runtime::GetScalarS16(numerator, denominator)
							: Constexpr::GetScalarS16(numerator, denominator);
#else
						return Constexpr::GetScalarS16(numerator, denominator);
#endif
					}

					template<typename T>
					inline int32_t GetScalarS32(const T numerator, const T denominator)
					{
#if (INTPTR_MAX == INT32_MAX)
						return Runtime::GetScalarS32(numerator, denominator);
#else
						return Constexpr::GetScalarS32(numerator, denominator);
#endif
					}
				}

				template<typename T>
				static constexpr int8_t GetScalarS8Constexpr(const T numerator, const T denominator)
				{
					return TemplateFormat<int8_t>::GetScalar(numerator, denominator);
				}

				template<typename T>
				static constexpr int16_t GetScalarS16Constexpr(const T numerator, const T denominator)
				{
					return TemplateFormat<int16_t>::GetScalar(numerator, denominator);
				}

				template<typename T>
				static constexpr int32_t GetScalarS32Constexpr(const T numerator, const T denominator)
				{
					return TemplateFormat<int32_t>::GetScalar(numerator, denominator);
				}

				template<typename T>
				static inline int8_t GetScalarS8Runtime(const T numerator, const T denominator)
				{
					return Runtime::GetScalarS8(numerator, denominator);
				}

				template<typename T>
				static inline int16_t GetScalarS16Runtime(const T numerator, const T denominator)
				{
					return Runtime::GetScalarS16(numerator, denominator);
				}

				template<typename T>
				static inline int32_t GetScalarS32Runtime(const T numerator, const T denominator)
				{
					return Runtime::GetScalarS32(numerator, denominator);
				}

				template<typename T>
				static inline int8_t GetScalarS8(const T numerator, const T denominator)
				{
					return Policy::GetScalarS8(numerator, denominator);
				}

				template<typename T>
				static inline int16_t GetScalarS16(const T numerator, const T denominator)
				{
					return Policy::GetScalarS16(numerator, denominator);
				}

				template<typename T>
				static inline int32_t GetScalarS32(const T numerator, const T denominator)
				{
					return Policy::GetScalarS32(numerator, denominator);
				}
			}

			/// <summary>
			/// Explicit runtime-optimized versions of GetScalar for specific types and platforms.
			/// Q-format owns these entry points; higher-level fixed-point facades only forward to them.
			/// </summary>
			namespace Runtime
			{
				template<typename T>
				inline int8_t GetScalarS8(const T numerator, const T denominator)
				{
					return Implementation::Runtime::GetScalarS8(numerator, denominator);
				}

				template<typename T>
				inline int8_t GetScalarS8Explicit(const T numerator, const T denominator)
				{
					return Implementation::GetScalarS8Runtime(numerator, denominator);
				}

				template<typename T>
				inline int16_t GetScalarS16(const T numerator, const T denominator)
				{
					return Implementation::Runtime::GetScalarS16(numerator, denominator);
				}

				template<typename T>
				inline int16_t GetScalarS16Explicit(const T numerator, const T denominator)
				{
					return Implementation::GetScalarS16Runtime(numerator, denominator);
				}

				template<typename T>
				inline int32_t GetScalarS32(const T numerator, const T denominator)
				{
					return Implementation::Runtime::GetScalarS32(numerator, denominator);
				}

				template<typename T>
				inline int32_t GetScalarS32Explicit(const T numerator, const T denominator)
				{
					return Implementation::GetScalarS32Runtime(numerator, denominator);
				}
			}

			/// <summary>
			/// Explicit constexpr versions of GetScalar for compile-time evaluation.
			/// Q-format owns these entry points; higher-level fixed-point facades only forward to them.
			/// </summary>
			namespace Constexpr
			{
				template<typename T>
				static constexpr int8_t GetScalarS8(const T numerator, const T denominator)
				{
					return Implementation::Constexpr::GetScalarS8(numerator, denominator);
				}

				template<typename T>
				static constexpr int8_t GetScalarS8Explicit(const T numerator, const T denominator)
				{
					return Implementation::GetScalarS8Constexpr(numerator, denominator);
				}

				template<typename T>
				static constexpr int16_t GetScalarS16(const T numerator, const T denominator)
				{
					return Implementation::Constexpr::GetScalarS16(numerator, denominator);
				}

				template<typename T>
				static constexpr int16_t GetScalarS16Explicit(const T numerator, const T denominator)
				{
					return Implementation::GetScalarS16Constexpr(numerator, denominator);
				}

				template<typename T>
				static constexpr int32_t GetScalarS32(const T numerator, const T denominator)
				{
					return Implementation::Constexpr::GetScalarS32(numerator, denominator);
				}

				template<typename T>
				static constexpr int32_t GetScalarS32Explicit(const T numerator, const T denominator)
				{
					return Implementation::GetScalarS32Constexpr(numerator, denominator);
				}
			}

			template<typename T>
			inline int8_t GetScalarS8(const T numerator, const T denominator)
			{
				return Implementation::Policy::GetScalarS8(numerator, denominator);
			}

			template<typename T>
			inline int8_t GetScalarS8Explicit(const T numerator, const T denominator)
			{
				return Implementation::GetScalarS8(numerator, denominator);
			}

			template<typename T>
			inline int16_t GetScalarS16(const T numerator, const T denominator)
			{
				return Implementation::Policy::GetScalarS16(numerator, denominator);
			}

			template<typename T>
			inline int16_t GetScalarS16Explicit(const T numerator, const T denominator)
			{
				return Implementation::GetScalarS16(numerator, denominator);
			}

			template<typename T>
			inline int32_t GetScalarS32(const T numerator, const T denominator)
			{
				return Implementation::Policy::GetScalarS32(numerator, denominator);
			}

			template<typename T>
			inline int32_t GetScalarS32Explicit(const T numerator, const T denominator)
			{
				return Implementation::GetScalarS32(numerator, denominator);
			}
		}
	}
}

#endif