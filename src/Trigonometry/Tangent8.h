#ifndef _INTEGER_TRIGONOMETRY_TANGENT8_h
#define _INTEGER_TRIGONOMETRY_TANGENT8_h

#include "Lut/Tangent8Lut.h"

namespace IntegerSignal
{
	namespace Trigonometry
	{
		using namespace FixedPoint::ScalarFraction;

		/// <summary>
		/// Tangent using fixed-point Q-format (8-bit signed fraction, Q0.6).
		/// - Input: angle_t is a modular full-rotation fixed-point type in [0; ANGLE_RANGE].
		/// - Output: Fraction8::scalar_t (approximately in [-Fraction8::FRACTION_1X; +Fraction8::FRACTION_1X]).
		/// Uses a quarter-wave LUT and identities to cover the full turn. Results are aligned
		/// to Fraction8::FRACTION_1X (power-of-two) via shifts.
		/// Note: Near 90 deg and 270 deg the magnitude grows quickly; this implementation uses LUT
		/// interpolation and the reciprocal identity (tan(x) = 1 / tan(90 deg - x)) and may saturate to Fraction8::FRACTION_1X.
		/// </summary>
		/// <param name="angle">Modular angle_t in [0; ANGLE_RANGE].</param>
		/// <returns>Signed Q-format fraction.</returns>
		static Fraction8::scalar_t Tangent8(const angle_t angle)
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
					return Lut::Tangent8::GetInterpolated(angle) >> 2;
				}
				else
				{
					// For angles between 45 and 90 deg, use:
					// tan(x) = 1 / tan(90 deg - x)
					const Fraction8::scalar_t tanComplement = Lut::Tangent8::GetInterpolated(ANGLE_90 - angle) >> 2;
					if (tanComplement == 0)
					{
						// Avoid division by zero; saturate to unit.
						return Fraction8::FRACTION_1X;
					}
					return Fraction8::FRACTION_1X / tanComplement;
				}
			}
			else if (angle < ANGLE_180)
			{
				// Second quadrant: tan(x) = -tan(180 deg - x)
				return -Tangent8(ANGLE_180 - angle);
			}
			else if (angle < ANGLE_270)
			{
				// Third quadrant: tan(x) = tan(x - 180 deg)
				return Tangent8(angle - ANGLE_180);
			}
			else if (angle < ANGLE_RANGE)
			{
				// Fourth quadrant: tan(x) = -tan(360 deg - x)
				return -Tangent8(ANGLE_RANGE - angle);
			}
			else
			{
				// When angle == ANGLE_RANGE (i.e. 360 deg) we treat it as 0.
				return 0;
			}
		}

		/// <summary>
		/// Cotangent using fixed-point Q-format (8-bit signed fraction, Q0.6).
		/// Defined as cot(x) = 1 / tan(x), with simple handling for tan(x) == 0.
		/// </summary>
		/// <param name="angle">Modular angle_t in [0; ANGLE_RANGE].</param>
		/// <returns>Signed Q-format fraction.</returns>
		static Fraction8::scalar_t Cotangent8(const angle_t angle)
		{
			const Fraction8::scalar_t tanValue = Tangent8(angle);
			if (tanValue == 0)
			{
				// Handle division-by-zero case (choose 0 or a sentinel per application).
				return 0;
			}
			return Fraction8::FRACTION_1X / tanValue;
		}
	}
}
#endif