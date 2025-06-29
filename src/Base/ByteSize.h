#ifndef _INTEGER_SIGNAL_SIZE_h
#define _INTEGER_SIGNAL_SIZE_h

#include "BitSize.h"

namespace IntegerSignal
{
	/// <summary>
	/// ByteSize provides utilities for compile-time selection of unsigned integer types
	/// based on the minimum number of bytes required to store a given value.
	/// It contains helpers for determining the appropriate type for a value or byte count,
	/// and exposes user-facing templates for type selection by value.
	/// </summary>
	namespace ByteSize
	{
		/// <summary>
		/// ByteType contains internal helpers for selecting unsigned integer types
		/// based on the minimum number of bytes required. It provides templates for
		/// determining the number of bytes needed to store a value, and for mapping
		/// a byte count to the corresponding standard, least, or fast unsigned integer type.
		/// </summary>
		namespace ByteType
		{
			using namespace BitSize;

			/// <summary>
			/// Computes the number of bytes required to represent N.
			/// </summary>
			/// <typeparam name="N">The value to analyze.</typeparam>
			template<uint64_t N>
			struct byte_count : BitType::constant<((bit_count<N>::value + 7) >> 3)> {};

			/// <summary>
			/// Selects an unsigned integer type with at least the specified number of bytes.
			/// For 1, 2, 3, or 4 bytes, returns uint8_t, uint16_t, or uint32_t as appropriate.
			/// Defaults to uint64_t for larger sizes.
			/// </summary>
			/// <typeparam name="N">Number of bytes required.</typeparam>
			template<uint64_t N>
			struct byte_type : BitType::return_type<uint64_t> {};

			template<>
			struct byte_type<4> : BitType::return_type<uint32_t> {};

			template<>
			struct byte_type<3> : BitType::return_type<uint32_t> {};

			template<>
			struct byte_type<2> : BitType::return_type<uint16_t> {};

			template<>
			struct byte_type<1> : BitType::return_type<uint8_t> {};

			/// <summary>
			/// Selects the smallest uint_least type with at least the specified number of bytes.
			/// For 1, 2, 3, or 4 bytes, returns uint_least8_t, uint_least16_t, or uint_least32_t as appropriate.
			/// Defaults to uint_least64_t for larger sizes.
			/// </summary>
			template<uint64_t N>
			struct byte_least_type : BitType::return_type<uint_least64_t> {};

			template<>
			struct byte_least_type<4> : BitType::return_type<uint_least32_t> {};

			template<>
			struct byte_least_type<3> : BitType::return_type<uint_least32_t> {};

			template<>
			struct byte_least_type<2> : BitType::return_type<uint_least16_t> {};

			template<>
			struct byte_least_type<1> : BitType::return_type<uint_least8_t> {};

			/// <summary>
			/// Selects the fastest uint_fast type with at least the specified number of bytes.
			/// For 1, 2, 3, or 4 bytes, returns uint_fast8_t, uint_fast16_t, or uint_fast32_t as appropriate.
			/// Defaults to uint_fast64_t for larger sizes.
			/// </summary>
			template<uint64_t N>
			struct byte_fast_type : BitType::return_type<uint_fast64_t> {};

			template<>
			struct byte_fast_type<4> : BitType::return_type<uint_fast32_t> {};

			template<>
			struct byte_fast_type<3> : BitType::return_type<uint_fast32_t> {};

			template<>
			struct byte_fast_type<2> : BitType::return_type<uint_fast16_t> {};

			template<>
			struct byte_fast_type<1> : BitType::return_type<uint_fast8_t> {};
		}

		/// <summary>
		/// Selects the smallest unsigned integer type that can store values up to N.
		/// Uses ByteType::byte_type to map the required byte count to a type.
		/// </summary>
		/// <typeparam name="N">Maximum value to store.</typeparam>
		template<uint64_t N>
		struct uint_type : ByteType::byte_type<ByteType::byte_count<N>::value> {};

		/// <summary>
		/// Selects the smallest uint_least type that can store values up to N.
		/// Uses ByteType::byte_least_type to map the required byte count to a type.
		/// </summary>
		/// <typeparam name="N">Maximum value to store.</typeparam>
		template<uint64_t N>
		struct uint_least_type : ByteType::byte_least_type<ByteType::byte_count<N>::value> {};

		/// <summary>
		/// Selects the fastest uint_fast type that can store values up to N.
		/// Uses ByteType::byte_fast_type to map the required byte count to a type.
		/// </summary>
		/// <typeparam name="N">Maximum value to store.</typeparam>
		template<uint64_t N>
		struct uint_fast_type : ByteType::byte_fast_type<ByteType::byte_count<N>::value> {};
	}

	using namespace ByteSize;
}
#endif