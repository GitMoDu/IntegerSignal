#ifndef _INTEGER_SIGNAL_COLOR_RGB_FRACTION16_h
#define _INTEGER_SIGNAL_COLOR_RGB_FRACTION16_h

#include "../Scale/Fraction.h"

namespace IntegerSignal
{
	/// <summary>
	/// Provides utilities for working with 15-bit (A)RGB color fraction.
	/// Includes color creation, component extraction, modification, and interpolation.
	/// High precision and dynamic range color model, suitable for color applications requiring accurate color representation.
	/// </summary>
	namespace RgbFraction16
	{
		// Color components are represented as ufraction16_t.
		using component_t = Fraction::ufraction16_t;

		// Maximum value for color components.
		static constexpr component_t COMPONENT_MAX = Fraction::UFRACTION16_1X;

		// Color with red, green, and blue fraction components.
		struct color_t
		{
			component_t red;
			component_t green;
			component_t blue;

			component_t Red() const { return red; }
			component_t Green() const { return green; }
			component_t Blue() const { return blue; }
		};

		using ufraction16_t = Fraction::ufraction16_t;

		/// <summary>
		/// Creates a color value from red, green, and blue components.
		/// </summary>
		/// <param name="red">The red component of the color.</param>
		/// <param name="green">The green component of the color.</param>
		/// <param name="blue">The blue component of the color.</param>
		/// <returns>A color_t value constructed from the specified red, green, and blue components.</returns>
		static constexpr color_t Color(const component_t red, const component_t green, const component_t blue)
		{
			return { red, green, blue };
		}

		/// <summary>
		/// Creates a color_t value from 8-bit red, green, and blue components.
		/// </summary>
		/// <param name="red">The red component of the color (0-255).</param>
		/// <param name="green">The green component of the color (0-255).</param>
		/// <param name="blue">The blue component of the color (0-255).</param>
		/// <returns>A color_t value representing the specified RGB color.</returns>
		static constexpr color_t Color8(const uint8_t red, const uint8_t green, const uint8_t blue)
		{
			return { Fraction::GetUFraction16(red, UINT8_MAX),
				Fraction::GetUFraction16(green, UINT8_MAX),
				Fraction::GetUFraction16(blue, UINT8_MAX) };
		}

		/// <summary>
		/// Creates a color_t value from a 32-bit RGB integer.
		/// </summary>
		/// <param name="rgb">A 32-bit unsigned integer representing the RGB color value. The red component is in bits 16-23, green in bits 8-15, and blue in bits 0-7.</param>
		/// <returns>A color_t value constructed from the red, green, and blue components of the input.</returns>
		static constexpr color_t Color8(const uint32_t rgb)
		{
			return Color8(
				uint8_t(rgb >> 16),
				uint8_t(rgb >> 8),
				uint8_t(rgb));
		}

		static constexpr void SetRed(color_t& color, const component_t red)
		{
			color.red = red;
		}

		static constexpr void SetGreen(color_t& color, const component_t green)
		{
			color.green = green;
		}

		static constexpr void SetBlue(color_t& color, const component_t blue)
		{
			color.blue = blue;
		}

		static constexpr component_t Red(const color_t color)
		{
			return color.red;
		}

		static constexpr component_t Green(const color_t color)
		{
			return color.green;
		}

		static constexpr component_t Blue(const color_t color)
		{
			return color.blue;
		}

		/// <summary>
		/// Converts a color_t object to a 32-bit (A)RGB 0xAARRGGBB format.
		/// </summary>
		/// <param name="color">The color_t object containing red, green, and blue components to convert.</param>
		/// <returns>A 32-bit unsigned integer representing the color in 0xRRGGBB format, where the red, green, and blue components are each scaled to 8 bits.</returns>
		static constexpr uint32_t Rgb8(const color_t color)
		{
			return (uint32_t(UINT8_MAX) << 24) |
				(uint32_t(Fraction::Scale(color.red, UINT8_MAX)) << 16) |
				(uint16_t(Fraction::Scale(color.green, UINT8_MAX)) << 8) |
				uint8_t(Fraction::Scale(color.blue, UINT8_MAX));
		}

		/// <summary>
		/// Performs linear interpolation between two colors based on a fractional value.
		/// </summary>
		/// <param name="from">The starting color for interpolation.</param>
		/// <param name="to">The ending color for interpolation.</param>
		/// <param name="fraction">A fractional value (typically between 0 and UFRACTION16_1X) indicating the interpolation position between 'from' and 'to'.</param>
		/// <returns>A color_t value representing the interpolated color between 'from' and 'to' according to the specified fraction.</returns>
		static color_t ColorInterpolateLinear(const color_t& from, const color_t& to, const ufraction16_t fraction)
		{
			const ufraction16_t inverse = UFRACTION16_1X - fraction;

			return { component_t(Fraction::Scale(inverse, uint16_t(from.red)) + Fraction::Scale(fraction, uint16_t(to.red))),
					component_t(Fraction::Scale(inverse, uint16_t(from.green)) + Fraction::Scale(fraction, uint16_t(to.green))),
					component_t(Fraction::Scale(inverse, uint16_t(from.blue)) + Fraction::Scale(fraction, uint16_t(to.blue))) };
		}

		/// <summary>
		/// Interpolates between two colors using a weighted root mean square (RMS) method based on the given fraction.
		/// </summary>
		/// <param name="from">The starting color for interpolation.</param>
		/// <param name="to">The ending color for interpolation.</param>
		/// <param name="fraction">A fractional value (typically between 0 and UFRACTION16_1X) indicating the interpolation weight toward the 'to' color.</param>
		/// <returns>A color_t value representing the interpolated color between 'from' and 'to'.</returns>
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

			return { red, green, blue };
		}

		/// <summary>
		/// Converts HSV color values, given as fractions, to a color_t value.
		/// </summary>
		/// <param name="hue">The hue component of the color, represented as a ufraction16_t fraction.</param>
		/// <param name="saturation">The saturation component of the color, represented as a ufraction16_t fraction.</param>
		/// <param name="value">The value (brightness) component of the color, represented as a ufraction16_t fraction.</param>
		/// <returns>A color_t value representing the color corresponding to the specified HSV components.</returns>
		static color_t ColorHsvFraction(const ufraction16_t hue, const ufraction16_t saturation, const ufraction16_t value)
		{
			return Hsv::TemplateHsvFraction<color_t, component_t, COMPONENT_MAX>(hue, saturation, value,
				[](const component_t red, const component_t green, const component_t blue)
				{
					return Color(red, green, blue);
				});
		}

		/// <summary>
		/// Creates a color value from HSV (hue, saturation, value) components.
		/// </summary>
		/// <param name="hue">The hue angle, specifying the color's position on the color wheel.</param>
		/// <param name="saturation">The saturation component, representing the intensity or purity of the color.</param>
		/// <param name="value">The value (brightness) component, representing the brightness of the color.</param>
		/// <returns>A color_t value representing the color specified by the given HSV components.</returns>
		static color_t ColorHsv(const Trigonometry::angle_t hue, const component_t saturation, const component_t value)
		{
			return ColorHsvFraction(Fraction::GetUFraction16(uint16_t(hue), uint16_t(Trigonometry::ANGLE_RANGE))
				, saturation, value);
		}

		static constexpr color_t BLACK = { 0,0,0 };
		static constexpr color_t WHITE = { COMPONENT_MAX, COMPONENT_MAX, COMPONENT_MAX };
		static constexpr color_t RED = { COMPONENT_MAX, 0, 0 };
		static constexpr color_t GREEN = { 0, COMPONENT_MAX, 0 };
		static constexpr color_t BLUE = { 0, 0, COMPONENT_MAX };
	}
}
#endif