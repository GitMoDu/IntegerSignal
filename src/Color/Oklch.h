#ifndef _INTEGER_SIGNAL_COLOR_OKLCH_h
#define _INTEGER_SIGNAL_COLOR_OKLCH_h

#include "../Base/Operation.h"
#include "../Base/SquareRoot.h"
#include "../FixedPoint/ScalarFraction.h"
#include "../Trigonometry/Sine16.h"

namespace IntegerSignal
{
	namespace Oklch
	{
		using namespace FixedPoint::ScalarFraction;

		/// <summary>
		/// Oklch color with lightness and chroma in UQ0.15 scale and hue in the modular angle domain.
		/// Chroma may exceed one UQ0.15 unit; lightness is in [0, UFraction16::FRACTION_1X].
		/// </summary>
		struct oklch_t
		{
			ufraction16_t lightness;
			ufraction16_t chroma;
			Trigonometry::angle_t hue;
		};

		namespace Implementation
		{
			namespace Constants
			{
				static constexpr uint8_t Q15_FRACTION_BITS = 15;
				static constexpr int32_t Q15_UNIT = UFraction16::FRACTION_1X;
				static constexpr uint8_t CUBE_ROOT_TARGET_SHIFT = Q15_FRACTION_BITS * 2;
				static constexpr uint8_t CORDIC_ITERATION_COUNT = 15;
				static constexpr int32_t CORDIC_HALF_TURN = 32768;
				static constexpr int32_t CHROMA_FAST_COMPONENT_LIMIT = Q15_UNIT - 1;

				namespace RgbToLms
				{
					static constexpr int32_t L_RED = 13508;
					static constexpr int32_t L_GREEN = 17574;
					static constexpr int32_t L_BLUE = 1686;
					static constexpr int32_t M_RED = 6944;
					static constexpr int32_t M_GREEN = 22305;
					static constexpr int32_t M_BLUE = 3519;
					static constexpr int32_t S_RED = 2894;
					static constexpr int32_t S_GREEN = 9231;
					static constexpr int32_t S_BLUE = 20643;
				}

				namespace LmsToOklab
				{
					static constexpr int32_t LIGHTNESS_L = 6896;
					static constexpr int32_t LIGHTNESS_M = 26005;
					static constexpr int32_t LIGHTNESS_S = -133;
					static constexpr int32_t A_L = 64815;
					static constexpr int32_t A_M = -79580;
					static constexpr int32_t A_S = 14765;
					static constexpr int32_t B_L = 849;
					static constexpr int32_t B_M = 25650;
					static constexpr int32_t B_S = -26499;
				}

				namespace OklabToLms
				{
					static constexpr int32_t L_LIGHTNESS = Q15_UNIT;
					static constexpr int32_t L_A = 12987;
					static constexpr int32_t L_B = 7072;
					static constexpr int32_t M_LIGHTNESS = Q15_UNIT;
					static constexpr int32_t M_A = -3459;
					static constexpr int32_t M_B = -2092;
					static constexpr int32_t S_LIGHTNESS = Q15_UNIT;
					static constexpr int32_t S_A = -2932;
					static constexpr int32_t S_B = -42319;
				}

				namespace LmsToRgb
				{
					static constexpr int32_t RED_L = 133587;
					static constexpr int32_t RED_M = -108387;
					static constexpr int32_t RED_S = 7568;
					static constexpr int32_t GREEN_L = -41564;
					static constexpr int32_t GREEN_M = 85517;
					static constexpr int32_t GREEN_S = -11184;
					static constexpr int32_t BLUE_L = -137;
					static constexpr int32_t BLUE_M = -23050;
					static constexpr int32_t BLUE_S = 55955;
				}

				namespace CordicAngleSteps
				{
					static constexpr int16_t ITERATION_0 = 8192;
					static constexpr int16_t ITERATION_1 = 4836;
					static constexpr int16_t ITERATION_2 = 2555;
					static constexpr int16_t ITERATION_3 = 1297;
					static constexpr int16_t ITERATION_4 = 651;
					static constexpr int16_t ITERATION_5 = 326;
					static constexpr int16_t ITERATION_6 = 163;
					static constexpr int16_t ITERATION_7 = 81;
					static constexpr int16_t ITERATION_8 = 41;
					static constexpr int16_t ITERATION_9 = 20;
					static constexpr int16_t ITERATION_10 = 10;
					static constexpr int16_t ITERATION_11 = 5;
					static constexpr int16_t ITERATION_12 = 3;
					static constexpr int16_t ITERATION_13 = 1;
					static constexpr int16_t ITERATION_14 = 1;
				}
			}

			static inline int32_t MultiplyQ15(const int32_t value, const int32_t scalar)
			{
				const uint32_t magnitude = scalar < 0 ? uint32_t(-int64_t(scalar)) : uint32_t(scalar);
				const uint32_t whole = magnitude / Constants::Q15_UNIT;
				const ufraction16_t fraction = ufraction16_t(magnitude % Constants::Q15_UNIT);
				const int32_t result = int32_t(whole) * value + Fraction(fraction, value);
				return scalar < 0 ? -result : result;
			}

			static inline int32_t MatrixQ15(const int32_t x, const int32_t y, const int32_t z,
				const int32_t cx, const int32_t cy, const int32_t cz)
			{
				return MultiplyQ15(x, cx) + MultiplyQ15(y, cy) + MultiplyQ15(z, cz);
			}

			static inline int32_t CubeQ15(const int32_t value)
			{
				return MultiplyQ15(MultiplyQ15(value, value), value);
			}

			static inline int32_t CubeRootQ15(const int32_t value)
			{
				const uint32_t bounded = uint32_t(LimitValue<int32_t>(value, 0, Constants::Q15_UNIT));
				const uint64_t target = uint64_t(bounded) << Constants::CUBE_ROOT_TARGET_SHIFT;
				uint32_t low = 0;
				uint32_t high = Constants::Q15_UNIT;

				while (low < high)
				{
					const uint32_t middle = (low + high + 1) >> 1;
					const uint64_t cube = uint64_t(middle * middle) * middle;
					if (cube <= target)
					{
						low = middle;
					}
					else
					{
						high = middle - 1;
					}
				}

				return int32_t(low);
			}

			static inline uint16_t CordicAngle(const int32_t a, const int32_t b)
			{
				if (a == 0 && b == 0)
				{
					return 0;
				}

				int32_t x = a;
				int32_t y = b;
				int32_t angle = 0;
				if (x < 0)
				{
					x = -x;
					y = -y;
					angle = Constants::CORDIC_HALF_TURN;
				}

				for (uint8_t iteration = 0; iteration < Constants::CORDIC_ITERATION_COUNT; ++iteration)
				{
					int16_t angleStep;
					switch (iteration)
					{
					case 0: angleStep = Constants::CordicAngleSteps::ITERATION_0; break;
					case 1: angleStep = Constants::CordicAngleSteps::ITERATION_1; break;
					case 2: angleStep = Constants::CordicAngleSteps::ITERATION_2; break;
					case 3: angleStep = Constants::CordicAngleSteps::ITERATION_3; break;
					case 4: angleStep = Constants::CordicAngleSteps::ITERATION_4; break;
					case 5: angleStep = Constants::CordicAngleSteps::ITERATION_5; break;
					case 6: angleStep = Constants::CordicAngleSteps::ITERATION_6; break;
					case 7: angleStep = Constants::CordicAngleSteps::ITERATION_7; break;
					case 8: angleStep = Constants::CordicAngleSteps::ITERATION_8; break;
					case 9: angleStep = Constants::CordicAngleSteps::ITERATION_9; break;
					case 10: angleStep = Constants::CordicAngleSteps::ITERATION_10; break;
					case 11: angleStep = Constants::CordicAngleSteps::ITERATION_11; break;
					case 12: angleStep = Constants::CordicAngleSteps::ITERATION_12; break;
					case 13: angleStep = Constants::CordicAngleSteps::ITERATION_13; break;
					case 14: angleStep = Constants::CordicAngleSteps::ITERATION_14; break;
					default: angleStep = 0; break;
					}

					const int32_t previousX = x;
					if (y > 0)
					{
						x += y >> iteration;
						y -= previousX >> iteration;
						angle += angleStep;
					}
					else if (y < 0)
					{
						x -= y >> iteration;
						y += previousX >> iteration;
						angle -= angleStep;
					}
				}

				return uint16_t(angle);
			}

			static inline oklch_t FromLmsRoots(const int32_t lRoot, const int32_t mRoot, const int32_t sRoot)
			{
				const int32_t lightness = MatrixQ15(lRoot, mRoot, sRoot,
					Constants::LmsToOklab::LIGHTNESS_L, Constants::LmsToOklab::LIGHTNESS_M, Constants::LmsToOklab::LIGHTNESS_S);
				const int32_t a = MatrixQ15(lRoot, mRoot, sRoot,
					Constants::LmsToOklab::A_L, Constants::LmsToOklab::A_M, Constants::LmsToOklab::A_S);
				const int32_t b = MatrixQ15(lRoot, mRoot, sRoot,
					Constants::LmsToOklab::B_L, Constants::LmsToOklab::B_M, Constants::LmsToOklab::B_S);
				const uint32_t magnitudeA = a < 0 ? 0u - uint32_t(a) : uint32_t(a);
				const uint32_t magnitudeB = b < 0 ? 0u - uint32_t(b) : uint32_t(b);
				const uint32_t chroma = magnitudeA <= Constants::CHROMA_FAST_COMPONENT_LIMIT
					&& magnitudeB <= Constants::CHROMA_FAST_COMPONENT_LIMIT
					? SquareRoot32(magnitudeA * magnitudeA + magnitudeB * magnitudeB)
					: SquareRoot64(uint64_t(int64_t(a) * a) + uint64_t(int64_t(b) * b));

				return {
					ufraction16_t(LimitValue<int32_t>(lightness, 0, Constants::Q15_UNIT)),
					ufraction16_t(LimitValue<uint32_t>(chroma, 0, UINT16_MAX)),
					Trigonometry::angle_t(CordicAngle(a, b))
				};
			}

			static inline oklch_t FromLinearRgb(const ufraction16_t red, const ufraction16_t green,
				const ufraction16_t blue)
			{
				const int32_t lRoot = CubeRootQ15(MatrixQ15(red, green, blue,
					Constants::RgbToLms::L_RED, Constants::RgbToLms::L_GREEN, Constants::RgbToLms::L_BLUE));
				const int32_t mRoot = CubeRootQ15(MatrixQ15(red, green, blue,
					Constants::RgbToLms::M_RED, Constants::RgbToLms::M_GREEN, Constants::RgbToLms::M_BLUE));
				const int32_t sRoot = CubeRootQ15(MatrixQ15(red, green, blue,
					Constants::RgbToLms::S_RED, Constants::RgbToLms::S_GREEN, Constants::RgbToLms::S_BLUE));

				return FromLmsRoots(lRoot, mRoot, sRoot);
			}

			template<typename component_t, uint32_t COMPONENT_MAX>
			static inline oklch_t FromNativeRgb(const component_t red, const component_t green, const component_t blue)
			{
				return FromLinearRgb(
					UFraction16::GetScalar<component_t>(red, component_t(COMPONENT_MAX)),
					UFraction16::GetScalar<component_t>(green, component_t(COMPONENT_MAX)),
					UFraction16::GetScalar<component_t>(blue, component_t(COMPONENT_MAX)));
			}

			template<typename color_t, typename component_t, component_t COMPONENT_MAX, typename ColorFunc>
			static inline color_t ToRgb(const ufraction16_t lightness, const ufraction16_t chroma,
				const Trigonometry::angle_t hue, ColorFunc&& colorFunc)
			{
				const fraction16_t sine = Trigonometry::Sine16(hue);
				const fraction16_t cosine = Trigonometry::Cosine16(hue);
				const int32_t a = Fraction(cosine, int32_t(chroma));
				const int32_t b = Fraction(sine, int32_t(chroma));

				const int32_t lRoot = MatrixQ15(lightness, a, b,
					Constants::OklabToLms::L_LIGHTNESS, Constants::OklabToLms::L_A, Constants::OklabToLms::L_B);
				const int32_t mRoot = MatrixQ15(lightness, a, b,
					Constants::OklabToLms::M_LIGHTNESS, Constants::OklabToLms::M_A, Constants::OklabToLms::M_B);
				const int32_t sRoot = MatrixQ15(lightness, a, b,
					Constants::OklabToLms::S_LIGHTNESS, Constants::OklabToLms::S_A, Constants::OklabToLms::S_B);
				const int32_t l = CubeQ15(lRoot);
				const int32_t m = CubeQ15(mRoot);
				const int32_t s = CubeQ15(sRoot);

				const ufraction16_t red = ufraction16_t(LimitValue<int32_t>(MatrixQ15(l, m, s,
					Constants::LmsToRgb::RED_L, Constants::LmsToRgb::RED_M, Constants::LmsToRgb::RED_S), 0, Constants::Q15_UNIT));
				const ufraction16_t green = ufraction16_t(LimitValue<int32_t>(MatrixQ15(l, m, s,
					Constants::LmsToRgb::GREEN_L, Constants::LmsToRgb::GREEN_M, Constants::LmsToRgb::GREEN_S), 0, Constants::Q15_UNIT));
				const ufraction16_t blue = ufraction16_t(LimitValue<int32_t>(MatrixQ15(l, m, s,
					Constants::LmsToRgb::BLUE_L, Constants::LmsToRgb::BLUE_M, Constants::LmsToRgb::BLUE_S), 0, Constants::Q15_UNIT));

				return colorFunc(component_t(Fraction(red, COMPONENT_MAX)),
					component_t(Fraction(green, COMPONENT_MAX)), component_t(Fraction(blue, COMPONENT_MAX)));
			}
		}

		/// <summary>
		/// Convert linear-light RGB components to Oklch using integer arithmetic.
		/// </summary>
		/// <param name="red">Linear-light red component in UQ0.15.</param>
		/// <param name="green">Linear-light green component in UQ0.15.</param>
		/// <param name="blue">Linear-light blue component in UQ0.15.</param>
		/// <returns>Oklch color.</returns>
		static inline oklch_t FromLinearRgb(const ufraction16_t red, const ufraction16_t green,
			const ufraction16_t blue)
		{
			return Implementation::FromLinearRgb(red, green, blue);
		}

		template<typename rgb_component_t, uint32_t COMPONENT_MAX>
		static inline oklch_t FromRgb(const rgb_component_t red, const rgb_component_t green,
			const rgb_component_t blue)
		{
			return Implementation::FromNativeRgb<rgb_component_t, COMPONENT_MAX>(red, green, blue);
		}

		/// <summary>
		/// Convert Oklch to linear-light RGB components. Out-of-gamut results are clamped to [0, 1].
		/// </summary>
		/// <typeparam name="color_t">Concrete color model's return type.</typeparam>
		/// <typeparam name="component_t">Concrete color model's component type.</typeparam>
		/// <typeparam name="COMPONENT_MAX">Maximum component value for the concrete color model.</typeparam>
		/// <typeparam name="ColorFunc">Callable constructing the concrete color from RGB components.</typeparam>
		template<typename color_t, typename component_t, component_t COMPONENT_MAX, typename ColorFunc>
		static inline color_t TemplateOklch(const ufraction16_t lightness, const ufraction16_t chroma,
			const Trigonometry::angle_t hue, ColorFunc&& colorFunc)
		{
			return Implementation::ToRgb<color_t, component_t, COMPONENT_MAX>(lightness, chroma, hue, colorFunc);
		}
	}
}

#endif