#ifndef _INTEGER_SIGNAL_SQUARE_ROOT_h
#define _INTEGER_SIGNAL_SQUARE_ROOT_h

#include <stdint.h>

namespace IntegerSignal
{
#if defined(__arm__) && !defined(INTEGER_SIGNAL_DISABLE_ACCELERATION)
	/// <summary>
	/// Square root of uint32_t value.
	/// Based on article: https://en.wikipedia.org/wiki/Methods_of_computing_square_roots
	/// Which involves Newton's method which gives the following iterative formula:
	/// 
	/// X(n + 1) = (X(n) + S / X(n)) / 2
	/// Thanks to ARM CLZ instruction(which counts how many bits in a number are
	/// zeros starting from the most significant one) we can very successfully
	/// choose the starting value, so just three iterations are enough to achieve
	/// maximum possible error of off by 1. The algorithm uses division, but fortunately
	/// it is fast enough here, so square root computation takes only about 50 - 55
	/// cycles with maximum compiler optimization.
	/// Added final check to correct off by one errors.
	/// </summary>
	/// <param name="value">[0 ; UINT32_MAX]</param>
	/// <returns>[0 ; UINT16_MAX].</returns>
	static uint16_t SquareRoot32(const uint32_t value)
	{
		if (value == 0)
			return 0;

		uint32_t root = (uint32_t)1 << ((32 - __builtin_clz(value)) >> 1);

		root = (root + (value / root)) >> 1;
		root = (root + (value / root)) >> 1;
		root = (root + (value / root)) >> 1;

		// If (root+1)^2 is still <= value, then the computed value is too low.
		if (((uint64_t)(root + 1) * (root + 1)) <= value)
		{
			root++; // adjust upward
		}
		// If root^2 overshoots the value, adjust downward.
		else if (((uint64_t)root * root) > value)
		{
			root--; // adjust downward
		}

		return (uint16_t)root;
	}

	/// <summary>
	/// Square root of uint16_t value.
	/// Minimized version of SquareRoot32 implementation, optimized for uint16_t values.
	/// </summary>
	/// <param name="value">[0 ; UINT16_MAX]</param>
	/// <returns>[0 ; UINT8_MAX].</returns>
	static uint8_t SquareRoot16(const uint16_t value)
	{
		if (value == 0)
			return 0;

		uint16_t root = (uint16_t)1 << ((32 - __builtin_clz((uint32_t)value)) >> 1);

		root = (root + (value / root)) >> 1;
		root = (root + (value / root)) >> 1;

		// If (root+1)^2 is still <= value, then the computed value is too low.
		if (((uint32_t)(root + 1) * (root + 1)) <= value)
		{
			root++;
		}
		else if (((uint32_t)root * root) > value)
		{
			// If root^2 overshoots the value, adjust downward.
			root--;
		}

		return (uint8_t)root;
	}

	/// <summary>
	/// Square root of uint64_t value.
	/// Expanded version of SquareRoot32 implementation for uint64_t values.
	/// </summary>
	/// <param name="value">[0 ; UINT64_MAX]</param>
	/// <returns>[0 ; UINT32_MAX].</returns>
	static uint32_t SquareRoot64(const uint64_t value)
	{
		if (value == 0)
		{
			return 0;
		}
		else if (value <= UINT32_MAX)
		{
			return SquareRoot32((uint32_t)value);
		}
		else
		{
			uint64_t root = (uint64_t)1 << ((64 - __builtin_clzll(value)) >> 1);

			root = (root + (value / root)) >> 1;
			root = (root + (value / root)) >> 1;
			root = (root + (value / root)) >> 1;
			root = (root + (value / root)) >> 1;

			// Upward correction:
				// Instead of computing (root+1)^2, we check if (root+1) <= value/(root+1)
			if ((root + 1) <= (value / (root + 1)))
				root++;

			// Downward correction:
			// Instead of computing root^2, we check if root > value/root.
			else if (root > (value / root))
				root--;

			return (uint32_t)root;
		}
	}
#elif (defined(__i386__) || defined(__x86_64__) || defined(_M_IX86) || defined(_M_X64)) && !defined(INTEGER_SIGNAL_DISABLE_ACCELERATION)
	/// <summary>
	/// Square root of uint32_t value (x86 optimized).
	/// </summary>
	static uint16_t SquareRoot32(const uint32_t value)
	{
		if (value == 0)
			return 0;

#if defined(_MSC_VER)
		unsigned long index;
		_BitScanReverse(&index, value);
		uint32_t root = (uint32_t)1 << ((index + 1) >> 1);
#else
		uint32_t root = (uint32_t)1 << ((32 - __builtin_clz(value)) >> 1);
#endif

		root = (root + (value / root)) >> 1;
		root = (root + (value / root)) >> 1;
		root = (root + (value / root)) >> 1;

		// Upward correction
		if (((uint64_t)(root + 1) * (root + 1)) <= value)
			root++;
		// Downward correction
		else if (((uint64_t)root * root) > value)
			root--;

		return (uint16_t)root;
	}

	/// <summary>
	/// Square root of uint16_t value (x86 optimized).
	/// </summary>
	static uint8_t SquareRoot16(const uint16_t value)
	{
		if (value == 0)
			return 0;

#if defined(_MSC_VER)
		unsigned long index;
		_BitScanReverse(&index, value);
		uint16_t root = (uint16_t)1 << ((index + 1) >> 1);
#else
		uint16_t root = (uint16_t)1 << ((32 - __builtin_clz((uint32_t)value)) >> 1);
#endif

		root = (root + (value / root)) >> 1;
		root = (root + (value / root)) >> 1;

		// Upward correction
		if (((uint32_t)(root + 1) * (root + 1)) <= value)
			root++;
		// Downward correction
		else if (((uint32_t)root * root) > value)
			root--;

		return (uint8_t)root;
	}

	/// <summary>
	/// Square root of uint64_t value (x86 optimized).
	/// </summary>
	static uint32_t SquareRoot64(const uint64_t value)
	{
		if (value == 0)
			return 0;
		else if (value <= UINT32_MAX)
			return SquareRoot32((uint32_t)value);
		else
		{
#if defined(_MSC_VER)
			unsigned long index;
			_BitScanReverse64(&index, value);
			uint64_t root = (uint64_t)1 << ((index + 1) >> 1);
#else
			uint64_t root = (uint64_t)1 << ((64 - __builtin_clzll(value)) >> 1);
#endif

			root = (root + (value / root)) >> 1;
			root = (root + (value / root)) >> 1;
			root = (root + (value / root)) >> 1;
			root = (root + (value / root)) >> 1;

			// Upward correction
			if ((root + 1) <= (value / (root + 1)))
				root++;
			// Downward correction
			else if (root > (value / root))
				root--;

			return (uint32_t)root;
		}
	}
#else
	/// <summary>
	/// Square root of uint16_t value.
	/// </summary>
	/// <param name="value">[0 ; UINT16_MAX]</param>
	/// <returns>[0 ; UINT8_MAX].</returns>
	static uint8_t SquareRoot16(const uint16_t value)
	{
		uint16_t op = value;
		uint16_t result = 0;

		// Start with the highest power of 4 that fits in a 16-bit value.
		// (For 16 bits, 1 << 14 == 16384 is the largest power of four <= 65535.)
		uint16_t bit = 1U << 14;

		// Shift 'bit' downward until it is no larger than the operand.
		while (bit > op)
			bit >>= 2;

		// Iterate over candidate bits (like long division) to build "result"
		while (bit != 0)
		{
			if (op >= result + bit)
			{
				op -= result + bit;
				result = (result >> 1) + bit;
			}
			else
			{
				result >>= 1;
			}
			bit >>= 2;
		}
		return (uint8_t)result;
	}

	/// <summary>
	/// Square root of uint32_t value.
	/// </summary>
	/// <param name="value">[0 ; UINT32_MAX]</param>
	/// <returns>[0 ; UINT16_MAX].</returns>
	static uint16_t SquareRoot32(const uint32_t value)
	{
		uint32_t op = value;

		uint32_t result = 0; // Result (will be <= 0xFFFF)
		uint32_t bit = 1UL << 30;  // "bit" starts at the highest power of 4 <= 2^32

		// First, shift 'bit' down until it is <= op.
		while (bit > op)
			bit >>= 2;

		// The algorithm below is akin to a "long division" for square roots.
		// In each iteration, we compare (result + bit) with the remaining op.
		// If op is large enough, we subtract (result + bit) from it and add bit to result.
		// Regardless, we shift result right by one and bit right by two bits.
		while (bit != 0)
		{
			if (op >= result + bit)
			{
				op -= result + bit;
				result = (result >> 1) + bit;
			}
			else
			{
				result >>= 1;
			}
			bit >>= 2;
		}
		return (uint16_t)result;
	}

	/// <summary>
	/// Square root of uint64_t value.
	/// </summary>
	/// <param name="value">[0 ; UINT64_MAX]</param>
	/// <returns>[0 ; UINT32_MAX].</returns>
	static uint32_t SquareRoot64(const uint64_t value)
	{
		uint64_t op = value;

		// Will contain the result (<= 0xFFFFFFFF)
		uint64_t result = 0;

		// Start with the highest power of 4 for a 64-bit input: 1ULL << 62.
		uint64_t bit = 1ULL << 62;

		// Shift 'bit' downward until it is <= op.
		while (bit > op)
			bit >>= 2;

		// Iteratively compute the square root.
		while (bit != 0)
		{
			if (op >= result + bit)
			{
				op -= result + bit;
				result = (result >> 1) + bit;
			}
			else
			{
				result >>= 1;
			}
			bit >>= 2;
		}
		return (uint32_t)result;
	}
#endif
}
#endif