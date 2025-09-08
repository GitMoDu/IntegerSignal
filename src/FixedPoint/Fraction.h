#ifndef _INTEGER_SIGNAL_FIXED_POINT_FRACTION_h
#define _INTEGER_SIGNAL_FIXED_POINT_FRACTION_h

#include "QFormat/QFormatUnsigned.h"	
#include "QFormat/QFormatSigned.h"

namespace IntegerSignal
{
	namespace FixedPoint
	{
		/// <summary>
		/// Fixed-point fraction types in Q-format for both unsigned [0;+1] and signed [-1;+1] fractions.
		/// </summary>
		namespace Fraction
		{
			/// <summary>
			/// UQ0.7 format (8-bit unsigned with 7 fractional bits).
			/// Represents fraction range [0.0 ; +1.0].
			/// </summary>
			struct UFraction8 final : QFormat::TemplateUnsignedQFormat<uint8_t>
			{
				using Base = QFormat::TemplateUnsignedQFormat<uint8_t>;

				static constexpr auto FRACTION_1X = Base::SCALAR_UNIT;
			};

			/// <summary>
			/// UQ0.15 format (16-bit unsigned with 15 fractional bits).
			/// Represents fraction range [0.0 ; +1.0].
			/// </summary>
			struct UFraction16 final : QFormat::TemplateUnsignedQFormat<uint16_t>
			{
				using Base = QFormat::TemplateUnsignedQFormat<uint16_t>;

				static constexpr auto FRACTION_1X = Base::SCALAR_UNIT;
			};

			/// <summary>
			/// UQ0.31 format (32-bit unsigned with 31 fractional bits).
			/// Represents fraction range [0.0 ; +1.0].
			/// </summary>
			struct UFraction32 final : QFormat::TemplateUnsignedQFormat<uint32_t>
			{
				using Base = QFormat::TemplateUnsignedQFormat<uint32_t>;

				static constexpr auto FRACTION_1X = Base::SCALAR_UNIT;
			};

			/// <summary>
			/// Q0.6 format (8-bit signed with 1 sign bit and 6 fractional bits).
			/// Represents fraction range [-1.0 ; +1.0].
			/// </summary>
			struct Fraction8 final : QFormat::TemplateSignedQFormat<int8_t>
			{
				using Base = QFormat::TemplateSignedQFormat<int8_t>;

				static constexpr auto FRACTION_1X = Base::SCALAR_UNIT;
				static constexpr auto FRACTION_1X_NEGATIVE = Base::SCALAR_UNIT_NEGATIVE;
			};

			/// <summary>
			/// Q0.14 format (16-bit signed with 1 sign bit and 14 fractional bits).
			/// Represents fraction range [-1.0 ; +1.0].
			/// </summary>
			struct Fraction16 final : QFormat::TemplateSignedQFormat<int16_t>
			{
				using Base = QFormat::TemplateSignedQFormat<int16_t>;

				static constexpr auto FRACTION_1X = Base::SCALAR_UNIT;
				static constexpr auto FRACTION_1X_NEGATIVE = Base::SCALAR_UNIT_NEGATIVE;
			};

			/// <summary>
			/// Q0.30 format (32-bit signed with 1 sign bit and 30 fractional bits).
			/// Represents fraction range [-1.0 ; +1.0].
			/// </summary>
			struct Fraction32 final : QFormat::TemplateSignedQFormat<int32_t>
			{
				using Base = QFormat::TemplateSignedQFormat<int32_t>;

				static constexpr auto FRACTION_1X = Base::SCALAR_UNIT;
				static constexpr auto FRACTION_1X_NEGATIVE = Base::SCALAR_UNIT_NEGATIVE;
			};
		}

		// Backwards compatibility static definitions.
		namespace Fraction
		{
			static constexpr auto UFRACTION8_1X = UFraction8::FRACTION_1X;
			static constexpr auto UFRACTION16_1X = UFraction16::FRACTION_1X;
			static constexpr auto UFRACTION32_1X = UFraction32::FRACTION_1X;


			static constexpr auto FRACTION8_1X = Fraction8::FRACTION_1X;
			static constexpr auto FRACTION8_NEGATIVE_1X = Fraction8::FRACTION_1X_NEGATIVE;

			static constexpr auto FRACTION16_1X = Fraction16::FRACTION_1X;
			static constexpr auto FRACTION16_NEGATIVE_1X = Fraction16::FRACTION_1X_NEGATIVE;

			static constexpr auto FRACTION32_1X = Fraction32::FRACTION_1X;
			static constexpr auto FRACTION32_NEGATIVE_1X = Fraction32::FRACTION_1X_NEGATIVE;
		}
	}
}

#endif