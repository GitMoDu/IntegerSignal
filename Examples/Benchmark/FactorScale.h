#ifndef _INTEGER_SIGNAL_BENCHMARKS_FACTOR_SCALE_h
#define _INTEGER_SIGNAL_BENCHMARKS_FACTOR_SCALE_h

#include "Benchmark.h"

template<uint32_t TestSize = BenchmarkConfig::TestSize>
void BenchmarkFactorScale()
{
#if defined(__AVR__) || (INTPTR_MAX == INT32_MAX)
	typedef BenchmarkScalarGetPath::RuntimeTag selected_uq4_4_path_t;
#else
	typedef BenchmarkScalarGetPath::ConstexprTag selected_uq4_4_path_t;
#endif
#if defined(__AVR__)
	typedef BenchmarkScalarGetPath::RuntimeTag selected_uq8_8_path_t;
#else
	typedef BenchmarkScalarGetPath::ConstexprTag selected_uq8_8_path_t;
#endif
#if (INTPTR_MAX == INT32_MAX)
	typedef BenchmarkScalarGetPath::RuntimeTag selected_uq16_16_path_t;
#else
	typedef BenchmarkScalarGetPath::ConstexprTag selected_uq16_16_path_t;
#endif

	volatile uint8_t testU8 = 0;
	volatile uint16_t testU16 = 0;
	volatile uint32_t  testU32 = 0;
	const uint8_t getFactorDenominatorU8 = static_cast<uint8_t>(MinValue<uint32_t>(TestSize, UINT8_MAX));
	const uint16_t getFactorDenominatorU16 = static_cast<uint16_t>(MinValue<uint32_t>(TestSize, UINT16_MAX));
	const uint32_t getFactorDenominatorU32 = MaxValue<uint32_t>(1u, MinValue<uint32_t>(TestSize, UINT16_MAX));

	uint32_t DurationTotal = 0;

	Serial.println(F("\tUQ4.4 GetFactor"));
	Serial.print(F("\t(Constexpr)\t"));
	DurationTotal = micros();
	for (uint8_t i = 0; i < UINT8_MAX; i++)
	{
		testU8 = FactorScale::Factor::Constexpr::GetFactor8<uint8_t>(i, getFactorDenominatorU8);
	}
	DurationTotal = micros() - DurationTotal;
	PrintAverageNanoseconds(DurationTotal, UINT8_MAX);
	Serial.print(F(" ns"));
	BenchmarkScalarGetPath::PrintSelectedMarker<selected_uq4_4_path_t, BenchmarkScalarGetPath::ConstexprTag>();
	Serial.println();

	Serial.print(F("\t(Runtime)\t"));
	DurationTotal = micros();
	for (uint8_t i = 0; i < UINT8_MAX; i++)
	{
		testU8 = FactorScale::Factor::Runtime::GetFactor8<uint8_t>(i, getFactorDenominatorU8);
	}
	DurationTotal = micros() - DurationTotal;
	PrintAverageNanoseconds(DurationTotal, UINT8_MAX);
	Serial.print(F(" ns"));
	BenchmarkScalarGetPath::PrintSelectedMarker<selected_uq4_4_path_t, BenchmarkScalarGetPath::RuntimeTag>();
	Serial.println();

	Serial.println(F("\tUQ8.8 GetFactor"));
	Serial.print(F("\t(Constexpr)\t"));
	DurationTotal = micros();
	for (benchmark_count_t i = 0; i < TestSize; i++)
	{
		testU16 = FactorScale::Factor::Constexpr::GetFactor16<uint16_t>(static_cast<uint16_t>(i), getFactorDenominatorU16);
	}
	DurationTotal = micros() - DurationTotal;
	PrintAverageNanoseconds(DurationTotal, TestSize);
	Serial.print(F(" ns"));
	BenchmarkScalarGetPath::PrintSelectedMarker<selected_uq8_8_path_t, BenchmarkScalarGetPath::ConstexprTag>();
	Serial.println();

	Serial.print(F("\t(Runtime)\t"));
	DurationTotal = micros();
	for (benchmark_count_t i = 0; i < TestSize; i++)
	{
		testU16 = FactorScale::Factor::Runtime::GetFactor16<uint16_t>(static_cast<uint16_t>(i), getFactorDenominatorU16);
	}
	DurationTotal = micros() - DurationTotal;
	PrintAverageNanoseconds(DurationTotal, TestSize);
	Serial.print(F(" ns"));
	BenchmarkScalarGetPath::PrintSelectedMarker<selected_uq8_8_path_t, BenchmarkScalarGetPath::RuntimeTag>();
	Serial.println();

	Serial.println(F("\tUQ16.16 GetFactor"));
	Serial.print(F("\t(Constexpr)\t"));
	uint32_t numeratorU32 = 0;
	DurationTotal = micros();
	for (benchmark_count_t i = 0; i < TestSize; i++)
	{
		testU32 = FactorScale::Factor::Constexpr::GetFactor32<uint32_t>(numeratorU32, getFactorDenominatorU32);
		numeratorU32++;
		if (numeratorU32 >= getFactorDenominatorU32)
			numeratorU32 = 0;
	}
	DurationTotal = micros() - DurationTotal;
	PrintAverageNanoseconds(DurationTotal, TestSize);
	Serial.print(F(" ns"));
	BenchmarkScalarGetPath::PrintSelectedMarker<selected_uq16_16_path_t, BenchmarkScalarGetPath::ConstexprTag>();
	Serial.println();

	Serial.print(F("\t(Runtime)\t"));
	numeratorU32 = 0;
	DurationTotal = micros();
	for (benchmark_count_t i = 0; i < TestSize; i++)
	{
		testU32 = FactorScale::Factor::Runtime::GetFactor32<uint32_t>(numeratorU32, getFactorDenominatorU32);
		numeratorU32++;
		if (numeratorU32 >= getFactorDenominatorU32)
			numeratorU32 = 0;
	}
	DurationTotal = micros() - DurationTotal;
	PrintAverageNanoseconds(DurationTotal, TestSize);
	Serial.print(F(" ns"));
	BenchmarkScalarGetPath::PrintSelectedMarker<selected_uq16_16_path_t, BenchmarkScalarGetPath::RuntimeTag>();
	Serial.println();

	Serial.print(F("\tUQ4.4 Apply\t"));
	DurationTotal = micros();
	for (FactorScale::scale8_t i = 0; i <= FactorScale::SCALE8_1X; i++)
	{
		testU8 = FactorScale::Scale(i, i);
	}
	DurationTotal = micros() - DurationTotal;
	PrintAverageNanoseconds(DurationTotal, FactorScale::SCALE8_1X);
	Serial.println(F(" ns"));

	Serial.print(F("\tUQ8.8 Apply\t"));
	DurationTotal = micros();
	for (uint32_t i = 0; i < ((uint32_t)TestSize * UINT8_MAX); i += UINT8_MAX)
	{
		testU16 = FactorScale::Scale(FactorScale::scale16_t(i), (uint16_t)i);
	}
	DurationTotal = micros() - DurationTotal;
	PrintAverageNanoseconds(DurationTotal, TestSize);
	Serial.println(F(" ns"));

	Serial.print(F("\tUQ16.16 Apply\t"));
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

