#ifndef _COLOR_TEST_h
#define _COLOR_TEST_h

#include <IntegerSignalTesting.h>
#include <IntegerSignal.h>

namespace IntegerSignal
{
	namespace Color
	{
		namespace Test
		{
			using namespace FixedPoint::ScalarFraction;

			static bool ExpectEqualU32(const uint32_t actual, const uint32_t expected, const __FlashStringHelper* testName)
			{
				if (actual == expected)
				{
					return true;
				}

				Serial.print(testName);
				Serial.print(F(" failed. expected="));
				Serial.print(expected);
				Serial.print(F(" actual="));
				Serial.println(actual);
				return false;
			}

			static bool ExpectRgb8Components(const Rgb8::color_t actual,
				const uint8_t expectedRed,
				const uint8_t expectedGreen,
				const uint8_t expectedBlue,
				const uint8_t tolerance,
				const __FlashStringHelper* testName)
			{
				const uint8_t red = Rgb8::Red(actual);
				const uint8_t green = Rgb8::Green(actual);
				const uint8_t blue = Rgb8::Blue(actual);

				const uint8_t redDiff = red > expectedRed ? (red - expectedRed) : (expectedRed - red);
				const uint8_t greenDiff = green > expectedGreen ? (green - expectedGreen) : (expectedGreen - green);
				const uint8_t blueDiff = blue > expectedBlue ? (blue - expectedBlue) : (expectedBlue - blue);

				if (redDiff <= tolerance && greenDiff <= tolerance && blueDiff <= tolerance)
				{
					return true;
				}

				Serial.print(testName);
				Serial.print(F(" failed. expected rgb="));
				Serial.print(expectedRed);
				Serial.print(',');
				Serial.print(expectedGreen);
				Serial.print(',');
				Serial.print(expectedBlue);
				Serial.print(F(" actual rgb="));
				Serial.print(red);
				Serial.print(',');
				Serial.print(green);
				Serial.print(',');
				Serial.println(blue);
				return false;
			}

			static bool ExpectEqualU16(const uint16_t actual, const uint16_t expected, const __FlashStringHelper* testName)
			{
				if (actual == expected)
				{
					return true;
				}

				Serial.print(testName);
				Serial.print(F(" failed. expected="));
				Serial.print(expected);
				Serial.print(F(" actual="));
				Serial.println(actual);
				return false;
			}

			static bool TestRgb8Packing()
			{
				const Rgb8::color_t color = Rgb8::Color(0x12, 0x34, 0x56, 0x78);
				bool pass = true;

				pass &= ExpectEqualU32(color, 0x12345678UL, F("Rgb8 packed color"));
				pass &= ExpectEqualU32(Rgb8::Alpha(color), 0x12, F("Rgb8 alpha"));
				pass &= ExpectEqualU32(Rgb8::Red(color), 0x34, F("Rgb8 red"));
				pass &= ExpectEqualU32(Rgb8::Green(color), 0x56, F("Rgb8 green"));
				pass &= ExpectEqualU32(Rgb8::Blue(color), 0x78, F("Rgb8 blue"));

				return pass;
			}

			static bool TestRgb10Packing()
			{
				const Rgb10::color_t color = Rgb10::Color(2, 0x155, 0x2AA, 0x3FF);
				bool pass = true;

				pass &= ExpectEqualU32(Rgb10::Alpha(color), 2, F("Rgb10 alpha"));
				pass &= ExpectEqualU16(Rgb10::Red(color), 0x155, F("Rgb10 red"));
				pass &= ExpectEqualU16(Rgb10::Green(color), 0x2AA, F("Rgb10 green"));
				pass &= ExpectEqualU16(Rgb10::Blue(color), 0x3FF, F("Rgb10 blue"));

				return pass;
			}

			static bool TestRgbFraction16Color8()
			{
				const RgbFraction16::color_t color = RgbFraction16::Color8(255, 128, 0);
				bool pass = true;

				pass &= ExpectEqualU16(color.red, UFraction16::FRACTION_1X, F("RgbFraction16 Color8 red"));
				pass &= ExpectEqualU16(color.green, UFraction16::GetScalar<uint8_t>(128, UINT8_MAX), F("RgbFraction16 Color8 green"));
				pass &= ExpectEqualU16(color.blue, 0, F("RgbFraction16 Color8 blue"));

				return pass;
			}

			static bool TestHsvGrayRegression()
			{
				const ufraction16_t half = UFraction16::FRACTION_1X / 2;
				bool pass = true;

				pass &= ExpectEqualU32(Rgb8::ColorHsvFraction(0, 0, half), Rgb8::Color(127, 127, 127), F("HSV gray rgb8"));

				const Rgb10::color_t rgb10 = Rgb10::ColorHsvFraction(0, 0, half);
				pass &= ExpectEqualU16(Rgb10::Red(rgb10), Rgb10::Green(rgb10), F("HSV gray rgb10 rg"));
				pass &= ExpectEqualU16(Rgb10::Green(rgb10), Rgb10::Blue(rgb10), F("HSV gray rgb10 gb"));

				const RgbFraction16::color_t rgbf = RgbFraction16::ColorHsvFraction(0, 0, half);
				pass &= ExpectEqualU16(rgbf.red, half, F("HSV gray rgbf red"));
				pass &= ExpectEqualU16(rgbf.green, half, F("HSV gray rgbf green"));
				pass &= ExpectEqualU16(rgbf.blue, half, F("HSV gray rgbf blue"));

				return pass;
			}

			static bool TestHsvPrimaryRegression()
			{
				const ufraction16_t unit = UFraction16::FRACTION_1X;
				const ufraction16_t hueGreen = unit / 3;
				const ufraction16_t hueBlue = (ufraction16_t)(((uint32_t)unit * 2UL) / 3UL);
				bool pass = true;

				pass &= ExpectRgb8Components(Rgb8::ColorHsvFraction(0, unit, unit), 255, 0, 0, 0, F("HSV primary red rgb8"));
				pass &= ExpectRgb8Components(Rgb8::ColorHsvFraction(hueGreen, unit, unit), 0, 255, 0, 1, F("HSV primary green rgb8"));
				pass &= ExpectRgb8Components(Rgb8::ColorHsvFraction(hueBlue, unit, unit), 0, 0, 255, 1, F("HSV primary blue rgb8"));

				return pass;
			}

			static bool TestHsvWrapRegression()
			{
				const ufraction16_t unit = UFraction16::FRACTION_1X;
				bool pass = true;

				pass &= ExpectEqualU32(Rgb8::ColorHsvFraction(0, unit, unit), Rgb8::ColorHsvFraction(unit, unit, unit), F("HSV wrap rgb8"));

				const RgbFraction16::color_t zero = RgbFraction16::ColorHsvFraction(0, unit, unit);
				const RgbFraction16::color_t wrapped = RgbFraction16::ColorHsvFraction(unit, unit, unit);
				pass &= ExpectEqualU16(zero.red, wrapped.red, F("HSV wrap rgbf red"));
				pass &= ExpectEqualU16(zero.green, wrapped.green, F("HSV wrap rgbf green"));
				pass &= ExpectEqualU16(zero.blue, wrapped.blue, F("HSV wrap rgbf blue"));

				return pass;
			}

			static bool TestHsvBlackWhiteRegression()
			{
				const ufraction16_t unit = UFraction16::FRACTION_1X;
				bool pass = true;

				pass &= ExpectEqualU32(Rgb8::ColorHsvFraction(0, unit, 0), Rgb8::Color(0, 0, 0), F("HSV black rgb8"));
				pass &= ExpectEqualU32(Rgb8::ColorHsvFraction(0, 0, unit), Rgb8::WHITE, F("HSV white rgb8"));

				const RgbFraction16::color_t black = RgbFraction16::ColorHsvFraction(0, unit, 0);
				pass &= ExpectEqualU16(black.red, 0, F("HSV black rgbf red"));
				pass &= ExpectEqualU16(black.green, 0, F("HSV black rgbf green"));
				pass &= ExpectEqualU16(black.blue, 0, F("HSV black rgbf blue"));

				return pass;
			}

			static bool RunTests()
			{
				Serial.println(F("Starting color tests..."));

				bool pass = true;
				pass &= TestRgb8Packing();
				pass &= TestRgb10Packing();
				pass &= TestRgbFraction16Color8();
				pass &= TestHsvGrayRegression();
				pass &= TestHsvPrimaryRegression();
				pass &= TestHsvWrapRegression();
				pass &= TestHsvBlackWhiteRegression();

				if (pass)
				{
					Serial.println(F("Color tests PASSED."));
				}
				else
				{
					Serial.println(F("Color tests FAILED."));
				}

				return pass;
			}
		}
	}
}

#endif