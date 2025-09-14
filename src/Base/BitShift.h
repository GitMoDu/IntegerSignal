#ifndef _INTEGER_SIGNAL_BIT_SHIFT_h
#define _INTEGER_SIGNAL_BIT_SHIFT_h

#include "TypeTraits.h"

namespace IntegerSignal
{
	/// <summary>
	/// BitShifts provides well-defined, constexpr-capable right-shift helpers
	/// for both signed and unsigned integer types.
	/// </summary>
	/// <remarks>
	/// - Unsigned values use the native logical right shift (fills with zeros).
	/// - Signed values emulate an arithmetic right shift (preserves sign for negatives),
	///   avoiding implementation-defined behavior for right-shifting negative values.
	/// - Both runtime shift (shift count as a parameter) and compile-time shift
	///   (shift count as a template parameter) variants are provided.
	/// - Uses the library's type traits (no std dependency) for dispatch.
	/// - Precondition: shift count must be less than the bit width of T (as per C++ rules).
	/// </remarks>
	namespace BitShifts
	{
		/// <summary>
		/// Internal helpers that dispatch to the appropriate implementation based on type signedness.
		/// </summary>
		namespace SignedShifts
		{
			/// <summary>
			/// Right shift with a runtime shift count for unsigned types. Logical shift.
			/// </summary>
			/// <typeparam name="T">Unsigned integer type.</typeparam>
			/// <param name="value">Value to shift.</param>
			/// <param name="shifts">Number of bit positions to shift (0..bitwidth(T)-1).</param>
			/// <returns>value >> shifts</returns>
			template<typename T>
			static constexpr T TemplateRightShift(const T value, const uint8_t shifts, TypeTraits::TypeDispatch::TrueType) // unsigned
			{
				return value >> shifts;
			}

			/// <summary>
			/// Right shift with a runtime shift count for signed types. Emulates arithmetic shift.
			/// For negative values, shifts the magnitude and reapplies the sign to preserve sign.
			/// </summary>
			/// <typeparam name="T">Signed integer type.</typeparam>
			/// <param name="value">Value to shift.</param>
			/// <param name="shifts">Number of bit positions to shift (0..bitwidth(T)-1).</param>
			/// <returns>Arithmetic right-shifted value.</returns>
			template<typename T>
			static constexpr T TemplateRightShift(const T value, const uint8_t shifts, TypeTraits::TypeDispatch::FalseType) // signed
			{
				return (value < 0) ? T(-(-value >> shifts)) : T(value >> shifts);
			}

			/// <summary>
			/// Right shift with a compile-time shift count (template parameter) for unsigned types. Logical shift.
			/// </summary>
			/// <typeparam name="T">Unsigned integer type.</typeparam>
			/// <typeparam name="shifts">Number of bit positions to shift (0..bitwidth(T)-1).</typeparam>
			/// <param name="value">Value to shift.</param>
			/// <returns>value >> shifts</returns>
			template<typename T, uint8_t shifts>
			static constexpr T TemplateRightShift(const T value, TypeTraits::TypeDispatch::TrueType) // unsigned
			{
				return value >> shifts;
			}

			/// <summary>
			/// Right shift with a compile-time shift count (template parameter) for signed types. Emulates arithmetic shift.
			/// For negative values, shifts the magnitude and reapplies the sign.
			/// </summary>
			/// <typeparam name="T">Signed integer type.</typeparam>
			/// <typeparam name="shifts">Number of bit positions to shift (0..bitwidth(T)-1).</typeparam>
			/// <param name="value">Value to shift.</param>
			/// <returns>Arithmetic right-shifted value.</returns>
			template<typename T, uint8_t shifts>
			static constexpr T TemplateRightShift(const T value, TypeTraits::TypeDispatch::FalseType) // signed
			{
				return (value < 0) ? T(-(-value >> shifts)) : T(value >> shifts);
			}
		}

		/// <summary>
		/// Performs a sign-preserving right shift.
		/// - Unsigned T: logical shift (zero-fill).
		/// - Signed T: arithmetic shift emulation (preserves sign for negatives).
		/// </summary>
		/// <typeparam name="T">Integral type.</typeparam>
		/// <param name="value">Value to shift.</param>
		/// <param name="shifts">Number of bit positions to shift (0..bitwidth(T)-1).</param>
		/// <returns>Right-shifted value with appropriate semantics for T.</returns>
		template<typename T>
		static constexpr T SignedRightShift(const T value, const uint8_t shifts)
		{
			return SignedShifts::TemplateRightShift(
				value, shifts,
				typename TypeTraits::TypeSign::IsUnsignedType<T>::type{});
		}

		/// <summary>
		/// Performs a sign-preserving right shift with a compile-time shift count.
		/// - Unsigned T: logical shift (zero-fill).
		/// - Signed T: arithmetic shift emulation (preserves sign for negatives).
		/// </summary>
		/// <typeparam name="T">Integral type.</typeparam>
		/// <typeparam name="shifts">Number of bit positions to shift (0..bitwidth(T)-1).</typeparam>
		/// <param name="value">Value to shift.</param>
		/// <returns>Right-shifted value with appropriate semantics for T.</returns>
		template<typename T, const uint8_t shifts>
		constexpr T SignedRightShift(const T value)
		{
			return SignedShifts::TemplateRightShift<T, shifts>(
				value,
				typename TypeTraits::TypeSign::IsUnsignedType<T>::type{});
		}
	}
	using namespace BitShifts;
}
#endif