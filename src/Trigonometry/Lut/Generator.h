#ifndef _INTEGER_TRIGONOMETRY_LUT_GENERATOR_h
#define _INTEGER_TRIGONOMETRY_LUT_GENERATOR_h

#include "../../FixedPoint/FactorScale.h"
#include "../../FixedPoint/ScalarFraction.h"
#include "../Sine16.h"
#include "../Sine8.h"

#include "../Tangent16.h"
#include "../Tangent8.h"

namespace IntegerSignal::Trigonometry::Lut::Generator
{
	using namespace FixedPoint::ScalarFraction;

	static constexpr double DegreesToRadians(const double degrees) {
		return degrees * (double)(M_PI) / 180.0;
	}

	inline uint16_t GetTangentUnit16(const double angle)
	{
		// tan(45)=1.0 -> UINT16_MAX
		const double v = tan(DegreesToRadians(angle));
		const double scaled = v * (double)UINT16_MAX;

		// Round and clamp.
		if (scaled <= 0.0) return 0;
		if (scaled >= (double)UINT16_MAX) return UINT16_MAX;
		return (uint16_t)(scaled + 0.5);
	}

	inline uint16_t GetSineUnit16(const double angle)
	{
		return sin((double)DegreesToRadians(angle)) * (double)(UINT16_MAX - 0);
	}

	static inline uint8_t GetSineUnit8(const double angle)
	{
		// Runtime does: (lut + 2) >> 2 into Q0.6 where 1.0 == 64.
		// LUT should therefore be in ~[0..256], stored in uint8_t (clamped to 255).
		const double v = sin(DegreesToRadians(angle));
		const double scaled = v * (double)((uint16_t)Fraction8::FRACTION_1X << 2); // 256.0

		// Floor (truncate) on purpose to avoid double-round bias with (+2)>>2 at runtime.
		if (scaled <= 0.0) return 0;
		if (scaled >= 255.0) return 255;
		return (uint8_t)(scaled);
	}

	static inline void PrintQuarterTableSine16(const uint16_t tableSize)
	{
		Serial.println(F("16 bit Sin Table"));
		Serial.println();

		Serial.println('{');

		uint8_t item = 0;
		for (uint16_t i = 0; i < tableSize; i++)
		{
			// Inclusive mapping: i==tableSize-1 => angle==90.0
			const double t = (tableSize <= 1) ? 0.0 : (double)i / (double)(tableSize - 1);
			const double angle = 90.0 * t;

			const uint16_t value = GetSineUnit16(angle);

			if (i < tableSize - 1)
			{
				Serial.print(value);
				Serial.print(',');
			}
			else
			{
				Serial.print(value);
				break;
			}

			item++;
			if (item >= 8)
			{
				Serial.println();
				item = 0;
			}
			else
			{
				Serial.print(' ');
			}
		}

		Serial.println(F("};"));
	}

	static inline void PrintQuarterTableSine8(const uint8_t tableSize)
	{
		Serial.println(F("8 bit Sin Table"));
		Serial.println();

		Serial.println('{');

		uint8_t item = 0;
		for (uint8_t i = 0; i < tableSize; i++)
		{
			// Inclusive mapping: i==tableSize-1 => angle==90.0
			const double t = (tableSize <= 1) ? 0.0 : (double)i / (double)(tableSize - 1);
			const double angle = 90.0 * t;

			const uint8_t value = GetSineUnit8(angle);

			if (i < tableSize - 1)
			{
				Serial.print(value);
				Serial.print(',');
			}
			else
			{
				Serial.print(value);
				break;
			}

			item++;
			if (item >= 8)
			{
				Serial.println();
				item = 0;
			}
			else
			{
				Serial.print(' ');
			}
		}

		Serial.println(F("};"));
	}

	static inline void PrintQuarterTableTangent16(const uint16_t tableSize)
	{
		Serial.println(F("16 bit Tan Table"));
		Serial.println();

		Serial.println('{');

		uint8_t item = 0;

		for (uint16_t i = 0; i < tableSize; i++)
		{
			// Inclusive mapping: i==tableSize-1 => angle==45.0
			const double t = (tableSize <= 1) ? 0.0 : (double)i / (double)(tableSize - 1);
			const double angle = 45.0 * t;

			const uint16_t value = GetTangentUnit16(angle);

			if (i < tableSize - 1)
			{
				Serial.print(value);
				Serial.print(',');
			}
			else
			{
				Serial.print(value);
				break;
			}

			item++;
			if (item >= 8)
			{
				Serial.println();
				item = 0;
			}
			else
			{
				Serial.print(' ');
			}
		}

		Serial.println(F("};"));
	}
}
#endif
