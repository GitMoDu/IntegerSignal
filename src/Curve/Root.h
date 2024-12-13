// Root.h

#ifndef _INTEGER_SIGNAL_ROOT_CURVE_h
#define _INTEGER_SIGNAL_ROOT_CURVE_h

#include "ICurve.h"
#include "../Common/Math.h"
#include "../Common/Scale.h"

namespace IntegerSignal::Curves
{
	/// <summary>
	/// Root^2 curves the input.
	/// </summary>
	/// <typeparam name="BaseCurve">Base curve for chaining.</typeparam>
	template<typename BaseCurve = Curve::ICurve<uint8_t>>
	struct Root2U8 : public BaseCurve
	{
		Root2U8() : BaseCurve() {}

		virtual const uint8_t Get(const uint8_t input) const
		{
			return IntegerSignal::Math::Root2U16(IntegerSignal::Scale::U8ToU16(BaseCurve::Get(input)));
		}
	};

	/// <summary>
	/// Root^2 curves the input.
	/// </summary>
	/// <typeparam name="BaseCurve">Base curve for chaining.</typeparam>
	template<typename BaseCurve = Curve::ICurve<uint16_t>>
	struct Root2U16 : public BaseCurve
	{
		Root2U16() : BaseCurve() {}

		virtual const uint16_t Get(const uint16_t input) const
		{
			return IntegerSignal::Math::Root2U32(IntegerSignal::Scale::U16ToU32(BaseCurve::Get(input)));
		}
	};

	/// <summary>
	/// Root^2 curves the input.
	/// </summary>
	/// <typeparam name="BaseCurve">Base curve for chaining.</typeparam>
	template<typename BaseCurve = Curve::ICurve<uint32_t>>
	struct Root2U32 : public BaseCurve
	{
		Root2U32() : BaseCurve() {}

		virtual const uint32_t Get(const uint32_t input) const
		{
			return IntegerSignal::Math::Root2U64(IntegerSignal::Scale::U32ToU64(BaseCurve::Get(input)));
		}
	};
}

#endif