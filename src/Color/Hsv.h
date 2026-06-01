#ifndef _INTEGER_SIGNAL_COLOR_HSV_h
#define _INTEGER_SIGNAL_COLOR_HSV_h

#include "../FixedPoint/ScalarFraction.h"

namespace IntegerSignal
{
	namespace Hsv
	{
		using namespace FixedPoint::ScalarFraction;

		/// <summary>
		/// Convert HSV to a color using Q-format fractional scalars.
		/// Uses UQ0.15 (ufraction16_t) for hue/saturation/value, enabling fast shift-based math.
		/// </summary>
		/// <typeparam name="color_t">Return type of colorFunc (e.g., a struct or packed RGB value).</typeparam>
		/// <typeparam name="component_t">Component storage type (e.g., uint8_t, uint16_t, or ufraction16_t).</typeparam>
		/// <typeparam name="ColorFunc">Callable taking (component_t r, component_t g, component_t b) and returning color_t.</typeparam>
		/// <typeparam name="COMPONENT_MAX">
		/// Max component value for scaling:
		/// - Use UFraction16::FRACTION_1X when components are fractional (component_t == ufraction16_t) for a no-rescale fast path.
		/// - Use an integer maximum (e.g., 255 for 8-bit) to scale fractional components to the target range.
		/// </typeparam>
		/// <param name="hue">
		/// ufraction16_t in [0, UFraction16::FRACTION_1X]. Value wraps at unit (i.e., unit == 0).
		/// Internally mapped into 6 equal segments (0..5) with linear interpolation inside the segment.
		/// </param>
		/// <param name="saturation">ufraction16_t in [0, UFraction16::FRACTION_1X].</param>
		/// <param name="value">ufraction16_t in [0, UFraction16::FRACTION_1X].</param>
		/// <param name="colorFunc">Callable to construct the final color from (r, g, b) components.</param>
		/// <returns>color_t constructed by colorFunc from the computed components.</returns>
		template<typename color_t,
			typename component_t,
			component_t COMPONENT_MAX,
			typename ColorFunc>
		inline color_t TemplateHsvFraction(
			const ufraction16_t hue, const ufraction16_t saturation, const ufraction16_t value,
			ColorFunc&& colorFunc)
		{
			// Fast path: if components are fractional (unit-scaled), stay in ufraction16_t end-to-end.
			static constexpr bool DirectComponents = COMPONENT_MAX == UFraction16::FRACTION_1X;

			// Hue is split into 6 equal segments with power-of-two unit scaling.
			static constexpr uint8_t Segments = 6;
			static constexpr uint32_t FullScale = uint32_t(Segments) * UFraction16::FRACTION_1X;

			// Achromatic (gray) shortcut
			if (saturation == 0)
			{
				if (DirectComponents)
				{
					return colorFunc(component_t(value), component_t(value), component_t(value));
				}

				const component_t valueScaled = Fraction(value, COMPONENT_MAX);
				return colorFunc(component_t(valueScaled), component_t(valueScaled), component_t(valueScaled));
			}

			// Scale hue to [0, 6*unit] for segment math and derive integer segment index [0..5].
			// Note: if hue == unit, hueSegment == 6 and wraps via (hueSegment % 6) to 0.
			const uint32_t hueScaled = Fraction(hue, FullScale);
			const uint8_t hueSegment = Fraction(hue, Segments);

			// Fractional position within the current segment in [0, unit).
			const ufraction16_t segmentHue = hueScaled - (uint32_t(hueSegment) * UFraction16::FRACTION_1X);
			const ufraction16_t oneMinusSegment = static_cast<ufraction16_t>(UFraction16::FRACTION_1X - segmentHue);

			// Interpolants (shared for both branches)
			const ufraction16_t t = Fraction(segmentHue, saturation);        // segmentHue * saturation
			const ufraction16_t u = Fraction(oneMinusSegment, saturation);  // (1 - segmentHue) * saturation

			if (DirectComponents) // All math in ufraction16_t, no component rescaling.
			{
				const ufraction16_t valueMinSaturation = value - Fraction(saturation, value);
				const ufraction16_t valueMinusT = value - Fraction(t, value);
				const ufraction16_t valueMinusU = value - Fraction(u, value);

				switch (hueSegment % Segments)
				{
				case 0: return colorFunc(component_t(value), component_t(valueMinusU), component_t(valueMinSaturation));
				case 1: return colorFunc(component_t(valueMinusT), component_t(value), component_t(valueMinSaturation));
				case 2: return colorFunc(component_t(valueMinSaturation), component_t(value), component_t(valueMinusU));
				case 3: return colorFunc(component_t(valueMinSaturation), component_t(valueMinusT), component_t(value));
				case 4: return colorFunc(component_t(valueMinusU), component_t(valueMinSaturation), component_t(value));
				case (Segments - 1):
				default: return colorFunc(component_t(value), component_t(valueMinSaturation), component_t(valueMinusT));
				}
			}
			else
			{
				// Math in component_t; components are scaled from ufraction16_t to [0, COMPONENT_MAX].
				const component_t valueComp = Fraction(value, COMPONENT_MAX);
				const component_t valueMinSaturation = valueComp - Fraction(saturation, valueComp);
				const component_t valueMinusT = valueComp - Fraction(t, valueComp);
				const component_t valueMinusU = valueComp - Fraction(u, valueComp);

				switch (hueSegment % Segments)
				{
				case 0:	return colorFunc(valueComp, valueMinusU, valueMinSaturation);
				case 1: return colorFunc(valueMinusT, valueComp, valueMinSaturation);
				case 2: return colorFunc(valueMinSaturation, valueComp, valueMinusU);
				case 3: return colorFunc(valueMinSaturation, valueMinusT, valueComp);
				case 4: return colorFunc(valueMinusU, valueMinSaturation, valueComp);
				case (Segments - 1):
				default: return colorFunc(valueComp, valueMinSaturation, valueMinusT);
				}
			}
		}
	}
}
#endif