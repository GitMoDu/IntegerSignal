// Power.h

#ifndef _INTEGER_SIGNAL_POWER_CURVE_h
#define _INTEGER_SIGNAL_POWER_CURVE_h

#include "ICurve.h"

namespace IntegerSignal::Curves
{
	namespace Power::Template
	{
		/// <summary>
		/// Power^2 curves the input.
		/// </summary>
		/// <typeparam name="value_t">Curve value type.</typeparam>
		/// <typeparam name="intermediate_t">Curve intermediate type for scaling operations. Usually the higher unsigned type (uint8_t -> uint16_t).</typeparam>
		/// <typeparam name="BaseCurve">Base curve for chaining.</typeparam>
		template<typename value_t,
			typename intermediate_t,
			typename BaseCurve = Curve::ICurve<value_t>>
			struct Curve2 : public BaseCurve
		{
		public:
			Curve2() : BaseCurve() {}

			virtual const value_t Get(const value_t input) const
			{
				const value_t in = BaseCurve::Get(input);

				return (((intermediate_t)in * in) + in) >> (sizeof(value_t) * 8);
			}
		};

		/// <summary>
		/// Power^3 curves the input.
		/// </summary>
		/// <typeparam name="value_t">Curve value type.</typeparam>
		/// <typeparam name="intermediate_t">Curve intermediate type for scaling operations. Usually the higher unsigned type (uint8_t -> uint16_t).</typeparam>
		/// <typeparam name="BaseCurve">Base curve for chaining.</typeparam>
		template<typename value_t,
			typename intermediate_t,
			typename BaseCurve = Curve::ICurve<value_t>>
			struct Curve3 : public BaseCurve
		{
		public:
			Curve3() : BaseCurve() {}

			virtual const value_t Get(const value_t input) const
			{
				const value_t in = BaseCurve::Get(input);
				const intermediate_t power = (intermediate_t)in * in >> (sizeof(value_t) * 8);

				return (((power)*in) + (power + ((intermediate_t)in + 1))) >> (sizeof(value_t) * 8);
			}
		};
	}

	/// <summary>
	/// Power^2 curves the input.
	/// </summary>
	/// <typeparam name="BaseCurve">Base curve for chaining.</typeparam>
	template<typename BaseCurve = Curve::ICurve<uint8_t>>
	using Power2U8 = Power::Template::Curve2<uint8_t, uint16_t, BaseCurve>;

	/// <summary>
	/// Power^2 curves the input.
	/// </summary>
	/// <typeparam name="BaseCurve">Base curve for chaining.</typeparam>
	template<typename BaseCurve = Curve::ICurve<uint16_t>>
	using Power2U16 = Power::Template::Curve2<uint16_t, uint32_t, BaseCurve>;

	/// <summary>
	/// Power^2 curves the input.
	/// </summary>
	/// <typeparam name="BaseCurve">Base curve for chaining.</typeparam>
	template<typename BaseCurve = Curve::ICurve<uint32_t>>
	using Power2U32 = Power::Template::Curve2<uint32_t, uint64_t, BaseCurve>;

	/// <summary>
	/// Power^3 curves the input.
	/// </summary>
	/// <typeparam name="BaseCurve">Base curve for chaining.</typeparam>
	template<typename BaseCurve = Curve::ICurve<uint8_t>>
	using Power3U8 = Power::Template::Curve3<uint8_t, uint16_t, BaseCurve>;

	/// <summary>
	/// Power^3 curves the input.
	/// </summary>
	/// <typeparam name="BaseCurve">Base curve for chaining.</typeparam>
	template<typename BaseCurve = Curve::ICurve<uint16_t>>
	using Power3U16 = Power::Template::Curve3<uint16_t, uint32_t, BaseCurve>;

	/// <summary>
	/// Power^3 curves the input.
	/// </summary>
	/// <typeparam name="BaseCurve">Base curve for chaining.</typeparam>
	template<typename BaseCurve = Curve::ICurve<uint32_t>>
	using Power3U32 = Power::Template::Curve3<uint32_t, uint64_t, BaseCurve>;
}

#endif