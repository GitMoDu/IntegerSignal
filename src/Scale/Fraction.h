#ifndef _INTEGER_SIGNAL_FRACTION_h
#define _INTEGER_SIGNAL_FRACTION_h

#include <stdint.h>

namespace IntegerSignal
{
	namespace Fraction
	{
		/// <summary>
		/// [0; UINT8_MAX].
		/// Corresponds to [0.0 ; +1.0] unsigned fraction scale.
		/// </summary>
		typedef uint8_t ufraction8_t;

		/// <summary>
		/// [0 ; UINT16_MAX].
		/// Corresponds to [0.0 ; +1.0] unsigned fraction scale.
		/// </summary>
		typedef uint16_t ufraction16_t;

		/// <summary>
		/// [0; UINT32_MAX].
		/// Corresponds to [0.0 ; +1.0] unsigned fraction scale.
		/// </summary>
		typedef uint32_t ufraction32_t;

		/// <summary>
		/// [-INT8_MAX ; INT8_MAX].
		/// Corresponds to [-1.0 ; +1.0] signed fraction scale.
		/// </summary>
		typedef int8_t fraction8_t;

		/// <summary>
		/// [-INT16_MAX ; INT16_MAX].
		/// Corresponds to [-1.0 ; +1.0] signed fraction scale.
		/// </summary>
		typedef int16_t fraction16_t;

		/// <summary>
		/// [-INT32_MAX ; INT32_MAX].
		/// Corresponds to [-1.0 ; +1.0] signed fraction scale.
		/// </summary>
		typedef int32_t fraction32_t;


		static constexpr ufraction8_t UFRACTION8_1X = ((uint16_t)INT8_MAX + 1);
		static constexpr ufraction16_t UFRACTION16_1X = ((uint32_t)INT16_MAX + 1);
		static constexpr ufraction32_t UFRACTION32_1X = ((uint64_t)INT32_MAX + 1);


		static constexpr fraction8_t FRACTION8_1X = UFRACTION8_1X >> 1;
		static constexpr fraction8_t FRACTION8_NEGATIVE_1X = -FRACTION8_1X;

		static constexpr fraction16_t FRACTION16_1X = UFRACTION16_1X >> 1;
		static constexpr fraction16_t FRACTION16_NEGATIVE_1X = -FRACTION16_1X;

		static constexpr fraction32_t FRACTION32_1X = UFRACTION32_1X >> 1;
		static constexpr fraction32_t FRACTION32_NEGATIVE_1X = -FRACTION32_1X;

		/// <summary>
		/// Calculator for fractions based on numerator/denominator ratio.
		/// </summary>
		/// <param name="numerator">Ratio multiplier.</param>
		/// <param name="denominator">Ratio divider.</param>
		/// <returns>Fraction scale to be used with FractionN().</returns>
		namespace FractionCalculator
		{
			static constexpr ufraction8_t GetUFraction8(const uint8_t value, const uint8_t reference)
			{
				return MinValue(UFRACTION8_1X,
					(uint8_t)(((uint16_t)value << GetBitShifts(UFRACTION8_1X)) / reference));
			}

			static constexpr ufraction8_t GetUFraction8(const uint16_t value, const uint16_t reference)
			{
				return MinValue(UFRACTION8_1X,
					(uint8_t)(((uint32_t)value << GetBitShifts(UFRACTION8_1X)) / reference));
			}

			static constexpr ufraction8_t GetUFraction8(const uint32_t value, const uint32_t reference)
			{
				return MinValue(UFRACTION8_1X,
					(uint8_t)(((uint64_t)value << GetBitShifts(UFRACTION8_1X)) / reference));
			}

			static constexpr ufraction16_t GetUFraction16(const uint8_t value, const uint8_t reference)
			{
				return MinValue(UFRACTION16_1X,
					(uint16_t)(((uint32_t)value << GetBitShifts(UFRACTION16_1X)) / reference));
			}

			static constexpr ufraction16_t GetUFraction16(const uint16_t value, const uint16_t reference)
			{
				return MinValue(UFRACTION16_1X,
					(uint16_t)(((uint32_t)value << GetBitShifts(UFRACTION16_1X)) / reference));
			}

			static constexpr ufraction16_t GetUFraction16(const uint32_t value, const uint32_t reference)
			{
				return MinValue(UFRACTION16_1X,
					(uint16_t)(((uint64_t)value << GetBitShifts(UFRACTION16_1X)) / reference));
			}

			static constexpr ufraction32_t GetUFraction32(const uint8_t value, const uint8_t reference)
			{
				return MinValue(UFRACTION32_1X,
					(uint32_t)(((uint64_t)value << GetBitShifts(UFRACTION32_1X)) / reference));
			}

			static constexpr ufraction32_t GetUFraction32(const uint16_t value, const uint16_t reference)
			{
				return MinValue(UFRACTION32_1X,
					(uint32_t)(((uint64_t)value << GetBitShifts(UFRACTION32_1X)) / reference));
			}

			static constexpr ufraction32_t GetUFraction32(const uint32_t value, const uint32_t reference)
			{
				return MinValue(UFRACTION32_1X,
					(uint32_t)(((uint64_t)value << GetBitShifts(UFRACTION32_1X)) / reference));
			}


			static constexpr fraction8_t GetFraction8(const uint8_t value, const uint8_t reference)
			{
				return ((int16_t)value * FRACTION8_1X) / reference;
			}

			static constexpr fraction8_t GetFraction8(const int8_t value, const int8_t reference)
			{
				return MaxValue((int16_t)FRACTION8_NEGATIVE_1X, (int16_t)(((int16_t)value * FRACTION8_1X) / reference));
			}

			static constexpr fraction8_t GetFraction8(const uint16_t value, const uint16_t reference)
			{
				return ((int32_t)value * FRACTION8_1X) / reference;
			}

			static constexpr fraction8_t GetFraction8(const int16_t value, const int16_t reference)
			{
				return MaxValue((int32_t)FRACTION8_NEGATIVE_1X, (int32_t)(((int32_t)value * FRACTION8_1X) / reference));
			}

			static constexpr fraction8_t GetFraction8(const uint32_t value, const uint32_t reference)
			{
				return ((int64_t)value * FRACTION8_1X) / reference;
			}

			static constexpr fraction8_t GetFraction8(const int32_t value, const int32_t reference)
			{
				return MaxValue((int64_t)FRACTION8_NEGATIVE_1X, (int64_t)(((int64_t)value * FRACTION8_1X) / reference));
			}

			static constexpr fraction16_t GetFraction16(const uint8_t value, const uint8_t reference)
			{
				return ((int32_t)value * FRACTION16_1X) / reference;
			}

			static constexpr fraction16_t GetFraction16(const int8_t value, const int8_t reference)
			{
				return MaxValue((int32_t)FRACTION16_NEGATIVE_1X, (int32_t)(((int32_t)value * FRACTION16_1X) / reference));
			}

			static constexpr fraction16_t GetFraction16(const uint16_t value, const uint16_t reference)
			{
				return ((int32_t)value * FRACTION16_1X) / reference;
			}

			static constexpr fraction16_t GetFraction16(const int16_t value, const int16_t reference)
			{
				return MaxValue((int32_t)FRACTION16_NEGATIVE_1X, (int32_t)(((int32_t)value * FRACTION16_1X) / reference));
			}

			static constexpr fraction16_t GetFraction16(const uint32_t value, const uint32_t reference)
			{
				return ((uint64_t)value * FRACTION16_1X) / reference;
			}

			static constexpr fraction16_t GetFraction16(const int32_t value, const int32_t reference)
			{
				return MaxValue((int64_t)FRACTION16_NEGATIVE_1X, (int64_t)(((int64_t)value * FRACTION16_1X) / reference));
			}

			static constexpr fraction32_t GetFraction32(const uint8_t value, const uint8_t reference)
			{
				return ((uint64_t)value * FRACTION32_1X) / reference;
			}

			static constexpr fraction32_t GetFraction32(const int8_t value, const int8_t reference)
			{
				return MaxValue((int64_t)FRACTION32_NEGATIVE_1X, (int64_t)(((int64_t)value * FRACTION32_1X) / reference));
			}

			static constexpr fraction32_t GetFraction32(const uint16_t value, const uint16_t reference)
			{
				return ((uint64_t)value * FRACTION32_1X) / reference;
			}

			static constexpr fraction32_t GetFraction32(const int16_t value, const int16_t reference)
			{
				return MaxValue((int64_t)FRACTION32_NEGATIVE_1X, (int64_t)(((int64_t)value * FRACTION32_1X) / reference));
			}

			static constexpr fraction32_t GetFraction32(const uint32_t value, const uint32_t reference)
			{
				return ((int64_t)value * FRACTION32_1X) / reference;
			}

			static constexpr fraction32_t GetFraction32(const int32_t value, const int32_t reference)
			{
				return MaxValue((int64_t)FRACTION32_NEGATIVE_1X, (int64_t)(((int64_t)value * FRACTION32_1X) / reference));
			}
		}

		using namespace FractionCalculator;

		static constexpr uint8_t Scale(const ufraction8_t fraction, const uint8_t value)
		{
			return ((uint16_t)value * fraction) >> GetBitShifts(UFRACTION8_1X);
		}

		static constexpr uint16_t Scale(const ufraction8_t fraction, const uint16_t value)
		{
			return ((uint32_t)value * fraction) >> GetBitShifts(UFRACTION8_1X);
		}

		static constexpr uint32_t Scale(const ufraction8_t fraction, const uint32_t value)
		{
			return ((uint64_t)value * fraction) >> GetBitShifts(UFRACTION8_1X);
		}

		static constexpr uint8_t Scale(const ufraction16_t fraction, const uint8_t value)
		{
			return ((uint32_t)value * fraction) >> GetBitShifts(UFRACTION16_1X);
		}

		static constexpr uint16_t Scale(const ufraction16_t fraction, const uint16_t value)
		{
			return ((uint32_t)value * fraction) >> GetBitShifts(UFRACTION16_1X);
		}

		static constexpr uint32_t Scale(const ufraction16_t fraction, const uint32_t value)
		{
			return ((uint64_t)value * fraction) >> GetBitShifts(UFRACTION16_1X);
		}

		static constexpr uint8_t Scale(const ufraction32_t fraction, const uint8_t value)
		{
			return ((uint64_t)value * fraction) >> GetBitShifts(UFRACTION32_1X);
		}

		static constexpr uint16_t Scale(const ufraction32_t fraction, const uint16_t value)
		{
			return ((uint64_t)value * fraction) >> GetBitShifts(UFRACTION32_1X);
		}

		static constexpr uint32_t Scale(const ufraction32_t fraction, const uint32_t value)
		{
			return ((uint64_t)value * fraction) >> GetBitShifts(UFRACTION32_1X);
		}

		static constexpr int8_t Scale(const fraction8_t fraction, const int8_t value)
		{
			return SignedRightShift((int16_t)fraction * value, GetBitShifts(FRACTION8_1X));
		}

		static constexpr int16_t Scale(const fraction8_t fraction, const int16_t value)
		{
			return SignedRightShift((int32_t)fraction * value, GetBitShifts(FRACTION8_1X));
		}

		static constexpr int32_t Scale(const fraction8_t fraction, const int32_t value)
		{
			return SignedRightShift((int64_t)fraction * value, GetBitShifts(FRACTION8_1X));
		}

		static constexpr int8_t Scale(const fraction16_t fraction, const int8_t value)
		{
			return SignedRightShift((int32_t)fraction * value, GetBitShifts(FRACTION16_1X));
		}

		static constexpr int16_t Scale(const fraction16_t fraction, const int16_t value)
		{
			return SignedRightShift((int32_t)fraction * value, GetBitShifts(FRACTION16_1X));
		}

		static constexpr int32_t Scale(const fraction16_t fraction, const int32_t value)
		{
			return SignedRightShift((int64_t)fraction * value, GetBitShifts(FRACTION16_1X));
		}

		static constexpr int8_t Scale(const fraction32_t fraction, const int8_t value)
		{
			return SignedRightShift((int64_t)fraction * value, GetBitShifts(FRACTION32_1X));
		}

		static constexpr int16_t Scale(const fraction32_t fraction, const int16_t value)
		{
			return SignedRightShift((int64_t)fraction * value, GetBitShifts(FRACTION32_1X));
		}

		static constexpr int32_t Scale(const fraction32_t fraction, const int32_t value)
		{
			return SignedRightShift((int64_t)fraction * value, GetBitShifts(FRACTION32_1X));
		}

		template<typename T>
		static constexpr T Interpolate(const ufraction8_t fraction, const T from, const T to)
		{
			return Fraction::Scale(fraction, to) + Fraction::Scale(ufraction8_t(UFRACTION8_1X - fraction), from);
		}

		template<typename T>
		static constexpr T Interpolate(const ufraction16_t fraction, const T from, const T to)
		{
			return Fraction::Scale(fraction, to) + Fraction::Scale(ufraction16_t(UFRACTION16_1X - fraction), from);
		}

		template<typename T>
		static constexpr T Interpolate(const ufraction32_t fraction, const T from, const T to)
		{
			return Fraction::Scale(fraction, to) + Fraction::Scale(ufraction32_t(UFRACTION32_1X - fraction), from);
		}

		template<typename T>
		static constexpr T Interpolate(const fraction8_t fraction, const T from, const T to)
		{
			return Fraction::Scale(fraction, to) + Fraction::Scale(fraction8_t(FRACTION8_1X - fraction), from);
		}

		template<typename T>
		static constexpr T Interpolate(const fraction16_t fraction, const T from, const T to)
		{
			return Fraction::Scale(fraction, to) + Fraction::Scale(fraction16_t(FRACTION16_1X - fraction), from);
		}

		template<typename T>
		static constexpr T Interpolate(const fraction32_t fraction, const T from, const T to)
		{
			return Fraction::Scale(fraction, to) + Fraction::Scale(fraction32_t(FRACTION32_1X - fraction), from);
		}
	}
}

#endif