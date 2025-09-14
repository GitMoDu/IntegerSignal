#ifndef _INTEGER_SIGNAL_COLOR_RGB_FRACTION16_h
#define _INTEGER_SIGNAL_COLOR_RGB_FRACTION16_h

#include "../FixedPoint/ScalarFraction.h"
#include "../Trigonometry/Trigonometry.h"
#include "Hsv.h"

namespace IntegerSignal
{
	/// <summary>
	/// Utilities for RGB colors whose components are unsigned Q-format fractions (UQ0.15).
	/// High precision and dynamic range for accurate color representation.
	/// Uses a power-of-two unit (UFraction16::FRACTION_1X) enabling fast shift-based math.
	/// Provides construction, component access/mutation, interpolation, and HSV conversion.
	/// </summary>
	namespace RgbFraction16
	{
		using namespace FixedPoint::ScalarFraction;

		/// <summary>
		/// Fractional component storage type (UQ0.15).
		/// </summary>
		using component_t = ufraction16_t;

		/// <summary>
		/// Maximum component value (unit in UQ0.15).
		/// </summary>
		static constexpr component_t COMPONENT_MAX = UFraction16::FRACTION_1X;

		/// <summary>
		/// RGB color with fractional components.
		/// </summary>
		struct color_t
		{
			component_t red;
			component_t green;
			component_t blue;

			/// <summary>Get red component.</summary>
			component_t Red() const { return red; }
			/// <summary>Get green component.</summary>
			component_t Green() const { return green; }
			/// <summary>Get blue component.</summary>
			component_t Blue() const { return blue; }
		};

		/// <summary>
		/// Construct a color from fractional RGB components.
		/// </summary>
		/// <param name="red">Red in [0, COMPONENT_MAX].</param>
		/// <param name="green">Green in [0, COMPONENT_MAX].</param>
		/// <param name="blue">Blue in [0, COMPONENT_MAX].</param>
		/// <returns>Fractional color.</returns>
		static constexpr color_t Color(const component_t red, const component_t green, const component_t blue)
		{
			return { red, green, blue };
		}

		/// <summary>
		/// Construct a fractional color from 8-bit RGB components.
		/// Each channel is mapped to UQ0.15 using UFraction16::GetScalar.
		/// </summary>
		/// <param name="red">Red [0..255].</param>
		/// <param name="green">Green [0..255].</param>
		/// <param name="blue">Blue [0..255].</param>
		/// <returns>Fractional color.</returns>
		static constexpr color_t Color8(const uint8_t red, const uint8_t green, const uint8_t blue)
		{
			return color_t{
				UFraction16::GetScalar<uint8_t>(red, UINT8_MAX),
				UFraction16::GetScalar<uint8_t>(green, UINT8_MAX),
				UFraction16::GetScalar<uint8_t>(blue, UINT8_MAX)
			};
		}

		/// <summary>
		/// Construct a fractional color from 0xRRGGBB.
		/// </summary>
		/// <param name="rgb">Packed 24-bit RGB 0xRRGGBB.</param>
		/// <returns>Fractional color.</returns>
		static constexpr color_t Color8(const uint32_t rgb)
		{
			return Color8(uint8_t(rgb >> 16),
				uint8_t(rgb >> 8),
				uint8_t(rgb));
		}

		/// <summary>Set the red component.</summary>
		static void SetRed(color_t& color, const component_t red) { color.red = red; }
		/// <summary>Set the green component.</summary>
		static void SetGreen(color_t& color, const component_t green) { color.green = green; }
		/// <summary>Set the blue component.</summary>
		static void SetBlue(color_t& color, const component_t blue) { color.blue = blue; }

		/// <summary>Get the red component.</summary>
		static constexpr component_t Red(const color_t color) { return color.red; }
		/// <summary>Get the green component.</summary>
		static constexpr component_t Green(const color_t color) { return color.green; }
		/// <summary>Get the blue component.</summary>
		static constexpr component_t Blue(const color_t color) { return color.blue; }

		/// <summary>
		/// Convert a fractional color to ARGB8 (0xAARRGGBB, alpha = 255),
		/// scaling each UQ0.15 channel to 8-bit using UQ0.15's power-of-two unit.
		/// </summary>
		/// <param name="color">Fractional color.</param>
		/// <returns>Packed ARGB8 color.</returns>
		static constexpr uint32_t Rgb8(const color_t color)
		{
			return (uint32_t(UINT8_MAX) << 24)
				| (uint32_t(Fraction(color.red, uint8_t(UINT8_MAX))) << 16)
				| (uint16_t(Fraction(color.green, uint8_t(UINT8_MAX))) << 8)
				| uint8_t(Fraction(color.blue, uint8_t(UINT8_MAX)));
		}

		/// <summary>
		/// Linear interpolation between two fractional colors using a Q-format unsigned fraction (UQ0.15).
		/// </summary>
		/// <param name="from">Start color.</param>
		/// <param name="to">End color.</param>
		/// <param name="fraction">ufraction16_t in [0, UFraction16::FRACTION_1X].</param>
		/// <returns>Interpolated fractional color.</returns>
		static color_t ColorInterpolateLinear(const color_t& from, const color_t& to, const ufraction16_t fraction)
		{
			const ufraction16_t inverse = UFraction16::FRACTION_1X - fraction;

			return {
				component_t(Fraction(inverse, uint16_t(from.red)) + Fraction(fraction, uint16_t(to.red))),
				component_t(Fraction(inverse, uint16_t(from.green)) + Fraction(fraction, uint16_t(to.green))),
				component_t(Fraction(inverse, uint16_t(from.blue)) + Fraction(fraction, uint16_t(to.blue)))
			};
		}

		/// <summary>
		/// Weighted RMS interpolation between two fractional colors using a Q-format unsigned fraction (UQ0.15).
		/// For each component c: result = sqrt((w1*c1)^2 + (w2*c2)^2), where w1 = (1 - fraction), w2 = fraction.
		/// </summary>
		/// <param name="from">Start color.</param>
		/// <param name="to">End color.</param>
		/// <param name="fraction">ufraction16_t in [0, UFraction16::FRACTION_1X].</param>
		/// <returns>Interpolated fractional color.</returns>
		static color_t ColorInterpolate(const color_t& from, const color_t& to, const ufraction16_t fraction)
		{
			const ufraction16_t inverse = UFraction16::FRACTION_1X - fraction;

			component_t red = 0, green = 0, blue = 0;

			int32_t x = Fraction(inverse, Red(from));
			int32_t y = Fraction(fraction, Red(to));
			red = SquareRoot32((x * x) + (y * y));

			x = Fraction(inverse, Green(from));
			y = Fraction(fraction, Green(to));
			green = SquareRoot32((x * x) + (y * y));

			x = Fraction(inverse, Blue(from));
			y = Fraction(fraction, Blue(to));
			blue = SquareRoot32((x * x) + (y * y));

			return { red, green, blue };
		}

		/// <summary>
		/// Convert HSV (UQ0.15) to a fractional RGB color.
		/// Hue, saturation, and value are ufraction16_t in [0, UFraction16::FRACTION_1X].
		/// Hue wraps modulo unit and is internally mapped to 6 equal segments.
		/// </summary>
		/// <param name="hue">Hue in [0, UFraction16::FRACTION_1X], wraps at unit.</param>
		/// <param name="saturation">Saturation in [0, UFraction16::FRACTION_1X].</param>
		/// <param name="value">Value (brightness) in [0, UFraction16::FRACTION_1X].</param>
		/// <returns>Fractional RGB color.</returns>
		static color_t ColorHsvFraction(const ufraction16_t hue, const ufraction16_t saturation, const ufraction16_t value)
		{
			return Hsv::TemplateHsvFraction<color_t, component_t, COMPONENT_MAX>(hue, saturation, value,
				[](const component_t red, const component_t green, const component_t blue)
				{
					return Color(red, green, blue);
				});
		}

		/// <summary>
		/// Convert HSV to a fractional RGB color where hue is an angle_t (modular full-rotation index).
		/// Saturation and value are fractional components in [0, COMPONENT_MAX].
		/// Inputs are mapped as needed and delegated to ColorHsvFraction.
		/// </summary>
		/// <param name="hue">Modular angle in [0, Trigonometry::ANGLE_RANGE].</param>
		/// <param name="saturation">Saturation in [0, COMPONENT_MAX].</param>
		/// <param name="value">Value (brightness) in [0, COMPONENT_MAX].</param>
		/// <returns>Fractional RGB color.</returns>
		static color_t ColorHsv(const Trigonometry::angle_t hue, const component_t saturation, const component_t value)
		{
			return ColorHsvFraction(
				UFraction16::GetScalar(uint16_t(hue), uint16_t(Trigonometry::ANGLE_RANGE)),
				saturation, value);
		}

		/// <summary>Fractional RGB constant for black (0,0,0).</summary>
		static constexpr color_t BLACK = { 0, 0, 0 };

		/// <summary>Fractional RGB constant for white (unit,unit,unit).</summary>
		static constexpr color_t WHITE = { COMPONENT_MAX, COMPONENT_MAX, COMPONENT_MAX };

		/// <summary>Fractional RGB constant for red (unit,0,0).</summary>
		static constexpr color_t RED = { COMPONENT_MAX, 0, 0 };

		/// <summary>Fractional RGB constant for green (0,unit,0).</summary>
		static constexpr color_t GREEN = { 0, COMPONENT_MAX, 0 };

		/// <summary>Fractional RGB constant for blue (0,0,unit).</summary>
		static constexpr color_t BLUE = { 0, 0, COMPONENT_MAX };
	}
}
#endif