// Signed.h

#ifndef _INTEGER_SIGNAL_SINED_FILTER_h
#define _INTEGER_SIGNAL_SINED_FILTER_h

#include "IFilter.h"

namespace IntegerSignal
{
	namespace Filter
	{
		namespace Template
		{
			/// <summary>
			/// Wraps an unsigned IFilter with a signed input/output.
			/// </summary>
			/// <typeparam name="FilterType">Unsigned value IFilter type.</typeparam>
			/// <typeparam name="signed_t">The matching signed type, of the same bitwidth.</typeparam>
			template<typename FilterType,
				typename unsigned_t,
				typename signed_t>
			class Signed : public IFilter<signed_t>
			{
			private:
				static constexpr unsigned_t Middle()
				{
					return (((INT8_MAX * (sizeof(unsigned_t) == sizeof(uint8_t)))
						| (INT16_MAX * (sizeof(unsigned_t) == sizeof(uint16_t)))
						| (INT32_MAX * (sizeof(unsigned_t) == sizeof(uint32_t)))
						| (INT64_MAX * (sizeof(unsigned_t) == sizeof(uint64_t)))));
				}

			private:
				FilterType Signal{};

			public:
				Signed() : IFilter<signed_t>() {}

				virtual void Clear()
				{
					Signal.Clear(Middle());
				}

				virtual void Set(const signed_t value)
				{
					Signal.Set((unsigned_t)(Middle() + value));
				}

				virtual void Step()
				{
					Signal.Step();
				}

				virtual const signed_t Get() const
				{
					return (signed_t)(Signal.Get()) - Middle();
				}
			};
		}

		template<typename FilterType>
		using SignedI8 = Template::Signed<FilterType, uint8_t, int8_t>;

		template<typename FilterType>
		using SignedI16 = Template::Signed<FilterType, uint16_t, int16_t>;

		template<typename FilterType>
		using SignedI32 = Template::Signed<FilterType, uint32_t, int32_t>;
	}
}

#endif