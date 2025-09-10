#ifndef _DEMA_FILTER_TEST_h
#define _DEMA_FILTER_TEST_h

#include <IntegerSignalTesting.h>
#include <IntegerSignal.h>

namespace IntegerSignal::Filters::Dema::Test
{
	// Helpers
	template<typename T>
	static T AbsDiff(T a, T b) { return (a > b) ? (a - b) : (b - a); }

	// One-stage EMA ref step (residual form): y = round((R + x) / 2^f); R' = R + x - y
	template<typename ResidualT, typename InputT, uint8_t Factor>
	static inline InputT EmaRefStep(ResidualT& residual, const InputT x)
	{
		const ResidualT half = (ResidualT)1 << (Factor - 1);
		const ResidualT s = residual + (ResidualT)x;
		const InputT y = (InputT)((s + half) >> Factor);
		residual = s - (ResidualT)y;
		return y;
	}

	// Two-stage DEMA ref step: stage1 EMA then stage2 EMA on stage1 output.
	template<typename ResidualT, typename InputT, uint8_t Factor>
	static inline InputT DemaRefStep(ResidualT& r1, ResidualT& r2, const InputT x)
	{
		const InputT y1 = EmaRefStep<ResidualT, InputT, Factor>(r1, x);
		return EmaRefStep<ResidualT, InputT, Factor>(r2, y1);
	}

	// Signed wrapper tests: symmetric behavior around 0, both directions monotonic.
	template<typename SFilterT, typename ValueT>
	static bool TestSignedBehavior(const ValueT neg, const ValueT pos, const uint32_t steps)
	{
		SFilterT f;
		f.Clear();

		// Step to positive
		{
			ValueT prev = f.Get();
			for (uint32_t i = 0; i < steps; i++)
			{
				f.Set(pos);
				f.Step();
				const ValueT y = f.Get();
				if (y < prev || y > pos) return false;
				prev = y;
			}
			if (f.Get() != pos) return false;
		}

		// Step to negative
		{
			ValueT prev = f.Get();
			for (uint32_t i = 0; i < steps; i++)
			{
				f.Set(neg);
				f.Step();
				const ValueT y = f.Get();
				if (y > prev || y < neg) return false;
				prev = y;
			}
			if (f.Get() != neg) return false;
		}
		return true;
	}

	// DC convergence: starting from Clear(0), with constant input 'dc', output must converge to 'dc'.
	template<typename FilterT, typename ValueT>
	static bool TestDCConvergence(const ValueT dc, const uint32_t steps)
	{
		FilterT f;
		f.Clear(0);

		ValueT prev = f.Get();
		for (uint32_t i = 0; i < steps; i++)
		{
			f.Set(dc);
			f.Step();
			const ValueT y = f.Get();
			if ((y > dc) || (y < prev)) return false;
			prev = y;
		}
		return (prev == dc);
	}

	// Monotonic step response up/down and settle to target.
	template<typename FilterT, typename ValueT>
	static bool TestStepMonotonic(const ValueT start, const ValueT target, const uint32_t steps, const bool expectIncrease)
	{
		FilterT f;
		f.Clear(start);

		ValueT prev = f.Get();
		if (prev != start) return false;

		for (uint32_t i = 0; i < steps; i++)
		{
			f.Set(target);
			f.Step();
			const ValueT y = f.Get();

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

	// Saturation: max input should reach and hold max output without overflow or decrease.
	template<typename FilterT, typename ValueT>
	static bool TestSaturation(const ValueT vmax, const uint32_t steps)
	{
		FilterT f;
		f.Clear(0);
		ValueT prev = 0;

		for (uint32_t i = 0; i < steps; i++)
		{
			f.Set(vmax);
			f.Step();
			const ValueT y = f.Get();
			if (y < prev || y > vmax) return false;
			prev = y;
		}
		return (prev == vmax);
	}

	// Linearity (approximate under quantization): filter(k*x) ~= k*filter(x)
	template<typename FilterT, typename ValueT>
	static bool TestLinearity(const ValueT maxInput, const uint8_t k, const uint8_t tolerance = 0)
	{
		const uint8_t tol = (tolerance == 0) ? (uint8_t)(k ? (k - 1) : 0) : tolerance;

		FilterT f1, f2;
		f1.Clear(0);
		f2.Clear(0);

		for (ValueT x = 0; x <= maxInput; x++)
		{
			f1.Set(x); f1.Step();
			f2.Set((ValueT)(x * k)); f2.Step();

			const ValueT y1 = f1.Get();
			const ValueT y2 = f2.Get();
			const ValueT ky1 = (ValueT)(y1 * k);
			const ValueT diff = (ky1 > y2) ? (ky1 - y2) : (y2 - ky1);
			if (diff > tol) return false;
		}
		return true;
	}

	// -----------------------------------------------------------------------
	// Exhaustive and Sampled tests
	// -----------------------------------------------------------------------

	// Exhaustive one-step correctness for U8 DEMA:
	// Prepare steady state at y0 via Clear(y0); reference uses residuals r1=r2=y0*(2^f -1).
	template<uint8_t Factor>
	static bool TestDemaU8ExhaustiveOneStep()
	{
#if defined(ARDUINO)
		Serial.print(F("Starting exhaustive DemaU8<"));
		Serial.print(Factor);
		Serial.println(F("> one-step tests..."));
#endif
		using F = IntegerSignal::Filters::DemaU8<Factor>;

		uint32_t errorCount = 0;

		for (uint16_t y0 = 0; y0 <= UINT8_MAX; y0++)
		{
			for (uint16_t x = 0; x <= UINT8_MAX; x++)
			{
				F f; f.Clear((uint8_t)y0);
				f.Set((uint8_t)x);
				f.Step();
				const uint8_t y = f.Get();

				uint32_t r1 = (uint32_t)y0 * ((1u << Factor) - 1u);
				uint32_t r2 = (uint32_t)y0 * ((1u << Factor) - 1u);
				const uint8_t yRef = DemaRefStep<uint32_t, uint8_t, Factor>(r1, r2, (uint8_t)x);

				if (y != yRef)
				{
#if defined(ARDUINO)
					Serial.print(F("DemaU8<")); Serial.print(Factor); Serial.print(F("> one-step error: y0="));
					Serial.print((uint8_t)y0);
					Serial.print(F(" x=")); Serial.print((uint8_t)x);
					Serial.print(F(" got=")); Serial.print(y);
					Serial.print(F(" ref=")); Serial.println(yRef);
#endif
					errorCount++;
				}
			}
#if defined(ARDUINO)
			if ((y0 % 16) == 0)
			{
				Serial.print(F("DemaU8<")); Serial.print(Factor); Serial.print(F("> progress: y0="));
				Serial.println((uint8_t)y0);
			}
#endif
		}
#if defined(ARDUINO)
		if (errorCount == 0) Serial.println(F("All DemaU8 one-step exhaustive tests PASSED."));
		else { Serial.print(F("DemaU8 one-step exhaustive tests FAILED. Errors: ")); Serial.println(errorCount); }
#endif
		return errorCount == 0;
	}

	// Sampled U8 multi-step sequences vs integer DEMA reference.
	template<uint8_t Factor, uint32_t MaxIterations = 50000>
	static bool TestDemaU8Sample()
	{
		const uint32_t iterations = min<uint32_t>(MaxIterations, UINT16_MAX);
#if defined(ARDUINO)
		Serial.print(F("Starting sampled DemaU8<")); Serial.print(Factor); Serial.println(F("> tests..."));
#endif
		using F = IntegerSignal::Filters::DemaU8<Factor>;
		const uint8_t initialY[] = { 0, 1, 64, 127, 128, 200, 255 };

		uint32_t errorCount = 0;

		for (uint8_t y0 : initialY)
		{
			F f; f.Clear(y0);
			uint32_t r1 = (uint32_t)y0 * ((1u << Factor) - 1u);
			uint32_t r2 = r1;

			for (uint32_t i = 0; i <= iterations; i++)
			{
				const uint8_t x = (uint8_t)((i * UINT8_MAX) / (iterations ? iterations : 1));
				f.Set(x);
				f.Step();
				const uint8_t y = f.Get();

				const uint8_t yRef = DemaRefStep<uint32_t, uint8_t, Factor>(r1, r2, x);

				if (y != yRef)
				{
#if defined(ARDUINO)
					Serial.print(F("DemaU8<")); Serial.print(Factor); Serial.print(F("> sampled error: y0="));
					Serial.print(y0);
					Serial.print(F(" i=")); Serial.print(i);
					Serial.print(F(" x=")); Serial.print(x);
					Serial.print(F(" y=")); Serial.print(y);
					Serial.print(F(" ref=")); Serial.println(yRef);
#endif
					errorCount++;
					break;
				}
#if defined(ARDUINO)
				if ((i % (max<uint32_t>(1, iterations / 10))) == 0)
				{
					Serial.print(F("DemaU8<")); Serial.print(Factor); Serial.print(F("> progress: "));
					Serial.print(i); Serial.println(F(" samples..."));
				}
#endif
			}
		}
#if defined(ARDUINO)
		if (errorCount == 0) Serial.println(F("All DemaU8 sampled tests PASSED."));
		else { Serial.print(F("DemaU8 sampled tests FAILED. Errors: ")); Serial.println(errorCount); }
#endif
		return errorCount == 0;
	}

	// Sampled U16 multi-step sequences.
	template<uint8_t Factor, uint32_t MaxIterations = 10000>
	static bool TestDemaU16Sample()
	{
		const uint32_t iterations = min<uint32_t>(MaxIterations, (uint32_t)UINT16_MAX);
#if defined(ARDUINO)
		Serial.print(F("Starting sampled DemaU16<")); Serial.print(Factor); Serial.println(F("> tests..."));
#endif
		using F = IntegerSignal::Filters::DemaU16<Factor>;

		const uint16_t initialY[] = { 0u, 1u, 255u, 256u, 32767u, 32768u, 65535u };
		const uint16_t inputs[] = { 0u, 1u, 255u, 256u, 32767u, 32768u, 65535u };

		uint32_t errorCount = 0;

		for (uint16_t y0 : initialY)
		{
			for (uint16_t xEdge : inputs)
			{
				F f; f.Clear(y0);
				uint64_t r1 = (uint64_t)y0 * ((1u << Factor) - 1u);
				uint64_t r2 = r1;

				for (uint32_t i = 0; i <= iterations; i += max<uint32_t>(1, iterations / 64))
				{
					const uint16_t xSweep = (uint16_t)((i * UINT16_MAX) / (iterations ? iterations : 1));
					const uint16_t x = (uint16_t)((xSweep >> 1) + (xEdge >> 1));

					f.Set(x);
					f.Step();
					const uint16_t y = f.Get();

					const uint16_t yRef = DemaRefStep<uint64_t, uint16_t, Factor>(r1, r2, x);

					if (y != yRef)
					{
#if defined(ARDUINO)
						Serial.print(F("DemaU16<")); Serial.print(Factor); Serial.print(F("> sampled error: y0="));
						Serial.print(y0);
						Serial.print(F(" x=")); Serial.print(x);
						Serial.print(F(" y=")); Serial.print(y);
						Serial.print(F(" ref=")); Serial.println(yRef);
#endif
						errorCount++;
						break;
					}
				}
			}
		}
#if defined(ARDUINO)
		if (errorCount == 0) Serial.println(F("All DemaU16 sampled tests PASSED."));
		else { Serial.print(F("DemaU16 sampled tests FAILED. Errors: ")); Serial.println(errorCount); }
#endif
		return errorCount == 0;
	}

	// Sampled U32 multi-step sequences.
	template<uint8_t Factor, uint32_t MaxIterations = 5000>
	static bool TestDemaU32Sample()
	{
		const uint32_t iterations = min<uint32_t>(MaxIterations, (uint32_t)65536);
#if defined(ARDUINO)
		Serial.print(F("Starting sampled DemaU32<")); Serial.print(Factor); Serial.println(F("> tests..."));
#endif
		using F = IntegerSignal::Filters::DemaU32<Factor>;

		const uint32_t initialY[] = {
			0u, 1u, 255u, 256u, 65535u, 65536u, 2147483647u, 2147483648u, 4294967295u
		};
		const uint32_t inputs[] = {
			0u, 1u, 255u, 256u, 65535u, 65536u, 2147483647u, 2147483648u, 4294967295u
		};

		uint32_t errorCount = 0;

		for (uint32_t y0 : initialY)
		{
			for (uint32_t xEdge : inputs)
			{
				F f; f.Clear(y0);
				uint64_t r1 = (uint64_t)y0 * ((1u << Factor) - 1u);
				uint64_t r2 = r1;

				for (uint32_t i = 0; i <= iterations; i += max<uint32_t>(1, iterations / 128))
				{
					const uint32_t xSweep = (uint32_t)((uint64_t)i * 4294967295ULL / (iterations ? iterations : 1));
					const uint32_t x = (xSweep >> 1) + (xEdge >> 1);

					f.Set(x);
					f.Step();
					const uint32_t y = f.Get();

					const uint32_t yRef = DemaRefStep<uint64_t, uint32_t, Factor>(r1, r2, x);

					if (y != yRef)
					{
#if defined(ARDUINO)
						Serial.print(F("DemaU32<")); Serial.print(Factor); Serial.print(F("> sampled error: y0="));
						IntegerSignal::Testing::PrintUInt64(y0);
						Serial.print(F(" x=")); IntegerSignal::Testing::PrintUInt64(x);
						Serial.print(F(" y=")); IntegerSignal::Testing::PrintUInt64(y);
						Serial.print(F(" ref=")); IntegerSignal::Testing::PrintUInt64(yRef);
						Serial.println();
#endif
						errorCount++;
						break;
					}
				}
			}
		}
#if defined(ARDUINO)
		if (errorCount == 0) Serial.println(F("All DemaU32 sampled tests PASSED."));
		else { Serial.print(F("DemaU32 sampled tests FAILED. Errors: ")); Serial.println(errorCount); }
#endif
		return errorCount == 0;
	}

	// -----------------------------------------------------------------------
	// Parametric run groups
	// -----------------------------------------------------------------------

	template<uint32_t MaxIterations, uint8_t Tol>
	static bool RunDemaU8()
	{
		bool pass = true;

		// Exhaustive one-step for representative factors
		pass &= TestDemaU8ExhaustiveOneStep<1>();
		pass &= TestDemaU8ExhaustiveOneStep<2>();
		pass &= TestDemaU8ExhaustiveOneStep<4>();

		// Sampled
		pass &= TestDemaU8Sample<2, MaxIterations>();
		pass &= TestDemaU8Sample<4, (MaxIterations > 1 ? MaxIterations / 2 : 1)>();

		// Sanity: DC / monotonic / saturation / linearity (factor=2)
		{
			using F = IntegerSignal::Filters::DemaU8<2>;
			pass &= TestDCConvergence<F, uint8_t>(200, 2048);
			pass &= TestStepMonotonic<F, uint8_t>(0, 200, 2048, true);
			pass &= TestLinearity<F, uint8_t>(85, 3, Tol);
			pass &= TestSaturation<F, uint8_t>(UINT8_MAX, 4096);
		}
		return pass;
	}

	template<uint32_t MaxIterations, uint8_t Tol>
	static bool RunDemaI8()
	{
		using SF = IntegerSignal::Filters::DemaI8<2>;
		return TestSignedBehavior<SF, int8_t>(-64, 64, 2048);
	}

	template<uint32_t MaxIterations, uint8_t Tol>
	static bool RunDemaU16()
	{
		bool pass = true;

		// Sampled
		pass &= TestDemaU16Sample<8, (MaxIterations > 4 ? MaxIterations / 5 : 1)>();

		// Sanity
		{
			using F = IntegerSignal::Filters::DemaU16<8>;
			pass &= TestDCConvergence<F, uint16_t>(60000, 4096);
			pass &= TestStepMonotonic<F, uint16_t>(0, 60000, 4096, true);
			pass &= TestSaturation<F, uint16_t>(UINT16_MAX, 8192);
		}
		return pass;
	}

	template<uint32_t MaxIterations, uint8_t Tol>
	static bool RunDemaU32()
	{
		bool pass = true;

		// Sampled
		pass &= TestDemaU32Sample<8, (MaxIterations > 10 ? MaxIterations / 10 : 1)>();

		// Sanity
		{
			using F = IntegerSignal::Filters::DemaU32<8>;
			pass &= TestDCConvergence<F, uint32_t>(4294967295u, 8192);
			pass &= TestStepMonotonic<F, uint32_t>(0u, 4294967295u, 8192, true);
			pass &= TestSaturation<F, uint32_t>(UINT32_MAX, 8192);
		}
		return pass;
	}

	template<uint32_t MaxIterations, uint8_t Tol>
	static bool RunDemaI32()
	{
		using SF = IntegerSignal::Filters::DemaI32<4>;
		return TestSignedBehavior<SF, int32_t>(-1000000000, 1000000000, 8192);
	}

	// Main entry (templated like other suites)
	template<uint32_t MaxIterations = 50000, uint8_t LinearityTolerance = 0>
	static bool RunTests()
	{
#if defined(ARDUINO)
		Serial.println(F("Starting DEMA filter tests..."));
#endif
		bool pass = true;

		pass &= RunDemaU8<MaxIterations, LinearityTolerance>();
		pass &= RunDemaI8<MaxIterations, LinearityTolerance>();
		pass &= RunDemaU16<MaxIterations, LinearityTolerance>();
		pass &= RunDemaU32<MaxIterations, LinearityTolerance>();
		pass &= RunDemaI32<MaxIterations, LinearityTolerance>();

#if defined(ARDUINO)
		if (pass) Serial.println(F("DEMA filter tests PASSED."));
		else      Serial.println(F("DEMA filter tests FAILED."));
#endif
		return pass;
	}
}

#endif