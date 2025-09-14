#ifndef _INTEGER_SIGNAL_FIXED_POINT_FACTOR_SCALE_h
#define _INTEGER_SIGNAL_FIXED_POINT_FACTOR_SCALE_h

#include "FactorScale/FactorScaleUnsigned.h"

namespace IntegerSignal
{
	namespace FixedPoint
	{
		/// <summary>
		/// Scale factors for general fixed-point scaling (unbounded above 1.0).
		/// Complements ScalarFraction (which represents fractions in [0,1] or [-1,1]).
		/// Uses a power-of-two unit so operations reduce to shifts and multiplications.
		/// </summary>
		namespace FactorScale
		{
			/// <summary>
			/// Unsigned fixed-point scale factor (8-bit).
			/// Precision: 4 fractional bits (16 steps per 1x).
			/// Range: [1/16x, 15x]. Unit (1.0x) = 16.
			/// </summary>
			struct Scale8 final : FactorScaleUnsigned::TemplateFormat<uint8_t>
			{
				using Base = FactorScaleUnsigned::TemplateFormat<uint8_t>;

				static constexpr uint8_t SCALE_15X = Base::SCALE_MAX;
				static constexpr uint8_t SCALE_1X = Base::SCALE_UNIT;
				static constexpr uint8_t SCALE_1_16X = Base::SCALE_MIN;
			};

			/// <summary>
			/// Unsigned fixed-point scale factor (16-bit).
			/// Precision: 8 fractional bits (256 steps per 1x).
			/// Range: [1/256x, 255x]. Unit (1.0x) = 256.
			/// </summary>
			struct Scale16 final : FactorScaleUnsigned::TemplateFormat<uint16_t>
			{
				using Base = FactorScaleUnsigned::TemplateFormat<uint16_t>;

				static constexpr uint16_t SCALE_255X = Base::SCALE_MAX;
				static constexpr uint16_t SCALE_1X = Base::SCALE_UNIT;
				static constexpr uint8_t SCALE_1_256X = Base::SCALE_MIN;
			};

			/// <summary>
			/// Unsigned fixed-point scale factor (32-bit).
			/// Precision: 16 fractional bits (65536 steps per 1x).
			/// Range: [1/65536x, 65535x]. Unit (1.0x) = 65536.
			/// </summary>
			struct Scale32 final : FactorScaleUnsigned::TemplateFormat<uint32_t>
			{
				using Base = FactorScaleUnsigned::TemplateFormat<uint32_t>;

				static constexpr uint32_t SCALE_65535X = Base::SCALE_MAX;
				static constexpr uint32_t SCALE_1X = Base::SCALE_UNIT;
				static constexpr uint8_t SCALE_1_65536X = Base::SCALE_MIN;
			};

			/// <summary>
			/// Scalar type aliases for the scale factor formats.
			/// </summary>
			namespace ScalarAliases
			{
				using scale8_t = typename Scale8::factor_t;
				using scale16_t = typename Scale16::factor_t;
				using scale32_t = typename Scale32::factor_t;
			}
			using namespace ScalarAliases;

			/// <summary>
			/// Typed constant aliases for common scale values.
			/// </summary>
			namespace ConstantAliases
			{
				static constexpr scale8_t  SCALE8_1X = Scale8::SCALE_1X;
				static constexpr scale8_t  SCALE8_1_16X = Scale8::SCALE_1_16X;
				static constexpr scale8_t  SCALE8_15X = Scale8::SCALE_15X;

				static constexpr scale16_t SCALE16_1X = Scale16::SCALE_1X;
				static constexpr scale16_t SCALE16_1_256X = Scale16::SCALE_1_256X;
				static constexpr scale16_t SCALE16_255X = Scale16::SCALE_255X;

				static constexpr scale32_t SCALE32_1X = Scale32::SCALE_1X;
				static constexpr scale32_t SCALE32_1_65536X = Scale32::SCALE_1_65536X;
				static constexpr scale32_t SCALE32_65535X = Scale32::SCALE_65535X;
			}
			using namespace ConstantAliases;

			/// <summary>
			/// Overloads for scaling a value by a scale factor.
			/// Forwards to the corresponding base implementation.
			/// </summary>
			namespace ScaleAliases
			{
				template<typename T>
				static constexpr T Scale(const scale8_t factor, const T value)
				{
					return Scale8::Base::Scale(factor, value);
				}

				template<typename T>
				static constexpr T Scale(const scale16_t factor, const T value)
				{
					return Scale16::Base::Scale(factor, value);
				}

				template<typename T>
				static constexpr T Scale(const scale32_t factor, const T value)
				{
					return Scale32::Base::Scale(factor, value);
				}
			}
			using namespace ScaleAliases;

			/// <summary>
			/// Helpers to compute a scale factor from numerator/denominator.
			/// Forwards to Base::GetFactor for each width.
			/// </summary>
			namespace FactorAliases
			{
				template<typename T>
				static constexpr scale8_t GetFactor8(const T numerator, const T denominator)
				{
					return Scale8::Base::GetFactor(numerator, denominator);
				}

				template<typename T>
				static constexpr scale16_t GetFactor16(const T numerator, const T denominator)
				{
					return Scale16::Base::GetFactor(numerator, denominator);
				}

				template<typename T>
				static constexpr scale32_t GetFactor32(const T numerator, const T denominator)
				{
					return Scale32::Base::GetFactor(numerator, denominator);
				}
			}
			using namespace FactorAliases;
		}
	}
}
#endif