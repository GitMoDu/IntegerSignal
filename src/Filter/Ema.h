#ifndef _INTEGER_SIGNAL_EMA_FILTER_h
#define _INTEGER_SIGNAL_EMA_FILTER_h

#include "IFilter.h"

namespace IntegerSignal
{
	namespace Filters
	{
		namespace Ema
		{
			namespace Template
			{
				/// <summary>
				/// Exponential Moving Average filter.
				/// </summary>
				/// <typeparam name="unsigned_t"></typeparam>
				/// <typeparam name="intermediate_t"></typeparam>
				/// <typeparam name="factor"></typeparam>
				template<typename unsigned_t,
					typename intermediate_t,
					uint8_t factor>
				class Filter : public IntegerSignal::Filter::AbstractFilter<unsigned_t>
				{
				private:
					using Base = IntegerSignal::Filter::AbstractFilter<unsigned_t>;

				public:
					static constexpr uint8_t MaxFactor = sizeof(unsigned_t) * 8;
					static constexpr unsigned_t Half = 1 << (factor - 1);

				protected:
					using Base::Input;

				private:
					intermediate_t HighValue = 0;
					unsigned_t Output = 0;

				public:
					Filter() : Base() {}

					virtual void Clear(const unsigned_t value = 0)
					{
						Base::Clear(value);
						HighValue = value;
						Output = (HighValue + Half) >> factor;
						HighValue -= Output;
					}

					virtual void Step()
					{
						HighValue += Input;
						Output = (HighValue + Half) >> factor;
						HighValue -= Output;
					}

					virtual const unsigned_t Get() const
					{
						return Output;
					}
				};
			}
		}

		template<uint8_t factor = 4>
		using EmaU8 = Ema::Template::Filter<uint8_t, uint_fast16_t, factor>;

		template<uint8_t factor = 4>
		using EmaU16 = Ema::Template::Filter<uint16_t, uint_fast32_t, factor>;

		template<uint8_t factor = 4>
		using EmaU32 = Ema::Template::Filter<uint32_t, uint_fast64_t, factor>;

		template<uint8_t factor = 4>
		using EmaI8 = Filter::SignedI8<EmaU8<factor>>;

		template<uint8_t factor = 4>
		using EmaI16 = Filter::SignedI16<EmaU16<factor>>;

		template<uint8_t factor = 4>
		using EmaI32 = Filter::SignedI32<EmaU32<factor>>;
	}
}
#endif