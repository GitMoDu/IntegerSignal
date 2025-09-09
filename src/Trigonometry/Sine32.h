#ifndef _INTEGER_TRIGONOMETRY_SINE32_h
#define _INTEGER_TRIGONOMETRY_SINE32_h

#include "Lut/Sine16Lut.h"

namespace IntegerSignal
{
	namespace Trigonometry
	{
		using namespace FixedPoint::Fraction;

		/// <summary>
		/// Sine using fixed-point Q-format (32-bit signed fraction, Q0.30).
		/// - Input: angle_t is a modular full-rotation fixed-point type in [0; ANGLE_RANGE].
		/// - Output: Fraction32::scalar_t in [-Fraction32::FRACTION_1X; +Fraction32::FRACTION_1X].
		/// Implementation uses a 16-bit quarter-wave LUT and symmetry; the interpolated 16-bit value
		/// is scaled to Q0.30 using a bit composition that spans the full unit range.
		/// </summary>
		/// <param name="angle">Modular angle_t in [0; ANGLE_RANGE] (wrap-around at ANGLE_RANGE+1).</param>
		/// <returns>Signed Q-format fraction in [-Fraction32::FRACTION_1X; +Fraction32::FRACTION_1X].</returns>
		static Fraction32::scalar_t Sine32(const angle_t angle)
		{
			if (angle == 0)
			{
				// Skip interpolation and reading Table[0]
				return 0;
			}
			else if (angle >= ANGLE_180)
			{
				// sin(x) = -sin(x - 180 deg)
				return -Sine32(angle - ANGLE_180);
			}
			else if (angle > ANGLE_90)
			{
				// sin(x) = sin(180 deg - x)
				return Sine32(ANGLE_90 - (angle - ANGLE_90));
			}
			else
			{
				const uint16_t interpolated = Lut::Sine16::GetInterpolated(angle);
				// Map 16-bit quarter-wave amplitude to Q0.30 range
				return (static_cast<uint32_t>(interpolated) << 14) | (interpolated >> 2);
			}
		}

		/// <summary>
		/// Cosine via phase shift: cos(x) = sin(x + 90 deg).
		/// - Input: angle_t (modular full-rotation fixed-point).
		/// - Output: Fraction32::scalar_t in [-Fraction32::FRACTION_1X; +Fraction32::FRACTION_1X].
		/// </summary>
		/// <param name="angle">Modular angle_t in [0; ANGLE_RANGE].</param>
		/// <returns>Signed Q-format fraction in [-Fraction32::FRACTION_1X; +Fraction32::FRACTION_1X].</returns>
		static Fraction32::scalar_t Cosine32(const angle_t angle)
		{
			return Sine32(angle + ANGLE_90);
		}
	}
}
#endif