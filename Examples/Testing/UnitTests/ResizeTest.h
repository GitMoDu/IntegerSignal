#ifndef _RESIZE_SCALE_TEST_h
#define _RESIZE_SCALE_TEST_h

#include <IntegerSignalTesting.h>

namespace IntegerSignal::Resize::Test
{
	// Reference function for resize8_t
	static uint8_t RefResize8(const uint8_t value, const resize8_t resize)
	{
		return min((uint16_t)UINT8_MAX, (uint16_t)(((uint16_t)value * resize) / RESIZE8_1X));
	}

	static uint16_t RefResize8(const uint16_t value, const resize8_t resize)
	{
		return min((uint32_t)UINT16_MAX, (uint32_t)(((uint32_t)value * resize) / RESIZE8_1X));
	}

	static uint32_t RefResize8(const uint32_t value, const resize8_t resize)
	{
		return min((uint64_t)UINT32_MAX, (uint64_t)(((uint64_t)value * resize) / RESIZE8_1X));
	}


	// Reference function for resize6_t
	static uint8_t RefResize16(const uint8_t value, const resize16_t resize)
	{
		return min((uint32_t)UINT8_MAX, (uint32_t)(((uint32_t)value * resize) / RESIZE16_1X));
	}

	static uint16_t RefResize16(const uint16_t value, const resize16_t resize)
	{
		return min((uint32_t)UINT16_MAX, (uint32_t)(((uint32_t)value * resize) / RESIZE16_1X));
	}

	static uint32_t RefResize16(const uint32_t value, const resize16_t resize)
	{
		return min((uint64_t)UINT32_MAX, (uint64_t)(((uint64_t)value * resize) / RESIZE16_1X));
	}


	// Reference function for GetResize8
	static resize8_t RefGetResize8(uint8_t numerator, uint8_t denominator)
	{
		return (denominator == 0) ? 0 : ((uint16_t)numerator * RESIZE8_1X) / denominator;
	}

	static resize16_t RefGetResize16(uint8_t numerator, uint8_t denominator)
	{
		return (denominator == 0) ? 0 : ((uint32_t)numerator * RESIZE16_1X) / denominator;
	}

	static resize32_t RefGetResize32(uint8_t numerator, uint8_t denominator)
	{
		return (denominator == 0) ? 0 : ((uint64_t)numerator * RESIZE32_1X) / denominator;
	}

	// Exhaustive test for resize8_t
	static bool TestResize8Exhaustive8()
	{
		Serial.println(F("Starting exhaustive Resize (8 bit) tests..."));

		uint32_t errorCount = 0;
		for (uint16_t v = 0; v <= UINT8_MAX; v++) 
		{
			for (uint16_t r = 1; r <= UINT8_MAX; r++)
			{
				const uint8_t value = (uint8_t)v;
				const resize8_t resize = (resize8_t)r;
				const uint8_t result = Scale(resize, value);
				const uint8_t refResult = RefResize8(value, resize);
				if (result != refResult)
				{
					Serial.print(F("Resize (8 bit) error: value="));
					Serial.print(value);
					Serial.print(F(" resize="));
					Serial.print(resize);
					Serial.print(F(" result="));
					Serial.print(result);
					Serial.print(F(" ref="));
					Serial.println(refResult);
					errorCount++;
				}
			}
			if ((v % 16) == 0)
			{
				Serial.print(F("Resize (8 bit) progress: "));
				Serial.print(v);
				Serial.println(F(" values tested..."));
			}
		}
		if (errorCount == 0)
		{
			Serial.println(F("All Resize (8 bit) tests PASSED."));
		}
		else
		{
			Serial.print(F("Resize (8 bit) tests FAILED. Errors: "));
			Serial.println(errorCount);
		}

		return errorCount == 0;
	}

	static bool TestResize16Exhaustive8()
	{
		Serial.println(F("Starting exhaustive Resize (16 bit) tests..."));

		uint32_t errorCount = 0;
		for (uint16_t v = 0; v <= UINT8_MAX; v++)
		{
			for (uint32_t r = 1; r <= UINT16_MAX; r++)
			{
				const uint8_t value = (uint8_t)v;
				const resize16_t resize = (resize16_t)r;
				const uint8_t result = Scale(resize, value);
				const uint8_t refResult = RefResize16(value, resize);
				if (result != refResult)
				{
					Serial.print(F("Resize (16 bit) error: value="));
					Serial.print(value);
					Serial.print(F(" resize="));
					Serial.print(resize);
					Serial.print(F(" result="));
					Serial.print(result);
					Serial.print(F(" ref="));
					Serial.println(refResult);
					errorCount++;
				}
			}
			if ((v % 16) == 0)
			{
				Serial.print(F("Resize (16 bit) progress: "));
				Serial.print(v);
				Serial.println(F(" values tested..."));
			}
		}
		if (errorCount == 0)
		{
			Serial.println(F("All Resize (16 bit) tests PASSED."));
		}
		else
		{
			Serial.print(F("Resize (16 bit) tests FAILED. Errors: "));
			Serial.println(errorCount);
		}

		return errorCount == 0;
	}

	// Sampled test for resize8_t for uint8_t, uint16_t and uint32_t.
	template<uint32_t MaxIterations = 50000>
	static bool TestResize8Sample()
	{
		const uint32_t iterations = min(MaxIterations, (uint32_t)UINT16_MAX);

		Serial.println(F("Starting sampled Resize (8 bit) tests..."));

		uint32_t errorCount = 0;

		for (uint16_t r = 1; r <= UINT8_MAX; r++)
		{
			const resize8_t resize8 = (resize8_t)r;

			for (uint32_t i = 0; i <= iterations; i++)
			{
				const uint32_t value32 = (uint32_t)(((uint64_t)i * UINT32_MAX) / (iterations - 1));
				const uint16_t value16 = value32 >> 16;
				const uint8_t value8 = value16 >> 8;

				const uint8_t result8 = Scale(resize8, value8);
				const uint8_t refResult8 = RefResize8(value8, resize8);

				const uint16_t result16 = Scale(resize8, value16);
				const uint16_t refResult16 = RefResize8(value16, resize8);

				const uint32_t result32 = Scale(resize8, value32);
				const uint32_t refResult32 = RefResize8(value32, resize8);

				if (result8 != refResult8
					|| result16 != refResult16
					|| result32 != refResult32
					)
				{
					Serial.print(F("Resize (8 bit) error: resize="));
					Serial.print(resize8);
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

			if ((r % 16) == 0)
			{
				Serial.print(F("Resize (8 bit) progress: "));
				Serial.print(r);
				Serial.println(F(" values tested..."));
			}
		}

		if (errorCount == 0)
		{
			Serial.println(F("All Resize (8 bit) tests PASSED."));
		}
		else
		{
			Serial.print(F("Resize (8 bit) tests FAILED. Errors: "));
			Serial.println(errorCount);
		}

		return errorCount == 0;
	}


	// Sampled test for resize16_t for uint8_t, uint16_t, and uint32_t
	template<uint32_t MaxIterations = 50000>
	static bool TestResize16Sample()
	{
		const uint32_t iterations = min(MaxIterations, (uint32_t)UINT16_MAX);

		Serial.println(F("Starting sampled (16 bit) tests..."));

		uint32_t errorCount = 0;

		for (uint32_t r = 1; r <= iterations; r++)
		{
			const resize16_t resize16 = (uint32_t)(((uint64_t)r * UINT16_MAX) / (iterations - 1));
			
			for (uint32_t i = 0; i <= iterations; i++)
			{
				const uint32_t value32 = (uint32_t)(((uint64_t)i * UINT32_MAX) / (iterations - 1));
				const uint16_t value16 = value32 >> 16;
				const uint8_t value8 = value16 >> 8;

				const uint8_t result8 = Scale(resize16, value8);
				const uint8_t refResult8 = RefResize16(value8, resize16);

				const uint16_t result16 = Scale(resize16, value16);
				const uint16_t refResult16 = RefResize16(value16, resize16);

				const uint32_t result32 = Scale(resize16, value32);
				const uint32_t refResult32 = RefResize16(value32, resize16);

				if (result8 != refResult8 || result16 != refResult16 || result32 != refResult32)
				{
					Serial.print(F("Resize (16 bit) error: resize="));
					Serial.print(resize16);
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
						Serial.print(F(" result32="));
						Serial.print(result32);
						Serial.print(F(" ref="));
						Serial.println(refResult32);
					}
					errorCount++;
				}
			}

			if ((r % 256) == 0)
			{
				Serial.print(F("Resize (16 bit) progress: "));
				Serial.print(r);
				Serial.println(F(" resizes tested..."));
			}
		}

		if (errorCount == 0)
		{
			Serial.println(F("All Resize (16 bit) tests PASSED."));
		}
		else
		{
			Serial.print(F("Resize (16 bit) tests FAILED. Errors: "));
			Serial.println(errorCount);
		}

		return errorCount == 0;
	}


	// Exhaustive test for GetResizeN for all 255*255 combinations of numerator/denominator.
	static bool TestGetResizeExhaustive8()
	{
		Serial.println(F("Starting exhaustive GetResize (8 bit) tests..."));

		uint32_t errorCount = 0;
		for (uint16_t n = 1; n <= UINT8_MAX; n++)
		{
			for (uint16_t d = 1; d <= UINT8_MAX; d++) 
{
				const uint8_t numerator = (uint8_t)n;
				const uint8_t denominator = (uint8_t)d;
				const resize8_t result8 = GetResize8(numerator, denominator);
				const resize8_t refResult8 = RefGetResize8(numerator, denominator);

				const resize16_t result16 = GetResize16(numerator, denominator);
				const resize16_t refResult16 = RefGetResize16(numerator, denominator);

				const resize32_t result32 = GetResize32(numerator, denominator);
				const resize32_t refResult32 = RefGetResize32(numerator, denominator);

				if (result8 != refResult8
					|| result16 != refResult16
					|| result32 != refResult32
					)
				{
					Serial.print(F("GetResize (8 bit) error: numerator="));
					Serial.print(numerator);
					Serial.print(F(" denominator="));
					Serial.print(denominator);
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
			if ((n % 16) == 0)
			{
				Serial.print(F("GetResize (8 bit) progress: "));
				Serial.print(n);
				Serial.println(F(" numerators tested..."));
			}
		}
		if (errorCount == 0)
		{
			Serial.println(F("All GetResize (8 bit) tests PASSED."));
		}
		else
		{
			Serial.print(F("GetResize (8 bit) tests FAILED. Errors: "));
			Serial.println(errorCount);
		}

		return errorCount == 0;
	}

	template<uint32_t MaxIterations = 50000>
	static bool RunTests()
	{
		bool pass = true;

		pass &= TestGetResizeExhaustive8();

		pass &= TestResize8Exhaustive8();
		pass &= TestResize16Exhaustive8();

		pass &= TestResize8Sample<MaxIterations>();
		pass &= TestResize16Sample<MaxIterations/10>();


		if (!pass)
		{
			Serial.println(F("ResizeScale tests FAILED."));
		}

		return pass;
	}
}

#endif