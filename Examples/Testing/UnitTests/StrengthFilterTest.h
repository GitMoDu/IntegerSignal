#ifndef _STRENGTH_FILTER_TEST_h
#define _STRENGTH_FILTER_TEST_h

#include <IntegerSignalTesting.h>
#include <IntegerSignal.h>

namespace IntegerSignal::Filters::Strength::Test
{
	// ------------------------------------------------------------
	// Mock base filters for exact reference (stateless, deterministic)
	// ------------------------------------------------------------
	template<typename T, uint8_t Shift>
	struct ShiftDownFilter : public IntegerSignal::Filter::AbstractFilter<T>
	{
		using Base = IntegerSignal::Filter::AbstractFilter<T>;
		using Base::Input;

	private:
		T Out = 0;

	public:
		void Clear(const T value = 0) { Base::Clear(value); Out = value >> Shift; }
		void Step() { Out = (T)(Input >> Shift); }
		const T Get() const { return Out; }
	};

	// Reference Mix (use the same implementation as Strength)
	template<typename T, typename Wide>
	static inline T RefMix(const T input, const T raw, const uint8_t s)
	{
		return IntegerSignal::Mix::Template::Linear<T, Wide>(input, raw, s);
	}

	// ------------------------------------------------------------
	// Exhaustive U8 on mock base (ShiftDown = 1 and 2), various strengths
	// ------------------------------------------------------------
	template<uint8_t Shift, uint8_t StrengthScale>
	static bool TestU8ExhaustiveWithMock()
	{
		using Base = ShiftDownFilter<uint8_t, Shift>;
		using F = IntegerSignal::Filters::StrengthU8<Base, StrengthScale>;

		Serial.print(F("StrengthU8 exhaustive (mock) shift="));
		Serial.print(Shift);
		Serial.print(F(" strength="));
		Serial.println(StrengthScale);

		uint32_t errorCount = 0;

		F sf; Base base;
		sf.Clear();
		base.Clear();

		for (uint16_t v = 0; v <= UINT8_MAX; v++)
		{
			const uint8_t in = (uint8_t)v;

			// Base step
			base.Set(in);
			base.Step();
			const uint8_t raw = base.Get();

			// Strength step
			sf.Set(in);
			sf.Step();
			const uint8_t out = sf.Get();

			const uint8_t ref = RefMix<uint8_t, uint16_t>(in, raw, StrengthScale);
			if (out != ref)
			{
				Serial.print(F("StrengthU8 mock mismatch: in="));
				Serial.print(in);
				Serial.print(F(" raw="));
				Serial.print(raw);
				Serial.print(F(" out="));
				Serial.print(out);
				Serial.print(F(" ref="));
				Serial.println(ref);
				errorCount++;
			}
		}

		if (errorCount == 0)
			Serial.println(F("OK"));
		else
		{
			Serial.print(F("FAILED errors="));
			Serial.println(errorCount);
		}
		return errorCount == 0;
	}

	// ------------------------------------------------------------
	// Sampled tests against real base filters (run in lockstep)
	// ------------------------------------------------------------
	template<typename BaseFilterT, template<typename, uint8_t> class StrengthAlias, uint8_t StrengthScale, typename T, typename WideT, uint32_t MaxIterations>
	static bool TestSampledAgainstBase()
	{
		using SFilter = StrengthAlias<BaseFilterT, StrengthScale>;

		Serial.print(F("Strength sampled vs base: S="));
		Serial.print(StrengthScale);
		Serial.println();

		BaseFilterT base;
		SFilter sf;

		base.Clear(0);
		sf.Clear();

		uint32_t errorCount = 0;
		const uint32_t iters = (MaxIterations == 0) ? 1 : MaxIterations;

		for (uint32_t i = 0; i <= iters; i += (iters / 64) ? (iters / 64) : 1)
		{
			const T in = (T)((WideT)i * (WideT)TypeTraits::TypeLimits::type_limits<T>::MAX() / (WideT)iters);

			// base path
			base.Set(in);
			base.Step();
			const T raw = base.Get();

			// strength path
			sf.Set(in);
			sf.Step();
			const T out = sf.Get();

			const T ref = RefMix<T, WideT>(in, raw, StrengthScale);
			if (out != ref)
			{
				Serial.print(F("Strength mismatch: in="));
				IntegerSignal::Testing::PrintUInt64(in);
				Serial.print(F(" raw="));
				IntegerSignal::Testing::PrintUInt64(raw);
				Serial.print(F(" out="));
				IntegerSignal::Testing::PrintUInt64(out);
				Serial.print(F(" ref="));
				IntegerSignal::Testing::PrintUInt64(ref);
				Serial.println();
				errorCount++;
				break;
			}
		}

		if (errorCount == 0) Serial.println(F("OK"));
		else { Serial.print(F("FAILED errors=")); Serial.println(errorCount); }

		return errorCount == 0;
	}

	// ------------------------------------------------------------
	// Edge strengths on real bases: 0 -> pass-through input; 255 -> pure base output
	// ------------------------------------------------------------
	template<typename BaseFilterT, typename T, uint32_t MaxIterations>
	static bool TestEdgeStrengths()
	{
		bool pass = true;

		// S=0: Output == Input
		{
			using S0 = IntegerSignal::Filters::StrengthU8<BaseFilterT, 0>;
			BaseFilterT base; S0 sf; base.Clear(0); sf.Clear();
			for (uint32_t i = 0; i <= MaxIterations; i += (MaxIterations / 64) ? (MaxIterations / 64) : 1)
			{
				const T in = (T)(i & TypeTraits::TypeLimits::type_limits<T>::MAX());
				base.Set(in); base.Step();
				sf.Set(in); sf.Step();
				if (sf.Get() != in)
				{
					Serial.println(F("Edge S=0 failed"));
					pass = false; break;
				}
			}
		}

		// S=255: Output == Base.Get()
		if (pass)
		{
			using S255 = IntegerSignal::Filters::StrengthU8<BaseFilterT, 255>;
			BaseFilterT base; S255 sf; base.Clear(0); sf.Clear();
			for (uint32_t i = 0; i <= MaxIterations; i += (MaxIterations / 64) ? (MaxIterations / 64) : 1)
			{
				const T in = (T)(i & TypeTraits::TypeLimits::type_limits<T>::MAX());
				base.Set(in); base.Step();
				sf.Set(in); sf.Step();
				if (sf.Get() != base.Get())
				{
					Serial.println(F("Edge S=255 failed"));
					pass = false; break;
				}
			}
		}

		return pass;
	}

	// ------------------------------------------------------------
	// Run groups
	// ------------------------------------------------------------
	template<uint32_t MaxIterations, uint8_t DummyTol>
	static bool RunU8()
	{
		bool pass = true;

		// Exhaustive with mock base
		pass &= TestU8ExhaustiveWithMock<1, 0>();    // pass-through input
		pass &= TestU8ExhaustiveWithMock<1, 255>();  // pure base (in>>1)
		pass &= TestU8ExhaustiveWithMock<1, 128>();  // ~50/50
		pass &= TestU8ExhaustiveWithMock<2, 192>();  // biased

		// Real bases sampled vs base (LowPass<4>, EMA<4>, DEMA<2>)
		pass &= TestSampledAgainstBase<IntegerSignal::Filters::LowPassU8<4>, IntegerSignal::Filters::StrengthU8, 128, uint8_t, uint16_t, MaxIterations>();
		pass &= TestSampledAgainstBase<IntegerSignal::Filters::EmaU8<4>, IntegerSignal::Filters::StrengthU8, 200, uint8_t, uint16_t, MaxIterations>();
		pass &= TestSampledAgainstBase<IntegerSignal::Filters::DemaU8<2>, IntegerSignal::Filters::StrengthU8, 64, uint8_t, uint16_t, MaxIterations>();

		// Edge strengths on a real base
		pass &= TestEdgeStrengths<IntegerSignal::Filters::LowPassU8<4>, uint8_t, MaxIterations>();

		return pass;
	}

	template<uint32_t MaxIterations, uint8_t DummyTol>
	static bool RunU16()
	{
		bool pass = true;

		// Real bases sampled vs base
		pass &= TestSampledAgainstBase<IntegerSignal::Filters::LowPassU16<8>, IntegerSignal::Filters::StrengthU16, 128, uint16_t, uint32_t, MaxIterations / 5>();
		pass &= TestSampledAgainstBase<IntegerSignal::Filters::EmaU16<8>, IntegerSignal::Filters::StrengthU16, 200, uint16_t, uint32_t, MaxIterations / 5>();
		pass &= TestSampledAgainstBase<IntegerSignal::Filters::DemaU16<8>, IntegerSignal::Filters::StrengthU16, 64, uint16_t, uint32_t, MaxIterations / 5>();

		return pass;
	}

	template<uint32_t MaxIterations, uint8_t DummyTol>
	static bool RunU32()
	{
		bool pass = true;

		// Real bases sampled vs base
		pass &= TestSampledAgainstBase<IntegerSignal::Filters::LowPassU32<8>, IntegerSignal::Filters::StrengthU32, 128, uint32_t, uint64_t, MaxIterations / 10>();
		pass &= TestSampledAgainstBase<IntegerSignal::Filters::EmaU32<8>, IntegerSignal::Filters::StrengthU32, 200, uint32_t, uint64_t, MaxIterations / 10>();
		pass &= TestSampledAgainstBase<IntegerSignal::Filters::DemaU32<8>, IntegerSignal::Filters::StrengthU32, 64, uint32_t, uint64_t, MaxIterations / 10>();

		return pass;
	}

	// Main entry
	template<uint32_t MaxIterations = 50000, uint8_t DummyTol = 0>
	static bool RunTests()
	{
		Serial.println(F("Starting Strength meta-filter tests..."));
		bool pass = true;

		pass &= RunU8<MaxIterations, DummyTol>();
		pass &= RunU16<MaxIterations, DummyTol>();
		pass &= RunU32<MaxIterations, DummyTol>();

		if (pass) Serial.println(F("Strength meta-filter tests PASSED."));
		else      Serial.println(F("Strength meta-filter tests FAILED."));
		return pass;
	}
}

#endif