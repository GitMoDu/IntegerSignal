#ifndef _INTEGER_SIGNAL_FIXED_POINT_FACTOR_SCALE_h
#define _INTEGER_SIGNAL_FIXED_POINT_FACTOR_SCALE_h

#include "FactorScale/FactorScaleUnsigned.h"
#include "FactorScale/FactorScaleSigned.h"

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
			/// Unsigned fixed-point scale factor formats (8-, 16-, and 32-bit).
			/// </summary>
			using UFactorScale8 = Scale8;
			using UFactorScale16 = Scale16;
			using UFactorScale32 = Scale32;

			/// <summary>
			/// Signed fixed-point scale factor (8-bit, 4 fractional bits).
			/// Range: [-8x, 7 15/16x]. Unit (1.0x) = 16.
			/// </summary>
			struct FactorScale8 final : FactorScaleSigned::TemplateFormat<int8_t>
			{
				using Base = FactorScaleSigned::TemplateFormat<int8_t>;
			};

			/// <summary>
			/// Signed fixed-point scale factor (16-bit, 8 fractional bits).
			/// Range: [-128x, 127 255/256x]. Unit (1.0x) = 256.
			/// </summary>
			struct FactorScale16 final : FactorScaleSigned::TemplateFormat<int16_t>
			{
				using Base = FactorScaleSigned::TemplateFormat<int16_t>;
			};

			/// <summary>
			/// Signed fixed-point scale factor (32-bit, 16 fractional bits).
			/// Range: [-32768x, 32767 65535/65536x]. Unit (1.0x) = 65536.
			/// </summary>
			struct FactorScale32 final : FactorScaleSigned::TemplateFormat<int32_t>
			{
				using Base = FactorScaleSigned::TemplateFormat<int32_t>;
			};

			/// <summary>
			/// Scalar type aliases for the scale factor formats.
			/// </summary>
			namespace ScalarAliases
			{
				using scale8_t = typename Scale8::factor_t;
				using scale16_t = typename Scale16::factor_t;
				using scale32_t = typename Scale32::factor_t;

				using ufactorscale8_t = typename UFactorScale8::factor_t;
				using ufactorscale16_t = typename UFactorScale16::factor_t;
				using ufactorscale32_t = typename UFactorScale32::factor_t;

				using factorscale8_t = typename FactorScale8::factor_t;
				using factorscale16_t = typename FactorScale16::factor_t;
				using factorscale32_t = typename FactorScale32::factor_t;
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

				static constexpr ufactorscale8_t UFACTOR_SCALE8_1X = UFactorScale8::SCALE_1X;
				static constexpr ufactorscale16_t UFACTOR_SCALE16_1X = UFactorScale16::SCALE_1X;
				static constexpr ufactorscale32_t UFACTOR_SCALE32_1X = UFactorScale32::SCALE_1X;

				static constexpr factorscale8_t FACTORSCALE8_1X = FactorScale8::SCALE_1X;
				static constexpr factorscale16_t FACTORSCALE16_1X = FactorScale16::SCALE_1X;
				static constexpr factorscale32_t FACTORSCALE32_1X = FactorScale32::SCALE_1X;
				static constexpr factorscale8_t FACTORSCALE8_NEGATIVE_1X = FactorScale8::SCALE_NEGATIVE_1X;
				static constexpr factorscale16_t FACTORSCALE16_NEGATIVE_1X = FactorScale16::SCALE_NEGATIVE_1X;
				static constexpr factorscale32_t FACTORSCALE32_NEGATIVE_1X = FactorScale32::SCALE_NEGATIVE_1X;
			}
			using namespace ConstantAliases;

			/// <summary>
			/// Overloads for scaling a value by a scale factor.
			/// Forwards to the corresponding base implementation.
			/// </summary>
			namespace ScaleAliases
			{
				namespace Constexpr
				{
					template<typename T>
					static constexpr T Scale(const scale8_t factor, const T value)
					{
						return FactorScaleUnsigned::Implementation::Constexpr::Scale(factor, value);
					}

					template<typename T>
					static constexpr T Scale(const scale16_t factor, const T value)
					{
						return FactorScaleUnsigned::Implementation::Constexpr::Scale(factor, value);
					}

					template<typename T>
					static constexpr T Scale(const scale32_t factor, const T value)
					{
						return FactorScaleUnsigned::Implementation::Constexpr::Scale(factor, value);
					}

					template<typename T>
					static constexpr T Scale(const factorscale8_t factor, const T value)
					{
						return FactorScale8::Base::Scale(factor, value);
					}

					template<typename T>
					static constexpr T Scale(const factorscale16_t factor, const T value)
					{
						return FactorScale16::Base::Scale(factor, value);
					}

					template<typename T>
					static constexpr T Scale(const factorscale32_t factor, const T value)
					{
						return FactorScale32::Base::Scale(factor, value);
					}
				}

				namespace Runtime
				{
					template<typename T>
					static constexpr T Scale(const scale8_t factor, const T value)
					{
						return FactorScaleUnsigned::Implementation::Runtime::Scale(factor, value);
					}

					template<typename T>
					static constexpr T Scale(const scale16_t factor, const T value)
					{
						return FactorScaleUnsigned::Implementation::Runtime::Scale(factor, value);
					}

					template<typename T>
					static constexpr T Scale(const scale32_t factor, const T value)
					{
						return FactorScaleUnsigned::Implementation::Runtime::Scale(factor, value);
					}

					template<typename T>
					static constexpr T Scale(const factorscale8_t factor, const T value)
					{
						return FactorScale8::Base::Scale(factor, value);
					}

					template<typename T>
					static constexpr T Scale(const factorscale16_t factor, const T value)
					{
						return FactorScale16::Base::Scale(factor, value);
					}

					template<typename T>
					static constexpr T Scale(const factorscale32_t factor, const T value)
					{
						return FactorScale32::Base::Scale(factor, value);
					}
				}

				template<typename T>
				static constexpr T Scale(const scale8_t factor, const T value)
				{
					return FactorScaleUnsigned::Implementation::Policy::Scale(factor, value);
				}

				template<typename T>
				static constexpr T Scale(const scale16_t factor, const T value)
				{
					return FactorScaleUnsigned::Implementation::Policy::Scale(factor, value);
				}

				template<typename T>
				static constexpr T Scale(const scale32_t factor, const T value)
				{
					return FactorScaleUnsigned::Implementation::Policy::Scale(factor, value);
				}

				template<typename T>
				static constexpr T Scale(const factorscale8_t factor, const T value)
				{
					return FactorScale8::Base::Scale(factor, value);
				}

				template<typename T>
				static constexpr T Scale(const factorscale16_t factor, const T value)
				{
					return FactorScale16::Base::Scale(factor, value);
				}

				template<typename T>
				static constexpr T Scale(const factorscale32_t factor, const T value)
				{
					return FactorScale32::Base::Scale(factor, value);
				}
			}

			using namespace ScaleAliases;

			/// <summary>
			/// Helpers to compute a scale factor from numerator/denominator.
			/// Forwards to Base::GetFactor for each width.
			/// </summary>
			namespace Factor
			{
				namespace Constexpr
				{
					template<typename T>
					static constexpr ufactorscale8_t GetFactorU8(const T numerator, const T denominator)
					{
						return FactorScaleUnsigned::Implementation::Constexpr::GetFactor<ufactorscale8_t>(numerator, denominator);
					}

					template<typename T>
					static constexpr ufactorscale16_t GetFactorU16(const T numerator, const T denominator)
					{
						return FactorScaleUnsigned::Implementation::Constexpr::GetFactor<ufactorscale16_t>(numerator, denominator);
					}

					template<typename T>
					static constexpr ufactorscale32_t GetFactorU32(const T numerator, const T denominator)
					{
						return FactorScaleUnsigned::Implementation::Constexpr::GetFactor<ufactorscale32_t>(numerator, denominator);
					}

					template<typename T>
					static constexpr factorscale8_t GetFactorS8(const T numerator, const T denominator)
					{
						return FactorScale8::Base::GetFactor(numerator, denominator);
					}

					template<typename T>
					static constexpr factorscale16_t GetFactorS16(const T numerator, const T denominator)
					{
						return FactorScale16::Base::GetFactor(numerator, denominator);
					}

					template<typename T>
					static constexpr factorscale32_t GetFactorS32(const T numerator, const T denominator)
					{
						return FactorScale32::Base::GetFactor(numerator, denominator);
					}

					template<typename T>
					static constexpr scale8_t GetFactor8(const T numerator, const T denominator)
					{
						return FactorScaleUnsigned::Implementation::Constexpr::GetFactor<scale8_t>(numerator, denominator);
					}

					template<typename T>
					static constexpr scale16_t GetFactor16(const T numerator, const T denominator)
					{
						return FactorScaleUnsigned::Implementation::Constexpr::GetFactor<scale16_t>(numerator, denominator);
					}

					template<typename T>
					static constexpr scale32_t GetFactor32(const T numerator, const T denominator)
					{
						return FactorScaleUnsigned::Implementation::Constexpr::GetFactor<scale32_t>(numerator, denominator);
					}
				}

				namespace Runtime
				{
					template<typename T>
					inline ufactorscale8_t GetFactorU8(const T numerator, const T denominator)
					{
						return FactorScaleUnsigned::Implementation::Runtime::GetFactor8(numerator, denominator);
					}

					template<typename T>
					inline ufactorscale16_t GetFactorU16(const T numerator, const T denominator)
					{
						return FactorScaleUnsigned::Implementation::Runtime::GetFactor16(numerator, denominator);
					}

					template<typename T>
					inline ufactorscale32_t GetFactorU32(const T numerator, const T denominator)
					{
						return FactorScaleUnsigned::Implementation::Runtime::GetFactor32(numerator, denominator);
					}

					template<typename T>
					inline factorscale8_t GetFactorS8(const T numerator, const T denominator)
					{
						return FactorScale8::Base::GetFactor(numerator, denominator);
					}

					template<typename T>
					inline factorscale16_t GetFactorS16(const T numerator, const T denominator)
					{
						return FactorScale16::Base::GetFactor(numerator, denominator);
					}

					template<typename T>
					inline factorscale32_t GetFactorS32(const T numerator, const T denominator)
					{
						return FactorScale32::Base::GetFactor(numerator, denominator);
					}

					template<typename T>
					inline scale8_t GetFactor8(const T numerator, const T denominator)
					{
						return FactorScaleUnsigned::Implementation::Runtime::GetFactor8(numerator, denominator);
					}

					template<typename T>
					inline scale16_t GetFactor16(const T numerator, const T denominator)
					{
						return FactorScaleUnsigned::Implementation::Runtime::GetFactor16(numerator, denominator);
					}

					template<typename T>
					inline scale32_t GetFactor32(const T numerator, const T denominator)
					{
						return FactorScaleUnsigned::Implementation::Runtime::GetFactor32(numerator, denominator);
					}
				}

				template<typename T>
				static constexpr ufactorscale8_t GetFactorU8(const T numerator, const T denominator)
				{
					return Constexpr::GetFactorU8(numerator, denominator);
				}

				template<typename T>
				static constexpr ufactorscale16_t GetFactorU16(const T numerator, const T denominator)
				{
					return Constexpr::GetFactorU16(numerator, denominator);
				}

				template<typename T>
				static constexpr ufactorscale32_t GetFactorU32(const T numerator, const T denominator)
				{
					return Constexpr::GetFactorU32(numerator, denominator);
				}

				template<typename T>
				static constexpr factorscale8_t GetFactorS8(const T numerator, const T denominator)
				{
					return FactorScale8::Base::GetFactor(numerator, denominator);
				}

				template<typename T>
				static constexpr factorscale16_t GetFactorS16(const T numerator, const T denominator)
				{
					return FactorScale16::Base::GetFactor(numerator, denominator);
				}

				template<typename T>
				static constexpr factorscale32_t GetFactorS32(const T numerator, const T denominator)
				{
					return FactorScale32::Base::GetFactor(numerator, denominator);
				}

				template<typename T>
				static constexpr scale8_t GetFactor8(const T numerator, const T denominator)
				{
					return Constexpr::GetFactor8(numerator, denominator);
				}

				template<typename T>
				static constexpr scale16_t GetFactor16(const T numerator, const T denominator)
				{
					return Constexpr::GetFactor16(numerator, denominator);
				}

				template<typename T>
				static constexpr scale32_t GetFactor32(const T numerator, const T denominator)
				{
					return Constexpr::GetFactor32(numerator, denominator);
				}
			}

			using namespace Factor;
		}
	}
}
#endif