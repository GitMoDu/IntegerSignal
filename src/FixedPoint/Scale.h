#ifndef _INTEGER_SIGNAL_FIXED_POINT_SCALE_h
#define _INTEGER_SIGNAL_FIXED_POINT_SCALE_h

#include <stdint.h>
#include "Base/Operation.h"
#include "Base/TypeTraits.h"
#include "Base/BitSize.h"


namespace IntegerSignal
{
	namespace FixedPoint
	{
		namespace FactorScale
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
			class TemplateFactorScale
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
				static constexpr factor_t GetFactor(const T numerator, const T denominator, TypeTraits::TypeDispatch::TrueType)
				{
					using intermediate_t = typename next_uint_type<T>::type;

					return denominator == 0 ? SCALE_UNIT : numerator > denominator ? SCALE_UNIT :
						factor_t((static_cast<intermediate_t>(numerator) * SCALE_UNIT) / denominator);
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
				static constexpr factor_t GetFactor(const T numerator, const T denominator, TypeTraits::TypeDispatch::FalseType)
				{
					using intermediate_t = typename next_int_type<T>::type;

					return numerator < 0 ? SCALE_MIN : denominator <= 0 ? SCALE_UNIT : numerator > denominator ? SCALE_UNIT :
						factor_t((static_cast<intermediate_t>(numerator) * SCALE_UNIT) / denominator);
				}

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
					return GetFactor<T>(numerator, denominator, typename IsUnsignedType<T>::type());
				}

				/// <summary>
				/// Scales a value by a given scale factor.
				/// </summary>
				template<typename T>
				static constexpr T Scale(const factor_t factorValue, const T value)
				{
					using larger_t = typename larger_type<T, factor_t>::type;
					using intermediate_t = typename next_int_type<larger_t>::type;

					return static_cast<T>(LimitValue<intermediate_t>(
						SignedRightShift(static_cast<intermediate_t>(value) * factorValue, BIT_SHIFTS),
						type_limits<T>::MIN(), type_limits<T>::MAX()));
				}
			};
		}

		namespace Scale
		{
			/// <summary>
			/// 8-bit unsigned scale factor for fixed-point scaling operations.
			/// Provides scaling with 4-bit precision (16 distinct steps).
			/// 
			/// Scale range: 1/16x (0.0625) to 15x
			/// Unit scale (1.0x): 16
			/// </summary>
			struct Scale8 final : FactorScale::TemplateFactorScale<uint8_t>
			{
				using Base = FactorScale::TemplateFactorScale<uint8_t>;

				static constexpr uint8_t SCALE_15X = UINT8_MAX;
				static constexpr uint8_t SCALE_1X = Base::SCALE_UNIT;
				static constexpr uint8_t SCALE_1_16X = 1;
			};

			/// <summary>
			/// 16-bit unsigned scale factor for fixed-point scaling operations.
			/// Provides scaling with 8-bit precision (256 distinct steps).
			/// 
			/// Scale range: 1/256x (0.00390625) to 255x
			/// Unit scale (1.0x): 256
			/// </summary>
			struct Scale16 final : FactorScale::TemplateFactorScale<uint16_t>
			{
				using Base = FactorScale::TemplateFactorScale<uint16_t>;

				static constexpr uint16_t SCALE_255X = UINT16_MAX;
				static constexpr uint16_t SCALE_1X = Base::SCALE_UNIT;
				static constexpr uint8_t SCALE_1_256X = 1;
			};

			/// <summary>
			/// 32-bit unsigned scale factor for fixed-point scaling operations.
			/// Provides scaling with 16-bit precision (65536 distinct steps).
			/// 
			/// Scale range: 1/65536x (0.0000152587890625) to 65535x
			/// Unit scale (1.0x): 65536
			/// </summary>
			struct Scale32 final : FactorScale::TemplateFactorScale<uint32_t>
			{
				using Base = FactorScale::TemplateFactorScale<uint32_t>;

				static constexpr uint32_t SCALE_65535X = UINT16_MAX;
				static constexpr uint32_t SCALE_1X = Base::SCALE_UNIT;
				static constexpr uint8_t SCALE_1_65536X = 1;
			};
		}
	}
}
#endif