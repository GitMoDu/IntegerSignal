#ifndef _INTEGER_SIGNAL_h
#define _INTEGER_SIGNAL_h

#include <stdint.h>

namespace IntegerSignal
{
	/// <summary>
	/// Replacement for std::min() and Arduino min().
	/// Returns the smaller of the two values.
	/// </summary>
	/// <typeparam name="T">The type of the values being compared, inferred from the arguments.</typeparam>
	/// <param name="a">The first value to compare.</param>
	/// <param name="b">The second value to compare.</param>
	/// <returns>The smaller of the two values.</returns>
	template<typename T>
	constexpr T MinValue(const T a, const T b)
	{
		return (a < b) ? a : b;
	}

	/// <summary>
	/// Replacement for std::max() and Arduino max().
	/// Returns the larger of the two values.
	/// </summary>
	/// <typeparam name="T">The type of the values being compared, inferred from the arguments.</typeparam>
	/// <param name="a">The first value to compare.</param>
	/// <param name="b">The second value to compare.</param>
	/// <returns>The larger of the two values.</returns>
	template<typename T>
	constexpr T MaxValue(const T a, const T b)
	{
		return (a > b) ? a : b;
	}

	/// <summary>
	/// Constrains a value to lie within the specified lower and upper bounds.
	/// </summary>
	/// <typeparam name="T">The type of the value and bounds.</typeparam>
	/// <param name="value">The value to be limited.</param>
	/// <param name="low">The lower bound.</param>
	/// <param name="high">The upper bound.</param>
	/// <returns>The value constrained to the range [low, high].</returns>
	template<typename T>
	constexpr T LimitValue(const T value, const T low, const T high)
	{
		return (value < low) ? low : (value > high) ? high : value;
	}

	/// <summary>
	/// Constrains a value to lie within the specified lower and upper bounds.
	/// </summary>
	/// <typeparam name="T">The type of the value and bounds.</typeparam>
	/// <typeparam name="low">Templated lower bound for the value.</typeparam>
	/// <typeparam name="high">Templated upper bound for the value.</typeparam>
	/// <param name="value">The value to be limited.</param>
	/// <returns>The value limited to the range [low, high].</returns>
	template<typename T,
		T low, T high>
	constexpr T LimitValue(const T value)
	{
		return (value < low) ? low : (value > high) ? high : value;
	}

	/// <summary>
	/// Replacement for std::abs() and Arduino abs().
	/// Returns the absolute value of the given number.
	/// </summary>
	/// <typeparam name="T">The type of the value, inferred from the argument.</typeparam>
	/// <param name="a">The value to get the absolute value of.</param>
	/// <returns>The absolute value of the given number.</returns>
	template<typename T>
	constexpr T AbsValue(const T a)
	{
		return (a >= 0) ? a : -a;
	}

	/// <summary>
	/// Calculates the number of bit shifts required to reduce the value to 1 or less.
	/// </summary>
	/// <typeparam name="T">The type of the value, inferred from the argument.</typeparam>
	/// <param name="value">The value to calculate the bit shifts for.</param>
	/// <returns>The number of bit shifts required.</returns>
	template <typename T>
	constexpr uint8_t GetBitShifts(const T value)
	{
		return (value <= 1) ? 0 : 1 + GetBitShifts(value >> 1);
	}
}

#endif