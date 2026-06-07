#ifndef _INTEGER_SIGNAL_BENCHMARKS_TRIGONOMETRY_h
#define _INTEGER_SIGNAL_BENCHMARKS_TRIGONOMETRY_h

#include "Benchmark.h"

template<uint32_t TestSize = BenchmarkConfig::TrigonometryTestSize>
void BenchmarkSine()
{
	volatile IntegerSignal::FixedPoint::ScalarFraction::fraction8_t testU8 = UINT8_MAX;
	volatile IntegerSignal::FixedPoint::ScalarFraction::fraction16_t testU16 = UINT16_MAX;
	volatile IntegerSignal::FixedPoint::ScalarFraction::fraction32_t testU32 = UINT32_MAX;

	uint32_t DurationTotal = 0;

	Serial.print(F("\t(Sine8)\t\t"));
	DurationTotal = micros();
	for (benchmark_count_t t = 0; t < TestSize; t++)
	{
		for (angle_t i = 0; i < ANGLE_RANGE; i++)
		{
			testU8 = IntegerSignal::Trigonometry::Sine8(i);
		}
	}
	DurationTotal = micros() - DurationTotal;
	PrintAverageNanoseconds(DurationTotal, static_cast<uint64_t>(ANGLE_RANGE) * TestSize);
	Serial.println(F(" ns"));

	Serial.print(F("\t(Sine16)\t"));
	DurationTotal = micros();
	for (benchmark_count_t t = 0; t < TestSize; t++)
	{
		for (angle_t i = 0; i < ANGLE_RANGE; i++)
		{
			testU16 = IntegerSignal::Trigonometry::Sine16(i);
		}
	}
	DurationTotal = micros() - DurationTotal;
	PrintAverageNanoseconds(DurationTotal, static_cast<uint64_t>(ANGLE_RANGE) * TestSize);
	Serial.println(F(" ns"));

	Serial.print(F("\t(Sine32)\t"));
	DurationTotal = micros();
	for (benchmark_count_t t = 0; t < TestSize; t++)
	{
		for (angle_t i = 0; i < ANGLE_RANGE; i++)
		{
			testU32 = IntegerSignal::Trigonometry::Sine32(i);
		}
	}
	DurationTotal = micros() - DurationTotal;
	PrintAverageNanoseconds(DurationTotal, static_cast<uint64_t>(ANGLE_RANGE) * TestSize);
	Serial.println(F(" ns"));
	ConsumeBenchmarkResult(testU8);
	ConsumeBenchmarkResult(testU16);
	ConsumeBenchmarkResult(testU32);


	Serial.println();
}

template<uint32_t TestSize = BenchmarkConfig::TrigonometryTestSize>
void BenchmarkTangent()
{
	volatile ScalarFraction::fraction8_t testU8 = UINT8_MAX;
	volatile ScalarFraction::fraction16_t testU16 = UINT16_MAX;
	volatile ScalarFraction::fraction32_t testU32 = UINT32_MAX;

	constexpr angle_t AngleRange = GetAngle(45 / 2);

	uint32_t DurationTotal = 0;

	Serial.print(F("\t(Tangent8)\t"));
	DurationTotal = micros();
	for (benchmark_count_t t = 0; t < TestSize; t++)
	{
		for (uint16_t i = 0; i < AngleRange; i++)
		{
			testU8 = IntegerSignal::Trigonometry::Tangent8(i);
		}
	}
	DurationTotal = micros() - DurationTotal;
	PrintAverageNanoseconds(DurationTotal, static_cast<uint64_t>(AngleRange) * TestSize);
	Serial.println(F(" ns"));

	Serial.print(F("\t(Tangent16)\t"));
	DurationTotal = micros();
	for (benchmark_count_t t = 0; t < TestSize; t++)
	{
		for (uint16_t i = 0; i < AngleRange; i++)
		{
			testU16 = IntegerSignal::Trigonometry::Tangent16(i);
		}
	}
	DurationTotal = micros() - DurationTotal;
	PrintAverageNanoseconds(DurationTotal, static_cast<uint64_t>(AngleRange) * TestSize);
	Serial.println(F(" ns"));

	Serial.print(F("\t(Tangent32)\t"));
	DurationTotal = micros();
	for (benchmark_count_t t = 0; t < TestSize; t++)
	{
		for (uint16_t i = 0; i < AngleRange; i++)
		{
			testU32 = IntegerSignal::Trigonometry::Tangent32(i);
		}
	}
	DurationTotal = micros() - DurationTotal;
	PrintAverageNanoseconds(DurationTotal, static_cast<uint64_t>(AngleRange) * TestSize);
	Serial.println(F(" ns"));
	ConsumeBenchmarkResult(testU8);
	ConsumeBenchmarkResult(testU16);
	ConsumeBenchmarkResult(testU32);

	Serial.println();
}

#endif

