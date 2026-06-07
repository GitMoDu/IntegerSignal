#ifndef _INTEGER_SIGNAL_BENCHMARKS_SCALAR_FRACTION_h
#define _INTEGER_SIGNAL_BENCHMARKS_SCALAR_FRACTION_h

#include "Benchmark.h"

template<uint32_t TestSize = BenchmarkConfig::TestSize>
void BenchmarkUFraction()
{
	typedef typename BenchmarkScalarGetPath::UnsignedSelected<uint8_t>::type selected_u8_path_t;
	typedef typename BenchmarkScalarGetPath::UnsignedSelected<uint16_t>::type selected_u16_path_t;
	typedef typename BenchmarkScalarGetPath::UnsignedSelected<uint32_t>::type selected_u32_path_t;

	volatile uint8_t testU8 = 0;
	volatile uint16_t testU16 = 0;
	volatile uint32_t  testU32 = 0;
	volatile uint32_t testSizeU8Source = MaxValue<uint32_t>(1u, MinValue<uint32_t>(TestSize, UINT8_MAX));
	volatile uint32_t testSizeU16Source = MaxValue<uint32_t>(1u, MinValue<uint32_t>(TestSize, UINT16_MAX));
	volatile uint32_t testSizeU32Source = MaxValue<uint32_t>(1u, MinValue<uint32_t>(TestSize, 0xFFFFu));

	const uint8_t testSizeU8 = static_cast<uint8_t>(testSizeU8Source);
	const uint16_t testSizeU16 = static_cast<uint16_t>(testSizeU16Source);
	const uint32_t testSizeU32 = static_cast<uint32_t>(testSizeU32Source);
	const ufraction8_t applyScalarU8 = ufraction8_t(testSizeU8 >> 1);
	const ufraction16_t applyScalarU16 = ufraction16_t(uint16_t(testSizeU16 << 3));
	const ufraction32_t applyScalarU32 = ufraction32_t(testSizeU32 << 16);
	const uint8_t applyValueU8 = testSizeU8;
	const uint16_t applyValueU16 = uint16_t(UINT16_MAX - testSizeU16);
	const uint32_t applyValueU32 = uint32_t(UINT32_MAX - testSizeU32);

	uint32_t DurationTotal = 0;

	Serial.println(F("\tGet"));
	Serial.print(F("\t(8 Bit Constexpr)\t"));
	uint8_t numeratorU8 = 0;
	DurationTotal = micros();
	for (benchmark_count_t i = 0; i < TestSize; i++)
	{
		testU8 = ScalarFraction::Scalar::Constexpr::GetScalarU8(numeratorU8, testSizeU8);
		numeratorU8++;
		if (numeratorU8 >= testSizeU8)
			numeratorU8 = 0;
	}
	DurationTotal = micros() - DurationTotal;
	PrintAverageNanoseconds(DurationTotal, TestSize);
	Serial.print(F(" ns"));
	BenchmarkScalarGetPath::PrintSelectedMarker<selected_u8_path_t, BenchmarkScalarGetPath::ConstexprTag>();
	Serial.println();

	Serial.print(F("\t(8 Bit Runtime)\t\t"));
	numeratorU8 = 0;
	DurationTotal = micros();
	for (benchmark_count_t i = 0; i < TestSize; i++)
	{
		testU8 = ScalarFraction::Scalar::Fast::GetScalarU8<uint8_t>(numeratorU8, testSizeU8);
		numeratorU8++;
		if (numeratorU8 >= testSizeU8)
			numeratorU8 = 0;
	}
	DurationTotal = micros() - DurationTotal;
	PrintAverageNanoseconds(DurationTotal, TestSize);
	Serial.print(F(" ns"));
	BenchmarkScalarGetPath::PrintSelectedMarker<selected_u8_path_t, BenchmarkScalarGetPath::RuntimeTag>();
	Serial.println();

	Serial.print(F("\t(16 Bit Constexpr)\t"));
	uint16_t numeratorU16 = 0;
	DurationTotal = micros();
	for (benchmark_count_t i = 0; i < TestSize; i++)
	{
		testU16 = ScalarFraction::Scalar::Constexpr::GetScalarU16<uint16_t>(numeratorU16, testSizeU16);
		numeratorU16++;
		if (numeratorU16 >= testSizeU16)
			numeratorU16 = 0;
	}
	DurationTotal = micros() - DurationTotal;
	PrintAverageNanoseconds(DurationTotal, TestSize);
	Serial.print(F(" ns"));
	BenchmarkScalarGetPath::PrintSelectedMarker<selected_u16_path_t, BenchmarkScalarGetPath::ConstexprTag>();
	Serial.println();

	Serial.print(F("\t(16 Bit Runtime)\t"));
	numeratorU16 = 0;
	DurationTotal = micros();
	for (benchmark_count_t i = 0; i < TestSize; i++)
	{
		testU16 = ScalarFraction::Scalar::Fast::GetScalarU16<uint16_t>(numeratorU16, testSizeU16);
		numeratorU16++;
		if (numeratorU16 >= testSizeU16)
			numeratorU16 = 0;
	}
	DurationTotal = micros() - DurationTotal;
	PrintAverageNanoseconds(DurationTotal, TestSize);
	Serial.print(F(" ns"));
	BenchmarkScalarGetPath::PrintSelectedMarker<selected_u16_path_t, BenchmarkScalarGetPath::RuntimeTag>();
	Serial.println();

	Serial.print(F("\t(32 Bit Constexpr)\t"));
	uint32_t numeratorU32 = 0;
	DurationTotal = micros();
	for (benchmark_count_t i = 0; i < TestSize; i++)
	{
		testU32 = ScalarFraction::Scalar::Constexpr::GetScalarU32<uint32_t>(numeratorU32, testSizeU32);
		numeratorU32++;
		if (numeratorU32 >= testSizeU32)
			numeratorU32 = 0;
	}
	DurationTotal = micros() - DurationTotal;
	PrintAverageNanoseconds(DurationTotal, TestSize);
	Serial.print(F(" ns"));
	BenchmarkScalarGetPath::PrintSelectedMarker<selected_u32_path_t, BenchmarkScalarGetPath::ConstexprTag>();
	Serial.println();

	Serial.print(F("\t(32 Bit Runtime)\t"));
	numeratorU32 = 0;
	DurationTotal = micros();
	for (benchmark_count_t i = 0; i < TestSize; i++)
	{
		testU32 = ScalarFraction::Scalar::Fast::GetScalarU32<uint32_t>(numeratorU32, testSizeU32);
		numeratorU32++;
		if (numeratorU32 >= testSizeU32)
			numeratorU32 = 0;
	}
	DurationTotal = micros() - DurationTotal;
	PrintAverageNanoseconds(DurationTotal, TestSize);
	Serial.print(F(" ns"));
	BenchmarkScalarGetPath::PrintSelectedMarker<selected_u32_path_t, BenchmarkScalarGetPath::RuntimeTag>();
	Serial.println();

	Serial.println(F("\tScale"));
	Serial.print(F("\t(8 Bit)\t\t"));
	DurationTotal = micros();
	for (benchmark_count_t i = 0; i < TestSize; i++)
	{
		testU8 = ScalarFraction::Fraction(applyScalarU8, applyValueU8);
	}
	DurationTotal = micros() - DurationTotal;
	PrintAverageNanoseconds(DurationTotal, TestSize);
	Serial.print(F(" ns"));
	Serial.println();

	Serial.print(F("\t(16 Bit)\t"));
	DurationTotal = micros();
	for (benchmark_count_t i = 0; i < TestSize; i++)
	{
		testU16 = ScalarFraction::Fraction(applyScalarU16, applyValueU16);
	}
	DurationTotal = micros() - DurationTotal;
	PrintAverageNanoseconds(DurationTotal, TestSize);
	Serial.print(F(" ns"));
	Serial.println();

	Serial.print(F("\t(32 Bit)\t"));
	DurationTotal = micros();
	for (benchmark_count_t i = 0; i < TestSize; i++)
	{
		testU32 = ScalarFraction::Fraction(applyScalarU32, applyValueU32);
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

template<uint32_t TestSize = BenchmarkConfig::TestSize>
void BenchmarkFraction()
{
	typedef typename BenchmarkScalarGetPath::SignedSelected<int8_t>::type selected_s8_path_t;
	typedef typename BenchmarkScalarGetPath::SignedSelected<int16_t>::type selected_s16_path_t;
	typedef typename BenchmarkScalarGetPath::SignedSelected<int32_t>::type selected_s32_path_t;

	volatile int8_t testU8 = 0;
	volatile int16_t testU16 = 0;
	volatile int32_t  testU32 = 0;
	volatile uint32_t testSizeS8Source = MaxValue<uint32_t>(1u, MinValue<uint32_t>(TestSize, INT8_MAX));
	volatile uint32_t testSizeS16Source = MaxValue<uint32_t>(1u, MinValue<uint32_t>(TestSize, INT16_MAX));
	volatile uint32_t testSizeS32Source = MaxValue<uint32_t>(1u, MinValue<uint32_t>(TestSize, 0x7FFFu));
	const int8_t testSizeS8 = static_cast<int8_t>(testSizeS8Source);
	const int16_t testSizeS16 = static_cast<int16_t>(testSizeS16Source);
	const int32_t testSizeS32 = static_cast<int32_t>(testSizeS32Source);
	const fraction8_t applyScalarS8 = fraction8_t(-(testSizeS8 >> 1));
	const fraction16_t applyScalarS16 = fraction16_t(-int16_t(testSizeS16 << 2));
	const fraction32_t applyScalarS32 = fraction32_t(-testSizeS32 << 14);
	const int8_t applyValueS8 = int8_t(INT8_MAX - testSizeS8);
	const int16_t applyValueS16 = int16_t(INT16_MAX - testSizeS16);
	const int32_t applyValueS32 = int32_t(INT32_MAX - testSizeS32);

	uint32_t DurationTotal = 0;

	Serial.println(F("\tGet"));
	Serial.print(F("\t(8 Bit Constexpr)\t"));
	int8_t numeratorS8 = 0;
	DurationTotal = micros();
	for (benchmark_count_t i = 0; i < TestSize; i++)
	{
		testU8 = ScalarFraction::Scalar::Constexpr::GetScalarS8<int8_t>(numeratorS8, testSizeS8);
		numeratorS8++;
		if (numeratorS8 >= testSizeS8)
			numeratorS8 = 0;
	}
	DurationTotal = micros() - DurationTotal;
	PrintAverageNanoseconds(DurationTotal, TestSize);
	Serial.print(F(" ns"));
	BenchmarkScalarGetPath::PrintSelectedMarker<selected_s8_path_t, BenchmarkScalarGetPath::ConstexprTag>();
	Serial.println();

	Serial.print(F("\t(8 Bit Runtime)\t\t"));
	numeratorS8 = 0;
	DurationTotal = micros();
	for (benchmark_count_t i = 0; i < TestSize; i++)
	{
		testU8 = ScalarFraction::Scalar::Fast::GetScalarS8<int8_t>(numeratorS8, testSizeS8);
		numeratorS8++;
		if (numeratorS8 >= testSizeS8)
			numeratorS8 = 0;
	}
	DurationTotal = micros() - DurationTotal;
	PrintAverageNanoseconds(DurationTotal, TestSize);
	Serial.print(F(" ns"));
	BenchmarkScalarGetPath::PrintSelectedMarker<selected_s8_path_t, BenchmarkScalarGetPath::RuntimeTag>();
	Serial.println();

	Serial.print(F("\t(16 Bit Constexpr)\t"));
	int16_t numeratorS16 = 0;
	DurationTotal = micros();
	for (benchmark_count_t i = 0; i < TestSize; i++)
	{
		testU16 = ScalarFraction::Scalar::Constexpr::GetScalarS16<int16_t>(numeratorS16, testSizeS16);
		numeratorS16++;
		if (numeratorS16 >= testSizeS16)
			numeratorS16 = 0;
	}
	DurationTotal = micros() - DurationTotal;
	PrintAverageNanoseconds(DurationTotal, TestSize);
	Serial.print(F(" ns"));
	BenchmarkScalarGetPath::PrintSelectedMarker<selected_s16_path_t, BenchmarkScalarGetPath::ConstexprTag>();
	Serial.println();

	Serial.print(F("\t(16 Bit Runtime)\t"));
	numeratorS16 = 0;
	DurationTotal = micros();
	for (benchmark_count_t i = 0; i < TestSize; i++)
	{
		testU16 = ScalarFraction::Scalar::Fast::GetScalarS16<int16_t>(numeratorS16, testSizeS16);
		numeratorS16++;
		if (numeratorS16 >= testSizeS16)
			numeratorS16 = 0;
	}
	DurationTotal = micros() - DurationTotal;
	PrintAverageNanoseconds(DurationTotal, TestSize);
	Serial.print(F(" ns"));
	BenchmarkScalarGetPath::PrintSelectedMarker<selected_s16_path_t, BenchmarkScalarGetPath::RuntimeTag>();
	Serial.println();

	Serial.print(F("\t(32 Bit Constexpr)\t"));
	int32_t numeratorS32 = 0;
	DurationTotal = micros();
	for (benchmark_count_t i = 0; i < TestSize; i++)
	{
		testU32 = ScalarFraction::Scalar::Constexpr::GetScalarS32<int32_t>(numeratorS32, testSizeS32);
		numeratorS32++;
		if (numeratorS32 >= testSizeS32)
			numeratorS32 = 0;
	}
	DurationTotal = micros() - DurationTotal;
	PrintAverageNanoseconds(DurationTotal, TestSize);
	Serial.print(F(" ns"));
	BenchmarkScalarGetPath::PrintSelectedMarker<selected_s32_path_t, BenchmarkScalarGetPath::ConstexprTag>();
	Serial.println();

	Serial.print(F("\t(32 Bit Runtime)\t"));
	numeratorS32 = 0;
	DurationTotal = micros();
	for (benchmark_count_t i = 0; i < TestSize; i++)
	{
		testU32 = ScalarFraction::Scalar::Fast::GetScalarS32<int32_t>(numeratorS32, testSizeS32);
		numeratorS32++;
		if (numeratorS32 >= testSizeS32)
			numeratorS32 = 0;
	}
	DurationTotal = micros() - DurationTotal;
	PrintAverageNanoseconds(DurationTotal, TestSize);
	Serial.print(F(" ns"));
	BenchmarkScalarGetPath::PrintSelectedMarker<selected_s32_path_t, BenchmarkScalarGetPath::RuntimeTag>();
	Serial.println();

	Serial.println(F("\tScale"));
	Serial.print(F("\t(8 Bit)\t\t"));
	DurationTotal = micros();
	for (benchmark_count_t i = 0; i < TestSize; i++)
	{
		testU8 = ScalarFraction::Fraction(applyScalarS8, applyValueS8);
	}
	DurationTotal = micros() - DurationTotal;
	PrintAverageNanoseconds(DurationTotal, TestSize);
	Serial.println(F(" ns"));

	Serial.print(F("\t(16 Bit)\t"));
	DurationTotal = micros();
	for (benchmark_count_t i = 0; i < TestSize; i++)
	{
		testU16 = ScalarFraction::Fraction(applyScalarS16, applyValueS16);
	}
	DurationTotal = micros() - DurationTotal;
	PrintAverageNanoseconds(DurationTotal, TestSize);
	Serial.println(F(" ns"));

	Serial.print(F("\t(32 Bit)\t"));
	DurationTotal = micros();
	for (benchmark_count_t i = 0; i < TestSize; i++)
	{
		testU32 = ScalarFraction::Fraction(applyScalarS32, applyValueS32);
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