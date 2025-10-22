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
				static constexpr factor_t SCALE_MAX = type_limits<factor_t>::MAX();

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
				/// If the numerator is greater than the denominator, returns SCALE_UNIT. 
				/// Otherwise, returns factor.</returns>
				template<typename T>
				static constexpr factor_t GetFactor(const T numerator, const T denominator)
				{
					// Accept signed or unsigned integral types. Tag-dispatch handles signed vs unsigned behaviour.
					static_assert((is_signed<T>::value || is_unsigned<T>::value), "GetFactor requires an integer type.");
					return TemplateGetFactor<T>(numerator, denominator, typename IsUnsignedType<T>::type());
				}

				/// <summary>
				/// Scales a value by a given scale factor.
				/// </summary>
				template<typename T>
				static constexpr T Scale(const factor_t factorValue, const T value)
				{
					return TemplateScale<T>(factorValue, value, typename IsUnsignedType<T>::type());
				}

			private:
				/// <summary>
				/// Calculates a scaling factor based on the given numerator and denominator, using unsigned integer arithmetic.
				/// </summary>
				/// <typeparam name="T">The unsigned integer type of the numerator and denominator.</typeparam>
				/// <param name="numerator">The numerator value for the scaling calculation.</param>
				/// <param name="denominator">The denominator value for the scaling calculation.</param>
				/// <param name="">Unused parameter of type TypeTraits::TypeDispatch::TrueType, used for tag dispatching.</param>
				/// <returns>The computed scaling factor as a value of type factor_t. 
				/// Returns SCALE_UNIT if the denominator is zero or the numerator is greater than the denominator; 
				/// otherwise, returns scale factor.</returns>
				template<typename T>
				static constexpr factor_t TemplateGetFactor(const T numerator, const T denominator, TypeTraits::TypeDispatch::TrueType)
				{
					using intermediate_t = typename next_uint_type<T>::type;

					return denominator == 0 ? SCALE_UNIT :
						static_cast<factor_t>((static_cast<intermediate_t>(numerator) << GetBitShifts(SCALE_UNIT)) / denominator);
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
				/// SCALE_UNIT if the numerator is greater than the denominator; 
				/// otherwise returns scale factor</returns>
				template<typename T>
				static constexpr factor_t TemplateGetFactor(const T numerator, const T denominator, TypeTraits::TypeDispatch::FalseType)
				{
					using intermediate_t = typename next_int_type<T>::type;

					return numerator < 0 ? SCALE_MIN : denominator <= 0 ? SCALE_UNIT :
						static_cast<factor_t>((static_cast<intermediate_t>(numerator) << GetBitShifts(SCALE_UNIT)) / denominator);
				}

				template<typename T>
				static constexpr T TemplateScale(const factor_t factorValue, const T value, TypeTraits::TypeDispatch::TrueType)
				{
					using larger_t = typename larger_type<T, factor_t>::type;
					using intermediate_t = typename next_uint_type<larger_t>::type;

					return static_cast<T>(LimitValue<intermediate_t>(
						SignedRightShift(static_cast<intermediate_t>(value) * static_cast<intermediate_t>(factorValue), BIT_SHIFTS),
						static_cast<intermediate_t>(type_limits<T>::MIN()),
						static_cast<intermediate_t>(type_limits<T>::MAX())));
				}

				template<typename T>
				static constexpr T TemplateScale(const factor_t factorValue, const T value, TypeTraits::TypeDispatch::FalseType)
				{
					using larger_t = typename larger_type<T, factor_t>::type;
					using intermediate_t = typename next_int_type<larger_t>::type;

					return static_cast<T>(LimitValue<intermediate_t>(
						SignedRightShift(static_cast<intermediate_t>(value) * static_cast<intermediate_t>(factorValue), BIT_SHIFTS),
						type_limits<T>::MIN(), type_limits<T>::MAX()));
				}
			};
		}
	}
}

#endif