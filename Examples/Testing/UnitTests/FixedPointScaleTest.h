#ifndef _FIXED_POINT_SCALE_TEST_h
#define _FIXED_POINT_SCALE_TEST_h

#include <IntegerSignalTesting.h>

namespace IntegerSignal
{
	namespace FixedPoint
	{
		namespace FactorScale
		{
			namespace Test
			{
				// Reference function for Scale8 (unsigned)
				static uint8_t RefScale8(const uint8_t value, const Scale8::factor_t factor)
				{
					return min((uint16_t)UINT8_MAX, (uint16_t)(((uint16_t)value * factor) / Scale8::SCALE_1X));
				}

				static uint16_t RefScale8(const uint16_t value, const Scale8::factor_t factor)
				{
					return min((uint32_t)UINT16_MAX, (uint32_t)(((uint32_t)value * factor) / Scale8::SCALE_1X));
				}

				static uint32_t RefScale8(const uint32_t value, const Scale8::factor_t factor)
				{
					return min((uint64_t)UINT32_MAX, (uint64_t)(((uint64_t)value * factor) / Scale8::SCALE_1X));
				}

				// Reference function for Scale8 (signed)
				static int8_t RefScale8(const int8_t value, const Scale8::factor_t factor)
				{
					return constrain((int16_t)(((int32_t)value * (int32_t)factor) / (int32_t)Scale8::SCALE_1X), (int16_t)INT8_MIN, (int16_t)INT8_MAX);
				}

				static int16_t RefScale8(const int16_t value, const Scale8::factor_t factor)
				{
					return constrain((int32_t)(((int32_t)value * (int32_t)factor) / (int32_t)Scale8::SCALE_1X), (int32_t)INT16_MIN, (int32_t)INT16_MAX);
				}

				static int32_t RefScale8(const int32_t value, const Scale8::factor_t factor)
				{
					return constrain((int64_t)(((int64_t)value * (int64_t)factor) / (int64_t)Scale8::SCALE_1X), (int64_t)INT32_MIN, (int64_t)INT32_MAX);
				}

				// Reference function for Scale16 (unsigned)
				static uint8_t RefScale16(const uint8_t value, const Scale16::factor_t factor)
				{
					return min((uint32_t)UINT8_MAX, (uint32_t)(((uint32_t)value * factor) / Scale16::SCALE_1X));
				}

				static uint16_t RefScale16(const uint16_t value, const Scale16::factor_t factor)
				{
					return min((uint32_t)UINT16_MAX, (uint32_t)(((uint32_t)value * factor) / Scale16::SCALE_1X));
				}

				static uint32_t RefScale16(const uint32_t value, const Scale16::factor_t factor)
				{
					return min((uint64_t)UINT32_MAX, (uint64_t)(((uint64_t)value * factor) / Scale16::SCALE_1X));
				}

				// Reference function for Scale16 (signed)
				static int8_t RefScale16(const int8_t value, const Scale16::factor_t factor)
				{
					return constrain((int16_t)(((int32_t)value * (int32_t)factor) / (int32_t)Scale16::SCALE_1X), (int16_t)INT8_MIN, (int16_t)INT8_MAX);
				}

				static int16_t RefScale16(const int16_t value, const Scale16::factor_t factor)
				{
					return constrain((int32_t)(((int32_t)value * (int32_t)factor) / (int32_t)Scale16::SCALE_1X), (int32_t)INT16_MIN, (int32_t)INT16_MAX);
				}

				static int32_t RefScale16(const int32_t value, const Scale16::factor_t factor)
				{
					return constrain((int64_t)(((int64_t)value * (int64_t)factor) / (int64_t)Scale16::SCALE_1X), (int64_t)INT32_MIN, (int64_t)INT32_MAX);
				}

				// Reference function for Scale32 (unsigned)
				static uint8_t RefScale32(const uint8_t value, const Scale32::factor_t factor)
				{
					return min((uint16_t)UINT8_MAX, (uint16_t)(((uint64_t)value * (uint64_t)factor) / (uint64_t)Scale32::SCALE_1X));
				}

				static uint16_t RefScale32(const uint16_t value, const Scale32::factor_t factor)
				{
					return min((uint32_t)UINT16_MAX, (uint32_t)(((uint64_t)value * (uint64_t)factor) / (uint64_t)Scale32::SCALE_1X));
				}

				static uint32_t RefScale32(const uint32_t value, const Scale32::factor_t factor)
				{
					return min((uint64_t)UINT32_MAX, (uint64_t)(((uint64_t)value * factor) / Scale32::SCALE_1X));
				}

				// Reference function for Scale32 (signed)
				static int8_t RefScale32(const int8_t value, const Scale32::factor_t factor)
				{
					return constrain((int16_t)(((int32_t)value * (int64_t)factor) / (int64_t)Scale32::SCALE_1X), (int16_t)INT8_MIN, (int16_t)INT8_MAX);
				}

				static int16_t RefScale32(const int16_t value, const Scale32::factor_t factor)
				{
					return constrain((int32_t)(((int64_t)value * (int64_t)factor) / (int64_t)Scale32::SCALE_1X), (int32_t)INT16_MIN, (int32_t)INT16_MAX);
				}

				static int32_t RefScale32(const int32_t value, const Scale32::factor_t factor)
				{
					return constrain((int64_t)(((int64_t)value * (int64_t)factor) / (int64_t)Scale32::SCALE_1X), (int64_t)INT32_MIN, (int64_t)INT32_MAX);
				}

				// Reference function for GetFactor (unsigned semantics)
				static Scale8::factor_t RefGetFactor8(uint8_t numerator, uint8_t denominator)
				{
					return (denominator == 0) ? Scale8::SCALE_1X
						: (numerator > denominator) ? Scale8::SCALE_1X
						: (Scale8::factor_t)(((uint16_t)numerator * Scale8::SCALE_1X) / denominator);
				}

				static Scale16::factor_t RefGetFactor16(uint8_t numerator, uint8_t denominator)
				{
					return (denominator == 0) ? Scale16::SCALE_1X
						: (numerator > denominator) ? Scale16::SCALE_1X
						: (Scale16::factor_t)(((uint32_t)numerator * Scale16::SCALE_1X) / denominator);
				}

				static Scale32::factor_t RefGetFactor32(uint8_t numerator, uint8_t denominator)
				{
					return (denominator == 0) ? Scale32::SCALE_1X
						: (numerator > denominator) ? Scale32::SCALE_1X
						: (Scale32::factor_t)(((uint64_t)numerator * Scale32::SCALE_1X) / denominator);
				}

				// Reference function for GetFactor (signed semantics)
				static Scale8::factor_t RefGetFactor8Signed(int32_t numerator, int32_t denominator)
				{
					if (numerator < 0) return Scale8::SCALE_MIN;
					if (denominator <= 0) return Scale8::SCALE_1X;
					if (numerator > denominator) return Scale8::SCALE_1X;
					return (Scale8::factor_t)(((int32_t)numerator * (int32_t)Scale8::SCALE_1X) / (int32_t)denominator);
				}

				static Scale16::factor_t RefGetFactor16Signed(int32_t numerator, int32_t denominator)
				{
					if (numerator < 0) return Scale16::SCALE_MIN;
					if (denominator <= 0) return Scale16::SCALE_1X;
					if (numerator > denominator) return Scale16::SCALE_1X;
					return (Scale16::factor_t)(((int64_t)numerator * (int64_t)Scale16::SCALE_1X) / (int64_t)denominator);
				}

				static Scale32::factor_t RefGetFactor32Signed(int32_t numerator, int32_t denominator)
				{
					if (numerator < 0) return Scale32::SCALE_MIN;
					if (denominator <= 0) return Scale32::SCALE_1X;
					if (numerator > denominator) return Scale32::SCALE_1X;
					return (Scale32::factor_t)(((int64_t)numerator * (int64_t)Scale32::SCALE_1X) / (int64_t)denominator);
				}

				// Exhaustive test for Scale8::factor_t with uint8_t values
				static bool TestScale8Exhaustive8()
				{
					Serial.println(F("Starting exhaustive Scale8 (8-bit value) tests..."));

					uint32_t errorCount = 0;
					for (uint16_t v = 0; v <= UINT8_MAX; v++)
					{
						for (uint16_t r = 1; r <= UINT8_MAX; r++)
						{
							const uint8_t value = (uint8_t)v;
							const Scale8::factor_t factor = (Scale8::factor_t)r;
							const uint8_t result = Scale8::Scale(factor, value);
							const uint8_t refResult = RefScale8(value, factor);
							if (result != refResult)
							{
								Serial.print(F("Scale8 error: value="));
								Serial.print(value);
								Serial.print(F(" factor="));
								Serial.print(factor);
								Serial.print(F(" result="));
								Serial.print(result);
								Serial.print(F(" ref="));
								Serial.println(refResult);
								errorCount++;
							}
						}
						if ((v % 16) == 0)
						{
							Serial.print(F("Scale8 progress: "));
							Serial.print(v);
							Serial.println(F(" values tested..."));
						}
					}
					if (errorCount == 0)
					{
						Serial.println(F("All Scale8 (8-bit value) tests PASSED."));
					}
					else
					{
						Serial.print(F("Scale8 (8-bit value) tests FAILED. Errors: "));
						Serial.println(errorCount);
					}

					return errorCount == 0;
				}

				// Exhaustive test for Scale16::factor_t with uint8_t values
				static bool TestScale16Exhaustive8()
				{
					Serial.println(F("Starting exhaustive Scale16 (8-bit value) tests..."));

					uint32_t errorCount = 0;
					for (uint16_t v = 0; v <= UINT8_MAX; v++)
					{
						for (uint32_t r = 1; r <= UINT16_MAX; r++)
						{
							const uint8_t value = (uint8_t)v;
							const Scale16::factor_t factor = (Scale16::factor_t)r;
							const uint8_t result = Scale16::Scale(factor, value);
							const uint8_t refResult = RefScale16(value, factor);
							if (result != refResult)
							{
								Serial.print(F("Scale16 error: value="));
								Serial.print(value);
								Serial.print(F(" factor="));
								Serial.print(factor);
								Serial.print(F(" result="));
								Serial.print(result);
								Serial.print(F(" ref="));
								Serial.println(refResult);
								errorCount++;
							}
						}
						if ((v % 16) == 0)
						{
							Serial.print(F("Scale16 progress: "));
							Serial.print(v);
							Serial.println(F(" values tested..."));
						}
					}
					if (errorCount == 0)
					{
						Serial.println(F("All Scale16 (8-bit value) tests PASSED."));
					}
					else
					{
						Serial.print(F("Scale16 (8-bit value) tests FAILED. Errors: "));
						Serial.println(errorCount);
					}

					return errorCount == 0;
				}

				// Signed tests ported from Test2: Scale8
				static bool TestScale8SignedValues()
				{
					Serial.println(F("Testing Scale8 with signed values..."));

					uint32_t errorCount = 0;
					const int8_t testValues[] = { INT8_MIN, -64, -1, 0, 1, 64, INT8_MAX };
					const uint8_t testFactors[] = { 1, 8, 16, Scale8::SCALE_1X, 200, 255 };

					for (int8_t value : testValues)
					{
						for (uint8_t factor : testFactors)
						{
							const int8_t result = Scale8::Scale(factor, value);
							const int8_t refResult = RefScale8(value, factor);

							if (result != refResult)
							{
								Serial.print(F("Scale8 signed error: value="));
								Serial.print(value);
								Serial.print(F(" factor="));
								Serial.print(factor);
								Serial.print(F(" result="));
								Serial.print(result);
								Serial.print(F(" ref="));
								Serial.println(refResult);
								errorCount++;
							}
						}
					}

					if (errorCount == 0)
						Serial.println(F("All Scale8 signed tests PASSED."));
					else
					{
						Serial.print(F("Scale8 signed tests FAILED. Errors: "));
						Serial.println(errorCount);
					}
					return errorCount == 0;
				}

				// Signed tests ported from Test2: Scale16
				static bool TestScale16SignedSample()
				{
					Serial.println(F("Testing Scale16 with signed values..."));

					uint32_t errorCount = 0;
					const uint16_t testFactors[] = { 1, 256, Scale16::SCALE_1X, 40000, 65535 };

					const int32_t steps = 100;
					for (uint16_t factor : testFactors)
					{
						for (int32_t i = 0; i <= steps; i++)
						{
							const int16_t value = (int16_t)((((int32_t)i * INT16_MAX) / steps) - (INT16_MAX / 2));
							const int16_t result = Scale16::Scale(factor, value);
							const int16_t refResult = RefScale16(value, factor);

							if (result != refResult)
							{
								Serial.print(F("Scale16 signed error: value="));
								Serial.print(value);
								Serial.print(F(" factor="));
								Serial.print(factor);
								Serial.print(F(" result="));
								Serial.print(result);
								Serial.print(F(" ref="));
								Serial.println(refResult);
								errorCount++;
							}
						}
					}

					if (errorCount == 0)
						Serial.println(F("All Scale16 signed tests PASSED."));
					else
					{
						Serial.print(F("Scale16 signed tests FAILED. Errors: "));
						Serial.println(errorCount);
					}
					return errorCount == 0;
				}

				// Sampled test for Scale8 with uint8_t, uint16_t and uint32_t values
				template<uint32_t MaxIterations = 50000>
				static bool TestScale8Sample()
				{
					const uint32_t iterations = min(MaxIterations, (uint32_t)UINT16_MAX);

					Serial.println(F("Starting sampled Scale8 tests..."));

					uint32_t errorCount = 0;

					for (uint16_t r = 1; r <= UINT8_MAX; r++)
					{
						const Scale8::factor_t factor8 = (Scale8::factor_t)r;

						for (uint32_t i = 0; i <= iterations; i++)
						{
							const uint32_t value32 = (uint32_t)(((uint64_t)i * UINT32_MAX) / (iterations - 1));
							const uint16_t value16 = value32 >> 16;
							const uint8_t value8 = value16 >> 8;

							const uint8_t result8 = Scale8::Scale(factor8, value8);
							const uint8_t refResult8 = RefScale8(value8, factor8);

							const uint16_t result16 = Scale8::Scale(factor8, value16);
							const uint16_t refResult16 = RefScale8(value16, factor8);

							const uint32_t result32 = Scale8::Scale(factor8, value32);
							const uint32_t refResult32 = RefScale8(value32, factor8);

							if (result8 != refResult8
								|| result16 != refResult16
								|| result32 != refResult32)
							{
								Serial.print(F("Scale8 error: factor="));
								Serial.print(factor8);
								Serial.print(F(" value="));
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
								}
								errorCount++;
							}
						}

						if ((r % 16) == 0)
						{
							Serial.print(F("Scale8 progress: "));
							Serial.print(r);
							Serial.println(F(" factors tested..."));
						}
					}

					if (errorCount == 0)
					{
						Serial.println(F("All Scale8 tests PASSED."));
					}
					else
					{
						Serial.print(F("Scale8 tests FAILED. Errors: "));
						Serial.println(errorCount);
					}

					return errorCount == 0;
				}

				// Sampled test for Scale16 with uint8_t, uint16_t, and uint32_t values
				template<uint32_t MaxIterations = 50000>
				static bool TestScale16Sample()
				{
					const uint32_t iterations = min(MaxIterations, (uint32_t)UINT16_MAX);

					Serial.println(F("Starting sampled Scale16 tests..."));

					uint32_t errorCount = 0;

					for (uint32_t r = 1; r <= iterations; r++)
					{
						const Scale16::factor_t factor16 = (uint32_t)(((uint64_t)r * UINT16_MAX) / (iterations - 1));

						for (uint32_t i = 0; i <= iterations; i++)
						{
							const uint32_t value32 = (uint32_t)(((uint64_t)i * UINT32_MAX) / (iterations - 1));
							const uint16_t value16 = value32 >> 16;
							const uint8_t value8 = value16 >> 8;

							const uint8_t result8 = Scale16::Scale(factor16, value8);
							const uint8_t refResult8 = RefScale16(value8, factor16);

							const uint16_t result16 = Scale16::Scale(factor16, value16);
							const uint16_t refResult16 = RefScale16(value16, factor16);

							const uint32_t result32 = Scale16::Scale(factor16, value32);
							const uint32_t refResult32 = RefScale16(value32, factor16);

							if (result8 != refResult8 || result16 != refResult16 || result32 != refResult32)
							{
								Serial.print(F("Scale16 error: factor="));
								Serial.print(factor16);
								Serial.print(F(" value="));
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
								}
								errorCount++;
							}
						}

						if ((r % 256) == 0)
						{
							Serial.print(F("Scale16 progress: "));
							Serial.print(r);
							Serial.println(F(" factors tested..."));
						}
					}

					if (errorCount == 0)
					{
						Serial.println(F("All Scale16 tests PASSED."));
					}
					else
					{
						Serial.print(F("Scale16 tests FAILED. Errors: "));
						Serial.println(errorCount);
					}

					return errorCount == 0;
				}

				// Sampled tests for Scale32 (unsigned and signed) - ported from Test2
				template<uint32_t MaxIterations = 1000>
				static bool TestScale32Sample()
				{
					const uint32_t iterations = min(MaxIterations, (uint32_t)1000);
					Serial.println(F("Starting sampled Scale32 tests..."));

					uint32_t errorCount = 0;

					const uint32_t testFactors[] = { 1u, 256u, Scale32::SCALE_1X, 1000000000u, UINT32_MAX };

					for (uint32_t factor : testFactors)
					{
						for (uint32_t i = 0; i < iterations; i += MaxValue<uint32_t>(1, iterations / 50))
						{
							const uint32_t value32 = (uint32_t)((uint64_t)i * (UINT32_MAX / iterations));
							const uint16_t value16 = (uint16_t)((uint32_t)i * (UINT16_MAX / iterations));
							const uint8_t value8 = (uint8_t)((uint32_t)i * (UINT8_MAX / iterations));

							const uint8_t result8 = Scale32::Scale(factor, value8);
							const uint8_t refResult8 = RefScale32(value8, factor);
							const uint16_t result16 = Scale32::Scale(factor, value16);
							const uint16_t refResult16 = RefScale32(value16, factor);
							const uint32_t result32 = Scale32::Scale(factor, value32);
							const uint32_t refResult32 = RefScale32(value32, factor);

							if (result8 != refResult8 || result16 != refResult16 || result32 != refResult32)
							{
								Serial.print(F("Scale32 error: factor="));
								Serial.print(factor);
								if (result8 != refResult8)
								{
									Serial.print(F(" value8="));
									Serial.print(value8);
									Serial.print(F(" result8="));
									Serial.print(result8);
									Serial.print(F(" ref8="));
									Serial.print(refResult8);
								}
								if (result16 != refResult16)
								{
									Serial.print(F(" value16="));
									Serial.print(value16);
									Serial.print(F(" result16="));
									Serial.print(result16);
									Serial.print(F(" ref16="));
									Serial.print(refResult16);
								}
								if (result32 != refResult32)
								{
									Serial.print(F(" value32="));
									Serial.print(value32);
									Serial.print(F(" result32="));
									Serial.print(result32);
									Serial.print(F(" ref32="));
									Serial.print(refResult32);
								}
								Serial.println();
								errorCount++;
							}

							// Signed path
							const int32_t signedValue = (int32_t)((int64_t)i * (INT32_MAX / iterations) - (INT32_MAX / 2));
							{
								const int32_t sResult = Scale32::Scale(factor, signedValue);
								const int32_t sRef = RefScale32(signedValue, factor);
								if (sResult != sRef)
								{
									Serial.print(F("Scale32 signed error: factor="));
									Serial.print(factor);
									Serial.print(F(" value="));
									Serial.print(signedValue);
									Serial.print(F(" result="));
									Serial.print(sResult);
									Serial.print(F(" ref="));
									Serial.println(sRef);
									errorCount++;
								}
							}
						}
						Serial.print(F("Scale32 factor "));
						Serial.print(factor);
						Serial.println(F(" tested"));
					}

					if (errorCount == 0)
						Serial.println(F("All Scale32 tests PASSED."));
					else
					{
						Serial.print(F("Scale32 tests FAILED. Errors: "));
						Serial.println(errorCount);
					}
					return errorCount == 0;
				}

				// GetFactor tests with signed semantics (ported)
				static bool TestScale8GetFactorSigned()
				{
					Serial.println(F("Testing Scale8 GetFactor (signed inputs)..."));
					uint32_t errorCount = 0;

					const int32_t nums[] = { -10, 0, 1, 10, 50, 100, 200 };
					const int32_t dens[] = { -10, 0, 1, 10, 50, 100, 200 };

					for (int32_t n : nums)
					{
						for (int32_t d : dens)
						{
							const uint8_t res = Scale8::GetFactor(n, d);
							const uint8_t ref = RefGetFactor8Signed(n, d);
							if (res != ref)
							{
								Serial.print(F("Scale8 GetFactor(signed) error: num="));
								Serial.print(n);
								Serial.print(F(" denom="));
								Serial.print(d);
								Serial.print(F(" result="));
								Serial.print(res);
								Serial.print(F(" ref="));
								Serial.println(ref);
								errorCount++;
							}
						}
					}

					if (errorCount == 0) Serial.println(F("All Scale8 GetFactor(signed) tests PASSED."));
					else { Serial.print(F("Scale8 GetFactor(signed) tests FAILED. Errors: ")); Serial.println(errorCount); }
					return errorCount == 0;
				}

				static bool TestScale16GetFactorSigned()
				{
					Serial.println(F("Testing Scale16 GetFactor (signed inputs)..."));
					uint32_t errorCount = 0;

					const int32_t nums[] = { -100, 0, 1, 100, 500, 1000, 2000 };
					const int32_t dens[] = { -100, 0, 1, 100, 500, 1000, 2000 };

					for (int32_t n : nums)
					{
						for (int32_t d : dens)
						{
							const uint16_t res = Scale16::GetFactor(n, d);
							const uint16_t ref = RefGetFactor16Signed(n, d);
							if (res != ref)
							{
								Serial.print(F("Scale16 GetFactor(signed) error: num="));
								Serial.print(n);
								Serial.print(F(" denom="));
								Serial.print(d);
								Serial.print(F(" result="));
								Serial.print(res);
								Serial.print(F(" ref="));
								Serial.println(ref);
								errorCount++;
							}
						}
					}

					if (errorCount == 0) Serial.println(F("All Scale16 GetFactor(signed) tests PASSED."));
					else { Serial.print(F("Scale16 GetFactor(signed) tests FAILED. Errors: ")); Serial.println(errorCount); }
					return errorCount == 0;
				}

				static bool TestScale32GetFactor()
				{
					Serial.println(F("Testing Scale32 GetFactor (signed inputs)..."));
					uint32_t errorCount = 0;

					const int32_t nums[] = { -1000, 0, 1, 1000, 10000, 1000000, INT32_MAX };
					const int32_t dens[] = { -1000, 0, 1, 1000, 10000, 1000000, INT32_MAX };

					for (int32_t n : nums)
					{
						for (int32_t d : dens)
						{
							const uint32_t res = Scale32::GetFactor(n, d);
							const uint32_t ref = RefGetFactor32Signed(n, d);
							if (res != ref)
							{
								Serial.print(F("Scale32 GetFactor error: num="));
								Serial.print(n);
								Serial.print(F(" denom="));
								Serial.print(d);
								Serial.print(F(" result="));
								Serial.print(res);
								Serial.print(F(" ref="));
								Serial.println(ref);
								errorCount++;
							}
						}
					}

					if (errorCount == 0) Serial.println(F("All Scale32 GetFactor tests PASSED."));
					else { Serial.print(F("Scale32 GetFactor tests FAILED. Errors: ")); Serial.println(errorCount); }
					return errorCount == 0;
				}

				// Edge cases for Scale32 (ported)
				static bool TestScale32EdgeCases()
				{
					Serial.println(F("Testing Scale32 edge cases..."));

					uint32_t errorCount = 0;

					const uint32_t scale32Factors[] = { Scale32::SCALE_MIN, Scale32::SCALE_1X, Scale32::SCALE_MAX };
					for (uint32_t factor : scale32Factors)
					{
						const uint32_t testValue = UINT32_MAX;
						const uint32_t result = Scale32::Scale(factor, testValue);
						const uint32_t refResult = RefScale32(testValue, factor);
						if (result != refResult)
						{
							Serial.print(F("Scale32 edge case error: value="));
							Serial.print(testValue);
							Serial.print(F(" factor="));
							Serial.print(factor);
							Serial.print(F(" result="));
							Serial.print(result);
							Serial.print(F(" ref="));
							Serial.println(refResult);
							errorCount++;
						}
					}

					if (errorCount == 0)
						Serial.println(F("All Scale32 edge case tests PASSED."));
					else
					{
						Serial.print(F("Scale32 edge case tests FAILED. Errors: "));
						Serial.println(errorCount);
					}
					return errorCount == 0;
				}

				// Exhaustive test for GetFactor for all 255*255 combinations of numerator/denominator.
				static bool TestGetFactorExhaustive8()
				{
					Serial.println(F("Starting exhaustive GetFactor (8-bit inputs) tests..."));

					uint32_t errorCount = 0;
					for (uint16_t n = 1; n <= UINT8_MAX; n++)
					{
						for (uint16_t d = 1; d <= UINT8_MAX; d++)
						{
							const uint8_t numerator = (uint8_t)n;
							const uint8_t denominator = (uint8_t)d;

							const Scale8::factor_t result8 = Scale8::GetFactor(numerator, denominator);
							const Scale8::factor_t refResult8 = RefGetFactor8(numerator, denominator);

							const Scale16::factor_t result16 = Scale16::GetFactor(numerator, denominator);
							const Scale16::factor_t refResult16 = RefGetFactor16(numerator, denominator);

							const Scale32::factor_t result32 = Scale32::GetFactor(numerator, denominator);
							const Scale32::factor_t refResult32 = RefGetFactor32(numerator, denominator);

							if (result8 != refResult8
								|| result16 != refResult16
								|| result32 != refResult32)
							{
								Serial.print(F("GetFactor error: numerator="));
								Serial.print(numerator);
								Serial.print(F(" denominator="));
								Serial.print(denominator);
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
								}
								errorCount++;
							}
						}
						if ((n % 16) == 0)
						{
							Serial.print(F("GetFactor progress: "));
							Serial.print(n);
							Serial.println(F(" numerators tested..."));
						}
					}
					if (errorCount == 0)
					{
						Serial.println(F("All GetFactor (8-bit inputs) tests PASSED."));
					}
					else
					{
						Serial.print(F("GetFactor (8-bit inputs) tests FAILED. Errors: "));
						Serial.println(errorCount);
					}

					return errorCount == 0;
				}

				template<uint32_t MaxIterations = 50000>
				static bool RunTests()
				{
					bool pass = true;

					pass &= TestGetFactorExhaustive8();

					pass &= TestScale8Exhaustive8();
					pass &= TestScale16Exhaustive8();

					pass &= TestScale8SignedValues();
					pass &= TestScale16SignedSample();

					pass &= TestScale8Sample<MaxIterations>();
					pass &= TestScale16Sample<MaxIterations / 10>();

					// New: Scale32 coverage and signed GetFactor coverage
					pass &= TestScale32Sample<1000>();
					pass &= TestScale32GetFactor();
					pass &= TestScale32EdgeCases();
					pass &= TestScale8GetFactorSigned();
					pass &= TestScale16GetFactorSigned();

					if (!pass)
					{
						Serial.println(F("FixedPoint::Scale tests FAILED."));
					}

					return pass;
				}
			}
		}
	}
}
#endif