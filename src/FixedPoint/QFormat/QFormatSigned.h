#ifndef _INTEGER_SIGNAL_Q_FORMAT_SIGNED_h
#define _INTEGER_SIGNAL_Q_FORMAT_SIGNED_h

#include <stdint.h>
#include "Base/Operation.h"
#include "Base/TypeTraits.h"
#include "Base/BitSize.h"


namespace IntegerSignal
{
	namespace QFormat
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
		class TemplateSignedQFormat
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
			static constexpr scalar_t SCALAR_UNIT = SignedRightShift(type_limits<scalar_t>::MAX(), 1);

			/// <summary>
			/// Maximum negative scalar value for unit in the Q-format.
			/// </summary>
			static constexpr scalar_t SCALAR_UNIT_NEGATIVE = -SCALAR_UNIT;

		private:
			/// <summary>
			/// Number of bits to shift for scaling operations.
			/// </summary>
			static constexpr uint8_t BIT_SHIFTS = bit_count<SCALAR_UNIT * 2>::value - 1;

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
			/// </summary>
			template<typename ArgT>
			static constexpr scalar_t GetScalar(const ArgT numerator, const ArgT denominator)
			{
				using intermediate_t = typename next_int_type<ArgT>::type;

				return (denominator == 0) ? (numerator >= 0 ? SCALAR_UNIT : SCALAR_UNIT_NEGATIVE)
					//: LimitValue<intermediate_t, SCALAR_UNIT_NEGATIVE, SCALAR_UNIT>((static_cast<intermediate_t>(numerator) * SCALAR_UNIT) / denominator);
					: LimitValue<intermediate_t, SCALAR_UNIT_NEGATIVE, SCALAR_UNIT>((static_cast<intermediate_t>(numerator) << BIT_SHIFTS) / denominator);
			}

			/// <summary>
			/// Fractions a value by a given Q-format scalar.
			/// </summary>
			template<typename T>
			static constexpr T Fraction(const scalar_t scalarValue, const T value)
			{
				using larger_t = typename larger_type<T, scalar_t>::type;
				using intermediate_t = typename next_int_type<larger_t>::type;
				return static_cast<T>(SignedRightShift(static_cast<intermediate_t>(value) * scalarValue, BIT_SHIFTS));
			}

			/// <summary>
			/// Interpolates between two values using the Q-format scalar.
			/// </summary>
			template<typename T>
			static constexpr T Interpolate(const scalar_t scalarValue, const T from, const T to)
			{
				using larger_t = typename larger_type<T, scalar_t>::type; // Larger of value type and unsigned scalar type.
				using intermediate_t = typename next_int_type<larger_t>::type; // Next larger signed type for intermediate calculations.

				return static_cast<T>(SignedRightShift((static_cast<intermediate_t>(to) * scalarValue) +
					(static_cast<intermediate_t>(from) * (SCALAR_UNIT - scalarValue)), BIT_SHIFTS));
			}
		};
	}
}

#endif