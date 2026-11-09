#ifndef _FIXED_POINT_FRACTION_SCALARTEST_h
#define _FIXED_POINT_FRACTION_SCALARTEST_h

#include "FixedPointFractionReferenceTest.h"

namespace IntegerSignal
{
	namespace FixedPoint
	{
		namespace ScalarFraction
		{
			namespace Test
			{
				static inline bool TestGetScalarU8ExhaustiveValidation()
				{
					Serial.println(F("Starting exhaustive GetScalarU8 tests..."));

					uint32_t errorCount = 0;
					for (uint16_t n = 0; n <= UINT8_MAX; n++)
					{
						for (uint16_t d = 1; d <= UINT8_MAX; d++)
						{
							const uint8_t numerator = (uint8_t)n;
							const uint8_t denominator = (uint8_t)d;

							const UFraction8::scalar_t result = GetScalarU8(numerator, denominator);
							const UFraction8::scalar_t refResult = numerator >= denominator ? UFraction8::FRACTION_1X
								: (UFraction8::scalar_t)(((uint16_t)numerator * UFraction8::FRACTION_1X) / denominator);

							if (result != refResult)
							{
								Serial.print(F("GetScalarU8 error: numerator="));
								Serial.print(numerator);
								Serial.print(F(" denominator="));
								Serial.print(denominator);
								Serial.print(F(" result="));
								Serial.print(result);
								Serial.print(F(" ref="));
								Serial.println(refResult);
								errorCount++;
							}
						}

						if ((n % 16) == 0)
						{
							Serial.print(F("GetScalarU8 progress: "));
							Serial.print(n);
							Serial.println(F(" numerators tested..."));
						}
					}

					if (errorCount == 0)
					{
						Serial.println(F("All GetScalarU8 tests PASSED."));
					}
					else
					{
						Serial.print(F("GetScalarU8 tests FAILED. Errors: "));
						Serial.println(errorCount);
					}

					return errorCount == 0;
				}

				static inline bool TestGetScalarU8SignedInputs()
				{
					Serial.println(F("Starting GetScalarU8 signed-input regression tests..."));

					const int8_t numerators[] = { INT8_MIN, -1, 0, 1, 3, 7, 15, 31, 63, 127 };
					const int8_t denominators[] = { INT8_MIN, -1, 0, 1, 2, 3, 5, 7, 63, 127 };
					bool pass = true;

					for (size_t n = 0; n < sizeof(numerators) / sizeof(numerators[0]); ++n)
					{
						for (size_t d = 0; d < sizeof(denominators) / sizeof(denominators[0]); ++d)
						{
							const int8_t numerator = numerators[n];
							const int8_t denominator = denominators[d];

							const UFraction8::scalar_t result = GetScalarU8(numerator, denominator);
							const UFraction8::scalar_t refResult = numerator < 0 ? 0
								: denominator <= 0 ? UFraction8::FRACTION_1X
								: numerator >= denominator ? UFraction8::FRACTION_1X
								: (UFraction8::scalar_t)(((uint16_t)numerator * UFraction8::FRACTION_1X) / (uint8_t)denominator);

							if (result != refResult)
							{
								Serial.print(F("GetScalarU8 signed-input error: numerator="));
								Serial.print((int)numerator);
								Serial.print(F(" denominator="));
								Serial.print((int)denominator);
								Serial.print(F(" result="));
								Serial.print(result);
								Serial.print(F(" ref="));
								Serial.println(refResult);
								pass = false;
							}
						}
					}

					const int32_t wideUnsignedNumerators[] = { INT32_MIN, -65536, -1, 0, 1, 255, 256, 65535, 65536, INT32_MAX };
					const int32_t wideUnsignedDenominators[] = { INT32_MIN, -65536, -1, 0, 1, 2, 255, 256, 65535, 65536, INT32_MAX };

					for (size_t n = 0; n < sizeof(wideUnsignedNumerators) / sizeof(wideUnsignedNumerators[0]); ++n)
					{
						for (size_t d = 0; d < sizeof(wideUnsignedDenominators) / sizeof(wideUnsignedDenominators[0]); ++d)
						{
							const int32_t numerator = wideUnsignedNumerators[n];
							const int32_t denominator = wideUnsignedDenominators[d];

							const UFraction8::scalar_t aliasConstexpr8 = Scalar::Constexpr::GetScalarU8(numerator, denominator);
							const UFraction8::scalar_t aliasRuntime8 = Scalar::Runtime::GetScalarU8(numerator, denominator);
							const UFraction8::scalar_t aliasDefault8 = Scalar::GetScalarU8(numerator, denominator);
							const UFraction8::scalar_t refAlias8 = UFraction8::Base::GetScalar(numerator, denominator);

							if (aliasConstexpr8 != refAlias8 || aliasRuntime8 != refAlias8 || aliasDefault8 != refAlias8)
							{
								Serial.print(F("ScalarAliases U8 mismatch: n="));
								Serial.print(numerator);
								Serial.print(F(" d="));
								Serial.print(denominator);
								Serial.print(F(" constexpr="));
								Serial.print(aliasConstexpr8);
								Serial.print(F(" runtime="));
								Serial.print(aliasRuntime8);
								Serial.print(F(" default="));
								Serial.print(aliasDefault8);
								Serial.print(F(" ref="));
								Serial.println(refAlias8);
								pass = false;
							}

							const UFraction16::scalar_t aliasConstexpr16 = Scalar::Constexpr::GetScalarU16(numerator, denominator);
							const UFraction16::scalar_t aliasRuntime16 = Scalar::Runtime::GetScalarU16(numerator, denominator);
							const UFraction16::scalar_t aliasDefault16 = Scalar::GetScalarU16(numerator, denominator);
							const UFraction16::scalar_t refAlias16 = UFraction16::Base::GetScalar(numerator, denominator);

							if (aliasConstexpr16 != refAlias16 || aliasRuntime16 != refAlias16 || aliasDefault16 != refAlias16)
							{
								Serial.print(F("ScalarAliases U16 mismatch: n="));
								Serial.print(numerator);
								Serial.print(F(" d="));
								Serial.print(denominator);
								Serial.print(F(" constexpr="));
								Serial.print(aliasConstexpr16);
								Serial.print(F(" runtime="));
								Serial.print(aliasRuntime16);
								Serial.print(F(" default="));
								Serial.print(aliasDefault16);
								Serial.print(F(" ref="));
								Serial.println(refAlias16);
								pass = false;
							}

							const UFraction32::scalar_t aliasConstexpr32 = Scalar::Constexpr::GetScalarU32(numerator, denominator);
							const UFraction32::scalar_t aliasRuntime32 = Scalar::Runtime::GetScalarU32(numerator, denominator);
							const UFraction32::scalar_t aliasDefault32 = Scalar::GetScalarU32(numerator, denominator);
							const UFraction32::scalar_t refAlias32 = UFraction32::Base::GetScalar(numerator, denominator);

							if (aliasConstexpr32 != refAlias32 || aliasRuntime32 != refAlias32 || aliasDefault32 != refAlias32)
							{
								Serial.print(F("ScalarAliases U32 mismatch: n="));
								Serial.print(numerator);
								Serial.print(F(" d="));
								Serial.print(denominator);
								Serial.print(F(" constexpr="));
								Serial.print(aliasConstexpr32);
								Serial.print(F(" runtime="));
								Serial.print(aliasRuntime32);
								Serial.print(F(" default="));
								Serial.print(aliasDefault32);
								Serial.print(F(" ref="));
								Serial.println(refAlias32);
								pass = false;
							}
						}
					}

					if (pass)
					{
						Serial.println(F("GetScalarU8 signed-input regression tests PASSED."));
					}
					else
					{
						Serial.println(F("GetScalarU8 signed-input regression tests FAILED."));
					}

					return pass;
				}

				static inline bool TestGetScalarFastUnsignedRegression()
				{
					Serial.println(F("Starting fast unsigned GetScalar regression tests..."));
					bool pass = true;

					const uint16_t numerators16[] = { 0u, 1u, 3u, 7u, 15u, 31u, 63u, 127u, 255u, 256u, 1023u, 4095u, 16383u, 32767u };
					const uint16_t denominators16[] = { 1u, 2u, 3u, 5u, 7u, 11u, 127u, 255u, 256u, 1023u, 4095u, 16384u, 32768u, 65535u };

					for (size_t n = 0; n < sizeof(numerators16) / sizeof(numerators16[0]); ++n)
					{
						for (size_t d = 0; d < sizeof(denominators16) / sizeof(denominators16[0]); ++d)
						{
							const uint16_t numerator = numerators16[n];
							const uint16_t denominator = denominators16[d];
							const UFraction16::scalar_t result = QFormatUnsigned::Scalar::Runtime::GetScalarU16(numerator, denominator);
							const UFraction16::scalar_t ref = QFormatUnsigned::Scalar::Constexpr::GetScalarU16(numerator, denominator);

							if (result != ref)
							{
								Serial.print(F("GetScalarRuntimeU16 error: numerator="));
								Serial.print(numerator);
								Serial.print(F(" denominator="));
								Serial.print(denominator);
								Serial.print(F(" result="));
								Serial.print(result);
								Serial.print(F(" ref="));
								Serial.println(ref);
								pass = false;
							}
						}
					}

					const uint32_t numerators32[] = { 0u, 1u, 3u, 7u, 15u, 31u, 63u, 127u, 255u, 65535u, 65536u, 1048575u, 16777215u, 268435455u, 1073741823u, 2147483647u };
					const uint32_t denominators32[] = { 1u, 2u, 3u, 5u, 7u, 11u, 127u, 255u, 65535u, 65536u, 1048576u, 16777216u, 268435456u, 1073741824u, 2147483648u, 4294967295u };

					for (size_t n = 0; n < sizeof(numerators32) / sizeof(numerators32[0]); ++n)
					{
						for (size_t d = 0; d < sizeof(denominators32) / sizeof(denominators32[0]); ++d)
						{
							const uint32_t numerator = numerators32[n];
							const uint32_t denominator = denominators32[d];
							const UFraction32::scalar_t result = QFormatUnsigned::Scalar::Runtime::GetScalarU32(numerator, denominator);
							const UFraction32::scalar_t ref = QFormatUnsigned::Scalar::Constexpr::GetScalarU32(numerator, denominator);

							if (result != ref)
							{
								Serial.print(F("GetScalarRuntimeU32 error: numerator="));
								Serial.print(numerator);
								Serial.print(F(" denominator="));
								Serial.print(denominator);
								Serial.print(F(" result="));
								Serial.print(result);
								Serial.print(F(" ref="));
								Serial.println(ref);
								pass = false;
							}
						}
					}

					const int32_t signedNumerators[] = { INT32_MIN, -65536, -1, 0, 1, 3, 7, 127, 255, 65535, 65536, 1048575, INT32_MAX };
					const int32_t signedDenominators[] = { INT32_MIN, -65536, -1, 0, 1, 2, 3, 7, 127, 255, 65535, 65536, INT32_MAX };

					for (size_t n = 0; n < sizeof(signedNumerators) / sizeof(signedNumerators[0]); ++n)
					{
						for (size_t d = 0; d < sizeof(signedDenominators) / sizeof(signedDenominators[0]); ++d)
						{
							const int32_t numerator = signedNumerators[n];
							const int32_t denominator = signedDenominators[d];

							const UFraction16::scalar_t result16 = QFormatUnsigned::Scalar::Runtime::GetScalarU16(numerator, denominator);
							const UFraction16::scalar_t ref16 = QFormatUnsigned::Scalar::Constexpr::GetScalarU16(numerator, denominator);
							const UFraction32::scalar_t result32 = QFormatUnsigned::Scalar::Runtime::GetScalarU32(numerator, denominator);
							const UFraction32::scalar_t ref32 = QFormatUnsigned::Scalar::Constexpr::GetScalarU32(numerator, denominator);

							if (result16 != ref16 || result32 != ref32)
							{
								Serial.print(F("Signed fast GetScalar error: numerator="));
								Serial.print(numerator);
								Serial.print(F(" denominator="));
								Serial.print(denominator);
								if (result16 != ref16)
								{
									Serial.print(F(" u16="));
									Serial.print(result16);
									Serial.print(F(" ref16="));
									Serial.print(ref16);
								}
								if (result32 != ref32)
								{
									Serial.print(F(" u32="));
									Serial.print(result32);
									Serial.print(F(" ref32="));
									Serial.print(ref32);
								}
								Serial.println();
								pass = false;
							}
						}
					}

					if (pass)
					{
						Serial.println(F("Runtime unsigned GetScalar regression tests PASSED."));
					}
					else
					{
						Serial.println(F("Runtime unsigned GetScalar regression tests FAILED."));
					}

					return pass;
				}

				// Exhaustive test for GetScalar (signed Fraction8/16/32) for all 255*255 combinations of numerator/denominator.
				static inline bool TestGetFraction8Exhaustive()
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

				static inline bool TestGetScalarWideIntermediateRegression()
				{
					Serial.println(F("Starting GetScalar wide-intermediate regression tests..."));
					bool pass = true;

					const uint8_t unsignedNumerators[] = { 0u, 1u, 3u, 7u, 15u, 31u, 63u, 127u, 255u };
					const uint8_t unsignedDenominators[] = { 0u, 1u, 2u, 3u, 5u, 7u, 11u, 127u, 255u };

					for (size_t n = 0; n < sizeof(unsignedNumerators) / sizeof(unsignedNumerators[0]); ++n)
					{
						for (size_t d = 0; d < sizeof(unsignedDenominators) / sizeof(unsignedDenominators[0]); ++d)
						{
							const uint8_t numerator = unsignedNumerators[n];
							const uint8_t denominator = unsignedDenominators[d];

							const UFraction16::scalar_t resultU16 = GetScalarU16(numerator, denominator);
							const UFraction16::scalar_t refU16 = (denominator == 0) ? UFraction16::FRACTION_1X
								: (numerator > denominator) ? UFraction16::FRACTION_1X
								: (UFraction16::scalar_t)(((uint32_t)numerator * UFraction16::FRACTION_1X) / denominator);
							const UFraction32::scalar_t resultU32 = GetScalarU32(numerator, denominator);
							const UFraction32::scalar_t refU32 = (denominator == 0) ? UFraction32::FRACTION_1X
								: (numerator > denominator) ? UFraction32::FRACTION_1X
								: (UFraction32::scalar_t)(((uint64_t)numerator * UFraction32::FRACTION_1X) / denominator);

							if (resultU16 != refU16 || resultU32 != refU32)
							{
								Serial.print(F("Unsigned GetScalar regression: n="));
								Serial.print(numerator);
								Serial.print(F(" d="));
								Serial.print(denominator);
								if (resultU16 != refU16)
								{
									Serial.print(F(" u16="));
									Serial.print(resultU16);
									Serial.print(F(" ref16="));
									Serial.print(refU16);
								}
								if (resultU32 != refU32)
								{
									Serial.print(F(" u32="));
									Serial.print(resultU32);
									Serial.print(F(" ref32="));
									Serial.print(refU32);
								}
								Serial.println();
								pass = false;
							}
						}
					}

					const int8_t signedNumerators[] = { -127, -63, -1, 0, 1, 3, 7, 15, 31, 63, 127 };
					const int8_t signedDenominators[] = { -127, -1, 0, 1, 2, 3, 5, 7, 63, 127 };

					for (size_t n = 0; n < sizeof(signedNumerators) / sizeof(signedNumerators[0]); ++n)
					{
						for (size_t d = 0; d < sizeof(signedDenominators) / sizeof(signedDenominators[0]); ++d)
						{
							const int8_t numerator = signedNumerators[n];
							const int8_t denominator = signedDenominators[d];

							const Fraction16::scalar_t resultS16 = GetScalarS16(numerator, denominator);
							const Fraction16::scalar_t refS16 = (denominator == 0)
								? (numerator >= 0 ? Fraction16::FRACTION_1X : Fraction16::FRACTION_1X_NEGATIVE)
								: (Fraction16::scalar_t)LimitValue<int32_t, int32_t(Fraction16::FRACTION_1X_NEGATIVE), int32_t(Fraction16::FRACTION_1X)>((((int32_t)numerator) * Fraction16::FRACTION_1X) / denominator);
							const Fraction32::scalar_t resultS32 = GetScalarS32(numerator, denominator);
							const Fraction32::scalar_t refS32 = (denominator == 0)
								? (numerator >= 0 ? Fraction32::FRACTION_1X : Fraction32::FRACTION_1X_NEGATIVE)
								: (Fraction32::scalar_t)LimitValue<int64_t, int64_t(Fraction32::FRACTION_1X_NEGATIVE), int64_t(Fraction32::FRACTION_1X)>((((int64_t)numerator) * Fraction32::FRACTION_1X) / denominator);

							if (resultS16 != refS16 || resultS32 != refS32)
							{
								Serial.print(F("Signed GetScalar regression: n="));
								Serial.print((int)numerator);
								Serial.print(F(" d="));
								Serial.print((int)denominator);
								if (resultS16 != refS16)
								{
									Serial.print(F(" s16="));
									Serial.print(resultS16);
									Serial.print(F(" ref16="));
									Serial.print(refS16);
								}
								if (resultS32 != refS32)
								{
									Serial.print(F(" s32="));
									Serial.print(resultS32);
									Serial.print(F(" ref32="));
									Serial.print(refS32);
								}
								Serial.println();
								pass = false;
							}
						}
					}

					if (pass)
					{
						Serial.println(F("GetScalar wide-intermediate regression tests PASSED."));
					}
					else
					{
						Serial.println(F("GetScalar wide-intermediate regression tests FAILED."));
					}

					return pass;
				}
			}
		}
	}
}

#endif
