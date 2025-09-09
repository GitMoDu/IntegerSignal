#ifndef _INTEGER_SIGNAL_COLOR_RGB10_h
#define _INTEGER_SIGNAL_COLOR_RGB10_h

#include "../FixedPoint/Fraction.h"

namespace IntegerSignal
{
	/// <summary>
	/// Utilities for 10-bit (A)RGB color 0xARRGGBB packed into a 32-bit integer.
	/// Includes color creation, component extraction, modification, and interpolation.
	/// Optimized for 10-bit RGB colors.
	/// </summary>
	namespace Rgb10
	{
		using namespace FixedPoint::Fraction;

		// RGB10 color components are represented as uint16_t.
		using component_t = uint16_t;

		// RGB10 color components maximum value (10-bit).
		static constexpr component_t COMPONENT_MAX = (1 << 10) - 1;

		// RGB10 alpha component is represented as a 2-bit value.
		using alpha_t = uint8_t;

		// RGB10 alpha component maximum value (2 bits).
		static constexpr alpha_t ALPHA_MAX = 0x3;

		// RGB10 color is packed into a single uint32_t value.
		using color_t = uint32_t;

		// Fraction type used for interpolation (unsigned Q0.15).
		using fraction_t = Hsv::fraction_t;

		/// <summary>
		/// Construct ARGB10 color from components.
		/// </summary>
		static constexpr color_t Color(const alpha_t alpha, const component_t red, const component_t green, const component_t blue)
		{
			return (uint32_t(alpha & ALPHA_MAX) << 30)
				| (uint32_t(blue & COMPONENT_MAX) << 20)
				| (uint32_t(green & COMPONENT_MAX) << 10)
				| (red & COMPONENT_MAX);
		}

		/// <summary>
		/// Construct ARGB10 color from RGB components (alpha = ALPHA_MAX).
		/// </summary>
		static constexpr color_t Color(const component_t red, const component_t green, const component_t blue)
		{
			return (uint32_t(ALPHA_MAX) << 30)
				| (uint32_t(blue & COMPONENT_MAX) << 20)
				| (uint32_t(green & COMPONENT_MAX) << 10)
				| (red & COMPONENT_MAX);
		}

		/// <summary>
		/// Construct ARGB10 from 8-bit ARGB by expanding 8-bit channels to 10-bit.
		/// Alpha is downscaled to 2 bits.
		/// </summary>
		static constexpr color_t Color8(const uint8_t alpha, const uint8_t red, const uint8_t green, const uint8_t blue)
		{
			return (uint32_t(alpha >> 6) << 30)
				| (uint32_t((blue << 2) | (blue >> 6)) << 20)
				| (uint32_t((green << 2) | (green >> 6)) << 10)
				| (uint32_t((red << 2) | (red >> 6)));
		}

		/// <summary>
		/// Construct ARGB10 from 8-bit RGB (alpha = 255).
		/// </summary>
		static constexpr color_t Color8(const uint8_t red, const uint8_t green, const uint8_t blue)
		{
			return Color8(UINT8_MAX, red, green, blue);
		}

		static void SetAlpha(color_t& color, const alpha_t alpha)
		{
			color = (color & ~(uint32_t(ALPHA_MAX) << 30)) | ((uint32_t(alpha & ALPHA_MAX)) << 30);
		}

		static void SetRed(color_t& color, const component_t red)
		{
			color = (color & ~COMPONENT_MAX) | red;
		}

		static void SetGreen(color_t& color, const component_t green)
		{
			color = (color & ~(uint32_t(COMPONENT_MAX) << 10)) | (uint32_t(green) << 10);
		}

		static void SetBlue(color_t& color, const component_t blue)
		{
			color = (color & ~(uint32_t(COMPONENT_MAX) << 20)) | (uint32_t(blue) << 20);
		}

		static constexpr alpha_t Alpha(const color_t color)
		{
			return (color >> 30) & ALPHA_MAX;
		}

		static constexpr component_t Red(const color_t color)
		{
			return color & COMPONENT_MAX;
		}

		static constexpr component_t Green(const color_t color)
		{
			return (color >> 10) & COMPONENT_MAX;
		}

		static constexpr component_t Blue(const color_t color)
		{
			return (color >> 20) & COMPONENT_MAX;
		}

		static void SetAlpha8(color_t& color, const component_t alpha)
		{
			color = (color & ~(uint32_t(ALPHA_MAX) << 30)) | ((uint32_t(alpha >> 6)) << 30);
		}

		static constexpr uint8_t Alpha8(const color_t color)
		{
			// Expand 2-bit alpha to 8-bit.
			return uint8_t(Alpha(color)) * 0x55;
		}

		static constexpr uint8_t Red8(const color_t color)
		{
			return (color >> 2) & UINT8_MAX;
		}

		static constexpr uint8_t Green8(const color_t color)
		{
			return (color >> 12) & UINT8_MAX;
		}

		static constexpr uint8_t Blue8(const color_t color)
		{
			return (color >> 22) & UINT8_MAX;
		}

		/// <summary>
		/// Convert ARGB10 to RGB8 0xRRGGBB (no alpha).
		/// </summary>
		static constexpr uint32_t Rgb8(const color_t color)
		{
			return (uint32_t((Red(color) >> 2)) << 16)
				| (uint16_t((Green(color) >> 2)) << 8)
				| (Blue(color) >> 2);
		}

		/// <summary>
		/// Convert ARGB10 to ARGB8 (alpha in the highest 8 bits).
		/// </summary>
		static constexpr uint32_t Argb8(const color_t color)
		{
			return (uint32_t(Alpha8(color)) << 24)
				| (uint32_t(Red(color) >> 2) << 16)
				| (uint16_t(Green(color) >> 2) << 8)
				| (Blue(color) >> 2);
		}

		/// <summary>
		/// Linear interpolation between two ARGB10 colors using an unsigned Q-format fraction.
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
		/// Weighted RMS interpolation between two ARGB10 colors using an unsigned Q-format fraction.
		/// fraction is fraction_t in [0; UFraction16::FRACTION_1X].
		/// </summary>
		static color_t ColorInterpolate(const color_t& from, const color_t& to, const fraction_t fraction)
		{
			const fraction_t inverse = UFraction16::FRACTION_1X - fraction;

			component_t red = 0;
			component_t green = 0;
			component_t blue = 0;

			int32_t x = UFraction16::Fraction(inverse, Red(from));
			int32_t y = UFraction16::Fraction(fraction, Red(to));
			red = SquareRoot32((x * x) + (y * y));

			x = UFraction16::Fraction(inverse, Green(from));
			y = UFraction16::Fraction(fraction, Green(to));
			green = SquareRoot32((x * x) + (y * y));

			x = UFraction16::Fraction(inverse, Blue(from));
			y = UFraction16::Fraction(fraction, Blue(to));
			blue = SquareRoot32((x * x) + (y * y));

			return Color(red, green, blue);
		}

		/// <summary>
		/// Convert HSV (fraction_t) to ARGB10.
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
		/// Convert HSV to ARGB10 where hue is an angle_t (modular full-rotation index)
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