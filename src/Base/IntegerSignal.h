#ifndef _INTEGER_SIGNAL_h
#define _INTEGER_SIGNAL_h

#include <stdint.h>

namespace IntegerSignal
{
	/// <summary>
	/// Replacement for std::min() and Arduino min().
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="a"></param>
	/// <param name="b"></param>
	/// <returns></returns>
	template<typename T>
	constexpr const T& MinValue(const T& a, const T& b) {
		return (a < b) ? a : b;
	}

	/// <summary>
	/// Replacement for std::max() and Arduino max().
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="a"></param>
	/// <param name="b"></param>
	/// <returns></returns>
	template<typename T>
	constexpr const T& MaxValue(const T& a, const T& b) {
		return (a > b) ? a : b;
	}

	template <typename T>
	constexpr T GetBitShifts(const T value)
	{
		return (value <= 1) ? 0 : 1 + GetBitShifts(value >> 1);
	}

	template<typename T>
	static constexpr T SignedRightShift(const T value, const uint8_t shifts)
	{
		return (value < 0) ? -(-value >> shifts) : (value >> shifts);
	}
}
#endif