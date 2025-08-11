#ifndef _INTEGER_SIGNAL_COLOR_RGB10_h
#define _INTEGER_SIGNAL_COLOR_RGB10_h

#include "../Scale/Fraction.h"

namespace IntegerSignal
{
	/// <summary>
	/// Provides utilities for working with 10-bit (A)RGB color 0xARRGGBB packed into a 32-bit integer.
	/// Includes color creation, component extraction, modification, and interpolation.
	/// Optimized for 10-bit RGB colors, suitable for high dynamic range graphics applications.
	/// Not as fast as Rgb8, but provides better color depth and range.
	/// </summary>
	namespace Rgb10
	{
		// RGB10 color components are represented as uint16_t.
		using component_t = uint16_t;

		// RGB10 color components maximum value.
		static constexpr component_t COMPONENT_MAX = (1 << 10) - 1;

		// RGB10 alpha component is represented as a 2-bit value.
		using alpha_t = uint8_t;

		// RGB10 alpha component maximum value (2 bits).
		static constexpr alpha_t ALPHA_MAX = 0x3;

		// RGB10 color is packed into a single uint32_t value.
		using color_t = uint32_t;

		using ufraction16_t = Fraction::ufraction16_t;

		/// <summary>
		/// Constructs a color value from individual alpha2, red, green, and blue components.
		/// </summary>
		/// <param name="alpha">The alpha (2 bit) component of the color.</param>
		/// <param name="red">The red component of the color.</param>
		/// <param name="green">The green component of the color.</param>
		/// <param name="blue">The blue component of the color.</param>
		/// <returns>A color_t value representing the combined color with the specified alpha, red, green, and blue components.</returns>
		static constexpr color_t Color(const alpha_t alpha, const component_t red, const component_t green, const component_t blue)
		{
			return (uint32_t(alpha & ALPHA_MAX) << 30) 
				| (uint32_t(blue & COMPONENT_MAX) << 20) 
				| (uint32_t(green & COMPONENT_MAX) << 10) 
				| (red & COMPONENT_MAX);
		}

		/// <summary>
		/// Constructs a color value from red, green, and blue components.
		/// </summary>
		/// <param name="red">The red component of the color.</param>
		/// <param name="green">The green component of the color.</param>
		/// <param name="blue">The blue component of the color.</param>
		/// <returns>A color_t value representing the combined RGB color.</returns>
		static constexpr color_t Color(const component_t red, const component_t green, const component_t blue)
		{
			return (uint32_t(ALPHA_MAX) << 30) 
				| (uint32_t(blue & COMPONENT_MAX) << 20) 
				| (uint32_t(green & COMPONENT_MAX) << 10)
				| (red & COMPONENT_MAX);
		}

		/// <summary>
		/// Constructs a color value from 8-bit alpha, red, green, and blue components, mapping them into a packed color_t format.
		/// </summary>
		/// <param name="alpha">The 8-bit alpha (transparency) component. This is converted down to 2 bit range.</param>
		/// <param name="red">The 8-bit red color component.</param>
		/// <param name="green">The 8-bit green color component.</param>
		/// <param name="blue">The 8-bit blue color component.</param>
		/// <returns>A packed color_t value representing the color with the specified alpha, red, green, and blue components.</returns>
		static constexpr color_t Color8(const uint8_t alpha, const uint8_t red, const uint8_t green, const uint8_t blue)
		{
			return (uint32_t(alpha >> 6) << 30)
				| (uint32_t((blue << 2) | (blue >> 6)) << 20)
				| (uint32_t((green << 2) | (green >> 6)) << 10)
				| (uint32_t((red << 2) | (red >> 6)));
		}

		/// <summary>
		/// Creates a color value from 8-bit red, green, and blue components.
		/// </summary>
		/// <param name="red">The red component of the color (0-255).</param>
		/// <param name="green">The green component of the color (0-255).</param>
		/// <param name="blue">The blue component of the color (0-255).</param>
		/// <returns>A color_t value representing the specified RGB color.</returns>
		static constexpr color_t Color8(const uint8_t red, const uint8_t green, const uint8_t blue)
		{
			return Color8(UINT8_MAX, red, green, blue);
		}

		static constexpr void SetAlpha(color_t& color, const alpha_t alpha)
		{
			color = (color & ~(uint32_t(ALPHA_MAX) << 30)) | ((uint32_t(alpha & ALPHA_MAX)) << 30);
		}

		static constexpr void SetRed(color_t& color, const component_t red)
		{
			color = (color & ~COMPONENT_MAX) | red;
		}

		static constexpr void SetGreen(color_t& color, const component_t green)
		{
			color = (color & ~(uint32_t(COMPONENT_MAX) << 10)) | (uint32_t(green) << 10);
		}

		static constexpr void SetBlue(color_t& color, const component_t blue)
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

		static constexpr void SetAlpha8(color_t& color, const component_t alpha)
		{
			color = (color & ~(uint32_t(ALPHA_MAX) << 30)) | ((uint32_t(alpha >> 6)) << 30);
		}

		static constexpr uint8_t Alpha8(const color_t color)
		{
			// Convert the 2-bit alpha to an 8-bit value.
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
		/// Converts a RGB10 color to a RGB8 color with 8 bits per channel.
		/// </summary>
		/// <param name="color">The color value to convert, of type color_t.</param>
		/// <returns>A 32-bit unsigned integer representing the color in 0xRRGGBB format, with each channel reduced to 8 bits.</returns>
		static constexpr uint32_t Rgb8(const color_t color)
		{
			return (uint32_t((Red(color) >> 2)) << 16) |
				(uint16_t((Green(color) >> 2)) << 8) |
				((Blue(color) >> 2));
		}

		/// <summary>
		/// Converts a RGB10 color to a ARGB8 format with 8 bits per channel.
		/// </summary>
		/// <param name="color">The color value to convert, of type color_t.</param>
		/// <returns>A 32-bit unsigned integer representing the color in ARGB8 format (alpha in the highest 8 bits, followed by red, green, and blue).</returns>
		static constexpr uint32_t Argb8(const color_t color)
		{
			return (uint32_t(Alpha8(color)) << 24) |
				(uint32_t(Red(color) >> 2) << 16) |
				(uint16_t(Green(color) >> 2) << 8) |
				(Blue(color) >> 2);
		}

		/// <summary>
		/// Performs linear interpolation between two colors based on a given fraction.
		/// </summary>
		/// <param name="from">The starting color for interpolation.</param>
		/// <param name="to">The ending color for interpolation.</param>
		/// <param name="fraction">A fractional value (typically between 0 and UFRACTION16_1X) indicating the interpolation position between 'from' and 'to'.</param>
		/// <returns>A color_t value representing the interpolated color between 'from' and 'to' according to the specified fraction.</returns>
		static color_t ColorInterpolateLinear(const color_t& from, const color_t& to, const ufraction16_t fraction)
		{
			const ufraction16_t inverse = UFRACTION16_1X - fraction;

			return Color(
				Fraction::Scale(inverse, Red(from)) + Fraction::Scale(fraction, Red(to)),
				Fraction::Scale(inverse, Green(from)) + Fraction::Scale(fraction, Green(to)),
				Fraction::Scale(inverse, Blue(from)) + Fraction::Scale(fraction, Blue(to))
			);
		}

		/// <summary>
		/// Interpolates between two colors using a weighted root-mean-square (RMS) method.
		/// </summary>
		/// <param name="from">The starting color for interpolation.</param>
		/// <param name="to">The ending color for interpolation.</param>
		/// <param name="fraction">A fractional value (typically between 0 and UFRACTION16_1X) representing the interpolation weight toward the 'to' color.</param>
		/// <returns>A color_t value representing the interpolated color between 'from' and 'to' using the specified fraction.</returns>
		static color_t ColorInterpolate(const color_t& from, const color_t& to, const ufraction16_t fraction)
		{
			const ufraction16_t inverse = UFRACTION16_1X - fraction;

			component_t red = 0;
			component_t green = 0;
			component_t blue = 0;

			int32_t x = Fraction::Scale(inverse, Red(from));
			int32_t y = Fraction::Scale(fraction, Red(to));
			red = SquareRoot32((x * x) + (y * y));

			x = Fraction::Scale(inverse, Green(from));
			y = Fraction::Scale(fraction, Green(to));
			green = SquareRoot32((x * x) + (y * y));

			x = Fraction::Scale(inverse, Blue(from));
			y = Fraction::Scale(fraction, Blue(to));
			blue = SquareRoot32((x * x) + (y * y));

			return Color(red, green, blue);
		}

		/// <summary>
		/// Converts HSV color values, given as fractions, to a color_t RGB color.
		/// </summary>
		/// <param name="hue">The hue component of the color, represented as a ufraction16_t fraction.</param>
		/// <param name="saturation">The saturation component of the color, represented as a ufraction16_t fraction.</param>
		/// <param name="value">The value (brightness) component of the color, represented as a ufraction16_t fraction.</param>
		/// <returns>The resulting color in RGB format as a color_t value.</returns>
		static color_t ColorHsvFraction(const ufraction16_t hue, const ufraction16_t saturation, const ufraction16_t value)
		{
			return Hsv::TemplateHsvFraction<color_t, component_t, COMPONENT_MAX>(hue, saturation, value,
				[](const component_t red, const component_t green, const component_t blue)
				{
					return Color(red, green, blue);
				});
		}

		/// <summary>
		/// Converts HSV (Hue, Saturation, Value) color components to a color_t value.
		/// </summary>
		/// <param name="hue">The hue component of the color, specified as a Trigonometry::angle_t value.</param>
		/// <param name="saturation">The saturation component of the color, specified as an 8-bit unsigned integer (0-255).</param>
		/// <param name="value">The value (brightness) component of the color, specified as an 8-bit unsigned integer (0-255).</param>
		/// <returns>A color_t value representing the color in HSV space converted to the internal color representation.</returns>
		static color_t ColorHsv(const Trigonometry::angle_t hue, const uint8_t saturation, const uint8_t value)
		{
			// Convert uint8_t saturation and value to ufraction16_t
			const ufraction16_t hue16 = Fraction::GetUFraction16(uint16_t(hue), uint16_t(Trigonometry::ANGLE_RANGE));
			const ufraction16_t sat16 = Fraction::GetUFraction16(saturation, uint8_t(UINT8_MAX));
			const ufraction16_t val16 = Fraction::GetUFraction16(value, uint8_t(UINT8_MAX));

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