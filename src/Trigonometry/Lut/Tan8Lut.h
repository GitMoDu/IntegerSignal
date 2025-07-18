#ifndef _INTEGER_TRIGONOMETRY_LUT_TANGENT8_h
#define _INTEGER_TRIGONOMETRY_LUT_TANGENT8_h

#include "../Trigonometry.h"

#if defined(ARDUINO_ARCH_AVR)
#include <avr/pgmspace.h>
#endif

namespace IntegerSignal::Trigonometry::Lut::Tangent8
{
	/// <summary>
	/// [0 ; 45] degrees Tan function table.
	/// [0 ; UINT8_MAX] scale.
	/// </summary>
	static constexpr uint8_t Table[] PROGMEM
	{
		0,   3,   6,  10,  13,  16,  19,  22,
		25,  29,  32,  35,  38,  42,  45,  48,
		51,  55,  58,  62,  65,  69,  72,  76,
		79,  82,  86,  90,  93,  96, 100, 104,
		107, 111, 115, 119, 122, 127, 131, 135,
		139, 143, 147, 152, 156, 161, 166, 170,
		174, 179, 183, 188, 193, 198, 203, 208,
		214, 220, 227, 233, 240, 247, 255, 255
	};

	static constexpr uint8_t LutSize = sizeof(Table) / sizeof(Table[0]);

	static uint8_t GetInterpolated(const angle_t angle)
	{
		// Use ANGLE_45 because our LUT is built for 0–45°.
		static constexpr auto StepError = (ANGLE_45 / LutSize);

		// Map the angle (in the range [0, ANGLE_45]) to a table index in [0, LutSize).
		const uint8_t flooredIndex = ((uint32_t)angle << GetBitShifts(LutSize))
			>> GetBitShifts(ANGLE_45);

		if (flooredIndex < (LutSize - 1))
		{
			// Compute the lower bound angle corresponding to the floored index.
			const angle_t flooredAngle = ((uint32_t)(flooredIndex) << GetBitShifts(ANGLE_45))
				>> GetBitShifts(LutSize);
			const uint8_t angleError = angle - flooredAngle;

#if defined(ARDUINO_ARCH_AVR)
			const uint8_t a = pgm_read_byte(&Table[flooredIndex]);
#else
			const uint8_t a = Table[flooredIndex];
#endif
			if (angleError == 0)
			{
				return a;
			}

#if defined(ARDUINO_ARCH_AVR)
			const uint8_t b = pgm_read_byte(&Table[flooredIndex + 1]);
#else
			const uint8_t b = Table[flooredIndex + 1];
#endif

			return a + (((uint16_t)(b - a) * angleError) >> GetBitShifts(StepError));
		}
		else
		{
			// If angle is at or beyond the last step, return the maximum table value.
			return UINT8_MAX;
		}
	}
}
#endif

