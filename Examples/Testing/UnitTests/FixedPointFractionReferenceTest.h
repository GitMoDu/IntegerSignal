#ifndef _FIXED_POINT_FRACTION_REFERENCE_TEST_h
#define _FIXED_POINT_FRACTION_REFERENCE_TEST_h

#include <IntegerSignalTesting.h>

namespace IntegerSignal
{
	namespace FixedPoint
	{
		namespace ScalarFraction
		{
			namespace Test
			{
				static constexpr ufraction8_t ConstexprScalarU8 = Scalar::Constexpr::GetScalarU8<uint8_t>(1u, 2u);
				static constexpr ufraction16_t ConstexprScalarU16 = Scalar::Constexpr::GetScalarU16<uint16_t>(1u, 2u);
				static constexpr ufraction32_t ConstexprScalarU32 = Scalar::Constexpr::GetScalarU32<uint32_t>(1u, 2u);
				static constexpr fraction8_t ConstexprScalarS8 = Scalar::Constexpr::GetScalarS8<int8_t>(1, 2);
				static constexpr fraction16_t ConstexprScalarS16 = Scalar::Constexpr::GetScalarS16<int16_t>(1, 2);
				static constexpr fraction32_t ConstexprScalarS32 = Scalar::Constexpr::GetScalarS32<int32_t>(1, 2);

				static constexpr uint8_t ConstexprFractionU8 = ScalarFraction::Fraction(UFRACTION8_1X, uint8_t(7));
				static constexpr uint16_t ConstexprFractionU16 = ScalarFraction::Fraction(UFRACTION16_1X, uint16_t(9));
				static constexpr uint32_t ConstexprFractionU32 = ScalarFraction::Fraction(UFRACTION32_1X, uint32_t(11));
				static constexpr int8_t ConstexprFractionS8 = ScalarFraction::Fraction(FRACTION8_1X, int8_t(7));
				static constexpr int16_t ConstexprFractionS16 = ScalarFraction::Fraction(FRACTION16_1X, int16_t(9));
				static constexpr int32_t ConstexprFractionS32 = ScalarFraction::Fraction(FRACTION32_1X, int32_t(11));

				static constexpr uint8_t ConstexprInterpolateU8 = ScalarFraction::Interpolate(UFRACTION8_1X, uint8_t(3), uint8_t(9));
				static constexpr uint16_t ConstexprInterpolateU16 = ScalarFraction::Interpolate(UFRACTION16_1X, uint16_t(5), uint16_t(13));
				static constexpr uint32_t ConstexprInterpolateU32 = ScalarFraction::Interpolate(UFRACTION32_1X, uint32_t(7), uint32_t(17));

				static_assert(ConstexprScalarU8 == UFraction8::Base::GetScalar(uint8_t(1), uint8_t(2)), "Scalar::Constexpr::GetScalarU8 must remain constexpr-capable.");
				static_assert(ConstexprScalarU16 == UFraction16::Base::GetScalar(uint16_t(1), uint16_t(2)), "Scalar::Constexpr::GetScalarU16 must remain constexpr-capable.");
				static_assert(ConstexprScalarU32 == UFraction32::Base::GetScalar(uint32_t(1), uint32_t(2)), "Scalar::Constexpr::GetScalarU32 must remain constexpr-capable.");
				static_assert(ConstexprScalarS8 == QFormatSigned::Scalar::Constexpr::GetScalarS8<int8_t>(1, 2), "Scalar::Constexpr::GetScalarS8 must remain constexpr-capable.");
				static_assert(ConstexprScalarS16 == QFormatSigned::Scalar::Constexpr::GetScalarS16<int16_t>(1, 2), "Scalar::Constexpr::GetScalarS16 must remain constexpr-capable.");
				static_assert(ConstexprScalarS32 == QFormatSigned::Scalar::Constexpr::GetScalarS32<int32_t>(1, 2), "Scalar::Constexpr::GetScalarS32 must remain constexpr-capable.");

				static_assert(ConstexprFractionU8 == uint8_t(7), "Fraction(UFRACTION8_1X, value) must remain constexpr-capable.");
				static_assert(ConstexprFractionU16 == uint16_t(9), "Fraction(UFRACTION16_1X, value) must remain constexpr-capable.");
				static_assert(ConstexprFractionU32 == uint32_t(11), "Fraction(UFRACTION32_1X, value) must remain constexpr-capable.");
				static_assert(ConstexprFractionS8 == int8_t(7), "Fraction(FRACTION8_1X, value) must remain constexpr-capable.");
				static_assert(ConstexprFractionS16 == int16_t(9), "Fraction(FRACTION16_1X, value) must remain constexpr-capable.");
				static_assert(ConstexprFractionS32 == int32_t(11), "Fraction(FRACTION32_1X, value) must remain constexpr-capable.");

				static_assert(ConstexprInterpolateU8 == uint8_t(9), "Interpolate(UFRACTION8_1X, from, to) must remain constexpr-capable.");
				static_assert(ConstexprInterpolateU16 == uint16_t(13), "Interpolate(UFRACTION16_1X, from, to) must remain constexpr-capable.");
				static_assert(ConstexprInterpolateU32 == uint32_t(17), "Interpolate(UFRACTION32_1X, from, to) must remain constexpr-capable.");

				// Reference function for Fraction8::scalar_t (signed)
				template<typename T>
				static inline T RefFraction8(const T value, const Fraction8::scalar_t fraction) {
					return (static_cast<int64_t>(value) * fraction) / Fraction8::FRACTION_1X;
				}

				template<typename T>
				static inline T RefFraction16(const T value, const Fraction16::scalar_t fraction) {
					return (static_cast<int64_t>(value) * fraction) / Fraction16::FRACTION_1X;
				}

				template<typename T>
				static inline T RefFraction32(const T value, const Fraction32::scalar_t fraction) {
					return (static_cast<int64_t>(value) * fraction) / Fraction32::FRACTION_1X;
				}

				// Reference function for UFractionX::scalar_t (unsigned)
				template<typename T>
				static inline T RefUFraction8(const T value, const UFraction8::scalar_t fraction) {
					return (static_cast<uint64_t>(value) * fraction) / UFraction8::FRACTION_1X;
				}

				template<typename T>
				static inline T RefUFraction16(const T value, const UFraction16::scalar_t fraction) {
					return (static_cast<uint64_t>(value) * fraction) / UFraction16::FRACTION_1X;
				}

				template<typename T>
				static inline T RefUFraction32(const T value, const UFraction32::scalar_t fraction) {
					return (static_cast<uint64_t>(value) * fraction) / UFraction32::FRACTION_1X;
				}

				// Reference function for GetScalar (signed Fraction8)
				static inline Fraction8::scalar_t RefGetFraction8(uint8_t numerator, uint8_t denominator)
				{
					const int16_t scaled = (denominator == 0) ? 0
						: (int16_t)(((int16_t)numerator * Fraction8::FRACTION_1X) / denominator);
					return (Fraction8::scalar_t)MinValue<int16_t>(
						Fraction8::FRACTION_1X,
						MaxValue<int16_t>(Fraction8::FRACTION_1X_NEGATIVE, scaled));
				}

				// Reference function for GetScalar (signed Fraction16)
				static inline Fraction16::scalar_t RefGetFraction16(uint8_t numerator, uint8_t denominator)
				{
					const int32_t scaled = (denominator == 0) ? 0
						: (int32_t)(((int32_t)numerator * Fraction16::FRACTION_1X) / denominator);
					return (Fraction16::scalar_t)MinValue<int32_t>(
						Fraction16::FRACTION_1X,
						MaxValue<int32_t>(Fraction16::FRACTION_1X_NEGATIVE, scaled));
				}

				// Reference function for GetScalar (signed Fraction32)
				static inline Fraction32::scalar_t RefGetFraction32(uint8_t numerator, uint8_t denominator)
				{
					const int64_t scaled = (denominator == 0) ? 0
						: (int64_t)(((int64_t)numerator * Fraction32::FRACTION_1X) / denominator);
					return (Fraction32::scalar_t)MinValue<int64_t>(
						Fraction32::FRACTION_1X,
						MaxValue<int64_t>(Fraction32::FRACTION_1X_NEGATIVE, scaled));
				}

				// Reference interpolation functions
				template<typename T>
				static inline T RefUInterpolate8(const T from, const T to, const uint8_t scalar)
				{
					constexpr uint8_t SCALE_UNIT = 128;
					return (static_cast<uint64_t>(from) * (SCALE_UNIT - scalar) +
						static_cast<uint64_t>(to) * scalar) / SCALE_UNIT;
				}
			}
		}
	}
}

#endif
