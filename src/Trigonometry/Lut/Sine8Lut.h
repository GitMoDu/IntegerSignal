#ifndef _INTEGER_TRIGONOMETRY_LUT_SINE8_h
#define _INTEGER_TRIGONOMETRY_LUT_SINE8_h

#include "../Trigonometry.h"

#if defined(ARDUINO_ARCH_AVR)
#include <avr/pgmspace.h>
#endif

namespace IntegerSignal
{
	namespace Trigonometry
	{
		namespace Lut
		{
			namespace Sine8
			{
				/// <summary>
				/// [0 ; 90] degrees Sine function table.
				/// [0 ; UINT8_MAX] scale.
				/// </summary>
				static constexpr uint8_t Table[] PROGMEM
				{
					0, 6, 13, 19, 25, 31, 37, 44,
					50, 56, 62, 68, 74, 80, 86, 92,
					98, 103, 109, 115, 120, 126, 131, 136,
					142, 147, 152, 157, 162, 167, 171, 176,
					180, 185, 189, 193, 197, 201, 205, 208,
					212, 215, 219, 222, 225, 228, 231, 233,
					236, 238, 240, 242, 244, 246, 247, 249,
					250, 251, 252, 253, 254, 254, 255, 255
				};

				static constexpr uint16_t LutSize = sizeof(Table) / sizeof(Table[0]);

				static uint8_t GetInterpolated(const angle_t angle)
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
						// Ceiling interpolation within the LUT step (+StepError), which de-biases the final Q0.6 rounding.
						return a + (((static_cast<uint16_t>(b - a) * angleError) + (StepError >> 1)) >> GetBitShifts(StepError));
					}
					else
					{
						// Skip interpolation and reading Table8[LutSize - 1].
						return UINT8_MAX;
					}
				}
			}
		}
	}
}
#endif

