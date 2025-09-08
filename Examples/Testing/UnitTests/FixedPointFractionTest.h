#ifndef _FIXED_POINT_FRACTION_TEST_h
#define _FIXED_POINT_FRACTION_TEST_h

#include <IntegerSignalTesting.h>

namespace IntegerSignal::FixedPoint::Fraction::Test
{
	// ===================== REFERENCE FUNCTIONS =====================

	// Unsigned reference functions
	template<typename T>
	static T RefUFraction8(const T value, const uint8_t scalar)
	{
		constexpr uint8_t SCALE_UNIT = 128;
		return (static_cast<uint64_t>(value) * scalar) / SCALE_UNIT;
	}

	template<typename T>
	static T RefUFraction16(const T value, const uint16_t scalar)
	{
		constexpr uint16_t SCALE_UNIT = 32768;
		return (static_cast<uint64_t>(value) * scalar) / SCALE_UNIT;
	}

	template<typename T>
	static T RefUFraction32(const T value, const uint32_t scalar)
	{
		constexpr uint32_t SCALE_UNIT = 2147483648u;
		return (static_cast<uint64_t>(value) * scalar) / SCALE_UNIT;
	}

	// Signed reference functions
	template<typename T>
	static T RefSFraction8(const T value, const int8_t scalar)
	{
		constexpr int8_t SCALE_UNIT = 64;
		return (static_cast<int64_t>(value) * scalar) / SCALE_UNIT;
	}

	template<typename T>
	static T RefSFraction16(const T value, const int16_t scalar)
	{
		constexpr int16_t SCALE_UNIT = 16384;
		return (static_cast<int64_t>(value) * scalar) / SCALE_UNIT;
	}

	template<typename T>
	static T RefSFraction32(const T value, const int32_t scalar)
	{
		constexpr int32_t SCALE_UNIT = 1073741824;
		return (static_cast<int64_t>(value) * scalar) / SCALE_UNIT;
	}

	// Reference interpolation functions
	template<typename T>
	static T RefUInterpolate8(const T from, const T to, const uint8_t scalar)
	{
		constexpr uint8_t SCALE_UNIT = 128;
		return (static_cast<uint64_t>(from) * (SCALE_UNIT - scalar) +
			static_cast<uint64_t>(to) * scalar) / SCALE_UNIT;
	}

	template<typename T>
	static T RefSInterpolate8(const T from, const T to, const int8_t scalar)
	{
		constexpr int8_t SCALE_UNIT = 64;
		return (static_cast<int64_t>(from) * (SCALE_UNIT - (scalar > 0 ? scalar : 0)) +
			static_cast<int64_t>(to) * (scalar > 0 ? scalar : 0)) / SCALE_UNIT;
	}

	// ===================== UNSIGNED TESTS =====================

	// Exhaustive test for UFraction8
	static bool TestUFraction8Exhaustive()
	{
		Serial.println(F("Starting exhaustive UFraction8 tests..."));
		uint32_t errorCount = 0;
		for (uint16_t v = 0; v <= UINT8_MAX; v++)
		{
			for (uint16_t s = 0; s <= UINT8_MAX; s++)
			{
				const uint8_t value = (uint8_t)v;
				const uint8_t scalar = (uint8_t)s;
				const uint8_t result = UFraction8::Fraction(scalar, value);
				const uint8_t refResult = RefUFraction8(value, scalar);
				if (result != refResult)
				{
					Serial.print(F("UFraction8 error: value="));
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
			if ((v % 16) == 0)
			{
				Serial.print(F("UFraction8 progress: "));
				Serial.print(v);
				Serial.println(F(" values tested..."));
			}
		}
		if (errorCount == 0)
			Serial.println(F("All UFraction8 tests PASSED."));
		else
		{
			Serial.print(F("UFraction8 tests FAILED. Errors: "));
			Serial.println(errorCount);
		}
		return errorCount == 0;
	}

	// Test UInterpolate8 with systematic samples
	static bool TestUInterpolate8()
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
	static bool TestUFraction8Types()
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

	// Sampled test for UFraction16
	template<uint32_t MaxIterations = 50000>
	static bool TestUFraction16Sample()
	{
		const uint32_t iterations = min(MaxIterations, (uint32_t)UINT16_MAX);
		Serial.println(F("Starting sampled UFraction16 tests..."));
		uint32_t errorCount = 0;

		// Test specific edge cases
		const uint16_t edgeScalars[] = { 0, 1, 16383, 16384, 32767, 32768, 65535 };
		const uint16_t edgeValues[] = { 0, 1, 127, 128, 255, 256, 32767, 32768, 65535 };

		for (uint16_t scalar : edgeScalars)
		{
			for (uint16_t value : edgeValues)
			{
				const uint16_t result = UFraction16::Fraction(scalar, value);
				const uint16_t refResult = RefUFraction16(value, scalar);
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

		// Test with sampling
		for (uint32_t s = 0; s <= UINT16_MAX; s += 257)
		{
			const uint16_t scalar = (uint16_t)s;
			for (uint16_t v = 0; v <= UINT8_MAX; v++)
			{
				const uint16_t value = (uint16_t)v;
				const uint16_t result = UFraction16::Fraction(scalar, value);
				const uint16_t refResult = RefUFraction16(value, scalar);
				if (result != refResult)
				{
					Serial.print(F("UFraction16 error: value="));
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
			if ((s % 4096) == 0)
			{
				Serial.print(F("UFraction16 progress: "));
				Serial.print(s);
				Serial.println(F(" scalars tested..."));
			}
		}

		if (errorCount == 0)
			Serial.println(F("All UFraction16 tests PASSED."));
		else
		{
			Serial.print(F("UFraction16 tests FAILED. Errors: "));
			Serial.println(errorCount);
		}
		return errorCount == 0;
	}

	// ===================== SIGNED TESTS =====================

	// Test SFraction8 with key edge cases
	static bool TestSFraction8EdgeCases()
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
				const int8_t refResult = RefSFraction8(value, scalar);

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

	// Test SInterpolate8 with systematic samples
	static bool TestSInterpolate8()
	{
		Serial.println(F("Starting SFraction8 interpolation tests..."));
		uint32_t errorCount = 0;

		const int8_t testValues[] = { -128, -64, -1, 0, 1, 64, 127 };

		for (int8_t from : testValues)
		{
			for (int8_t to : testValues)
			{
				for (int16_t s = -64; s <= 64; s += 16)
				{
					const Fraction8::scalar_t scalar = (int8_t)s;
					const int8_t result = Fraction8::Interpolate<int8_t>(scalar, from, to);
					const int8_t refResult = RefSInterpolate8(from, to, scalar);

					if (result != refResult)
					{
						Serial.print(F("SInterpolate8 error: from="));
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
			Serial.println(F("All SInterpolate8 tests PASSED."));
		else
		{
			Serial.print(F("SInterpolate8 tests FAILED. Errors: "));
			Serial.println(errorCount);
		}
		return errorCount == 0;
	}

	// Sampled test for UFraction32
	template<uint32_t MaxIterations = 50000>
	static bool TestUFraction32Sample()
	{
		const uint32_t iterations = min(MaxIterations, (uint32_t)UINT16_MAX);
		Serial.println(F("Starting sampled UFraction32 tests..."));
		uint32_t errorCount = 0;

		// Test specific edge cases
		const uint32_t edgeScalars[] = { 0, 1, 1073741823, 1073741824, 2147483647, 2147483648u, 4294967295u };
		const uint32_t edgeValues[] = { 0, 1, 127, 128, 255, 256, 65535, 65536, 16777215, 16777216, 2147483647, 2147483648u };

		for (uint32_t scalar : edgeScalars)
		{
			for (uint32_t value : edgeValues)
			{
				const uint32_t result = UFraction32::Fraction(scalar, value);
				const uint32_t refResult = RefUFraction32(value, scalar);
				if (result != refResult)
				{
					Serial.print(F("UFraction32 edge error: value="));
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

		// Test with sampling (using larger steps due to 32-bit range)
		for (uint64_t s = 0; s <= UINT32_MAX; s += 179424673)  // Large prime step to get good coverage
		{
			const uint32_t scalar = (uint32_t)s;
			for (uint32_t v = 0; v <= UINT8_MAX; v += 16)  // Sample values
			{
				const uint32_t value = (uint32_t)v;
				const uint32_t result = UFraction32::Fraction(scalar, value);
				const uint32_t refResult = RefUFraction32(value, scalar);
				if (result != refResult)
				{
					Serial.print(F("UFraction32 error: value="));
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

			// Also test some larger values
			const uint32_t largeValues[] = { 65535, 16777215, 2147483647 };
			for (uint32_t value : largeValues)
			{
				const uint32_t result = UFraction32::Fraction(scalar, value);
				const uint32_t refResult = RefUFraction32(value, scalar);
				if (result != refResult)
				{
					Serial.print(F("UFraction32 large value error: value="));
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

			if ((s % 536870912) == 0)  // Report progress at 1/8 of the range
			{
				Serial.print(F("UFraction32 progress: "));
				Serial.print((uint32_t)(s >> 30));  // Show progress as 0-4
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

	// ===================== RUN ALL TESTS =====================

	template<uint32_t MaxIterations = 50000>
	static bool RunTests()
	{
		bool pass = true;

		// Unsigned fraction tests
		pass &= TestUFraction8Exhaustive();
		pass &= TestUFraction16Sample<MaxIterations>();
		pass &= TestUFraction32Sample<MaxIterations>();
		pass &= TestUFraction8Types();
		//pass &= TestUInterpolate8();

		// Signed fraction tests
		pass &= TestSFraction8EdgeCases();
		//pass &= TestSInterpolate8();

		// Summary
		if (pass)
			Serial.println(F("ALL FIXED POINT FRACTION TESTS PASSED!"));
		else
			Serial.println(F("SOME FIXED POINT FRACTION TESTS FAILED!"));

		return pass;
	}
}

#endif