#ifndef _INTEGER_SIGNAL_SIZE_h
#define _INTEGER_SIGNAL_SIZE_h

#include <stdint.h>

namespace IntegerSignal
{
	/// <summary>
	/// BitSize provides utilities for compile-time bit and byte calculations,
	/// as well as type selection based on required storage size.
	/// </summary>
	namespace BitSize
	{
		/// <summary>
		/// Compile-time constant value.
		/// </summary>
		/// <typeparam name="N">The constant value.</typeparam>
		template<uint64_t N>
		struct constant
		{
			enum { value = N };
		};

		/// <summary>
		/// Helper for returning a type.
		/// </summary>
		/// <typeparam name="T">The type to return.</typeparam>
		template<typename T>
		struct return_type
		{
			typedef T type;
		};

		/// <summary>
		/// Computes the number of bits required to represent N.
		/// </summary>
		/// <typeparam name="N">The value to analyze.</typeparam>
		template<uint64_t N>
		struct bit_count : constant<1 + bit_count<(N >> 1)>::value> {};

		/// <summary>
		/// Specialization for N = 0.
		/// </summary>
		template<>
		struct bit_count<0> : constant<1> {};

		/// <summary>
		/// Specialization for N = 1.
		/// </summary>
		template<>
		struct bit_count<1> : constant<1> {};

		/// <summary>
		/// Computes the number of bytes required to represent N.
		/// </summary>
		/// <typeparam name="N">The value to analyze.</typeparam>
		template<uint64_t N>
		struct byte_count : constant<((bit_count<N>::value + 7) >> 3)> {};

		/// <summary>
		/// Selects an unsigned integer type with at least the specified number of bytes.
		/// </summary>
		/// <typeparam name="N">Number of bytes required.</typeparam>
		template<uint64_t N>
		struct byte_type : BitSize::return_type<uint64_t> {};

		template<>
		struct byte_type<4> : BitSize::return_type<uint32_t> {};

		template<>
		struct byte_type<3> : BitSize::return_type<uint32_t> {};

		template<>
		struct byte_type<2> : BitSize::return_type<uint16_t> {};

		template<>
		struct byte_type<1> : BitSize::return_type<uint8_t> {};

		/// <summary>
		/// Selects the smallest uint_least type with at least the specified number of bytes.
		/// </summary>
		template<uint64_t N>
		struct byte_least_type : BitSize::return_type<uint_least64_t> {};

		template<>
		struct byte_least_type<4> : BitSize::return_type<uint_least32_t> {};

		template<>
		struct byte_least_type<3> : BitSize::return_type<uint_least32_t> {};

		template<>
		struct byte_least_type<2> : BitSize::return_type<uint_least16_t> {};

		template<>
		struct byte_least_type<1> : BitSize::return_type<uint_least8_t> {};

		/// <summary>
		/// Selects the fastest uint_fast type with at least the specified number of bytes.
		/// </summary>
		template<uint64_t N>
		struct byte_fast_type : BitSize::return_type<uint_fast64_t> {};

		template<>
		struct byte_fast_type<4> : BitSize::return_type<uint_fast32_t> {};

		template<>
		struct byte_fast_type<3> : BitSize::return_type<uint_fast32_t> {};

		template<>
		struct byte_fast_type<2> : BitSize::return_type<uint_fast16_t> {};

		template<>
		struct byte_fast_type<1> : BitSize::return_type<uint_fast8_t> {};
	}

	/// <summary>
	/// UIntSize provides a type alias for the smallest unsigned integer type
	/// that can store values up to N.
	/// </summary>
	namespace UIntSize
	{
		/// <summary>
		/// Selects the smallest unsigned integer type that can store values up to N.
		/// </summary>
		/// <typeparam name="N">Maximum value to store.</typeparam>
		template<uint64_t N>
		struct uint_type : BitSize::byte_type<BitSize::byte_count<N>::value> {};
	}

	/// <summary>
	/// UIntLeastSize provides a type alias for the smallest uint_least type
	/// that can store values up to N.
	/// </summary>
	namespace UIntLeastSize
	{
		/// <summary>
		/// Selects the smallest uint_least type that can store values up to N.
		/// </summary>
		/// <typeparam name="N">Maximum value to store.</typeparam>
		template<uint64_t N>
		struct uint_least_type : BitSize::byte_least_type<BitSize::byte_count<N>::value> {};
	}

	/// <summary>
	/// UIntFastSize provides a type alias for the fastest uint_fast type
	/// that can store values up to N.
	/// </summary>
	namespace UIntFastSize
	{
		/// <summary>
		/// Selects the fastest uint_fast type that can store values up to N.
		/// </summary>
		/// <typeparam name="N">Maximum value to store.</typeparam>
		template<uint64_t N>
		struct uint_fast_type : BitSize::byte_fast_type<BitSize::byte_count<N>::value> {};
	}

	using namespace UIntSize;
	using namespace UIntLeastSize;
	using namespace UIntFastSize;
}
#endif