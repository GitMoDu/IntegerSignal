// Strength.h

#ifndef _INTEGER_SIGNAL_STRENGTH_CURVE_h
#define _INTEGER_SIGNAL_STRENGTH_CURVE_h

#include "ICurve.h"
#include "../Base/Mix.h"

namespace IntegerSignal
{
	namespace Curves
	{
		namespace Template
		{
			/// <summary>
			/// Scales the base curve strength, between the input and raw output.
			/// </summary>
			/// <typeparam name="CurveType">Unsigned value ICurve type.</typeparam>
			/// <typeparam name="filterStrength">Strength scale, 0 is zero filtering: [0;UINT8_MAX]</typeparam>
			template<typename CurveType,
				typename value_t,
				typename intermediate_t,
				const uint8_t curveStrength = UINT8_MAX>
			class Strength : public CurveType
			{
			public:
				Strength() : CurveType() {}

				virtual const uint8_t Get(const uint8_t input) const
				{
					return Mix::Template::Linear<value_t, intermediate_t>(input, CurveType::Get(input), curveStrength);
				}
			};
		}

		/// <summary>
		/// Scales the base curve strength, between the input and raw output.
		/// </summary>
		/// <typeparam name="CurveType">Unsigned value ICurve type.</typeparam>
		/// <typeparam name="filterStrength">Strength scale, 0 is zero filtering: [0;UINT8_MAX]</typeparam>
		template<typename CurveType,
			const uint8_t curveStrength = UINT8_MAX>
		using StrengthU8 = Template::Strength<CurveType, uint8_t, uint16_t, curveStrength>;

		/// <summary>
		/// Scales the base curve strength, between the input and raw output.
		/// </summary>
		/// <typeparam name="CurveType">Unsigned value ICurve type.</typeparam>
		/// <typeparam name="filterStrength">Strength scale, 0 is zero filtering: [0;UINT8_MAX]</typeparam>
		template<typename CurveType,
			const uint8_t curveStrength = UINT8_MAX>
		using StrengthU16 = Template::Strength<CurveType, uint16_t, uint32_t, curveStrength>;

		/// <summary>
		/// Scales the base curve strength, between the input and raw output.
		/// </summary>
		/// <typeparam name="CurveType">Unsigned value ICurve type.</typeparam>
		/// <typeparam name="filterStrength">Strength scale, 0 is zero filtering: [0;UINT8_MAX]</typeparam>
		template<typename CurveType,
			const uint8_t curveStrength = UINT8_MAX>
		using StrengthU32 = Template::Strength<CurveType, uint32_t, uint64_t, curveStrength>;
	}
}

#endif