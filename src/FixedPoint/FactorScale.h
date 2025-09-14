#ifndef _INTEGER_SIGNAL_FIXED_POINT_FACTOR_SCALE_h
#define _INTEGER_SIGNAL_FIXED_POINT_FACTOR_SCALE_h

#include "FactorScale/FactorScaleUnsigned.h"

namespace IntegerSignal
{
	namespace FixedPoint
	{
		namespace FactorScale
		{
			/// <summary>
			/// 8-bit unsigned scale factor for fixed-point scaling operations.
			/// Provides scaling with 4-bit precision (16 distinct steps).
			/// 
			/// Scale range: 1/16x (0.0625) to 15x
			/// Unit scale (1.0x): 16
			/// </summary>
			struct Scale8 final : FactorScaleUnsigned::TemplateFormat<uint8_t>
			{
				using Base = FactorScaleUnsigned::TemplateFormat<uint8_t>;

				static constexpr uint8_t SCALE_15X = Base::SCALE_MAX;
				static constexpr uint8_t SCALE_1X = Base::SCALE_UNIT;
				static constexpr uint8_t SCALE_1_16X = Base::SCALE_MIN;
			};

			/// <summary>
			/// 16-bit unsigned scale factor for fixed-point scaling operations.
			/// Provides scaling with 8-bit precision (256 distinct steps).
			/// 
			/// Scale range: 1/256x (0.00390625) to 255x
			/// Unit scale (1.0x): 256
			/// </summary>
			struct Scale16 final : FactorScaleUnsigned::TemplateFormat<uint16_t>
			{
				using Base = FactorScaleUnsigned::TemplateFormat<uint16_t>;

				static constexpr uint16_t SCALE_255X = Base::SCALE_MAX;
				static constexpr uint16_t SCALE_1X = Base::SCALE_UNIT;
				static constexpr uint8_t SCALE_1_256X = Base::SCALE_MIN;
			};

			/// <summary>
			/// 32-bit unsigned scale factor for fixed-point scaling operations.
			/// Provides scaling with 16-bit precision (65536 distinct steps).
			/// 
			/// Scale range: 1/65536x (0.0000152587890625) to 65535x
			/// Unit scale (1.0x): 65536
			/// </summary>
			struct Scale32 final : FactorScaleUnsigned::TemplateFormat<uint32_t>
			{
				using Base = FactorScaleUnsigned::TemplateFormat<uint32_t>;

				static constexpr uint32_t SCALE_65535X = Base::SCALE_MAX;
				static constexpr uint32_t SCALE_1X = Base::SCALE_UNIT;
				static constexpr uint8_t SCALE_1_65536X = Base::SCALE_MIN;
			};

			// Scalar type aliases
			namespace ScalarAliases
			{
				using scale8_t = typename Scale8::factor_t;
				using scale16_t = typename Scale16::factor_t;
				using scale32_t = typename Scale32::factor_t;
			}
			using namespace ScalarAliases;

			// Constant aliases (typed to the respective scalar types)
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

			// Function aliases for scaling, overloaded by factor width
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