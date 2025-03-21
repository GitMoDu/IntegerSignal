#ifndef _INTEGER_TRIGONOMETRY_SINE32_h
#define _INTEGER_TRIGONOMETRY_SINE32_h

#include "Lut/Sine16Lut.h"

namespace IntegerSignal::Trigonometry
{
	using namespace Fraction;

	/// <summary>
	/// Get scale fraction from Sine(angle) .
	/// </summary>
	/// <param name="angle">Angle (0;360) degrees [0; ANGLE_MAX].</param>
	/// <returns>Scale fraction [-FRACTION32_1X; +FRACTION32_1X].</returns>
	static const fraction32_t Sine32(const angle_t angle)
	{
		if (angle == 0)
		{
			// Skip interpolation and reading Table[0];
			return 0;
		}
		else if (angle >= ANGLE_180)
		{
			// Sine funcionts loops inverted after 180�.
			return -Sine32(angle - ANGLE_180);
		}
		else if (angle > ANGLE_90)
		{
			// Sine function repeats reversed after 90�.
			return Sine32(ANGLE_90 - (angle - ANGLE_90));
		}
		else
		{
			const uint16_t interpolated = Lut::Sine16::GetInterpolated(angle);
			return ((uint32_t)interpolated << 14) | (interpolated >> 2);
		}
	}

	/// <summary>
	/// Get scale fraction from Cosine(angle) .
	/// </summary>
	/// <param name="angle">Angle (0;360) degrees [0; ANGLE_MAX].</param>
	/// <returns>Scale fraction [-FRACTION16_1X; +FRACTION16_1X].</returns>
	static const fraction32_t Cosine32(const angle_t angle)
	{
		return Sine32(angle + ANGLE_90);
	}
}
#endif