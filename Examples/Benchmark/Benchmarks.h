// Benchmarks.h

#ifndef _BENCHMARKS_h
#define _BENCHMARKS_h

#include <IntegerSignal.h>
#include <Arduino.h>


template<typename Filter8, typename Filter16, typename Filter32, uint16_t TestSize = 2000>
void BenchmarkFilters()
{
	Filter8 filter8{};
	Filter16 filter16{};
	Filter32 filter32{};

	volatile uint8_t testU8 = UINT8_MAX;
	volatile uint16_t testU16 = UINT16_MAX;
	volatile uint32_t testU32 = UINT32_MAX;

	filter8.Clear();
	filter16.Clear();
	filter32.Clear();

	filter8.Set(UINT8_MAX);
	filter16.Set(UINT16_MAX);
	filter32.Set(UINT32_MAX);

	uint32_t DurationTotal = 0;

	Serial.print(F("\t(8 Bit)\t\t"));
	DurationTotal = micros();
	for (uint16_t i = 0; i < TestSize; i++)
	{
		filter8.Step();
		testU8 = filter8.Get();
	}
	DurationTotal = micros() - DurationTotal;
	Serial.print((DurationTotal * 1000L) / TestSize);
	Serial.println(F(" ns"));

	Serial.print(F("\t(16 Bit)\t"));
	DurationTotal = micros();
	for (uint16_t i = 0; i < TestSize; i++)
	{
		filter16.Step();
		testU16 = filter16.Get();
	}
	DurationTotal = micros() - DurationTotal;
	Serial.print((DurationTotal * 1000L) / TestSize);
	Serial.println(F(" ns"));

	Serial.print(F("\t(32 Bit)\t"));
	DurationTotal = micros();
	for (uint16_t i = 0; i < TestSize; i++)
	{
		filter32.Step();
		testU32 = filter32.Get();
	}
	DurationTotal = micros() - DurationTotal;
	Serial.print((DurationTotal * 1000L) / TestSize);
	Serial.println(F(" ns"));
	Serial.println();
}

template<uint16_t TestSize = 2000>
void BenchmarkInterfaceCurves(IntegerSignal::Curves::ICurve<uint8_t>& curve8, IntegerSignal::Curves::ICurve<uint16_t>& curve16, IntegerSignal::Curves::ICurve<uint32_t>& curve32)
{
	volatile uint8_t testU8 = UINT8_MAX;
	volatile uint16_t testU16 = UINT16_MAX;
	volatile uint32_t testU32 = UINT32_MAX;

	uint32_t DurationTotal = 0;

	Serial.print(F("\t(8 Bit)\t\t"));
	testU8 = UINT8_MAX;
	DurationTotal = micros();
	for (uint16_t i = 0; i < TestSize; i++)
	{
		testU8 = curve8.Get(testU8);
	}
	DurationTotal = micros() - DurationTotal;
	Serial.print((DurationTotal * 1000L) / TestSize);
	Serial.println(F(" ns"));

	Serial.print(F("\t(16 Bit)\t"));
	testU16 = UINT16_MAX;
	DurationTotal = micros();
	for (uint16_t i = 0; i < TestSize; i++)
	{
		testU16 = curve16.Get(testU16);
	}
	DurationTotal = micros() - DurationTotal;
	Serial.print((DurationTotal * 1000L) / TestSize);
	Serial.println(F(" ns"));

	Serial.print(F("\t(32 Bit)\t"));
	testU32 = UINT32_MAX;
	DurationTotal = micros();
	for (uint16_t i = 0; i < TestSize; i++)
	{
		testU32 = curve32.Get(testU32);
	}
	DurationTotal = micros() - DurationTotal;
	Serial.print((DurationTotal * 1000L) / TestSize);
	Serial.println(F(" ns"));
	Serial.println();
}

template<typename Curve8, typename Curve16, typename Curve32, uint16_t TestSize = 2000>
void BenchmarkCurves()
{
	volatile uint8_t testU8 = UINT8_MAX;
	volatile uint16_t testU16 = UINT16_MAX;
	volatile uint32_t testU32 = UINT32_MAX;

	uint32_t DurationTotal = 0;

	Serial.print(F("\t(8 Bit)\t\t"));
	testU8 = UINT8_MAX;
	DurationTotal = micros();
	for (uint16_t i = 0; i < TestSize; i++)
	{
		testU8 = Curve8::Get(testU8);
	}
	DurationTotal = micros() - DurationTotal;
	Serial.print((DurationTotal * 1000L) / TestSize);
	Serial.println(F(" ns"));

	Serial.print(F("\t(16 Bit)\t"));
	testU16 = UINT16_MAX;
	DurationTotal = micros();
	for (uint16_t i = 0; i < TestSize; i++)
	{
		testU16 = Curve16::Get(testU16);
	}
	DurationTotal = micros() - DurationTotal;
	Serial.print((DurationTotal * 1000L) / TestSize);
	Serial.println(F(" ns"));

	Serial.print(F("\t(32 Bit)\t"));
	testU32 = UINT32_MAX;
	DurationTotal = micros();
	for (uint16_t i = 0; i < TestSize; i++)
	{
		testU32 = Curve32::Get(testU32);
	}
	DurationTotal = micros() - DurationTotal;
	Serial.print((DurationTotal * 1000L) / TestSize);
	Serial.println(F(" ns"));
	Serial.println();
}

template<uint16_t TestSize = 2000>
void BenchmarkMix()
{
	volatile uint8_t testU8 = UINT8_MAX;
	volatile uint16_t testU16 = UINT16_MAX;
	volatile uint32_t testU32 = UINT32_MAX;

	uint32_t DurationTotal = 0;

	Serial.println(F("\tLinear 50/50"));
	Serial.print(F("\t(8 Bit)\t\t"));
	testU8 = UINT8_MAX;
	DurationTotal = micros();
	for (uint16_t i = 0; i < TestSize; i++)
	{
		testU8 = IntegerSignal::Mix::LinearU8(testU8, UINT8_MAX);
	}
	DurationTotal = micros() - DurationTotal;
	Serial.print((DurationTotal * 1000L) / TestSize);
	Serial.println(F(" ns"));

	Serial.print(F("\t(16 Bit)\t"));
	testU16 = UINT16_MAX;
	DurationTotal = micros();
	for (uint16_t i = 0; i < TestSize; i++)
	{
		testU16 = IntegerSignal::Mix::LinearU16(testU16, UINT16_MAX);
	}
	DurationTotal = micros() - DurationTotal;
	Serial.print((DurationTotal * 1000L) / TestSize);
	Serial.println(F(" ns"));

	Serial.print(F("\t(32 Bit)\t"));
	testU32 = UINT32_MAX;
	DurationTotal = micros();
	for (uint16_t i = 0; i < TestSize; i++)
	{
		testU32 = IntegerSignal::Mix::LinearU32(testU32, UINT32_MAX);
	}
	DurationTotal = micros() - DurationTotal;
	Serial.print((DurationTotal * 1000L) / TestSize);
	Serial.println(F(" ns"));
	Serial.println();

	Serial.println(F("\tLinear [0;255]"));
	Serial.print(F("\t(8 Bit)\t\t"));
	testU8 = UINT8_MAX;
	DurationTotal = micros();
	for (uint16_t i = 0; i < TestSize; i++)
	{
		testU8 = IntegerSignal::Mix::LinearU8(testU8, UINT8_MAX, (uint8_t)i);
	}
	DurationTotal = micros() - DurationTotal;
	Serial.print((DurationTotal * 1000L) / TestSize);
	Serial.println(F(" ns"));

	Serial.print(F("\t(16 Bit)\t"));
	testU16 = UINT16_MAX;
	DurationTotal = micros();
	for (uint16_t i = 0; i < TestSize; i++)
	{
		testU16 = IntegerSignal::Mix::LinearU16(testU16, UINT16_MAX, (uint8_t)i);
	}
	DurationTotal = micros() - DurationTotal;
	Serial.print((DurationTotal * 1000L) / TestSize);
	Serial.println(F(" ns"));

	Serial.print(F("\t(32 Bit)\t"));
	testU32 = UINT32_MAX;
	DurationTotal = micros();
	for (uint16_t i = 0; i < TestSize; i++)
	{
		testU32 = IntegerSignal::Mix::LinearU32(testU32, UINT32_MAX, (uint8_t)i);
	}
	DurationTotal = micros() - DurationTotal;
	Serial.print((DurationTotal * 1000L) / TestSize);
	Serial.println(F(" ns"));
	Serial.println();


	Serial.println(F("\tPower 50/50"));
	Serial.print(F("\t(8 Bit)\t\t"));
	testU8 = UINT8_MAX;
	DurationTotal = micros();
	for (uint16_t i = 0; i < TestSize; i++)
	{
		testU8 = IntegerSignal::Mix::Power2U8(testU8, UINT8_MAX);
	}
	DurationTotal = micros() - DurationTotal;
	Serial.print((DurationTotal * 1000L) / TestSize);
	Serial.println(F(" ns"));

	Serial.print(F("\t(16 Bit)\t"));
	testU16 = UINT16_MAX;
	DurationTotal = micros();
	for (uint16_t i = 0; i < TestSize; i++)
	{
		testU16 = IntegerSignal::Mix::Power2U16(testU16, UINT16_MAX);
	}
	DurationTotal = micros() - DurationTotal;
	Serial.print((DurationTotal * 1000L) / TestSize);
	Serial.println(F(" ns"));

	Serial.print(F("\t(32 Bit)\t"));
	testU32 = UINT32_MAX;
	DurationTotal = micros();
	for (uint16_t i = 0; i < TestSize; i++)
	{
		testU32 = IntegerSignal::Mix::Power2U32(testU32, UINT32_MAX);
	}
	DurationTotal = micros() - DurationTotal;
	Serial.print((DurationTotal * 1000L) / TestSize);
	Serial.println(F(" ns"));
	Serial.println();
}

template<uint16_t TestSize = 2000>
void BenchmarkSquareRoot()
{
	volatile uint16_t testU16 = UINT16_MAX;
	volatile uint32_t testU32 = UINT32_MAX;
	volatile uint64_t testU64 = UINT64_MAX;

	uint32_t DurationTotal = 0;

	Serial.println(F("\tSquare Root (Root2)"));
	Serial.print(F("\t(16 Bit)\t"));
	DurationTotal = micros();
	for (uint16_t i = 0; i < TestSize; i++)
	{
		testU16 = IntegerSignal::Math::Root2U16(testU16);
	}
	DurationTotal = micros() - DurationTotal;
	Serial.print((DurationTotal * 1000L) / TestSize);
	Serial.println(F(" ns"));

	Serial.print(F("\t(32 Bit)\t"));
	DurationTotal = micros();
	for (uint16_t i = 0; i < TestSize; i++)
	{
		testU32 = IntegerSignal::Math::Root2U32(testU32);
	}
	DurationTotal = micros() - DurationTotal;
	Serial.print((DurationTotal * 1000L) / TestSize);
	Serial.println(F(" ns"));

	Serial.print(F("\t(64 Bit)\t"));
	DurationTotal = micros();
	for (uint16_t i = 0; i < TestSize; i++)
	{
		testU64 = IntegerSignal::Math::Root2U64(testU64);
	}
	DurationTotal = micros() - DurationTotal;
	Serial.print((DurationTotal * 1000L) / TestSize);
	Serial.println(F(" ns"));
	Serial.println();
}

template<uint16_t TestSize = 2000>
void BenchmarkScaleUp()
{
	volatile uint8_t testU8 = UINT8_MAX;
	volatile uint16_t testU16 = UINT16_MAX;
	volatile uint32_t testU32 = UINT32_MAX;
	volatile uint32_t testU64 = UINT64_MAX;

	uint32_t DurationTotal = 0;

	Serial.println(F("\tuint8_t"));
	Serial.print(F("\t(16 Bit)\t"));
	testU8 = UINT8_MAX;
	DurationTotal = micros();
	for (uint16_t i = 0; i < TestSize; i++)
	{
		testU16 = IntegerSignal::Scale::U8ToU16(testU8);
	}
	DurationTotal = micros() - DurationTotal;
	Serial.print((DurationTotal * 1000L) / TestSize);
	Serial.println(F(" ns"));

	Serial.print(F("\t(32 Bit)\t"));
	testU8 = UINT8_MAX;
	DurationTotal = micros();
	for (uint16_t i = 0; i < TestSize; i++)
	{
		testU32 = IntegerSignal::Scale::U8ToU32(testU8);
	}
	DurationTotal = micros() - DurationTotal;
	Serial.print((DurationTotal * 1000L) / TestSize);
	Serial.println(F(" ns"));

	Serial.print(F("\t(64 Bit)\t"));
	testU8 = UINT8_MAX;
	DurationTotal = micros();
	for (uint16_t i = 0; i < TestSize; i++)
	{
		testU64 = IntegerSignal::Scale::U8ToU64(testU8);
	}
	DurationTotal = micros() - DurationTotal;
	Serial.print((DurationTotal * 1000L) / TestSize);
	Serial.println(F(" ns"));

	Serial.println(F("\tuint16_t"));
	Serial.print(F("\t(32 Bit)\t"));
	testU16 = UINT16_MAX;
	DurationTotal = micros();
	for (uint16_t i = 0; i < TestSize; i++)
	{
		testU32 = IntegerSignal::Scale::U16ToU32(testU16);
	}
	DurationTotal = micros() - DurationTotal;
	Serial.print((DurationTotal * 1000L) / TestSize);
	Serial.println(F(" ns"));

	Serial.print(F("\t(64 Bit)\t"));
	testU16 = UINT16_MAX;
	DurationTotal = micros();
	for (uint16_t i = 0; i < TestSize; i++)
	{
		testU64 = IntegerSignal::Scale::U16ToU64(testU16);
	}
	DurationTotal = micros() - DurationTotal;
	Serial.print((DurationTotal * 1000L) / TestSize);
	Serial.println(F(" ns"));

	Serial.println(F("\tuint32_t"));
	Serial.print(F("\t(64 Bit)\t"));
	testU32 = UINT32_MAX;
	DurationTotal = micros();
	for (uint16_t i = 0; i < TestSize; i++)
	{
		testU64 = IntegerSignal::Scale::U32ToU64(testU32);
	}
	DurationTotal = micros() - DurationTotal;
	Serial.print((DurationTotal * 1000L) / TestSize);
	Serial.println(F(" ns"));
}




#endif

