// Root.h

#ifndef _INTEGER_SIGNAL_ROOT_CURVE_h
#define _INTEGER_SIGNAL_ROOT_CURVE_h

#include "ICurve.h"
#include "../Common/Math.h"
#include "../Common/BitScale.h"

namespace IntegerSignal::Curves
{
	/// <summary>
	/// Root^2 curves the input.
	/// </summary>
	/// <typeparam name="BaseCurve">Base curve for chaining.</typeparam>
	template<typename BaseCurve = Curves::Template::Curve<uint8_t>>
	struct Root2U8
	{
		static const uint8_t Get(const uint8_t input)
		{
			return IntegerSignal::Math::Root2U16(IntegerSignal::BitScale::U8ToU16(BaseCurve::Get(input)));
		}
	};

	/// <summary>
	/// Root^2 curves the input.
	/// </summary>
	/// <typeparam name="BaseCurve">Base curve for chaining.</typeparam>
	template<typename BaseCurve = Curves::Template::Curve<uint16_t>>
	struct Root2U16
	{
		static const uint16_t Get(const uint16_t input)
		{
			return IntegerSignal::Math::Root2U32(IntegerSignal::BitScale::U16ToU32(BaseCurve::Get(input)));
		}
	};

	/// <summary>
	/// Root^2 curves the input.
	/// </summary>
	/// <typeparam name="BaseCurve">Base curve for chaining.</typeparam>
	template<typename BaseCurve = Curves::Template::Curve<uint32_t>>
	struct Root2U32
	{
		static const uint32_t Get(const uint32_t input)
		{
			return IntegerSignal::Math::Root2U64(IntegerSignal::BitScale::U32ToU64(BaseCurve::Get(input)));
		}
	};
}

#endif