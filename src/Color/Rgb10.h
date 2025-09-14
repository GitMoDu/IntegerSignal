#ifndef _INTEGER_SIGNAL_COLOR_RGB10_h
#define _INTEGER_SIGNAL_COLOR_RGB10_h

#include "../FixedPoint/ScalarFraction.h"
#include "../Trigonometry/Trigonometry.h"
#include "Hsv.h"

namespace IntegerSignal
{
	/// <summary>
	/// Utilities for 10-bit (A)RGB colors packed into a 32-bit integer.
	/// Layout (from LSB to MSB): [R:10][G:10][B:10][A:2] => 0bAA BBBBBBBBBB GGGGGGGGGG RRRRRRRRRR.
	/// Provides construction, component access/mutation, interpolation, and HSV conversion.
	/// Interpolation and HSV conversion use Q-format fractional scalars (UQ0.15) from ScalarFraction
	/// to enable fast shift-based math.
	/// </summary>
	namespace Rgb10
	{
		using namespace FixedPoint::ScalarFraction;

		/// <summary>
		/// RGB10 component storage type (10-bit stored in 16-bit).
		/// </summary>
		using component_t = uint16_t;

		/// <summary>
		/// Maximum value for a 10-bit component (1023).
		/// </summary>
		static constexpr component_t COMPONENT_MAX = (1 << 10) - 1;

		/// <summary>
		/// Alpha storage type (2-bit stored in 8-bit).
		/// </summary>
		using alpha_t = uint8_t;

		/// <summary>
		/// Maximum value for 2-bit alpha (3).
		/// </summary>
		static constexpr alpha_t ALPHA_MAX = 0x3;

		/// <summary>
		/// Packed ARGB10 color type.
		/// </summary>
		using color_t = uint32_t;

		/// <summary>
		/// Construct an ARGB10 color from components.
		/// </summary>
		/// <param name="alpha">Alpha [0..3].</param>
		/// <param name="red">Red [0..1023].</param>
		/// <param name="green">Green [0..1023].</param>
		/// <param name="blue">Blue [0..1023].</param>
		/// <returns>Packed ARGB10 color.</returns>
		static constexpr color_t Color(const alpha_t alpha, const component_t red, const component_t green, const component_t blue)
		{
			return (uint32_t(alpha & ALPHA_MAX) << 30)
				| (uint32_t(blue & COMPONENT_MAX) << 20)
				| (uint32_t(green & COMPONENT_MAX) << 10)
				| (red & COMPONENT_MAX);
		}

		/// <summary>
		/// Construct an opaque ARGB10 color from RGB components (alpha = ALPHA_MAX).
		/// </summary>
		/// <param name="red">Red [0..1023].</param>
		/// <param name="green">Green [0..1023].</param>
		/// <param name="blue">Blue [0..1023].</param>
		/// <returns>Packed ARGB10 color.</returns>
		static constexpr color_t Color(const component_t red, const component_t green, const component_t blue)
		{
			return (uint32_t(ALPHA_MAX) << 30)
				| (uint32_t(blue & COMPONENT_MAX) << 20)
				| (uint32_t(green & COMPONENT_MAX) << 10)
				| (red & COMPONENT_MAX);
		}

		/// <summary>
		/// Construct ARGB10 from 8-bit ARGB by expanding 8-bit channels to 10-bit (approximation).
		/// Alpha is downscaled to 2 bits via shift.
		/// </summary>
		/// <param name="alpha">Alpha [0..255], downscaled to 2-bit (alpha >> 6).</param>
		/// <param name="red">Red [0..255], expanded to 10-bit via (c << 2) | (c >> 6).</param>
		/// <param name="green">Green [0..255], expanded to 10-bit via (c << 2) | (c >> 6).</param>
		/// <param name="blue">Blue [0..255], expanded to 10-bit via (c << 2) | (c >> 6).</param>
		/// <returns>Packed ARGB10 color.</returns>
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
		/// <param name="red">Red [0..255].</param>
		/// <param name="green">Green [0..255].</param>
		/// <param name="blue">Blue [0..255].</param>
		/// <returns>Packed ARGB10 color.</returns>
		static constexpr color_t Color8(const uint8_t red, const uint8_t green, const uint8_t blue)
		{
			return Color8(UINT8_MAX, red, green, blue);
		}

		/// <summary>
		/// Set the alpha component of a packed ARGB10 color.
		/// </summary>
		/// <param name="color">Packed color to modify.</param>
		/// <param name="alpha">Alpha [0..3].</param>
		static void SetAlpha(color_t& color, const alpha_t alpha)
		{
			color = (color & ~(uint32_t(ALPHA_MAX) << 30)) | ((uint32_t(alpha & ALPHA_MAX)) << 30);
		}

		/// <summary>
		/// Set the red component of a packed ARGB10 color.
		/// </summary>
		/// <param name="color">Packed color to modify.</param>
		/// <param name="red">Red [0..1023].</param>
		static void SetRed(color_t& color, const component_t red)
		{
			color = (color & ~COMPONENT_MAX) | red;
		}

		/// <summary>
		/// Set the green component of a packed ARGB10 color.
		/// </summary>
		/// <param name="color">Packed color to modify.</param>
		/// <param name="green">Green [0..1023].</param>
		static void SetGreen(color_t& color, const component_t green)
		{
			color = (color & ~(uint32_t(COMPONENT_MAX) << 10)) | (uint32_t(green) << 10);
		}

		/// <summary>
		/// Set the blue component of a packed ARGB10 color.
		/// </summary>
		/// <param name="color">Packed color to modify.</param>
		/// <param name="blue">Blue [0..1023].</param>
		static void SetBlue(color_t& color, const component_t blue)
		{
			color = (color & ~(uint32_t(COMPONENT_MAX) << 20)) | (uint32_t(blue) << 20);
		}

		/// <summary>
		/// Get the 2-bit alpha component.
		/// </summary>
		/// <param name="color">Packed ARGB10 color.</param>
		/// <returns>Alpha [0..3].</returns>
		static constexpr alpha_t Alpha(const color_t color)
		{
			return (color >> 30) & ALPHA_MAX;
		}

		/// <summary>
		/// Get the red component.
		/// </summary>
		/// <param name="color">Packed ARGB10 color.</param>
		/// <returns>Red [0..1023].</returns>
		static constexpr component_t Red(const color_t color)
		{
			return color & COMPONENT_MAX;
		}

		/// <summary>
		/// Get the green component.
		/// </summary>
		/// <param name="color">Packed ARGB10 color.</param>
		/// <returns>Green [0..1023].</returns>
		static constexpr component_t Green(const color_t color)
		{
			return (color >> 10) & COMPONENT_MAX;
		}

		/// <summary>
		/// Get the blue component.
		/// </summary>
		/// <param name="color">Packed ARGB10 color.</param>
		/// <returns>Blue [0..1023].</returns>
		static constexpr component_t Blue(const color_t color)
		{
			return (color >> 20) & COMPONENT_MAX;
		}

		/// <summary>
		/// Set the alpha component using an 8-bit value by downscaling to 2 bits.
		/// </summary>
		/// <param name="color">Packed ARGB10 color to modify.</param>
		/// <param name="alpha">Alpha [0..255], downscaled via (alpha >> 6).</param>
		static void SetAlpha8(color_t& color, const component_t alpha)
		{
			color = (color & ~(uint32_t(ALPHA_MAX) << 30)) | ((uint32_t(alpha >> 6)) << 30);
		}

		/// <summary>
		/// Get alpha as 8-bit by expanding 2 bits to 8 bits (replication pattern).
		/// </summary>
		/// <param name="color">Packed ARGB10 color.</param>
		/// <returns>Alpha [0..255].</returns>
		static constexpr uint8_t Alpha8(const color_t color)
		{
			// Expand 2-bit alpha to 8-bit (00->0x00, 01->0x55, 10->0xAA, 11->0xFF).
			return uint8_t(Alpha(color)) * 0x55;
		}

		/// <summary>
		/// Get the red channel as 8-bit by truncating the lowest 2 bits.
		/// </summary>
		static constexpr uint8_t Red8(const color_t color)
		{
			return (color >> 2) & UINT8_MAX;
		}

		/// <summary>
		/// Get the green channel as 8-bit by truncating the lowest 2 bits.
		/// </summary>
		static constexpr uint8_t Green8(const color_t color)
		{
			return (color >> 12) & UINT8_MAX;
		}

		/// <summary>
		/// Get the blue channel as 8-bit by truncating the lowest 2 bits.
		/// </summary>
		static constexpr uint8_t Blue8(const color_t color)
		{
			return (color >> 22) & UINT8_MAX;
		}

		/// <summary>
		/// Convert ARGB10 to RGB8 0xRRGGBB (no alpha), truncating the lowest 2 bits per channel.
		/// </summary>
		/// <param name="color">Packed ARGB10 color.</param>
		/// <returns>Packed RGB8 0xRRGGBB.</returns>
		static constexpr uint32_t Rgb8(const color_t color)
		{
			return (uint32_t((Red(color) >> 2)) << 16)
				| (uint16_t((Green(color) >> 2)) << 8)
				| (Blue(color) >> 2);
		}

		/// <summary>
		/// Convert ARGB10 to ARGB8 (alpha in the highest 8 bits), truncating the lowest 2 bits per color channel.
		/// </summary>
		/// <param name="color">Packed ARGB10 color.</param>
		/// <returns>Packed ARGB8 0xAARRGGBB.</returns>
		static constexpr uint32_t Argb8(const color_t color)
		{
			return (uint32_t(Alpha8(color)) << 24)
				| (uint32_t(Red(color) >> 2) << 16)
				| (uint16_t(Green(color) >> 2) << 8)
				| (Blue(color) >> 2);
		}

		/// <summary>
		/// Linear interpolation between two ARGB10 colors using a Q-format unsigned fraction (UQ0.15).
		/// </summary>
		/// <param name="from">Start color.</param>
		/// <param name="to">End color.</param>
		/// <param name="fraction">ufraction16_t in [0, UFraction16::FRACTION_1X].</param>
		/// <returns>Interpolated ARGB10 color (alpha remains ALPHA_MAX in the returned value).</returns>
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
		/// Weighted RMS interpolation between two ARGB10 colors using a Q-format unsigned fraction (UQ0.15).
		/// For each component c: result = sqrt((w1*c1)^2 + (w2*c2)^2), where w1 = (1 - fraction), w2 = fraction.
		/// </summary>
		/// <param name="from">Start color.</param>
		/// <param name="to">End color.</param>
		/// <param name="fraction">ufraction16_t in [0, UFraction16::FRACTION_1X].</param>
		/// <returns>Interpolated ARGB10 color (alpha remains ALPHA_MAX in the returned value).</returns>
		static color_t ColorInterpolate(const color_t& from, const color_t& to, const ufraction16_t fraction)
		{
			const ufraction16_t inverse = UFraction16::FRACTION_1X - fraction;

			component_t red = 0;
			component_t green = 0;
			component_t blue = 0;

			int32_t x = Fraction(inverse, Red(from));
			int32_t y = Fraction(fraction, Red(to));
			red = SquareRoot32((x * x) + (y * y));

			x = Fraction(inverse, Green(from));
			y = Fraction(fraction, Green(to));
			green = SquareRoot32((x * x) + (y * y));

			x = Fraction(inverse, Blue(from));
			y = Fraction(fraction, Blue(to));
			blue = SquareRoot32((x * x) + (y * y));

			return Color(red, green, blue);
		}

		/// <summary>
		/// Convert HSV (UQ0.15) to ARGB10 (alpha = ALPHA_MAX).
		/// Hue, saturation, and value are ufraction16_t in [0, UFraction16::FRACTION_1X].
		/// Hue wraps modulo unit and is internally mapped to 6 equal segments.
		/// </summary>
		/// <param name="hue">Hue in [0, UFraction16::FRACTION_1X], wraps at unit.</param>
		/// <param name="saturation">Saturation in [0, UFraction16::FRACTION_1X].</param>
		/// <param name="value">Value (brightness) in [0, UFraction16::FRACTION_1X].</param>
		/// <returns>Packed ARGB10 color.</returns>
		static color_t ColorHsvFraction(const ufraction16_t hue, const ufraction16_t saturation, const ufraction16_t value)
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
		/// Inputs are mapped to ufraction16_t via UFraction16::GetScalar, then delegated to ColorHsvFraction.
		/// </summary>
		/// <param name="hue">Modular angle in [0, Trigonometry::ANGLE_RANGE].</param>
		/// <param name="saturation">Saturation [0..255].</param>
		/// <param name="value">Value (brightness) [0..255].</param>
		/// <returns>Packed ARGB10 color.</returns>
		static color_t ColorHsv(const Trigonometry::angle_t hue, const uint8_t saturation, const uint8_t value)
		{
			// Map inputs to ufraction16_t scalars.
			const ufraction16_t hue16 = UFraction16::GetScalar<uint16_t>(hue, Trigonometry::ANGLE_RANGE);
			const ufraction16_t sat16 = UFraction16::GetScalar(saturation, uint8_t(UINT8_MAX));
			const ufraction16_t val16 = UFraction16::GetScalar(value, uint8_t(UINT8_MAX));

			return ColorHsvFraction(hue16, sat16, val16);
		}

		/// <summary>ARGB10 constant for black.</summary>
		static constexpr color_t BLACK = 0;

		/// <summary>ARGB10 constant for white (alpha = ALPHA_MAX).</summary>
		static constexpr color_t WHITE = Color(COMPONENT_MAX, COMPONENT_MAX, COMPONENT_MAX);

		/// <summary>ARGB10 constant for red (alpha = ALPHA_MAX).</summary>
		static constexpr color_t RED = Color(COMPONENT_MAX, 0, 0);

		/// <summary>ARGB10 constant for green (alpha = ALPHA_MAX).</summary>
		static constexpr color_t GREEN = Color(0, COMPONENT_MAX, 0);

		/// <summary>ARGB10 constant for blue (alpha = ALPHA_MAX).</summary>
		static constexpr color_t BLUE = Color(0, 0, COMPONENT_MAX);
	}
}
#endif