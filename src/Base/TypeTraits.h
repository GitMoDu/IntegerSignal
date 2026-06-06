#ifndef _INTEGER_SIGNAL_TYPE_TRAITS_h
#define _INTEGER_SIGNAL_TYPE_TRAITS_h

#include <stdint.h>

namespace IntegerSignal
{
	/// <summary>
	/// Provides type traits and utilities for integer types.
	/// Organized into namespaces for limits, type promotion, tag dispatch, and signed/unsigned detection.
	/// Used throughout IntegerSignal for safe, efficient fixed-point and signal processing.
	/// </summary>
	namespace TypeTraits
	{
		namespace TypeLimits
		{
			/// <summary>
			/// Returns min/max values for supported integer types.
			/// Specializations for each type.
			/// </summary>
			template<typename T>
			struct type_limits;

			// Specializations for unsigned types
			template<> struct type_limits<uint8_t> { static constexpr uint8_t Max() { return UINT8_MAX; } static constexpr uint8_t Min() { return 0; } };
			template<> struct type_limits<uint16_t> { static constexpr uint16_t Max() { return UINT16_MAX; } static constexpr uint16_t Min() { return 0; } };
			template<> struct type_limits<uint32_t> { static constexpr uint32_t Max() { return UINT32_MAX; } static constexpr uint32_t Min() { return 0; } };
			template<> struct type_limits<uint64_t> { static constexpr uint64_t Max() { return UINT64_MAX; } static constexpr uint64_t Min() { return 0; } };

			// Specializations for signed types
			template<> struct type_limits<int8_t> { static constexpr int8_t Max() { return INT8_MAX; } static constexpr int8_t Min() { return INT8_MIN; } };
			template<> struct type_limits<int16_t> { static constexpr int16_t Max() { return INT16_MAX; } static constexpr int16_t Min() { return INT16_MIN; } };
			template<> struct type_limits<int32_t> { static constexpr int32_t Max() { return INT32_MAX; } static constexpr int32_t Min() { return INT32_MIN; } };
			template<> struct type_limits<int64_t> { static constexpr int64_t Max() { return INT64_MAX; } static constexpr int64_t Min() { return INT64_MIN; } };
		}

		namespace TypeNext
		{
			/// <summary>
			/// Promotes an unsigned type to the next larger unsigned type.
			/// Used for intermediate calculations to prevent overflow.
			/// </summary>
			template<typename T> struct next_uint_type;
			template<> struct next_uint_type<uint8_t> { using type = uint16_t; };
			template<> struct next_uint_type<uint16_t> { using type = uint32_t; };
			template<> struct next_uint_type<uint32_t> { using type = uint64_t; };
			template<> struct next_uint_type<uint64_t> { using type = uint64_t; };
			template<> struct next_uint_type<int8_t> { using type = uint16_t; };
			template<> struct next_uint_type<int16_t> { using type = uint32_t; };
			template<> struct next_uint_type<int32_t> { using type = uint64_t; };
			template<> struct next_uint_type<int64_t> { using type = uint64_t; };

			/// <summary>
			/// Promotes an integer type to the next larger signed type.
			/// Used for intermediate calculations to prevent overflow.
			/// </summary>
			template<typename T> struct next_int_type;
			template<> struct next_int_type<uint8_t> { using type = int16_t; };
			template<> struct next_int_type<uint16_t> { using type = int32_t; };
			template<> struct next_int_type<uint32_t> { using type = int64_t; };
			template<> struct next_int_type<uint64_t> { using type = int64_t; };
			template<> struct next_int_type<int8_t> { using type = int16_t; };
			template<> struct next_int_type<int16_t> { using type = int32_t; };
			template<> struct next_int_type<int32_t> { using type = int64_t; };
			template<> struct next_int_type<int64_t> { using type = int64_t; };
		}

		/// <summary>
		/// Tag type for true/false value in type traits.
		/// Used for tag dispatch in template metaprogramming.
		/// </summary>
		namespace TypeDispatch
		{
			struct TrueType {};
			struct FalseType {};

			// std::is_same equivalent (C++11, no <type_traits> dependency).
			template<typename T, typename U>
			struct is_same { enum { value = false }; using type = FalseType; };

			template<typename T>
			struct is_same<T, T> { enum { value = true }; using type = TrueType; };
		}

		/// <summary>
		/// Provides type traits for a conditional type selector.
		/// </summary>
		namespace TypeConditional
		{
			template<typename T1, typename T2, bool use_first>
			struct conditional_type { using type = T1; };
			template<typename T1, typename T2>
			struct conditional_type<T1, T2, false> { using type = T2; };

			template<typename T1, typename T2>
			struct larger_type {
				enum { value = (sizeof(T1) >= sizeof(T2)) };
				using type = typename conditional_type<T1, T2, value>::type;
			};

			template<typename T1, typename T2>
			struct smaller_type {
				enum { value = (sizeof(T1) < sizeof(T2)) };
				using type = typename conditional_type<T1, T2, value>::type;
			};
		}

		/// <summary>
		/// Provides enable_if/disable_if SFINAE helpers.
		/// </summary>
		namespace TypeEnableIf
		{
			template<bool Condition, typename T = void>
			struct enable_if {};

			template<typename T>
			struct enable_if<true, T> { using type = T; };

			template<bool Condition, typename T = void>
			struct disable_if { using type = T; };

			template<typename T>
			struct disable_if<true, T> {};
		}

		namespace Evaluation
		{
			using ConstantEvaluatedType = TypeDispatch::TrueType;
			using RuntimeEvaluatedType = TypeDispatch::FalseType;

			template<bool IsConstant>
			struct evaluation_tag { using type = RuntimeEvaluatedType; };

			template<>
			struct evaluation_tag<true> { using type = ConstantEvaluatedType; };

			/// <summary>
			/// Detects whether the current evaluation context is a constant-evaluated expression.
			/// Uses the strongest compiler feature available and falls back to false when unsupported.
			/// 
			/// Intended targets include Arduino boards, such as Uno 328P (AVR), 
			/// STM32 (ARM Cortex-M), RP2040/RP2350 (ARM Cortex-M, RISC-V), and other embedded platforms.
			/// On newer embedded compilers, this can detect constant evaluation and enable constexpr-aware dispatch.
			/// On older AVR-style toolchains that do not expose the required compiler support, it returns false so callers remain correct and simply skip constexpr-only dispatch.
			/// 
			/// Implementation notes:
			/// - Uses if consteval when the compiler provides it.
			/// - Uses __builtin_is_constant_evaluated when the compiler exposes it.
			/// - Falls back to false when neither mechanism is available.
			/// </summary>
			static constexpr bool IsConstantEvaluated()
			{
#if defined(__cpp_if_consteval)
				if consteval
				{
					return true;
				}
				return false;
#elif defined(__has_builtin)
#if __has_builtin(__builtin_is_constant_evaluated)
				return __builtin_is_constant_evaluated();
#else
				return false;
#endif
#elif defined(_MSC_VER) && defined(__builtin_is_constant_evaluated)
				return __builtin_is_constant_evaluated();
#elif defined(__GNUC__) && (__GNUC__ >= 9)
				return __builtin_is_constant_evaluated();
#else
				return false;
#endif
			}

			/// <summary>
			/// Returns a tag that can be used for compile-time dispatch between constant-evaluated and runtime paths.
			/// This matches the library's existing tag-dispatch style used by other type traits helpers.
			/// </summary>
			static constexpr typename evaluation_tag<IsConstantEvaluated()>::type GetEvaluationTag()
			{
				return typename evaluation_tag<IsConstantEvaluated()>::type();
			}
		}

		namespace TypeSign
		{
			/// <summary>
			/// Type trait to detect unsigned types.
			/// </summary>
			template<typename T> struct is_unsigned { enum { value = false }; };
			template<> struct is_unsigned<uint8_t> { enum { value = true }; };
			template<> struct is_unsigned<uint16_t> { enum { value = true }; };
			template<> struct is_unsigned<uint32_t> { enum { value = true }; };
			template<> struct is_unsigned<uint64_t> { enum { value = true }; };

			/// <summary>
			/// Type trait to detect signed types.
			/// </summary>
			template<typename T> struct is_signed { enum { value = false }; };
			template<> struct is_signed<int8_t> { enum { value = true }; };
			template<> struct is_signed<int16_t> { enum { value = true }; };
			template<> struct is_signed<int32_t> { enum { value = true }; };
			template<> struct is_signed<int64_t> { enum { value = true }; };

			/// <summary>
			/// Provides a tag type for unsigned types for tag dispatch.
			/// Used to select implementation for unsigned/signed types.
			/// </summary>
			template<typename T> struct IsUnsignedType { typedef TypeDispatch::FalseType type; };
			template<> struct IsUnsignedType<uint8_t> { typedef TypeDispatch::TrueType type; };
			template<> struct IsUnsignedType<uint16_t> { typedef TypeDispatch::TrueType type; };
			template<> struct IsUnsignedType<uint32_t> { typedef TypeDispatch::TrueType type; };
			template<> struct IsUnsignedType<uint64_t> { typedef TypeDispatch::TrueType type; };

			/// <summary>
			/// make_unsigned / make_signed replacements (no <type_traits> dependency).
			/// Maps signed -> corresponding unsigned and unsigned -> identity.
			/// </summary>
			template<typename T> struct make_unsigned { using type = T; };
			template<> struct make_unsigned<int8_t> { using type = uint8_t; };
			template<> struct make_unsigned<int16_t> { using type = uint16_t; };
			template<> struct make_unsigned<int32_t> { using type = uint32_t; };
			template<> struct make_unsigned<int64_t> { using type = uint64_t; };

			template<> struct make_unsigned<uint8_t> { using type = uint8_t; };
			template<> struct make_unsigned<uint16_t> { using type = uint16_t; };
			template<> struct make_unsigned<uint32_t> { using type = uint32_t; };
			template<> struct make_unsigned<uint64_t> { using type = uint64_t; };

			/// <summary>
			/// make_signed: maps unsigned -> corresponding signed and signed -> identity.
			/// </summary>
			template<typename T> struct make_signed { using type = T; };
			template<> struct make_signed<uint8_t> { using type = int8_t; };
			template<> struct make_signed<uint16_t> { using type = int16_t; };
			template<> struct make_signed<uint32_t> { using type = int32_t; };
			template<> struct make_signed<uint64_t> { using type = int64_t; };

			template<> struct make_signed<int8_t> { using type = int8_t; };
			template<> struct make_signed<int16_t> { using type = int16_t; };
			template<> struct make_signed<int32_t> { using type = int32_t; };
			template<> struct make_signed<int64_t> { using type = int64_t; };
		}
	}
}
#endif