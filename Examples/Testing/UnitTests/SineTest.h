// SineLutTest.h

#ifndef _SINELUTTEST_h
#define _SINELUTTEST_h

#include <IntegerSignalTesting.h>

namespace IntegerSignal::Trigonometry::Sine::Test
{
	// Reference function for sine calculation using floating-point math for Sine32
	static uint32_t RefSine32(const angle_t angle)
	{
		double radians = (angle * M_PI) / (2.0 * (double)ANGLE_90);
		return round(sin(radians) * (double)FRACTION32_1X);
	}

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

	// Exhaustive test for GetInterpolated function for Sine32
	template<uint32_t ErrorTolerance>
	static bool TestSine32Exhaustive()
	{
		Serial.println(F("Starting exhaustive GetInterpolated tests for Sine32..."));

		uint32_t errorCount = 0;
		uint32_t maxError = 0;
		for (uint32_t angle = 0; angle <= ANGLE_90; angle++)
		{
			const uint32_t result = IntegerSignal::Trigonometry::Sine32(angle);
			const uint32_t refResult = RefSine32(angle);

			maxError = max((int32_t)maxError, (int32_t)abs((int64_t)result - (int64_t)refResult));

			if (abs((int64_t)result - (int64_t)refResult) > ErrorTolerance)
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
			Serial.println(F("All GetInterpolated tests for Sine32 PASSED."));
		}
		else
		{
			Serial.print(F("GetInterpolated tests for Sine32 FAILED. Errors: "));
			Serial.println(errorCount);
		}

		Serial.print(F(" Max unit error:"));
		Serial.println(maxError);

		return errorCount == 0;
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

	static void PrintErrorResults8()
	{
		Serial.println(F("Integer Trigonometry Sine8"));


		const angle_t angleRange = GetAngle(90);

		fraction8_t errorLargestI = 0;
		double errorBottomF = 0;
		double errorTopF = 0;

		double errorSum = 0;

		for (uint32_t i = 0; i <= ANGLE_RANGE; i++)
		{
			const double degrees = ((double)i * 90.0) / (double)ANGLE_RANGE;
			const angle_t angle = ((uint32_t)i * ANGLE_90) / ANGLE_RANGE;

			const double sineUnit = sin(DegreesToRadians(degrees));
			const fraction8_t unitFraction = roundf(sineUnit * double(FRACTION8_1X));
			const fraction8_t sineFraction = IntegerSignal::Trigonometry::Sine8(angle);

			const int16_t errorI = (int16_t)sineFraction - unitFraction;
			const double errorF = errorI / double(FRACTION8_1X);

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

			errorSum += abs(errorF);
		}

		const double errorAverageF = (double)errorSum / (double)ANGLE_RANGE;
		const double errorAverageI = errorAverageF * (double)FRACTION8_1X;

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

	static void PrintErrorResults16()
	{
		Serial.println(F("Integer Trigonometry Sine16"));

		const angle_t angleRange = GetAngle(90);

		fraction16_t errorLargestI = 0;
		double errorBottomF = 0;
		double errorTopF = 0;

		double errorSum = 0;

		for (uint32_t i = 0; i <= ANGLE_RANGE; i++)
		{
			const double degrees = ((double)i * 90.0) / (double)ANGLE_RANGE;
			const angle_t angle = ((uint32_t)i * ANGLE_90) / ANGLE_RANGE;

			const double sineUnit = sin(DegreesToRadians(degrees));
			const fraction16_t unitFraction = round(sineUnit * double(FRACTION16_1X));
			const fraction16_t sineFraction = IntegerSignal::Trigonometry::Sine16(angle);

			const int32_t errorI = (int32_t)sineFraction - unitFraction;
			const double errorF = errorI / double(FRACTION16_1X);

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

			errorSum += abs(errorF);
		}

		const double errorAverageF = (double)errorSum / (double)ANGLE_RANGE;
		const double errorAverageI = errorAverageF * (double)FRACTION16_1X;

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

	static void PrintErrorResults32()
	{
		Serial.println(F("Integer Trigonometry Sine32"));

		const angle_t angleRange = GetAngle(90);

		fraction32_t errorLargestI = 0;
		double errorBottomF = 0;
		double errorTopF = 0;

		double errorSum = 0;

		for (uint32_t i = 0; i <= ANGLE_RANGE; i++)
		{
			const double degrees = ((double)i * 90.0) / (double)ANGLE_RANGE;
			const angle_t angle = ((uint32_t)i * ANGLE_90) / ANGLE_RANGE;

			const double sineUnit = sin(DegreesToRadians(degrees));
			const fraction32_t unitFraction = round(sineUnit * double(FRACTION32_1X));
			const fraction32_t sineFraction = IntegerSignal::Trigonometry::Sine32(angle);

			const int64_t errorI = (int64_t)sineFraction - unitFraction;
			const double errorF = errorI / double(FRACTION32_1X);

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

			errorSum += abs(errorF);
		}

		const double errorAverageF = (double)errorSum / (double)ANGLE_RANGE;
		const double errorAverageI = errorAverageF * (double)FRACTION32_1X;

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
		static constexpr uint8_t Error16Max = 6;
		static constexpr uint8_t Error32Max = 222202;
#else
		static constexpr uint8_t Error8Max = 1;
		static constexpr uint8_t Error16Max = 3;
		static constexpr uint32_t Error32Max = 111101;
#endif
		pass &= TestSine8Exhaustive<Error8Max>();
		pass &= TestSine16Exhaustive<Error16Max>();
		pass &= TestSine32Exhaustive<Error32Max>();

		PrintErrorResults8();
		PrintErrorResults16();
		PrintErrorResults32();

		if (!pass)
		{
			Serial.println(F("Sine16, Sine8, and Sine32 tests FAILED."));
		}

		return pass;
	}
}

#endif

