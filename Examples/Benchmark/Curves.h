#ifndef _INTEGER_SIGNAL_BENCHMARKS_CURVES_h
#define _INTEGER_SIGNAL_BENCHMARKS_CURVES_h

#include "Benchmark.h"

template<uint32_t TestSize = BenchmarkConfig::TestSize>
void BenchmarkInterfaceCurves(IntegerSignal::Curves::ICurve<uint8_t>& curve8, IntegerSignal::Curves::ICurve<uint16_t>& curve16, IntegerSignal::Curves::ICurve<uint32_t>& curve32)
{
	volatile uint8_t testU8 = UINT8_MAX;
	volatile uint16_t testU16 = UINT16_MAX;
	volatile uint32_t testU32 = UINT32_MAX;

	uint32_t DurationTotal = 0;

	Serial.print(F("\t(8 Bit)\t\t"));
	testU8 = UINT8_MAX;
	DurationTotal = micros();
	for (benchmark_count_t i = 0; i < TestSize; i++)
	{
		testU8 = curve8.Get(testU8);
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
		testU16 = curve16.Get(testU16);
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
		testU32 = curve32.Get(testU32);
	}
	DurationTotal = micros() - DurationTotal;
	PrintAverageNanoseconds(DurationTotal, TestSize);
	Serial.print(F(" ns"));
	Serial.println();
	ConsumeBenchmarkResult(testU8);
	ConsumeBenchmarkResult(testU16);
	ConsumeBenchmarkResult(testU32);
	Serial.println();
}

template<typename Curve8, typename Curve16, typename Curve32, uint32_t TestSize = BenchmarkConfig::TestSize>
void BenchmarkCurves()
{
	volatile uint8_t testU8 = UINT8_MAX;
	volatile uint16_t testU16 = UINT16_MAX;
	volatile uint32_t testU32 = UINT32_MAX;

	uint32_t DurationTotal = 0;

	Serial.print(F("\t(8 Bit)\t\t"));
	testU8 = UINT8_MAX;
	DurationTotal = micros();
	for (benchmark_count_t i = 0; i < TestSize; i++)
	{
		testU8 = Curve8::Get(testU8);
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
		testU16 = Curve16::Get(testU16);
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
		testU32 = Curve32::Get(testU32);
	}
	DurationTotal = micros() - DurationTotal;
	PrintAverageNanoseconds(DurationTotal, TestSize);
	Serial.print(F(" ns"));
	Serial.println();
	ConsumeBenchmarkResult(testU8);
	ConsumeBenchmarkResult(testU16);
	ConsumeBenchmarkResult(testU32);
	Serial.println();
}

#endif

