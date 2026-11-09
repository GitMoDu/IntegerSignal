#ifndef _FIXED_POINT_FRACTION_SIGNEDTEST_h
#define _FIXED_POINT_FRACTION_SIGNEDTEST_h

#include "FixedPointFractionReferenceTest.h"

namespace IntegerSignal
{
	namespace FixedPoint
	{
		namespace ScalarFraction
		{
			namespace Test
			{
				// Exhaustive test for Fraction8::scalar_t
				static inline bool TestFraction8Exhaustive()
				{
					Serial.println(F("Starting exhaustive fraction8_t tests..."));

					uint32_t errorCount = 0;
					for (uint16_t v = 0; v <= UINT8_MAX; v++)
					{
						for (uint16_t f = 1; f <= UINT8_MAX; f++)
						{
							const int8_t value = (uint8_t)v;
							const Fraction8::scalar_t fraction8 = (Fraction8::scalar_t)MaxValue<int8_t>(Fraction8::FRACTION_1X_NEGATIVE, MinValue<int8_t>(Fraction8::FRACTION_1X, (int8_t)f));
							const int8_t result = Fraction8::Fraction(fraction8, value);
							const int8_t refResult = RefFraction8(value, fraction8);
							if (result != refResult)
							{
								Serial.print(F("fraction8_t error: value="));
								Serial.print(value);
								Serial.print(F(" fraction="));
								Serial.print(fraction8);
								Serial.print(F(" result="));
								Serial.print(result);
								Serial.print(F(" ref="));
								Serial.println(refResult);
								errorCount++;
							}
						}
						if ((v % 16) == 0)
						{
							Serial.print(F("fraction8_t progress: "));
							Serial.print(v);
							Serial.println(F(" values tested..."));
						}
					}
					if (errorCount == 0)
					{
						Serial.println(F("All fraction8_t tests PASSED."));
					}
					else
					{
						Serial.print(F("fraction8_t tests FAILED. Errors: "));
						Serial.println(errorCount);
					}

					return errorCount == 0;
				}

				// Exhaustive test for Fraction16::scalar_t
				static inline bool TestFraction16Exhaustive()
				{
					Serial.println(F("Starting exhaustive fraction16_t tests..."));

					uint32_t errorCount = 0;
					for (uint32_t v = 0; v <= UINT16_MAX; v++)
					{
						for (uint32_t f = 1; f <= UINT16_MAX; f++)
						{
							const int16_t value = (uint16_t)v;
							const Fraction16::scalar_t fraction16 = (Fraction16::scalar_t)MaxValue<int32_t>(Fraction16::FRACTION_1X_NEGATIVE, MinValue<int32_t>(Fraction16::FRACTION_1X, (int32_t)f));
							const int16_t result = Fraction16::Fraction(fraction16, value);
							const int16_t refResult = RefFraction16(value, fraction16);
							if (result != refResult)
							{
								Serial.print(F("fraction16_t error: value="));
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
							Serial.print(F("fraction16_t progress: "));
							Serial.print(v);
							Serial.println(F(" values tested..."));
						}
					}
					if (errorCount == 0)
					{
						Serial.println(F("All fraction16_t tests PASSED."));
					}
					else
					{
						Serial.print(F("fraction16_t tests FAILED. Errors: "));
						Serial.println(errorCount);
					}

					return errorCount == 0;
				}

				// Sampled test for Fraction16 for uint8_t, uint16_t, and uint32_t.
				template<uint32_t MaxIterations = 50000>
				static inline bool TestFraction16Sample()
				{
					constexpr uint32_t iterations = MinValue<uint32_t>(MaxIterations, (uint32_t)UINT16_MAX);
					const uint32_t fracSamples = MinValue<uint32_t>(iterations, 1024u);
					const uint32_t fracStep = MaxValue<uint32_t>(1u, (uint32_t)UINT16_MAX / fracSamples);
					const uint32_t valStep = MaxValue<uint32_t>(1u, iterations / 256u);

					Serial.println(F("Starting sampled fraction16_t tests..."));

					uint32_t errorCount = 0;

					for (uint32_t f = 0; f <= UINT16_MAX; f += fracStep)
					{
						const Fraction16::scalar_t fraction16 =
							(Fraction16::scalar_t)MaxValue<int32_t>(Fraction16::FRACTION_1X_NEGATIVE,
								MinValue<int32_t>(Fraction16::FRACTION_1X, (int32_t)f));

						for (uint32_t i = 0; i <= iterations; i += valStep)
						{
							const int32_t value32 = (uint32_t)(((uint64_t)i * UINT32_MAX) / (iterations ? (iterations - 1) : 1));
							const int16_t value16 = value32 / INT16_MAX;
							const int8_t  value8 = value16 / INT8_MAX;

							const int8_t  result8 = Fraction16::Fraction(fraction16, value8);
							const int8_t  ref8 = RefFraction16(value8, fraction16);

							const int16_t result16 = Fraction16::Fraction(fraction16, value16);
							const int16_t ref16 = RefFraction16(value16, fraction16);

							const int32_t result32 = Fraction16::Fraction(fraction16, value32);
							const int32_t ref32 = RefFraction16(value32, fraction16);

							if (result8 != ref8 || result16 != ref16 || result32 != ref32)
							{
								Serial.print(F("fraction16_t error: fraction="));
								Serial.print(fraction16);
								if (result8 != ref8) { Serial.print(F(" r8="));  Serial.print(result8);  Serial.print(F(" ref=")); Serial.println(ref8); }
								if (result16 != ref16) { Serial.print(F(" r16=")); Serial.print(result16); Serial.print(F(" ref=")); Serial.println(ref16); }
								if (result32 != ref32) { Serial.print(F(" r32=")); Serial.print(result32); Serial.print(F(" ref=")); Serial.println(ref32); }
								errorCount++;
								break;
							}
						}

						if ((f % (uint32_t)(UINT16_MAX / 32u)) == 0)
						{
							Serial.print(F("fraction16_t progress: f="));
							Serial.println(f);
						}
					}

					if (errorCount == 0) Serial.println(F("All fraction16_t sampled tests PASSED."));
					else {
						Serial.print(F("fraction16_t sampled tests FAILED. Errors: "));
						Serial.println(errorCount);
					}

					return errorCount == 0;
				}

				// Sampled test for Fraction32 for uint8_t, uint16_t, and uint32_t.
				template<uint32_t MaxIterations = 50000>
				static inline bool TestFraction32Sample()
				{
					const uint32_t iterations = MaxIterations; // already a 32-bit domain
					const uint32_t fracSamples = MinValue<uint32_t>(iterations, 4096u);
					const uint32_t fracStep = MaxValue<uint32_t>(1u, (uint32_t)UINT16_MAX / fracSamples);
					const uint32_t valStep = MaxValue<uint32_t>(1u, iterations / 256u);

					Serial.println(F("Starting sampled fraction32_t tests..."));

					uint32_t errorCount = 0;

					for (uint32_t f = 0; f <= UINT16_MAX; f += fracStep)
					{
						// Map 16-bit sweep to full 32-bit scalar domain
						const uint32_t frac32map = (uint32_t)(((uint64_t)f * UINT32_MAX) / UINT16_MAX);
						const Fraction32::scalar_t fraction32 =
							(Fraction32::scalar_t)MaxValue<int64_t>(Fraction32::FRACTION_1X_NEGATIVE,
								MinValue<int64_t>(Fraction32::FRACTION_1X, (int64_t)frac32map));

						for (uint32_t i = 0; i <= iterations; i += valStep)
						{
							const int32_t value32 = (uint32_t)(((uint64_t)i * UINT32_MAX) / (iterations ? (iterations - 1) : 1));
							const int16_t value16 = value32 / INT16_MAX;
							const int8_t  value8 = value16 / INT8_MAX;

							const int8_t  r8 = Fraction32::Fraction(fraction32, value8);
							const int8_t  ref8 = RefFraction32(value8, fraction32);

							const int16_t r16 = Fraction32::Fraction(fraction32, value16);
							const int16_t ref16 = RefFraction32(value16, fraction32);

							const int32_t r32 = Fraction32::Fraction(fraction32, value32);
							const int32_t ref32 = RefFraction32(value32, fraction32);

							if (r8 != ref8 || r16 != ref16 || r32 != ref32)
							{
								Serial.print(F("fraction32_t error: fraction="));
								Serial.print(fraction32);
								if (r8 != ref8) { Serial.print(F(" r8="));  Serial.print(r8);  Serial.print(F(" ref=")); Serial.println(ref8); }
								if (r16 != ref16) { Serial.print(F(" r16=")); Serial.print(r16); Serial.print(F(" ref=")); Serial.println(ref16); }
								if (r32 != ref32) { Serial.print(F(" r32=")); Serial.print(r32); Serial.print(F(" ref=")); Serial.println(ref32); }
								errorCount++;
								break;
							}
						}

						if ((f % (uint32_t)(UINT16_MAX / 32u)) == 0)
						{
							Serial.print(F("fraction32_t progress: f="));
							Serial.println(f);
						}
					}

					if (errorCount == 0) Serial.println(F("All fraction32_t sampled tests PASSED."));
					else {
						Serial.print(F("fraction32_t sampled tests FAILED. Errors: "));
						Serial.println(errorCount);
					}

					return errorCount == 0;
				}

				// Sampled test for Fraction8 for uint8_t, uint16_t, and uint32_t.
				template<uint32_t MaxIterations = 50000>
				static inline bool TestFraction8Sample()
				{
					const uint32_t iterations = min(MaxIterations, (uint32_t)UINT16_MAX);

					Serial.println(F("Starting sampled fraction8_t tests..."));

					uint32_t errorCount = 0;

					for (uint16_t f = 1; f <= UINT8_MAX; f++)
					{
						const Fraction8::scalar_t fraction8 = (Fraction8::scalar_t)MaxValue<int8_t>(Fraction8::FRACTION_1X_NEGATIVE, MinValue<int8_t>(Fraction8::FRACTION_1X, (int8_t)f));

						for (uint32_t i = 0; i <= iterations; i++)
						{
							const int32_t value32 = (uint32_t)(((uint64_t)i * UINT32_MAX) / (iterations - 1));
							const int16_t value16 = value32 / INT16_MAX;
							const int8_t value8 = value16 / INT8_MAX;

							const int8_t result8 = Fraction8::Fraction(fraction8, value8);
							const int8_t refResult8 = RefFraction8(value8, fraction8);

							const int16_t result16 = Fraction8::Fraction(fraction8, value16);
							const int16_t refResult16 = RefFraction8(value16, fraction8);

							const int32_t result32 = Fraction8::Fraction(fraction8, value32);
							const int32_t refResult32 = RefFraction8(value32, fraction8);

							if (result8 != refResult8
								|| result16 != refResult16
								|| result32 != refResult32
								)
							{
								Serial.print(F("fraction8_t error: fraction="));
								Serial.print(fraction8);
								Serial.print(F(" value="));
								if (result8 != refResult8)
								{
									Serial.print(F(" result="));
									Serial.print(result8);
									Serial.print(F(" ref="));
									Serial.println(refResult8);
								}
								if (result16 != refResult16)
								{
									Serial.print(F(" result="));
									Serial.print(result16);
									Serial.print(F(" ref="));
									Serial.println(refResult16);
								}
								if (result32 != refResult32)
								{
									Serial.print(F(" result="));
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
							Serial.print(F("fraction8_t progress: "));
							Serial.print(f);
							Serial.println(F(" values tested..."));
						}
					}

					if (errorCount == 0)
					{
						Serial.println(F("All fraction8_t tests PASSED."));
					}
					else
					{
						Serial.print(F("fraction8_t tests FAILED. Errors: "));
						Serial.println(errorCount);
					}

					return errorCount == 0;
				}

				// Test SFraction8 with key edge cases
				static inline bool TestSFraction8EdgeCases()
				{
					Serial.println(F("Testing SFraction8 edge cases..."));
					uint32_t errorCount = 0;

					const int8_t testScalars[] = { -64, -63, -32, -1, 0, 1, 32, 63, 64 };
					const int8_t testValues[] = { -128, -127, -64, -1, 0, 1, 64, 127 };

					for (int8_t scalar : testScalars)
					{
						for (int8_t value : testValues)
						{
							const int8_t result = Fraction8::Fraction(scalar, value);
							const int8_t refResult = RefFraction8(value, scalar);

							if (result != refResult)
							{
								Serial.print(F("SFraction8 error: value="));
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
						Serial.println(F("All SFraction8 edge case tests PASSED."));
					else
					{
						Serial.print(F("SFraction8 edge case tests FAILED. Errors: "));
						Serial.println(errorCount);
					}
					return errorCount == 0;
				}
			}
		}
	}
}

#endif
