#ifndef _UINT_SIZE_TEST_h
#define _UINT_SIZE_TEST_h

#include <IntegerSignalTesting.h>

// Include for is_same definition.
#include "TypeTraitsTest.h"


namespace IntegerSignal
{
	namespace UIntSize
	{
		namespace Test
		{
			// Helper to print a compact error message
			static void PrintTypeError(const char* test, uint64_t value, const char* expected)
			{
				Serial.print(F("TypeErr:"));
				Serial.print(test);
				Serial.print(F("("));
				IntegerSignal::Testing::PrintUInt64(value);
				Serial.print(F(")!= "));
				Serial.println(expected);
			}

			static bool Test_uint_type_points()
			{
				const char* test = "uint_type";
				uint32_t errorCount = 0;

				if (!is_same<typename uint_type<0>::type, uint8_t>::value) {
					PrintTypeError(test, 0, "uint8_t"); errorCount++;
				}
				if (!is_same<typename uint_type<42>::type, uint8_t>::value) {
					PrintTypeError(test, 42, "uint8_t"); errorCount++;
				}
				if (!is_same<typename uint_type<UINT8_MAX>::type, uint8_t>::value) {
					PrintTypeError(test, UINT8_MAX, "uint8_t"); errorCount++;
				}
				if (!is_same<typename uint_type<UINT8_MAX + 1>::type, uint16_t>::value) {
					PrintTypeError(test, UINT8_MAX + 1, "uint16_t"); errorCount++;
				}
				if (!is_same<typename uint_type<12345>::type, uint16_t>::value) {
					PrintTypeError(test, 12345, "uint16_t"); errorCount++;
				}
				if (!is_same<typename uint_type<UINT16_MAX>::type, uint16_t>::value) {
					PrintTypeError(test, UINT16_MAX, "uint16_t"); errorCount++;
				}
				if (!is_same<typename uint_type<uint32_t(UINT16_MAX) + 1>::type, uint32_t>::value) {
					PrintTypeError(test, UINT16_MAX + 1, "uint32_t"); errorCount++;
				}
				if (!is_same<typename uint_type<1000000>::type, uint32_t>::value) {
					PrintTypeError(test, 1000000, "uint32_t"); errorCount++;
				}
				if (!is_same<typename uint_type<UINT32_MAX>::type, uint32_t>::value) {
					PrintTypeError(test, UINT32_MAX, "uint32_t"); errorCount++;
				}
				if (!is_same<typename uint_type<uint64_t(UINT32_MAX) + 1ULL>::type, uint64_t>::value) {
					PrintTypeError(test, UINT32_MAX + 1ULL, "uint64_t"); errorCount++;
				}
				if (!is_same<typename uint_type<1000000000000ULL>::type, uint64_t>::value) {
					PrintTypeError(test, 1000000000000ULL, "uint64_t"); errorCount++;
				}
				if (!is_same<typename uint_type<UINT64_MAX>::type, uint64_t>::value) {
					PrintTypeError(test, UINT64_MAX, "uint64_t"); errorCount++;
				}

				if (errorCount == 0)
					Serial.println(F("UIntSize::uint_type tests passed."));
				else
					Serial.println(F("UIntSize::uint_type tests FAILED."));
				return errorCount == 0;
			}

			static bool Test_uint_least_type_points()
			{
				const char* test = "uint_least_type";
				uint32_t errorCount = 0;

				if (!is_same<typename uint_least_type<0>::type, uint_least8_t>::value) {
					PrintTypeError(test, 0, "uint_least8_t"); errorCount++;
				}
				if (!is_same<typename uint_least_type<42>::type, uint_least8_t>::value) {
					PrintTypeError(test, 42, "uint_least8_t"); errorCount++;
				}
				if (!is_same<typename uint_least_type<UINT8_MAX>::type, uint_least8_t>::value) {
					PrintTypeError(test, UINT8_MAX, "uint_least8_t"); errorCount++;
				}
				if (!is_same<typename uint_least_type<UINT8_MAX + 1>::type, uint_least16_t>::value) {
					PrintTypeError(test, UINT8_MAX + 1, "uint_least16_t"); errorCount++;
				}
				if (!is_same<typename uint_least_type<12345>::type, uint_least16_t>::value) {
					PrintTypeError(test, 12345, "uint_least16_t"); errorCount++;
				}
				if (!is_same<typename uint_least_type<UINT16_MAX>::type, uint_least16_t>::value) {
					PrintTypeError(test, UINT16_MAX, "uint_least16_t"); errorCount++;
				}
				if (!is_same<typename uint_least_type<uint32_t(UINT16_MAX) + 1>::type, uint_least32_t>::value) {
					PrintTypeError(test, UINT16_MAX + 1, "uint_least32_t"); errorCount++;
				}
				if (!is_same<typename uint_least_type<1000000>::type, uint_least32_t>::value) {
					PrintTypeError(test, 1000000, "uint_least32_t"); errorCount++;
				}
				if (!is_same<typename uint_least_type<UINT32_MAX>::type, uint_least32_t>::value) {
					PrintTypeError(test, UINT32_MAX, "uint_least32_t"); errorCount++;
				}
				if (!is_same<typename uint_least_type<uint64_t(UINT32_MAX) + 1ULL>::type, uint_least64_t>::value) {
					PrintTypeError(test, UINT32_MAX + 1ULL, "uint_least64_t"); errorCount++;
				}
				if (!is_same<typename uint_least_type<1000000000000ULL>::type, uint_least64_t>::value) {
					PrintTypeError(test, 1000000000000ULL, "uint_least64_t"); errorCount++;
				}
				if (!is_same<typename uint_least_type<UINT64_MAX>::type, uint_least64_t>::value) {
					PrintTypeError(test, UINT64_MAX, "uint_least64_t"); errorCount++;
				}

				if (errorCount == 0)
					Serial.println(F("UIntLeastSize::uint_least_type tests passed."));
				else
					Serial.println(F("UIntLeastSize::uint_least_type tests FAILED."));
				return errorCount == 0;
			}

			static bool Test_uint_fast_type_points()
			{
				const char* test = "uint_fast_type";
				uint32_t errorCount = 0;

				if (!is_same<typename uint_fast_type<uint64_t(0)>::type, uint_fast8_t>::value) {
					PrintTypeError(test, 0, "uint_fast8_t"); errorCount++;
				}
				if (!is_same<typename uint_fast_type<42>::type, uint_fast8_t>::value) {
					PrintTypeError(test, 42, "uint_fast8_t"); errorCount++;
				}
				if (!is_same<typename uint_fast_type<uint64_t(UINT8_MAX)>::type, uint_fast8_t>::value) {
					PrintTypeError(test, UINT8_MAX, "uint_fast8_t"); errorCount++;
				}
				if (!is_same<typename uint_fast_type<uint64_t(UINT8_MAX) + 1>::type, uint_fast16_t>::value) {
					PrintTypeError(test, UINT8_MAX + 1, "uint_fast16_t"); errorCount++;
				}
				if (!is_same<typename uint_fast_type<uint64_t(12345)>::type, uint_fast16_t>::value) {
					PrintTypeError(test, 12345, "uint_fast16_t"); errorCount++;
				}
				if (!is_same<typename uint_fast_type<uint64_t(UINT16_MAX)>::type, uint_fast16_t>::value) {
					PrintTypeError(test, UINT16_MAX, "uint_fast16_t"); errorCount++;
				}
				if (!is_same<typename uint_fast_type<uint64_t(UINT16_MAX) + 1>::type, uint_fast32_t>::value) {
					PrintTypeError(test, UINT16_MAX + 1, "uint_fast32_t"); errorCount++;
				}
				if (!is_same<typename uint_fast_type<uint64_t(1000000)>::type, uint_fast32_t>::value) {
					PrintTypeError(test, 1000000, "uint_fast32_t"); errorCount++;
				}
				if (!is_same<typename uint_fast_type<uint64_t(UINT32_MAX)>::type, uint_fast32_t>::value) {
					PrintTypeError(test, UINT32_MAX, "uint_fast32_t"); errorCount++;
				}
				if (!is_same<typename uint_fast_type<uint64_t(UINT32_MAX) + 1ULL>::type, uint_fast64_t>::value) {
					PrintTypeError(test, UINT32_MAX + 1ULL, "uint_fast64_t"); errorCount++;
				}
				if (!is_same<typename uint_fast_type<uint64_t(1000000000000ULL)>::type, uint_fast64_t>::value) {
					PrintTypeError(test, 1000000000000ULL, "uint_fast64_t"); errorCount++;
				}
				if (!is_same<typename uint_fast_type<UINT64_MAX>::type, uint_fast64_t>::value) {
					PrintTypeError(test, UINT64_MAX, "uint_fast64_t"); errorCount++;
				}

				if (errorCount == 0)
					Serial.println(F("UIntFastSize::uint_fast_type tests passed."));
				else
					Serial.println(F("UIntFastSize::uint_fast_type tests FAILED."));
				return errorCount == 0;
			}

			static bool RunTests()
			{
				bool pass = true;
				pass &= Test_uint_type_points();
				pass &= Test_uint_least_type_points();
				pass &= Test_uint_fast_type_points();
				return pass;
			}
		}
	}
}
#endif