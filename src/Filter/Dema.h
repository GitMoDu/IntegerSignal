#ifndef _INTEGER_SIGNAL_DEMA_FILTER_h
#define _INTEGER_SIGNAL_DEMA_FILTER_h

#include "IFilter.h"

namespace IntegerSignal
{
	namespace Filters
	{
		namespace Dema
		{
			namespace Template
			{
				/// <summary>
				/// Double Exponential Moving Average filter.
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
					intermediate_t HighValue1 = 0;
					intermediate_t HighValue2 = 0;

					unsigned_t Output = 0;

				public:
					Filter() : Base() {}

					virtual void Clear(const unsigned_t value = 0)
					{
						Base::Clear(value);

						// Set both stages of the filter to produce 'value' as output
						Output = value;

						// In a DEMA filter, we need both HighValue1 and HighValue2 to be in a steady state.
						HighValue2 = value << factor;
						HighValue1 = value << factor;
					}

					virtual void Step()
					{
						HighValue1 += Input;
						const unsigned_t intermediate = (HighValue1 + Half) >> factor;
						HighValue1 -= intermediate;

						HighValue2 += intermediate;
						Output = (HighValue2 + Half) >> factor;
						HighValue2 -= Output;
					}

					virtual const unsigned_t Get() const
					{
						return Output;
					}
				};
			}
		}

		template<uint8_t factor = 2>
		using DemaU8 = Dema::Template::Filter<uint8_t, uint_fast16_t, factor>;

		template<uint8_t factor = 2>
		using DemaU16 = Dema::Template::Filter<uint16_t, uint_fast32_t, factor>;

		template<uint8_t factor = 2>
		using DemaU32 = Dema::Template::Filter<uint32_t, uint_fast64_t, factor>;

		template<uint8_t factor = 2>
		using DemaI8 = Filter::SignedI8<DemaU8<factor>>;

		template<uint8_t factor = 2>
		using DemaI16 = Filter::SignedI16<DemaU16<factor>>;

		template<uint8_t factor = 2>
		using DemaI32 = Filter::SignedI32<DemaU32<factor>>;
	}
}
#endif