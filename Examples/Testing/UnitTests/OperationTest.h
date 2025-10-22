#ifndef _OPERATION_TEST_h
#define _OPERATION_TEST_h

#include <IntegerSignalTesting.h>

namespace IntegerSignal
{
	namespace Operation
	{
		namespace Test
		{
			static bool TestMinValue()
			{
				bool pass = true;
				pass &= (MinValue(5, 10) == 5);
				pass &= (MinValue(-3, 2) == -3);
				pass &= (MinValue(0u, 255u) == 0u);
				pass &= (MinValue(100, 100) == 100);
				return pass;
			}

			static bool TestMaxValue()
			{
				bool pass = true;
				pass &= (MaxValue(5, 10) == 10);
				pass &= (MaxValue(-3, 2) == 2);
				pass &= (MaxValue(0u, 255u) == 255u);
				pass &= (MaxValue(100, 100) == 100);
				return pass;
			}

			static bool TestLimitValue()
			{
				bool pass = true;
				pass &= (LimitValue(5, 0, 10) == 5);
				pass &= (LimitValue(-5, 0, 10) == 0);
				pass &= (LimitValue(15, 0, 10) == 10);
				pass &= (LimitValue(10, 0, 10) == 10);
				pass &= (LimitValue(0, 0, 10) == 0);
				return pass;
			}

			static bool TestAbsValue()
			{
				bool pass = true;
				pass &= (AbsValue(5) == 5);
				pass &= (AbsValue(-5) == 5);
				pass &= (AbsValue(0) == 0);
				pass &= (AbsValue(-128) == 128);
				return pass;
			}

			static bool TestGetBitShifts()
			{
				bool pass = true;
				pass &= (GetBitShifts(0) == 0);
				pass &= (GetBitShifts(1) == 0);
				pass &= (GetBitShifts(2) == 1);
				pass &= (GetBitShifts(8) == 3);
				pass &= (GetBitShifts(255) == 7);
				return pass;
			}

			static bool TestSignedRightShift()
			{
				bool pass = true;
				pass &= (SignedRightShift(8, 1) == 4);
				pass &= (SignedRightShift(-8, 1) == -4);
				pass &= (SignedRightShift(7, 2) == 1);
				pass &= (SignedRightShift(-7, 2) == -1);
				pass &= (SignedRightShift(0, 3) == 0);
				return pass;
			}

			static bool TestSignedRightShiftTemplate()
			{
				bool pass = true;
				pass &= (SignedRightShift<int16_t, 2>(8) == 2);
				pass &= (SignedRightShift<int16_t, 2>(-8) == -2);
				pass &= (SignedRightShift<int16_t, 0>(5) == 5);
				pass &= (SignedRightShift<int16_t, 3>(-16) == -2);
				return pass;
			}

			// Reference helpers that compute expected SignedLeftShift results using unsigned-domain arithmetic.
			template<typename T>
			static T ExpectedSignedLeftShiftRuntime(const T value, const uint8_t shifts)
			{
				using U = typename TypeTraits::TypeSign::make_unsigned<T>::type;

				if (value >= 0)
				{
					return static_cast<T>(static_cast<U>(value) << shifts);
				}
				else
				{
					// match the implementation: (U)0 - ( (U)( (U)0 - (U)value ) << shifts )
					return static_cast<T>(static_cast<U>(0) - ( (static_cast<U>(static_cast<U>(0) - static_cast<U>(value))) << shifts ));
				}
			}

			template<typename T, uint8_t shifts>
			static T ExpectedSignedLeftShiftTemplate(const T value)
			{
				using U = typename TypeTraits::TypeSign::make_unsigned<T>::type;

				if (value >= 0)
				{
					return static_cast<T>(static_cast<U>(value) << shifts);
				}
				else
				{
					return static_cast<T>(static_cast<U>(0) - ( (static_cast<U>(static_cast<U>(0) - static_cast<U>(value))) << shifts ));
				}
			}

			// Expanded runtime tests for SignedLeftShift across multiple types, values and shift counts.
			static bool TestSignedLeftShift()
			{
				Serial.println(F("Starting expanded SignedLeftShift runtime tests..."));
				bool pass = true;

				// Helper lambda to print failures (keeps code concise)
				auto PrintFail = [](const char* msg, int64_t inVal, uint8_t shifts, int64_t expect, int64_t got)
				{
					Serial.print(F("SignedLeftShift FAIL: "));
					Serial.print(msg);
					Serial.print(F(" in="));
					Serial.print((long)inVal);
					Serial.print(F(" s="));
					Serial.print((int)shifts);
					Serial.print(F(" out="));
					Serial.print((long)got);
					Serial.print(F(" exp="));
					Serial.println((long)expect);
				};

				// Test int8_t exhaustively for a set of representative values and all valid shifts.
				{
					const int8_t values[] = { 0, 1, 2, 3, 0x3F, 0x7F, -1, -2, -3, static_cast<int8_t>(INT8_MIN + 1), INT8_MIN };
					const uint8_t maxShift = sizeof(int8_t) * 8 - 1;
					for (uint8_t s = 0; s <= maxShift; ++s)
					{
						for (size_t i = 0; i < sizeof(values) / sizeof(values[0]); ++i)
						{
							int8_t v = values[i];
							int8_t r = SignedLeftShift<int8_t>(v, s);
							int8_t e = ExpectedSignedLeftShiftRuntime<int8_t>(v, s);
							if (r != e)
							{
								PrintFail("int8_t", v, s, e, r);
								pass = false;
							}
						}
					}
				}

				// Test int16_t across representative values and full shift range.
				{
					const int16_t values[] = { 0, 1, 2, 3, 0x1234, INT16_MAX, -1, -2, -3, static_cast<int16_t>(INT16_MIN + 1), INT16_MIN };
					const uint8_t maxShift = sizeof(int16_t) * 8 - 1;
					for (uint8_t s = 0; s <= maxShift; ++s)
					{
						for (size_t i = 0; i < sizeof(values) / sizeof(values[0]); ++i)
						{
							int16_t v = values[i];
							int16_t r = SignedLeftShift<int16_t>(v, s);
							int16_t e = ExpectedSignedLeftShiftRuntime<int16_t>(v, s);
							if (r != e)
							{
								PrintFail("int16_t", v, s, e, r);
								pass = false;
							}
						}
					}
				}

				// Test int32_t with a limited shift range (avoid long-running loops on constrained platforms).
				{
					const int32_t values[] = { 0, 1, 2, 3, 0x123456, INT32_MAX, -1, -2, -3, static_cast<int32_t>(INT32_MIN + 1), INT32_MIN };
					const uint8_t maxShift = 7; // sample lower shifts; larger shifts behave similarly modulo arithmetic
					for (uint8_t s = 0; s <= maxShift; ++s)
					{
						for (size_t i = 0; i < sizeof(values) / sizeof(values[0]); ++i)
						{
							int32_t v = values[i];
							int32_t r = SignedLeftShift<int32_t>(v, s);
							int32_t e = ExpectedSignedLeftShiftRuntime<int32_t>(v, s);
							if (r != e)
							{
								PrintFail("int32_t", v, s, e, r);
								pass = false;
							}
						}
					}
				}

				// Test unsigned types (verify logical left-shift semantics)
				{
					const uint8_t u8vals[] = { 0, 1, 2, 0x7F, 0xFF };
					for (uint8_t s = 0; s < 8; ++s)
					{
						for (size_t i = 0; i < sizeof(u8vals) / sizeof(u8vals[0]); ++i)
						{
							uint8_t v = u8vals[i];
							uint8_t r = SignedLeftShift<uint8_t>(v, s);
							uint8_t e = static_cast<uint8_t>(static_cast<uint8_t>(v) << s);
							if (r != e)
							{
								PrintFail("uint8_t", v, s, e, r);
								pass = false;
							}
						}
					}

					const uint16_t u16vals[] = { 0, 1, 0x1234, 0xFFFF };
					for (uint8_t s = 0; s < 16; ++s)
					{
						for (size_t i = 0; i < sizeof(u16vals) / sizeof(u16vals[0]); ++i)
						{
							uint16_t v = u16vals[i];
							uint16_t r = SignedLeftShift<uint16_t>(v, s);
							uint16_t e = static_cast<uint16_t>(static_cast<uint16_t>(v) << s);
							if (r != e)
							{
								PrintFail("uint16_t", v, s, e, r);
								pass = false;
							}
						}
					}
				}

				if (pass)
					Serial.println(F("SignedLeftShift runtime tests PASSED."));
				else
					Serial.println(F("SignedLeftShift runtime tests FAILED."));

				return pass;
			}

			// Expanded template (compile-time shifts) tests.
			static bool TestSignedLeftShiftTemplate()
			{
				Serial.println(F("Starting expanded SignedLeftShift template tests..."));
				bool pass = true;

				// small set of compile-time shifts and values for each type
				pass &= (SignedLeftShift<int8_t, 0>(0) == ExpectedSignedLeftShiftTemplate<int8_t, 0>(0));
				pass &= (SignedLeftShift<int8_t, 1>(1) == ExpectedSignedLeftShiftTemplate<int8_t, 1>(1));
				pass &= (SignedLeftShift<int8_t, 2>(3) == ExpectedSignedLeftShiftTemplate<int8_t, 2>(3));
				pass &= (SignedLeftShift<int8_t, 3>(-1) == ExpectedSignedLeftShiftTemplate<int8_t, 3>(-1));
				pass &= (SignedLeftShift<int8_t, 7>(-2) == ExpectedSignedLeftShiftTemplate<int8_t, 7>(-2));

				pass &= (SignedLeftShift<int16_t, 0>(123) == ExpectedSignedLeftShiftTemplate<int16_t, 0>(123));
				pass &= (SignedLeftShift<int16_t, 1>(123) == ExpectedSignedLeftShiftTemplate<int16_t, 1>(123));
				pass &= (SignedLeftShift<int16_t, 4>(0x0100) == ExpectedSignedLeftShiftTemplate<int16_t, 4>(0x0100));
				pass &= (SignedLeftShift<int16_t, 5>(-0x0100) == ExpectedSignedLeftShiftTemplate<int16_t, 5>(-0x0100));

				pass &= (SignedLeftShift<uint8_t, 3>((uint8_t)0x01) == ExpectedSignedLeftShiftTemplate<uint8_t, 3>((uint8_t)0x01));
				pass &= (SignedLeftShift<uint16_t, 8>((uint16_t)0x00FF) == ExpectedSignedLeftShiftTemplate<uint16_t, 8>((uint16_t)0x00FF));

				if (pass)
					Serial.println(F("SignedLeftShift template tests PASSED."));
				else
					Serial.println(F("SignedLeftShift template tests FAILED."));

				return pass;
			}

			static bool RunTests()
			{
				Serial.println(F("Starting Operation tests..."));
				bool pass = true;
				pass &= TestMinValue();
				pass &= TestMaxValue();
				pass &= TestLimitValue();
				pass &= TestAbsValue();
				pass &= TestGetBitShifts();
				pass &= TestSignedRightShift();
				pass &= TestSignedRightShiftTemplate();
				pass &= TestSignedLeftShift();
				pass &= TestSignedLeftShiftTemplate();

				if (pass)
					Serial.println(F("Operation tests PASSED."));
				else
					Serial.println(F("Operation tests FAILED."));

				return pass;
			}
		}
	}
}
#endif