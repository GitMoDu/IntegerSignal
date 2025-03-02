// SineLutTest.h

#ifndef _SINELUTTEST_h
#define _SINELUTTEST_h

#include <IntegerSignalTesting.h>

namespace IntegerSignal::Trigonometry::Sine::Test
{
	// Reference function for sine calculation using floating-point math
	static uint16_t RefSine16(const angle_t angle)
	{
		double radians = (angle * M_PI) / (2.0 * (double)ANGLE_90);
		return round(sin(radians) * (double)FRACTION16_1X);
	}

	// Reference function for sine calculation using floating-point math for Sine8
	static uint8_t RefSine8(const angle_t angle)
	{
		double radians = (angle * M_PI) / (2.0 * (double)ANGLE_90);
		return round(sin(radians) * (double)FRACTION8_1X);
	}

	static constexpr double DegreesToRadians(const double degrees) {
		return degrees * (double)(M_PI) / 180.0;
	}

	// Exhaustive test for GetInterpolated function for Sine16
	template<uint8_t ErrorTolerance>
	static bool TestSine16Exhaustive()
	{
		Serial.println(F("Starting exhaustive GetInterpolated tests for Sine16..."));

		uint32_t errorCount = 0;
		uint32_t maxError = 0;
		for (uint16_t angle = 0; angle <= ANGLE_90; angle++)
		{
			const fraction16_t result = IntegerSignal::Trigonometry::Sine16(angle);
			const fraction16_t refResult = RefSine16(angle);

			maxError = max((int32_t)maxError, abs((int32_t)result - (int32_t)refResult));

			if (abs((int32_t)result - (int32_t)refResult) > maxError)
			{
				Serial.print(F("GetInterpolated error: angle="));
				Serial.print(angle);
				Serial.print(F(" result="));
				Serial.print(result);
				Serial.print(F(" ref="));
				Serial.println(refResult);
				errorCount++;
			}
			if ((angle % (ANGLE_90 / 10)) == 0)
			{
				Serial.print(F("GetInterpolated progress: "));
				Serial.print(angle);
				Serial.println(F(" angles tested..."));
			}
		}
		if (errorCount == 0)
		{
			Serial.println(F("All GetInterpolated tests for Sine16 PASSED."));
		}
		else
		{
			Serial.print(F("GetInterpolated tests for Sine16 FAILED. Errors: "));
			Serial.println(errorCount);
		}

		Serial.print(F(" Max unit error:"));
		Serial.println(maxError);

		return errorCount == 0;
	}

	// Exhaustive test for GetInterpolated function for Sine8
	template<uint8_t ErrorTolerance>
	static bool TestSine8Exhaustive()
	{
		Serial.println(F("Starting exhaustive GetInterpolated tests for Sine8..."));

		uint32_t errorCount = 0;
		uint32_t maxError = 0;
		for (uint32_t angle = 0; angle <= ANGLE_90; angle++)
		{
			const uint8_t result = IntegerSignal::Trigonometry::Sine8(angle);
			const uint8_t refResult = RefSine8(angle);

			maxError = max((int32_t)maxError, abs((int32_t)result - (int32_t)refResult));

			if (abs((int32_t)result - (int32_t)refResult) > ErrorTolerance)
			{
				Serial.print(F("GetInterpolated error: angle="));
				Serial.print(angle);
				Serial.print(F(" result="));
				Serial.print(result);
				Serial.print(F(" ref="));
				Serial.println(refResult);
				errorCount++;
			}
			if ((angle % (ANGLE_90 / 10)) == 0)
			{
				Serial.print(F("GetInterpolated progress: "));
				Serial.print(angle);
				Serial.println(F(" angles tested..."));
			}
		}
		if (errorCount == 0)
		{
			Serial.println(F("All GetInterpolated tests for Sine8 PASSED."));
		}
		else
		{
			Serial.print(F("GetInterpolated tests for Sine8 FAILED. Errors: "));
			Serial.println(errorCount);
		}

		Serial.print(F(" Max unit error:"));
		Serial.println(maxError);

		return errorCount == 0;
	}

	// Sampled test for GetInterpolated function for Sine8
	template<uint32_t MaxIterations = 50000, uint8_t maxError>
	static bool TestGetInterpolatedSampleSine8()
	{
		Serial.println(F("Starting sampled GetInterpolated tests for Sine8..."));

		uint32_t errorCount = 0;
		for (uint32_t i = 0; i < MaxIterations; i++) {
			// Map i from [0, MaxIterations-1] to the full angle range [0, ANGLE_90]
			const angle_t angle = (angle_t)(((uint64_t)i * ANGLE_90) / (MaxIterations - 1));
			const int8_t result = IntegerSignal::Trigonometry::Sine8(angle);
			const int8_t refResult = RefSine8(angle);
			if (abs((int32_t)result - (int32_t)refResult) > maxError)
			{
				Serial.print(F("GetInterpolated error: angle="));
				Serial.print(angle);
				Serial.print(F(" result8="));
				Serial.print(result);
				Serial.print(F(" ref="));
				Serial.println(refResult);
				errorCount++;
			}
			if (i % (MaxIterations / 10) == 0)
			{
				Serial.print(F("GetInterpolated progress: "));
				Serial.print(i);
				Serial.println(F(" samples tested..."));
			}
		}
		if (errorCount == 0)
		{
			Serial.println(F("All GetInterpolated tests for Sine8 PASSED."));
		}
		else
		{
			Serial.print(F("GetInterpolated tests for Sine8 FAILED. Errors: "));
			Serial.println(errorCount);
		}

		return errorCount == 0;
	}

	static void PrintErrorResults16()
	{
		Serial.print(F("Integer Trigonometry "));
		Serial.print(Lut::Sine16::LutSize);
		Serial.println(F(" LUT Sin"));

		const angle_t angleRange = GetAngle(90);

		fraction16_t errorLargestI = 0;
		double errorBottomF = 0;
		double errorTopF = 0;

		int32_t errorSum = 0;

		for (uint32_t i = 0; i <= UINT16_MAX; i++)
		{
			const angle_t angle = ((uint32_t)i * ANGLE_90) / UINT16_MAX;
			const double degrees = ((double)i * 90.0) / (double)UINT16_MAX;

			const fraction16_t sinScale = IntegerSignal::Trigonometry::Sine16(angle);
			const double sinF = sin(DegreesToRadians(degrees)) * (double)FRACTION16_1X;

			const double sinI = round(sinScale);

			const fraction16_t errorI = sinScale - (fraction16_t)sinF;
			const double errorF = (double)errorI / (double)FRACTION16_1X;

			if (errorF > errorTopF)
			{
				errorTopF = errorF;
			}
			if (errorF < errorBottomF)
			{
				errorBottomF = errorF;
			}
			if (abs(errorI) > errorLargestI)
			{
				errorLargestI = abs(errorI);
			}

			errorSum += abs(errorI);
		}

		const double errorAverageI = (double)errorSum / (double)FRACTION16_1X;
		const double errorAverageF = (double)errorAverageI / (double)FRACTION16_1X;


		Serial.print(F("\tMax Error\t"));
		Serial.println(errorLargestI);
		Serial.print(F("\tAverage Error\t"));
		Serial.print((int)(errorAverageF * 1000000.0));
		Serial.println("ppm");
		Serial.print(F("\tError Range\t["));
		Serial.print((int)(errorBottomF * 1000000.0));
		Serial.print(" ; ");
		Serial.print((int)(errorTopF * 1000000.0));
		Serial.print("] ppm");

		Serial.println();
	}

	static void PrintErrorResults8()
	{
		Serial.print(F("Integer Trigonometry "));
		Serial.print(Lut::Sine8::LutSize);
		Serial.println(F(" LUT Sin"));

		const angle_t angleRange = GetAngle(90);

		fraction8_t errorLargestI = 0;
		double errorBottomF = 0;
		double errorTopF = 0;

		int32_t errorSum = 0;

		for (uint32_t i = 0; i <= UINT8_MAX; i++)
		{
			const angle_t angle = ((uint32_t)i * ANGLE_90) / UINT8_MAX;
			const double degrees = ((double)i * 90.0) / (double)UINT8_MAX;

			const fraction8_t sinScale = IntegerSignal::Trigonometry::Sine8(angle);
			const double sinF = sin(DegreesToRadians(degrees)) * (double)FRACTION8_1X;

			const double sinI = round(sinScale);

			const fraction8_t errorI = sinScale - (fraction8_t)sinF;
			const double errorF = (double)errorI / (double)FRACTION8_1X;

			if (errorF > errorTopF)
			{
				errorTopF = errorF;
			}
			if (errorF < errorBottomF)
			{
				errorBottomF = errorF;
			}
			if (abs(errorI) > errorLargestI)
			{
				errorLargestI = abs(errorI);
			}

			errorSum += abs(errorI);
		}

		const double errorAverageI = (double)errorSum / (double)FRACTION8_1X;
		const double errorAverageF = (double)errorAverageI / (double)FRACTION8_1X;

		Serial.print(F("\tMax Error\t"));
		Serial.println(errorLargestI);
		Serial.print(F("\tAverage Error\t"));
		Serial.print((int)(errorAverageF * 1000000.0));
		Serial.println("ppm");
		Serial.print(F("\tError Range\t["));
		Serial.print((int)(errorBottomF * 1000000.0));
		Serial.print(" ; ");
		Serial.print((int)(errorTopF * 1000000.0));
		Serial.print("] ppm");

		Serial.println();
	}


	template<uint32_t MaxIterations = 50000>
	static bool RunTests()
	{
		bool pass = true;

#if (INTEGER_TRIGONOMETRY_LUT == INTEGER_TRIGONOMETRY_LUT_TINY)
		static constexpr uint8_t Error8Max = 1;
		static constexpr uint8_t Error16Max = 4;
#else
		static constexpr uint8_t Error8Max = 1;
		static constexpr uint8_t Error16Max = 2;
#endif
		pass &= TestSine8Exhaustive<Error8Max>();
		pass &= TestSine16Exhaustive<Error16Max>();

		PrintErrorResults8();
		PrintErrorResults16();

		if (!pass)
		{
			Serial.println(F("Sine16 and Sine8 tests FAILED."));

		}

		return pass;
	}
}

#endif

