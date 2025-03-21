
#define SERIAL_BAUD_RATE 115200

//#define INTEGER_TRIGONOMETRY_LUT INTEGER_TRIGONOMETRY_LUT_TINY
#define INTEGER_SIGNAL_DISABLE_ACCELERATION

#include <Arduino.h>
#include <IntegerSignal.h>
#include <IntegerTrigonometry16.h>
#include <IntegerTrigonometry.h>

#include "SquareRootTest.h"
#include "BitScaleTest.h"
#include "ResizeTest.h"
#include "FractionTest.h"
#include "SineTest.h"
#include "TangentTest.h"

inline void PrintPlaform();

void setup()
{
	Serial.begin(SERIAL_BAUD_RATE);
	while (!Serial)
		;
	delay(1000);

	Serial.println();
	Serial.println();
	Serial.println(F("Integer Signal Unit Testing"));
	Serial.print('\t');
	PrintPlaform();
	Serial.println();
	Serial.println();


	static constexpr uint32_t MaxIterations = 1000;

	bool pass = true;


	Serial.flush();
	//pass &= IntegerSignal::Trigonometry::Sine::Test::RunTests<MaxIterations>();
	//delay(5000000);

	pass &= IntegerSignal::Trigonometry::Tangent::Test::RunTests<MaxIterations>();
	pass &= IntegerSignal::Trigonometry::Sine::Test::RunTests<MaxIterations>();
	pass &= IntegerSignal::Fraction::Test::RunTests<MaxIterations>();
	pass &= IntegerSignal::BitScale::Test::RunTests<MaxIterations>();
	pass &= IntegerSignal::Resize::Test::RunTests<MaxIterations>();
	pass &= IntegerSignal::SquareRoot::Test::RunTests<MaxIterations>();

	if (pass)
	{
		Serial.println();
		Serial.println();
		Serial.println(F("Integer Signal all major tests PASSED."));
		Serial.println();
		Serial.println();

		Serial.flush();
		delay(5000);

		Serial.println(F("Starting Long running exhaustive tests, don't wait up."));
		Serial.println();

		pass &= IntegerSignal::BitScale::Test::RunExhaustive();
		pass &= IntegerSignal::SquareRoot::Test::RunExhaustive();
		pass &= IntegerSignal::Fraction::Test::RunExhaustive();

		if (pass)
		{
			Serial.println();
			Serial.println();
			Serial.println(F("Integer Signal all Long running exhaustive tests PASSED."));
			Serial.println();
			Serial.println();
		}
		else
		{
			Serial.println();
			Serial.println();
			Serial.println(F("Integer Signal long running tests FAILED."));
			Serial.println();
		}
	}
	else
	{
		Serial.println();
		Serial.println();
		Serial.println(F("Integer Signal major tests FAILED."));
		Serial.println();
		Serial.println();
	}

}

void loop()
{
}


void PrintPlaform()
{
#if defined(ARDUINO_ARCH_AVR)
	Serial.println(F("AVR"));
#elif defined(ARDUINO_ARCH_STM32F1) || defined(ARDUINO_ARCH_STM32)
	Serial.println(F("STM32 F1"));
#elif defined(ARDUINO_ARCH_STM32F4)
	Serial.println(F("STM32 F4"));
#elif defined(ARDUINO_ARCH_RP2040)
#if defined(PICO_RP2350)
	Serial.println(F("RP2350"));
#else
	Serial.println(F("RP2040"));
#endif
#elif defined(ARDUINO_ARCH_NRF52)
	Serial.println(F("NRF52840"));
#elif defined(ARDUINO_ARCH_ESP32)
	Serial.println(F("ESP32"));
#endif
}


