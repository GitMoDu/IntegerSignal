
#define SERIAL_BAUD_RATE 115200

#include <Arduino.h>
#include <IntegerSignal.h>
#include "Benchmarks.h"

using namespace IntegerSignal;

static constexpr uint16_t TestSize = 5000;


void setup()
{
	Serial.begin(SERIAL_BAUD_RATE);
	while (!Serial)
		;
	delay(1000);

	Serial.println();
	Serial.println();
	Serial.println(F("Integer Signals Benchmark"));
	Serial.print('\t');
	PrintPlaform();
	Serial.println();
	Serial.println();

	Serial.println(F("Average Call Duration (ns)"));
	Serial.println();

	Serial.println(F("Sine"));
	BenchmarkSine<(TestSize / 200) | 3>();
	Serial.println();

	Serial.println(F("Tangent"));
	BenchmarkTangent<(TestSize / 200) | 3>();
	Serial.println();

	Serial.println(F("Resize"));
	BenchmarkResize<(TestSize / 200) | 3>();
	Serial.println();

	Serial.println(F("UFraction"));
	BenchmarkUFraction<(TestSize / 200) | 3>();
	Serial.println();

	Serial.println(F("Fraction"));
	BenchmarkUFraction<(TestSize / 200) | 3>();
	Serial.println();

	Serial.println(F("Mix"));
	BenchmarkMix();
	Serial.println();

	Serial.println(F("Scale Up"));
	BenchmarkScaleUp();
	Serial.println();

	Serial.println(F("Square Root"));
	BenchmarkSquareRoot();
	Serial.println();

	Serial.println(F("Filters"));
	Serial.println(F("\tVirtual Interface"));
	BenchmarkFilters<Filter::IFilter<uint8_t>, Filter::IFilter<uint16_t>, Filter::IFilter<uint32_t>, TestSize>();
	Serial.println(F("\tLowPass"));
	BenchmarkFilters<Filters::LowPassU8<>, Filters::LowPassU16<>, Filters::LowPassU32<>, TestSize>();
	Serial.println(F("\tEMA"));
	BenchmarkFilters<Filters::EmaU8<>, Filters::EmaU16<>, Filters::EmaU32<>, TestSize>();
	Serial.println(F("\tDEMA"));
	BenchmarkFilters<Filters::DemaU8<>, Filters::DemaU16<>, Filters::DemaU32<>, TestSize>();
	Serial.println();

	Serial.println(F("Curves"));
	Serial.println(F("\tVirtual Interface"));
	Curves::Template::Interface<Curves::Template::Curve<uint8_t>, uint8_t> iCurve8{};
	Curves::Template::Interface<Curves::Template::Curve<uint16_t>, uint16_t> iCurve16{};
	Curves::Template::Interface<Curves::Template::Curve<uint32_t>, uint32_t> iCurve32{};
	BenchmarkInterfaceCurves<TestSize>(iCurve8, iCurve16, iCurve32);
	Serial.println(F("\tSigned"));
	BenchmarkCurves<Curves::Template::SignedI8<Curves::Template::Curve<uint8_t>>, Curves::Template::SignedI16<Curves::Template::Curve<uint16_t>>, Curves::Template::SignedI32<Curves::Template::Curve<uint32_t>>, TestSize>();
	Serial.println(F("\tLimit"));
	BenchmarkCurves<Curves::LimitU8<0, INT8_MAX>, Curves::LimitU16<0, INT16_MAX>, Curves::LimitU32<0, INT32_MAX>, TestSize>();
	Serial.println(F("\tScaleDown"));
	BenchmarkCurves<Curves::ScaleDownU8<INT8_MAX>, Curves::ScaleDownU16<INT8_MAX>, Curves::ScaleDownU32<INT8_MAX>, TestSize>();
	Serial.println(F("\tPower2"));
	BenchmarkCurves<Curves::Power2U8<>, Curves::Power2U16<>, Curves::Power2U32<>, TestSize>();
	Serial.println(F("\tPower3"));
	BenchmarkCurves<Curves::Power3U8<>, Curves::Power3U16<>, Curves::Power3U32<>, TestSize>();
	Serial.println(F("\tRoot2"));
	BenchmarkCurves<Curves::Root2U8<>, Curves::Root2U16<>, Curves::Root2U32<>, TestSize>();

	Serial.println(F("\tPower2+Limit")); // Chained curves.
	BenchmarkCurves<Curves::LimitU8<0, INT8_MAX, Curves::Power2U8<>>, Curves::LimitU16<0, INT16_MAX, Curves::Power2U16<>>, Curves::LimitU32<0, INT32_MAX, Curves::Power2U32<>>, TestSize>();
	Serial.println();

}

void loop()
{
}


void PrintPlaform()
{
#if defined(ARDUINO_ARCH_AVR)
	Serial.print(F("AVR"));
#elif defined(ARDUINO_ARCH_STM32F1) || defined(ARDUINO_ARCH_STM32)
	Serial.print(F("STM32 F1"));
#elif defined(ARDUINO_ARCH_STM32F4)
	Serial.print(F("STM32 F4"));
#elif defined(ARDUINO_ARCH_RP2040)
#if defined(PICO_RP2350)
	Serial.print(F("RP2350"));
#else
	Serial.print(F("RP2040"));
#endif
#elif defined(ARDUINO_ARCH_NRF52)
	Serial.print(F("NRF52840"));
#elif defined(ARDUINO_ARCH_ESP32)
	Serial.print(F("ESP32"));
#endif

	Serial.print(F(" @ "));
#if defined(ARDUINO_ARCH_STM32F4)
	Serial.print(CLOCK_SPEED_MHZ);
#else
	Serial.print(F_CPU / 1000000);
#endif
	Serial.println(F(" MHz"));
}
