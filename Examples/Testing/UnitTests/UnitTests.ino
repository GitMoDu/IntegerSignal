
#define SERIAL_BAUD_RATE 115200


#define INTEGER_SIGNAL_DISABLE_ACCELERATION

#include <Arduino.h>
#include <IntegerSignal.h>

#include "SquareRootTest.h"



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

	IntegerSignal::Testing::SquareRoot::RunTests<1000000>();

	Serial.println(F("Integer Signal Major tests complete."));
	Serial.println();
	Serial.println(F("Starting Long running exhaustive tests, don't wait up."));
	Serial.println();

	IntegerSignal::Testing::SquareRoot::RunExhaustive();
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


