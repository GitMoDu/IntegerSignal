#ifndef _INTEGER_TRIGONOMETRY_LUT_TANGENT16_h
#define _INTEGER_TRIGONOMETRY_LUT_TANGENT16_h

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
			namespace Tangent16
			{
				/// <summary>
				/// [0 ; 45] degrees Tan function table.
				/// [0 ; UINT16_MAX] scale.
				/// </summary>
				static constexpr uint16_t Table[] //PROGMEM
				{
					0, 201, 402, 603, 804, 1005, 1206, 1407,
					1608, 1809, 2011, 2212, 2413, 2615, 2816, 3018,
					3219, 3421, 3622, 3824, 4026, 4228, 4430, 4632,
					4834, 5036, 5238, 5441, 5643, 5846, 6048, 6251,
					6454, 6657, 6860, 7064, 7267, 7471, 7675, 7878,
					8082, 8287, 8491, 8696, 8900, 9105, 9310, 9515,
					9721, 9926, 10132, 10338, 10544, 10751, 10957, 11164,
					11371, 11578, 11786, 11993, 12201, 12409, 12618, 12826,
					13035, 13244, 13454, 13663, 13873, 14084, 14294, 14505,
					14716, 14927, 15139, 15351, 15563, 15776, 15988, 16202,
					16415, 16629, 16843, 17058, 17273, 17488, 17703, 17919,
					18135, 18352, 18569, 18787, 19004, 19222, 19441, 19660,
					19879, 20099, 20319, 20540, 20761, 20982, 21204, 21427,
					21649, 21873, 22096, 22320, 22545, 22770, 22996, 23222,
					23448, 23675, 23903, 24131, 24360, 24589, 24818, 25048,
					25279, 25510, 25742, 25975, 26207, 26441, 26675, 26910,
					27145, 27381, 27617, 27854, 28092, 28330, 28569, 28809,
					29049, 29290, 29532, 29774, 30017, 30260, 30505, 30750,
					30995, 31242, 31489, 31737, 31985, 32235, 32485, 32735,
					32987, 33239, 33493, 33747, 34001, 34257, 34513, 34771,
					35029, 35288, 35547, 35808, 36070, 36332, 36595, 36859,
					37125, 37391, 37658, 37926, 38194, 38464, 38735, 39007,
					39280, 39553, 39828, 40104, 40381, 40659, 40938, 41218,
					41499, 41781, 42065, 42349, 42635, 42921, 43209, 43498,
					43789, 44080, 44373, 44666, 44962, 45258, 45555, 45854,
					46154, 46456, 46759, 47063, 47368, 47675, 47983, 48293,
					48604, 48916, 49230, 49545, 49862, 50180, 50500, 50821,
					51144, 51468, 51794, 52121, 52450, 52781, 53113, 53447,
					53783, 54120, 54459, 54800, 55142, 55487, 55833, 56181,
					56530, 56882, 57235, 57591, 57948, 58307, 58669, 59032,
					59397, 59764, 60134, 60505, 60878, 61254, 61632, 62012,
					62394, 62778, 63165, 63554, 63945, 64339, 64735, 65134
				};

				static constexpr uint16_t LutSize = sizeof(Table) / sizeof(Table[0]);

				static const uint16_t GetInterpolated(const angle_t angle)
				{
					// Use ANGLE_45 because our LUT is built for 0–45°.
					static constexpr auto StepError = (ANGLE_45 / LutSize);

					// Map the angle (in the range [0, ANGLE_45]) to a table index in [0, LutSize).
					const uint16_t flooredIndex = ((uint32_t)angle << GetBitShifts(LutSize))
						>> GetBitShifts(ANGLE_45);

					if (flooredIndex < (LutSize - 1))
					{
						// Compute the lower bound angle corresponding to the floored index.
						const angle_t flooredAngle = ((uint32_t)(flooredIndex) << GetBitShifts(ANGLE_45)) >> GetBitShifts(LutSize);
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
						// If angle is at or beyond the last step, return the maximum table value.
						return UINT16_MAX;
					}
				}
			}
		}
	}
}
#endif

