#ifndef _INTEGER_TRIGONOMETRY_SINE32_h
#define _INTEGER_TRIGONOMETRY_SINE32_h

#include "Lut/Sine16Lut.h"

namespace IntegerSignal
{
	namespace Trigonometry
	{
		using namespace Fraction;

		/// <summary>
		/// Get scale fraction from Sine(angle) .
		/// </summary>
		/// <param name="angle">Angle (0;360) degrees [0; ANGLE_MAX].</param>
		/// <returns>Scale fraction [-FRACTION32_1X; +FRACTION32_1X].</returns>
		static fraction32_t Sine32(const angle_t angle)
		{
			if (angle == 0)
			{
				// Skip interpolation and reading Table[0];
				return 0;
			}
			else if (angle >= ANGLE_180)
			{
				// Sine funcionts loops inverted after 180º.
				return -Sine32(angle - ANGLE_180);
			}
			else if (angle > ANGLE_90)
			{
				// Sine function repeats reversed after 90º.
				return Sine32(ANGLE_90 - (angle - ANGLE_90));
			}
			else
			{
				// Map 16-bit quarter-wave amplitude to Q0.30 range
				return ((static_cast<uint32_t>(Lut::Sine16::GetInterpolated(angle)) + 3) << 14);
			}
		}

		/// <summary>
		/// Get scale fraction from Cosine(angle) .
		/// </summary>
		/// <param name="angle">Angle (0;360) degrees [0; ANGLE_MAX].</param>
		/// <returns>Scale fraction [-FRACTION16_1X; +FRACTION16_1X].</returns>
		static fraction32_t Cosine32(const angle_t angle)
		{
			return Sine32(angle + ANGLE_90);
		}
	}
}
#endif