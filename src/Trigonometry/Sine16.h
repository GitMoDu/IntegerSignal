#ifndef _INTEGER_TRIGONOMETRY_SINE16_h
#define _INTEGER_TRIGONOMETRY_SINE16_h

#include "Lut/Sine16Lut.h"

namespace IntegerSignal
{
	namespace Trigonometry
	{
		using namespace FixedPoint::Fraction;

		/// <summary>
		/// Sine using fixed-point Q-format (16-bit signed fraction, Q0.14).
		/// - Input: angle_t is a modular full-rotation fixed-point type in [0; ANGLE_RANGE].
		/// - Output: Fraction16::scalar_t in [-Fraction16::FRACTION_1X; +Fraction16::FRACTION_1X].
		/// Implementation uses a quarter-wave LUT and symmetry; the LUT amplitude is aligned
		/// to Fraction16::FRACTION_1X (power-of-two) via a final right-shift.
		/// </summary>
		/// <param name="angle">Modular angle_t in [0; ANGLE_RANGE] (wrap-around at ANGLE_RANGE+1).</param>
		/// <returns>Signed Q-format fraction in [-Fraction16::FRACTION_1X; +Fraction16::FRACTION_1X].</returns>
		static Fraction16::scalar_t Sine16(const angle_t angle)
		{
			if (angle == 0)
			{
				// Skip interpolation and reading Table[0]
				return 0;
			}
			else if (angle >= ANGLE_180)
			{
				// sin(x) = -sin(x - 180 deg)
				return -Sine16(angle - ANGLE_180);
			}
			else if (angle > ANGLE_90)
			{
				// sin(x) = sin(180 deg - x)
				return Sine16(ANGLE_90 - (angle - ANGLE_90));
			}
			else
			{
				// Scale quarter-wave LUT to Q-format fraction (power-of-two unit).
				return static_cast<uint32_t>(Lut::Sine16::GetInterpolated(angle) + 4) >> 2;
			}
		}

		/// <summary>
		/// Cosine via phase shift: cos(x) = sin(x + 90 deg).
		/// - Input: angle_t (modular full-rotation fixed-point).
		/// - Output: Fraction16::scalar_t in [-Fraction16::FRACTION_1X; +Fraction16::FRACTION_1X].
		/// </summary>
		/// <param name="angle">Modular angle_t in [0; ANGLE_RANGE].</param>
		/// <returns>Signed Q-format fraction in [-Fraction16::FRACTION_1X; +Fraction16::FRACTION_1X].</returns>
		static Fraction16::scalar_t Cosine16(const angle_t angle)
		{
			return Sine16(angle + ANGLE_90);
		}
	}
}
#endif