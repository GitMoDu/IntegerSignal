#ifndef _FIXED_POINT_FRACTION_UNSIGNEDTEST_h
#define _FIXED_POINT_FRACTION_UNSIGNEDTEST_h

#include "FixedPointFractionReferenceTest.h"

namespace IntegerSignal
{
	namespace FixedPoint
	{
		namespace ScalarFraction
		{
			namespace Test
			{
				// Sampled test for UFraction16 for uint8_t, uint16_t, and uint32_t.
				template<uint32_t MaxIterations = 50000>
				static inline bool TestUFraction16Sample()
				{
					const uint32_t iterations = MinValue<uint32_t>(MaxIterations, (uint32_t)UINT16_MAX);
					const uint32_t fracSamples = MinValue<uint32_t>(iterations, 1024u);
					const uint32_t fracStep = MaxValue<uint32_t>(1u, (uint32_t)UINT16_MAX / fracSamples);
					const uint32_t valStep = MaxValue<uint32_t>(1u, iterations / 256u);

					Serial.println(F("Starting sampled ufraction16_t tests..."));

					uint32_t errorCount = 0;

					for (uint32_t f = 0; f <= UINT16_MAX; f += fracStep)
					{
						const UFraction16::scalar_t fraction16 =
							(UFraction16::scalar_t)MinValue<int32_t>(UFraction16::FRACTION_1X, (int32_t)f);

						for (uint32_t i = 0; i <= iterations; i += valStep)
						{
							const uint32_t value32 = (uint32_t)(((uint64_t)i * UINT32_MAX) / (iterations ? (iterations - 1) : 1));
							const uint16_t value16 = value32 / INT16_MAX;
							const uint8_t  value8 = value16 / INT8_MAX;

							const uint8_t  r8 = UFraction16::Fraction(fraction16, value8);
							const uint8_t  ref8 = RefUFraction16(value8, fraction16);

							const uint16_t r16 = UFraction16::Fraction(fraction16, value16);
							const uint16_t ref16 = RefUFraction16(value16, fraction16);

							const uint32_t r32 = UFraction16::Fraction(fraction16, value32);
							const uint32_t ref32 = RefUFraction16(value32, fraction16);

							if (r8 != ref8 || r16 != ref16 || r32 != ref32)
							{
								Serial.print(F("ufraction16_t error: fraction="));
								Serial.print(fraction16);
								if (r8 != ref8) { Serial.print(F(" r8="));  Serial.print(r8);  Serial.print(F(" ref=")); Serial.println(ref8); }
								if (r16 != ref16) { Serial.print(F(" r16=")); Serial.print(r16); Serial.print(F(" ref=")); Serial.println(ref16); }
								if (r32 != ref32) { Serial.print(F(" r32=")); Serial.print(r32); Serial.print(F(" ref=")); Serial.println(ref32); }
								errorCount++;
								break;
							}
						}

						if ((f % (uint32_t)(UINT16_MAX / 32u)) == 0)
						{
							Serial.print(F("ufraction16_t progress: f="));
							Serial.println(f);
						}
					}

					if (errorCount == 0) Serial.println(F("All ufraction16_t sampled tests PASSED."));
					else {
						Serial.print(F("ufraction16_t sampled tests FAILED. Errors: "));
						Serial.println(errorCount);
					}

					return errorCount == 0;
				}

				// Sampled test for UFraction8 for uint8_t, uint16_t, and uint32_t.
				template<uint32_t MaxIterations = 50000>
				static inline bool TestUFraction8Sample()
				{
					const uint32_t iterations = min(MaxIterations, (uint32_t)UINT16_MAX);

					Serial.println(F("Starting sampled ufraction8_t tests..."));

					uint32_t errorCount = 0;

					for (uint16_t f = 1; f <= UINT8_MAX; f++)
					{
						const UFraction8::scalar_t fraction8 = (UFraction8::scalar_t)f; // allow full [0..~2) range

						for (uint32_t i = 0; i <= iterations; i++)
						{
							const uint32_t value32 = (uint32_t)(((uint64_t)i * UINT32_MAX) / (iterations - 1));
							const uint16_t value16 = value32 / UINT16_MAX;
							const uint8_t value8 = value16 / UINT8_MAX;

							const uint8_t result8 = UFraction8::Fraction(fraction8, value8);
							const uint8_t refResult8 = RefUFraction8(value8, fraction8);

							const uint16_t result16 = UFraction8::Fraction(fraction8, value16);
							const uint16_t refResult16 = RefUFraction8(value16, fraction8);

							const uint32_t result32 = UFraction8::Fraction(fraction8, value32);
							const uint32_t refResult32 = RefUFraction8(value32, fraction8);

							if (result8 != refResult8
								|| result16 != refResult16
								|| result32 != refResult32)
							{
								Serial.print(F("ufraction8_t error: fraction="));
								Serial.print(fraction8);
								if (result8 != refResult8)
								{
									Serial.print(F(" result8="));
									Serial.print(result8);
									Serial.print(F(" ref="));
									Serial.println(refResult8);
								}
								if (result16 != refResult16)
								{
									Serial.print(F(" result16="));
									Serial.print(result16);
									Serial.print(F(" ref="));
									Serial.println(refResult16);
								}
								if (result32 != refResult32)
								{
									Serial.print(F(" result32="));
									Serial.print(result32);
									Serial.print(F(" ref="));
									Serial.println(refResult32);
									Serial.print(F(" error="));
									Serial.println(result32 - refResult32);
								}
								errorCount++;
							}
						}

						if ((f % 16) == 0)
						{
							Serial.print(F("ufraction8_t progress: "));
							Serial.print(f);
							Serial.println(F(" values tested..."));
						}
					}

					if (errorCount == 0)
					{
						Serial.println(F("All ufraction8_t tests PASSED."));
					}
					else
					{
						Serial.print(F("ufraction8_t tests FAILED. Errors: "));
						Serial.println(errorCount);
					}

					return errorCount == 0;
				}

				// Exhaustive test for UFraction8::scalar_t (expanded full coverage, no clamp to 1x)
				static inline bool TestUFraction8Exhaustive()
				{
					Serial.println(F("Starting exhaustive ufraction8_t tests..."));

					uint32_t errorCount = 0;
					for (uint16_t v = 0; v <= UINT8_MAX; v++)
					{
						for (uint16_t f = 0; f <= UINT8_MAX; f++)
						{
							const uint8_t value = (uint8_t)v;
							const UFraction8::scalar_t fraction8 = (UFraction8::scalar_t)f;
							const uint8_t result = UFraction8::Fraction(fraction8, value);
							const uint8_t refResult = RefUFraction8(value, fraction8);
							if (result != refResult)
							{
								Serial.print(F("ufraction8_t error: value="));
								Serial.print(value);
								Serial.print(F(" ufraction8="));
								Serial.print(fraction8);
								Serial.print(F(" result8="));
								Serial.print(result);
								Serial.print(F(" ref="));
								Serial.println(refResult);
								errorCount++;
							}
						}
						if ((v % 16) == 0)
						{
							Serial.print(F("ufraction8_t progress: "));
							Serial.print(v);
							Serial.println(F(" values tested..."));
						}
					}
					if (errorCount == 0)
					{
						Serial.println(F("All ufraction8_t tests PASSED."));
					}
					else
					{
						Serial.print(F("ufraction8_t tests FAILED. Errors: "));
						Serial.println(errorCount);
					}

					return errorCount == 0;
				}

				// Exhaustive test for UFraction16::scalar_t
				static inline bool TestUFraction16Exhaustive()
				{
					Serial.println(F("Starting exhaustive ufraction16_t tests..."));

					uint32_t errorCount = 0;
					for (uint32_t v = 0; v <= UINT16_MAX; v++)
					{
						for (uint32_t f = 1; f <= UINT16_MAX; f++)
						{
							const uint16_t value = (uint16_t)v;
							const UFraction16::scalar_t fraction16 = (UFraction16::scalar_t)MinValue<uint16_t>(UINT16_MAX, (uint16_t)f); // allow >1x
							const uint16_t result = UFraction16::Fraction(fraction16, value);
							const uint16_t refResult = RefUFraction16(value, fraction16);
							if (result != refResult)
							{
								Serial.print(F("ufraction16_t error: value="));
								Serial.print(value);
								Serial.print(F(" fraction="));
								Serial.print(fraction16);
								Serial.print(F(" result="));
								Serial.print(result);
								Serial.print(F(" ref="));
								Serial.println(refResult);
								errorCount++;
							}
						}
						if ((v % 256) == 0)
						{
							Serial.print(F("ufraction16_t progress: "));
							Serial.print(v);
							Serial.println(F(" values tested..."));
						}
					}
					if (errorCount == 0)
					{
						Serial.println(F("All ufraction16_t tests PASSED."));
					}
					else
					{
						Serial.print(F("ufraction16_t tests FAILED. Errors: "));
						Serial.println(errorCount);
					}

					return errorCount == 0;
				}

				// Added: explicit UFraction16 edge tests around 1x boundaries
				static inline bool TestUFraction16Edges()
				{
					Serial.println(F("Starting ufraction16_t edge tests..."));
					uint32_t errorCount = 0;

					const uint16_t edgeScalars[] = { 0, 1, 16383, 16384, 32767, 32768, 65535 };
					const uint16_t edgeValues[] = { 0, 1, 127, 128, 255, 256, 32767, 32768, 65535 };

					for (uint16_t scalar : edgeScalars)
					{
						for (uint16_t value : edgeValues)
						{
							const uint16_t result = UFraction16::Fraction((UFraction16::scalar_t)scalar, value);
							const uint16_t refResult = RefUFraction16(value, (UFraction16::scalar_t)scalar);
							if (result != refResult)
							{
								Serial.print(F("UFraction16 edge error: value="));
								Serial.print(value);
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

					if (errorCount == 0)
						Serial.println(F("All ufraction16_t edge tests PASSED."));
					else
					{
						Serial.print(F("ufraction16_t edge tests FAILED. Errors: "));
						Serial.println(errorCount);
					}
					return errorCount == 0;
				}

				// Sampled test for UFraction32 (edge cases + large prime stepping).
				template<uint32_t MaxIterations = 50000>
				static inline bool TestUFraction32Sample()
				{
					Serial.println(F("Starting sampled UFraction32 tests..."));
					uint32_t errorCount = 0;

					// Edge coverage around 1x and extremes
					const uint32_t edgeScalars[] = { 0u, 1u, 1073741823u, 1073741824u, 2147483647u, 2147483648u, 4294967295u };
					const uint32_t edgeValues[] = { 0u, 1u, 127u, 128u, 255u, 256u, 65535u, 65536u, 16777215u, 16777216u, 2147483647u, 2147483648u };

					for (uint32_t scalar : edgeScalars)
					{
						for (uint32_t value : edgeValues)
						{
							const uint32_t result = UFraction32::Fraction((UFraction32::scalar_t)scalar, value);
							const uint32_t ref = RefUFraction32(value, (UFraction32::scalar_t)scalar);
							if (result != ref)
							{
								Serial.print(F("UFraction32 edge error: value="));
								Serial.print(value);
								Serial.print(F(" scalar="));
								Serial.print(scalar);
								Serial.print(F(" result="));
								Serial.print(result);
								Serial.print(F(" ref="));
								Serial.println(ref);
								errorCount++;
							}
						}
					}

					// Sample the 32-bit scalar space with a large prime step and a few value subsets
					for (uint64_t s = 0; s <= UINT32_MAX; s += 179424673ULL)
					{
						const uint32_t scalar = (uint32_t)s;

						// Light sweep of small values
						for (uint32_t v = 0; v <= UINT8_MAX; v += 16)
						{
							const uint32_t value = (uint32_t)v;
							const uint32_t result = UFraction32::Fraction((UFraction32::scalar_t)scalar, value);
							const uint32_t ref = RefUFraction32(value, (UFraction32::scalar_t)scalar);
							if (result != ref)
							{
								Serial.print(F("UFraction32 error: value="));
								Serial.print(value);
								Serial.print(F(" scalar="));
								Serial.print(scalar);
								Serial.print(F(" result="));
								Serial.print(result);
								Serial.print(F(" ref="));
								Serial.println(ref);
								errorCount++;
							}
						}

						// A few large values
						const uint32_t largeValues[] = { 65535u, 16777215u, 2147483647u };
						for (uint32_t value : largeValues)
						{
							const uint32_t result = UFraction32::Fraction((UFraction32::scalar_t)scalar, value);
							const uint32_t ref = RefUFraction32(value, (UFraction32::scalar_t)scalar);
							if (result != ref)
							{
								Serial.print(F("UFraction32 large value error: value="));
								Serial.print(value);
								Serial.print(F(" scalar="));
								Serial.print(scalar);
								Serial.print(F(" result="));
								Serial.print(result);
								Serial.print(F(" ref="));
								Serial.println(ref);
								errorCount++;
							}
						}

						if ((s % 536870912ULL) == 0)
						{
							Serial.print(F("UFraction32 progress: "));
							Serial.print((uint32_t)(s >> 30));
							Serial.println(F("/4 completed..."));
						}
					}

					if (errorCount == 0)
						Serial.println(F("All UFraction32 tests PASSED."));
					else
					{
						Serial.print(F("UFraction32 tests FAILED. Errors: "));
						Serial.println(errorCount);
					}
					return errorCount == 0;
				}

				// Test UFraction8 with different input types
				static inline bool TestUFraction8Types()
				{
					Serial.println(F("Testing UFraction8 with different input types..."));
					uint32_t errorCount = 0;

					// Test with uint8_t
					{
						const uint8_t value = 127;
						const uint8_t scalar = 64;
						const uint8_t result = UFraction8::Fraction(scalar, value);
						const uint8_t refResult = RefUFraction8(value, scalar);
						if (result != refResult)
						{
							Serial.println(F("UFraction8 with uint8_t failed"));
							errorCount++;
						}
					}

					// Test with uint16_t
					{
						const uint16_t value = 300;
						const uint8_t scalar = 64;
						const uint16_t result = UFraction8::Fraction(scalar, value);
						const uint16_t refResult = RefUFraction8(value, scalar);
						if (result != refResult)
						{
							Serial.println(F("UFraction8 with uint16_t failed"));
							errorCount++;
						}
					}

					// Test with uint32_t
					{
						const uint32_t value = 70000;
						const uint8_t scalar = 64;
						const uint32_t result = UFraction8::Fraction(scalar, value);
						const uint32_t refResult = RefUFraction8(value, scalar);
						if (result != refResult)
						{
							Serial.println(F("UFraction8 with uint32_t failed"));
							errorCount++;
						}
					}

					if (errorCount == 0)
						Serial.println(F("All UFraction8 type tests PASSED."));
					else
					{
						Serial.print(F("UFraction8 type tests FAILED. Errors: "));
						Serial.println(errorCount);
					}
					return errorCount == 0;
				}
			}
		}
	}
}

#endif
