// Root.h

#ifndef _INTEGER_SIGNAL_ROOT_CURVE_h
#define _INTEGER_SIGNAL_ROOT_CURVE_h

#include "ICurve.h"
#include "../Base/SquareRoot.h"
#include "../Base/Convert.h"

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
			return SquareRoot16(U8ToU16(BaseCurve::Get(input)));
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
			return SquareRoot32(U16ToU32(BaseCurve::Get(input)));
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
			return SquareRoot64(U32ToU64(BaseCurve::Get(input)));
		}
	};
}

#endif