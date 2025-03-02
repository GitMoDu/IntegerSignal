#ifndef _INTEGER_SIGNAL_MIX_h
#define _INTEGER_SIGNAL_MIX_h

#include <stdint.h>

namespace IntegerSignal::Mix
{
	namespace Template
	{
		/// <summary>
		/// 256 gradations of mix.
		/// </summary>
		/// <param name="from">0 scale value.</param>
		/// <param name="to">Full scale value.</param>
		/// <param name="scale">Abstract scale of From and To [0 ; UINT8_MAX].</param>
		/// <returns>Proportional linear mix.</returns>
		template<typename value_t,
			typename intermediate_t>
		static constexpr value_t Linear(const value_t from, const value_t to, const uint8_t scale)
		{
			return ((((intermediate_t)from) * (UINT8_MAX - scale)) + (((intermediate_t)to) * scale)) / UINT8_MAX;
		}
	}

	/// <summary>
	/// Fast mix, fixed proportions.
	/// </summary>
	/// <param name="from"></param>
	/// <param name="to"></param>
	/// <returns>50/50 linear mix.</returns>
	static constexpr uint8_t LinearU8(const uint8_t from, const uint8_t to)
	{
		return ((uint16_t)from + to) >> 1;
	}

	/// <summary>
	/// Fast mix, fixed proportions.
	/// </summary>
	/// <param name="from"></param>
	/// <param name="to"></param>
	/// <returns>50/50 linear mix.</returns>
	static constexpr uint16_t LinearU16(const uint16_t from, const uint16_t to)
	{
		return ((uint32_t)from + to) >> 1;
	}

	/// <summary>
	/// Fast mix, fixed proportions.
	/// </summary>
	/// <param name="from"></param>
	/// <param name="to"></param>
	/// <returns>50/50 linear mix.</returns>
	static constexpr uint32_t LinearU32(const uint32_t from, const uint32_t to)
	{
		return ((uint64_t)from + to) >> 1;
	}

	/// <summary>
	/// Fast mix, fixed proportions.
	/// </summary>
	/// <param name="from"></param>
	/// <param name="to"></param>
	/// <returns>50/50 mix in power^2 space.</returns>
	static constexpr uint8_t Power2U8(const uint8_t from, const uint8_t to)
	{
		return (((uint16_t)from * from) + ((uint16_t)to * to)) >> 8;
	}

	/// <summary>
	/// Fast mix, fixed proportions.
	/// </summary>
	/// <param name="from"></param>
	/// <param name="to"></param>
	/// <returns>50/50 mix in power^2 space.</returns>
	static constexpr uint16_t Power2U16(const uint16_t from, const uint16_t to)
	{
		return (((uint32_t)from * from) + ((uint32_t)to * to)) >> 16;
	}

	/// <summary>
	/// Fast mix, fixed proportions.
	/// </summary>
	/// <param name="from"></param>
	/// <param name="to"></param>
	/// <returns>50/50 mix in power^2 space.</returns>
	static constexpr uint32_t Power2U32(const uint32_t from, const uint32_t to)
	{
		return (((uint64_t)from * from) + ((uint64_t)to * to)) >> 32;
	}

	/// <summary>
	/// 256 gradations of mix.
	/// </summary>
	/// <param name="from">0 scale value.</param>
	/// <param name="to">Full scale value.</param>
	/// <param name="scale">Abstract scale of From and To [0 ; UINT8_MAX].</param>
	/// <returns>Proportional linear mix.</returns>
	static constexpr uint8_t LinearU8(const uint8_t from, const uint8_t to, const uint8_t scale)
	{
		return Template::Linear<uint8_t, uint16_t>(from, to, scale);
	}

	/// <summary>
	/// 256 gradations of mix.
	/// </summary>
	/// <param name="from">0 scale value.</param>
	/// <param name="to">Full scale value.</param>
	/// <param name="scale">Abstract scale of From and To [0 ; UINT8_MAX].</param>
	/// <returns>Proportional linear mix.</returns>
	static constexpr uint16_t LinearU16(const uint16_t from, const uint16_t to, const uint8_t scale)
	{
		return Template::Linear<uint16_t, uint32_t>(from, to, scale);
	}

	/// <summary>
	/// 256 gradations of mix.
	/// </summary>
	/// <param name="from">0 scale value.</param>
	/// <param name="to">Full scale value.</param>
	/// <param name="scale">Abstract scale of From and To [0 ; UINT8_MAX].</param>
	/// <returns>Proportional linear mix.</returns>
	static constexpr uint16_t LinearU32(const uint32_t from, const uint32_t to, const uint8_t scale)
	{
		return Template::Linear<uint32_t, uint64_t>(from, to, scale);
	}
}
#endif