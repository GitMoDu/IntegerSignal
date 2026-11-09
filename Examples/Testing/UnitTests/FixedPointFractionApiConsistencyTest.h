#ifndef _FIXED_POINT_FRACTION_API_CONSISTENCY_TEST_h
#define _FIXED_POINT_FRACTION_API_CONSISTENCY_TEST_h

#include "FixedPointFractionReferenceTest.h"

namespace IntegerSignal
{
	namespace FixedPoint
	{
		namespace ScalarFraction
		{
			namespace Test
			{
				static inline bool TestSplitScalarApiConsistency()
				{
					Serial.println(F("Starting split scalar API consistency tests..."));
					bool pass = true;

					const uint16_t unsignedNumerators[] = { 0u, 1u, 3u, 7u, 15u, 31u, 63u, 127u, 255u, 256u, 1023u, 4095u, 16383u, 32767u, 65535u };
					const uint16_t unsignedDenominators[] = { 0u, 1u, 2u, 3u, 5u, 7u, 11u, 127u, 255u, 256u, 1023u, 4095u, 16384u, 32768u, 65535u };

					for (size_t n = 0; n < sizeof(unsignedNumerators) / sizeof(unsignedNumerators[0]); ++n)
					{
						for (size_t d = 0; d < sizeof(unsignedDenominators) / sizeof(unsignedDenominators[0]); ++d)
						{
							const uint16_t numerator = unsignedNumerators[n];
							const uint16_t denominator = unsignedDenominators[d];

							const uint8_t d8 = denominator > UINT8_MAX ? UINT8_MAX : (uint8_t)denominator;
							const uint8_t n8 = numerator > UINT8_MAX ? UINT8_MAX : (uint8_t)numerator;
							const UFraction8::scalar_t ref8 = QFormatUnsigned::Scalar::GetScalarU8(n8, d8);
							const UFraction8::scalar_t runtime8 = QFormatUnsigned::Scalar::Runtime::GetScalarU8(n8, d8);
							const UFraction8::scalar_t constexpr8 = QFormatUnsigned::Scalar::Constexpr::GetScalarU8(n8, d8);
							const UFraction8::scalar_t auto8 = QFormatUnsigned::Scalar::GetScalarU8(n8, d8);

							if (constexpr8 != QFormatUnsigned::Scalar::Constexpr::GetScalarU8(n8, d8))
							{
								Serial.print(F("Split U8 constexpr mismatch: n="));
								Serial.print(n8);
								Serial.print(F(" d="));
								Serial.print(d8);
								Serial.print(F(" constexpr="));
								Serial.print(constexpr8);
								Serial.print(F(" ref="));
								Serial.println(QFormatUnsigned::Scalar::Constexpr::GetScalarU8(n8, d8));
								pass = false;
							}

							if (runtime8 != ref8)
							{
								Serial.print(F("Split U8 runtime mismatch: n="));
								Serial.print(n8);
								Serial.print(F(" d="));
								Serial.print(d8);
								Serial.print(F(" runtime="));
								Serial.print(runtime8);
								Serial.print(F(" ref="));
								Serial.println(ref8);
								pass = false;
							}
							const UFraction8::scalar_t expectedAuto8 =
#if defined(__AVR__)
								runtime8;
#else
								constexpr8;
#endif

							if (auto8 != expectedAuto8)
							{
								Serial.print(F("Split U8 auto mismatch: n="));
								Serial.print(n8);
								Serial.print(F(" d="));
								Serial.print(d8);
								Serial.print(F(" auto="));
								Serial.print(auto8);
								Serial.print(F(" runtime="));
								Serial.print(runtime8);
								Serial.print(F(" constexpr="));
								Serial.println(constexpr8);
								pass = false;
							}

							const UFraction16::scalar_t ref16 = QFormatUnsigned::Scalar::GetScalarU16(numerator, denominator);
							const UFraction16::scalar_t runtime16 = QFormatUnsigned::Scalar::Runtime::GetScalarU16(numerator, denominator);
							const UFraction16::scalar_t constexpr16 = QFormatUnsigned::Scalar::Constexpr::GetScalarU16(numerator, denominator);
							const UFraction16::scalar_t auto16 = QFormatUnsigned::Scalar::GetScalarU16(numerator, denominator);

							if (constexpr16 != QFormatUnsigned::Scalar::Constexpr::GetScalarU16(numerator, denominator))
							{
								Serial.print(F("Split U16 constexpr mismatch: n="));
								Serial.print(numerator);
								Serial.print(F(" d="));
								Serial.print(denominator);
								Serial.print(F(" constexpr="));
								Serial.print(constexpr16);
								Serial.print(F(" ref="));
								Serial.println(QFormatUnsigned::Scalar::Constexpr::GetScalarU16(numerator, denominator));
								pass = false;
							}

							if (runtime16 != ref16)
							{
								Serial.print(F("Split U16 runtime mismatch: n="));
								Serial.print(numerator);
								Serial.print(F(" d="));
								Serial.print(denominator);
								Serial.print(F(" runtime="));
								Serial.print(runtime16);
								Serial.print(F(" ref="));
								Serial.println(ref16);
								pass = false;
							}
							const UFraction16::scalar_t expectedAuto16 =
#if defined(__AVR__)
								runtime16;
#else
								constexpr16;
#endif

							if (auto16 != expectedAuto16)
							{
								Serial.print(F("Split U16 auto mismatch: n="));
								Serial.print(numerator);
								Serial.print(F(" d="));
								Serial.print(denominator);
								Serial.print(F(" auto="));
								Serial.print(auto16);
								Serial.print(F(" runtime="));
								Serial.print(runtime16);
								Serial.print(F(" constexpr="));
								Serial.println(constexpr16);
								pass = false;
							}

							const uint32_t numerator32 = numerator;
							const uint32_t denominator32 = denominator;
							const UFraction32::scalar_t ref32 = QFormatUnsigned::Scalar::GetScalarU32(numerator32, denominator32);
							const UFraction32::scalar_t runtime32 = QFormatUnsigned::Scalar::Runtime::GetScalarU32(numerator32, denominator32);
							const UFraction32::scalar_t constexpr32 = QFormatUnsigned::Scalar::Constexpr::GetScalarU32(numerator32, denominator32);
							const UFraction32::scalar_t auto32 = QFormatUnsigned::Scalar::GetScalarU32(numerator32, denominator32);

							if (constexpr32 != QFormatUnsigned::Scalar::Constexpr::GetScalarU32(numerator32, denominator32))
							{
								Serial.print(F("Split U32 constexpr mismatch: n="));
								Serial.print(numerator32);
								Serial.print(F(" d="));
								Serial.print(denominator32);
								Serial.print(F(" constexpr="));
								Serial.print(constexpr32);
								Serial.print(F(" ref="));
								Serial.println(QFormatUnsigned::Scalar::Constexpr::GetScalarU32(numerator32, denominator32));
								pass = false;
							}

							if (runtime32 != ref32)
							{
								Serial.print(F("Split U32 runtime mismatch: n="));
								Serial.print(numerator32);
								Serial.print(F(" d="));
								Serial.print(denominator32);
								Serial.print(F(" runtime="));
								Serial.print(runtime32);
								Serial.print(F(" ref="));
								Serial.println(ref32);
								pass = false;
							}

							if (auto32 != constexpr32)
							{
								Serial.print(F("Split U32 auto->constexpr mismatch: n="));
								Serial.print(numerator32);
								Serial.print(F(" d="));
								Serial.print(denominator32);
								Serial.print(F(" auto="));
								Serial.print(auto32);
								Serial.print(F(" constexpr="));
								Serial.println(constexpr32);
								pass = false;
							}
						}
					}

					const int16_t signedNumerators[] = { INT16_MIN, -255, -1, 0, 1, 3, 7, 31, 127, 255, 1023, INT16_MAX };
					const int16_t signedDenominators[] = { INT16_MIN, -255, -1, 0, 1, 2, 3, 7, 31, 127, 255, INT16_MAX };

					for (size_t n = 0; n < sizeof(signedNumerators) / sizeof(signedNumerators[0]); ++n)
					{
						for (size_t d = 0; d < sizeof(signedDenominators) / sizeof(signedDenominators[0]); ++d)
						{
							const int16_t numerator = signedNumerators[n];
							const int16_t denominator = signedDenominators[d];
							const int8_t numerator8 = (int8_t)numerator;
							const int8_t denominator8 = (int8_t)denominator;

							const Fraction8::scalar_t runtime8 = QFormatSigned::Scalar::Runtime::GetScalarS8(numerator8, denominator8);
							const Fraction8::scalar_t constexpr8 = QFormatSigned::Scalar::Constexpr::GetScalarS8(numerator8, denominator8);
							const Fraction8::scalar_t auto8 = QFormatSigned::Scalar::GetScalarS8(numerator8, denominator8);

							if (constexpr8 != QFormatSigned::Scalar::Constexpr::GetScalarS8(numerator8, denominator8))
							{
								Serial.print(F("Split S8 constexpr mismatch: n="));
								Serial.print((int)numerator8);
								Serial.print(F(" d="));
								Serial.print((int)denominator8);
								Serial.print(F(" constexpr="));
								Serial.print(constexpr8);
								Serial.print(F(" ref="));
								Serial.println(QFormatSigned::Scalar::Constexpr::GetScalarS8(numerator8, denominator8));
								pass = false;
							}
							const Fraction8::scalar_t expectedAutoS8 =
#if defined(__AVR__)
								((numerator8 >= 0 && denominator8 > 0) ? runtime8 : constexpr8);
#else
								constexpr8;
#endif

							if (auto8 != expectedAutoS8)
							{
								Serial.print(F("Split S8 auto mismatch: n="));
								Serial.print((int)numerator8);
								Serial.print(F(" d="));
								Serial.print((int)denominator8);
								Serial.print(F(" auto="));
								Serial.print(auto8);
								Serial.print(F(" runtime="));
								Serial.print(runtime8);
								Serial.print(F(" constexpr="));
								Serial.println(constexpr8);
								pass = false;
							}

							const Fraction16::scalar_t runtime16 = QFormatSigned::Scalar::Runtime::GetScalarS16(numerator, denominator);
							const Fraction16::scalar_t constexpr16 = QFormatSigned::Scalar::Constexpr::GetScalarS16(numerator, denominator);
							const Fraction16::scalar_t auto16 = QFormatSigned::Scalar::GetScalarS16(numerator, denominator);

							if (constexpr16 != QFormatSigned::Scalar::Constexpr::GetScalarS16(numerator, denominator))
							{
								Serial.print(F("Split S16 constexpr mismatch: n="));
								Serial.print(numerator);
								Serial.print(F(" d="));
								Serial.print(denominator);
								Serial.print(F(" constexpr="));
								Serial.print(constexpr16);
								Serial.print(F(" ref="));
								Serial.println(QFormatSigned::Scalar::Constexpr::GetScalarS16(numerator, denominator));
								pass = false;
							}
							const Fraction16::scalar_t expectedAutoS16 =
#if defined(__AVR__)
								((numerator >= 0 && denominator > 0) ? runtime16 : constexpr16);
#else
								constexpr16;
#endif

							if (auto16 != expectedAutoS16)
							{
								Serial.print(F("Split S16 auto mismatch: n="));
								Serial.print(numerator);
								Serial.print(F(" d="));
								Serial.print(denominator);
								Serial.print(F(" auto="));
								Serial.print(auto16);
								Serial.print(F(" runtime="));
								Serial.print(runtime16);
								Serial.print(F(" constexpr="));
								Serial.println(constexpr16);
								pass = false;
							}

							const int32_t numerator32 = numerator;
							const int32_t denominator32 = denominator;
							const Fraction32::scalar_t ref32 = QFormatSigned::Scalar::GetScalarS32(numerator32, denominator32);
							const Fraction32::scalar_t runtime32 = QFormatSigned::Scalar::Runtime::GetScalarS32(numerator32, denominator32);
							const Fraction32::scalar_t constexpr32 = QFormatSigned::Scalar::Constexpr::GetScalarS32(numerator32, denominator32);
							const Fraction32::scalar_t auto32 = QFormatSigned::Scalar::GetScalarS32(numerator32, denominator32);

							if (constexpr32 != QFormatSigned::Scalar::Constexpr::GetScalarS32(numerator32, denominator32))
							{
								Serial.print(F("Split S32 constexpr mismatch: n="));
								Serial.print(numerator32);
								Serial.print(F(" d="));
								Serial.print(denominator32);
								Serial.print(F(" constexpr="));
								Serial.print(constexpr32);
								Serial.print(F(" ref="));
								Serial.println(QFormatSigned::Scalar::Constexpr::GetScalarS32(numerator32, denominator32));
								pass = false;
							}

							if (runtime32 != ref32)
							{
								Serial.print(F("Split S32 runtime mismatch: n="));
								Serial.print(numerator32);
								Serial.print(F(" d="));
								Serial.print(denominator32);
								Serial.print(F(" runtime="));
								Serial.print(runtime32);
								Serial.print(F(" ref="));
								Serial.println(ref32);
								pass = false;
							}

							if (auto32 != constexpr32)
							{
								Serial.print(F("Split S32 auto->constexpr mismatch: n="));
								Serial.print(numerator32);
								Serial.print(F(" d="));
								Serial.print(denominator32);
								Serial.print(F(" auto="));
								Serial.print(auto32);
								Serial.print(F(" constexpr="));
								Serial.println(constexpr32);
								pass = false;
							}
						}
					}

					if (pass)
					{
						Serial.println(F("Split scalar API consistency tests PASSED."));
					}
					else
					{
						Serial.println(F("Split scalar API consistency tests FAILED."));
					}

					return pass;
				}
			}
		}
	}
}

#endif
