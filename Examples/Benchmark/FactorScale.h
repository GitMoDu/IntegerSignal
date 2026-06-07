#ifndef _INTEGER_SIGNAL_BENCHMARKS_FACTOR_SCALE_h
#define _INTEGER_SIGNAL_BENCHMARKS_FACTOR_SCALE_h

#include "Benchmark.h"

template<uint32_t TestSize = BenchmarkConfig::TestSize>
void BenchmarkResize()
{
	volatile uint8_t testU8 = 0;
	volatile uint16_t testU16 = 0;
	volatile uint32_t  testU32 = 0;

	uint32_t DurationTotal = 0;

	Serial.println(F("\tGet"));
	Serial.print(F("\t(8 Bit)\t\t"));
	DurationTotal = micros();
	for (uint8_t i = 0; i < UINT8_MAX; i++)
	{
		testU8 = FactorScale::GetFactor8<uint8_t>(i, static_cast<uint8_t>(MinValue<uint32_t>(TestSize, UINT8_MAX)));
	}
	DurationTotal = micros() - DurationTotal;
	PrintAverageNanoseconds(DurationTotal, UINT8_MAX);
	Serial.println(F(" ns"));

	Serial.print(F("\t(16 Bit)\t"));
	DurationTotal = micros();
	for (benchmark_count_t i = 0; i < TestSize; i++)
	{
		testU16 = FactorScale::GetFactor16<uint16_t>(static_cast<uint16_t>(i), static_cast<uint16_t>(MinValue<uint32_t>(TestSize, UINT16_MAX)));
	}
	DurationTotal = micros() - DurationTotal;
	PrintAverageNanoseconds(DurationTotal, TestSize);
	Serial.println(F(" ns"));

	Serial.print(F("\t(32 Bit)\t"));
	DurationTotal = micros();
	for (benchmark_count_t i = 0; i < TestSize; i++)
	{
		testU32 = FactorScale::GetFactor32<uint32_t>(i, TestSize);
	}
	DurationTotal = micros() - DurationTotal;
	PrintAverageNanoseconds(DurationTotal, TestSize);
	Serial.println(F(" ns"));

	Serial.println(F("\tScale"));
	Serial.print(F("\t(8 Bit)\t\t"));
	DurationTotal = micros();
	for (FactorScale::scale8_t i = 0; i <= FactorScale::SCALE8_1X; i++)
	{
		testU8 = FactorScale::Scale(i, i);
	}
	DurationTotal = micros() - DurationTotal;
	PrintAverageNanoseconds(DurationTotal, FactorScale::SCALE8_1X);
	Serial.println(F(" ns"));

	Serial.print(F("\t(16 Bit)\t"));
	DurationTotal = micros();
	for (uint32_t i = 0; i < ((uint32_t)TestSize * UINT8_MAX); i += UINT8_MAX)
	{
		testU16 = FactorScale::Scale(FactorScale::scale16_t(i), (uint16_t)i);
	}
	DurationTotal = micros() - DurationTotal;
	PrintAverageNanoseconds(DurationTotal, TestSize);
	Serial.println(F(" ns"));

	Serial.print(F("\t(32 Bit)\t"));
	DurationTotal = micros();
	for (uint32_t i = 0; i < ((uint32_t)TestSize * UINT16_MAX); i += UINT16_MAX)
	{
		testU32 = FactorScale::Scale(FactorScale::scale32_t(i), i);
	}
	DurationTotal = micros() - DurationTotal;
	PrintAverageNanoseconds(DurationTotal, TestSize);
	Serial.println(F(" ns"));
	ConsumeBenchmarkResult(testU8);
	ConsumeBenchmarkResult(testU16);
	ConsumeBenchmarkResult(testU32);
	Serial.println();
}

#endif

