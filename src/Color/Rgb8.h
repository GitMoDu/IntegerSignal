#ifndef _INTEGER_SIGNAL_COLOR_RGB8_h
#define _INTEGER_SIGNAL_COLOR_RGB8_h

#include "../FixedPoint/ScalarFraction.h"
#include "../Trigonometry/Trigonometry.h"
#include "Hsv.h"

namespace IntegerSignal
{
	/// <summary>
	/// Utilities for 8-bit (A)RGB colors packed into a 32-bit integer (0xAARRGGBB).
	/// Provides construction, component access/mutation, interpolation, and HSV conversion.
	/// Interpolation and HSV conversion use Q-format fractional scalars (UQ0.15) from ScalarFraction
	/// to enable fast shift-based math.
	/// </summary>
	namespace Rgb8
	{
		using namespace FixedPoint::ScalarFraction;

		/// <summary>
		/// RGB8 component storage type (8-bit).
		/// </summary>
		using component_t = uint8_t;

		/// <summary>
		/// Maximum value for an RGB8 component (255).
		/// </summary>
		static constexpr component_t COMPONENT_MAX = UINT8_MAX;

		/// <summary>
		/// Packed ARGB color type (0xAARRGGBB).
		/// </summary>
		using color_t = uint32_t;

		/// <summary>
		/// Construct an ARGB color from components.
		/// </summary>
		/// <param name="alpha">Alpha component [0..255].</param>
		/// <param name="red">Red component [0..255].</param>
		/// <param name="green">Green component [0..255].</param>
		/// <param name="blue">Blue component [0..255].</param>
		/// <returns>Packed color 0xAARRGGBB.</returns>
		static constexpr color_t Color(const component_t alpha, const component_t red, const component_t green, const component_t blue)
		{
			return (uint32_t(alpha) << 24)
				| (uint32_t(red) << 16)
				| (uint16_t(green) << 8)
				| (blue);
		}

		/// <summary>
		/// Construct an opaque RGB color from components (alpha = 255).
		/// </summary>
		/// <param name="red">Red component [0..255].</param>
		/// <param name="green">Green component [0..255].</param>
		/// <param name="blue">Blue component [0..255].</param>
		/// <returns>Packed color 0xFFRRGGBB.</returns>
		static constexpr color_t Color(const component_t red, const component_t green, const component_t blue)
		{
			return (uint32_t(COMPONENT_MAX) << 24)
				| (uint32_t(red) << 16)
				| (uint16_t(green) << 8)
				| (blue);
		}

		/// <summary>
		/// Set the alpha component of a packed color.
		/// </summary>
		/// <param name="color">Packed color to modify.</param>
		/// <param name="alpha">Alpha value [0..255].</param>
		static void SetAlpha(color_t& color, const component_t alpha)
		{
			color = (color & ~(uint32_t(COMPONENT_MAX) << 24)) | (uint32_t(alpha) << 24);
		}

		/// <summary>
		/// Set the red component of a packed color.
		/// </summary>
		/// <param name="color">Packed color to modify.</param>
		/// <param name="red">Red value [0..255].</param>
		static void SetRed(color_t& color, const component_t red)
		{
			color = (color & ~(uint32_t(COMPONENT_MAX) << 16)) | (uint32_t(red) << 16);
		}

		/// <summary>
		/// Set the green component of a packed color.
		/// </summary>
		/// <param name="color">Packed color to modify.</param>
		/// <param name="green">Green value [0..255].</param>
		static void SetGreen(color_t& color, const component_t green)
		{
			color = (color & ~(uint32_t(COMPONENT_MAX) << 8)) | (uint32_t(green) << 8);
		}

		/// <summary>
		/// Set the blue component of a packed color.
		/// </summary>
		/// <param name="color">Packed color to modify.</param>
		/// <param name="blue">Blue value [0..255].</param>
		static void SetBlue(color_t& color, const component_t blue)
		{
			color = (color & ~COMPONENT_MAX) | blue;
		}

		/// <summary>
		/// Get the alpha component of a packed color.
		/// </summary>
		/// <param name="color">Packed color.</param>
		/// <returns>Alpha value [0..255].</returns>
		static constexpr component_t Alpha(const color_t color)
		{
			return (color >> 24) & COMPONENT_MAX;
		}

		/// <summary>
		/// Get the red component of a packed color.
		/// </summary>
		/// <param name="color">Packed color.</param>
		/// <returns>Red value [0..255].</returns>
		static constexpr component_t Red(const color_t color)
		{
			return (color >> 16) & COMPONENT_MAX;
		}

		/// <summary>
		/// Get the green component of a packed color.
		/// </summary>
		/// <param name="color">Packed color.</param>
		/// <returns>Green value [0..255].</returns>
		static constexpr component_t Green(const color_t color)
		{
			return (color >> 8) & COMPONENT_MAX;
		}

		/// <summary>
		/// Get the blue component of a packed color.
		/// </summary>
		/// <param name="color">Packed color.</param>
		/// <returns>Blue value [0..255].</returns>
		static constexpr component_t Blue(const color_t color)
		{
			return color & COMPONENT_MAX;
		}

		/// <summary>
		/// Linear interpolation between two colors using a Q-format unsigned fraction (UQ0.15).
		/// </summary>
		/// <param name="from">Start color (packed 0xAARRGGBB).</param>
		/// <param name="to">End color (packed 0xAARRGGBB).</param>
		/// <param name="fraction">
		/// ufraction16_t in [0, UFraction16::FRACTION_1X]. 0 returns 'from', unit returns 'to'.
		/// </param>
		/// <returns>Interpolated color.</returns>
		static color_t ColorInterpolateLinear(const color_t& from, const color_t& to, const ufraction16_t fraction)
		{
			const ufraction16_t inverse = UFraction16::FRACTION_1X - fraction;

			return Color(
				Fraction(inverse, Red(from)) + Fraction(fraction, Red(to)),
				Fraction(inverse, Green(from)) + Fraction(fraction, Green(to)),
				Fraction(inverse, Blue(from)) + Fraction(fraction, Blue(to))
			);
		}

		/// <summary>
		/// Weighted RMS interpolation between two colors using a Q-format unsigned fraction (UQ0.15).
		/// For each component c: result = sqrt((w1*c1)^2 + (w2*c2)^2), where w1 = (1 - fraction), w2 = fraction.
		/// </summary>
		/// <param name="from">Start color (packed 0xAARRGGBB).</param>
		/// <param name="to">End color (packed 0xAARRGGBB).</param>
		/// <param name="fraction">
		/// ufraction16_t in [0, UFraction16::FRACTION_1X]. 0 returns 'from', unit returns 'to'.
		/// </param>
		/// <returns>Interpolated color.</returns>
		static color_t ColorInterpolate(const color_t& from, const color_t& to, const ufraction16_t fraction)
		{
			const ufraction16_t inverse = UFraction16::FRACTION_1X - fraction;

			component_t red, green, blue;

			// The largest possible square for a component is 255^2 = 65025 (fits in uint16_t).
			uint16_t x, y;

			// Red
			x = Fraction(inverse, Red(from));
			y = Fraction(fraction, Red(to));
			red = SquareRoot16((x * x) + (y * y));

			// Green
			x = Fraction(inverse, Green(from));
			y = Fraction(fraction, Green(to));
			green = SquareRoot16((x * x) + (y * y));

			// Blue
			x = Fraction(inverse, Blue(from));
			y = Fraction(fraction, Blue(to));
			blue = SquareRoot16((x * x) + (y * y));

			return Color(red, green, blue);
		}

		/// <summary>
		/// Convert HSV (UQ0.15) to RGB8.
		/// Hue, saturation, and value are ufraction16_t in [0, UFraction16::FRACTION_1X].
		/// Hue wraps modulo unit and is internally mapped to 6 equal segments.
		/// </summary>
		/// <param name="hue">Hue in [0, UFraction16::FRACTION_1X], wraps at unit.</param>
		/// <param name="saturation">Saturation in [0, UFraction16::FRACTION_1X].</param>
		/// <param name="value">Value (brightness) in [0, UFraction16::FRACTION_1X].</param>
		/// <returns>Packed RGB8 color (alpha = 255).</returns>
		static color_t ColorHsvFraction(const ufraction16_t hue, const ufraction16_t saturation, const ufraction16_t value)
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
		/// Inputs are mapped to ufraction16_t via UFraction16::GetScalar, then delegated to ColorHsvFraction.
		/// </summary>
		/// <param name="hue">Modular angle in [0, Trigonometry::ANGLE_RANGE].</param>
		/// <param name="saturation">Saturation [0..255].</param>
		/// <param name="value">Value (brightness) [0..255].</param>
		/// <returns>Packed RGB8 color (alpha = 255).</returns>
		static color_t ColorHsv(const Trigonometry::angle_t hue, const uint8_t saturation, const uint8_t value)
		{
			// Map inputs to ufraction16_t scalars.
			const ufraction16_t hue16 = UFraction16::GetScalar<uint16_t>(hue, Trigonometry::ANGLE_RANGE);
			const ufraction16_t sat16 = UFraction16::GetScalar(saturation, uint8_t(UINT8_MAX));
			const ufraction16_t val16 = UFraction16::GetScalar(value, uint8_t(UINT8_MAX));

			return ColorHsvFraction(hue16, sat16, val16);
		}

		/// <summary>RGB8 constant for black (0xFF000000).</summary>
		static constexpr color_t BLACK = 0;

		/// <summary>RGB8 constant for white (0xFFFFFFFF).</summary>
		static constexpr color_t WHITE = Color(COMPONENT_MAX, COMPONENT_MAX, COMPONENT_MAX);

		/// <summary>RGB8 constant for red (0xFFFF0000).</summary>
		static constexpr color_t RED = Color(COMPONENT_MAX, 0, 0);

		/// <summary>RGB8 constant for green (0xFF00FF00).</summary>
		static constexpr color_t GREEN = Color(0, COMPONENT_MAX, 0);

		/// <summary>RGB8 constant for blue (0xFF0000FF).</summary>
		static constexpr color_t BLUE = Color(0, 0, COMPONENT_MAX);
	}
}
#endif