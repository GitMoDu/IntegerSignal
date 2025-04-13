#ifndef _INTEGER_TRIGONOMETRY_SINE16_h
#define _INTEGER_TRIGONOMETRY_SINE16_h

#include "Lut/Sine16Lut.h"

namespace IntegerSignal
{
	namespace Trigonometry
	{
		using namespace Fraction;

		/// <summary>
		/// Get scale fraction from Sine(angle) .
		/// </summary>
		/// <param name="angle">Angle (0;360) degrees [0; ANGLE_RANGE].</param>
		/// <returns>Scale fraction [-FRACTION16_1X; +FRACTION16_1X].</returns>
		static const fraction16_t Sine16(const angle_t angle)
		{
			if (angle == 0)
			{
				// Skip interpolation and reading Table[0];
				return 0;
			}
			else if (angle >= ANGLE_180)
			{
				// Sine funcionts loops inverted after 180º.
				return -Sine16(angle - ANGLE_180);
			}
			else if (angle > ANGLE_90)
			{
				// Sine function repeats reversed after 90º.
				return Sine16(ANGLE_90 - (angle - ANGLE_90));
			}
			else
			{
				// Scale Sine16 to positive Fraction.
				return Lut::Sine16::GetInterpolated(angle) >> 2;
			}
		}

		/// <summary>
		/// Get scale fraction from Cosine(angle) .
		/// </summary>
		/// <param name="angle">Angle (0;360) degrees [0; ANGLE_RANGE].</param>
		/// <returns>Scale fraction [-FRACTION16_1X; +FRACTION16_1X].</returns>
		static const fraction16_t Cosine16(const angle_t angle)
		{
			return Sine16(angle + ANGLE_90);
		}
	}
}
#endif