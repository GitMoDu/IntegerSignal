#ifndef _INTEGER_SIGNAL_COLOR_RGB8_h
#define _INTEGER_SIGNAL_COLOR_RGB8_h

#include "../FixedPoint/Fraction.h"
#include "../Trigonometry/Trigonometry.h"
#include "Hsv.h"

namespace IntegerSignal
{
	/// <summary>
	/// Utilities for 8-bit (A)RGB colors packed into a 32-bit integer (0xAARRGGBB).
	/// Includes color construction, component access, modification, and interpolation.
	/// </summary>
	namespace Rgb8
	{
		using namespace FixedPoint::Fraction;

		// RGB8 color components are represented as uint8_t.
		using component_t = uint8_t;

		// RGB8 color components maximum value.
		static constexpr component_t COMPONENT_MAX = UINT8_MAX;

		// RGB8 color is packed into a single uint32_t value.
		using color_t = uint32_t;

		// Fraction type used for interpolation (unsigned Q0.15).
		using fraction_t = Hsv::fraction_t;

		/// <summary>
		/// Construct ARGB color from components.
		/// </summary>
		static constexpr color_t Color(const component_t alpha, const component_t red, const component_t green, const component_t blue)
		{
			return (uint32_t(alpha) << 24)
				| (uint32_t(red) << 16)
				| (uint16_t(green) << 8)
				| (blue);
		}

		/// <summary>
		/// Construct opaque RGB color from components (alpha = 255).
		/// </summary>
		static constexpr color_t Color(const component_t red, const component_t green, const component_t blue)
		{
			return (uint32_t(COMPONENT_MAX) << 24)
				| (uint32_t(red) << 16)
				| (uint16_t(green) << 8)
				| (blue);
		}

		static void SetAlpha(color_t& color, const component_t alpha)
		{
			color = (color & ~(uint32_t(COMPONENT_MAX) << 24)) | (uint32_t(alpha) << 24);
		}

		static void SetRed(color_t& color, const component_t red)
		{
			color = (color & ~(uint32_t(COMPONENT_MAX) << 16)) | (uint32_t(red) << 16);
		}

		static void SetGreen(color_t& color, const component_t green)
		{
			color = (color & ~(uint32_t(COMPONENT_MAX) << 8)) | (uint32_t(green) << 8);
		}

		static void SetBlue(color_t& color, const component_t blue)
		{
			color = (color & ~COMPONENT_MAX) | blue;
		}

		static constexpr component_t Alpha(const color_t color)
		{
			return (color >> 24) & COMPONENT_MAX;
		}

		static constexpr component_t Red(const color_t color)
		{
			return (color >> 16) & COMPONENT_MAX;
		}

		static constexpr component_t Green(const color_t color)
		{
			return (color >> 8) & COMPONENT_MAX;
		}

		static constexpr component_t Blue(const color_t color)
		{
			return color & COMPONENT_MAX;
		}

		/// <summary>
		/// Linear interpolation between two colors using an unsigned Q-format fraction.
		/// fraction is fraction_t in [0; UFraction16::FRACTION_1X].
		/// </summary>
		static color_t ColorInterpolateLinear(const color_t& from, const color_t& to, const fraction_t fraction)
		{
			const fraction_t inverse = UFraction16::FRACTION_1X - fraction;

			return Color(
				UFraction16::Fraction(inverse, Red(from)) + UFraction16::Fraction(fraction, Red(to)),
				UFraction16::Fraction(inverse, Green(from)) + UFraction16::Fraction(fraction, Green(to)),
				UFraction16::Fraction(inverse, Blue(from)) + UFraction16::Fraction(fraction, Blue(to))
			);
		}

		/// <summary>
		/// Weighted RMS interpolation between two colors using an unsigned Q-format fraction.
		/// fraction is fraction_t in [0; UFraction16::FRACTION_1X].
		/// </summary>
		static color_t ColorInterpolate(const color_t& from, const color_t& to, const fraction_t fraction)
		{
			const fraction_t inverse = UFraction16::FRACTION_1X - fraction;

			component_t red, green, blue;

			// The largest possible square for a component is 255^2 = 65025 (fits in uint16_t).
			uint16_t x, y;

			// Red
			x = UFraction16::Fraction(inverse, Red(from));
			y = UFraction16::Fraction(fraction, Red(to));
			red = SquareRoot16((x * x) + (y * y));

			// Green
			x = UFraction16::Fraction(inverse, Green(from));
			y = UFraction16::Fraction(fraction, Green(to));
			green = SquareRoot16((x * x) + (y * y));

			// Blue
			x = UFraction16::Fraction(inverse, Blue(from));
			y = UFraction16::Fraction(fraction, Blue(to));
			blue = SquareRoot16((x * x) + (y * y));

			return Color(red, green, blue);
		}

		/// <summary>
		/// Convert HSV (fraction_t) to RGB8.
		/// Hue, saturation, value are fraction_t in [0; UFraction16::FRACTION_1X].
		/// Hue wraps modulo unit and is internally mapped to 6 equal segments.
		/// </summary>
		static color_t ColorHsvFraction(const fraction_t hue, const fraction_t saturation, const fraction_t value)
		{
			return Hsv::TemplateHsvFraction<color_t, component_t, COMPONENT_MAX>(hue, saturation, value,
				[](const component_t red, const component_t green, const component_t blue)
				{
					return Color(red, green, blue);
				});
		}

		/// <summary>
		/// Convert HSV to RGB8 where hue is an angle_t (modular full-rotation index)
		/// and saturation/value are 8-bit components [0..255].
		/// </summary>
		static color_t ColorHsv(const Trigonometry::angle_t hue, const uint8_t saturation, const uint8_t value)
		{
			// Map inputs to fraction_t scalars.
			const fraction_t hue16 = UFraction16::GetScalar<uint16_t>(hue, Trigonometry::ANGLE_RANGE);
			const fraction_t sat16 = UFraction16::GetScalar(saturation, uint8_t(UINT8_MAX));
			const fraction_t val16 = UFraction16::GetScalar(value, uint8_t(UINT8_MAX));

			return ColorHsvFraction(hue16, sat16, val16);
		}

		static constexpr color_t BLACK = 0;
		static constexpr color_t WHITE = Color(COMPONENT_MAX, COMPONENT_MAX, COMPONENT_MAX);
		static constexpr color_t RED = Color(COMPONENT_MAX, 0, 0);
		static constexpr color_t GREEN = Color(0, COMPONENT_MAX, 0);
		static constexpr color_t BLUE = Color(0, 0, COMPONENT_MAX);
	}
}
#endif