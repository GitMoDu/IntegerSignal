#ifndef _INTEGER_TRIGONOMETRY_TANGENT32_h
#define _INTEGER_TRIGONOMETRY_TANGENT32_h

#include "Lut/Tangent16Lut.h"

namespace IntegerSignal
{
	namespace Trigonometry
	{
		using namespace FixedPoint::ScalarFraction;

		/// <summary>
		/// Tangent in fixed-point Q-format (32-bit signed fraction, Q0.30).
		/// Produces a Fraction32::scalar_t. Uses quarter-wave LUT and identities to cover
		/// the full 0..360 degrees range. Results are scaled to Fraction32::FRACTION_1X (power-of-two) via shifts.
		/// Note: Near 90 deg and 270 deg the magnitude grows quickly; this implementation uses LUT
		/// interpolation and the reciprocal identity (tan(x) = 1 / tan(90 deg - x)) and may saturate to Fraction32::FRACTION_1X.
		/// </summary>
		/// <param name="angle">Modular angle_t in [0; ANGLE_RANGE].</param>
		/// <returns>Signed Q-format fraction.</returns>
		static Fraction32::scalar_t Tangent32(const angle_t angle)
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
					// For angles up to 45 deg use the LUT directly (map 16-bit to Q0.30).
					const uint16_t interpolated = Lut::Tangent16::GetInterpolated(angle);
					return (static_cast<uint32_t>(interpolated) << 14) | (interpolated >> 2);
				}
				else
				{
					// For angles between 45 and 90 deg, use: tan(x) = 1 / tan(90 deg - x)
					const uint16_t tanComp16 = Lut::Tangent16::GetInterpolated(ANGLE_90 - angle);
					// Map complement to Q0.30
					const Fraction32::scalar_t tanComplement = (static_cast<uint32_t>(tanComp16) << 14) | (tanComp16 >> 2);

					if (tanComplement == 0)
					{
						// Avoid division by zero; saturate to unit.
						return Fraction32::FRACTION_1X;
					}
					return Fraction32::FRACTION_1X / tanComplement;
				}
			}
			else if (angle < ANGLE_180)
			{
				// Second quadrant: tan(x) = -tan(180 deg - x)
				return -Tangent32(ANGLE_180 - angle);
			}
			else if (angle < ANGLE_270)
			{
				// Third quadrant: tan(x) = tan(x - 180 deg)
				return Tangent32(angle - ANGLE_180);
			}
			else if (angle < ANGLE_RANGE)
			{
				// Fourth quadrant: tan(x) = -tan(360 deg - x)
				return -Tangent32(ANGLE_RANGE - angle);
			}
			else
			{
				// angle == ANGLE_RANGE wraps to 0
				return 0;
			}
		}

		/// <summary>
		/// Cotangent in fixed-point Q-format (32-bit signed fraction, Q0.30).
		/// Defined as cot(x) = 1 / tan(x), with simple handling of tan(x) == 0.
		/// </summary>
		/// <param name="angle">Modular angle_t in [0; ANGLE_RANGE].</param>
		/// <returns>Signed Q-format fraction.</returns>
		static Fraction32::scalar_t Cotangent32(const angle_t angle)
		{
			const Fraction32::scalar_t tanValue = Tangent32(angle);
			if (tanValue == 0)
			{
				// Handle division-by-zero case (choose 0 or a sentinel per application).
				return 0;
			}
			return Fraction32::FRACTION_1X / tanValue;
		}
	}
}
#endif