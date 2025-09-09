#ifndef _INTEGER_TRIGONOMETRY_TANGENT16_h
#define _INTEGER_TRIGONOMETRY_TANGENT16_h

#include "Lut/Tangent16Lut.h"

namespace IntegerSignal
{
	namespace Trigonometry
	{
		using namespace FixedPoint::Fraction;

		/// <summary>
		/// Tangent in fixed-point Q-format (16-bit signed fraction, Q0.14).
		/// Produces a Fraction16::scalar_t. Uses quarter-wave LUT and identities to cover
		/// the full 0..360 degrees range. Results are scaled to Fraction16::FRACTION_1X (power-of-two) via shifts.
		/// Note: Near 90 deg and 270 deg the magnitude grows quickly; this implementation uses LUT
		/// interpolation and the reciprocal identity (tan(x) = 1 / tan(90 deg - x)) and may saturate to Fraction16::FRACTION_1X.
		/// </summary>
		/// <param name="angle">Modular angle_t in [0; ANGLE_RANGE].</param>
		/// <returns>Signed Q-format fraction, approximately in [-Fraction16::FRACTION_1X; +Fraction16::FRACTION_1X].</returns>
		static Fraction16::scalar_t Tangent16(const angle_t angle)
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
					// For angles up to 45 deg use the LUT directly.
					return Lut::Tangent16::GetInterpolated(angle) >> 2;
				}
				else
				{
					// For angles between 45 and 90 deg, use:
					// tan(x) = 1 / tan(90 deg - x)
					const Fraction16::scalar_t tanComplement = Lut::Tangent16::GetInterpolated(ANGLE_90 - angle) >> 2;
					if (tanComplement == 0)
					{
						// Avoid division by zero; saturate to unit.
						return Fraction16::FRACTION_1X;
					}
					return Fraction16::FRACTION_1X / tanComplement;
				}
			}
			else if (angle < ANGLE_180)
			{
				// Second quadrant: tan(x) = -tan(180 deg - x)
				return -Tangent16(ANGLE_180 - angle);
			}
			else if (angle < ANGLE_270)
			{
				// Third quadrant: tan(x) = tan(x - 180 deg)
				return Tangent16(angle - ANGLE_180);
			}
			else if (angle < ANGLE_RANGE)
			{
				// Fourth quadrant: tan(x) = -tan(360 deg - x)
				return -Tangent16(ANGLE_RANGE - angle);
			}
			else
			{
				// When angle == ANGLE_RANGE (i.e. 360 deg) we treat it as 0.
				return 0;
			}
		}

		/// <summary>
		/// Cotangent in fixed-point Q-format (16-bit signed fraction, Q0.14).
		/// Defined as cot(x) = 1 / tan(x), with simple handling of tan(x) == 0.
		/// </summary>
		/// <param name="angle">Modular angle_t in [0; ANGLE_RANGE].</param>
		/// <returns>Signed Q-format fraction.</returns>
		static Fraction16::scalar_t Cotangent16(const angle_t angle)
		{
			const Fraction16::scalar_t tanValue = Tangent16(angle);
			if (tanValue == 0)
			{
				// Handle division by zero case
				return 0; // Or a chosen sentinel for "infinite"
			}
			return Fraction16::FRACTION_1X / tanValue;
		}
	}
}
#endif