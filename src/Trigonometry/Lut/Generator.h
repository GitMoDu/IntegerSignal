#ifndef _INTEGER_TRIGONOMETRY_LUT_GENERATOR_h
#define _INTEGER_TRIGONOMETRY_LUT_GENERATOR_h

#include "../../Scale/Fraction.h"
#include "../Sine16.h"
#include "../Sine8.h"

#include "../Tangent16.h"
#include "../Tangent8.h"

namespace IntegerSignal::Trigonometry::Lut::Generator
{
	using namespace Fraction;

	static constexpr double DegreesToRadians(const double degrees) {
		return degrees * (double)(M_PI) / 180.0;
	}

	static uint16_t GetTangentUnit16(const double angle)
	{
		return tan((double)DegreesToRadians(angle)) * (double)(UINT16_MAX - 0);
	}

	static uint16_t GetSineUnit16(const double angle)
	{
		return sin((double)DegreesToRadians(angle)) * (double)(UINT16_MAX - 0);
	}

	static uint16_t GetSineUnit8(const double angle)
	{
		return sin((double)DegreesToRadians(angle)) * (double)UINT8_MAX;
	}


	static void PrintQuarterTableSine8()
	{
		Serial.println(F("8 bit Sin Table"));
		Serial.println();

		Serial.println('{');

		uint8_t item = 0;
		for (uint16_t i = 0; i <= UINT8_MAX; i++)
		{
			double angle = (90.0 * (double)i) / (double)UINT8_MAX;
			const uint8_t value = GetSineUnit8(angle);

			if (i < UINT8_MAX)
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

	static void PrintQuarterTableSine16(const uint16_t tableSize)
	{
		Serial.println(F("16 bit Sin Table"));
		Serial.println();

		Serial.println('{');

		uint8_t item = 0;
		for (uint16_t i = 0; i < tableSize; i++)
		{
			double angle = (90.0 * (double)i) / (double)tableSize;
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

	static void PrintQuarterTableTangent16(const uint16_t tableSize)
	{
		Serial.println(F("16 bit Tan Table"));
		Serial.println();

		Serial.println('{');

		uint8_t item = 0;
		for (uint16_t i = 0; i < tableSize; i++)
		{
			double angle = (45.0 * (double)i) / (double)tableSize;
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

	static void PrintQuarterTableSine8(const uint8_t tableSize)
	{
		Serial.println(F("8 bit Sin Table"));
		Serial.println();

		Serial.println('{');

		uint8_t item = 0;
		for (uint8_t i = 0; i < tableSize; i++)
		{
			double angle = (90.0 * (double)i) / (double)tableSize;
			const uint16_t value = GetSineUnit8(angle);
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
