#ifndef _INTEGER_SIGNAL_SIZE_h
#define _INTEGER_SIGNAL_SIZE_h

#include <stdint.h>

namespace IntegerSignal
{
	namespace BitSize
	{
		template<uint64_t N>
		struct constant
		{
			enum { value = N };
		};

		template<typename T>
		struct return_type
		{
			typedef T type;
		};

		template<uint64_t N>
		struct bit_count : constant<1 + bit_count<(N >> 1)>::value> {};

		template<>
		struct bit_count<0> : constant<1> {};

		template<>
		struct bit_count<1> : constant<1> {};

		template<uint64_t N>
		struct byte_count : constant<((bit_count<N>::value + 7) >> 3)> {};

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

	namespace UIntSize
	{
		template<uint64_t N>
		struct uint_type : BitSize::byte_type<BitSize::byte_count<N>::value> {};
	}

	namespace UIntLeastSize
	{
		template<uint64_t N>
		struct uint_least_type : BitSize::byte_least_type<BitSize::byte_count<N>::value> {};
	}

	namespace UIntFastSize
	{
		template<uint64_t N>
		struct uint_fast_type : BitSize::byte_fast_type<BitSize::byte_count<N>::value> {};
	}

	using namespace UIntSize;

	using namespace UIntLeastSize;

	using namespace UIntFastSize;
}
#endif