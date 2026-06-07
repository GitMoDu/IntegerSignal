#ifndef _INTEGER_SIGNAL_BENCHMARKS_FILTERS_h
#define _INTEGER_SIGNAL_BENCHMARKS_FILTERS_h

#include "Benchmark.h"

template<typename Filter8, typename Filter16, typename Filter32, uint32_t TestSize = BenchmarkConfig::TestSize>
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
	for (benchmark_count_t i = 0; i < TestSize; i++)
	{
		filter8.Step();
		testU8 = filter8.Get();
	}
	DurationTotal = micros() - DurationTotal;
	PrintAverageNanoseconds(DurationTotal, TestSize);
	Serial.print(F(" ns"));
	Serial.println();

	Serial.print(F("\t(16 Bit)\t"));
	DurationTotal = micros();
	for (benchmark_count_t i = 0; i < TestSize; i++)
	{
		filter16.Step();
		testU16 = filter16.Get();
	}
	DurationTotal = micros() - DurationTotal;
	PrintAverageNanoseconds(DurationTotal, TestSize);
	Serial.print(F(" ns"));
	Serial.println();

	Serial.print(F("\t(32 Bit)\t"));
	DurationTotal = micros();
	for (benchmark_count_t i = 0; i < TestSize; i++)
	{
		filter32.Step();
		testU32 = filter32.Get();
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

