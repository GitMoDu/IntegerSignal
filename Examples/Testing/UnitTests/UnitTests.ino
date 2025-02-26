
#define SERIAL_BAUD_RATE 115200


#define INTEGER_SIGNAL_DISABLE_ACCELERATION

#include <Arduino.h>
#include <IntegerSignal.h>

#include "SquareRootTest.h"
#include "BitScaleTest.h"



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

	bool pass = true;
	pass &= IntegerSignal::BitScale::Test::RunTests<1000000>();
	pass &= IntegerSignal::SquareRoot::Test::RunTests<1000000>();

	if (pass)
	{
		Serial.println();
		Serial.println();
		Serial.println(F("Integer Signal all major tests PASSED."));
		Serial.println();
		Serial.println();
		Serial.println(F("Starting Long running exhaustive tests, don't wait up."));
		Serial.println();

		pass &= IntegerSignal::SquareRoot::Test::RunExhaustive();
		//pass &= IntegerSignal::BitScale::Test::RunExhaustive();

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


