// Math.h

#ifndef _INTEGER_SIGNAL_MATH_h
#define _INTEGER_SIGNAL_MATH_h

#include <stdint.h>

namespace IntegerSignal::Math
{
#if defined(__arm__) 
	/// <summary>
	/// Based on article: https://en.wikipedia.org/wiki/Methods_of_computing_square_roots
	/// Which involves Newton's method which gives the following iterative formula:
	/// 
	/// X(n + 1) = (X(n) + S / X(n)) / 2
	/// Thanks to ARM CLZ instruction(which counts how many bits in a number are
	/// zeros starting from the most significant one) we can very successfully
	/// choose the starting value, so just three iterations are enough to achieve
	/// maximum possible error of 1. The algorithm uses division, but fortunately
	/// it is fast enough here, so square root computation takes only about 50 - 55
	/// cycles with maximum compiler optimization.
	/// </summary>
	/// <param name="value"></param>
	/// <returns>Square root of uint32 value.</returns>
	static const uint16_t Root2U32(const uint32_t input)
	{
		uint32_t root = 1 << ((32 - __builtin_clz(input)) >> 1);

		root = (root + (input / root)) >> 1;
		root = (root + (input / root)) >> 1;
		root = (root + (input / root)) >> 1;

		return root;
	}

	/// <summary>
	/// </summary>
	/// <param name="input"></param>
	/// <returns>Square root of uint16 value.</returns>
	static const uint8_t Root2U16(const uint16_t input)
	{
		return Root2U32(input);
	}
#else
	/// <summary>
	/// I find that most algorithms are based on simple ideas,
	/// but are implemented in a way more complicated manner than necessary.
	/// I've taken the idea from here:
	/// http ://ww1.microchip.com/downloads/en/AppNotes/91040a.pdf (by Ross M. Fosler)
	/// and made it into a very short C - function :
	/// https://stackoverflow.com/questions/1100090/looking-for-an-efficient-integer-square-root-algorithm-for-arm-thumb2
	/// </summary>
	/// <param name="value"></param>
	/// <returns>Square root of uint32 value.</returns>
	static const uint16_t Root2U32(const uint32_t value)
	{
		uint16_t res = 0;
		uint16_t add = 0x8000;

		for (uint_fast8_t i = 0; i < 16; i++)
		{
			uint16_t temp = res | add;
			uint32_t g2 = (uint32_t)temp * temp;
			if (value >= g2)
			{
				res = temp;
			}
			add >>= 1;
		}

		return res;
	}

	/// <summary>
	/// </summary>
	/// <param name="input"></param>
	/// <returns>Square root of uint16 value.</returns>
	static const uint8_t Root2U16(const uint16_t value)
	{
		uint8_t res = 0;
		uint8_t add = 0x80;

		for (uint_fast8_t i = 0; i < 8; i++)
		{
			uint8_t temp = res | add;
			uint16_t g2 = (uint16_t)temp * temp;
			if (value >= g2)
			{
				res = temp;
			}
			add >>= 1;
		}

		return res;
	}
#endif

	/// <summary>
	/// </summary>
	/// <param name="input"></param>
	/// <returns>Square root of uint64 value.</returns>
	static const uint32_t Root2U64(const uint64_t value)
	{
		uint32_t res = 0;
		uint32_t add = 0x800000;

		for (uint_fast8_t i = 0; i < 32; i++)
		{
			uint32_t temp = res | add;
			uint64_t g2 = (uint64_t)temp * temp;
			if (value >= g2)
			{
				res = temp;
			}
			add >>= 1;
		}

		return res;
	}
}

#endif