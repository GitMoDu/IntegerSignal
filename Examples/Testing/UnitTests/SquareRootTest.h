// SquareRootTest.h

#ifndef _SQUARE_ROOT_TEST_h
#define _SQUARE_ROOT_TEST_h

#include <IntegerSignalTesting.h>

namespace IntegerSignal::SquareRoot::Test
{
	// Reference for 64-bit: returns floor(sqrt(value)) in the range 0..4294967295.
	static uint32_t RefSqrt(uint64_t value)
	{
		uint32_t res = 0;
		// Iterate from MSB (bit 31) down to LSB (bit 0)
		for (int8_t i = 31; i >= 0; i--) {
			uint32_t candidate = res | (1UL << i);
			// Use 64-bit multiplication for candidate squared.
			if ((uint64_t)candidate * candidate <= value) {
				res = candidate;
			}
		}
		return res;
	}

	// Exhaustive test for 16-bit inputs (0..65535)
	static bool TestSqrt16Exhaustive()
	{
		Serial.println(F("Starting exhaustive 16-bit sqrt tests..."));

		uint32_t errorCount = 0;
		for (uint32_t v = 0; v <= UINT16_MAX; v++) {
			const uint16_t value = (uint16_t)v;
			const uint8_t result = SquareRoot16(value);
			const uint8_t refResult = RefSqrt(value);
			if (result != refResult)
			{
				Serial.print(F("16-bit error: value="));
				Serial.print(value);
				Serial.print(F(" result="));
				Serial.print(result);
				Serial.print(F(" ref="));
				Serial.println(refResult);
				errorCount++;
			}
			if ((v % 6553) == 0)
			{
				Serial.print(F("16-bit progress: "));
				Serial.print(v);
				Serial.println(F(" values tested..."));
			}
		}
		if (errorCount == 0)
		{
			Serial.println(F("All 16-bit tests PASSED."));
		}
		else
		{
			Serial.print(F("16-bit tests FAILED. Errors: "));
			Serial.println(errorCount);
		}

		return errorCount == 0;
	}

	static bool TestSqrt32Exhaustive()
	{
		Serial.println(F("Starting Exhaustive 32-bit sqrt tests..."));

		uint32_t errorCount = 0;
		for (uint64_t value = 0; value <= UINT32_MAX; value++)
		{
			const uint16_t result = SquareRoot32(value);
			const uint16_t refResult = RefSqrt(value);
			if (result != refResult)
			{
				Serial.print(F("32-bit error: value="));
				Serial.print((uint32_t)value);
				Serial.print(F(" result="));
				Serial.print(result);
				Serial.print(F(" ref="));
				Serial.println(refResult);
				errorCount++;
			}
			if (value % UINT16_MAX == 0)
			{
				Serial.print(F("32-bit progress: "));
				Serial.print((uint32_t)value);
				Serial.println(F(" values tested..."));
			}
		}

		if (errorCount == 0)
		{
			Serial.println(F("All 32-bit tests PASSED."));
		}
		else
		{
			Serial.print(F("32-bit tests FAILED. Errors: "));
			Serial.println(errorCount);
		}

		return errorCount > 0;
	}

	template<uint32_t MaxIterations>
	static bool TestSqrt32Sample()
	{
		Serial.println(F("Starting sampled 32-bit sqrt tests..."));

		uint32_t errorCount = 0;
		for (uint32_t i = 0; i < MaxIterations; i++)
		{
			// Map i from [0, iterations-1] to the full 32-bit range.
			const uint32_t value = (uint32_t)(((uint64_t)i * 0xFFFFFFFFULL) / (MaxIterations - 1));
			const uint16_t result = SquareRoot32(value);
			const uint16_t refResult = RefSqrt(value);
			if (result != refResult)
			{
				Serial.print(F("32-bit error: value="));
				Serial.print(value);
				Serial.print(F(" result="));
				Serial.print(result);
				Serial.print(F(" ref="));
				Serial.println(refResult);
				errorCount++;
			}
			if (i % (MaxIterations / 10) == 0)
			{
				Serial.print(F("32-bit progress: "));
				Serial.print(i);
				Serial.println(F(" samples tested..."));
			}
		}

		if (errorCount == 0)
		{
			Serial.println(F("All 32-bit tests PASSED."));
		}
		else
		{
			Serial.print(F("32-bit tests FAILED. Errors: "));
			Serial.println(errorCount);
		}

		return errorCount == 0;
	}

	// Sampled test for 64-bit inputs. Tests 50K values distributed over the 64-bit range.
	template<uint32_t MaxIterations>
	static bool TestSqrt64Sample()
	{
		Serial.println(F("Starting sampled 64-bit sqrt tests..."));

		uint64_t step = 0xFFFFFFFFFFFFFFFFULL / (MaxIterations - 1);
		uint32_t errorCount = 0;

		for (uint32_t i = 0; i < MaxIterations; i++)
		{
			const uint64_t value = (i == MaxIterations - 1) ? 0xFFFFFFFFFFFFFFFFULL : step * i;

			const uint32_t result = SquareRoot64(value);
			const uint32_t refResult = RefSqrt(value);
			if (result != refResult)
			{
				Serial.print(F("64-bit error: value="));
				IntegerSignal::Testing::PrintUInt64(value);
				Serial.print(F(" result="));
				Serial.print(result);
				Serial.print(F(" ref="));
				Serial.println(refResult);
				errorCount++;
			}
			if (i % (MaxIterations / 10) == 0)
			{
				Serial.print(F("64-bit progress: "));
				Serial.print(i);
				Serial.println(F(" samples tested..."));
			}
		}

		if (errorCount == 0)
		{
			Serial.println(F("All 64-bit tests PASSED."));
		}
		else
		{
			Serial.print(F("64-bit tests FAILED. Errors: "));
			Serial.println(errorCount);
		}

		return errorCount == 0;
	}

	template<uint32_t MaxIterations = 50000>
	static bool RunTests()
	{
		bool pass = true;

		pass &= TestSqrt16Exhaustive();
		pass &= TestSqrt32Sample<MaxIterations>();
		pass &= TestSqrt64Sample<MaxIterations>();

		if (!pass)
		{
			Serial.println(F("SquareRoot tests FAILED."));
		}

		return pass;
	}

	static bool RunExhaustive()
	{
		bool pass = true;

		pass &= TestSqrt16Exhaustive();
		pass &= TestSqrt32Exhaustive();

		return pass;
	}
}

#endif

