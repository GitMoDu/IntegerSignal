#ifndef _TYPE_TRAITS_TEST_h
#define _TYPE_TRAITS_TEST_h

#include <IntegerSignalTesting.h>
#include <stdint.h>

#if __has_include(<type_traits>)
#include <type_traits>
using std::is_same;
#else
template<typename A, typename B>
struct is_same { static const bool value = false; };

template<typename A>
struct is_same<A, A> { static const bool value = true; };
#endif

namespace IntegerSignal
{
	namespace TypeTraits
	{
		namespace Test
		{
			// Reference functions for type_limits
			static bool RefTypeLimits()
			{
				bool pass = true;
				pass &= (TypeTraits::TypeLimits::type_limits<uint8_t>::Max() == UINT8_MAX);
				pass &= (TypeTraits::TypeLimits::type_limits<uint8_t>::Min() == 0);
				pass &= (TypeTraits::TypeLimits::type_limits<int8_t>::Max() == INT8_MAX);
				pass &= (TypeTraits::TypeLimits::type_limits<int8_t>::Min() == INT8_MIN);

				pass &= (TypeTraits::TypeLimits::type_limits<uint16_t>::Max() == UINT16_MAX);
				pass &= (TypeTraits::TypeLimits::type_limits<int16_t>::Max() == INT16_MAX);
				pass &= (TypeTraits::TypeLimits::type_limits<int16_t>::Min() == INT16_MIN);

				pass &= (TypeTraits::TypeLimits::type_limits<uint32_t>::Max() == UINT32_MAX);
				pass &= (TypeTraits::TypeLimits::type_limits<int32_t>::Max() == INT32_MAX);
				pass &= (TypeTraits::TypeLimits::type_limits<int32_t>::Min() == INT32_MIN);

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
				pass &= is_same<TypeTraits::TypeSign::IsUnsignedType<uint8_t>::type, TypeTraits::TypeDispatch::TrueType>::value;
				pass &= is_same<TypeTraits::TypeSign::IsUnsignedType<int8_t>::type, TypeTraits::TypeDispatch::FalseType>::value;
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
	}
}
#endif