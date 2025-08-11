#ifndef _INTEGER_SIGNAL_LOW_PASS_FILTER_h
#define _INTEGER_SIGNAL_LOW_PASS_FILTER_h

#include "IFilter.h"

namespace IntegerSignal
{
	namespace Filters
	{
		namespace LowPass
		{
			namespace Template
			{
				/// <summary>
				/// Inspired by https://hackaday.io/project/158959-integer-low-pass-filter/log/147712-filtering-without-multiplication .
				/// </summary>
				/// <typeparam name="unsigned_t"></typeparam>
				/// <typeparam name="intermediate_t"></typeparam>
				/// <typeparam name="factor">[0 ; MaxFactor]</typeparam>
				template<typename unsigned_t,
					typename intermediate_t,
					uint8_t factor>
				class Filter : public IntegerSignal::Filter::AbstractFilter<unsigned_t>
				{
				private:
					using Base = IntegerSignal::Filter::AbstractFilter<unsigned_t>;

				public:
					static constexpr uint8_t MaxFactor = sizeof(unsigned_t) * 8;

				protected:
					using Base::Input;

				private:
					intermediate_t HighValue = 0;

				public:
					Filter() : Base() {}

					virtual void Clear(const unsigned_t value = 0)
					{
						Base::Clear(value);
						HighValue = intermediate_t(value) << factor;
					}

					virtual void Step()
					{
						HighValue = HighValue - (HighValue >> factor) + Input;
					}

					virtual const unsigned_t Get() const
					{
						// Fast operation, no need to store the raw output.
						return (unsigned_t)(HighValue >> factor);
					}
				};
			}
		}

		/// <summary>
		/// Low Pass Filter (unsigned 8 bit).
		/// </summary>
		/// <typeparam name="Factor">[0 ; 8]</typeparam>
		template<const uint8_t Factor = 4>
		using LowPassU8 = LowPass::Template::Filter<uint8_t, uint_fast16_t, Factor>;

		/// <summary>
		/// Low Pass Filter (unsigned 16 bit).
		/// </summary>
		/// <typeparam name="Factor">[0 ; 16]</typeparam>
		template<const uint8_t Factor = 4>
		using LowPassU16 = LowPass::Template::Filter<uint16_t, uint_fast32_t, Factor>;

		/// <summary>
		/// Low Pass Filter (unsigned 32 bit).
		/// </summary>
		/// <typeparam name="Factor">[0 ; 32]</typeparam>
		template<const uint8_t Factor = 4>
		using LowPassU32 = LowPass::Template::Filter<uint32_t, uint_fast64_t, Factor>;

		/// <summary>
		/// Low Pass Filter (signed 8 bit).
		/// </summary>
		/// <typeparam name="Factor">[0 ; 8]</typeparam>
		template<uint8_t factor = 4>
		using LowPassI8 = Filter::SignedI8<LowPassU8<factor>>;

		/// <summary>
		/// Low Pass Filter (signed 16 bit).
		/// </summary>
		/// <typeparam name="Factor">[0 ; 16]</typeparam>
		template<uint8_t factor = 4>
		using LowPassI16 = Filter::SignedI16<LowPassU16<factor>>;

		/// <summary>
		/// Low Pass Filter (signed 32 bit).
		/// </summary>
		/// <typeparam name="Factor">[0 ; 32]</typeparam>
		template<uint8_t factor = 4>
		using LowPassI32 = Filter::SignedI32<LowPassU32<factor>>;
	}
}
#endif