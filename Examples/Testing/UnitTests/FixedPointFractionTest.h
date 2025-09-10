#ifndef _FIXED_POINT_FRACTION_TEST_h
#define _FIXED_POINT_FRACTION_TEST_h

#include <IntegerSignalTesting.h>

namespace IntegerSignal
{
	namespace FixedPoint
	{
		namespace Fraction
		{
			namespace Test
			{
				// Reference function for Fraction8::scalar_t (signed)
				template<typename T>
				static T RefFraction8(const T value, const Fraction8::scalar_t fraction) {
					return (static_cast<int64_t>(value) * fraction) / Fraction8::FRACTION_1X;
				}

				template<typename T>
				static T RefFraction16(const T value, const Fraction16::scalar_t fraction) {
					return (static_cast<int64_t>(value) * fraction) / Fraction16::FRACTION_1X;
				}

				template<typename T>
				static T RefFraction32(const T value, const Fraction32::scalar_t fraction) {
					return (static_cast<int64_t>(value) * fraction) / Fraction32::FRACTION_1X;
				}

				// Reference function for UFractionX::scalar_t (unsigned)
				template<typename T>
				static T RefUFraction8(const T value, const UFraction8::scalar_t fraction) {
					return (static_cast<uint64_t>(value) * fraction) / UFraction8::FRACTION_1X;
				}

				template<typename T>
				static T RefUFraction16(const T value, const UFraction16::scalar_t fraction) {
					return (static_cast<uint64_t>(value) * fraction) / UFraction16::FRACTION_1X;
				}

				template<typename T>
				static T RefUFraction32(const T value, const UFraction32::scalar_t fraction) {
					return (static_cast<uint64_t>(value) * fraction) / UFraction32::FRACTION_1X;
				}

				// Reference function for GetScalar (signed Fraction8)
				static Fraction8::scalar_t RefGetFraction8(uint8_t numerator, uint8_t denominator)
				{
					const int16_t scaled = (denominator == 0) ? 0
						: (int16_t)(((int16_t)numerator * Fraction8::FRACTION_1X) / denominator);
					return (Fraction8::scalar_t)min<int16_t>(
						Fraction8::FRACTION_1X,
						max<int16_t>(Fraction8::FRACTION_1X_NEGATIVE, scaled));
				}

				// Reference function for GetScalar (signed Fraction16)
				static Fraction16::scalar_t RefGetFraction16(uint8_t numerator, uint8_t denominator)
				{
					const int32_t scaled = (denominator == 0) ? 0
						: (int32_t)(((int32_t)numerator * Fraction16::FRACTION_1X) / denominator);
					return (Fraction16::scalar_t)min<int32_t>(
						Fraction16::FRACTION_1X,
						max<int32_t>(Fraction16::FRACTION_1X_NEGATIVE, scaled));
				}

				// Reference function for GetScalar (signed Fraction32)
				static Fraction32::scalar_t RefGetFraction32(uint8_t numerator, uint8_t denominator)
				{
					const int64_t scaled = (denominator == 0) ? 0
						: (int64_t)(((int64_t)numerator * Fraction32::FRACTION_1X) / denominator);
					return (Fraction32::scalar_t)min<int64_t>(
						Fraction32::FRACTION_1X,
						max<int64_t>(Fraction32::FRACTION_1X_NEGATIVE, scaled));
				}

				// Reference interpolation functions
				template<typename T>
				static T RefUInterpolate8(const T from, const T to, const uint8_t scalar)
				{
					constexpr uint8_t SCALE_UNIT = 128;
					return (static_cast<uint64_t>(from) * (SCALE_UNIT - scalar) +
						static_cast<uint64_t>(to) * scalar) / SCALE_UNIT;
				}

				// Exhaustive test for Fraction8::scalar_t
				static bool TestFraction8Exhaustive()
				{
					Serial.println(F("Starting exhaustive fraction8_t tests..."));

					uint32_t errorCount = 0;
					for (uint16_t v = 0; v <= UINT8_MAX; v++)
					{
						for (uint16_t f = 1; f <= UINT8_MAX; f++)
						{
							const int8_t value = (uint8_t)v;
							const Fraction8::scalar_t fraction8 = (Fraction8::scalar_t)max<int8_t>(Fraction8::FRACTION_1X_NEGATIVE, min<int8_t>(Fraction8::FRACTION_1X, (int8_t)f));
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
				static bool TestFraction16Exhaustive()
				{
					Serial.println(F("Starting exhaustive fraction16_t tests..."));

					uint32_t errorCount = 0;
					for (uint32_t v = 0; v <= UINT16_MAX; v++)
					{
						for (uint32_t f = 1; f <= UINT16_MAX; f++)
						{
							const int16_t value = (uint16_t)v;
							const Fraction16::scalar_t fraction16 = (Fraction16::scalar_t)max<int32_t>(Fraction16::FRACTION_1X_NEGATIVE, min<int32_t>(Fraction16::FRACTION_1X, (int32_t)f));
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
				static bool TestFraction16Sample()
				{
					const uint32_t iterations = min(MaxIterations, (uint32_t)UINT16_MAX);
					const uint32_t fracSamples = min<uint32_t>(iterations, 1024u);
					const uint32_t fracStep = max<uint32_t>(1u, (uint32_t)UINT16_MAX / fracSamples);
					const uint32_t valStep = max<uint32_t>(1u, iterations / 256u);

					Serial.println(F("Starting sampled fraction16_t tests..."));

					uint32_t errorCount = 0;

					for (uint32_t f = 0; f <= UINT16_MAX; f += fracStep)
					{
						const Fraction16::scalar_t fraction16 =
							(Fraction16::scalar_t)max<int32_t>(Fraction16::FRACTION_1X_NEGATIVE,
								min<int32_t>(Fraction16::FRACTION_1X, (int32_t)f));

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

				// Sampled test for UFraction16 for uint8_t, uint16_t, and uint32_t.
				template<uint32_t MaxIterations = 50000>
				static bool TestUFraction16Sample()
				{
					const uint32_t iterations = min(MaxIterations, (uint32_t)UINT16_MAX);
					const uint32_t fracSamples = min<uint32_t>(iterations, 1024u);
					const uint32_t fracStep = max<uint32_t>(1u, (uint32_t)UINT16_MAX / fracSamples);
					const uint32_t valStep = max<uint32_t>(1u, iterations / 256u);

					Serial.println(F("Starting sampled ufraction16_t tests..."));

					uint32_t errorCount = 0;

					for (uint32_t f = 0; f <= UINT16_MAX; f += fracStep)
					{
						const UFraction16::scalar_t fraction16 =
							(UFraction16::scalar_t)min<int32_t>(UFraction16::FRACTION_1X, (int32_t)f);

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

				// Sampled test for Fraction32 for uint8_t, uint16_t, and uint32_t.
				template<uint32_t MaxIterations = 50000>
				static bool TestFraction32Sample()
				{
					const uint32_t iterations = MaxIterations; // already a 32-bit domain
					const uint32_t fracSamples = min<uint32_t>(iterations, 4096u);
					const uint32_t fracStep = max<uint32_t>(1u, (uint32_t)UINT16_MAX / fracSamples);
					const uint32_t valStep = max<uint32_t>(1u, iterations / 256u);

					Serial.println(F("Starting sampled fraction32_t tests..."));

					uint32_t errorCount = 0;

					for (uint32_t f = 0; f <= UINT16_MAX; f += fracStep)
					{
						// Map 16-bit sweep to full 32-bit scalar domain
						const uint32_t frac32map = (uint32_t)(((uint64_t)f * UINT32_MAX) / UINT16_MAX);
						const Fraction32::scalar_t fraction32 =
							(Fraction32::scalar_t)max<int64_t>(Fraction32::FRACTION_1X_NEGATIVE,
								min<int64_t>(Fraction32::FRACTION_1X, (int64_t)frac32map));

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
				static bool TestFraction8Sample()
				{
					const uint32_t iterations = min(MaxIterations, (uint32_t)UINT16_MAX);

					Serial.println(F("Starting sampled fraction8_t tests..."));

					uint32_t errorCount = 0;

					for (uint16_t f = 1; f <= UINT8_MAX; f++)
					{
						const Fraction8::scalar_t fraction8 = (Fraction8::scalar_t)max<int8_t>(Fraction8::FRACTION_1X_NEGATIVE, min<int8_t>(Fraction8::FRACTION_1X, (int8_t)f));

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

				// Sampled test for UFraction8 for uint8_t, uint16_t, and uint32_t.
				template<uint32_t MaxIterations = 50000>
				static bool TestUFraction8Sample()
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
				static bool TestUFraction8Exhaustive()
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
				static bool TestUFraction16Exhaustive()
				{
					Serial.println(F("Starting exhaustive ufraction16_t tests..."));

					uint32_t errorCount = 0;
					for (uint32_t v = 0; v <= UINT16_MAX; v++)
					{
						for (uint32_t f = 1; f <= UINT16_MAX; f++)
						{
							const uint16_t value = (uint16_t)v;
							const UFraction16::scalar_t fraction16 = (UFraction16::scalar_t)min<uint16_t>(UINT16_MAX, (uint16_t)f); // allow >1x
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
				static bool TestUFraction16Edges()
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
				static bool TestUFraction32Sample()
				{
					const uint32_t iterations = min(MaxIterations, (uint32_t)UINT16_MAX);
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

				// Exhaustive test for GetScalar (signed Fraction8/16/32) for all 255*255 combinations of numerator/denominator.
				static bool TestGetFraction8Exhaustive()
				{
					Serial.println(F("Starting exhaustive GetFraction8 tests..."));

					uint32_t errorCount = 0;
					for (uint16_t n = 1; n < UINT8_MAX; n++)
					{
						for (uint16_t d = n; d <= UINT8_MAX; d++)
						{
							const uint8_t numerator = (uint8_t)n;
							const uint8_t denominator = (uint8_t)d;

							const Fraction8::scalar_t fraction8 = Fraction8::GetScalar(numerator, denominator);
							const Fraction8::scalar_t reffraction8 = RefGetFraction8(numerator, denominator);

							const Fraction16::scalar_t fraction16 = Fraction16::GetScalar(numerator, denominator);
							const Fraction16::scalar_t reffraction16 = RefGetFraction16(numerator, denominator);

							const Fraction32::scalar_t fraction32 = Fraction32::GetScalar(numerator, denominator);
							const Fraction32::scalar_t reffraction32 = RefGetFraction32(numerator, denominator);

							if (fraction8 != reffraction8
								|| fraction16 != reffraction16
								|| fraction32 != reffraction32
								)
							{
								Serial.print(F("GetFraction8 error: numerator="));
								Serial.print(numerator);
								Serial.print(F(" denominator="));
								Serial.print(denominator);
								if (fraction8 != reffraction8)
								{
									Serial.print(F(" fraction8="));
									Serial.print(fraction8);
									Serial.print(F(" ref="));
									Serial.println(reffraction8);
								}
								if (fraction16 != reffraction16)
								{
									Serial.print(F(" fraction16="));
									Serial.print(fraction16);
									Serial.print(F(" ref="));
									Serial.println(reffraction16);
								}
								if (fraction32 != reffraction32)
								{
									Serial.print(F(" fraction32="));
									Serial.print(fraction32);
									Serial.print(F(" ref="));
									Serial.println(reffraction32);
								}
								errorCount++;
							}
						}
						if ((n % 16) == 0)
						{
							Serial.print(F("GetFraction8 progress: "));
							Serial.print(n);
							Serial.println(F(" numerators tested..."));
						}
					}
					if (errorCount == 0)
					{
						Serial.println(F("All GetFraction8 tests PASSED."));
					}
					else
					{
						Serial.print(F("GetFraction8 tests FAILED. Errors: "));
						Serial.println(errorCount);
					}

					return errorCount == 0;
				}

				// Test SFraction8 with key edge cases
				static bool TestSFraction8EdgeCases()
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

				// Test UInterpolate8 with systematic samples
				static bool TestUInterpolate8()
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

				// Test UFraction8 with different input types
				static bool TestUFraction8Types()
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


				template<uint32_t MaxIterations = 50000, uint8_t maxError32 = 0>
				static bool RunTests()
				{
					bool pass = true;

					pass &= TestGetFraction8Exhaustive();
					pass &= TestUFraction8Types();

					pass &= TestUInterpolate8();
					pass &= TestSFraction8EdgeCases();

					pass &= TestFraction8Exhaustive();
					pass &= TestUFraction8Exhaustive();

					pass &= TestUFraction16Edges();

					pass &= TestFraction8Sample<MaxIterations>();
					pass &= TestUFraction8Sample<MaxIterations>();

					pass &= TestFraction16Sample<MaxIterations>();
					pass &= TestUFraction16Sample<MaxIterations>();

					pass &= TestFraction32Sample<MaxIterations>();
					pass &= TestUFraction32Sample<MaxIterations>();

					if (!pass)
					{
						Serial.println(F("FractionScale tests FAILED."));
					}

					return pass;
				}

				static bool RunExhaustive()
				{
					bool pass = true;
					pass &= TestFraction16Exhaustive();
					pass &= TestUFraction16Exhaustive();

					return pass;
				}
			}
		}
	}
}
#endif