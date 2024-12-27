// Scale.h

#ifndef _INTEGER_SIGNAL_SCALE_h
#define _INTEGER_SIGNAL_SCALE_h

#include <stdint.h>

namespace IntegerSignal::Scale
{
	static constexpr uint32_t U64ToU32(const uint64_t value)
	{
		return value >> 32;
	}

	static constexpr uint16_t U32ToU16(const uint32_t value)
	{
		return value >> 16;
	}

	static constexpr uint8_t U16ToU8(const uint16_t value)
	{
		return value >> 8;
	}

	static constexpr uint16_t U64ToU16(const uint64_t value)
	{
		return U32ToU16(U64ToU32(value));
	}

	static constexpr uint8_t U64ToU8(const uint64_t value)
	{
		return U16ToU8(U64ToU16(value));
	}

	static constexpr uint8_t U32ToU8(const uint32_t value)
	{
		return U16ToU8(U32ToU16(value));
	}

	static constexpr uint16_t U8ToU16(const uint8_t value)
	{
		return ((uint16_t)value << 8) | value;
	}

	static constexpr uint32_t U16ToU32(const uint16_t value)
	{
		return ((uint32_t)value << 16) | value;
	}

	static constexpr uint64_t U32ToU64(const uint32_t value)
	{
		return ((uint64_t)value << 32) | value;
	}

	static constexpr uint32_t U8ToU32(const uint8_t value)
	{
		return U16ToU32(U8ToU16(value));
	}

	static constexpr uint64_t U8ToU64(const uint8_t value)
	{
		return U32ToU64(U8ToU32(value));
	}

	static constexpr uint64_t U16ToU64(const uint16_t value)
	{
		return U32ToU64(U16ToU32(value));
	}
}

#endif