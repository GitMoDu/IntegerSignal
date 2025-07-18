#ifndef _INTEGER_TRIGONOMETRY_TANGENT8_h
#define _INTEGER_TRIGONOMETRY_TANGENT8_h

#include "Lut/Tangent8Lut.h"

namespace IntegerSignal
{
	namespace Trigonometry
	{
		using namespace Fraction;

		/// <summary>
		/// Get scale fraction from Tan(angle).
		/// LUT only valid for angles up to 45°.
		/// </summary>
		/// <param name="angle">Angle (0;360) degrees [0; ANGLE_MAX].</param>
		/// <returns>Scale fraction [-FRACTION8_1X; +FRACTION8_1X].</returns>
		static fraction8_t Tangent8(const angle_t angle)
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
					return Lut::Tangent8::GetInterpolated(angle) >> 2;
				}
				else
				{
					// For angles between 45 and 90, use:
					// tan(angle) = 1 / tan(90 - angle)
					const fraction8_t tanComplement = Lut::Tangent8::GetInterpolated(ANGLE_90 - angle) >> 2;
					if (tanComplement == 0)
					{
						// Avoid division by zero; return the maximum representable value.
						return FRACTION8_1X;
					}
					return FRACTION8_1X / tanComplement;
				}
			}
			else if (angle < ANGLE_180)
			{
				// Second quadrant: tan(angle) = -tan(180º - angle)
				return -Tangent8(ANGLE_180 - angle);
			}
			else if (angle < ANGLE_270)
			{
				// Third quadrant: tan(angle) = tan(angle - 180)
				return Tangent8(angle - ANGLE_180);
			}
			else if (angle < ANGLE_RANGE)
			{
				// Fourth quadrant: tan(angle) = -tan(360 - angle)
				return -Tangent8(ANGLE_RANGE - angle);
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
		static fraction8_t Cotangent8(const angle_t angle)
		{
			fraction8_t tanValue = Tangent8(angle);
			if (tanValue == 0)
			{
				// Handle division by zero case
				return 0; // Or some large value to represent infinity
			}
			return FRACTION8_1X / tanValue;
		}
	}
}
#endif