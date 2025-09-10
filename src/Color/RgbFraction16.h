#ifndef _INTEGER_SIGNAL_COLOR_RGB_FRACTION16_h
#define _INTEGER_SIGNAL_COLOR_RGB_FRACTION16_h

#include "../FixedPoint/Fraction.h"

namespace IntegerSignal
{
	/// <summary>
	/// Utilities for RGB colors whose components are unsigned Q-format fractions (UQ0.15).
	/// High precision and dynamic range for accurate color representation.
	/// </summary>
	namespace RgbFraction16
	{
		using namespace FixedPoint::Fraction;

		// Fraction type for components (UQ0.15).
		using fraction_t = UFraction16::scalar_t;

		// Color components are represented as UQ0.15 fractions.
		using component_t = fraction_t;

		// Maximum value for color components (unit).
		static constexpr component_t COMPONENT_MAX = UFraction16::FRACTION_1X;

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

		/// <summary>
		/// Construct a color from fractional RGB components.
		/// </summary>
		static constexpr color_t Color(const component_t red, const component_t green, const component_t blue)
		{
			return { red, green, blue };
		}

		/// <summary>
		/// Construct a fractional color from 8-bit RGB components.
		/// </summary>
		static constexpr color_t Color8(const uint8_t red, const uint8_t green, const uint8_t blue)
		{
			return color_t{
				UFraction16::GetScalar<uint8_t>(red, UINT8_MAX),
				UFraction16::GetScalar<uint8_t>(green, UINT8_MAX),
				UFraction16::GetScalar<uint8_t>(blue, UINT8_MAX)
			};
		}

		/// <summary>
		/// Construct a fractional color from 0xRRGGBB integer.
		/// </summary>
		static constexpr color_t Color8(const uint32_t rgb)
		{
			return Color8(uint8_t(rgb >> 16),
				uint8_t(rgb >> 8),
				uint8_t(rgb));
		}

		static void SetRed(color_t& color, const component_t red) { color.red = red; }
		static void SetGreen(color_t& color, const component_t green) { color.green = green; }
		static void SetBlue(color_t& color, const component_t blue) { color.blue = blue; }

		static constexpr component_t Red(const color_t color) { return color.red; }
		static constexpr component_t Green(const color_t color) { return color.green; }
		static constexpr component_t Blue(const color_t color) { return color.blue; }

		/// <summary>
		/// Convert a fractional color to ARGB8 (alpha=255), scaling each channel to 8-bit.
		/// </summary>
		static constexpr uint32_t Rgb8(const color_t color)
		{
			return (uint32_t(UINT8_MAX) << 24)
				| (uint32_t(UFraction16::Fraction(color.red, uint8_t(UINT8_MAX))) << 16)
				| (uint16_t(UFraction16::Fraction(color.green, uint8_t(UINT8_MAX))) << 8)
				| uint8_t(UFraction16::Fraction(color.blue, uint8_t(UINT8_MAX)));
		}

		/// <summary>
		/// Linear interpolation between two fractional colors using an unsigned Q-format fraction.
		/// fraction is fraction_t in [0; UFraction16::FRACTION_1X].
		/// </summary>
		static color_t ColorInterpolateLinear(const color_t& from, const color_t& to, const fraction_t fraction)
		{
			const fraction_t inverse = UFraction16::FRACTION_1X - fraction;

			return {
				component_t(UFraction16::Fraction(inverse, uint16_t(from.red)) + UFraction16::Fraction(fraction, uint16_t(to.red))),
				component_t(UFraction16::Fraction(inverse, uint16_t(from.green)) + UFraction16::Fraction(fraction, uint16_t(to.green))),
				component_t(UFraction16::Fraction(inverse, uint16_t(from.blue)) + UFraction16::Fraction(fraction, uint16_t(to.blue)))
			};
		}

		/// <summary>
		/// Weighted RMS interpolation between two fractional colors using an unsigned Q-format fraction.
		/// fraction is fraction_t in [0; UFraction16::FRACTION_1X].
		/// </summary>
		static color_t ColorInterpolate(const color_t& from, const color_t& to, const fraction_t fraction)
		{
			const fraction_t inverse = UFraction16::FRACTION_1X - fraction;

			component_t red = 0, green = 0, blue = 0;

			int32_t x = UFraction16::Fraction(inverse, Red(from));
			int32_t y = UFraction16::Fraction(fraction, Red(to));
			red = SquareRoot32((x * x) + (y * y));

			x = UFraction16::Fraction(inverse, Green(from));
			y = UFraction16::Fraction(fraction, Green(to));
			green = SquareRoot32((x * x) + (y * y));

			x = UFraction16::Fraction(inverse, Blue(from));
			y = UFraction16::Fraction(fraction, Blue(to));
			blue = SquareRoot32((x * x) + (y * y));

			return { red, green, blue };
		}

		/// <summary>
		/// Convert HSV (fraction_t) to a fractional RGB color.
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
		/// Convert HSV to a fractional RGB color where hue is an angle_t (modular full-rotation index).
		/// Saturation and value are component_t in [0; COMPONENT_MAX].
		/// </summary>
		static color_t ColorHsv(const Trigonometry::angle_t hue, const component_t saturation, const component_t value)
		{
			return ColorHsvFraction(
				UFraction16::GetScalar(uint16_t(hue), uint16_t(Trigonometry::ANGLE_RANGE)),
				saturation, value);
		}

		static constexpr color_t BLACK = { 0, 0, 0 };
		static constexpr color_t WHITE = { COMPONENT_MAX, COMPONENT_MAX, COMPONENT_MAX };
		static constexpr color_t RED = { COMPONENT_MAX, 0, 0 };
		static constexpr color_t GREEN = { 0, COMPONENT_MAX, 0 };
		static constexpr color_t BLUE = { 0, 0, COMPONENT_MAX };
	}
}
#endif