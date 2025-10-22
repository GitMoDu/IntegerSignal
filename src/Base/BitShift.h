#ifndef _INTEGER_SIGNAL_BIT_SHIFT_h
#define _INTEGER_SIGNAL_BIT_SHIFT_h

#include "TypeTraits.h"

namespace IntegerSignal
{
	/// <summary>
	/// BitShifts provides well-defined, constexpr-capable shift helpers
	/// for both signed and unsigned integer types.
	/// </summary>
	/// <remarks>
	/// - Unsigned values use the native logical shift (fills with zeros).
	/// - Signed values emulate arithmetic right shift (preserves sign for negatives)
	///   and perform left-shift work in the unsigned domain to avoid signed UB.
	/// - Both runtime shift (shift count as a parameter) and compile-time shift
	///   (shift count as a template parameter) variants are provided.
	/// - Uses TypeTraits for signed/unsigned dispatch.
	/// - Precondition: shift count must be less than the bit width of T.
	/// </remarks>
	namespace BitShifts
	{
		/// <summary>
		/// Internal helpers that dispatch to the appropriate implementation based on type signedness.
		/// </summary>
		namespace SignedShifts
		{
			/// <summary>
			/// Logical right shift (unsigned types) with a runtime shift count.
			/// </summary>
			/// <typeparam name="T">Unsigned integer type.</typeparam>
			/// <param name="value">Value to shift.</param>
			/// <param name="shifts">Number of bit positions to shift (0..bitwidth(T)-1).</param>
			/// <returns>Logical right-shifted value (zero-filled).</returns>
			template<typename T>
			static constexpr T TemplateRightShift(const T value, const uint8_t shifts, TypeTraits::TypeDispatch::TrueType) // unsigned
			{
				return value >> shifts;
			}

			/// <summary>
			/// Arithmetic (sign-preserving) right shift emulation for signed types with a runtime shift count.
			/// </summary>
			/// <typeparam name="T">Signed integer type.</typeparam>
			/// <param name="value">Value to shift.</param>
			/// <param name="shifts">Number of bit positions to shift (0..bitwidth(T)-1).</param>
			/// <returns>Arithmetic right-shifted value (sign-preserving).</returns>
			template<typename T>
			static constexpr T TemplateRightShift(const T value, const uint8_t shifts, TypeTraits::TypeDispatch::FalseType) // signed
			{
				return (value < 0) ? T(-(-value >> shifts)) : T(value >> shifts);
			}

			/// <summary>
			/// Logical right shift (unsigned types) with a compile-time shift count.
			/// </summary>
			/// <typeparam name="T">Unsigned integer type.</typeparam>
			/// <typeparam name="shifts">Compile-time shift count (0..bitwidth(T)-1).</typeparam>
			/// <param name="value">Value to shift.</param>
			/// <returns>Logical right-shifted value (zero-filled).</returns>
			template<typename T, uint8_t shifts>
			static constexpr T TemplateRightShift(const T value, TypeTraits::TypeDispatch::TrueType) // unsigned
			{
				return value >> shifts;
			}

			/// <summary>
			/// Arithmetic (sign-preserving) right shift emulation for signed types with a compile-time shift count.
			/// </summary>
			/// <typeparam name="T">Signed integer type.</typeparam>
			/// <typeparam name="shifts">Compile-time shift count (0..bitwidth(T)-1).</typeparam>
			/// <param name="value">Value to shift.</param>
			/// <returns>Arithmetic right-shifted value (sign-preserving).</returns>
			template<typename T, uint8_t shifts>
			static constexpr T TemplateRightShift(const T value, TypeTraits::TypeDispatch::FalseType) // signed
			{
				return (value < 0) ? T(-(-value >> shifts)) : T(value >> shifts);
			}

			/// <summary>
			/// Logical left shift (unsigned types) with a runtime shift count.
			/// </summary>
			/// <typeparam name="T">Unsigned integer type.</typeparam>
			/// <param name="value">Value to shift.</param>
			/// <param name="shifts">Number of bit positions to shift (0..bitwidth(T)-1).</param>
			/// <returns>Logical left-shifted value (zero-filled on the right).</returns>
			template<typename T>
			static constexpr T TemplateLeftShift(const T value, const uint8_t shifts, TypeTraits::TypeDispatch::TrueType) // unsigned
			{
				return value << shifts;
			}

			/// <summary>
			/// Left-shift helper for negative signed inputs. All intermediate arithmetic is performed
			/// in the unsigned domain to avoid signed negation/left-shift UB.
			/// </summary>
			/// <typeparam name="T">Signed integer type.</typeparam>
			/// <param name="value">Negative input value to shift.</param>
			/// <param name="shifts">Number of bit positions to shift.</param>
			/// <returns>Signed result equivalent to -( |value| << shifts ) modulo 2^N.</returns>
			template<typename T>
			static constexpr T TemplateLeftShiftNegative(const T value, const uint8_t shifts) // signed negative case
			{
				using U = typename TypeTraits::TypeSign::make_unsigned<T>::type;

				return static_cast<T>(
					static_cast<U>(0) -
					(static_cast<U>(static_cast<U>(0) - static_cast<U>(value))
						<< shifts));
			}

			/// <summary>
			/// Signed left shift with a runtime shift count.
			/// Positive inputs are shifted in the unsigned domain then cast back.
			/// Negative inputs are handled by TemplateLeftShiftNegative to avoid UB.
			/// </summary>
			/// <typeparam name="T">Signed integer type.</typeparam>
			/// <param name="value">Value to shift.</param>
			/// <param name="shifts">Number of bit positions to shift (0..bitwidth(T)-1).</param>
			/// <returns>Left-shifted signed value (magnitude shifted in unsigned domain).</returns>
			template<typename T>
			static constexpr T TemplateLeftShift(const T value, const uint8_t shifts, TypeTraits::TypeDispatch::FalseType) // signed
			{
				using U = typename TypeTraits::TypeSign::make_unsigned<T>::type;

				return (value >= 0)
					? static_cast<T>(static_cast<U>(value) << shifts)   // unsigned logical shift then cast back
					: TemplateLeftShiftNegative<T>(value, shifts);      // negative path handled above
			}

			/// <summary>
			/// Logical left shift (unsigned types) with a compile-time shift count.
			/// </summary>
			/// <typeparam name="T">Unsigned integer type.</typeparam>
			/// <typeparam name="shifts">Compile-time shift count.</typeparam>
			/// <param name="value">Value to shift.</param>
			/// <returns>Logical left-shifted value (zero-filled on the right).</returns>
			template<typename T, uint8_t shifts>
			static constexpr T TemplateLeftShift(const T value, TypeTraits::TypeDispatch::TrueType) // unsigned, compile-time
			{
				static_assert(shifts < (sizeof(T) * 8), "shift count out of range for type T");
				return value << shifts;
			}

			/// <summary>
			/// Signed left shift with a compile-time shift count.
			/// Uses unsigned-domain arithmetic for safety; negative inputs use TemplateLeftShiftNegative.
			/// </summary>
			/// <typeparam name="T">Signed integer type.</typeparam>
			/// <typeparam name="shifts">Compile-time shift count.</typeparam>
			/// <param name="value">Value to shift.</param>
			/// <returns>Left-shifted signed value (magnitude shifted in unsigned domain).</returns>
			template<typename T, uint8_t shifts>
			static constexpr T TemplateLeftShift(const T value, TypeTraits::TypeDispatch::FalseType) // signed, compile-time
			{
				static_assert(shifts < (sizeof(T) * 8), "shift count out of range for type T");
				return (value >= 0)
					? static_cast<T>(static_cast<typename TypeTraits::TypeSign::make_unsigned<T>::type>(value) << shifts)
					: TemplateLeftShiftNegative<T>(value, shifts);
			}
		}

		/// <summary>
		/// Sign-preserving right shift (runtime shift count).
		/// - Unsigned T: logical right shift (zero-fill).
		/// - Signed T: arithmetic right shift emulation (sign-preserving).
		/// </summary>
		/// <typeparam name="T">Integral type.</typeparam>
		/// <param name="value">Value to shift.</param>
		/// <param name="shifts">Number of bit positions to shift.</param>
		/// <returns>Right-shifted value with semantics appropriate for T.</returns>
		template<typename T>
		static constexpr T SignedRightShift(const T value, const uint8_t shifts)
		{
			return SignedShifts::TemplateRightShift(
				value, shifts,
				typename TypeTraits::TypeSign::IsUnsignedType<T>::type{});
		}

		/// <summary>
		/// Sign-preserving right shift with a compile-time shift count.
		/// - Unsigned T: logical right shift (zero-fill).
		/// - Signed T: arithmetic right shift emulation (sign-preserving).
		/// </summary>
		/// <typeparam name="T">Integral type.</typeparam>
		/// <typeparam name="shifts">Compile-time shift count.</typeparam>
		/// <param name="value">Value to shift.</param>
		/// <returns>Right-shifted value with semantics appropriate for T.</returns>
		template<typename T, const uint8_t shifts>
		static constexpr T SignedRightShift(const T value)
		{
			return SignedShifts::TemplateRightShift<T, shifts>(
				value,
				typename TypeTraits::TypeSign::IsUnsignedType<T>::type{});
		}

		/// <summary>
		/// Sign-preserving left shift (runtime shift count).
		/// - Unsigned T: logical left shift (zero-fill on the right).
		/// - Signed T: magnitude shifted in the unsigned domain; negative inputs yield
		///   -( |v| << s ) modulo 2^N to avoid signed UB.
		/// </summary>
		/// <typeparam name="T">Integral type.</typeparam>
		/// <param name="value">Value to shift.</param>
		/// <param name="shifts">Number of bit positions to shift.</param>
		/// <returns>Left-shifted value with semantics appropriate for T.</returns>
		template<typename T>
		static constexpr T SignedLeftShift(const T value, const uint8_t shifts)
		{
			return SignedShifts::TemplateLeftShift(
				value, shifts,
				typename TypeTraits::TypeSign::IsUnsignedType<T>::type{});
		}

		/// <summary>
		/// Sign-preserving left shift with a compile-time shift count.
		/// - Unsigned T: logical left shift (zero-fill on the right).
		/// - Signed T: magnitude shifted in the unsigned domain; negative inputs yield
		///   -( |v| << s ) modulo 2^N to avoid signed UB.
		/// </summary>
		/// <typeparam name="T">Integral type.</typeparam>
		/// <typeparam name="shifts">Compile-time shift count.</typeparam>
		/// <param name="value">Value to shift.</param>
		/// <returns>Left-shifted value with semantics appropriate for T.</returns>
		template<typename T, const uint8_t shifts>
		static constexpr T SignedLeftShift(const T value)
		{
			static_assert(shifts < (sizeof(T) * 8));
			return SignedShifts::TemplateLeftShift<T, shifts>(
				value,
				typename TypeTraits::TypeSign::IsUnsignedType<T>::type{});
		}
	}
	using namespace BitShifts;
}
#endif