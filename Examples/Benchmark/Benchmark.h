#ifndef _INTEGER_SIGNAL_BENCHMARK_h
#define _INTEGER_SIGNAL_BENCHMARK_h

#include <stdint.h>
#include <IntegerSignal.h>
#include <IntegerTrigonometry8.h>
#include <IntegerTrigonometry16.h>
#include <Arduino.h>

using namespace IntegerSignal;
using namespace IntegerSignal::Trigonometry;
namespace ScalarFraction = IntegerSignal::FixedPoint::ScalarFraction;
namespace FactorScale = IntegerSignal::FixedPoint::FactorScale;

namespace BenchmarkConfig
{
#if defined(ARDUINO_ARCH_AVR)
	static constexpr uint32_t TestSize = 25000;
#else
	static constexpr uint32_t TestSize = 100000;
#endif
	static constexpr uint32_t TrigonometryTestSize = (TestSize / 200) | 3;
}

using benchmark_count_t = uint32_t;

static uint32_t GetAverageNanoseconds(const uint32_t durationMicroseconds, const uint64_t iterationCount)
{
	return iterationCount == 0 ? 0u : static_cast<uint32_t>((static_cast<uint64_t>(durationMicroseconds) * 1000ULL) / iterationCount);
}

static void PrintAverageNanoseconds(const uint32_t durationMicroseconds, const uint64_t iterationCount)
{
	Serial.print(GetAverageNanoseconds(durationMicroseconds, iterationCount));
}

static volatile uint32_t BenchmarkResultSink = 0;

namespace BenchmarkScalarGetPath
{
	struct ConstexprTag {};
	struct RuntimeTag {};

	template<typename T>
	struct UnsignedSelected
	{
		using type = ConstexprTag;
	};

	template<typename T>
	struct SignedSelected
	{
		using type = ConstexprTag;
	};

#if !defined(INTEGER_SIGNAL_DISABLE_ACCELERATION) && defined(__AVR__)
	template<>
	struct UnsignedSelected<uint8_t>
	{
		using type = RuntimeTag;
	};

	template<>
	struct UnsignedSelected<uint16_t>
	{
		using type = RuntimeTag;
	};

	template<>
	struct SignedSelected<int8_t>
	{
		using type = RuntimeTag;
	};

	template<>
	struct SignedSelected<int16_t>
	{
		using type = RuntimeTag;
	};
#endif

#if !defined(INTEGER_SIGNAL_DISABLE_ACCELERATION) && (INTPTR_MAX == INT32_MAX)
	template<>
	struct UnsignedSelected<uint32_t>
	{
		using type = RuntimeTag;
	};

	template<>
	struct SignedSelected<int32_t>
	{
		using type = RuntimeTag;
	};
#endif

	template<typename SelectedTag, typename CandidateTag>
	static void PrintSelectedMarker()
	{
		if (TypeTraits::TypeDispatch::is_same<SelectedTag, CandidateTag>::value)
			Serial.print(F(" [Auto]"));
	}
}

template<typename T>
static void ConsumeBenchmarkResult(const T value)
{
	BenchmarkResultSink ^= static_cast<uint32_t>(value);
}

#endif

