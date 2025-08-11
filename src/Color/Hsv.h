#ifndef _INTEGER_SIGNAL_COLOR_HSV_h
#define _INTEGER_SIGNAL_COLOR_HSV_h

#include "../Scale/Fraction.h"

namespace IntegerSignal
{
	using namespace IntegerSignal::Fraction;

	namespace Hsv
	{
		/// <summary>
		/// Converts HSV color values (with fractional components) to a color representation using a customizable color function and component scaling.
		/// Optimizes for both direct component types (e.g., ufraction16_t) and scaled component types (e.g., uint8_t, uint16_t).
		/// </summary>
		/// <typeparam name="color_t">The type representing the final color value returned by the function.</typeparam>
		/// <typeparam name="component_t">The type used for individual color components (e.g., uint8_t, uint16_t).</typeparam>
		/// <typeparam name="ColorFunc">The type of the callable object or function used to construct the color from its components.</typeparam>
		/// <typeparam name="COMPONENT_MAX">The maximum value for a color component (e.g., 255 for 8-bit components).</typeparam>
		/// <param name="hue">The hue component of the color, as a ufraction16_t fractional value in the range [0, 1].</param>
		/// <param name="saturation">The saturation component of the color, as a ufraction16_t fractional value in the range [0, 1].</param>
		/// <param name="value">The value (brightness) component of the color, as a ufraction16_t fractional value in the range [0, 1].</param>
		/// <param name="colorFunc">A callable object or function that constructs the final color from three component values (e.g., RGB).</param>
		/// <returns>A color value of type color_t, constructed from the HSV input using the provided color function and component scaling.</returns>
		template<typename color_t, 
			typename component_t, 
			component_t COMPONENT_MAX, 
			typename ColorFunc>
		static color_t TemplateHsvFraction(
			const ufraction16_t hue, const ufraction16_t saturation, const ufraction16_t value,
			ColorFunc&& colorFunc)
		{
			static constexpr bool DirectComponents = COMPONENT_MAX == UFRACTION16_1X;
			static constexpr uint8_t Segments = 6;
			static constexpr uint32_t FullScale = uint32_t(Segments) * UFRACTION16_1X;

			if (saturation == 0) // Achromatic (gray).
			{
				if (DirectComponents)
				{
					return colorFunc(component_t(value), component_t(value), component_t(value));
				}
				else
				{
					const component_t valueScaled = Fraction::Scale(value, COMPONENT_MAX);
					return colorFunc(component_t(valueScaled), component_t(valueScaled), component_t(valueScaled));
				}
			}
			else
			{
				// Scale hue to FullScale.
				const uint32_t hueScaled = Fraction::Scale(hue, FullScale);

				// Determine which segment of the color wheel the hue is in.
				const uint8_t hueSegment = Fraction::Scale(hue, Segments);

				// Fractional part within the segment.
				const ufraction16_t segmentHue = hueScaled - (uint32_t(hueSegment) * UFRACTION16_1X);

				if (DirectComponents) // All math is done in ufraction16_t, no scaling needed.
				{
					const ufraction16_t valueMinSaturation = value - Fraction::Scale(saturation, value);
					const ufraction16_t saturationPortion = Fraction::Scale(segmentHue, saturation);
					const ufraction16_t valueMinusSaturationPortion = value - Fraction::Scale(saturationPortion, value);
					const ufraction16_t valueMinusInverseSaturationPortion = value - Fraction::Scale(
						ufraction16_t(UFRACTION16_1X - saturationPortion), value);

					switch (hueSegment % Segments)
					{
					case 0:
						return colorFunc(component_t(value), component_t(valueMinusInverseSaturationPortion), component_t(valueMinSaturation));
					case 1:
						return colorFunc(component_t(valueMinusSaturationPortion), component_t(value), component_t(valueMinSaturation));
					case 2:
						return colorFunc(component_t(valueMinSaturation), component_t(value), component_t(valueMinusInverseSaturationPortion));
					case 3:
						return colorFunc(component_t(valueMinSaturation), component_t(valueMinusSaturationPortion), component_t(value));
					case 4:
						return colorFunc(component_t(valueMinusInverseSaturationPortion), component_t(valueMinSaturation), component_t(value));
					case (Segments - 1):
					default:
						return colorFunc(component_t(value), component_t(valueMinSaturation), component_t(valueMinusSaturationPortion));
					}
				}
				else // Math is done in component_t, requires component scaling.
				{
					const component_t valueComp = Fraction::Scale(value, COMPONENT_MAX);
					const component_t valueMinSaturation = valueComp - Fraction::Scale(saturation, valueComp);

					// Cache saturationPortion intermediate to avoid an extra scale operation.
					const ufraction16_t saturationPortion = Fraction::Scale(segmentHue, saturation);
					const component_t valueMinusSaturationPortion = valueComp - Fraction::Scale(saturationPortion, valueComp);
					const component_t valueMinusInverseSaturationPortion = valueComp - Fraction::Scale(
						ufraction16_t(UFRACTION16_1X - saturationPortion), valueComp);

					// Determine the RGB values based on the hue segment.
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