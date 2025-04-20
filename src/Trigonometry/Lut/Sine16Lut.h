#ifndef _INTEGER_TRIGONOMETRY_LUT_SINE16_h
#define _INTEGER_TRIGONOMETRY_LUT_SINE16_h

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
			namespace Sine16
			{
#if (INTEGER_TRIGONOMETRY_LUT == INTEGER_TRIGONOMETRY_LUT_DEFAULT)
				/// <summary>
				/// [0 ; 90] degrees Sine function table (1/256th divided).
				/// [0 ; UINT16_MAX] fraction.
				/// </summary>
				static constexpr uint16_t Table[] PROGMEM
				{
					0, 402, 804, 1206, 1608, 2010, 2412, 2813,
					3215, 3617, 4018, 4419, 4821, 5221, 5622, 6023,
					6423, 6823, 7223, 7622, 8022, 8421, 8819, 9218,
					9615, 10013, 10410, 10807, 11203, 11599, 11995, 12390,
					12785, 13179, 13573, 13966, 14358, 14750, 15142, 15533,
					15923, 16313, 16702, 17091, 17479, 17866, 18252, 18638,
					19023, 19408, 19791, 20174, 20557, 20938, 21319, 21699,
					22078, 22456, 22833, 23210, 23585, 23960, 24334, 24707,
					25079, 25450, 25820, 26189, 26557, 26924, 27290, 27655,
					28019, 28382, 28744, 29105, 29465, 29823, 30181, 30537,
					30892, 31247, 31599, 31951, 32302, 32651, 32999, 33346,
					33691, 34035, 34378, 34720, 35061, 35400, 35737, 36074,
					36409, 36742, 37075, 37406, 37735, 38063, 38390, 38715,
					39039, 39361, 39682, 40001, 40319, 40635, 40950, 41263,
					41574, 41885, 42193, 42500, 42805, 43109, 43411, 43711,
					44010, 44307, 44603, 44896, 45189, 45479, 45768, 46055,
					46340, 46623, 46905, 47185, 47463, 47739, 48014, 48287,
					48558, 48827, 49094, 49360, 49623, 49885, 50145, 50403,
					50659, 50913, 51165, 51415, 51664, 51910, 52155, 52397,
					52638, 52876, 53113, 53347, 53580, 53810, 54039, 54265,
					54490, 54712, 54933, 55151, 55367, 55581, 55793, 56003,
					56211, 56416, 56620, 56821, 57021, 57218, 57413, 57606,
					57796, 57985, 58171, 58355, 58537, 58717, 58894, 59069,
					59242, 59413, 59582, 59748, 59912, 60074, 60234, 60391,
					60546, 60699, 60849, 60997, 61143, 61287, 61428, 61567,
					61704, 61838, 61970, 62100, 62227, 62352, 62474, 62595,
					62713, 62828, 62941, 63052, 63161, 63267, 63370, 63472,
					63570, 63667, 63761, 63853, 63942, 64029, 64114, 64196,
					64275, 64353, 64427, 64500, 64570, 64637, 64702, 64765,
					64825, 64883, 64938, 64991, 65042, 65090, 65135, 65178,
					65219, 65257, 65293, 65326, 65357, 65385, 65411, 65435,
					65456, 65474, 65490, 65504, 65515, 65523, 65530, 65533
				};

#elif (INTEGER_TRIGONOMETRY_LUT == INTEGER_TRIGONOMETRY_LUT_TINY)
				/// <summary>
				/// [0 ; 90] degrees Sine function table (1/1024th divided).
				/// [0 ; UINT16_MAX] fraction.
				/// </summary>
				static constexpr uint16_t Table[] PROGMEM
				{
					0, 1608, 3215, 4821, 6423, 8022, 9615, 11203,
					12785, 14358, 15923, 17479, 19023, 20557, 22078, 23585,
					25079, 26557, 28019, 29465, 30892, 32302, 33691, 35061,
					36409, 37735, 39039, 40319, 41574, 42805, 44010, 45189,
					46340, 47463, 48558, 49623, 50659, 51664, 52638, 53580,
					54490, 55367, 56211, 57021, 57796, 58537, 59242, 59912,
					60546, 61143, 61704, 62227, 62713, 63161, 63570, 63942,
					64275, 64570, 64825, 65042, 65219, 65357, 65456, 65520
				};
#else
#error Integer Sine has no LUT size defined.
#endif

				static constexpr uint16_t LutSize = sizeof(Table) / sizeof(Table[0]);

				/// <summary>
				/// LUT table based fast Sine calculator up to 90 degrees.
				/// Calculates the interpolated fraction from the LUT.
				/// </summary>
				/// <param name="angle">[0 ; ANGLE_90]</param>
				/// <returns>Sine scale [0 ; UINT16_MAX].</returns>
				static const uint16_t GetInterpolated(const angle_t angle)
				{
					static constexpr auto StepError = (ANGLE_90 / LutSize);

					const uint16_t flooredIndex = ((uint32_t)angle << GetBitShifts(LutSize)) >> GetBitShifts(ANGLE_90);

					if (flooredIndex < (LutSize - 1))
					{
						const angle_t flooredAngle = ((uint32_t)(flooredIndex) << GetBitShifts(ANGLE_90)) >> GetBitShifts(LutSize);
						const uint8_t angleError = angle - flooredAngle;

#if defined(ARDUINO_ARCH_AVR)
						const uint16_t a = pgm_read_word(&Table[flooredIndex]);
#else
						const uint16_t a = Table[flooredIndex];
#endif
						if (angleError == 0)
						{
							return a;
						}

#if defined(ARDUINO_ARCH_AVR)
						const uint16_t b = pgm_read_word(&Table[flooredIndex + 1]);
#else
						const uint16_t b = Table[flooredIndex + 1];
#endif
						return a + (((uint32_t)(b - a) * angleError) >> GetBitShifts(StepError));
					}
					else
					{
						// Skip interpolation and reading Table16[LutSize - 1].
						return UINT16_MAX;
					}
				}
			}
		}
	}
}
#endif
