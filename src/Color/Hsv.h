#ifndef _INTEGER_SIGNAL_COLOR_HSV_h
#define _INTEGER_SIGNAL_COLOR_HSV_h

#include "../FixedPoint/ScalarFraction.h"
#include "../Trigonometry/Trigonometry.h"

namespace IntegerSignal
{
	namespace Hsv
	{
		using namespace FixedPoint::ScalarFraction;

		/// <summary>
		/// HSV color with hue in the modular angle domain and saturation/value in component_t scale.
		/// </summary>
		template<typename component_t>
		struct hsv_t
		{
			Trigonometry::angle_t hue;
			component_t saturation;
			component_t value;
		};

		using hsv_fraction_t = hsv_t<ufraction16_t>;

		template<typename component_t>
		struct rgb_statistics_t
		{
			component_t maximum;
			component_t delta;
		};

		template<typename component_t>
		inline rgb_statistics_t<component_t> GetRgbStatistics(
			const component_t red, const component_t green, const component_t blue)
		{
			const component_t maximum = red > green ? (red > blue ? red : blue) : (green > blue ? green : blue);
			const component_t minimum = red < green ? (red < blue ? red : blue) : (green < blue ? green : blue);
			return { maximum, maximum - minimum };
		}

		template<typename component_t>
		inline Trigonometry::angle_t GetHue(const component_t red, const component_t green,
			const component_t blue, const component_t maximum, const component_t delta)
		{
			if (delta == 0)
			{
				return 0;
			}

			uint32_t hueSector;
			uint16_t sectorRemainder;
			if (maximum == red)
			{
				if (green < blue)
				{
					hueSector = 5;
					sectorRemainder = uint16_t(delta - (blue - green));
				}
				else if (green - blue == delta)
				{
					hueSector = 1;
					sectorRemainder = 0;
				}
				else
				{
					hueSector = 0;
					sectorRemainder = uint16_t(green - blue);
				}
			}
			else if (maximum == green)
			{
				if (blue >= red)
				{
					if (blue == maximum)
					{
						hueSector = 3;
						sectorRemainder = 0;
					}
					else
					{
						hueSector = 2;
						sectorRemainder = uint16_t(blue - red);
					}
				}
				else
				{
					hueSector = 1;
					sectorRemainder = uint16_t(delta - (red - blue));
				}
			}
			else
			{
				if (red >= green)
				{
					hueSector = 4;
					sectorRemainder = uint16_t(red - green);
				}
				else
				{
					hueSector = 3;
					sectorRemainder = uint16_t(delta - (green - red));
				}
			}

			const ufraction16_t sectorFraction = UFraction16::GetScalar<uint16_t>(
				sectorRemainder, uint16_t(delta));
			const uint32_t hueBase = (hueSector * uint32_t(Trigonometry::ANGLE_RANGE)) / 6;
			const uint16_t hueOffset = Fraction(sectorFraction, uint16_t(Trigonometry::ANGLE_RANGE / 6));
			return Trigonometry::angle_t(hueBase + hueOffset);
		}

		/// <summary>
		/// Convert RGB components to HSV using integer arithmetic.
		/// Achromatic colors use hue zero. The input component maximum defines the RGB range,
		/// and hsvMaximum defines the output saturation/value range.
		/// </summary>
		template<typename hsv_component_t, typename rgb_component_t>
		inline hsv_t<hsv_component_t> FromRgb(const rgb_component_t red, const rgb_component_t green,
			const rgb_component_t blue, const uint32_t componentMaximum, const uint32_t hsvMaximum)
		{
			using wide_t = typename TypeTraits::TypeNext::next_uint_type<rgb_component_t>::type;
			const rgb_statistics_t<rgb_component_t> statistics = GetRgbStatistics(red, green, blue);

			const hsv_component_t value = hsv_component_t(
				(wide_t(statistics.maximum) * wide_t(hsvMaximum)) / componentMaximum);
			const hsv_component_t saturation = statistics.maximum == 0
				? hsv_component_t(0)
				: hsv_component_t((wide_t(statistics.delta) * wide_t(hsvMaximum)) / statistics.maximum);

			const Trigonometry::angle_t hue = GetHue(red, green, blue, statistics.maximum, statistics.delta);
			return { hue, saturation, value };
		}

		template<typename rgb_component_t>
		inline hsv_fraction_t FromRgbFraction(const rgb_component_t red, const rgb_component_t green,
			const rgb_component_t blue, const uint32_t componentMaximum)
		{
			const rgb_statistics_t<rgb_component_t> statistics = GetRgbStatistics(red, green, blue);
			const ufraction16_t value = UFraction16::GetScalar<uint16_t>(
				uint16_t(statistics.maximum), uint16_t(componentMaximum));
			const ufraction16_t saturation = statistics.maximum == 0
				? ufraction16_t(0)
				: UFraction16::GetScalar<uint16_t>(uint16_t(statistics.delta), uint16_t(statistics.maximum));
			const Trigonometry::angle_t hue = GetHue(red, green, blue, statistics.maximum, statistics.delta);
			return { hue, saturation, value };
		}

		/// <summary>
		/// Convert native 8-bit HSV components to an RGB color without UQ0.15 normalization.
		/// </summary>
		template<typename color_t,
			typename component_t,
			component_t COMPONENT_MAX,
			uint8_t COMPONENT_BITS,
			uint32_t HSV_MAX,
			typename ColorFunc>
		inline color_t TemplateHsvNative(const Trigonometry::angle_t hue,
			const uint16_t saturation, const uint16_t value, ColorFunc&& colorFunc)
		{
			const component_t valueComponent = component_t(
				(uint32_t(value) * COMPONENT_MAX) / HSV_MAX);
			if (saturation == 0)
			{
				return colorFunc(valueComponent, valueComponent, valueComponent);
			}

			const uint32_t huePosition = uint32_t(hue) * 6UL;
			const uint8_t hueSector = uint8_t(huePosition >> 16);
			const uint16_t hueFraction = uint16_t(huePosition) >> (16 - COMPONENT_BITS);
			const uint32_t chroma = (uint32_t(valueComponent) * saturation) / HSV_MAX;
			const uint32_t minimum = uint32_t(valueComponent) - chroma;
			const uint32_t ascending = (chroma * hueFraction) >> COMPONENT_BITS;
			const uint32_t descending = chroma - ascending;

			switch (hueSector)
			{
			case 0: return colorFunc(valueComponent, component_t(minimum + ascending), component_t(minimum));
			case 1: return colorFunc(component_t(minimum + descending), valueComponent, component_t(minimum));
			case 2: return colorFunc(component_t(minimum), valueComponent, component_t(minimum + ascending));
			case 3: return colorFunc(component_t(minimum), component_t(minimum + descending), valueComponent);
			case 4: return colorFunc(component_t(minimum + ascending), component_t(minimum), valueComponent);
			default: return colorFunc(valueComponent, component_t(minimum), component_t(minimum + descending));
			}
		}

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