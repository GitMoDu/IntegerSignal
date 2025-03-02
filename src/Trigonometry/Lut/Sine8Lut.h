#ifndef _INTEGER_TRIGONOMETRY_LUT_SINE8_h
#define _INTEGER_TRIGONOMETRY_LUT_SINE8_h

#include "../Trigonometry.h"

#if defined(ARDUINO_ARCH_AVR)
#include <avr/pgmspace.h>
#endif

namespace IntegerSignal::Trigonometry::Lut::Sine8
{
	/// <summary>
	/// [0 ; 90] degrees Sine function table.
	/// [0 ; UINT8_MAX] fraction.
	/// </summary>
	static constexpr uint8_t Table[] PROGMEM
	{
		0, 6, 12, 18, 24, 31, 37, 43,
		49, 55, 61, 68, 74, 79, 85, 91,
		97, 103, 109, 114, 120, 125, 131, 136,
		141, 146, 151, 156, 161, 166, 171, 175,
		180, 184, 188, 193, 197, 201, 204, 208,
		212, 215, 218, 221, 224, 227, 230, 233,
		235, 237, 240, 242, 244, 245, 247, 248,
		250, 251, 252, 253, 253, 254, 254, 254
	};

	static constexpr uint16_t LutSize = sizeof(Table) / sizeof(Table[0]);

	static const uint8_t GetInterpolated(const angle_t angle)
	{
		static constexpr auto StepError = (ANGLE_90 / LutSize);

		const uint8_t flooredIndex = ((uint32_t)angle << GetBitShifts(LutSize)) >> GetBitShifts(ANGLE_90);

		if (flooredIndex < (LutSize - 1))
		{
			const angle_t flooredAngle = ((uint32_t)(flooredIndex) << GetBitShifts(ANGLE_90)) >> GetBitShifts(LutSize);
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
			// Skip interpolation and reading Table8[LutSize - 1].
			return UINT8_MAX;
		}
	}
}
#endif

