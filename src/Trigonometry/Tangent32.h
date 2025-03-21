#ifndef _INTEGER_TRIGONOMETRY_TANGENT32_h
#define _INTEGER_TRIGONOMETRY_TANGENT32_h

#include "Lut/Tangent16Lut.h"

namespace IntegerSignal::Trigonometry
{
	using namespace Fraction;

	/// <summary>
	/// Get scale fraction from Tan(angle).
	/// </summary>
	/// <param name="angle">Angle (0;360) degrees [0; ANGLE_MAX].</param>
	/// <returns>Scale fraction [-FRACTION32_1X; +FRACTION32_1X].</returns>
	static const fraction32_t Tangent32(const angle_t angle)
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
				const uint16_t interpolated = Lut::Tangent16::GetInterpolated(angle);
				return ((uint32_t)interpolated << 14) | (interpolated >> 2);
			}
			else
			{
				// For angles between 45 and 90, use:
				// tan(angle) = 1 / tan(90 - angle)
				const fraction16_t tanComplement = Lut::Tangent16::GetInterpolated(ANGLE_90 - angle) >> 2;
				const uint32_t scaled = ((uint32_t)tanComplement << 14) | (tanComplement >> 2);

				if (tanComplement == 0)
				{
					// Avoid division by zero; return the maximum representable value.
					return FRACTION32_1X;
				}
				else
				{
					return FRACTION32_1X - scaled;
				}
			}
		}
		else if (angle < ANGLE_180)
		{
			// Second quadrant: tan(angle) = -tan(180º - angle)
			return -Tangent32(ANGLE_180 - angle);
		}
		else if (angle < ANGLE_270)
		{
			// Third quadrant: tan(angle) = tan(angle - 180)
			return Tangent32(angle - ANGLE_180);
		}
		else if (angle < ANGLE_RANGE)
		{
			// Fourth quadrant: tan(angle) = -tan(360 - angle)
			return -Tangent32(ANGLE_RANGE - angle);
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
	static const fraction32_t Cotangent32(const angle_t angle)
	{
		fraction32_t tanValue = Tangent32(angle);
		if (tanValue == 0)
		{
			// Handle division by zero case
			return 0; // Or some large value to represent infinity
		}
		else
		{
			const fraction16_t inverted = FRACTION16_1X / tanValue;
			return ((uint32_t)inverted << 14) | ((inverted & 0x3ff) >> 2);
		}
	}
}
#endif