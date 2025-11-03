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
			static_assert((SCALAR_UNIT & (SCALAR_UNIT - 1)) == 0,
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
				using intermediate_t = typename next_uint_type<T>::type; // Next larger unsigned type for intermediate calculations.

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
	}
}
#endif