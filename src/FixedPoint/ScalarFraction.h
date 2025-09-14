#ifndef _INTEGER_SIGNAL_FIXED_POINT_FRACTION_h
#define _INTEGER_SIGNAL_FIXED_POINT_FRACTION_h

#include "QFormat/QFormatUnsigned.h"	
#include "QFormat/QFormatSigned.h"

namespace IntegerSignal
{
	namespace FixedPoint
	{
		/// <summary>
		/// Q-format fractional scalars for fixed-point math.
		/// Unsigned types represent [0, 1]; signed types represent [-1, 1].
		/// Unit (1.0) is a power-of-two value, enabling fast shift-based math.
		/// </summary>
		namespace ScalarFraction
		{
			/// <summary>
			/// UQ0.7 (8-bit unsigned, 7 fractional bits). Range: [0.0, 1.0].
			/// </summary>
			struct UFraction8 final : QFormatUnsigned::TemplateFormat<uint8_t>
			{
				using Base = QFormatUnsigned::TemplateFormat<uint8_t>;

				static constexpr auto FRACTION_1X = Base::SCALAR_UNIT;
			};

			/// <summary>
			/// UQ0.15 (16-bit unsigned, 15 fractional bits). Range: [0.0, 1.0].
			/// </summary>
			struct UFraction16 final : QFormatUnsigned::TemplateFormat<uint16_t>
			{
				using Base = QFormatUnsigned::TemplateFormat<uint16_t>;

				static constexpr auto FRACTION_1X = Base::SCALAR_UNIT;
			};

			/// <summary>
			/// UQ0.31 (32-bit unsigned, 31 fractional bits). Range: [0.0, 1.0].
			/// </summary>
			struct UFraction32 final : QFormatUnsigned::TemplateFormat<uint32_t>
			{
				using Base = QFormatUnsigned::TemplateFormat<uint32_t>;

				static constexpr auto FRACTION_1X = Base::SCALAR_UNIT;
			};

			/// <summary>
			/// Q0.6 (8-bit signed, 6 fractional bits + sign). Range: [-1.0, 1.0].
			/// </summary>
			struct Fraction8 final : QFormatSigned::TemplateFormat<int8_t>
			{
				using Base = QFormatSigned::TemplateFormat<int8_t>;

				static constexpr auto FRACTION_1X = Base::SCALAR_UNIT;
				static constexpr auto FRACTION_1X_NEGATIVE = Base::SCALAR_UNIT_NEGATIVE;
			};

			/// <summary>
			/// Q0.14 (16-bit signed, 14 fractional bits + sign). Range: [-1.0, 1.0].
			/// </summary>
			struct Fraction16 final : QFormatSigned::TemplateFormat<int16_t>
			{
				using Base = QFormatSigned::TemplateFormat<int16_t>;

				static constexpr auto FRACTION_1X = Base::SCALAR_UNIT;
				static constexpr auto FRACTION_1X_NEGATIVE = Base::SCALAR_UNIT_NEGATIVE;
			};

			/// <summary>
			/// Q0.30 (32-bit signed, 30 fractional bits + sign). Range: [-1.0, 1.0].
			/// </summary>
			struct Fraction32 final : QFormatSigned::TemplateFormat<int32_t>
			{
				using Base = QFormatSigned::TemplateFormat<int32_t>;

				static constexpr auto FRACTION_1X = Base::SCALAR_UNIT;
				static constexpr auto FRACTION_1X_NEGATIVE = Base::SCALAR_UNIT_NEGATIVE;
			};

			/// <summary>
			/// Scalar type aliases for the above formats.
			/// </summary>
			namespace ScalarAliases
			{
				using ufraction8_t = typename UFraction8::scalar_t;
				using ufraction16_t = typename UFraction16::scalar_t;
				using ufraction32_t = typename UFraction32::scalar_t;

				using fraction8_t = typename Fraction8::scalar_t;
				using fraction16_t = typename Fraction16::scalar_t;
				using fraction32_t = typename Fraction32::scalar_t;
			}

			using namespace ScalarAliases;

			/// <summary>
			/// Typed constants for common fractional values.
			/// </summary>
			namespace ConstantAliases
			{
				static constexpr ufraction8_t  UFRACTION8_1X = UFraction8::FRACTION_1X;
				static constexpr ufraction16_t UFRACTION16_1X = UFraction16::FRACTION_1X;
				static constexpr ufraction32_t UFRACTION32_1X = UFraction32::FRACTION_1X;

				static constexpr fraction8_t   FRACTION8_1X = Fraction8::FRACTION_1X;
				static constexpr fraction16_t  FRACTION16_1X = Fraction16::FRACTION_1X;
				static constexpr fraction32_t  FRACTION32_1X = Fraction32::FRACTION_1X;

				static constexpr fraction8_t   FRACTION8_NEGATIVE_1X = Fraction8::FRACTION_1X_NEGATIVE;
				static constexpr fraction16_t  FRACTION16_NEGATIVE_1X = Fraction16::FRACTION_1X_NEGATIVE;
				static constexpr fraction32_t  FRACTION32_NEGATIVE_1X = Fraction32::FRACTION_1X_NEGATIVE;
			}
			using namespace ConstantAliases;

			/// <summary>
			/// Overloads for multiplying a value by a fractional scalar.
			/// Forwards to the corresponding base implementation.
			/// </summary>
			namespace FractionAliases
			{
				template<typename T>
				static constexpr T Fraction(const ufraction8_t scalar, const T value)
				{
					return UFraction8::Base::Fraction(scalar, value);
				}

				template<typename T>
				static constexpr T Fraction(const ufraction16_t scalar, const T value)
				{
					return UFraction16::Base::Fraction(scalar, value);
				}

				template<typename T>
				static constexpr T Fraction(const ufraction32_t scalar, const T value)
				{
					return UFraction32::Base::Fraction(scalar, value);
				}

				template<typename T>
				static constexpr T Fraction(const fraction8_t scalar, const T value)
				{
					return Fraction8::Base::Fraction(scalar, value);
				}

				template<typename T>
				static constexpr T Fraction(const fraction16_t scalar, const T value)
				{
					return Fraction16::Base::Fraction(scalar, value);
				}

				template<typename T>
				static constexpr T Fraction(const fraction32_t scalar, const T value)
				{
					return Fraction32::Base::Fraction(scalar, value);
				}
			}
			using namespace FractionAliases;

			/// <summary>
			/// Helpers to compute a scalar from numerator/denominator.
			/// Unsigned results clamp to [0, 1]; signed results clamp to [-1, 1].
			/// </summary>
			namespace ScalarAliases
			{
				// Unsigned fractions: UQ0.7, UQ0.15, UQ0.31
				template<typename T>
				static constexpr ufraction8_t GetScalarU8(const T numerator, const T denominator)
				{
					return UFraction8::Base::GetScalar(numerator, denominator);
				}

				template<typename T>
				static constexpr ufraction16_t GetScalarU16(const T numerator, const T denominator)
				{
					return UFraction16::Base::GetScalar(numerator, denominator);
				}

				template<typename T>
				static constexpr ufraction32_t GetScalarU32(const T numerator, const T denominator)
				{
					return UFraction32::Base::GetScalar(numerator, denominator);
				}

				// Signed fractions: Q0.6, Q0.14, Q0.30
				template<typename T>
				static constexpr fraction8_t GetScalarS8(const T numerator, const T denominator)
				{
					return Fraction8::Base::GetScalar(numerator, denominator);
				}

				template<typename T>
				static constexpr fraction16_t GetScalarS16(const T numerator, const T denominator)
				{
					return Fraction16::Base::GetScalar(numerator, denominator);
				}

				template<typename T>
				static constexpr fraction32_t GetScalarS32(const T numerator, const T denominator)
				{
					return Fraction32::Base::GetScalar(numerator, denominator);
				}
			}
			using namespace ScalarAliases;

			/// <summary>
			/// Linear interpolation between two values using an unsigned fractional scalar.
			/// The scalar must be in [0, UFractionN::FRACTION_1X].
			/// </summary>
			namespace InterpolateAliases
			{
				template<typename T>
				static constexpr T Interpolate(const ufraction8_t scalar, const T from, const T to)
				{
					return UFraction8::Base::Interpolate(scalar, from, to);
				}

				template<typename T>
				static constexpr T Interpolate(const ufraction16_t scalar, const T from, const T to)
				{
					return UFraction16::Base::Interpolate(scalar, from, to);
				}

				template<typename T>
				static constexpr T Interpolate(const ufraction32_t scalar, const T from, const T to)
				{
					return UFraction32::Base::Interpolate(scalar, from, to);
				}
			}
			using namespace InterpolateAliases;
		}
	}
}

#endif