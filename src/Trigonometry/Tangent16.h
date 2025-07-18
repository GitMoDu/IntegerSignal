#ifndef _INTEGER_TRIGONOMETRY_TANGENT16_h
#define _INTEGER_TRIGONOMETRY_TANGENT16_h

#include "Lut/Tangent16Lut.h"

namespace IntegerSignal
{
	namespace Trigonometry
	{
		using namespace Fraction;

		/// <summary>
		/// Get scale fraction from Tan(angle).
		/// </summary>
		/// <param name="angle">Angle (0;360) degrees [0; ANGLE_MAX].</param>
		/// <returns>Scale fraction [-FRACTION16_1X; +FRACTION16_1X].</returns>
		static fraction16_t Tangent16(const angle_t angle)
		{
			if (angle == 0)
			{
				// tan(0) = 0
				return 0;
			}
			else if (angle < ANGLE_90)
			{
				// First quadrant
				if (angle <= ANGLE_45)
				{
					// For angles up to 45 use the LUT directly.
					return Lut::Tangent16::GetInterpolated(angle) >> 2;
				}
				else
				{
					// For angles between 45 and 90, use:
					// tan(angle) = 1 / tan(90 - angle)
					const fraction16_t tanComplement = Lut::Tangent16::GetInterpolated(ANGLE_90 - angle) >> 2;
					if (tanComplement == 0)
					{
						// Avoid division by zero; return the maximum representable value.
						return FRACTION16_1X;
					}
					return FRACTION16_1X / tanComplement;
				}
			}
			else if (angle < ANGLE_180)
			{
				// Second quadrant: tan(angle) = -tan(180º - angle)
				return -Tangent16(ANGLE_180 - angle);
			}
			else if (angle < ANGLE_270)
			{
				// Third quadrant: tan(angle) = tan(angle - 180)
				return Tangent16(angle - ANGLE_180);
			}
			else if (angle < ANGLE_RANGE)
			{
				// Fourth quadrant: tan(angle) = -tan(360 - angle)
				return -Tangent16(ANGLE_RANGE - angle);
			}
			else
			{
				// When angle == ANGLE_RANGE (i.e. 360) we treat it as 0.
				return 0;
			}
		}

		/// <summary>
		/// Get scale fraction from Cot(angle).
		/// </summary>
		/// <param name="angle">Angle (0;360) degrees [0; ANGLE_RANGE].</param>
		/// <returns>Scale fraction [-FRACTION8_1X; +FRACTION8_1X].</returns>
		static fraction16_t Cotangent16(const angle_t angle)
		{
			fraction16_t tanValue = Tangent16(angle);
			if (tanValue == 0)
			{
				// Handle division by zero case
				return 0; // Or some large value to represent infinity
			}
			return FRACTION16_1X / tanValue;
		}
	}
}
#endif