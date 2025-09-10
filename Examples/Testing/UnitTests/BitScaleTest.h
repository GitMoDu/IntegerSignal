#ifndef _BIT_SCALE_TEST_h
#define _BIT_SCALE_TEST_h

#include <IntegerSignalTesting.h>

namespace IntegerSignal
{
	namespace BitScale
	{
		namespace Test
		{
			/////////////////////////////////////////////////////////////////////////////
			// Super Accurate Reference Calculators
			/////////////////////////////////////////////////////////////////////////////
			// Downscaling functions:
			static uint32_t Ref_U64ToU32(uint64_t value)
			{
				// Exactly divide by 2^32
				return (uint32_t)(value / 4294967296ULL);
			}

			static uint16_t Ref_U32ToU16(uint32_t value)
			{
				// Divide by 2^16.
				return (uint16_t)(value / 65536UL);
			}

			static uint8_t Ref_U16ToU8(uint16_t value)
			{
				// Divide by 2^8.
				return (uint8_t)(value / 256U);
			}

			static uint16_t Ref_U64ToU16(uint64_t value)
			{
				// U64ToU16 is equivalent to shifting right by 48 bits.
				return (uint16_t)(value / 281474976710656ULL);  // 2^48
			}

			static uint8_t Ref_U64ToU8(uint64_t value)
			{
				// U64ToU8 is equivalent to shifting right by 56 bits.
				return (uint8_t)(value / 72057594037927936ULL); // 2^56
			}

			static uint8_t Ref_U32ToU8(uint32_t value)
			{
				// U32ToU8 is equivalent to shifting right by 24 bits.
				return (uint8_t)(value / 16777216UL); // 2^24
			}

			// Upscaling (expansion) functions:
			static uint16_t Ref_U8ToU16(uint8_t value)
			{
				// Duplicates the 8-bit value into the high and low bytes.
				// (v << 8) | v = v * 257.
				return (uint16_t)value * 257;
			}

			static uint32_t Ref_U16ToU32(uint16_t value)
			{
				// (v << 16) | v = v * 65537.
				return (uint32_t)value * 65537;
			}

			static uint64_t Ref_U32ToU64(uint32_t value)
			{
				// (v << 32) | v = v * 4294967297ULL.
				return (uint64_t)value * 4294967297ULL;
			}

			static uint32_t Ref_U8ToU32(uint8_t value)
			{
				return Ref_U16ToU32(Ref_U8ToU16(value));
			}

			static uint64_t Ref_U8ToU64(uint8_t value)
			{
				return Ref_U32ToU64(Ref_U8ToU32(value));
			}

			static uint64_t Ref_U16ToU64(uint16_t value)
			{
				return Ref_U32ToU64(Ref_U16ToU32(value));
			}

			// Exhaustively test all 8-bit input values for functions that take uint8_t as input.
			static bool Test_Exhaustive_U8()
			{
				Serial.println(F("Starting exhaustive tests for 8-bit input conversions..."));
				uint32_t errorCount = 0;
				for (uint16_t v = 0; v <= 0xFF; v++)
				{
					uint8_t input = (uint8_t)v;
					// Test U8ToU16: expected duplicates the 8-bit value.
					uint16_t expected_U8ToU16 = ((uint16_t)input << 8) | input;
					uint16_t result_U8ToU16 = IntegerSignal::U8ToU16(input);
					if (result_U8ToU16 != expected_U8ToU16)
					{
						Serial.print(F("Error: U8ToU16("));
						Serial.print(input);
						Serial.print(F(") -> "));
						Serial.print(result_U8ToU16);
						Serial.print(F(", expected "));
						Serial.println(expected_U8ToU16);
						errorCount++;
					}
					// Test U8ToU32.
					uint32_t expected_U8ToU32 = IntegerSignal::U16ToU32(expected_U8ToU16);
					uint32_t result_U8ToU32 = IntegerSignal::U8ToU32(input);
					if (result_U8ToU32 != expected_U8ToU32)
					{
						Serial.print(F("Error: U8ToU32("));
						Serial.print(input);
						Serial.print(F(") -> "));
						Serial.print(result_U8ToU32);
						Serial.print(F(", expected "));
						Serial.println(expected_U8ToU32);
						errorCount++;
					}
					// Test U8ToU64.
					uint64_t expected_U8ToU64 = IntegerSignal::U32ToU64(result_U8ToU32);
					uint64_t result_U8ToU64 = IntegerSignal::U8ToU64(input);
					if (result_U8ToU64 != expected_U8ToU64)
					{
						Serial.print(F("Error: U8ToU64("));
						Serial.print(input);
						Serial.print(F(") -> "));
						IntegerSignal::Testing::PrintUInt64(result_U8ToU64);
						Serial.print(F(", expected "));
						IntegerSignal::Testing::PrintUInt64(expected_U8ToU64);
						Serial.println();
						errorCount++;
					}
				}
				if (errorCount == 0)
					Serial.println(F("Exhaustive 8-bit tests passed."));
				else
				{
					Serial.print(F("Exhaustive 8-bit tests FAILED with "));
					Serial.print(errorCount);
					Serial.println(F(" errors."));
				}
				return errorCount == 0;
			}

			// Exhaustively test all 16-bit input values for functions that take uint16_t.
			static bool Test_Exhaustive_U16()
			{
				Serial.println(F("Starting exhaustive tests for 16-bit input conversions..."));
				uint32_t errorCount = 0;
				for (uint32_t v = 0; v <= 0xFFFF; v++)
				{
					uint16_t input = (uint16_t)v;
					// Test U16ToU8: should equal input >> 8.
					uint8_t expected_U16ToU8 = (uint8_t)(input >> 8);
					uint8_t result_U16ToU8 = IntegerSignal::U16ToU8(input);
					if (result_U16ToU8 != expected_U16ToU8)
					{
						Serial.print(F("Error: U16ToU8("));
						Serial.print(input);
						Serial.print(F(") -> "));
						Serial.print(result_U16ToU8);
						Serial.print(F(", expected "));
						Serial.println(expected_U16ToU8);
						errorCount++;
					}
					// Test U16ToU32: expected duplicates the 16-bit value.
					uint32_t expected_U16ToU32 = ((uint32_t)input << 16) | input;
					uint32_t result_U16ToU32 = IntegerSignal::U16ToU32(input);
					if (result_U16ToU32 != expected_U16ToU32)
					{
						Serial.print(F("Error: U16ToU32("));
						Serial.print(input);
						Serial.print(F(") -> "));
						Serial.print(result_U16ToU32);
						Serial.print(F(", expected "));
						Serial.println(expected_U16ToU32);
						errorCount++;
					}
					// Test U16ToU64.
					uint64_t expected_U16ToU64 = IntegerSignal::U32ToU64(expected_U16ToU32);
					uint64_t result_U16ToU64 = IntegerSignal::U16ToU64(input);
					if (result_U16ToU64 != expected_U16ToU64)
					{
						Serial.print(F("Error: U16ToU64("));
						Serial.print(input);
						Serial.print(F(") -> "));
						IntegerSignal::Testing::PrintUInt64(result_U16ToU64);
						Serial.print(F(", expected "));
						IntegerSignal::Testing::PrintUInt64(expected_U16ToU64);
						Serial.println();
						errorCount++;
					}
				}
				if (errorCount == 0)
					Serial.println(F("Exhaustive 16-bit tests passed."));
				else
				{
					Serial.print(F("Exhaustive 16-bit tests FAILED with "));
					Serial.print(errorCount);
					Serial.println(F(" errors."));
				}
				return errorCount == 0;
			}


			/////////////////////////////////////////////////////////////////////////////
			// Basic Fixed-Value Tests (Using the Super Accurate References)
			/////////////////////////////////////////////////////////////////////////////

			static bool Test_U64ToU32()
			{
				Serial.println(F("Testing U64ToU32..."));
				uint32_t errorCount = 0;
				uint64_t testValues[] = {
					0ULL,
					1ULL,
					0x12345678ULL,
					0xFFFFFFFFULL,
					0x123456789ABCDEF0ULL,
					0xFFFFFFFFFFFFFFFFULL
				};
				for (uint8_t i = 0; i < sizeof(testValues) / sizeof(testValues[0]); i++)
				{
					uint64_t v = testValues[i];
					uint32_t expected = Ref_U64ToU32(v);
					uint32_t result = IntegerSignal::U64ToU32(v);
					if (result != expected)
					{
						errorCount++;
						Serial.print(F("Error: U64ToU32("));
						IntegerSignal::Testing::PrintUInt64(v);
						Serial.print(F(") -> "));
						Serial.print(result);
						Serial.print(F(", expected "));
						Serial.println(expected);
					}
				}
				Serial.println(F("U64ToU32 tests passed."));
				return errorCount == 0;
			}

			static bool Test_U32ToU16()
			{
				Serial.println(F("Testing U32ToU16..."));
				uint32_t errorCount = 0;
				uint32_t testValues[] = { 0UL, 1UL, 0x12345678UL, 0xFFFFFFFFUL };
				for (uint8_t i = 0; i < sizeof(testValues) / sizeof(testValues[0]); i++)
				{
					uint32_t v = testValues[i];
					uint16_t expected = Ref_U32ToU16(v);
					uint16_t result = IntegerSignal::U32ToU16(v);
					if (result != expected)
					{
						Serial.print(F("Error: U32ToU16("));
						Serial.print(v);
						Serial.print(F(") -> "));
						Serial.print(result);
						Serial.print(F(", expected "));
						Serial.println(expected);
						errorCount++;
					}
				}
				Serial.println(F("U32ToU16 tests passed."));
				return errorCount == 0;
			}

			static bool Test_U16ToU8()
			{
				Serial.println(F("Testing U16ToU8..."));
				uint32_t errorCount = 0;
				uint16_t testValues[] = { 0U, 1U, 0x1234U, 0xFFFFU };
				for (uint8_t i = 0; i < sizeof(testValues) / sizeof(testValues[0]); i++)
				{
					uint16_t v = testValues[i];
					uint8_t expected = Ref_U16ToU8(v);
					uint8_t result = IntegerSignal::U16ToU8(v);
					if (result != expected)
					{
						errorCount++;
						Serial.print(F("Error: U16ToU8("));
						Serial.print(v);
						Serial.print(F(") -> "));
						Serial.print(result);
						Serial.print(F(", expected "));
						Serial.println(expected);
					}
				}
				Serial.println(F("U16ToU8 tests passed."));
				return errorCount == 0;
			}

			static bool Test_U64ToU16()
			{
				Serial.println(F("Testing U64ToU16..."));
				uint32_t errorCount = 0;
				uint64_t testValues[] = {
					0ULL,
					1ULL,
					0x12345678ULL,
					0xFFFFFFFFULL,
					0x123456789ABCDEF0ULL,
					0xFFFFFFFFFFFFFFFFULL
				};
				for (uint8_t i = 0; i < sizeof(testValues) / sizeof(testValues[0]); i++)
				{
					uint64_t v = testValues[i];
					uint16_t expected = Ref_U64ToU16(v);
					uint16_t result = IntegerSignal::U64ToU16(v);
					if (result != expected)
					{
						errorCount++;
						Serial.print(F("Error: U64ToU16("));
						IntegerSignal::Testing::PrintUInt64(v);
						Serial.print(F(") -> "));
						Serial.print(result);
						Serial.print(F(", expected "));
						Serial.println(expected);
					}
				}
				Serial.println(F("U64ToU16 tests passed."));
				return errorCount == 0;
			}

			static bool Test_U64ToU8()
			{
				Serial.println(F("Testing U64ToU8..."));
				uint32_t errorCount = 0;
				uint64_t testValues[] = {
					0ULL,
					1ULL,
					0x12345678ULL,
					0xFFFFFFFFULL,
					0x123456789ABCDEF0ULL,
					0xFFFFFFFFFFFFFFFFULL
				};
				for (uint8_t i = 0; i < sizeof(testValues) / sizeof(testValues[0]); i++)
				{
					uint64_t v = testValues[i];
					uint8_t expected = Ref_U64ToU8(v);
					uint8_t result = IntegerSignal::U64ToU8(v);
					if (result != expected)
					{
						errorCount++;
						Serial.print(F("Error: U64ToU8("));
						IntegerSignal::Testing::PrintUInt64(v);
						Serial.print(F(") -> "));
						Serial.print(result);
						Serial.print(F(", expected "));
						Serial.println(expected);
					}
				}
				Serial.println(F("U64ToU8 tests passed."));
				return errorCount == 0;
			}

			static bool Test_U32ToU8()
			{
				Serial.println(F("Testing U32ToU8..."));
				uint32_t errorCount = 0;
				uint32_t testValues[] = { 0UL, 1UL, 0x12345678UL, 0xFFFFFFFFUL };
				for (uint8_t i = 0; i < sizeof(testValues) / sizeof(testValues[0]); i++)
				{
					uint32_t v = testValues[i];
					uint8_t expected = Ref_U32ToU8(v);
					uint8_t result = IntegerSignal::U32ToU8(v);
					if (result != expected)
					{
						errorCount++;
						Serial.print(F("Error: U32ToU8("));
						Serial.print(v);
						Serial.print(F(") -> "));
						Serial.print(result);
						Serial.print(F(", expected "));
						Serial.println(expected);
					}
				}
				Serial.println(F("U32ToU8 tests passed."));
				return errorCount == 0;
			}

			static bool Test_U8ToU16()
			{
				Serial.println(F("Testing U8ToU16..."));
				uint32_t errorCount = 0;
				for (uint16_t v = 0; v <= 0xFF; v++)
				{
					uint8_t input = (uint8_t)v;
					uint16_t expected = Ref_U8ToU16(input);  // i.e., input * 257
					uint16_t result = IntegerSignal::U8ToU16(input);
					if (result != expected)
					{
						errorCount++;
						Serial.print(F("Error: U8ToU16("));
						Serial.print(input);
						Serial.print(F(") -> "));
						Serial.print(result);
						Serial.print(F(", expected "));
						Serial.println(expected);
					}
				}
				Serial.println(F("U8ToU16 tests passed."));
				return errorCount == 0;
			}

			static bool Test_U16ToU32()
			{
				Serial.println(F("Testing U16ToU32..."));
				uint32_t errorCount = 0;
				for (uint32_t v = 0; v <= 0xFFFF; v++)
				{
					uint16_t input = (uint16_t)v;
					uint32_t expected = Ref_U16ToU32(input);  // i.e., input * 65537
					uint32_t result = IntegerSignal::U16ToU32(input);
					if (result != expected)
					{
						errorCount++;
						Serial.print(F("Error: U16ToU32("));
						Serial.print(input);
						Serial.print(F(") -> "));
						Serial.print(result);
						Serial.print(F(", expected "));
						Serial.println(expected);
					}
				}
				Serial.println(F("U16ToU32 tests passed."));
				return errorCount == 0;
			}

			/////////////////////////////////////////////////////////////////////////////
			// Sampled Tests for Wide Ranges Using MaxIterations Template
			/////////////////////////////////////////////////////////////////////////////

			// Sample test for U32ToU64 conversion.
			// Domain: 32-bit input values from 0 to 0xFFFFFFFF.
			template<uint32_t MaxIterations>
			static bool Test_U32ToU64_Sample()
			{
				Serial.println(F("Starting sampled U32ToU64 tests..."));

				// Compute the step to cover the full 32-bit range.
				uint64_t step = 0xFFFFFFFFULL / (MaxIterations - 1);
				uint32_t errorCount = 0;
				for (uint32_t i = 0; i < MaxIterations; i++)
				{
					uint32_t value = (i == MaxIterations - 1) ? 0xFFFFFFFFUL : (uint32_t)(step * i);
					uint64_t expected = Ref_U32ToU64(value);
					uint64_t result = IntegerSignal::U32ToU64(value);
					if (result != expected)
					{
						Serial.print(F("U32ToU64 error: value="));
						Serial.print(value);
						Serial.print(F(" result="));
						IntegerSignal::Testing::PrintUInt64(result);
						Serial.print(F(", expected "));
						IntegerSignal::Testing::PrintUInt64(expected);
						Serial.println();
						errorCount++;
					}
					if (i % (MaxIterations / 10) == 0)
					{
						Serial.print(F("U32ToU64 progress: "));
						Serial.print(i);
						Serial.println(F(" samples tested..."));
					}
				}
				if (errorCount == 0)
					Serial.println(F("Sampled U32ToU64 tests passed."));
				else
				{
					Serial.print(F("Sampled U32ToU64 tests FAILED with "));
					Serial.print(errorCount);
					Serial.println(F(" errors."));
				}

				return errorCount == 0;
			}

			// Sample test for U16ToU64 conversion.
			// Domain: 16-bit input values from 0 to 0xFFFF.
			template<uint32_t MaxIterations>
			static bool Test_U16ToU64_Sample()
			{
				Serial.println(F("Starting sampled U16ToU64 tests..."));

				uint32_t step = 0xFFFF / (MaxIterations - 1);
				uint32_t errorCount = 0;
				for (uint32_t i = 0; i < MaxIterations; i++)
				{
					uint16_t value = (i == MaxIterations - 1) ? 0xFFFF : (uint16_t)(step * i);
					uint64_t expected = Ref_U16ToU64(value);
					uint64_t result = IntegerSignal::U16ToU64(value);
					if (result != expected)
					{
						Serial.print(F("U16ToU64 error: value="));
						Serial.print(value);
						Serial.print(F(" result="));
						IntegerSignal::Testing::PrintUInt64(result);
						Serial.print(F(", expected "));
						IntegerSignal::Testing::PrintUInt64(expected);
						Serial.println();
						errorCount++;
					}
					if (i % (MaxIterations / 10) == 0)
					{
						Serial.print(F("U16ToU64 progress: "));
						Serial.print(i);
						Serial.println(F(" samples tested..."));
					}
				}
				if (errorCount == 0)
					Serial.println(F("Sampled U16ToU64 tests passed."));
				else
				{
					Serial.print(F("Sampled U16ToU64 tests FAILED with "));
					Serial.print(errorCount);
					Serial.println(F(" errors."));
				}

				return errorCount == 0;
			}

			/////////////////////////////////////////////////////////////////////////////
			// Convenience Run Functions
			/////////////////////////////////////////////////////////////////////////////
			template<uint32_t MaxIterations_U32ToU64 = 1000000, uint32_t MaxIterations_U16ToU64 = 10000>
			static bool RunTests()
			{
				bool pass = true;

				// Basic fixed-value tests:
				pass &= Test_U64ToU32();
				pass &= Test_U32ToU16();
				pass &= Test_U16ToU8();
				pass &= Test_U64ToU16();
				pass &= Test_U64ToU8();
				pass &= Test_U32ToU8();
				pass &= Test_U8ToU16();
				pass &= Test_U16ToU32();

				// Sampled tests for wide-range conversions:
				pass &= Test_U32ToU64_Sample<MaxIterations_U32ToU64>();
				pass &= Test_U16ToU64_Sample<MaxIterations_U16ToU64>();

				// Exhaustive tests for smaller ranges:
				pass &= Test_Exhaustive_U8();
				pass &= Test_Exhaustive_U16();

				return pass;
			}

			// Run only exhaustive tests.
			static bool RunExhaustive()
			{
				bool pass = true;
				pass &= Test_Exhaustive_U8();
				pass &= Test_Exhaustive_U16();
				//pass &= Test_Exhaustive_U32();
				return pass;
			}
		}
	}
}
#endif