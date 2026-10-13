#ifndef _INTEGER_SIGNAL_FIXED_POINT_FACTOR_SCALE_UNSIGNED_h
#define _INTEGER_SIGNAL_FIXED_POINT_FACTOR_SCALE_UNSIGNED_h

#include <stdint.h>
#include "../../Base/Operation.h"
#include "../../Base/TypeTraits.h"
#include "../../Base/BitSize.h"

namespace IntegerSignal
{
	namespace FixedPoint
	{
		namespace FactorScaleUnsigned
		{
			using namespace TypeTraits::TypeNext;
			using namespace TypeTraits::TypeLimits;
			using namespace TypeTraits::TypeSign;
			using namespace TypeTraits::TypeConditional;
			using namespace BitSize;

			/// <summary>
			/// Ratio-based scale factor for fixed-point math.
			/// Provides scaling and arithmetic for values using a configurable scale unit.
			/// 
			/// The scale unit is a power-of-2 value, allowing efficient scaling using bit-shifting.
			/// Only certain ranges are representable, and scaling is not fully general.
			/// 
			/// Performance: All scaling operations use fast bit-shifting instead of division or floating-point math.
			/// Suitable for real-time and embedded applications.
			/// </summary>
			/// <typeparam name="unsigned_factor_t">Underlying unsigned integer type for the scale factor.</typeparam>
			template<typename unsigned_factor_t>
			class TemplateFormat
			{
			public:
				/// <summary>
				/// Underlying unsigned integer type for the scale factor.
				/// </summary>
				using factor_t = unsigned_factor_t;

			public:
				/// <summary>
				/// Minimum allowed scale factor.
				/// </summary>
				static constexpr factor_t SCALE_MIN = 1;

				/// <summary>
				/// Maximum allowed scale factor.
				/// </summary>
				static constexpr factor_t SCALE_MAX = type_limits<factor_t>::Max();

			private:
				/// <summary>
				/// Bits required to represent the maximum value of type factor_t.
				/// </summary>
				static constexpr uint8_t FACTOR_BITS = bit_count<SCALE_MAX>::value;

				/// <summary>
				/// Number of bits to shift for scaling operations.
				/// </summary>
				static constexpr uint8_t BIT_SHIFTS = FACTOR_BITS / 2;

			public:
				/// <summary>
				/// Constant scale unit for normalization.
				/// </summary>
				static constexpr factor_t SCALE_UNIT = (SCALE_MAX >> BIT_SHIFTS) + 1;

				/// <summary>
				/// Number of distinct scale steps in the range.
				/// </summary>
				static constexpr auto SCALE_STEPS = SCALE_MAX / SCALE_UNIT;

			public:
				/// <summary>
				/// Calculates a scaling factor based on the given numerator and denominator values.
				/// </summary>
				/// <typeparam name="T">The type of the numerator and denominator values.</typeparam>
				/// <param name="numerator">The numerator value used in the scaling calculation.</param>
				/// <param name="denominator">The denominator value used in the scaling calculation.</param>
				/// <returns>Returns a factor_t value representing the computed scaling factor.
				/// If the numerator is negative, returns SCALE_MIN.
				/// If the denominator is zero or negative, returns SCALE_UNIT.
				/// The result is saturated at SCALE_MAX.</returns>
				template<typename T>
				static constexpr factor_t GetFactor(const T numerator, const T denominator)
				{
					// Accept signed or unsigned integral types. Tag-dispatch handles signed vs unsigned behaviour.
					static_assert((is_signed<T>::value || is_unsigned<T>::value), "GetFactor requires an integer type.");
					return TemplateGetFactor(numerator, denominator, typename IsUnsignedType<T>::type());
				}

				/// <summary>
				/// Scales a value by a given scale factor.
				/// </summary>
				template<typename T>
				static constexpr T Scale(const factor_t factorValue, const T value)
				{
					return TemplateScale(factorValue, value, typename IsUnsignedType<T>::type());
				}

				/// <summary>
				/// Calculates a scaling factor based on the given numerator and denominator, using unsigned integer arithmetic.
				/// </summary>
				/// <typeparam name="T">The unsigned integer type of the numerator and denominator.</typeparam>
				/// <param name="numerator">The numerator value for the scaling calculation.</param>
				/// <param name="denominator">The denominator value for the scaling calculation.</param>
				/// <param name="">Unused parameter of type TypeTraits::TypeDispatch::TrueType, used for tag dispatching.</param>
				/// <returns>The computed scaling factor as a value of type factor_t.
				/// Returns SCALE_UNIT if the denominator is zero and saturates at SCALE_MAX.</returns>
				template<typename T>
				static constexpr factor_t TemplateGetFactor(const T numerator, const T denominator, TypeTraits::TypeDispatch::TrueType)
				{
					using larger_t = typename larger_type<T, factor_t>::type;
					using intermediate_t = typename next_uint_type<larger_t>::type;

					return denominator == 0 ? SCALE_UNIT :
						static_cast<factor_t>(LimitValue<intermediate_t>(
							(static_cast<intermediate_t>(numerator) << GetBitShifts(SCALE_UNIT)) / denominator,
							0, static_cast<intermediate_t>(SCALE_MAX)));
				}

				/// <summary>
				/// Calculates a scaling factor based on the numerator and denominator values, using specific rules for edge cases.
				/// </summary>
				/// <typeparam name="T">The type of the numerator and denominator.</typeparam>
				/// <param name="numerator">The value to be scaled.</param>
				/// <param name="denominator">The value by which to scale the numerator.</param>
				/// <param name="">Unused parameter of type TypeTraits::TypeDispatch::FalseType, used for overload resolution.</param>
					/// <returns>A factor_t value representing the computed scaling factor.
					/// Returns SCALE_MIN if the numerator is negative,
					/// SCALE_UNIT if the denominator is zero or negative,
					/// and saturates at SCALE_MAX.</returns>
				template<typename T>
				static constexpr factor_t TemplateGetFactor(const T numerator, const T denominator, TypeTraits::TypeDispatch::FalseType)
				{
					using larger_t = typename larger_type<T, factor_t>::type;
					using intermediate_t = typename next_int_type<larger_t>::type;

					return numerator < 0 ? SCALE_MIN : denominator <= 0 ? SCALE_UNIT :
						static_cast<factor_t>(LimitValue<intermediate_t>(
							(static_cast<intermediate_t>(numerator) << GetBitShifts(SCALE_UNIT)) / denominator,
							0, static_cast<intermediate_t>(SCALE_MAX)));
				}

				template<typename T>
				static constexpr T TemplateScale(const factor_t factorValue, const T value, TypeTraits::TypeDispatch::TrueType)
				{
					using larger_t = typename larger_type<T, factor_t>::type;
					using intermediate_t = typename next_uint_type<larger_t>::type;

					return static_cast<T>(LimitValue<intermediate_t>(
						SignedRightShift(static_cast<intermediate_t>(value) * static_cast<intermediate_t>(factorValue), BIT_SHIFTS),
						static_cast<intermediate_t>(type_limits<T>::Min()),
						static_cast<intermediate_t>(type_limits<T>::Max())));
				}

				template<typename T>
				static constexpr T TemplateScale(const factor_t factorValue, const T value, TypeTraits::TypeDispatch::FalseType)
				{
					using larger_t = typename larger_type<T, factor_t>::type;
					using intermediate_t = typename next_int_type<larger_t>::type;

					return static_cast<T>(LimitValue<intermediate_t>(
						SignedRightShift(static_cast<intermediate_t>(value) * static_cast<intermediate_t>(factorValue), BIT_SHIFTS),
						type_limits<T>::Min(), type_limits<T>::Max()));
				}

			};

			namespace Implementation
			{
				/// <summary>
				/// Reference implementation of GetFactor and Scale for unsigned factor formats, constexpr-capable but not necessarily optimized for all platforms.
				/// </summary>
				namespace Constexpr
				{
					template<typename factor_t, typename T>
					static constexpr factor_t GetFactor(const T numerator, const T denominator)
					{
						return TemplateFormat<factor_t>::TemplateGetFactor(numerator, denominator, typename IsUnsignedType<T>::type());
					}

					template<typename factor_t, typename T>
					static constexpr T Scale(const factor_t factorValue, const T value)
					{
						return TemplateFormat<factor_t>::TemplateScale(factorValue, value, typename IsUnsignedType<T>::type());
					}
				}

				/// <summary>
				/// Runtime-only implementation of GetFactor for unsigned factor formats, optimized for platforms where the specialized path is faster than the constexpr reference implementation.
				/// </summary>
				namespace Runtime
				{
					inline uint8_t GetFactor8(const uint8_t numerator, const uint8_t denominator)
					{
						if (denominator == 0)
						{
							return TemplateFormat<uint8_t>::SCALE_UNIT;
						}

						const uint16_t scaledNumerator = static_cast<uint16_t>(numerator) * TemplateFormat<uint8_t>::SCALE_UNIT;
						uint32_t estimatedFactor = (static_cast<uint32_t>(scaledNumerator) *
							static_cast<uint32_t>(TemplateFormat<uint16_t>::SCALE_MAX / denominator)) >> 16;

						if ((estimatedFactor + 1) * denominator <= scaledNumerator)
							estimatedFactor++;

						return static_cast<uint8_t>(LimitValue<uint16_t>(
							static_cast<uint16_t>(estimatedFactor), 0, TemplateFormat<uint8_t>::SCALE_MAX));
					}

					template<typename T>
					inline uint8_t GetFactor8(const T numerator, const T denominator)
					{
#if defined(INTEGER_SIGNAL_DISABLE_ACCELERATION)
						return Constexpr::GetFactor<uint8_t>(numerator, denominator);
#else
						return numerator < 0 ? TemplateFormat<uint8_t>::SCALE_MIN
							: denominator <= 0 ? TemplateFormat<uint8_t>::SCALE_UNIT
							: !FitsIn<uint8_t>(numerator) ? Constexpr::GetFactor<uint8_t>(numerator, denominator)
							: !FitsIn<uint8_t>(denominator) ? Constexpr::GetFactor<uint8_t>(numerator, denominator)
							: GetFactor8(static_cast<uint8_t>(numerator), static_cast<uint8_t>(denominator));
#endif
					}

					template<typename factor_t, typename T>
					inline factor_t GetFactor(const T numerator, const T denominator)
					{
						return Constexpr::GetFactor<factor_t>(numerator, denominator);
					}

					template<typename T>
					inline uint32_t GetFactor32(const T numerator, const T denominator)
					{
#if defined(INTEGER_SIGNAL_DISABLE_ACCELERATION)
						return Constexpr::GetFactor<uint32_t>(numerator, denominator);
#else
						if (numerator < 0)
							return TemplateFormat<uint32_t>::SCALE_MIN;

						if (denominator <= 0)
							return TemplateFormat<uint32_t>::SCALE_UNIT;

						if (!FitsIn<uint32_t>(numerator) || !FitsIn<uint32_t>(denominator))
							return Constexpr::GetFactor<uint32_t>(numerator, denominator);

						const uint32_t numerator32 = static_cast<uint32_t>(numerator);
						const uint32_t denominator32 = static_cast<uint32_t>(denominator);

						if (denominator32 == 0u)
							return TemplateFormat<uint32_t>::SCALE_UNIT;

						const uint64_t factorHi = static_cast<uint64_t>(numerator32) / denominator32;
						if (factorHi > UINT16_MAX)
							return TemplateFormat<uint32_t>::SCALE_MAX;

						const uint32_t remainder = numerator32 - static_cast<uint32_t>(static_cast<uint64_t>(factorHi) * denominator32);
						const uint64_t factorLo = (static_cast<uint64_t>(remainder) << 16) / denominator32;

						return static_cast<uint32_t>((factorHi << 16) | factorLo);
#endif
					}

					template<typename factor_t, typename T>
					static constexpr T Scale(const factor_t factorValue, const T value)
					{
						return Constexpr::Scale<factor_t>(factorValue, value);
					}

					template<typename T>
					inline uint16_t GetFactor16(const T numerator, const T denominator)
					{
#if defined(INTEGER_SIGNAL_DISABLE_ACCELERATION)
						return Constexpr::GetFactor<uint16_t>(numerator, denominator);
#else
						if (numerator < 0)
							return TemplateFormat<uint16_t>::SCALE_MIN;

						if (denominator <= 0)
							return TemplateFormat<uint16_t>::SCALE_UNIT;

						if (!FitsIn<uint16_t>(numerator) || !FitsIn<uint16_t>(denominator))
							return Constexpr::GetFactor<uint16_t>(numerator, denominator);

						const uint16_t numerator16 = static_cast<uint16_t>(numerator);
						const uint16_t denominator16 = static_cast<uint16_t>(denominator);

						if (denominator16 == 0u)
							return TemplateFormat<uint16_t>::SCALE_UNIT;

						uint16_t integerFactor = 0;
						uint32_t remainder = 0;

						for (uint16_t bit = 1u << 15; bit != 0; bit >>= 1)
						{
							remainder = (remainder << 1) | ((numerator16 & bit) != 0 ? 1u : 0u);
							if (remainder >= denominator16)
							{
								remainder -= denominator16;
								integerFactor = static_cast<uint16_t>(integerFactor | bit);
							}
						}

						if (integerFactor > (TemplateFormat<uint16_t>::SCALE_MAX / TemplateFormat<uint16_t>::SCALE_UNIT))
							return TemplateFormat<uint16_t>::SCALE_MAX;

						uint16_t factor = static_cast<uint16_t>(integerFactor * TemplateFormat<uint16_t>::SCALE_UNIT);

						for (uint16_t bit = TemplateFormat<uint16_t>::SCALE_UNIT >> 1; bit != 0; bit >>= 1)
						{
							remainder <<= 1;
							if (remainder >= denominator16)
							{
								remainder -= denominator16;
								factor = static_cast<uint16_t>(factor | bit);
							}
						}

						return factor;
#endif
					}

					
				}

				/// <summary>
				/// Selects between constexpr and runtime implementations based on platform and evaluation tag.
				/// </summary>
				namespace Policy
				{
					template<typename T>
					inline uint8_t GetFactor8(const T numerator, const T denominator, TypeTraits::TypeEvaluation::Dispatch::RuntimeTag)
					{
#if defined(__AVR__) || (INTPTR_MAX == INT32_MAX)
						return Runtime::GetFactor8(numerator, denominator);
#else
						return Constexpr::GetFactor<uint8_t>(numerator, denominator);
#endif
					}

					template<typename T>
					constexpr uint8_t GetFactor8(const T numerator, const T denominator, TypeTraits::TypeEvaluation::Dispatch::CompileTimeTag)
					{
						return Constexpr::GetFactor<uint8_t>(numerator, denominator);
					}

					template<typename factor_t, typename T>
					inline factor_t GetFactor(const T numerator, const T denominator, TypeTraits::TypeEvaluation::Dispatch::RuntimeTag)
					{
						return Constexpr::GetFactor<factor_t>(numerator, denominator);
					}

					template<typename factor_t, typename T>
					inline factor_t GetFactor(const T numerator, const T denominator, TypeTraits::TypeEvaluation::Dispatch::CompileTimeTag)
					{
						return Constexpr::GetFactor<factor_t>(numerator, denominator);
					}

					template<typename T>
					inline uint16_t GetFactor16(const T numerator, const T denominator, TypeTraits::TypeEvaluation::Dispatch::RuntimeTag)
					{
#if defined(__AVR__)
						return Runtime::GetFactor16(numerator, denominator);
#else
						return Constexpr::GetFactor<uint16_t>(numerator, denominator);
#endif
					}

					template<typename T>
					constexpr uint16_t GetFactor16(const T numerator, const T denominator, TypeTraits::TypeEvaluation::Dispatch::CompileTimeTag)
					{
						return Constexpr::GetFactor<uint16_t>(numerator, denominator);
					}

					template<typename T>
					inline uint32_t GetFactor32(const T numerator, const T denominator, TypeTraits::TypeEvaluation::Dispatch::RuntimeTag)
					{
#if (INTPTR_MAX == INT32_MAX)
						return Runtime::GetFactor32(numerator, denominator);
#else
						return Constexpr::GetFactor<uint32_t>(numerator, denominator);
#endif
					}

					template<typename T>
					constexpr uint32_t GetFactor32(const T numerator, const T denominator, TypeTraits::TypeEvaluation::Dispatch::CompileTimeTag)
					{
						return Constexpr::GetFactor<uint32_t>(numerator, denominator);
					}

					template<typename factor_t, typename T>
					static constexpr T Scale(const factor_t factorValue, const T value)
					{
						return Constexpr::Scale(factor_t(factorValue), value);
					}
				}
			}
		}
	}
}

#endif