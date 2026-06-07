
#define SERIAL_BAUD_RATE 115200

#include <Arduino.h>
#include <IntegerSignal.h>

#include "Filters.h"
#include "Curves.h"
#include "Operation.h"
#include "ScalarFraction.h"
#include "FactorScale.h"
#include "Trigonometry.h"

using namespace IntegerSignal;

inline void PrintPlaform();

void setup()
{
	Serial.begin(SERIAL_BAUD_RATE);
	while (!Serial)
		;
	delay(1000);

	Serial.println();
	Serial.println();
	Serial.println(F("Integer Signal Benchmark"));
	Serial.print('\t');
	PrintPlaform();
	Serial.println();
	Serial.println();

	Serial.println(F("Average Call Duration (ns)"));
	Serial.println();

	Serial.println(F("FactorScale"));
	BenchmarkFactorScale<>();
	Serial.println();

	Serial.println(F("ScalarFraction"));
	BenchmarkUFraction<>();
	BenchmarkFraction<>();
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

	Serial.println(F("Sine"));
	BenchmarkSine<>();
	Serial.println();

	Serial.println(F("Tangent"));
	BenchmarkTangent<>();
	Serial.println();

	Serial.println(F("Filters"));
	Serial.println(F("\tVirtual Interface"));
	BenchmarkFilters<Filter::IFilter<uint8_t>, Filter::IFilter<uint16_t>, Filter::IFilter<uint32_t>>();
	Serial.println(F("\tLowPass"));
	BenchmarkFilters<Filters::LowPassU8<>, Filters::LowPassU16<>, Filters::LowPassU32<>>();
	Serial.println(F("\tEMA"));
	BenchmarkFilters<Filters::EmaU8<>, Filters::EmaU16<>, Filters::EmaU32<>>();
	Serial.println(F("\tDEMA"));
	BenchmarkFilters<Filters::DemaU8<>, Filters::DemaU16<>, Filters::DemaU32<>>();
	Serial.println();

	Serial.println(F("Curves"));
	Serial.println(F("\tVirtual Interface"));
	Curves::Template::Interface<Curves::Template::Curve<uint8_t>, uint8_t> iCurve8{};
	Curves::Template::Interface<Curves::Template::Curve<uint16_t>, uint16_t> iCurve16{};
	Curves::Template::Interface<Curves::Template::Curve<uint32_t>, uint32_t> iCurve32{};
	BenchmarkInterfaceCurves<>(iCurve8, iCurve16, iCurve32);
	Serial.println(F("\tSigned"));
	BenchmarkCurves<Curves::Template::SignedI8<Curves::Template::Curve<uint8_t>>, Curves::Template::SignedI16<Curves::Template::Curve<uint16_t>>, Curves::Template::SignedI32<Curves::Template::Curve<uint32_t>>>();
	Serial.println(F("\tLimit"));
	BenchmarkCurves<Curves::LimitU8<0, INT8_MAX>, Curves::LimitU16<0, INT16_MAX>, Curves::LimitU32<0, INT32_MAX>>();
	Serial.println(F("\tScaleDown"));
	BenchmarkCurves<Curves::ScaleDownU8<INT8_MAX>, Curves::ScaleDownU16<INT8_MAX>, Curves::ScaleDownU32<INT8_MAX>>();
	Serial.println(F("\tPower2"));
	BenchmarkCurves<Curves::Power2U8<>, Curves::Power2U16<>, Curves::Power2U32<>>();
	Serial.println(F("\tPower3"));
	BenchmarkCurves<Curves::Power3U8<>, Curves::Power3U16<>, Curves::Power3U32<>>();
	Serial.println(F("\tRoot2"));
	BenchmarkCurves<Curves::Root2U8<>, Curves::Root2U16<>, Curves::Root2U32<>>();

	Serial.println(F("\tPower2+Limit")); // Chained curves.
	BenchmarkCurves<Curves::LimitU8<0, INT8_MAX, Curves::Power2U8<>>, Curves::LimitU16<0, INT16_MAX, Curves::Power2U16<>>, Curves::LimitU32<0, INT32_MAX, Curves::Power2U32<>>>();
	Serial.println();

}

void loop()
{}


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

	Serial.print(F(" ("));
#if defined(__AVR__)
	Serial.print(F("AVR"));
#elif defined(ARDUINO_ARCH_ESP32)
	Serial.print(F("ESP32 "));
#if defined(__riscv)
	Serial.print(F("RISC-V"));
#elif defined(__XTENSA__)
	Serial.print(F("Xtensa"));
#else
	Serial.print(F("generic"));
#endif
#elif defined(__riscv)
	Serial.print(F("RISC-V"));
#elif defined(__aarch64__) || defined(__arm__) || defined(__thumb__) || defined(__ARM_ARCH)
	Serial.print(F("ARM"));
#else
	Serial.print(F("generic"));
#endif
	Serial.print(F(")"));

	Serial.print(F(" @ "));
#if defined(ARDUINO_ARCH_STM32F4)
	Serial.print(CLOCK_SPEED_MHZ);
#else
	Serial.print(F_CPU / 1000000);
#endif
	Serial.println(F(" MHz"));
}
