#ifndef _FRACTION_SCALE_TEST_h
#define _FRACTION_SCALE_TEST_h

#include <IntegerSignalTesting.h>

namespace IntegerSignal::Fraction::Test
{
	// Reference function for fraction8_t
	template<typename T>
	static T RefFraction8(const T value, const fraction8_t fraction) {
		return (static_cast<int64_t>(value) * fraction) / FRACTION8_1X;
	}

	template<typename T>
	static T RefFraction16(const T value, const fraction16_t fraction) {
		return (static_cast<int64_t>(value) * fraction) / FRACTION16_1X;
	}

	template<typename T>
	static T RefFraction32(const T value, const fraction32_t fraction) {
		return (static_cast<int64_t>(value) * fraction) / FRACTION32_1X;
	}

	template<typename T>
	static T RefFraction8(const T value, const ufraction8_t fraction) {
		return ((uint64_t)value * fraction) / UFRACTION8_1X;
	}

	template<typename T>
	static T RefFraction16(const T value, const ufraction16_t fraction) {
		return ((uint64_t)value * fraction) / UFRACTION16_1X;
	}

	template<typename T>
	static T RefFraction32(const T value, const ufraction32_t fraction) {
		return ((uint64_t)value * fraction) / UFRACTION32_1X;
	}

	// Reference function for GetFraction8
	static fraction8_t RefGetFraction8(uint8_t numerator, uint8_t denominator)
	{
		return (denominator == 0) ? 0 : min((int16_t)FRACTION8_1X, max((int16_t)FRACTION8_NEGATIVE_1X, (int16_t)(((int16_t)numerator * FRACTION8_1X) / denominator)));
	}

	// Reference function for GetFraction16
	static fraction16_t RefGetFraction16(uint8_t numerator, uint8_t denominator)
	{
		return (denominator == 0) ? 0 : min((int32_t)FRACTION16_1X, max((int32_t)FRACTION16_NEGATIVE_1X, (int32_t)(((int32_t)numerator * FRACTION16_1X) / denominator)));
	}

	// Reference function for GetFraction32
	static fraction32_t RefGetFraction32(uint8_t numerator, uint8_t denominator)
	{
		return (denominator == 0) ? 0 : min((int64_t)FRACTION32_1X, max((int64_t)FRACTION32_NEGATIVE_1X, (int64_t)(((int64_t)numerator * FRACTION32_1X) / denominator)));
	}

	// Exhaustive test for fraction8_t
	static bool TestFraction8Exhaustive()
	{
		Serial.println(F("Starting exhaustive fraction8_t tests..."));

		uint32_t errorCount = 0;
		for (uint16_t v = 0; v <= UINT8_MAX; v++)
		{
			for (uint16_t f = 1; f <= UINT8_MAX; f++)
			{
				const int8_t value = (uint8_t)v;
				const fraction8_t fraction8 = (fraction8_t)max((int8_t)FRACTION8_NEGATIVE_1X, (int8_t)min((int8_t)FRACTION8_1X, (int8_t)f));
				const int8_t result = Scale(fraction8, value);
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

	// Exhaustive test for fraction16_t
	static bool TestFraction16Exhaustive()
	{
		Serial.println(F("Starting exhaustive fraction16_t tests..."));

		uint32_t errorCount = 0;
		for (uint32_t v = 0; v <= UINT16_MAX; v++)
		{
			for (uint32_t f = 1; f <= UINT16_MAX; f++)
			{
				const int16_t value = (uint16_t)v;
				const fraction16_t fraction16 = (fraction16_t)max((int32_t)FRACTION16_NEGATIVE_1X, (int32_t)min((int32_t)FRACTION16_1X, (int32_t)f));
				const int16_t result = Scale(fraction16, value);
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

	// Sampled test for fraction16_t for uint8_t, uint16_t, and uint32_t.
	template<uint32_t MaxIterations = 50000>
	static bool TestFraction16Sample()
	{
		const uint32_t iterations = min(MaxIterations, (uint32_t)UINT16_MAX);

		Serial.println(F("Starting sampled fraction16_t tests..."));

		uint32_t errorCount = 0;

		for (uint32_t f = 0; f < UINT16_MAX; f++)
		{
			const uint32_t fractionIterate = (uint32_t)(((uint64_t)f * UINT16_MAX) / (iterations - 1));
			const fraction16_t fraction16 = (fraction16_t)max((int32_t)FRACTION16_NEGATIVE_1X, (int32_t)min((int32_t)FRACTION16_1X, (int32_t)fractionIterate));

			for (uint32_t i = 0; i <= iterations; i++)
			{
				const int32_t value32 = (uint32_t)(((uint64_t)i * UINT32_MAX) / (iterations - 1));
				const int16_t value16 = value32 / INT16_MAX;
				const int8_t value8 = value16 / INT8_MAX;

				const int8_t result8 = Scale(fraction16, value8);
				const int8_t refResult8 = RefFraction16(value8, fraction16);

				const int16_t result16 = Scale(fraction16, value16);
				const int16_t refResult16 = RefFraction16(value16, fraction16);

				const int32_t result32 = Scale(fraction16, value32);
				const int32_t refResult32 = RefFraction16(value32, fraction16);

				if (result8 != refResult8
					|| result16 != refResult16
					|| result32 != refResult32
					)
				{
					Serial.print(F("fraction16_t error: fraction="));
					Serial.print(fraction16);
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
					}
					errorCount++;
				}
			}

			if ((f % 4096) == 0)
			{
				Serial.print(F("fraction16_t progress: "));
				Serial.print(f);
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

	// Sampled test for fraction16_t for uint8_t, uint16_t, and uint32_t.
	template<uint32_t MaxIterations = 50000>
	static bool TestUFraction16Sample()
	{
		const uint32_t iterations = min(MaxIterations, (uint32_t)UINT16_MAX);

		Serial.println(F("Starting sampled ufraction16_t tests..."));

		uint32_t errorCount = 0;

		for (uint32_t f = 0; f < UINT16_MAX; f++)
		{
			const uint32_t fractionIterate = (uint32_t)(((uint64_t)f * UINT16_MAX) / (iterations - 1));
			const ufraction16_t fraction16 = (ufraction16_t)min((int32_t)FRACTION16_1X, (int32_t)fractionIterate);

			for (uint32_t i = 0; i <= iterations; i++)
			{
				const uint32_t value32 = (uint32_t)(((uint64_t)i * UINT32_MAX) / (iterations - 1));
				const uint16_t value16 = value32 / INT16_MAX;
				const uint8_t value8 = value16 / INT8_MAX;

				const uint8_t result8 = Scale(fraction16, value8);
				const uint8_t refResult8 = RefFraction16(value8, fraction16);

				const uint16_t result16 = Scale(fraction16, value16);
				const uint16_t refResult16 = RefFraction16(value16, fraction16);

				const uint32_t result32 = Scale(fraction16, value32);
				const uint32_t refResult32 = RefFraction16(value32, fraction16);

				if (result8 != refResult8
					|| result16 != refResult16
					|| result32 != refResult32
					)
				{
					Serial.print(F("ufraction16_t error: fraction="));
					Serial.print(fraction16);
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
					}
					errorCount++;
				}
			}

			if ((f % 4096) == 0)
			{
				Serial.print(F("ufraction16_t progress: "));
				Serial.print(f);
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

	// Sampled test for fraction32_t for uint8_t, uint16_t, and uint32_t.
	template<uint32_t MaxIterations = 50000>
	static bool TestFraction32Sample()
	{
		const uint32_t iterations = min(MaxIterations, UINT32_MAX);

		Serial.println(F("Starting sampled fraction32_t tests..."));

		uint32_t errorCount = 0;

		for (uint32_t f = 0; f < UINT16_MAX; f++)
		{
			const uint32_t fractionIterate = (uint32_t)(((uint64_t)f * UINT32_MAX) / (iterations - 1));
			const fraction32_t fraction32 = (fraction32_t)max((int64_t)FRACTION32_NEGATIVE_1X, (int64_t)min((int64_t)FRACTION32_1X, (int64_t)fractionIterate));

			for (uint64_t i = 0; i <= iterations; i++)
			{
				const int32_t value32 = (uint32_t)(((uint64_t)i * UINT32_MAX) / (iterations - 1));
				const int16_t value16 = value32 / INT16_MAX;
				const int8_t value8 = value16 / INT8_MAX;

				const int8_t result8 = Scale(fraction32, value8);
				const int8_t refResult8 = RefFraction32(value8, fraction32);

				const int16_t result16 = Scale(fraction32, value16);
				const int16_t refResult16 = RefFraction32(value16, fraction32);

				const int32_t result32 = Scale(fraction32, value32);
				const int32_t refResult32 = RefFraction32(value32, fraction32);

				if (result8 != refResult8
					|| result16 != refResult16
					|| result32 != refResult32)
				{
					Serial.print(F("fraction32_t error: fraction="));
					Serial.print(fraction32);
					Serial.print(F(" value="));
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
						Serial.print(F(" error32="));
						Serial.print((result32 - refResult32));
					}
					errorCount++;
				}
			}

			if ((f % (UINT16_MAX / 128)) == 0)
			{
				Serial.print(F("fraction32_t progress: "));
				Serial.print(f * iterations);
				Serial.println(F(" values tested..."));
			}
		}

		if (errorCount == 0)
		{
			Serial.println(F("All fraction32_t tests PASSED."));
		}
		else
		{
			Serial.print(F("fraction32_t tests FAILED. Errors: "));
			Serial.println(errorCount);
		}

		return errorCount == 0;
	}

	// Sampled test for fraction8_t for uint8_t, uint16_t, and uint32_t.
	template<uint32_t MaxIterations = 50000>
	static bool TestFraction8Sample()
	{
		const uint32_t iterations = min(MaxIterations, (uint32_t)UINT16_MAX);

		Serial.println(F("Starting sampled fraction8_t tests..."));

		uint32_t errorCount = 0;

		for (uint16_t f = 1; f <= UINT8_MAX; f++)
		{
			const fraction8_t fraction8 = (fraction8_t)max((int8_t)FRACTION8_NEGATIVE_1X, (int8_t)min((int8_t)FRACTION8_1X, (int8_t)f));

			for (uint32_t i = 0; i <= iterations; i++)
			{
				const int32_t value32 = (uint32_t)(((uint64_t)i * UINT32_MAX) / (iterations - 1));
				const int16_t value16 = value32 / INT16_MAX;
				const int8_t value8 = value16 / INT8_MAX;

				const int8_t result8 = Scale(fraction8, value8);
				const int8_t refResult8 = RefFraction8(value8, fraction8);

				const int16_t result16 = Scale(fraction8, value16);
				const int16_t refResult16 = RefFraction8(value16, fraction8);

				const int32_t result32 = Scale(fraction8, value32);
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

	// Sampled test for ufraction8_t for uint8_t, uint16_t, and uint32_t.
	template<uint32_t MaxIterations = 50000>
	static bool TestUFraction8Sample()
	{
		const uint32_t iterations = min(MaxIterations, (uint32_t)UINT16_MAX);

		Serial.println(F("Starting sampled ufraction8_t tests..."));

		uint32_t errorCount = 0;

		for (uint16_t f = 1; f <= UINT8_MAX; f++)
		{
			const ufraction8_t fraction8 = (ufraction8_t)min((uint8_t)UFRACTION8_1X, (uint8_t)f);

			for (uint32_t i = 0; i <= iterations; i++)
			{
				const uint32_t value32 = (uint32_t)(((uint64_t)i * UINT32_MAX) / (iterations - 1));
				const uint16_t value16 = value32 / UINT16_MAX;
				const uint8_t value8 = value16 / UINT8_MAX;

				const uint8_t result8 = Scale(fraction8, value8);
				const uint8_t refResult8 = RefFraction8(value8, fraction8);

				const uint16_t result16 = Scale(fraction8, value16);
				const uint16_t refResult16 = RefFraction8(value16, fraction8);

				const uint32_t result32 = Scale(fraction8, value32);
				const uint32_t refResult32 = RefFraction8(value32, fraction8);

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

	// Exhaustive test for ufraction8_t
	static bool TestUFraction8Exhaustive()
	{
		Serial.println(F("Starting exhaustive ufraction8_t tests..."));

		uint32_t errorCount = 0;
		for (uint16_t v = 1; v <= UINT8_MAX; v++)
		{
			for (uint16_t f = v; f <= UINT8_MAX; f++)
			{
				const uint8_t value = (uint8_t)v;
				const ufraction8_t fraction8 = (ufraction8_t)min((uint8_t)UFRACTION8_1X, (uint8_t)f);
				const uint8_t result = Scale(fraction8, value);
				const uint8_t refResult = RefFraction8(value, fraction8);
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

	// Exhaustive test for ufraction16_t
	static bool TestUFraction16Exhaustive()
	{
		Serial.println(F("Starting exhaustive ufraction16_t tests..."));

		uint32_t errorCount = 0;
		for (uint32_t v = 0; v <= UINT16_MAX; v++)
		{
			for (uint32_t f = 1; f <= UINT16_MAX; f++)
			{
				const uint16_t value = (uint16_t)v;
				const ufraction16_t fraction16 = (ufraction16_t)min((uint16_t)FRACTION16_1X, (uint16_t)f);
				const uint16_t result = Scale(fraction16, value);
				const uint16_t refResult = RefFraction16(value, fraction16);
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

	// Exhaustive test for GetFraction8 for all 255*255 combinations of numerator/denominator.
	static bool TestGetFraction8Exhaustive()
	{
		Serial.println(F("Starting exhaustive GetFraction8 tests..."));

		uint32_t errorCount = 0;
		for (uint16_t n = 1; n < UINT8_MAX; n++)
		{
			for (uint16_t d = n; d <= UINT8_MAX; d++)
			{
				const uint8_t numerator = (uint8_t)n;
				const uint8_t denominator = (uint8_t)d;
				const fraction8_t fraction8 = GetFraction8(numerator, denominator);
				const fraction8_t reffraction8 = RefGetFraction8(numerator, denominator);

				const fraction16_t fraction16 = GetFraction16(numerator, denominator);
				const fraction16_t reffraction16 = RefGetFraction16(numerator, denominator);

				const fraction32_t fraction32 = GetFraction32(numerator, denominator);
				const fraction32_t reffraction32 = RefGetFraction32(numerator, denominator);

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


	template<uint32_t MaxIterations = 50000, uint8_t maxError32 = 0>
	static bool RunTests()
	{
		bool pass = true;

		pass &= TestGetFraction8Exhaustive();

		pass &= TestFraction8Exhaustive();
		pass &= TestUFraction8Exhaustive();
		pass &= TestFraction8Sample<MaxIterations>();
		pass &= TestUFraction8Sample<MaxIterations>();

		pass &= TestFraction16Sample<MaxIterations>();
		pass &= TestUFraction16Sample<MaxIterations>();
		pass &= TestFraction32Sample<MaxIterations>();

		if (!pass)
		{
			Serial.println(F("FractionScale tests FAILED."));
		}

		return pass;
	}

	static bool RunExhaustive()
	{
		bool pass = true;
		pass &= TestFraction16Exhaustive();
		pass &= TestUFraction16Exhaustive();

		return pass;
	}
}

#endif

