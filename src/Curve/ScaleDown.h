// ScaleDown.h

#ifndef _INTEGER_SIGNAL_SCALE_DOWN_CURVE_h
#define _INTEGER_SIGNAL_SCALE_DOWN_CURVE_h

#include "ICurve.h"

namespace IntegerSignal::Curves
{
	namespace ScaleDown::Template
	{
		/// <summary>
		/// Scales down a unsigned value by a fixed proportion.
		/// </summary>
		/// <typeparam name="value_t">Curve value type.</typeparam>
		/// <typeparam name="intermediate_t">Curve intermediate type for scaling operations. Usually the higher unsigned type (uint8_t -> uint16_t).</typeparam>
		/// <typeparam name="scale">How much of the original signal is retained, abstract scale [0 ; UINT8_MAX].</typeparam>
		/// <typeparam name="BaseCurve">Base curve for chaining.</typeparam>
		template<typename value_t,
			typename intermediate_t,
			uint8_t scale,
			typename BaseCurve = Curves::Template::Curve<value_t>>
			struct Curve
		{
			static const value_t Get(const value_t input)
			{
				return ((intermediate_t)BaseCurve::Get(input) * scale) / UINT8_MAX;
			}
		};
	}

	/// <summary>
	/// Scales down a unsigned value by a fixed proportion.
	/// </summary>
	/// <typeparam name="scale">How much of the original signal is retained, abstract scale [0 ; UINT8_MAX].</typeparam>
	/// <typeparam name="BaseCurve">Base curve for chaining.</typeparam>
	template<uint8_t scale = UINT8_MAX, typename BaseCurve = Curves::Template::Curve<uint8_t>>
	using ScaleDownU8 = ScaleDown::Template::Curve<uint8_t, uint16_t, scale, BaseCurve>;

	/// <summary>
	/// Scales down a unsigned value by a fixed proportion.
	/// </summary>
	/// <typeparam name="scale">How much of the original signal is retained, abstract scale [0 ; UINT8_MAX].</typeparam>
	/// <typeparam name="BaseCurve">Base curve for chaining.</typeparam>
	template<uint8_t scale = UINT8_MAX, typename BaseCurve = Curves::Template::Curve<uint16_t>>
	using ScaleDownU16 = ScaleDown::Template::Curve<uint16_t, uint32_t, scale, BaseCurve>;

	/// <summary>
	/// Scales down a unsigned value by a fixed proportion.
	/// </summary>
	/// <typeparam name="scale">How much of the original signal is retained, abstract scale [0 ; UINT8_MAX].</typeparam>
	/// <typeparam name="BaseCurve">Base curve for chaining.</typeparam>
	template<uint8_t scale = UINT8_MAX, typename BaseCurve = Curves::Template::Curve<uint32_t>>
	using ScaleDownU32 = ScaleDown::Template::Curve<uint32_t, uint64_t, scale, BaseCurve>;
}

#endif