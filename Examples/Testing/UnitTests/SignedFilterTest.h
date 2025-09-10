#ifndef _SIGNED_FILTER_TEST_h
#define _SIGNED_FILTER_TEST_h

#include <IntegerSignalTesting.h>
#include <IntegerSignal.h>

// Tests for IntegerSignal::Filter::Signed meta-filter wrapper.
// Validates that wrapping an unsigned filter produces the same result as
// running the unsigned filter on an offset input and subtracting the offset.

namespace IntegerSignal::Filter::Template::Test
{
	// Compute Middle (unsigned offset) matching Signed::Middle() for given unsigned type.
	template<typename U>
	constexpr U MiddleOf()
	{
		return (U)(
			(INT8_MAX * (sizeof(U) == sizeof(uint8_t))) |
			(INT16_MAX * (sizeof(U) == sizeof(uint16_t))) |
			(INT32_MAX * (sizeof(U) == sizeof(uint32_t))) |
			(INT64_MAX * (sizeof(U) == sizeof(uint64_t)))
			);
	}

	// Generic equivalence test:
	// Compare a Signed wrapper against the underlying unsigned filter by feeding
	// the same logical signal in two domains:
	// - Signed:  x_s -> Signed(FilterU)
	// - Unsigned: x_u = Middle + x_s -> FilterU -> y_u, then y_s = y_u - Middle
	// and require y_s(wrapper) == y_s(reference).
	template<typename UnsignedFilter, typename SignedFilter, typename UnsignedT, typename SignedT>
	static bool TestSignedEquivalence(const SignedT inputs[], const size_t count)
	{
		const UnsignedT M = MiddleOf<UnsignedT>();

		UnsignedFilter uf;
		SignedFilter sf;

		// Initialize steady state to signed 0 -> unsigned Middle
		uf.Clear(M);
		sf.Clear();

		for (size_t i = 0; i < count; i++)
		{
			const SignedT xs = inputs[i];

			// Reference unsigned path
			const UnsignedT xu = (UnsignedT)(M + xs);
			uf.Set(xu);
			uf.Step();
			const UnsignedT yu = uf.Get();
			const SignedT y_ref = (SignedT)yu - (SignedT)M;

			// Signed wrapper path
			sf.Set(xs);
			sf.Step();
			const SignedT y_wrapped = sf.Get();

			if (y_wrapped != y_ref)
			{
				Serial.print(F("Signed equivalence failed at i="));
				Serial.print((uint32_t)i);
				Serial.print(F(" xs=")); Serial.print((int32_t)xs);
				Serial.print(F(" xu=")); Serial.print((uint32_t)xu);
				Serial.print(F(" y_wrapped=")); Serial.print((int32_t)y_wrapped);
				Serial.print(F(" y_ref=")); Serial.println((int32_t)y_ref);
				return false;
			}
		}
		return true;
	}

	// Simple monotonic step behavior for signed wrapper.
	template<typename SignedFilter, typename SignedT>
	static bool TestSignedMonotonic(const SignedT start, const SignedT target, const uint32_t steps, const bool expectIncrease)
	{
		SignedFilter f;
		f.Clear();

		// Bring it to steady-state at 'start'
		{
			SignedT prev = f.Get();
			for (uint32_t i = 0; i < steps; i++)
			{
				f.Set(start);
				f.Step();
				prev = f.Get();
			}
			if (prev != start) return false;
		}

		SignedT prev = f.Get();
		for (uint32_t i = 0; i < steps; i++)
		{
			f.Set(target);
			f.Step();
			const SignedT y = f.Get();
			if (expectIncrease)
			{
				if (y < prev || y > target) return false;
			}
			else
			{
				if (y > prev || y < target) return false;
			}
			prev = y;
		}
		return (prev == target);
	}

	// Exhaustive mapping test on 8-bit with pass-through LowPass<0>.
	// This isolates the Signed mapping: output must equal input for all values.
	static bool TestI8PassThrough()
	{
		using U = uint8_t;
		using S = int8_t;
		using UFilter = IntegerSignal::Filters::LowPassU8<0>;
		using SFilter = IntegerSignal::Filter::SignedI8<UFilter>;

		SFilter sf;
		sf.Clear();

		for (int v = INT8_MIN; v <= INT8_MAX; v++)
		{
			const S x = (S)v;
			sf.Set(x);
			sf.Step();
			const S y = sf.Get();
			if (y != x)
			{
				Serial.print(F("Signed pass-through I8 failed: x="));
				Serial.print((int32_t)x);
				Serial.print(F(" y="));
				Serial.println((int32_t)y);
				return false;
			}
		}
		return true;
	}

	// U8 equivalence tests with a smoothing filter (LowPass<4>, Ema<4>, Dema<2>)
	static bool TestI8Equivalence()
	{
		bool pass = true;

		// Test sequence: exhaustive small core + some ramps and edges
		int8_t seq[512];
		size_t n = 0;

		// Exhaustive small range around zero
		for (int v = -64; v <= 64; v++) seq[n++] = (int8_t)v;
		// Edges
		seq[n++] = INT8_MIN; seq[n++] = INT8_MAX;
		// Ramp up
		for (int v = -128; v <= 127; v += 8) seq[n++] = (int8_t)v;
		// Ramp down
		for (int v = 127; v >= -128; v -= 8) seq[n++] = (int8_t)v;

		// LowPass<4>
		{
			using UFilter = IntegerSignal::Filters::LowPassU8<4>;
			using SFilter = IntegerSignal::Filter::SignedI8<UFilter>;
			pass &= TestSignedEquivalence<UFilter, SFilter, uint8_t, int8_t>(seq, n);
		}
		// EMA<4>
		{
			using UFilter = IntegerSignal::Filters::EmaU8<4>;
			using SFilter = IntegerSignal::Filter::SignedI8<UFilter>;
			pass &= TestSignedEquivalence<UFilter, SFilter, uint8_t, int8_t>(seq, n);
		}
		// DEMA<2>
		{
			using UFilter = IntegerSignal::Filters::DemaU8<2>;
			using SFilter = IntegerSignal::Filter::SignedI8<UFilter>;
			pass &= TestSignedEquivalence<UFilter, SFilter, uint8_t, int8_t>(seq, n);
		}

		return pass;
	}

	// U16 sampled equivalence (LowPass<8>, Ema<8>, Dema<8>)
	template<uint32_t MaxIterations = 10000>
	static bool TestI16Equivalence()
	{
		bool pass = true;

		int16_t seq[512];
		size_t n = 0;

		// Key edges
		const int16_t edges[] = { INT16_MIN, (int16_t)-32767, -16384, -1, 0, 1, 16384, 32766, INT16_MAX };
		for (size_t i = 0; i < sizeof(edges) / sizeof(edges[0]); i++) seq[n++] = edges[i];

		// Sampled ramp across range
		for (uint32_t i = 0; i < MaxIterations; i += (MaxIterations / 64) ? (MaxIterations / 64) : 1)
		{
			const int32_t v = (int32_t)((int64_t)i * INT16_MAX / (MaxIterations ? MaxIterations : 1)) - (INT16_MAX / 2);
			seq[n++] = (int16_t)v;
			if (n >= sizeof(seq) / sizeof(seq[0])) break;
		}

		// LowPass<8>
		{
			using UFilter = IntegerSignal::Filters::LowPassU16<8>;
			using SFilter = IntegerSignal::Filter::SignedI16<UFilter>;
			pass &= TestSignedEquivalence<UFilter, SFilter, uint16_t, int16_t>(seq, n);
		}
		// EMA<8>
		{
			using UFilter = IntegerSignal::Filters::EmaU16<8>;
			using SFilter = IntegerSignal::Filter::SignedI16<UFilter>;
			pass &= TestSignedEquivalence<UFilter, SFilter, uint16_t, int16_t>(seq, n);
		}
		// DEMA<8>
		{
			using UFilter = IntegerSignal::Filters::DemaU16<8>;
			using SFilter = IntegerSignal::Filter::SignedI16<UFilter>;
			pass &= TestSignedEquivalence<UFilter, SFilter, uint16_t, int16_t>(seq, n);
		}

		return pass;
	}

	// U32 sampled equivalence (LowPass<8>, Ema<8>, Dema<8>)
	template<uint32_t MaxIterations = 5000>
	static bool TestI32Equivalence()
	{
		bool pass = true;

		int32_t seq[512];
		size_t n = 0;

		// Key edges
		const int32_t edges[] = { INT32_MIN, -2147483647, -1073741824, -1, 0, 1, 1073741824, 2147483646, INT32_MAX };
		for (size_t i = 0; i < sizeof(edges) / sizeof(edges[0]); i++) seq[n++] = edges[i];

		// Sampled ramp across range
		for (uint32_t i = 0; i < MaxIterations; i += (MaxIterations / 64) ? (MaxIterations / 64) : 1)
		{
			const int64_t v = (int64_t)i * INT32_MAX / (MaxIterations ? MaxIterations : 1) - (INT32_MAX / 2);
			seq[n++] = (int32_t)v;
			if (n >= sizeof(seq) / sizeof(seq[0])) break;
		}

		// LowPass<8>
		{
			using UFilter = IntegerSignal::Filters::LowPassU32<8>;
			using SFilter = IntegerSignal::Filter::SignedI32<UFilter>;
			pass &= TestSignedEquivalence<UFilter, SFilter, uint32_t, int32_t>(seq, n);
		}
		// EMA<8>
		{
			using UFilter = IntegerSignal::Filters::EmaU32<8>;
			using SFilter = IntegerSignal::Filter::SignedI32<UFilter>;
			pass &= TestSignedEquivalence<UFilter, SFilter, uint32_t, int32_t>(seq, n);
		}
		// DEMA<8>
		{
			using UFilter = IntegerSignal::Filters::DemaU32<8>;
			using SFilter = IntegerSignal::Filter::SignedI32<UFilter>;
			pass &= TestSignedEquivalence<UFilter, SFilter, uint32_t, int32_t>(seq, n);
		}

		return pass;
	}

	// Simple signed monotonic checks with a smoothing filter (LowPass).
	static bool TestSignedMonotonicI8()
	{
		using SFilter = IntegerSignal::Filter::SignedI8<IntegerSignal::Filters::LowPassU8<4>>;
		return TestSignedMonotonic<SFilter, int8_t>(-64, 64, 1024, true)
			&& TestSignedMonotonic<SFilter, int8_t>(64, -64, 1024, false);
	}

	static bool TestSignedMonotonicI16()
	{
		using SFilter = IntegerSignal::Filter::SignedI16<IntegerSignal::Filters::LowPassU16<8>>;
		return TestSignedMonotonic<SFilter, int16_t>(-10000, 10000, 4096, true)
			&& TestSignedMonotonic<SFilter, int16_t>(10000, -10000, 4096, false);
	}

	static bool TestSignedMonotonicI32()
	{
		using SFilter = IntegerSignal::Filter::SignedI32<IntegerSignal::Filters::LowPassU32<8>>;
		return TestSignedMonotonic<SFilter, int32_t>(-1000000000, 1000000000, 8192, true)
			&& TestSignedMonotonic<SFilter, int32_t>(1000000000, -1000000000, 8192, false);
	}

	// Templated main entry to match other suites.
	template<uint32_t MaxIterations = 50000>
	static bool RunTests()
	{
		Serial.println(F("Starting Signed meta-filter tests..."));
		bool pass = true;

		pass &= TestI8PassThrough();       // strict mapping with LowPass<0>
		pass &= TestI8Equivalence();       // U8 with LowPass/Ema/Dema

		pass &= TestI16Equivalence<MaxIterations / 5>();
		pass &= TestI32Equivalence<MaxIterations / 10>();

		pass &= TestSignedMonotonicI8();
		pass &= TestSignedMonotonicI16();
		pass &= TestSignedMonotonicI32();

		if (pass) Serial.println(F("Signed meta-filter tests PASSED."));
		else      Serial.println(F("Signed meta-filter tests FAILED."));

		return pass;
	}
}

#endif