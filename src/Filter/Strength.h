// Strength.h

#ifndef _INTEGER_SIGNAL_STRENGTH_FILTER_h
#define _INTEGER_SIGNAL_STRENGTH_FILTER_h

#include "IFilter.h"

namespace IntegerSignal::Filter
{
	namespace Template
	{
		/// <summary>
		/// Scales the base filter strength, between the input and raw output.
		/// </summary>
		/// <typeparam name="FilterType">Base filter type.</typeparam>
		/// <typeparam name="unsigned_t">Filter value type.</typeparam>
		/// <typeparam name="intermediate_t">Filter intermediate type for scaling operations. Usually the higher unsigned type (uint8_t -> uint16_t).</typeparam>
		/// <typeparam name="filterStrength">Strength scale, 0 is zero filtering: [0;UINT8_MAX]</typeparam>
		template<typename FilterType,
			typename unsigned_t,
			typename intermediate_t,
			const uint8_t filterStrength = UINT8_MAX>
		class Strength : public FilterType
		{
		protected:
			using FilterType::Input;

		private:
			unsigned_t Output = 0;

		public:
			Strength() : FilterType() {}

			virtual void Clear()
			{
				FilterType::Clear();
				Output = 0;
			}

			virtual void Step()
			{
				FilterType::Step();
				Output = Mix::Template::Linear<unsigned_t, intermediate_t>(Input, FilterType::Get(), filterStrength);
			}

			virtual const unsigned_t Get() const
			{
				return Output;
			}
		};
	}

	template<typename FilterType,
		const uint8_t filterStrength = UINT8_MAX>
	using StrengthU8 = Template::Strength<FilterType, uint8_t, uint16_t, filterStrength>;

	template<typename FilterType,
		const uint8_t filterStrength = UINT8_MAX>
	using StrengthU16 = Template::Strength<FilterType, uint16_t, uint32_t, filterStrength>;

	template<typename FilterType,
		const uint8_t filterStrength = UINT8_MAX>
	using StrengthU32 = Template::Strength<FilterType, uint32_t, uint64_t, filterStrength>;
}

#endif