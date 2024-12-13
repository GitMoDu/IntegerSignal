// Limit.h

#ifndef _INTEGER_SIGNAL_LIMIT_CURVE_h
#define _INTEGER_SIGNAL_LIMIT_CURVE_h

#include "ICurve.h"

namespace IntegerSignal::Curves
{
	namespace Limit::Template
	{
		/// <summary>
		/// Limits the output to a floor and ceiling,
		/// without destroying the raw output value.
		/// </summary>
		/// <typeparam name="value_t">Curve value type</typeparam>
		/// <typeparam name="floor">Lowest output allowed.</typeparam>
		/// <typeparam name="ceiling">Highest output allowed.</typeparam>
		/// <typeparam name="BaseCurve">Base curve for chaining.</typeparam>
		template<typename value_t,
			value_t floor,
			value_t ceiling,
			typename BaseCurve = IntegerSignal::Curve::ICurve<value_t>>
			struct Curve : public BaseCurve
		{
		public:
			Curve() : BaseCurve() {}

			virtual const value_t Get(const value_t input) const
			{
				const value_t out = BaseCurve::Get(input);
				if (out >= ceiling)
				{
					return ceiling;
				}
				else if (out <= floor)
				{
					return floor;
				}
				else
				{
					return out;
				}
			}
		};
	}

	/// <summary>
	/// Limits the output to a floor and ceiling,
	/// without destroying the raw output value.
	/// </summary>
	/// <typeparam name="floor">Lowest output allowed.</typeparam>
	/// <typeparam name="ceiling">Highest output allowed.</typeparam>
	/// <typeparam name="BaseCurve">Base curve for chaining.</typeparam>
	template<uint8_t floor, uint8_t ceiling, typename BaseCurve = Curve::ICurve<uint8_t>>
	using LimitU8 = Limit::Template::Curve<uint8_t, floor, ceiling, BaseCurve>;

	/// <summary>
	/// Limits the output to a floor and ceiling,
	/// without destroying the raw output value.
	/// </summary>
	/// <typeparam name="floor">Lowest output allowed.</typeparam>
	/// <typeparam name="ceiling">Highest output allowed.</typeparam>
	/// <typeparam name="BaseCurve">Base curve for chaining.</typeparam>
	template<uint16_t floor, uint16_t ceiling, typename BaseCurve = Curve::ICurve<uint16_t>>
	using LimitU16 = Limit::Template::Curve<uint16_t, floor, ceiling, BaseCurve>;

	/// <summary>
	/// Limits the output to a floor and ceiling,
	/// without destroying the raw output value.
	/// </summary>
	/// <typeparam name="floor">Lowest output allowed.</typeparam>
	/// <typeparam name="ceiling">Highest output allowed.</typeparam>
	/// <typeparam name="BaseCurve">Base curve for chaining.</typeparam>
	template<uint32_t floor, uint32_t ceiling, typename BaseCurve = Curve::ICurve<uint32_t>>
	using LimitU32 = Limit::Template::Curve<uint32_t, floor, ceiling, BaseCurve>;
}

#endif