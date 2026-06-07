#ifndef _LINEAR_INTERPOLATE_TEST_h
#define _LINEAR_INTERPOLATE_TEST_h

#include <IntegerSignalTesting.h>
#include <IntegerSignal.h>

namespace IntegerSignal
{
	namespace Curves
	{
		namespace LinearInterpolate
		{
			namespace Test
			{
				template<typename value_t>
				static inline bool ExpectEqual(const value_t actual, const value_t expected, const __FlashStringHelper* testName)
				{
					if (actual == expected)
					{
						return true;
					}

					Serial.print(testName);
					Serial.print(F(" failed. expected="));
					Serial.print(expected);
					Serial.print(F(" actual="));
					Serial.println(actual);
					return false;
				}

				static inline bool TestEmptyValuePairsU8()
				{
					LinearInterpolateTemplateU8<> curve;
					const uint8_t input = 42;
					const uint8_t output = curve.Get(input);

					return ExpectEqual(output, input, F("LinearInterpolateU8 empty value-pair pass-through"));
				}

				static inline bool TestSingleValuePairU16()
				{
					LinearInterpolateTemplateU16<100, 1234> curve;
					bool pass = true;

					pass &= ExpectEqual<uint16_t>(curve.Get(0), 1234, F("LinearInterpolateU16 single pair below"));
					pass &= ExpectEqual<uint16_t>(curve.Get(100), 1234, F("LinearInterpolateU16 single pair equal"));
					pass &= ExpectEqual<uint16_t>(curve.Get(200), 1234, F("LinearInterpolateU16 single pair above"));

					return pass;
				}

				static inline bool TestAllLowValuePairsU8()
				{
					LinearInterpolateTemplateU8<0, 0, 10, 0, UINT8_MAX, 0> curve;
					bool pass = true;

					pass &= ExpectEqual<uint8_t>(curve.Get(0), 0, F("LinearInterpolateU8 all-low start"));
					pass &= ExpectEqual<uint8_t>(curve.Get(5), 0, F("LinearInterpolateU8 all-low mid"));
					pass &= ExpectEqual<uint8_t>(curve.Get(UINT8_MAX), 0, F("LinearInterpolateU8 all-low max"));

					return pass;
				}

				static inline bool TestAllHighValuePairsU8()
				{
					LinearInterpolateTemplateU8<0, UINT8_MAX, 10, UINT8_MAX, UINT8_MAX, UINT8_MAX> curve;
					bool pass = true;

					pass &= ExpectEqual<uint8_t>(curve.Get(0), UINT8_MAX, F("LinearInterpolateU8 all-high start"));
					pass &= ExpectEqual<uint8_t>(curve.Get(5), UINT8_MAX, F("LinearInterpolateU8 all-high mid"));
					pass &= ExpectEqual<uint8_t>(curve.Get(UINT8_MAX), UINT8_MAX, F("LinearInterpolateU8 all-high max"));

					return pass;
				}

				static inline bool TestFullIdentityValuePairsU8()
				{
					LinearInterpolateTemplateU8<0, 0, UINT8_MAX, UINT8_MAX> curve;
					bool pass = true;

					pass &= ExpectEqual<uint8_t>(curve.Get(0), 0, F("LinearInterpolateU8 full identity start"));
					pass &= ExpectEqual<uint8_t>(curve.Get(1), 1, F("LinearInterpolateU8 full identity low"));
					pass &= ExpectEqual<uint8_t>(curve.Get(127), 127, F("LinearInterpolateU8 full identity mid"));
					pass &= ExpectEqual<uint8_t>(curve.Get(UINT8_MAX), UINT8_MAX, F("LinearInterpolateU8 full identity max"));

					return pass;
				}

				static inline bool TestUnsignedValuePairsU8()
				{
					using CurveU8 = LinearInterpolateTemplateU8<0, 0, 10, 100, 20, 200>;
					CurveU8 curve;
					bool pass = true;

					pass &= ExpectEqual<uint8_t>(curve.Get(0), 0, F("LinearInterpolateU8 rising pair start"));
					pass &= ExpectEqual<uint8_t>(curve.Get(5), 50, F("LinearInterpolateU8 rising pair mid1"));
					pass &= ExpectEqual<uint8_t>(curve.Get(10), 100, F("LinearInterpolateU8 rising pair control"));
					pass &= ExpectEqual<uint8_t>(curve.Get(15), 150, F("LinearInterpolateU8 rising pair mid2"));
					pass &= ExpectEqual<uint8_t>(curve.Get(20), 200, F("LinearInterpolateU8 rising pair end"));
					pass &= ExpectEqual<uint8_t>(curve.Get(25), 200, F("LinearInterpolateU8 rising pair clamp"));

					pass &= IntegerSignal::TypeTraits::TypeDispatch::is_same<CurveU8, LinearInterpolateTemplateU8<0, 0, 10, 100, 20, 200> >::value;

					if (!pass)
					{
						Serial.println(F("LinearInterpolateU8 value-pair interpolation failed."));
					}

					return pass;
				}

				static inline bool TestDescendingValuePairsU8()
				{
					LinearInterpolateTemplateU8<0, 200, 10, 100, 20, 0> curve;
					bool pass = true;

					pass &= ExpectEqual<uint8_t>(curve.Get(0), 200, F("LinearInterpolateU8 descending pair start"));
					pass &= ExpectEqual<uint8_t>(curve.Get(5), 150, F("LinearInterpolateU8 descending pair mid1"));
					pass &= ExpectEqual<uint8_t>(curve.Get(10), 100, F("LinearInterpolateU8 descending pair control"));
					pass &= ExpectEqual<uint8_t>(curve.Get(15), 50, F("LinearInterpolateU8 descending pair mid2"));
					pass &= ExpectEqual<uint8_t>(curve.Get(25), 0, F("LinearInterpolateU8 descending pair clamp"));

					return pass;
				}

				static inline bool TestSignedValuePairsI16()
				{
					LinearInterpolateTemplateI16<-10, -100, 0, 0, 10, 100> curve;
					bool pass = true;

					pass &= ExpectEqual<int16_t>(curve.Get(-20), -100, F("LinearInterpolateI16 signed pair clamp low"));
					pass &= ExpectEqual<int16_t>(curve.Get(-5), -50, F("LinearInterpolateI16 signed pair mid low"));
					pass &= ExpectEqual<int16_t>(curve.Get(0), 0, F("LinearInterpolateI16 signed pair center"));
					pass &= ExpectEqual<int16_t>(curve.Get(5), 50, F("LinearInterpolateI16 signed pair mid high"));
					pass &= ExpectEqual<int16_t>(curve.Get(20), 100, F("LinearInterpolateI16 signed pair clamp high"));

					if (!pass)
					{
						Serial.println(F("LinearInterpolateI16 signed value-pair interpolation failed."));
					}

					return pass;
				}

				static inline bool TestSignedDescendingValuePairsI8()
				{
					LinearInterpolateTemplateI8<-10, 100, 0, 0, 10, -100> curve;
					bool pass = true;

					pass &= ExpectEqual<int8_t>(curve.Get(-10), 100, F("LinearInterpolateI8 signed descending start"));
					pass &= ExpectEqual<int8_t>(curve.Get(-5), 50, F("LinearInterpolateI8 signed descending mid1"));
					pass &= ExpectEqual<int8_t>(curve.Get(5), -50, F("LinearInterpolateI8 signed descending mid2"));
					pass &= ExpectEqual<int8_t>(curve.Get(10), -100, F("LinearInterpolateI8 signed descending end"));

					return pass;
				}

				static inline bool TestUnsignedValuePairsU32()
				{
					LinearInterpolateTemplateU32<0UL, 0UL, 1000UL, 100000UL, 2000UL, 200000UL> curve;
					bool pass = true;

					pass &= ExpectEqual<uint32_t>(curve.Get(500UL), 50000UL, F("LinearInterpolateU32 value-pair mid1"));
					pass &= ExpectEqual<uint32_t>(curve.Get(1500UL), 150000UL, F("LinearInterpolateU32 value-pair mid2"));
					pass &= ExpectEqual<uint32_t>(curve.Get(2500UL), 200000UL, F("LinearInterpolateU32 value-pair clamp"));

					return pass;
				}

				static inline bool TestRuntimeEmptyU8()
				{
					LinearInterpolateRuntimeU8 curve;
					const uint8_t input = 42;

					return ExpectEqual<uint8_t>(curve.Get(input), input, F("LinearInterpolateRuntimeU8 empty pass-through"));
				}

				static inline bool TestRuntimeUnsignedU8()
				{
					const upoint8_t points[] =
					{
						{ 0, 0 },
						{ 10, 100 },
						{ 20, 200 }
					};

					LinearInterpolateRuntimeU8 curve(points);
					bool pass = true;

					pass &= ExpectEqual<uint8_t>(curve.Get(0), 0, F("LinearInterpolateRuntimeU8 rising start"));
					pass &= ExpectEqual<uint8_t>(curve.Get(5), 50, F("LinearInterpolateRuntimeU8 rising mid1"));
					pass &= ExpectEqual<uint8_t>(curve.Get(15), 150, F("LinearInterpolateRuntimeU8 rising mid2"));
					pass &= ExpectEqual<uint8_t>(curve.Get(25), 200, F("LinearInterpolateRuntimeU8 rising clamp"));

					return pass;
				}

				static inline bool TestRuntimeSignedDescendingI16()
				{
					const ipoint16_t points[] =
					{
						{ -10, 100 },
						{ 0, 0 },
						{ 10, -100 }
					};

					LinearInterpolateRuntimeI16 curve(points);
					bool pass = true;

					pass &= ExpectEqual<int16_t>(curve.Get(-20), 100, F("LinearInterpolateRuntimeI16 descending clamp low"));
					pass &= ExpectEqual<int16_t>(curve.Get(-5), 50, F("LinearInterpolateRuntimeI16 descending mid1"));
					pass &= ExpectEqual<int16_t>(curve.Get(5), -50, F("LinearInterpolateRuntimeI16 descending mid2"));
					pass &= ExpectEqual<int16_t>(curve.Get(20), -100, F("LinearInterpolateRuntimeI16 descending clamp high"));

					return pass;
				}

				static inline bool TestRuntimeValidityU8()
				{
					const upoint8_t validPoints[] =
					{
						{ 0, 0 },
						{ 10, 10 },
						{ 20, 20 }
					};

					const upoint8_t invalidPoints[] =
					{
						{ 0, 0 },
						{ 10, 10 },
						{ 10, 20 }
					};

					LinearInterpolateRuntimeU8 validCurve(validPoints);
					LinearInterpolateRuntimeU8 invalidCurve(invalidPoints);
					LinearInterpolateRuntimeU8 nullCurve(nullptr, 1);
					bool pass = true;

					pass &= ExpectEqual<uint8_t>(validCurve.IsValid() ? 1 : 0, 1, F("LinearInterpolateRuntimeU8 valid points"));
					pass &= ExpectEqual<uint8_t>(invalidCurve.IsValid() ? 1 : 0, 0, F("LinearInterpolateRuntimeU8 invalid duplicate input"));
					pass &= ExpectEqual<uint8_t>(nullCurve.IsValid() ? 1 : 0, 0, F("LinearInterpolateRuntimeU8 invalid null points"));

					return pass;
				}

				static inline bool TestRuntimeSetCurvePointsU8()
				{
					const upoint8_t lowPoints[] =
					{
						{ 0, 0 },
						{ 10, 10 }
					};

					const upoint8_t highPoints[] =
					{
						{ 0, 100 },
						{ 10, 200 }
					};

					LinearInterpolateRuntimeU8 curve(lowPoints);
					bool pass = true;

					pass &= ExpectEqual<uint8_t>(curve.Get(5), 5, F("LinearInterpolateRuntimeU8 set points initial"));
					curve.SetCurvePoints(highPoints);
					pass &= ExpectEqual<uint8_t>(curve.Get(5), 150, F("LinearInterpolateRuntimeU8 set points reassigned"));

					return pass;
				}

				static inline bool RunTests()
				{
					Serial.println(F("Starting LinearInterpolate value-pair tests..."));

					bool pass = true;
					pass &= TestEmptyValuePairsU8();
					pass &= TestSingleValuePairU16();
					pass &= TestAllLowValuePairsU8();
					pass &= TestAllHighValuePairsU8();
					pass &= TestFullIdentityValuePairsU8();
					pass &= TestUnsignedValuePairsU8();
					pass &= TestDescendingValuePairsU8();
					pass &= TestSignedValuePairsI16();
					pass &= TestSignedDescendingValuePairsI8();
					pass &= TestUnsignedValuePairsU32();
					pass &= TestRuntimeEmptyU8();
					pass &= TestRuntimeUnsignedU8();
					pass &= TestRuntimeSignedDescendingI16();
					pass &= TestRuntimeValidityU8();
					pass &= TestRuntimeSetCurvePointsU8();

					if (pass)
					{
						Serial.println(F("LinearInterpolate value-pair tests PASSED."));
					}
					else
					{
						Serial.println(F("LinearInterpolate value-pair tests FAILED."));
					}

					return pass;
				}
			}
		}
	}
}

#endif
