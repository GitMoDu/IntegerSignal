#ifndef _FIXED_POINT_FRACTION_TEST_h
#define _FIXED_POINT_FRACTION_TEST_h

#include <IntegerSignalTesting.h>

namespace IntegerSignal
{
	namespace FixedPoint
	{
		namespace ScalarFraction
		{
			namespace Test
			{
				static constexpr ufraction8_t ConstexprScalarU8 = Scalar::Constexpr::GetScalarU8<uint8_t>(1u, 2u);
				static constexpr ufraction16_t ConstexprScalarU16 = Scalar::Constexpr::GetScalarU16<uint16_t>(1u, 2u);
				static constexpr ufraction32_t ConstexprScalarU32 = Scalar::Constexpr::GetScalarU32<uint32_t>(1u, 2u);
				static constexpr fraction8_t ConstexprScalarS8 = Scalar::Constexpr::GetScalarS8<int8_t>(1, 2);
				static constexpr fraction16_t ConstexprScalarS16 = Scalar::Constexpr::GetScalarS16<int16_t>(1, 2);
				static constexpr fraction32_t ConstexprScalarS32 = Scalar::Constexpr::GetScalarS32<int32_t>(1, 2);

				static constexpr uint8_t ConstexprFractionU8 = ScalarFraction::Fraction(UFRACTION8_1X, uint8_t(7));
				static constexpr uint16_t ConstexprFractionU16 = ScalarFraction::Fraction(UFRACTION16_1X, uint16_t(9));
				static constexpr uint32_t ConstexprFractionU32 = ScalarFraction::Fraction(UFRACTION32_1X, uint32_t(11));
				static constexpr int8_t ConstexprFractionS8 = ScalarFraction::Fraction(FRACTION8_1X, int8_t(7));
				static constexpr int16_t ConstexprFractionS16 = ScalarFraction::Fraction(FRACTION16_1X, int16_t(9));
				static constexpr int32_t ConstexprFractionS32 = ScalarFraction::Fraction(FRACTION32_1X, int32_t(11));

				static constexpr uint8_t ConstexprInterpolateU8 = ScalarFraction::Interpolate(UFRACTION8_1X, uint8_t(3), uint8_t(9));
				static constexpr uint16_t ConstexprInterpolateU16 = ScalarFraction::Interpolate(UFRACTION16_1X, uint16_t(5), uint16_t(13));
				static constexpr uint32_t ConstexprInterpolateU32 = ScalarFraction::Interpolate(UFRACTION32_1X, uint32_t(7), uint32_t(17));

				static_assert(ConstexprScalarU8 == UFraction8::Base::GetScalar(uint8_t(1), uint8_t(2)), "Scalar::Constexpr::GetScalarU8 must remain constexpr-capable.");
				static_assert(ConstexprScalarU16 == UFraction16::Base::GetScalar(uint16_t(1), uint16_t(2)), "Scalar::Constexpr::GetScalarU16 must remain constexpr-capable.");
				static_assert(ConstexprScalarU32 == UFraction32::Base::GetScalar(uint32_t(1), uint32_t(2)), "Scalar::Constexpr::GetScalarU32 must remain constexpr-capable.");
				static_assert(ConstexprScalarS8 == QFormatSigned::Scalar::Constexpr::GetScalarS8<int8_t>(1, 2), "Scalar::Constexpr::GetScalarS8 must remain constexpr-capable.");
				static_assert(ConstexprScalarS16 == QFormatSigned::Scalar::Constexpr::GetScalarS16<int16_t>(1, 2), "Scalar::Constexpr::GetScalarS16 must remain constexpr-capable.");
				static_assert(ConstexprScalarS32 == QFormatSigned::Scalar::Constexpr::GetScalarS32<int32_t>(1, 2), "Scalar::Constexpr::GetScalarS32 must remain constexpr-capable.");

				static_assert(ConstexprFractionU8 == uint8_t(7), "Fraction(UFRACTION8_1X, value) must remain constexpr-capable.");
				static_assert(ConstexprFractionU16 == uint16_t(9), "Fraction(UFRACTION16_1X, value) must remain constexpr-capable.");
				static_assert(ConstexprFractionU32 == uint32_t(11), "Fraction(UFRACTION32_1X, value) must remain constexpr-capable.");
				static_assert(ConstexprFractionS8 == int8_t(7), "Fraction(FRACTION8_1X, value) must remain constexpr-capable.");
				static_assert(ConstexprFractionS16 == int16_t(9), "Fraction(FRACTION16_1X, value) must remain constexpr-capable.");
				static_assert(ConstexprFractionS32 == int32_t(11), "Fraction(FRACTION32_1X, value) must remain constexpr-capable.");

				static_assert(ConstexprInterpolateU8 == uint8_t(9), "Interpolate(UFRACTION8_1X, from, to) must remain constexpr-capable.");
				static_assert(ConstexprInterpolateU16 == uint16_t(13), "Interpolate(UFRACTION16_1X, from, to) must remain constexpr-capable.");
				static_assert(ConstexprInterpolateU32 == uint32_t(17), "Interpolate(UFRACTION32_1X, from, to) must remain constexpr-capable.");

				// Reference function for Fraction8::scalar_t (signed)
				template<typename T>
				static inline T RefFraction8(const T value, const Fraction8::scalar_t fraction) {
					return (static_cast<int64_t>(value) * fraction) / Fraction8::FRACTION_1X;
				}

				template<typename T>
				static inline T RefFraction16(const T value, const Fraction16::scalar_t fraction) {
					return (static_cast<int64_t>(value) * fraction) / Fraction16::FRACTION_1X;
				}

				template<typename T>
				static inline T RefFraction32(const T value, const Fraction32::scalar_t fraction) {
					return (static_cast<int64_t>(value) * fraction) / Fraction32::FRACTION_1X;
				}

				// Reference function for UFractionX::scalar_t (unsigned)
				template<typename T>
				static inline T RefUFraction8(const T value, const UFraction8::scalar_t fraction) {
					return (static_cast<uint64_t>(value) * fraction) / UFraction8::FRACTION_1X;
				}

				template<typename T>
				static inline T RefUFraction16(const T value, const UFraction16::scalar_t fraction) {
					return (static_cast<uint64_t>(value) * fraction) / UFraction16::FRACTION_1X;
				}

				template<typename T>
				static inline T RefUFraction32(const T value, const UFraction32::scalar_t fraction) {
					return (static_cast<uint64_t>(value) * fraction) / UFraction32::FRACTION_1X;
				}

				// Reference function for GetScalar (signed Fraction8)
				static inline Fraction8::scalar_t RefGetFraction8(uint8_t numerator, uint8_t denominator)
				{
					const int16_t scaled = (denominator == 0) ? 0
						: (int16_t)(((int16_t)numerator * Fraction8::FRACTION_1X) / denominator);
					return (Fraction8::scalar_t)MinValue<int16_t>(
						Fraction8::FRACTION_1X,
						MaxValue<int16_t>(Fraction8::FRACTION_1X_NEGATIVE, scaled));
				}

				// Reference function for GetScalar (signed Fraction16)
				static inline Fraction16::scalar_t RefGetFraction16(uint8_t numerator, uint8_t denominator)
				{
					const int32_t scaled = (denominator == 0) ? 0
						: (int32_t)(((int32_t)numerator * Fraction16::FRACTION_1X) / denominator);
					return (Fraction16::scalar_t)MinValue<int32_t>(
						Fraction16::FRACTION_1X,
						MaxValue<int32_t>(Fraction16::FRACTION_1X_NEGATIVE, scaled));
				}

				// Reference function for GetScalar (signed Fraction32)
				static inline Fraction32::scalar_t RefGetFraction32(uint8_t numerator, uint8_t denominator)
				{
					const int64_t scaled = (denominator == 0) ? 0
						: (int64_t)(((int64_t)numerator * Fraction32::FRACTION_1X) / denominator);
					return (Fraction32::scalar_t)MinValue<int64_t>(
						Fraction32::FRACTION_1X,
						MaxValue<int64_t>(Fraction32::FRACTION_1X_NEGATIVE, scaled));
				}

				// Reference interpolation functions
				template<typename T>
				static inline T RefUInterpolate8(const T from, const T to, const uint8_t scalar)
				{
					constexpr uint8_t SCALE_UNIT = 128;
					return (static_cast<uint64_t>(from) * (SCALE_UNIT - scalar) +
						static_cast<uint64_t>(to) * scalar) / SCALE_UNIT;
				}

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


				template<uint32_t MaxIterations = 50000, uint8_t maxError32 = 0>
				static inline bool RunTests()
				{
					bool pass = true;

					pass &= TestGetScalarU8ExhaustiveValidation();
					pass &= TestGetScalarU8SignedInputs();
					pass &= TestGetScalarFastUnsignedRegression();
					pass &= TestSplitScalarApiConsistency();
					pass &= TestGetFraction8Exhaustive();
					pass &= TestGetScalarWideIntermediateRegression();
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

				static inline bool RunExhaustive()
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