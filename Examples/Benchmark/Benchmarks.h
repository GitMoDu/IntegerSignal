// Benchmarks.h

#ifndef _BENCHMARKS_h
#define _BENCHMARKS_h

#include <IntegerSignal.h>
#include <IntegerTrigonometry8.h>
#include <IntegerTrigonometry16.h>
#include <Arduino.h>

using namespace IntegerSignal;
using namespace IntegerSignal::Trigonometry;
using namespace IntegerSignal::Resize;
using namespace IntegerSignal::Fraction;


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
void BenchmarkSine()
{
	volatile IntegerSignal::Fraction::fraction8_t testU8 = UINT8_MAX;
	volatile IntegerSignal::Fraction::fraction16_t testU16 = UINT16_MAX;
	volatile IntegerSignal::Fraction::fraction32_t testU32 = UINT32_MAX;

	uint32_t DurationTotal = 0;

	Serial.print(F("\t(Sine8)\t\t"));
	DurationTotal = micros();
	for (uint16_t t = 0; t < TestSize; t++)
	{
		for (angle_t i = 0; i < ANGLE_RANGE; i++)
		{
			testU8 = IntegerSignal::Trigonometry::Sine8(i);
		}
	}
	DurationTotal = micros() - DurationTotal;
	Serial.print((uint32_t)(((uint64_t)DurationTotal * 1000L) / ANGLE_RANGE / TestSize));
	Serial.println(F(" ns"));

	Serial.print(F("\t(Sine16)\t"));
	DurationTotal = micros();
	for (uint16_t t = 0; t < TestSize; t++)
	{
		for (angle_t i = 0; i < ANGLE_RANGE; i++)
		{
			testU16 = IntegerSignal::Trigonometry::Sine16(i);
		}
	}
	DurationTotal = micros() - DurationTotal;
	Serial.print((uint32_t)(((uint64_t)DurationTotal * 1000L) / ANGLE_RANGE / TestSize));
	Serial.println(F(" ns"));

	Serial.print(F("\t(Sine32)\t"));
	DurationTotal = micros();
	for (uint16_t t = 0; t < TestSize; t++)
	{
		for (angle_t i = 0; i < ANGLE_RANGE; i++)
		{
			testU32 = IntegerSignal::Trigonometry::Sine32(i);
		}
	}
	DurationTotal = micros() - DurationTotal;
	Serial.print((uint32_t)(((uint64_t)DurationTotal * 1000L) / ANGLE_RANGE / TestSize));
	Serial.println(F(" ns"));


	Serial.println();
}

template<uint16_t TestSize = 2000>
void BenchmarkTangent()
{
	volatile IntegerSignal::Fraction::fraction8_t testU8 = UINT8_MAX;
	volatile IntegerSignal::Fraction::fraction16_t testU16 = UINT16_MAX;
	volatile IntegerSignal::Fraction::fraction32_t testU32 = UINT32_MAX;

	constexpr angle_t AngleRange = GetAngle(45 / 2);

	uint32_t DurationTotal = 0;

	Serial.print(F("\t(Tangent8)\t"));
	DurationTotal = micros();
	for (uint16_t t = 0; t < TestSize; t++)
	{
		for (uint16_t i = 0; i < AngleRange; i++)
		{
			testU8 = IntegerSignal::Trigonometry::Tangent8(i);
		}
	}
	DurationTotal = micros() - DurationTotal;
	Serial.print((uint32_t)(((uint64_t)DurationTotal * 1000L) / AngleRange / TestSize));
	Serial.println(F(" ns"));

	Serial.print(F("\t(Tangent16)\t"));
	DurationTotal = micros();
	for (uint16_t t = 0; t < TestSize; t++)
	{
		for (uint16_t i = 0; i < AngleRange; i++)
		{
			testU16 = IntegerSignal::Trigonometry::Tangent16(i);
		}
	}
	DurationTotal = micros() - DurationTotal;
	Serial.print((uint32_t)(((uint64_t)DurationTotal * 1000L) / AngleRange / TestSize));
	Serial.println(F(" ns"));

	Serial.print(F("\t(Tangent32)\t"));
	DurationTotal = micros();
	for (uint16_t t = 0; t < TestSize; t++)
	{
		for (uint16_t i = 0; i < AngleRange; i++)
		{
			testU32 = IntegerSignal::Trigonometry::Tangent32(i);
		}
	}
	DurationTotal = micros() - DurationTotal;
	Serial.print((uint32_t)(((uint64_t)DurationTotal * 1000L) / AngleRange / TestSize));
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

	Serial.println(F("\tSquare Root (Root^2)"));
	Serial.print(F("\t(16 Bit)\t"));
	DurationTotal = micros();
	for (uint16_t i = 0; i < TestSize; i++)
	{
		testU16 = IntegerSignal::SquareRoot16(testU16);
	}
	DurationTotal = micros() - DurationTotal;
	Serial.print((DurationTotal * 1000L) / TestSize);
	Serial.println(F(" ns"));

	Serial.print(F("\t(32 Bit)\t"));
	DurationTotal = micros();
	for (uint16_t i = 0; i < TestSize; i++)
	{
		testU32 = IntegerSignal::SquareRoot32(testU32);
	}
	DurationTotal = micros() - DurationTotal;
	Serial.print((DurationTotal * 1000L) / TestSize);
	Serial.println(F(" ns"));

	Serial.print(F("\t(64 Bit)\t"));
	DurationTotal = micros();
	for (uint16_t i = 0; i < TestSize; i++)
	{
		testU64 = IntegerSignal::SquareRoot64(testU64);
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
	volatile uint64_t testU64 = UINT64_MAX;

	uint32_t DurationTotal = 0;

	Serial.println(F("\tuint8_t"));
	Serial.print(F("\t(16 Bit)\t"));
	testU8 = UINT8_MAX;
	DurationTotal = micros();
	for (uint16_t i = 0; i < TestSize; i++)
	{
		testU16 = IntegerSignal::U8ToU16(testU8);
	}
	DurationTotal = micros() - DurationTotal;
	Serial.print((DurationTotal * 1000L) / TestSize);
	Serial.println(F(" ns"));

	Serial.print(F("\t(32 Bit)\t"));
	testU8 = UINT8_MAX;
	DurationTotal = micros();
	for (uint16_t i = 0; i < TestSize; i++)
	{
		testU32 = IntegerSignal::U8ToU32(testU8);
	}
	DurationTotal = micros() - DurationTotal;
	Serial.print((DurationTotal * 1000L) / TestSize);
	Serial.println(F(" ns"));

	Serial.print(F("\t(64 Bit)\t"));
	testU8 = UINT8_MAX;
	DurationTotal = micros();
	for (uint16_t i = 0; i < TestSize; i++)
	{
		testU64 = IntegerSignal::U8ToU64(testU8);
	}
	DurationTotal = micros() - DurationTotal;
	Serial.print((DurationTotal * 1000L) / TestSize);
	Serial.println(F(" ns"));
	Serial.println();

	Serial.println(F("\tuint16_t"));
	Serial.print(F("\t(32 Bit)\t"));
	testU16 = UINT16_MAX;
	DurationTotal = micros();
	for (uint16_t i = 0; i < TestSize; i++)
	{
		testU32 = IntegerSignal::U16ToU32(testU16);
	}
	DurationTotal = micros() - DurationTotal;
	Serial.print((DurationTotal * 1000L) / TestSize);
	Serial.println(F(" ns"));

	Serial.print(F("\t(64 Bit)\t"));
	testU16 = UINT16_MAX;
	DurationTotal = micros();
	for (uint16_t i = 0; i < TestSize; i++)
	{
		testU64 = IntegerSignal::U16ToU64(testU16);
	}
	DurationTotal = micros() - DurationTotal;
	Serial.print((DurationTotal * 1000L) / TestSize);
	Serial.println(F(" ns"));
	Serial.println();

	Serial.println(F("\tuint32_t"));
	Serial.print(F("\t(64 Bit)\t"));
	testU32 = UINT32_MAX;
	DurationTotal = micros();
	for (uint16_t i = 0; i < TestSize; i++)
	{
		testU64 = IntegerSignal::U32ToU64(testU32);
	}
	DurationTotal = micros() - DurationTotal;
	Serial.print((DurationTotal * 1000L) / TestSize);
	Serial.println(F(" ns"));
}

template<uint16_t TestSize = 2000>
void BenchmarkUFraction()
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
		testU8 = Fraction::GetUFraction8(i, (uint8_t)TestSize);
	}
	DurationTotal = micros() - DurationTotal;
	Serial.print((DurationTotal * 1000L) / UINT8_MAX);
	Serial.println(F(" ns"));

	Serial.print(F("\t(16 Bit)\t"));
	DurationTotal = micros();
	for (uint16_t i = 0; i < TestSize; i++)
	{
		testU16 = Fraction::GetUFraction16(i, TestSize);
	}
	DurationTotal = micros() - DurationTotal;
	Serial.print((DurationTotal * 1000L) / TestSize);
	Serial.println(F(" ns"));

	Serial.print(F("\t(32 Bit)\t"));
	DurationTotal = micros();
	for (uint16_t i = 0; i < TestSize; i++)
	{
		testU8 = Fraction::GetUFraction32((uint32_t)i, (uint32_t)TestSize);
	}
	DurationTotal = micros() - DurationTotal;
	Serial.print((DurationTotal * 1000L) / TestSize);
	Serial.println(F(" ns"));

	Serial.println(F("\tScale"));
	Serial.print(F("\t(8 Bit)\t\t"));
	DurationTotal = micros();
	for (ufraction8_t i = 0; i <= UFRACTION8_1X; i++)
	{
		testU8 = Fraction::Scale(i, i);
	}
	DurationTotal = micros() - DurationTotal;
	Serial.print((DurationTotal * 1000L) / UFRACTION8_1X);
	Serial.println(F(" ns"));

	Serial.print(F("\t(16 Bit)\t"));
	DurationTotal = micros();
	for (uint32_t i = 0; i < ((uint32_t)TestSize * UINT8_MAX); i += UINT8_MAX)
	{
		testU16 = Fraction::Scale(ufraction16_t(i), (uint16_t)i);
	}
	DurationTotal = micros() - DurationTotal;
	Serial.print((DurationTotal * 1000L) / TestSize);
	Serial.println(F(" ns"));

	Serial.print(F("\t(32 Bit)\t"));
	DurationTotal = micros();
	for (uint32_t i = 0; i < ((uint32_t)TestSize * UINT16_MAX); i += UINT16_MAX)
	{
		testU32 = Fraction::Scale(ufraction32_t(i), i);
	}
	DurationTotal = micros() - DurationTotal;
	Serial.print((DurationTotal * 1000L) / TestSize);
	Serial.println(F(" ns"));
	Serial.println();
}

template<int16_t TestSize = 2000>
void BenchmarkFraction()
{
	volatile int8_t testU8 = 0;
	volatile int16_t testU16 = 0;
	volatile int32_t  testU32 = 0;

	uint32_t DurationTotal = 0;

	Serial.println(F("\tGet"));
	Serial.print(F("\t(8 Bit)\t\t"));
	DurationTotal = micros();
	for (int8_t i = INT8_MIN; i < INT8_MAX; i++)
	{
		testU8 = Fraction::GetFraction8((int16_t)i, TestSize);
	}
	DurationTotal = micros() - DurationTotal;
	Serial.print((DurationTotal * 1000L) / INT8_MAX);
	Serial.println(F(" ns"));

	Serial.print(F("\t(16 Bit)\t"));
	DurationTotal = micros();
	for (int16_t i = 0; i < TestSize; i++)
	{
		testU16 = Fraction::GetFraction16(i, TestSize);
	}
	DurationTotal = micros() - DurationTotal;
	Serial.print((DurationTotal * 1000L) / TestSize);
	Serial.println(F(" ns"));

	Serial.print(F("\t(32 Bit)\t"));
	DurationTotal = micros();
	for (int16_t i = 0; i < TestSize; i++)
	{
		testU8 = Fraction::GetFraction32((int32_t)i, (int32_t)TestSize);
	}
	DurationTotal = micros() - DurationTotal;
	Serial.print((DurationTotal * 1000L) / TestSize);
	Serial.println(F(" ns"));

	Serial.println(F("\tScale"));
	Serial.print(F("\t(8 Bit)\t\t"));
	DurationTotal = micros();
	for (ufraction8_t i = 0; i <= FRACTION8_1X; i++)
	{
		testU8 = Fraction::Scale(fraction8_t(-fraction8_t(i)), (uint8_t)i);
	}
	DurationTotal = micros() - DurationTotal;
	Serial.print((DurationTotal * 1000L) / FRACTION8_1X);
	Serial.println(F(" ns"));

	Serial.print(F("\t(16 Bit)\t"));
	DurationTotal = micros();
	for (uint32_t i = 0; i < ((uint32_t)TestSize * UINT8_MAX); i += UINT8_MAX)
	{
		testU16 = Fraction::Scale(fraction16_t(-fraction16_t(i)), (int16_t)i);
	}
	DurationTotal = micros() - DurationTotal;
	Serial.print((DurationTotal * 1000L) / TestSize);
	Serial.println(F(" ns"));

	Serial.print(F("\t(32 Bit)\t"));
	DurationTotal = micros();
	for (uint32_t i = 0; i < ((uint32_t)TestSize * UINT16_MAX); i += UINT16_MAX)
	{
		testU32 = Fraction::Scale(fraction32_t(-fraction32_t(i)), (int32_t)i);
	}
	DurationTotal = micros() - DurationTotal;
	Serial.print((DurationTotal * 1000L) / TestSize);
	Serial.println(F(" ns"));
	Serial.println();
}

template<uint16_t TestSize = 2000>
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
		testU8 = Resize::GetResize8(i, (uint8_t)TestSize);
	}
	DurationTotal = micros() - DurationTotal;
	Serial.print((DurationTotal * 1000L) / UINT8_MAX);
	Serial.println(F(" ns"));

	Serial.print(F("\t(16 Bit)\t"));
	DurationTotal = micros();
	for (uint16_t i = 0; i < TestSize; i++)
	{
		testU16 = Resize::GetResize16(i, TestSize);
	}
	DurationTotal = micros() - DurationTotal;
	Serial.print((DurationTotal * 1000L) / TestSize);
	Serial.println(F(" ns"));

	Serial.print(F("\t(32 Bit)\t"));
	DurationTotal = micros();
	for (uint16_t i = 0; i < TestSize; i++)
	{
		testU8 = Resize::GetResize32((uint32_t)i, (uint32_t)TestSize);
	}
	DurationTotal = micros() - DurationTotal;
	Serial.print((DurationTotal * 1000L) / TestSize);
	Serial.println(F(" ns"));

	Serial.println(F("\tScale"));
	Serial.print(F("\t(8 Bit)\t\t"));
	DurationTotal = micros();
	for (resize8_t i = 0; i <= RESIZE8_1X; i++)
	{
		testU8 = Resize::Scale(i, i);
	}
	DurationTotal = micros() - DurationTotal;
	Serial.print((DurationTotal * 1000L) / RESIZE8_1X);
	Serial.println(F(" ns"));

	Serial.print(F("\t(16 Bit)\t"));
	DurationTotal = micros();
	for (uint32_t i = 0; i < ((uint32_t)TestSize * UINT8_MAX); i += UINT8_MAX)
	{
		testU16 = Resize::Scale(resize16_t(i), (uint16_t)i);
	}
	DurationTotal = micros() - DurationTotal;
	Serial.print((DurationTotal * 1000L) / TestSize);
	Serial.println(F(" ns"));

	Serial.print(F("\t(32 Bit)\t"));
	DurationTotal = micros();
	for (uint32_t i = 0; i < ((uint32_t)TestSize * UINT16_MAX); i += UINT16_MAX)
	{
		testU32 = Resize::Scale(resize32_t(i), i);
	}
	DurationTotal = micros() - DurationTotal;
	Serial.print((DurationTotal * 1000L) / TestSize);
	Serial.println(F(" ns"));
	Serial.println();
}

#endif

