#ifndef _INTEGER_SIGNAL_COLOR_RGB8_h
#define _INTEGER_SIGNAL_COLOR_RGB8_h

#include "../Scale/Fraction.h"
#include "../Trigonometry/Trigonometry.h"
#include "Hsv.h"

namespace IntegerSignal
{
	/// <summary>
	/// Provides utilities for working with 8-bit (A)RGB colors 0xAARRGGBB packed into a 32-bit integer.
	/// Includes color creation, component extraction, modification, and interpolation.
	/// Fast and compatible color model for 8-bit RGB colors, suitable for graphics applications.
	/// </summary>
	namespace Rgb8
	{
		// RGB8 color components are represented as uint8_t.
		using component_t = uint8_t;

		// RGB8 color components maximum value.
		static constexpr component_t COMPONENT_MAX = UINT8_MAX;

		// RGB8 color is packed into a single uint32_t value.
		using color_t = uint32_t;

		using ufraction16_t = Fraction::ufraction16_t;

		/// <summary>
		/// Constructs a color value from alpha, red, green, and blue components.
		/// </summary>
		/// <param name="alpha">The alpha (transparency) component of the color.</param>
		/// <param name="red">The red component of the color.</param>
		/// <param name="green">The green component of the color.</param>
		/// <param name="blue">The blue component of the color.</param>
		/// <returns>A color_t value representing the combined ARGB color.</returns>
		static constexpr color_t Color(const component_t alpha, const component_t red, const component_t green, const component_t blue)
		{
			return (uint32_t(alpha) << 24) 
				| (uint32_t(red) << 16) 
				| (uint16_t(green) << 8) 
				| (blue);
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
			return (uint32_t(COMPONENT_MAX) << 24) 
				| (uint32_t(red) << 16)
				| (uint16_t(green) << 8)
				| (blue);
		}

		static constexpr void SetAlpha(color_t& color, const component_t alpha)
		{
			color = (color & ~(uint32_t(COMPONENT_MAX) << 24)) | (uint32_t(alpha) << 24);
		}

		static constexpr void SetRed(color_t& color, const component_t red)
		{
			color = (color & ~(uint32_t(COMPONENT_MAX) << 16)) | (uint32_t(red) << 16);
		}

		static constexpr void SetGreen(color_t& color, const component_t green)
		{
			color = (color & ~(uint32_t(COMPONENT_MAX) << 8)) | (uint32_t(green) << 8);
		}

		static constexpr void SetBlue(color_t& color, const component_t blue)
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
		/// Performs linear interpolation between two colors based on a given fraction.
		/// </summary>
		/// <param name="from">The starting color for interpolation.</param>
		/// <param name="to">The ending color for interpolation.</param>
		/// <param name="fraction">A fractional value (typically between 0 and UFRACTION16_1X) indicating the interpolation position between 'from' and 'to'.</param>
		/// <returns>A color_t value representing the interpolated color between 'from' and 'to' at the specified fraction.</returns>
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
		/// <param name="fraction">A fractional value (typically between 0 and UFRACTION16_1X) indicating the interpolation weight toward the 'to' color.</param>
		/// <returns>A color_t value representing the interpolated color between 'from' and 'to' according to the specified fraction.</returns>
		static color_t ColorInterpolate(const color_t& from, const color_t& to, const ufraction16_t fraction)
		{
			const ufraction16_t inverse = UFRACTION16_1X - fraction;

			component_t red, green, blue;

			// The largest possible square of from component is 255^2 = 65 025
			// 65 025 fits in from uint16_t(max 65 535)
			// Even for the (x^2 + y^2), the maximum value is less than 65 535 due to the relationship between inverse and fraction
			uint16_t x, y;

			// Red component
			x = Fraction::Scale(inverse, Red(from));
			y = Fraction::Scale(fraction, Red(to));
			red = SquareRoot16((x * x) + (y * y));

			// Green component
			x = Fraction::Scale(inverse, Green(from));
			y = Fraction::Scale(fraction, Green(to));
			green = SquareRoot16((x * x) + (y * y));

			// Blue component
			x = Fraction::Scale(inverse, Blue(from));
			y = Fraction::Scale(fraction, Blue(to));
			blue = SquareRoot16((x * x) + (y * y));

			return Color(red, green, blue);
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
		/// Converts HSV color values to a color_t value.
		/// </summary>
		/// <param name="hue">The hue component of the color, as an angle_t value.</param>
		/// <param name="saturation">The saturation component of the color, as an 8-bit unsigned integer (0-255).</param>
		/// <param name="value">The value (brightness) component of the color, as an 8-bit unsigned integer (0-255).</param>
		/// <returns>A color_t value representing the color in HSV space.</returns>
		static color_t ColorHsv(const Trigonometry::angle_t hue, const uint8_t saturation, const uint8_t value)
		{
			// Convert uint8_t saturation and value to ufraction16_t.
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