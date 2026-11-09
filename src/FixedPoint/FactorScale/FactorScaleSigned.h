#ifndef _INTEGER_SIGNAL_FIXED_POINT_FACTOR_SCALE_SIGNED_h
#define _INTEGER_SIGNAL_FIXED_POINT_FACTOR_SCALE_SIGNED_h

#include <stdint.h>
#include "../../Base/Operation.h"
#include "../../Base/TypeTraits.h"
#include "../../Base/BitSize.h"
#include "../../Base/BitShift.h"

namespace IntegerSignal
{
	namespace FixedPoint
	{
		namespace FactorScaleSigned
		{
			using namespace TypeTraits::TypeNext;
			using namespace TypeTraits::TypeLimits;
			using namespace TypeTraits::TypeSign;
			using namespace TypeTraits::TypeConditional;
			using namespace BitSize;
			using namespace BitShifts;

			template<typename signed_factor_t>
			class TemplateFormat
			{
			public:
				using factor_t = signed_factor_t;
				static constexpr factor_t SCALE_MAX = type_limits<factor_t>::Max();
				static constexpr factor_t SCALE_MIN = type_limits<factor_t>::Min();

			private:
				static constexpr uint8_t FACTOR_BITS = bit_count<SCALE_MAX>::value + 1;
				static constexpr uint8_t BIT_SHIFTS = FACTOR_BITS / 2;

			public:
				static constexpr factor_t SCALE_UNIT = static_cast<factor_t>(1) << BIT_SHIFTS;
				static constexpr factor_t SCALE_1X = SCALE_UNIT;
				static constexpr factor_t SCALE_NEGATIVE_1X = -SCALE_UNIT;

				template<typename T>
				static constexpr factor_t GetFactor(const T numerator, const T denominator)
				{
					static_assert((is_signed<T>::value || is_unsigned<T>::value), "GetFactor requires an integer type.");
					static_assert(sizeof(T) <= sizeof(uint32_t), "Signed FactorScale GetFactor supports inputs up to 32 bits.");
					using larger_t = typename larger_type<T, factor_t>::type;
					using intermediate_t = typename next_int_type<larger_t>::type;

					return denominator == 0 ? SCALE_UNIT :
						static_cast<factor_t>(LimitValue<intermediate_t>(
							(static_cast<intermediate_t>(numerator) * static_cast<intermediate_t>(SCALE_UNIT)) /
								static_cast<intermediate_t>(denominator),
							static_cast<intermediate_t>(SCALE_MIN), static_cast<intermediate_t>(SCALE_MAX)));
				}

				template<typename T>
				static constexpr T Scale(const factor_t factorValue, const T value)
				{
					static_assert(sizeof(T) <= sizeof(uint32_t), "Signed FactorScale Scale supports values up to 32 bits.");
					using larger_t = typename larger_type<T, factor_t>::type;
					using intermediate_t = typename next_int_type<larger_t>::type;
					return static_cast<T>(LimitValue<intermediate_t>(
						SignedRightShift(static_cast<intermediate_t>(factorValue) * static_cast<intermediate_t>(value), BIT_SHIFTS),
						static_cast<intermediate_t>(type_limits<T>::Min()),
						static_cast<intermediate_t>(type_limits<T>::Max())));
				}
			};
		}
	}
}

#endif
