// IntegerSignal.h

#ifndef _INTEGER_SIGNAL_TESTING_h
#define _INTEGER_SIGNAL_TESTING_h

#include <Arduino.h>

#ifndef INTEGER_SIGNAL_TEST_VERBOSE
#define INTEGER_SIGNAL_TEST_VERBOSE 0
#endif

/// <summary>
/// Tests the square root result against a slow but accurate reference implementation.
/// </summary>
namespace IntegerSignal::Testing
{
	struct TestLogger
	{
		static void PrintResult(const __FlashStringHelper* name, const bool pass)
		{
			Serial.print(name);
			Serial.println(pass ? F(": PASS") : F(": FAIL"));
		}

		static void PrintSuite(const __FlashStringHelper* name)
		{
			Serial.println(name);
		}

		static void PrintSuiteResult(const __FlashStringHelper* name, const bool pass)
		{
			PrintResult(name, pass);
		}

		static bool Report(const __FlashStringHelper* name, const bool pass)
		{
			PrintResult(name, pass);
			return pass;
		}

		static constexpr bool Verbose()
		{
			return INTEGER_SIGNAL_TEST_VERBOSE != 0;
		}
	};

	// Prints a uint64_t value in decimal format.
	void PrintUInt64(uint64_t value)
	{
		// Allocate a buffer large enough for 20 digits (max for 2^64 - 1) plus null terminator.
		char buffer[21];
		buffer[20] = '\0';
		int idx = 20;

		// Special-case: If value is 0, print "0" immediately.
		if (value == 0)
		{
			Serial.print("0");
			return;
		}
		while (value > 0 && idx > 0) {
			buffer[--idx] = '0' + (value % 10);
			value /= 10;
		}
		Serial.print(&buffer[idx]);
	}
}
#endif