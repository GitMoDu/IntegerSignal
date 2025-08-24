#ifndef _TYPE_TRAITS_TEST_h
#define _TYPE_TRAITS_TEST_h

#include <IntegerSignalTesting.h>

namespace IntegerSignal::TypeTraits::Test
{
	// Reference functions for type_limits
	static bool RefTypeLimits()
	{
		bool pass = true;
		pass &= (TypeTraits::TypeLimits::type_limits<uint8_t>::MAX() == UINT8_MAX);
		pass &= (TypeTraits::TypeLimits::type_limits<uint8_t>::MIN() == 0);
		pass &= (TypeTraits::TypeLimits::type_limits<int8_t>::MAX() == INT8_MAX);
		pass &= (TypeTraits::TypeLimits::type_limits<int8_t>::MIN() == INT8_MIN);
		pass &= (TypeTraits::TypeLimits::type_limits<uint16_t>::MAX() == UINT16_MAX);
		pass &= (TypeTraits::TypeLimits::type_limits<int16_t>::MIN() == INT16_MIN);
		return pass;
	}

	// Reference functions for next_uint_type
	static bool RefNextUintType()
	{
		bool pass = true;
		pass &= (sizeof(typename TypeTraits::TypeNext::next_uint_type<uint8_t>::type) == 2);
		pass &= (sizeof(typename TypeTraits::TypeNext::next_uint_type<uint16_t>::type) == 4);
		pass &= (sizeof(typename TypeTraits::TypeNext::next_uint_type<uint32_t>::type) == 8);
		return pass;
	}

	// Reference functions for next_int_type
	static bool RefNextIntType()
	{
		bool pass = true;
		pass &= (sizeof(typename TypeTraits::TypeNext::next_int_type<uint8_t>::type) == 2);
		pass &= (sizeof(typename TypeTraits::TypeNext::next_int_type<int8_t>::type) == 2);
		pass &= (sizeof(typename TypeTraits::TypeNext::next_int_type<int16_t>::type) == 4);
		pass &= (sizeof(typename TypeTraits::TypeNext::next_int_type<int32_t>::type) == 8);
		return pass;
	}

	// Reference functions for is_unsigned and is_signed
	static bool RefSignTraits()
	{
		bool pass = true;
		pass &= (TypeTraits::TypeSign::is_unsigned<uint8_t>::value == true);
		pass &= (TypeTraits::TypeSign::is_unsigned<int8_t>::value == false);
		pass &= (TypeTraits::TypeSign::is_signed<int8_t>::value == true);
		pass &= (TypeTraits::TypeSign::is_signed<uint8_t>::value == false);
		return pass;
	}

	// Reference functions for IsUnsignedType tag dispatch
	static bool RefIsUnsignedType()
	{
		bool pass = true;
		pass &= std::is_same<TypeTraits::TypeSign::IsUnsignedType<uint8_t>::type, TypeTraits::TypeDispatch::TrueType>::value;
		pass &= std::is_same<TypeTraits::TypeSign::IsUnsignedType<int8_t>::type, TypeTraits::TypeDispatch::FalseType>::value;
		return pass;
	}

	static bool RunTests()
	{
		Serial.println(F("Starting TypeTraits tests..."));
		bool pass = true;

		pass &= RefTypeLimits();
		pass &= RefNextUintType();
		pass &= RefNextIntType();
		pass &= RefSignTraits();
		pass &= RefIsUnsignedType();

		if (pass)
		{
			Serial.println(F("TypeTraits tests PASSED."));
		}
		else
		{
			Serial.println(F("TypeTraits tests FAILED."));
		}

		return pass;
	}
}

#endif