#ifndef _INTEGER_SIGNAL_COLOR_HSV_h
#define _INTEGER_SIGNAL_COLOR_HSV_h

#include "../FixedPoint/Fraction.h"

namespace IntegerSignal
{
	namespace Hsv
	{
		using namespace FixedPoint::Fraction;

		// Fraction type for HSV components.
		using fraction_t = UFraction16::scalar_t;

		/// <summary>
		/// Convert HSV to a color using fixed-point fractions.
		/// - Input components (hue, saturation, value) are fraction_t in [0; UFraction16::FRACTION_1X].
		/// - Output components are produced via the provided colorFunc as component_t, either:
		///   a) direct (component_t is fraction_t and COMPONENT_MAX == UFraction16::FRACTION_1X), or
		///   b) scaled to COMPONENT_MAX (e.g., 255 for 8-bit components).
		///
		/// Hue handling:
		/// - Hue wraps modulo UFraction16::FRACTION_1X and is mapped into 6 equal segments (0..5).
		/// - Segment interpolation uses UFraction16::Fraction with power-of-two scaling (no divisions).
		///
		/// Fast path:
		/// - If COMPONENT_MAX == UFraction16::FRACTION_1X, all arithmetic stays in fraction_t with no rescale.
		/// - Otherwise components are scaled to component_t using UFraction16::Fraction(component_scalar, COMPONENT_MAX).
		/// </summary>
		/// <typeparam name="color_t">Return type of colorFunc (e.g., a struct or packed RGB value).</typeparam>
		/// <typeparam name="component_t">Component storage type (e.g., uint8_t, uint16_t, or fraction_t).</typeparam>
		/// <typeparam name="ColorFunc">Callable taking (component_t r, component_t g, component_t b) and returning color_t.</typeparam>
		/// <typeparam name="COMPONENT_MAX">Max component value for scaling (e.g., 255 for 8-bit, or UFraction16::FRACTION_1X for direct path).</typeparam>
		/// <param name="hue">fraction_t in [0; UFraction16::FRACTION_1X], wraps at unit.</param>
		/// <param name="saturation">fraction_t in [0; UFraction16::FRACTION_1X].</param>
		/// <param name="value">fraction_t in [0; UFraction16::FRACTION_1X].</param>
		/// <param name="colorFunc">Callable to build the final color from (r, g, b) components.</param>
		/// <returns>color_t constructed by colorFunc from the computed components.</returns>
		template<typename color_t,
			typename component_t,
			component_t COMPONENT_MAX,
			typename ColorFunc>
		static color_t TemplateHsvFraction(
			const fraction_t hue, const fraction_t saturation, const fraction_t value,
			ColorFunc&& colorFunc)
		{
			static constexpr bool DirectComponents = COMPONENT_MAX == UFraction16::FRACTION_1X;
			static constexpr uint8_t Segments = 6;
			static constexpr uint32_t FullScale = uint32_t(Segments) * UFraction16::FRACTION_1X;

			if (saturation == 0) // Achromatic (gray).
			{
				if (DirectComponents)
				{
					return colorFunc(component_t(value), component_t(value), component_t(value));
				}
				else
				{
					const component_t valueScaled = UFraction16::Fraction(value, COMPONENT_MAX);
					return colorFunc(component_t(valueScaled), component_t(valueScaled), component_t(valueScaled));
				}
			}
			else
			{
				// Scale hue to [0; 6*unit] and derive segment index [0..5].
				const uint32_t hueScaled = UFraction16::Fraction(hue, FullScale);
				const uint8_t hueSegment = UFraction16::Fraction(hue, Segments);

				// Fractional position within the segment: [0; unit).
				const fraction_t segmentHue = hueScaled - (uint32_t(hueSegment) * UFraction16::FRACTION_1X);

				if (DirectComponents) // All math in fraction_t, no component scaling.
				{
					const fraction_t valueMinSaturation = value - UFraction16::Fraction(saturation, value);
					const fraction_t saturationPortion = UFraction16::Fraction(segmentHue, saturation);
					const fraction_t valueMinusSaturationPortion = value - UFraction16::Fraction(saturationPortion, value);
					const fraction_t valueMinusInverseSaturationPortion =
						value - UFraction16::Fraction(fraction_t(UFraction16::FRACTION_1X - saturationPortion), value);

					switch (hueSegment % Segments)
					{
					case 0: return colorFunc(component_t(value), component_t(valueMinusInverseSaturationPortion), component_t(valueMinSaturation));
					case 1: return colorFunc(component_t(valueMinusSaturationPortion), component_t(value), component_t(valueMinSaturation));
					case 2: return colorFunc(component_t(valueMinSaturation), component_t(value), component_t(valueMinusInverseSaturationPortion));
					case 3: return colorFunc(component_t(valueMinSaturation), component_t(valueMinusSaturationPortion), component_t(value));
					case 4: return colorFunc(component_t(valueMinusInverseSaturationPortion), component_t(valueMinSaturation), component_t(value));
					case (Segments - 1):
					default: return colorFunc(component_t(value), component_t(valueMinSaturation), component_t(valueMinusSaturationPortion));
					}
				}
				else // Math in component_t, components scaled from fraction_t
				{
					const component_t valueComp = UFraction16::Fraction(value, COMPONENT_MAX);
					const component_t valueMinSaturation = valueComp - UFraction16::Fraction(saturation, valueComp);

					// Cache saturationPortion intermediate to avoid an extra scale operation.
					const fraction_t saturationPortion = UFraction16::Fraction(segmentHue, saturation);
					const component_t valueMinusSaturationPortion = valueComp - UFraction16::Fraction(saturationPortion, valueComp);
					const component_t valueMinusInverseSaturationPortion =
						valueComp - UFraction16::Fraction(fraction_t(UFraction16::FRACTION_1X - saturationPortion), valueComp);

					switch (hueSegment % Segments)
					{
					case 0:
						return colorFunc(valueComp, valueMinusInverseSaturationPortion, valueMinSaturation);
					case 1:
						return colorFunc(valueMinusSaturationPortion, valueComp, valueMinSaturation);
					case 2:
						return colorFunc(valueMinSaturation, valueComp, valueMinusInverseSaturationPortion);
					case 3:
						return colorFunc(valueMinSaturation, valueMinusSaturationPortion, valueComp);
					case 4:
						return colorFunc(valueMinusInverseSaturationPortion, valueMinSaturation, valueComp);
					case (Segments - 1):
					default:
						return colorFunc(valueComp, valueMinSaturation, valueMinusSaturationPortion);
					}
				}
			}
		}
	}
}
#endif