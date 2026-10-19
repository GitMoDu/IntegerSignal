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

			static inline bool ExpectEqualU32(const uint32_t actual, const uint32_t expected, const __FlashStringHelper* testName)
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

			static inline void RecordComponentError(const uint16_t actual, const uint16_t expected, uint16_t& maximumError)
			{
				const uint16_t difference = actual > expected ? actual - expected : expected - actual;
				if (difference > maximumError)
				{
					maximumError = difference;
				}
			}

			static inline bool ExpectNearU16(const uint16_t actual, const uint16_t expected,
				const uint16_t tolerance, const __FlashStringHelper* testName)
			{
				const uint16_t difference = actual > expected ? actual - expected : expected - actual;
				if (difference <= tolerance)
				{
					return true;
				}

				Serial.print(testName);
				Serial.print(F(" failed. expected="));
				Serial.print(expected);
				Serial.print(F(" actual="));
				Serial.print(actual);
				Serial.print(F(" tolerance="));
				Serial.println(tolerance);
				return false;
			}

			static inline bool ExpectRgb8Components(const Rgb8::color_t actual,
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

			static inline bool ExpectEqualU16(const uint16_t actual, const uint16_t expected, const __FlashStringHelper* testName)
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

			static inline bool TestOklchNativeMatchesFractional()
			{
				bool pass = true;
				for (uint16_t i = 0; i < 16; ++i)
				{
					const Rgb8::component_t red8 = uint8_t(i * 17);
					const Rgb8::component_t green8 = uint8_t(i * 73);
					const Rgb8::component_t blue8 = uint8_t(i * 151);
					const Oklch::oklch_t native8 = Oklch::FromRgb<Rgb8::component_t, Rgb8::COMPONENT_MAX>(red8, green8, blue8);
					const Oklch::oklch_t fractional8 = Oklch::FromLinearRgb(
						UFraction16::GetScalar<Rgb8::component_t>(red8, Rgb8::COMPONENT_MAX),
						UFraction16::GetScalar<Rgb8::component_t>(green8, Rgb8::COMPONENT_MAX),
						UFraction16::GetScalar<Rgb8::component_t>(blue8, Rgb8::COMPONENT_MAX));

					const Rgb10::component_t red10 = uint16_t((i * 61) & Rgb10::COMPONENT_MAX);
					const Rgb10::component_t green10 = uint16_t((i * 127) & Rgb10::COMPONENT_MAX);
					const Rgb10::component_t blue10 = uint16_t((i * 251) & Rgb10::COMPONENT_MAX);
					const Oklch::oklch_t native10 = Oklch::FromRgb<Rgb10::component_t, Rgb10::COMPONENT_MAX>(red10, green10, blue10);
					const Oklch::oklch_t fractional10 = Oklch::FromLinearRgb(
						UFraction16::GetScalar<Rgb10::component_t>(red10, Rgb10::COMPONENT_MAX),
						UFraction16::GetScalar<Rgb10::component_t>(green10, Rgb10::COMPONENT_MAX),
						UFraction16::GetScalar<Rgb10::component_t>(blue10, Rgb10::COMPONENT_MAX));

					pass &= ExpectNearU16(native8.lightness, fractional8.lightness, 2, F("Oklch native/fractional RGB8 lightness"));
					pass &= ExpectNearU16(native8.chroma, fractional8.chroma, 2, F("Oklch native/fractional RGB8 chroma"));
					pass &= ExpectNearU16(native8.hue, fractional8.hue, 4, F("Oklch native/fractional RGB8 hue"));
					pass &= ExpectNearU16(native10.lightness, fractional10.lightness, 2, F("Oklch native/fractional RGB10 lightness"));
					pass &= ExpectNearU16(native10.chroma, fractional10.chroma, 2, F("Oklch native/fractional RGB10 chroma"));
					pass &= ExpectNearU16(native10.hue, fractional10.hue, 4, F("Oklch native/fractional RGB10 hue"));

					const ufraction16_t lightness = ufraction16_t(i * 2048);
					const ufraction16_t chroma = ufraction16_t(i * 1024);
					const Trigonometry::angle_t hue = Trigonometry::angle_t(i * 4096);
					const Rgb8::color_t nativeOutput8 = Rgb8::ColorOklch(lightness, chroma, hue);
					const Rgb10::color_t nativeOutput10 = Rgb10::ColorOklch(lightness, chroma, hue);
					const RgbFraction16::color_t fractionalOutput = RgbFraction16::ColorOklch(lightness, chroma, hue);
					pass &= ExpectNearU16(Rgb8::Red(nativeOutput8), Fraction(fractionalOutput.red, Rgb8::COMPONENT_MAX),
						1, F("Oklch native/fractional RGB8 red output"));
					pass &= ExpectNearU16(Rgb8::Green(nativeOutput8), Fraction(fractionalOutput.green, Rgb8::COMPONENT_MAX),
						1, F("Oklch native/fractional RGB8 green output"));
					pass &= ExpectNearU16(Rgb8::Blue(nativeOutput8), Fraction(fractionalOutput.blue, Rgb8::COMPONENT_MAX),
						1, F("Oklch native/fractional RGB8 blue output"));
					pass &= ExpectNearU16(Rgb10::Red(nativeOutput10), Fraction(fractionalOutput.red, Rgb10::COMPONENT_MAX),
						1, F("Oklch native/fractional RGB10 red output"));
					pass &= ExpectNearU16(Rgb10::Green(nativeOutput10), Fraction(fractionalOutput.green, Rgb10::COMPONENT_MAX),
						1, F("Oklch native/fractional RGB10 green output"));
					pass &= ExpectNearU16(Rgb10::Blue(nativeOutput10), Fraction(fractionalOutput.blue, Rgb10::COMPONENT_MAX),
						1, F("Oklch native/fractional RGB10 blue output"));
				}

				return pass;
			}

			static inline bool TestRgbToHsvSampledRoundTripError()
			{
				const ufraction16_t unit = UFraction16::FRACTION_1X;
				uint16_t maximumError8 = 0;
				uint16_t maximumError10 = 0;
				uint16_t maximumErrorFraction = 0;
				bool pass = true;

				for (uint16_t redIndex = 0; redIndex <= 16; ++redIndex)
				{
					const uint8_t red8 = uint8_t((redIndex * UINT8_MAX) / 16);
					const uint16_t red10 = uint16_t((redIndex * Rgb10::COMPONENT_MAX) / 16);
					const ufraction16_t redFraction = ufraction16_t((uint32_t(redIndex) * unit) / 16);

					for (uint16_t greenIndex = 0; greenIndex <= 16; ++greenIndex)
					{
						const uint8_t green8 = uint8_t((greenIndex * UINT8_MAX) / 16);
						const uint16_t green10 = uint16_t((greenIndex * Rgb10::COMPONENT_MAX) / 16);
						const ufraction16_t greenFraction = ufraction16_t((uint32_t(greenIndex) * unit) / 16);

						for (uint16_t blueIndex = 0; blueIndex <= 16; ++blueIndex)
						{
							const uint8_t blue8 = uint8_t((blueIndex * UINT8_MAX) / 16);
							const uint16_t blue10 = uint16_t((blueIndex * Rgb10::COMPONENT_MAX) / 16);
							const ufraction16_t blueFraction = ufraction16_t((uint32_t(blueIndex) * unit) / 16);

							const Rgb8::color_t input8 = Rgb8::Color(red8, green8, blue8);
							const Hsv::hsv_t<uint8_t> hsv8 = Rgb8::ToHsv(input8);
							const Rgb8::color_t output8 = Rgb8::ColorHsv(hsv8.hue, hsv8.saturation, hsv8.value);
							RecordComponentError(Rgb8::Red(output8), red8, maximumError8);
							RecordComponentError(Rgb8::Green(output8), green8, maximumError8);
							RecordComponentError(Rgb8::Blue(output8), blue8, maximumError8);

							const Rgb10::color_t input10 = Rgb10::Color(red10, green10, blue10);
							const Hsv::hsv_t<uint8_t> hsv10 = Rgb10::ToHsv(input10);
							const Rgb10::color_t output10 = Rgb10::ColorHsv(hsv10.hue, hsv10.saturation, hsv10.value);
							RecordComponentError(Rgb10::Red(output10), red10, maximumError10);
							RecordComponentError(Rgb10::Green(output10), green10, maximumError10);
							RecordComponentError(Rgb10::Blue(output10), blue10, maximumError10);

							const RgbFraction16::color_t inputFraction = RgbFraction16::Color(redFraction, greenFraction, blueFraction);
							const Hsv::hsv_t<ufraction16_t> hsvFraction = RgbFraction16::ToHsv(inputFraction);
							const RgbFraction16::color_t outputFraction = RgbFraction16::ColorHsv(
								hsvFraction.hue, hsvFraction.saturation, hsvFraction.value);
							RecordComponentError(outputFraction.red, redFraction, maximumErrorFraction);
							RecordComponentError(outputFraction.green, greenFraction, maximumErrorFraction);
							RecordComponentError(outputFraction.blue, blueFraction, maximumErrorFraction);
						}
					}
				}

				Serial.print(F("RGB-to-HSV sampled native max channel error (RGB8/RGB10/UQ0.15): "));
				Serial.print(maximumError8);
				Serial.print('/');
				Serial.print(maximumError10);
				Serial.print('/');
				Serial.println(maximumErrorFraction);

				pass &= ExpectNearU16(maximumError8, 0, 2, F("RGB-to-HSV sampled RGB8 maximum error"));
				pass &= ExpectNearU16(maximumError10, 0, 8, F("RGB-to-HSV sampled RGB10 maximum error"));
				pass &= ExpectNearU16(maximumErrorFraction, 0, 16, F("RGB-to-HSV sampled fractional maximum error"));
				return pass;
			}

			static inline bool TestRgbFraction16KeyColors()
			{
				const ufraction16_t unit = UFraction16::FRACTION_1X;
				const RgbFraction16::color_t colors[] =
				{
					RgbFraction16::BLACK,
					RgbFraction16::WHITE,
					RgbFraction16::Color(unit / 2, unit / 2, unit / 2),
					RgbFraction16::Color(unit / 2, unit / 2 + 1, unit / 2),
					RgbFraction16::RED,
					RgbFraction16::Color(unit, 1, 0),
					RgbFraction16::Color(unit, unit - 1, 0),
					RgbFraction16::Color(unit, unit, 0),
					RgbFraction16::Color(1, unit, 0),
					RgbFraction16::Color(unit - 1, unit, 0),
					RgbFraction16::GREEN,
					RgbFraction16::Color(0, unit, 1),
					RgbFraction16::Color(0, unit, unit - 1),
					RgbFraction16::Color(0, unit, unit),
					RgbFraction16::Color(0, 1, unit),
					RgbFraction16::Color(0, unit - 1, unit),
					RgbFraction16::BLUE,
					RgbFraction16::Color(1, 0, unit),
					RgbFraction16::Color(unit - 1, 0, unit),
					RgbFraction16::Color(unit, 0, unit),
					RgbFraction16::Color(unit, 0, 1),
					RgbFraction16::Color(unit, 0, unit - 1)
				};
				uint16_t maximumReferenceError = 0;
				uint16_t maximumRoundTripError = 0;

				for (uint8_t i = 0; i < sizeof(colors) / sizeof(colors[0]); ++i)
				{
					const Hsv::hsv_t<ufraction16_t> hsv = RgbFraction16::ToHsv(colors[i]);
					const RgbFraction16::color_t native = RgbFraction16::ColorHsv(
						hsv.hue, hsv.saturation, hsv.value);
					const RgbFraction16::color_t reference = RgbFraction16::ColorHsvFraction(
						UFraction16::GetScalar<uint16_t>(hsv.hue, Trigonometry::ANGLE_RANGE),
						hsv.saturation, hsv.value);

					RecordComponentError(native.red, reference.red, maximumReferenceError);
					RecordComponentError(native.green, reference.green, maximumReferenceError);
					RecordComponentError(native.blue, reference.blue, maximumReferenceError);
					RecordComponentError(native.red, colors[i].red, maximumRoundTripError);
					RecordComponentError(native.green, colors[i].green, maximumRoundTripError);
					RecordComponentError(native.blue, colors[i].blue, maximumRoundTripError);
				}

				Serial.print(F("RGBFraction16 key-color max error (reference/round-trip): "));
				Serial.print(maximumReferenceError);
				Serial.print('/');
				Serial.println(maximumRoundTripError);

				bool pass = true;
				pass &= ExpectNearU16(maximumReferenceError, 0, 8, F("RGBFraction16 key-color reference error"));
				pass &= ExpectNearU16(maximumRoundTripError, 0, 16, F("RGBFraction16 key-color round-trip error"));
				return pass;
			}

			static inline bool TestOklchLinearRgbRoundTrip()
			{
				const ufraction16_t unit = UFraction16::FRACTION_1X;
				const RgbFraction16::color_t colors[] = {
					RgbFraction16::BLACK,
					RgbFraction16::WHITE,
					RgbFraction16::RED,
					RgbFraction16::GREEN,
					RgbFraction16::BLUE,
					RgbFraction16::Color(unit / 3, unit / 2, unit)
				};
				bool pass = true;

				for (uint8_t i = 0; i < sizeof(colors) / sizeof(colors[0]); ++i)
				{
					const Oklch::oklch_t oklch = RgbFraction16::ToOklch(colors[i]);
					const RgbFraction16::color_t output = RgbFraction16::ColorOklch(
						oklch.lightness, oklch.chroma, oklch.hue);
					pass &= ExpectNearU16(output.red, colors[i].red, 32, F("Oklch linear RGB red round trip"));
					pass &= ExpectNearU16(output.green, colors[i].green, 32, F("Oklch linear RGB green round trip"));
					pass &= ExpectNearU16(output.blue, colors[i].blue, 32, F("Oklch linear RGB blue round trip"));
				}

				const Oklch::oklch_t white = RgbFraction16::ToOklch(RgbFraction16::WHITE);
				pass &= ExpectNearU16(white.lightness, unit, 2, F("Oklch white lightness"));
				pass &= ExpectNearU16(white.chroma, 0, 2, F("Oklch white chroma"));

				return pass;
			}

			static inline bool TestOklchIntegerRgbRoundTrip()
			{
				const Rgb8::color_t input8 = Rgb8::Color(64, 128, 200);
				const Oklch::oklch_t oklch8 = Rgb8::ToOklch(input8);
				const Rgb8::color_t output8 = Rgb8::ColorOklch(oklch8.lightness, oklch8.chroma, oklch8.hue);
				const Rgb10::color_t input10 = Rgb10::Color(200, 500, 900);
				const Oklch::oklch_t oklch10 = Rgb10::ToOklch(input10);
				const Rgb10::color_t output10 = Rgb10::ColorOklch(oklch10.lightness, oklch10.chroma, oklch10.hue);
				bool pass = true;

				pass &= ExpectNearU16(Rgb8::Red(output8), Rgb8::Red(input8), 2, F("Oklch RGB8 red round trip"));
				pass &= ExpectNearU16(Rgb8::Green(output8), Rgb8::Green(input8), 2, F("Oklch RGB8 green round trip"));
				pass &= ExpectNearU16(Rgb8::Blue(output8), Rgb8::Blue(input8), 2, F("Oklch RGB8 blue round trip"));
				pass &= ExpectNearU16(Rgb10::Red(output10), Rgb10::Red(input10), 4, F("Oklch RGB10 red round trip"));
				pass &= ExpectNearU16(Rgb10::Green(output10), Rgb10::Green(input10), 4, F("Oklch RGB10 green round trip"));
				pass &= ExpectNearU16(Rgb10::Blue(output10), Rgb10::Blue(input10), 4, F("Oklch RGB10 blue round trip"));

				return pass;
			}

			static inline bool TestRgbToHsvPrimaries()
			{
				const uint32_t hueRange = uint32_t(Trigonometry::ANGLE_RANGE);
				const Hsv::hsv_t<uint8_t> red = Rgb8::ToHsv(Rgb8::RED);
				const Hsv::hsv_t<uint8_t> yellow = Rgb8::ToHsv(Rgb8::Color(255, 255, 0));
				const Hsv::hsv_t<uint8_t> green = Rgb8::ToHsv(Rgb8::GREEN);
				const Hsv::hsv_t<uint8_t> cyan = Rgb8::ToHsv(Rgb8::Color(0, 255, 255));
				const Hsv::hsv_t<uint8_t> blue = Rgb8::ToHsv(Rgb8::BLUE);
				const Hsv::hsv_t<uint8_t> magenta = Rgb8::ToHsv(Rgb8::Color(255, 0, 255));
				bool pass = true;

				pass &= ExpectEqualU32(red.hue, 0, F("RGB-to-HSV red hue"));
				pass &= ExpectEqualU32(yellow.hue, hueRange / 6, F("RGB-to-HSV yellow hue"));
				pass &= ExpectEqualU32(green.hue, (2UL * hueRange) / 6, F("RGB-to-HSV green hue"));
				pass &= ExpectEqualU32(cyan.hue, hueRange / 2, F("RGB-to-HSV cyan hue"));
				pass &= ExpectEqualU32(blue.hue, (4UL * hueRange) / 6, F("RGB-to-HSV blue hue"));
				pass &= ExpectEqualU32(magenta.hue, (5UL * hueRange) / 6, F("RGB-to-HSV magenta hue"));
				pass &= ExpectEqualU16(red.saturation, UINT8_MAX, F("RGB-to-HSV primary native saturation"));
				pass &= ExpectEqualU16(blue.value, UINT8_MAX, F("RGB-to-HSV primary native value"));

				return pass;
			}

			static inline bool TestRgbToHsvAchromaticAndFormats()
			{
				const ufraction16_t unit = UFraction16::FRACTION_1X;
				const Hsv::hsv_t<uint8_t> gray8 = Rgb8::ToHsv(Rgb8::Color(128, 128, 128));
				const Hsv::hsv_fraction_t gray8Fraction = Rgb8::ToHsvFraction(Rgb8::Color(128, 128, 128));
				const Hsv::hsv_t<uint8_t> black8 = Rgb8::ToHsv(Rgb8::BLACK);
				const Hsv::hsv_t<uint8_t> white10 = Rgb10::ToHsv(Rgb10::WHITE);
				const Hsv::hsv_t<uint8_t> green10 = Rgb10::ToHsv(Rgb10::GREEN);
				const RgbFraction16::color_t fractional = RgbFraction16::Color(unit / 3, unit / 2, unit);
				const Hsv::hsv_t<ufraction16_t> fractionalHsv = RgbFraction16::ToHsv(fractional);
				bool pass = true;

				pass &= ExpectEqualU32(gray8.hue, 0, F("RGB-to-HSV gray hue"));
				pass &= ExpectEqualU16(gray8.saturation, 0, F("RGB-to-HSV gray saturation"));
				pass &= ExpectEqualU16(gray8.value, 128, F("RGB-to-HSV gray native value"));
				pass &= ExpectEqualU16(gray8Fraction.value, UFraction16::GetScalar<uint8_t>(128, UINT8_MAX), F("RGB-to-HSV gray fractional value"));
				pass &= ExpectEqualU16(black8.value, 0, F("RGB-to-HSV black value"));
				pass &= ExpectEqualU16(white10.value, UINT8_MAX, F("RGB-to-HSV RGB10 native value"));
				pass &= ExpectEqualU16(green10.saturation, UINT8_MAX, F("RGB-to-HSV RGB10 native saturation"));
				pass &= ExpectEqualU16(fractionalHsv.value, unit, F("RGB-to-HSV fractional value"));
				pass &= ExpectEqualU16(fractionalHsv.saturation,
					UFraction16::GetScalar<uint32_t>(unit - unit / 3, unit), F("RGB-to-HSV fractional saturation"));

				return pass;
			}

			static inline bool TestRgbToHsvRoundTrip()
			{
				const ufraction16_t unit = UFraction16::FRACTION_1X;
				const Rgb8::color_t input8 = Rgb8::Color(17, 99, 201);
				const Hsv::hsv_t<uint8_t> hsv8 = Rgb8::ToHsv(input8);
				const Rgb8::color_t output8 = Rgb8::ColorHsv(hsv8.hue, hsv8.saturation, hsv8.value);
				const Hsv::hsv_fraction_t hsv8Fraction = Rgb8::ToHsvFraction(input8);
				const Rgb8::color_t output8Fraction = Rgb8::ColorHsvFraction(
					UFraction16::GetScalar<uint16_t>(hsv8Fraction.hue, Trigonometry::ANGLE_RANGE),
					hsv8Fraction.saturation, hsv8Fraction.value);
				const Rgb10::color_t input10 = Rgb10::Color(0, 50, 700, 1023);
				const Hsv::hsv_t<uint8_t> hsv10 = Rgb10::ToHsv(input10);
				const Rgb10::color_t output10 = Rgb10::ColorHsv(hsv10.hue, hsv10.saturation, hsv10.value);
				const Hsv::hsv_fraction_t hsv10Fraction = Rgb10::ToHsvFraction(input10);
				const Rgb10::color_t output10Fraction = Rgb10::ColorHsvFraction(
					UFraction16::GetScalar<uint16_t>(hsv10Fraction.hue, Trigonometry::ANGLE_RANGE),
					hsv10Fraction.saturation, hsv10Fraction.value);
				const RgbFraction16::color_t inputFraction = RgbFraction16::Color(unit / 3, unit / 2, unit);
				const Hsv::hsv_t<ufraction16_t> hsvFraction = RgbFraction16::ToHsv(inputFraction);
				const RgbFraction16::color_t outputFraction = RgbFraction16::ColorHsv(
					hsvFraction.hue, hsvFraction.saturation, hsvFraction.value);
				bool pass = true;

				pass &= ExpectRgb8Components(output8, 17, 99, 201, 2, F("RGB-to-HSV RGB8 round trip"));
				pass &= ExpectRgb8Components(output8Fraction, 17, 99, 201, 1, F("RGB-to-HSV RGB8 fractional round trip"));
				pass &= ExpectNearU16(Rgb10::Red(output10), Rgb10::Red(input10), 5, F("RGB-to-HSV RGB10 red round trip"));
				pass &= ExpectNearU16(Rgb10::Green(output10), Rgb10::Green(input10), 5, F("RGB-to-HSV RGB10 green round trip"));
				pass &= ExpectNearU16(Rgb10::Blue(output10), Rgb10::Blue(input10), 5, F("RGB-to-HSV RGB10 blue round trip"));
				pass &= ExpectNearU16(Rgb10::Red(output10Fraction), Rgb10::Red(input10), 1, F("RGB-to-HSV RGB10 fractional red round trip"));
				pass &= ExpectNearU16(Rgb10::Green(output10Fraction), Rgb10::Green(input10), 1, F("RGB-to-HSV RGB10 fractional green round trip"));
				pass &= ExpectNearU16(Rgb10::Blue(output10Fraction), Rgb10::Blue(input10), 1, F("RGB-to-HSV RGB10 fractional blue round trip"));
				pass &= ExpectNearU16(outputFraction.red, inputFraction.red, 4, F("RGB-to-HSV fractional red round trip"));
				pass &= ExpectNearU16(outputFraction.green, inputFraction.green, 4, F("RGB-to-HSV fractional green round trip"));
				pass &= ExpectNearU16(outputFraction.blue, inputFraction.blue, 4, F("RGB-to-HSV fractional blue round trip"));

				return pass;
			}

			static inline bool TestRgb10Packing()
			{
				const Rgb10::color_t color = Rgb10::Color(2, 0x155, 0x2AA, 0x3FF);
				bool pass = true;

				pass &= ExpectEqualU32(Rgb10::Alpha(color), 2, F("Rgb10 alpha"));
				pass &= ExpectEqualU16(Rgb10::Red(color), 0x155, F("Rgb10 red"));
				pass &= ExpectEqualU16(Rgb10::Green(color), 0x2AA, F("Rgb10 green"));
				pass &= ExpectEqualU16(Rgb10::Blue(color), 0x3FF, F("Rgb10 blue"));

				return pass;
			}

			static inline bool TestRgbFraction16Color8()
			{
				const RgbFraction16::color_t color = RgbFraction16::Color8(255, 128, 0);
				bool pass = true;

				pass &= ExpectEqualU16(color.red, UFraction16::FRACTION_1X, F("RgbFraction16 Color8 red"));
				pass &= ExpectEqualU16(color.green, UFraction16::GetScalar<uint8_t>(128, UINT8_MAX), F("RgbFraction16 Color8 green"));
				pass &= ExpectEqualU16(color.blue, 0, F("RgbFraction16 Color8 blue"));

				return pass;
			}

			static inline bool TestHsvGrayRegression()
			{
				const ufraction16_t half = UFraction16::FRACTION_1X / 2;
				bool pass = true;

				pass &= ExpectEqualU32(Rgb8::ColorHsv(0, 0, 127), Rgb8::Color(127, 127, 127), F("HSV native gray rgb8"));
				const Rgb10::color_t nativeGray10 = Rgb10::ColorHsv(0, 0, 127);
				const uint16_t gray10 = uint16_t((uint32_t(127) * Rgb10::COMPONENT_MAX) / UINT8_MAX);
				pass &= ExpectEqualU16(Rgb10::Red(nativeGray10), gray10, F("HSV native gray rgb10"));

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

			static inline bool TestHsvNativeMatchesFractional()
			{
				static constexpr uint8_t Saturation = 173;
				static constexpr uint8_t Value = 219;
				bool pass = true;

				for (uint8_t sector = 0; sector < 6; ++sector)
				{
					const uint16_t sectorHue = uint16_t(
						(uint32_t(sector) * (uint32_t(Trigonometry::ANGLE_RANGE) + 1) + 5) / 6);
					for (uint8_t offset = 0; offset < 2; ++offset)
					{
						const Trigonometry::angle_t hue = Trigonometry::angle_t(sectorHue - offset);
						const Rgb8::color_t native8 = Rgb8::ColorHsv(hue, Saturation, Value);
						const Rgb8::color_t fractional8 = Rgb8::ColorHsvFraction(
							UFraction16::GetScalar<uint16_t>(hue, Trigonometry::ANGLE_RANGE),
							UFraction16::GetScalar<uint8_t>(Saturation, UINT8_MAX),
							UFraction16::GetScalar<uint8_t>(Value, UINT8_MAX));
						pass &= ExpectRgb8Components(native8,
							Rgb8::Red(fractional8), Rgb8::Green(fractional8), Rgb8::Blue(fractional8),
							2, F("HSV native/fractional RGB8"));

						const Rgb10::color_t native10 = Rgb10::ColorHsv(hue, Saturation, Value);
						const Rgb10::color_t fractional10 = Rgb10::ColorHsvFraction(
							UFraction16::GetScalar<uint16_t>(hue, Trigonometry::ANGLE_RANGE),
							UFraction16::GetScalar<uint8_t>(Saturation, UINT8_MAX),
							UFraction16::GetScalar<uint8_t>(Value, UINT8_MAX));
						pass &= ExpectNearU16(Rgb10::Red(native10), Rgb10::Red(fractional10), 8, F("HSV native/fractional RGB10 red"));
						pass &= ExpectNearU16(Rgb10::Green(native10), Rgb10::Green(fractional10), 8, F("HSV native/fractional RGB10 green"));
						pass &= ExpectNearU16(Rgb10::Blue(native10), Rgb10::Blue(fractional10), 8, F("HSV native/fractional RGB10 blue"));

						const ufraction16_t saturationFraction = UFraction16::GetScalar<uint8_t>(Saturation, UINT8_MAX);
						const ufraction16_t valueFraction = UFraction16::GetScalar<uint8_t>(Value, UINT8_MAX);
						const RgbFraction16::color_t nativeFraction = RgbFraction16::ColorHsv(
							hue, saturationFraction, valueFraction);
						const RgbFraction16::color_t referenceFraction = RgbFraction16::ColorHsvFraction(
							UFraction16::GetScalar<uint16_t>(hue, Trigonometry::ANGLE_RANGE),
							saturationFraction, valueFraction);
						pass &= ExpectNearU16(nativeFraction.red, referenceFraction.red, 8, F("HSV native/fractional RGBFraction16 red"));
						pass &= ExpectNearU16(nativeFraction.green, referenceFraction.green, 8, F("HSV native/fractional RGBFraction16 green"));
						pass &= ExpectNearU16(nativeFraction.blue, referenceFraction.blue, 8, F("HSV native/fractional RGBFraction16 blue"));
					}
				}

				return pass;
			}

			static inline bool TestHsvPrimaryRegression()
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

			static inline bool TestHsvWrapRegression()
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

			static inline bool TestHsvBlackWhiteRegression()
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

			static inline bool RunTests()
			{
				Serial.println(F("Starting color tests..."));

				bool pass = true;
				pass &= TestRgb8Packing();
				pass &= TestRgb10Packing();
				pass &= TestRgbFraction16Color8();
				pass &= TestRgbFraction16KeyColors();
				Serial.println(F("Starting Oklch tests..."));
				bool oklchPass = true;
				oklchPass &= TestOklchLinearRgbRoundTrip();
				oklchPass &= TestOklchIntegerRgbRoundTrip();
				oklchPass &= TestOklchNativeMatchesFractional();
				Serial.println(oklchPass ? F("Oklch tests PASSED.") : F("Oklch tests FAILED."));
				pass &= oklchPass;
				pass &= TestHsvGrayRegression();
				pass &= TestHsvNativeMatchesFractional();
				pass &= TestHsvPrimaryRegression();
				pass &= TestHsvWrapRegression();
				pass &= TestHsvBlackWhiteRegression();
				pass &= TestRgbToHsvPrimaries();
				pass &= TestRgbToHsvAchromaticAndFormats();
				pass &= TestRgbToHsvRoundTrip();
				pass &= TestRgbToHsvSampledRoundTripError();

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