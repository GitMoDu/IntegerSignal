#ifndef _INTEGER_SIGNAL_BENCHMARKS_OPERATION_h
#define _INTEGER_SIGNAL_BENCHMARKS_OPERATION_h

#include "Benchmark.h"

template<uint32_t TestSize = BenchmarkConfig::TestSize>
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
	for (benchmark_count_t i = 0; i < TestSize; i++)
	{
		testU8 = IntegerSignal::Mix::LinearU8(testU8, UINT8_MAX);
	}
	DurationTotal = micros() - DurationTotal;
	PrintAverageNanoseconds(DurationTotal, TestSize);
	Serial.print(F(" ns"));
	Serial.println();

	Serial.print(F("\t(16 Bit)\t"));
	testU16 = UINT16_MAX;
	DurationTotal = micros();
	for (benchmark_count_t i = 0; i < TestSize; i++)
	{
		testU16 = IntegerSignal::Mix::LinearU16(testU16, UINT16_MAX);
	}
	DurationTotal = micros() - DurationTotal;
	PrintAverageNanoseconds(DurationTotal, TestSize);
	Serial.print(F(" ns"));
	Serial.println();

	Serial.print(F("\t(32 Bit)\t"));
	testU32 = UINT32_MAX;
	DurationTotal = micros();
	for (benchmark_count_t i = 0; i < TestSize; i++)
	{
		testU32 = IntegerSignal::Mix::LinearU32(testU32, UINT32_MAX);
	}
	DurationTotal = micros() - DurationTotal;
	PrintAverageNanoseconds(DurationTotal, TestSize);
	Serial.print(F(" ns"));
	Serial.println();
	Serial.println();

	Serial.println(F("\tLinear [0;255]"));
	Serial.print(F("\t(8 Bit)\t\t"));
	testU8 = UINT8_MAX;
	DurationTotal = micros();
	for (benchmark_count_t i = 0; i < TestSize; i++)
	{
		testU8 = IntegerSignal::Mix::LinearU8(testU8, UINT8_MAX, (uint8_t)i);
	}
	DurationTotal = micros() - DurationTotal;
	PrintAverageNanoseconds(DurationTotal, TestSize);
	Serial.print(F(" ns"));
	Serial.println();

	Serial.print(F("\t(16 Bit)\t"));
	testU16 = UINT16_MAX;
	DurationTotal = micros();
	for (benchmark_count_t i = 0; i < TestSize; i++)
	{
		testU16 = IntegerSignal::Mix::LinearU16(testU16, UINT16_MAX, (uint8_t)i);
	}
	DurationTotal = micros() - DurationTotal;
	PrintAverageNanoseconds(DurationTotal, TestSize);
	Serial.print(F(" ns"));
	Serial.println();

	Serial.print(F("\t(32 Bit)\t"));
	testU32 = UINT32_MAX;
	DurationTotal = micros();
	for (benchmark_count_t i = 0; i < TestSize; i++)
	{
		testU32 = IntegerSignal::Mix::LinearU32(testU32, UINT32_MAX, (uint8_t)i);
	}
	DurationTotal = micros() - DurationTotal;
	PrintAverageNanoseconds(DurationTotal, TestSize);
	Serial.print(F(" ns"));
	Serial.println();
	Serial.println();


	Serial.println(F("\tPower 50/50"));
	Serial.print(F("\t(8 Bit)\t\t"));
	testU8 = UINT8_MAX;
	DurationTotal = micros();
	for (benchmark_count_t i = 0; i < TestSize; i++)
	{
		testU8 = IntegerSignal::Mix::Power2U8(testU8, UINT8_MAX);
	}
	DurationTotal = micros() - DurationTotal;
	PrintAverageNanoseconds(DurationTotal, TestSize);
	Serial.print(F(" ns"));
	Serial.println();

	Serial.print(F("\t(16 Bit)\t"));
	testU16 = UINT16_MAX;
	DurationTotal = micros();
	for (benchmark_count_t i = 0; i < TestSize; i++)
	{
		testU16 = IntegerSignal::Mix::Power2U16(testU16, UINT16_MAX);
	}
	DurationTotal = micros() - DurationTotal;
	PrintAverageNanoseconds(DurationTotal, TestSize);
	Serial.print(F(" ns"));
	Serial.println();

	Serial.print(F("\t(32 Bit)\t"));
	testU32 = UINT32_MAX;
	DurationTotal = micros();
	for (benchmark_count_t i = 0; i < TestSize; i++)
	{
		testU32 = IntegerSignal::Mix::Power2U32(testU32, UINT32_MAX);
	}
	DurationTotal = micros() - DurationTotal;
	PrintAverageNanoseconds(DurationTotal, TestSize);
	Serial.print(F(" ns"));
	Serial.println();
	Serial.println();
}

void BenchmarkScaleUp()
{
	const benchmark_count_t testSize = BenchmarkConfig::TestSize;
	volatile uint8_t testU8 = UINT8_MAX;
	volatile uint16_t testU16 = UINT16_MAX;
	volatile uint32_t testU32 = UINT32_MAX;
	volatile uint64_t testU64 = UINT64_MAX;

	uint32_t DurationTotal = 0;

	Serial.println(F("\tuint8_t"));
	Serial.print(F("\t(16 Bit)\t"));
	testU8 = UINT8_MAX;
	DurationTotal = micros();
	for (benchmark_count_t i = 0; i < testSize; i++)
	{
		testU16 = IntegerSignal::U8ToU16(testU8);
	}
	DurationTotal = micros() - DurationTotal;
	PrintAverageNanoseconds(DurationTotal, testSize);
	Serial.println(F(" ns"));

	Serial.print(F("\t(32 Bit)\t"));
	testU8 = UINT8_MAX;
	DurationTotal = micros();
	for (benchmark_count_t i = 0; i < testSize; i++)
	{
		testU32 = IntegerSignal::U8ToU32(testU8);
	}
	DurationTotal = micros() - DurationTotal;
	PrintAverageNanoseconds(DurationTotal, testSize);
	Serial.println(F(" ns"));

	Serial.print(F("\t(64 Bit)\t"));
	testU8 = UINT8_MAX;
	DurationTotal = micros();
	for (benchmark_count_t i = 0; i < testSize; i++)
	{
		testU64 = IntegerSignal::U8ToU64(testU8);
	}
	DurationTotal = micros() - DurationTotal;
	PrintAverageNanoseconds(DurationTotal, testSize);
	Serial.println(F(" ns"));
	Serial.println();

	Serial.println(F("\tuint16_t"));
	Serial.print(F("\t(32 Bit)\t"));
	testU16 = UINT16_MAX;
	DurationTotal = micros();
	for (benchmark_count_t i = 0; i < testSize; i++)
	{
		testU32 = IntegerSignal::U16ToU32(testU16);
	}
	DurationTotal = micros() - DurationTotal;
	PrintAverageNanoseconds(DurationTotal, testSize);
	Serial.println(F(" ns"));

	Serial.print(F("\t(64 Bit)\t"));
	testU16 = UINT16_MAX;
	DurationTotal = micros();
	for (benchmark_count_t i = 0; i < testSize; i++)
	{
		testU64 = IntegerSignal::U16ToU64(testU16);
	}
	DurationTotal = micros() - DurationTotal;
	PrintAverageNanoseconds(DurationTotal, testSize);
	Serial.println(F(" ns"));
	Serial.println();

	Serial.println(F("\tuint32_t"));
	Serial.print(F("\t(64 Bit)\t"));
	testU32 = UINT32_MAX;
	DurationTotal = micros();
	for (benchmark_count_t i = 0; i < testSize; i++)
	{
		testU64 = IntegerSignal::U32ToU64(testU32);
	}
	DurationTotal = micros() - DurationTotal;
	PrintAverageNanoseconds(DurationTotal, testSize);
	Serial.println(F(" ns"));
	ConsumeBenchmarkResult(testU8);
	ConsumeBenchmarkResult(testU16);
	ConsumeBenchmarkResult(testU32);
	ConsumeBenchmarkResult(testU64);
}

void BenchmarkSquareRoot()
{
	const benchmark_count_t testSize = BenchmarkConfig::TestSize;
	volatile uint16_t testU16 = UINT16_MAX;
	volatile uint32_t testU32 = UINT32_MAX;
	volatile uint64_t testU64 = UINT64_MAX;

	uint32_t DurationTotal = 0;

	Serial.println(F("\tSquare Root (Root^2)"));
	Serial.print(F("\t(16 Bit)\t"));
	DurationTotal = micros();
	for (benchmark_count_t i = 0; i < testSize; i++)
	{
		testU16 = IntegerSignal::SquareRoot16(testU16);
	}
	DurationTotal = micros() - DurationTotal;
	PrintAverageNanoseconds(DurationTotal, testSize);
	Serial.println(F(" ns"));

	Serial.print(F("\t(32 Bit)\t"));
	DurationTotal = micros();
	for (benchmark_count_t i = 0; i < testSize; i++)
	{
		testU32 = IntegerSignal::SquareRoot32(testU32);
	}
	DurationTotal = micros() - DurationTotal;
	PrintAverageNanoseconds(DurationTotal, testSize);
	Serial.println(F(" ns"));

	Serial.print(F("\t(64 Bit)\t"));
	DurationTotal = micros();
	for (benchmark_count_t i = 0; i < testSize; i++)
	{
		testU64 = IntegerSignal::SquareRoot64(testU64);
	}
	DurationTotal = micros() - DurationTotal;
	PrintAverageNanoseconds(DurationTotal, testSize);
	Serial.println(F(" ns"));
	ConsumeBenchmarkResult(testU16);
	ConsumeBenchmarkResult(testU32);
	ConsumeBenchmarkResult(testU64);
	Serial.println();
}

#endif

