#ifndef _INTEGER_SIGNAL_BENCHMARKS_COLORS_h
#define _INTEGER_SIGNAL_BENCHMARKS_COLORS_h

#include "Benchmark.h"

template<typename Operation>
static void BenchmarkColorConversion(const Operation& operation)
{
	uint32_t checksum = 0;
	volatile uint32_t input = 0;
	const uint32_t durationStart = micros();
	for (benchmark_count_t i = 0; i < BenchmarkConfig::TestSize; ++i)
	{
		checksum ^= operation(input);
		++input;
	}
	const uint32_t duration = micros() - durationStart;

	ConsumeBenchmarkResult(checksum);
	Serial.print(F("\t\t"));
	PrintAverageNanoseconds(duration, BenchmarkConfig::TestSize);
	Serial.println(F(" ns"));
}

static void BenchmarkColors()
{
	using namespace IntegerSignal;

	Serial.println(F("\tRGB8 ToHsv"));
	BenchmarkColorConversion([](const uint32_t i) -> uint32_t
	{
		const Rgb8::color_t color = Rgb8::Color(uint8_t(i), uint8_t(i >> 5), uint8_t(i >> 11));
		const Hsv::hsv_t<uint8_t> hsv = Rgb8::ToHsv(color);
		return uint32_t(hsv.hue) ^ (uint32_t(hsv.saturation) << 16) ^ uint32_t(hsv.value);
	});

	Serial.println(F("\tRGB8 ToHsvFraction"));
	BenchmarkColorConversion([](const uint32_t i) -> uint32_t
	{
		const Rgb8::color_t color = Rgb8::Color(uint8_t(i), uint8_t(i >> 5), uint8_t(i >> 11));
		const Hsv::hsv_fraction_t hsv = Rgb8::ToHsvFraction(color);
		return uint32_t(hsv.hue) ^ (uint32_t(hsv.saturation) << 16) ^ uint32_t(hsv.value);
	});

	Serial.println(F("\tRGB8 ColorHsv"));
	BenchmarkColorConversion([](const uint32_t i) -> uint32_t
	{
		return Rgb8::ColorHsv(angle_t(i), uint8_t(i), uint8_t(i >> 3));
	});

	Serial.println(F("\tRGB8 ColorHsvFraction"));
	BenchmarkColorConversion([](const uint32_t i) -> uint32_t
	{
		const ScalarFraction::ufraction16_t hue = ScalarFraction::ufraction16_t(i & ScalarFraction::UFraction16::FRACTION_1X);
		const ScalarFraction::ufraction16_t saturation = ScalarFraction::ufraction16_t((i >> 1) & ScalarFraction::UFraction16::FRACTION_1X);
		const ScalarFraction::ufraction16_t value = ScalarFraction::ufraction16_t((i >> 2) & ScalarFraction::UFraction16::FRACTION_1X);
		return Rgb8::ColorHsvFraction(hue, saturation, value);
	});

	Serial.println(F("\tRGB10 ToHsv"));
	BenchmarkColorConversion([](const uint32_t i) -> uint32_t
	{
		const Rgb10::color_t color = Rgb10::Color(
			uint16_t(i & Rgb10::COMPONENT_MAX),
			uint16_t((i >> 3) & Rgb10::COMPONENT_MAX),
			uint16_t((i >> 6) & Rgb10::COMPONENT_MAX));
		const Hsv::hsv_t<uint8_t> hsv = Rgb10::ToHsv(color);
		return uint32_t(hsv.hue) ^ (uint32_t(hsv.saturation) << 16) ^ uint32_t(hsv.value);
	});

	Serial.println(F("\tRGB10 ToHsvFraction"));
	BenchmarkColorConversion([](const uint32_t i) -> uint32_t
	{
		const Rgb10::color_t color = Rgb10::Color(
			uint16_t(i & Rgb10::COMPONENT_MAX),
			uint16_t((i >> 3) & Rgb10::COMPONENT_MAX),
			uint16_t((i >> 6) & Rgb10::COMPONENT_MAX));
		const Hsv::hsv_fraction_t hsv = Rgb10::ToHsvFraction(color);
		return uint32_t(hsv.hue) ^ (uint32_t(hsv.saturation) << 16) ^ uint32_t(hsv.value);
	});

	Serial.println(F("\tRGB10 ColorHsv"));
	BenchmarkColorConversion([](const uint32_t i) -> uint32_t
	{
		return Rgb10::ColorHsv(angle_t(i), uint8_t(i), uint8_t(i >> 3));
	});

	Serial.println(F("\tRGB10 ColorHsvFraction"));
	BenchmarkColorConversion([](const uint32_t i) -> uint32_t
	{
		const ScalarFraction::ufraction16_t hue = ScalarFraction::ufraction16_t(i & ScalarFraction::UFraction16::FRACTION_1X);
		const ScalarFraction::ufraction16_t saturation = ScalarFraction::ufraction16_t((i >> 1) & ScalarFraction::UFraction16::FRACTION_1X);
		const ScalarFraction::ufraction16_t value = ScalarFraction::ufraction16_t((i >> 2) & ScalarFraction::UFraction16::FRACTION_1X);
		return Rgb10::ColorHsvFraction(hue, saturation, value);
	});

	Serial.println(F("\tRGBFraction16 ToHsv"));
	BenchmarkColorConversion([](const uint32_t i) -> uint32_t
	{
		const RgbFraction16::color_t color = RgbFraction16::Color(
			ScalarFraction::ufraction16_t(i & ScalarFraction::UFraction16::FRACTION_1X),
			ScalarFraction::ufraction16_t((i >> 1) & ScalarFraction::UFraction16::FRACTION_1X),
			ScalarFraction::ufraction16_t((i >> 2) & ScalarFraction::UFraction16::FRACTION_1X));
		const Hsv::hsv_t<ScalarFraction::ufraction16_t> hsv = RgbFraction16::ToHsv(color);
		return uint32_t(hsv.hue) ^ (uint32_t(hsv.saturation) << 16) ^ uint32_t(hsv.value);
	});

	Serial.println(F("\tRGBFraction16 ColorHsv"));
	BenchmarkColorConversion([](const uint32_t i) -> uint32_t
	{
		const RgbFraction16::color_t color = RgbFraction16::ColorHsv(
			angle_t(i), ScalarFraction::ufraction16_t(i & ScalarFraction::UFraction16::FRACTION_1X),
			ScalarFraction::ufraction16_t((i >> 1) & ScalarFraction::UFraction16::FRACTION_1X));
		return uint32_t(color.red) ^ (uint32_t(color.green) << 16) ^ uint32_t(color.blue);
	});

	Serial.println(F("\tRGBFraction16 ColorHsvFraction"));
	BenchmarkColorConversion([](const uint32_t i) -> uint32_t
	{
		const RgbFraction16::color_t color = RgbFraction16::ColorHsvFraction(
			ScalarFraction::ufraction16_t(i & ScalarFraction::UFraction16::FRACTION_1X),
			ScalarFraction::ufraction16_t((i >> 1) & ScalarFraction::UFraction16::FRACTION_1X),
			ScalarFraction::ufraction16_t((i >> 2) & ScalarFraction::UFraction16::FRACTION_1X));
		return uint32_t(color.red) ^ (uint32_t(color.green) << 16) ^ uint32_t(color.blue);
	});
}

#endif
