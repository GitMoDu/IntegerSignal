#ifndef _INTEGER_TRIGONOMETRY_SINE8_h
#define _INTEGER_TRIGONOMETRY_SINE8_h

#include "Lut/Sine8Lut.h"

namespace IntegerSignal::Trigonometry
{
	using namespace Fraction;

	/// <summary>
	/// Get scale fraction from Sine(angle) .
	/// </summary>
	/// <param name="angle">Angle (0;360) degrees [0; ANGLE_RANGE].</param>
	/// <returns>Scale fraction [-FRACTION8_1X; +FRACTION8_1X].</returns>
	static const fraction8_t Sine8(const angle_t angle)
	{
		if (angle == 0)
		{
			// Skip interpolation and reading Table[0];
			return 0;
		}
		else if (angle < 0)
		{
			// Sine function is symmetrical.
			return Sine8(ANGLE_RANGE - angle);
		}
		else if (angle >= ANGLE_180)
		{
			// Sine funcionts loops inverted after 180º.
			return -Sine8(angle - ANGLE_180);
		}
		else if (angle > ANGLE_90)
		{
			// Sine function repeats reversed after 90º.
			return Sine8(ANGLE_90 - (angle - ANGLE_90));
		}
		else
		{
			// Scale Sine8 to positive Fraction.
			return Lut::Sine8::GetInterpolated(angle) >> 2;
		}
	}

	/// <summary>
	/// Get scale fraction from Cosine(angle) .
	/// </summary>
	/// <param name="angle">Angle (0;360) degrees [0; ANGLE_RANGE].</param>
	/// <returns>Scale fraction [-FRACTION8_1X; +FRACTION8_1X].</returns>
	static const fraction8_t Cosine8(const angle_t angle)
	{
		return Sine8(angle + ANGLE_90);
	}
}
#endif