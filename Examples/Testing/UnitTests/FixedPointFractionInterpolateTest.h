#ifndef _FIXED_POINT_FRACTION_INTERPOLATETEST_h
#define _FIXED_POINT_FRACTION_INTERPOLATETEST_h

#include "FixedPointFractionReferenceTest.h"

namespace IntegerSignal
{
	namespace FixedPoint
	{
		namespace ScalarFraction
		{
			namespace Test
			{
				// Test UInterpolate8 with systematic samples
				static inline bool TestUInterpolate8()
				{
					Serial.println(F("Starting UFraction8 interpolation tests..."));
					uint32_t errorCount = 0;

					const uint8_t testValues[] = { 0, 1, 127, 128, 255 };

					for (uint8_t from : testValues)
					{
						for (uint8_t to : testValues)
						{
							for (uint16_t s = 0; s <= 255; s += 16)
							{
								const uint8_t scalar = (uint8_t)s;
								const uint8_t result = UFraction8::Interpolate(scalar, from, to);
								const uint8_t refResult = RefUInterpolate8(from, to, scalar);

								if (result != refResult)
								{
									Serial.print(F("UInterpolate8 error: from="));
									Serial.print(from);
									Serial.print(F(" to="));
									Serial.print(to);
									Serial.print(F(" scalar="));
									Serial.print(scalar);
									Serial.print(F(" result="));
									Serial.print(result);
									Serial.print(F(" ref="));
									Serial.println(refResult);
									errorCount++;
								}
							}
						}
					}

					if (errorCount == 0)
						Serial.println(F("All UInterpolate8 tests PASSED."));
					else
					{
						Serial.print(F("UInterpolate8 tests FAILED. Errors: "));
						Serial.println(errorCount);
					}
					return errorCount == 0;
				}
			}
		}
	}
}

#endif
