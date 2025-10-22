

#define SERIAL_BAUD_RATE 115200

//#define INTEGER_TRIGONOMETRY_LUT INTEGER_TRIGONOMETRY_LUT_TINY
#define INTEGER_SIGNAL_DISABLE_ACCELERATION
#define EXTENDED_UNIT_TESTS

#include <Arduino.h>
#include <IntegerSignal.h>
#include <IntegerTrigonometry16.h>
#include <IntegerTrigonometry.h>

#include "TypeTraitsTest.h"
#include "OperationTest.h"
#include "BitScaleTest.h"
#include "UIntSizeTest.h"

#include "SquareRootTest.h"

#include "SineTest.h"
#include "TangentTest.h"

#include "FixedPointScaleTest.h"
#include "FixedPointFractionTest.h"

#include "LowPassFilterTest.h"
#include "EmaFilterTest.h"
#include "DemaFilterTest.h"
#include "SignedFilterTest.h"
#include "StrengthFilterTest.h"

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

	pass &= IntegerSignal::TypeTraits::Test::RunTests();
	pass &= IntegerSignal::Operation::Test::RunTests();
	pass &= IntegerSignal::BitScale::Test::RunTests<MaxIterations>();
	pass &= IntegerSignal::UIntSize::Test::RunTests();

	pass &= IntegerSignal::SquareRoot::Test::RunTests<MaxIterations>();

	pass &= IntegerSignal::Trigonometry::Tangent::Test::RunTests<MaxIterations>();
	pass &= IntegerSignal::Trigonometry::Sine::Test::RunTests<MaxIterations>();

	pass &= IntegerSignal::FixedPoint::ScalarFraction::Test::RunTests<MaxIterations>();
	pass &= IntegerSignal::FixedPoint::FactorScale::Test::RunTests<MaxIterations>();

	pass &= IntegerSignal::Filters::LowPass::Test::RunTests<MaxIterations>();
	pass &= IntegerSignal::Filters::Ema::Test::RunTests<MaxIterations>();
	pass &= IntegerSignal::Filters::Dema::Test::RunTests<MaxIterations>();
	pass &= IntegerSignal::Filters::Template::SignedTest::RunTests<MaxIterations>();
	pass &= IntegerSignal::Filters::Template::StrengthTest::RunTests<MaxIterations>();

	if (pass)
	{
		Serial.println();
		Serial.println();
		Serial.println(F("Integer Signal all major tests PASSED."));
		Serial.println();
		Serial.println();

		Serial.flush();
		delay(5000);

#if defined(EXTENDED_UNIT_TESTS)
		Serial.println(F("Starting Long running exhaustive tests, don't wait up."));
		Serial.println();

		pass &= IntegerSignal::BitScale::Test::RunExhaustive();
		pass &= IntegerSignal::SquareRoot::Test::RunExhaustive();
		pass &= IntegerSignal::FixedPoint::ScalarFraction::Test::RunExhaustive();

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
#endif
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


