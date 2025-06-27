#ifndef _UINT_SIZE_TEST_h
#define _UINT_SIZE_TEST_h

#include <IntegerSignalTesting.h>
#include <type_traits>
#include <stdint.h>

namespace IntegerSignal::UIntSize::Test
{
	static bool Test_uint_type_points()
	{
		Serial.println(F("Testing UIntSize::uint_type type selection at key points..."));
		uint32_t errorCount = 0;

		// uint8_t range
		if (!std::is_same<typename uint_type<0>::type, uint8_t>::value) {
			Serial.println(F("Error: uint_type<0> is not uint8_t"));
			errorCount++;
		}
		if (!std::is_same<typename uint_type<42>::type, uint8_t>::value) {
			Serial.println(F("Error: uint_type<42> is not uint8_t"));
			errorCount++;
		}
		if (!std::is_same<typename uint_type<UINT8_MAX>::type, uint8_t>::value) {
			Serial.println(F("Error: uint_type<UINT8_MAX> is not uint8_t"));
			errorCount++;
		}

		// uint16_t range
		if (!std::is_same<typename uint_type<UINT8_MAX + 1>::type, uint16_t>::value) {
			Serial.println(F("Error: uint_type<UINT8_MAX+1> is not uint16_t"));
			errorCount++;
		}
		if (!std::is_same<typename uint_type<12345>::type, uint16_t>::value) {
			Serial.println(F("Error: uint_type<12345> is not uint16_t"));
			errorCount++;
		}
		if (!std::is_same<typename uint_type<UINT16_MAX>::type, uint16_t>::value) {
			Serial.println(F("Error: uint_type<UINT16_MAX> is not uint16_t"));
			errorCount++;
		}

		// uint32_t range
		if (!std::is_same<typename uint_type<UINT16_MAX + 1>::type, uint32_t>::value) {
			Serial.println(F("Error: uint_type<UINT16_MAX+1> is not uint32_t"));
			errorCount++;
		}
		if (!std::is_same<typename uint_type<1000000>::type, uint32_t>::value) {
			Serial.println(F("Error: uint_type<1000000> is not uint32_t"));
			errorCount++;
		}
		if (!std::is_same<typename uint_type<UINT32_MAX>::type, uint32_t>::value) {
			Serial.println(F("Error: uint_type<UINT32_MAX> is not uint32_t"));
			errorCount++;
		}

		// uint64_t range
		if (!std::is_same<typename uint_type<UINT32_MAX + 1ULL>::type, uint64_t>::value) {
			Serial.println(F("Error: uint_type<UINT32_MAX+1> is not uint64_t"));
			errorCount++;
		}
		if (!std::is_same<typename uint_type<1000000000000ULL>::type, uint64_t>::value) {
			Serial.println(F("Error: uint_type<1000000000000> is not uint64_t"));
			errorCount++;
		}
		if (!std::is_same<typename uint_type<UINT64_MAX>::type, uint64_t>::value) {
			Serial.println(F("Error: uint_type<UINT64_MAX> is not uint64_t"));
			errorCount++;
		}

		if (errorCount == 0)
			Serial.println(F("UIntSize::uint_type key point tests passed."));
		else
		{
			Serial.print(F("UIntSize::uint_type key point tests FAILED with "));
			Serial.print(errorCount);
			Serial.println(F(" errors."));
		}
		return errorCount == 0;
	}

	static bool Test_uint_least_type_points()
	{
		Serial.println(F("Testing UIntLeastSize::uint_least_type type selection at key points..."));
		uint32_t errorCount = 0;

		// uint_least8_t range
		if (!std::is_same<typename uint_least_type<0>::type, uint_least8_t>::value) {
			Serial.println(F("Error: uint_least_type<0> is not uint_least8_t"));
			errorCount++;
		}
		if (!std::is_same<typename uint_least_type<42>::type, uint_least8_t>::value) {
			Serial.println(F("Error: uint_least_type<42> is not uint_least8_t"));
			errorCount++;
		}
		if (!std::is_same<typename uint_least_type<UINT8_MAX>::type, uint_least8_t>::value) {
			Serial.println(F("Error: uint_least_type<UINT8_MAX> is not uint_least8_t"));
			errorCount++;
		}

		// uint_least16_t range
		if (!std::is_same<typename uint_least_type<UINT8_MAX + 1>::type, uint_least16_t>::value) {
			Serial.println(F("Error: uint_least_type<UINT8_MAX+1> is not uint_least16_t"));
			errorCount++;
		}
		if (!std::is_same<typename uint_least_type<12345>::type, uint_least16_t>::value) {
			Serial.println(F("Error: uint_least_type<12345> is not uint_least16_t"));
			errorCount++;
		}
		if (!std::is_same<typename uint_least_type<UINT16_MAX>::type, uint_least16_t>::value) {
			Serial.println(F("Error: uint_least_type<UINT16_MAX> is not uint_least16_t"));
			errorCount++;
		}

		// uint_least32_t range
		if (!std::is_same<typename uint_least_type<UINT16_MAX + 1>::type, uint_least32_t>::value) {
			Serial.println(F("Error: uint_least_type<UINT16_MAX+1> is not uint_least32_t"));
			errorCount++;
		}
		if (!std::is_same<typename uint_least_type<1000000>::type, uint_least32_t>::value) {
			Serial.println(F("Error: uint_least_type<1000000> is not uint_least32_t"));
			errorCount++;
		}
		if (!std::is_same<typename uint_least_type<UINT32_MAX>::type, uint_least32_t>::value) {
			Serial.println(F("Error: uint_least_type<UINT32_MAX> is not uint_least32_t"));
			errorCount++;
		}

		// uint_least64_t range
		if (!std::is_same<typename uint_least_type<UINT32_MAX + 1ULL>::type, uint_least64_t>::value) {
			Serial.println(F("Error: uint_least_type<UINT32_MAX+1> is not uint_least64_t"));
			errorCount++;
		}
		if (!std::is_same<typename uint_least_type<1000000000000ULL>::type, uint_least64_t>::value) {
			Serial.println(F("Error: uint_least_type<1000000000000> is not uint_least64_t"));
			errorCount++;
		}
		if (!std::is_same<typename uint_least_type<UINT64_MAX>::type, uint_least64_t>::value) {
			Serial.println(F("Error: uint_least_type<UINT64_MAX> is not uint_least64_t"));
			errorCount++;
		}

		if (errorCount == 0)
			Serial.println(F("UIntLeastSize::uint_least_type key point tests passed."));
		else
		{
			Serial.print(F("UIntLeastSize::uint_least_type key point tests FAILED with "));
			Serial.print(errorCount);
			Serial.println(F(" errors."));
		}
		return errorCount == 0;
	}

	static bool Test_uint_fast_type_points()
	{
		Serial.println(F("Testing UIntFastSize::uint_fast_type type selection at key points..."));
		uint32_t errorCount = 0;

		// uint_fast8_t range
		if (!std::is_same<typename uint_fast_type<0>::type, uint_fast8_t>::value) {
			Serial.println(F("Error: uint_fast_type<0> is not uint_fast8_t"));
			errorCount++;
		}
		if (!std::is_same<typename uint_fast_type<42>::type, uint_fast8_t>::value) {
			Serial.println(F("Error: uint_fast_type<42> is not uint_fast8_t"));
			errorCount++;
		}
		if (!std::is_same<typename uint_fast_type<UINT8_MAX>::type, uint_fast8_t>::value) {
			Serial.println(F("Error: uint_fast_type<UINT8_MAX> is not uint_fast8_t"));
			errorCount++;
		}

		// uint_fast16_t range
		if (!std::is_same<typename uint_fast_type<UINT8_MAX + 1>::type, uint_fast16_t>::value) {
			Serial.println(F("Error: uint_fast_type<UINT8_MAX+1> is not uint_fast16_t"));
			errorCount++;
		}
		if (!std::is_same<typename uint_fast_type<12345>::type, uint_fast16_t>::value) {
			Serial.println(F("Error: uint_fast_type<12345> is not uint_fast16_t"));
			errorCount++;
		}
		if (!std::is_same<typename uint_fast_type<UINT16_MAX>::type, uint_fast16_t>::value) {
			Serial.println(F("Error: uint_fast_type<UINT16_MAX> is not uint_fast16_t"));
			errorCount++;
		}

		// uint_fast32_t range
		if (!std::is_same<typename uint_fast_type<UINT16_MAX + 1>::type, uint_fast32_t>::value) {
			Serial.println(F("Error: uint_fast_type<UINT16_MAX+1> is not uint_fast32_t"));
			errorCount++;
		}
		if (!std::is_same<typename uint_fast_type<1000000>::type, uint_fast32_t>::value) {
			Serial.println(F("Error: uint_fast_type<1000000> is not uint_fast32_t"));
			errorCount++;
		}
		if (!std::is_same<typename uint_fast_type<UINT32_MAX>::type, uint_fast32_t>::value) {
			Serial.println(F("Error: uint_fast_type<UINT32_MAX> is not uint_fast32_t"));
			errorCount++;
		}

		// uint_fast64_t range
		if (!std::is_same<typename uint_fast_type<UINT32_MAX + 1ULL>::type, uint_fast64_t>::value) {
			Serial.println(F("Error: uint_fast_type<UINT32_MAX+1> is not uint_fast64_t"));
			errorCount++;
		}
		if (!std::is_same<typename uint_fast_type<1000000000000ULL>::type, uint_fast64_t>::value) {
			Serial.println(F("Error: uint_fast_type<1000000000000> is not uint_fast64_t"));
			errorCount++;
		}
		if (!std::is_same<typename uint_fast_type<UINT64_MAX>::type, uint_fast64_t>::value) {
			Serial.println(F("Error: uint_fast_type<UINT64_MAX> is not uint_fast64_t"));
			errorCount++;
		}

		if (errorCount == 0)
			Serial.println(F("UIntFastSize::uint_fast_type key point tests passed."));
		else
		{
			Serial.print(F("UIntFastSize::uint_fast_type key point tests FAILED with "));
			Serial.print(errorCount);
			Serial.println(F(" errors."));
		}
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

#endif