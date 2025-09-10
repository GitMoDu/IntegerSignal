#ifndef _TANGENT_TEST_h
#define _TANGENT_TEST_h

#include <IntegerSignalTesting.h>

namespace IntegerSignal
{
	namespace Trigonometry
	{
		namespace Tangent
		{
			namespace Test
			{
				// Reference function for tangent calculation using floating-point math for Tangent32
				static uint32_t RefTangent32(const angle_t angle)
				{
					double radians = (angle * M_PI) / (2.0 * (double)ANGLE_90);
					return round(tan(radians) * (double)Fraction32::FRACTION_1X);
				}

				// Reference function for tangent calculation using floating-point math for Tangent16
				static uint16_t RefTangent16(const angle_t angle)
				{
					double radians = (angle * M_PI) / (2.0 * (double)ANGLE_90);
					return round(tan(radians) * (double)Fraction16::FRACTION_1X);
				}

				// Reference function for tangent calculation using floating-point math for Tangent8
				static uint8_t RefTangent8(const angle_t angle)
				{
					double radians = (angle * M_PI) / (2.0 * (double)ANGLE_90);
					return round(tan(radians) * (double)Fraction8::FRACTION_1X);
				}

				static constexpr double DegreesToRadians(const double degrees) {
					return degrees * (double)(M_PI) / 180.0;
				}

				// Exhaustive test for GetInterpolated function for Tangent8
				template<uint8_t ErrorTolerance>
				static bool TestTangent8Exhaustive(const double degreesRange)
				{
					Serial.println(F("Starting exhaustive GetInterpolated tests for Tangent8..."));

					const angle_t angleRange = (degreesRange * double(ANGLE_RANGE)) / 360.0;

					uint32_t errorCount = 0;
					int32_t maxError = 0;
					for (uint32_t angle = 0; angle <= angleRange; angle++)
					{
						const uint8_t result = IntegerSignal::Trigonometry::Tangent8(angle);
						const uint8_t refResult = RefTangent8(angle);

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
						if ((angle % (angleRange / 10)) == 0)
						{
							Serial.print(F("GetInterpolated progress: "));
							Serial.print(angle);
							Serial.println(F(" angles tested..."));
						}
					}
					if (errorCount == 0)
					{
						Serial.println(F("All GetInterpolated tests for Tangent8 PASSED."));
					}
					else
					{
						Serial.print(F("GetInterpolated tests for Tangent8 FAILED. Errors: "));
						Serial.println(errorCount);
					}

					Serial.print(F(" Max unit error:"));
					Serial.println(maxError);

					return errorCount == 0;
				}

				// Exhaustive test for GetInterpolated function for Tangent16
				template<uint8_t ErrorTolerance>
				static bool TestTangent16Exhaustive(const double degreesRange)
				{
					Serial.println(F("Starting exhaustive GetInterpolated tests for Tangent16..."));

					const angle_t angleRange = (degreesRange * double(ANGLE_RANGE)) / 360.0;

					uint32_t errorCount = 0;
					int32_t maxError = 0;
					for (uint16_t angle = 0; angle <= angleRange; angle++)
					{
						const Fraction16::scalar_t result = IntegerSignal::Trigonometry::Tangent16(angle);
						const Fraction16::scalar_t refResult = RefTangent16(angle);

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
						if ((angle % (angleRange / 10)) == 0)
						{
							Serial.print(F("GetInterpolated progress: "));
							Serial.print(angle);
							Serial.println(F(" angles tested..."));
						}
					}
					if (errorCount == 0)
					{
						Serial.println(F("All GetInterpolated tests for Tangent16 PASSED."));
					}
					else
					{
						Serial.print(F("GetInterpolated tests for Tangent16 FAILED. Errors: "));
						Serial.println(errorCount);
					}

					Serial.print(F(" Max unit error:"));
					Serial.println(maxError);

					return errorCount == 0;
				}

				// Exhaustive test for GetInterpolated function for Tangent32
				template<uint32_t ErrorTolerance>
				static bool TestTangent32Exhaustive(const double degreesRange)
				{
					Serial.println(F("Starting exhaustive GetInterpolated tests for Tangent32..."));

					const angle_t angleRange = (degreesRange * double(ANGLE_RANGE)) / 360.0;

					uint32_t errorCount = 0;
					int32_t maxError = 0;
					for (uint32_t angle = 0; angle <= angleRange; angle++)
					{
						const Fraction32::scalar_t result = IntegerSignal::Trigonometry::Tangent32(angle);
						const Fraction32::scalar_t refResult = RefTangent32(angle);

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
						if ((angle % (angleRange / 10)) == 0)
						{
							Serial.print(F("GetInterpolated progress: "));
							Serial.print(angle);
							Serial.println(F(" angles tested..."));
						}
					}
					if (errorCount == 0)
					{
						Serial.println(F("All GetInterpolated tests for Tangent32 PASSED."));
					}
					else
					{
						Serial.print(F("GetInterpolated tests for Tangent32 FAILED. Errors: "));
						Serial.println(errorCount);
					}

					Serial.print(F(" Max unit error:"));
					Serial.println(maxError);

					return errorCount == 0;
				}

				static void PrintErrorResults8(double degreesRange)
				{
					Serial.println(F("Integer Trigonometry Tangent8"));

					const angle_t angleRange = (degreesRange * double(ANGLE_RANGE)) / 360.0;

					uint8_t errorLargestI = 0;
					double errorBottomF = 0;
					double errorTopF = 0;

					double errorSum = 0;

					for (angle_t angle = 0; angle <= angleRange; angle++)
					{
						const double degrees = ((double)angle * degreesRange) / (double)(angleRange);

						const double tangentUnit = tan(DegreesToRadians(degrees));
						const Fraction8::scalar_t unitFraction = round(tangentUnit * double(Fraction8::FRACTION_1X));
						const Fraction8::scalar_t tangentFraction = IntegerSignal::Trigonometry::Tangent8(angle);

						const int16_t errorI = (int16_t)tangentFraction - unitFraction;
						const double errorF = double(errorI) / double(Fraction8::FRACTION_1X);

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

					const double errorAverageF = (double)errorSum / double(angleRange);
					const double errorAverageI = errorAverageF * (double)Fraction8::FRACTION_1X;

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

				static void PrintErrorResults16(const double degreesRange)
				{
					Serial.println(F("Integer Trigonometry Tangent16"));

					const angle_t angleRange = (degreesRange * double(ANGLE_RANGE)) / 360.0;

					uint16_t errorLargestI = 0;
					double errorBottomF = 0;
					double errorTopF = 0;

					double errorSum = 0;

					for (angle_t angle = 0; angle <= angleRange; angle++)
					{
						const double degrees = ((double)angle * degreesRange) / (double)angleRange;

						const double tangentUnit = tan(DegreesToRadians(degrees));
						const Fraction16::scalar_t unitFraction = round(tangentUnit * double(Fraction16::FRACTION_1X));
						const Fraction16::scalar_t tangentFraction = IntegerSignal::Trigonometry::Tangent16(angle);

						const int32_t errorI = (int32_t)tangentFraction - unitFraction;
						const double errorF = double(errorI) / double(Fraction16::FRACTION_1X);

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

					const double errorAverageF = (double)errorSum / (double)angleRange;
					const double errorAverageI = errorAverageF * (double)Fraction16::FRACTION_1X;

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

				static void PrintErrorResults32(const double degreesRange)
				{
					Serial.println(F("Integer Trigonometry Tangent32"));
					const angle_t angleRange = (degreesRange * double(ANGLE_RANGE)) / 360.0;

					uint32_t errorLargestI = 0;
					double errorBottomF = 0;
					double errorTopF = 0;

					double errorSum = 0;

					for (angle_t angle = 0; angle <= angleRange; angle++)
					{
						const double degrees = ((double)angle * degreesRange) / (double)angleRange;

						const double tangentUnit = tan(DegreesToRadians(degrees));
						const Fraction32::scalar_t unitFraction = round(tangentUnit * double(Fraction32::FRACTION_1X));
						const Fraction32::scalar_t tangentFraction = IntegerSignal::Trigonometry::Tangent32(angle);

						const int64_t errorI = (int64_t)tangentFraction - unitFraction;
						const double errorF = double(errorI) / double(Fraction32::FRACTION_1X);

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

							if (errorLargestI >= 6568266)
							{
								Serial.print(F("Tan error at angle: "));
								Serial.println(angle);
							}
						}

						errorSum += abs(errorF);
					}

					const double errorAverageF = (double)errorSum / (double)angleRange;
					const double errorAverageI = errorAverageF * (double)Fraction32::FRACTION_1X;

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
					static constexpr uint8_t Error16Max = 1;
					static constexpr uint8_t Error32Max = 30370 * 2;
					static constexpr double DegreesRange = 43.0;

#else
					static constexpr uint8_t Error8Max = 1;
					static constexpr uint8_t Error16Max = 1;
					static constexpr uint32_t Error32Max = 30370;
					static constexpr double DegreesRange = (45.0 * 7) / 8;
#endif
					pass &= TestTangent8Exhaustive<Error8Max>(DegreesRange);
					pass &= TestTangent16Exhaustive<Error16Max>(DegreesRange);
					pass &= TestTangent32Exhaustive<Error32Max>(DegreesRange);

					PrintErrorResults8(DegreesRange);
					PrintErrorResults16(DegreesRange);
					PrintErrorResults32(DegreesRange);

					if (!pass)
					{
						Serial.println(F("Tangent16, Tangent8, and Tangent32 tests FAILED."));
					}

					return pass;
				}
			}
		}
	}
}
#endif