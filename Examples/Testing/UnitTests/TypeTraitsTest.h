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
			template<typename T>
			struct HasNestedType
			{
			private:
				template<typename U>
				static uint8_t Match(typename U::type*);

				template<typename U>
				static uint16_t Match(...);

			public:
				enum { value = sizeof(Match<T>(nullptr)) == sizeof(uint8_t) };
			};

			// Reference functions for type_limits
			static inline bool RefTypeLimits()
			{
				static_assert(TypeTraits::TypeLimits::type_limits<uint8_t>::Max() == UINT8_MAX, "type_limits<uint8_t>::Max()");
				static_assert(TypeTraits::TypeLimits::type_limits<uint8_t>::Min() == 0, "type_limits<uint8_t>::Min()");
				static_assert(TypeTraits::TypeLimits::type_limits<int8_t>::Max() == INT8_MAX, "type_limits<int8_t>::Max()");
				static_assert(TypeTraits::TypeLimits::type_limits<int8_t>::Min() == INT8_MIN, "type_limits<int8_t>::Min()");
				static_assert(TypeTraits::TypeLimits::type_limits<uint16_t>::Max() == UINT16_MAX, "type_limits<uint16_t>::Max()");
				static_assert(TypeTraits::TypeLimits::type_limits<uint16_t>::Min() == 0, "type_limits<uint16_t>::Min()");
				static_assert(TypeTraits::TypeLimits::type_limits<int16_t>::Max() == INT16_MAX, "type_limits<int16_t>::Max()");
				static_assert(TypeTraits::TypeLimits::type_limits<int16_t>::Min() == INT16_MIN, "type_limits<int16_t>::Min()");
				static_assert(TypeTraits::TypeLimits::type_limits<uint32_t>::Max() == UINT32_MAX, "type_limits<uint32_t>::Max()");
				static_assert(TypeTraits::TypeLimits::type_limits<uint32_t>::Min() == 0, "type_limits<uint32_t>::Min()");
				static_assert(TypeTraits::TypeLimits::type_limits<int32_t>::Max() == INT32_MAX, "type_limits<int32_t>::Max()");
				static_assert(TypeTraits::TypeLimits::type_limits<int32_t>::Min() == INT32_MIN, "type_limits<int32_t>::Min()");
				static_assert(TypeTraits::TypeLimits::type_limits<uint64_t>::Max() == UINT64_MAX, "type_limits<uint64_t>::Max()");
				static_assert(TypeTraits::TypeLimits::type_limits<uint64_t>::Min() == 0, "type_limits<uint64_t>::Min()");
				static_assert(TypeTraits::TypeLimits::type_limits<int64_t>::Max() == INT64_MAX, "type_limits<int64_t>::Max()");
				static_assert(TypeTraits::TypeLimits::type_limits<int64_t>::Min() == INT64_MIN, "type_limits<int64_t>::Min()");

				bool pass = true;
				pass &= (TypeTraits::TypeLimits::type_limits<uint8_t>::Max() == UINT8_MAX);
				pass &= (TypeTraits::TypeLimits::type_limits<uint8_t>::Min() == 0);
				pass &= (TypeTraits::TypeLimits::type_limits<int8_t>::Max() == INT8_MAX);
				pass &= (TypeTraits::TypeLimits::type_limits<int8_t>::Min() == INT8_MIN);

				pass &= (TypeTraits::TypeLimits::type_limits<uint16_t>::Max() == UINT16_MAX);
				pass &= (TypeTraits::TypeLimits::type_limits<uint16_t>::Min() == 0);
				pass &= (TypeTraits::TypeLimits::type_limits<int16_t>::Max() == INT16_MAX);
				pass &= (TypeTraits::TypeLimits::type_limits<int16_t>::Min() == INT16_MIN);

				pass &= (TypeTraits::TypeLimits::type_limits<uint32_t>::Max() == UINT32_MAX);
				pass &= (TypeTraits::TypeLimits::type_limits<uint32_t>::Min() == 0);
				pass &= (TypeTraits::TypeLimits::type_limits<int32_t>::Max() == INT32_MAX);
				pass &= (TypeTraits::TypeLimits::type_limits<int32_t>::Min() == INT32_MIN);
				pass &= (TypeTraits::TypeLimits::type_limits<uint64_t>::Max() == UINT64_MAX);
				pass &= (TypeTraits::TypeLimits::type_limits<uint64_t>::Min() == 0);
				pass &= (TypeTraits::TypeLimits::type_limits<int64_t>::Max() == INT64_MAX);
				pass &= (TypeTraits::TypeLimits::type_limits<int64_t>::Min() == INT64_MIN);

				return pass;
			}

			// Reference functions for next_uint_type
			static inline bool RefNextUintType()
			{
				static_assert(TypeTraits::TypeDispatch::is_same<typename TypeTraits::TypeNext::next_uint_type<uint8_t>::type, uint16_t>::value, "next_uint_type<uint8_t>");
				static_assert(TypeTraits::TypeDispatch::is_same<typename TypeTraits::TypeNext::next_uint_type<uint16_t>::type, uint32_t>::value, "next_uint_type<uint16_t>");
				static_assert(TypeTraits::TypeDispatch::is_same<typename TypeTraits::TypeNext::next_uint_type<uint32_t>::type, uint64_t>::value, "next_uint_type<uint32_t>");
				static_assert(TypeTraits::TypeDispatch::is_same<typename TypeTraits::TypeNext::next_uint_type<uint64_t>::type, uint64_t>::value, "next_uint_type<uint64_t>");
				static_assert(TypeTraits::TypeDispatch::is_same<typename TypeTraits::TypeNext::next_uint_type<int8_t>::type, uint16_t>::value, "next_uint_type<int8_t>");
				static_assert(TypeTraits::TypeDispatch::is_same<typename TypeTraits::TypeNext::next_uint_type<int16_t>::type, uint32_t>::value, "next_uint_type<int16_t>");
				static_assert(TypeTraits::TypeDispatch::is_same<typename TypeTraits::TypeNext::next_uint_type<int32_t>::type, uint64_t>::value, "next_uint_type<int32_t>");
				static_assert(TypeTraits::TypeDispatch::is_same<typename TypeTraits::TypeNext::next_uint_type<int64_t>::type, uint64_t>::value, "next_uint_type<int64_t>");

				bool pass = true;
				pass &= TypeTraits::TypeDispatch::is_same<typename TypeTraits::TypeNext::next_uint_type<uint8_t>::type, uint16_t>::value;
				pass &= TypeTraits::TypeDispatch::is_same<typename TypeTraits::TypeNext::next_uint_type<uint16_t>::type, uint32_t>::value;
				pass &= TypeTraits::TypeDispatch::is_same<typename TypeTraits::TypeNext::next_uint_type<uint32_t>::type, uint64_t>::value;
				pass &= TypeTraits::TypeDispatch::is_same<typename TypeTraits::TypeNext::next_uint_type<uint64_t>::type, uint64_t>::value;
				pass &= TypeTraits::TypeDispatch::is_same<typename TypeTraits::TypeNext::next_uint_type<int8_t>::type, uint16_t>::value;
				pass &= TypeTraits::TypeDispatch::is_same<typename TypeTraits::TypeNext::next_uint_type<int16_t>::type, uint32_t>::value;
				pass &= TypeTraits::TypeDispatch::is_same<typename TypeTraits::TypeNext::next_uint_type<int32_t>::type, uint64_t>::value;
				pass &= TypeTraits::TypeDispatch::is_same<typename TypeTraits::TypeNext::next_uint_type<int64_t>::type, uint64_t>::value;

				return pass;
			}

			// Reference functions for next_int_type
			static inline bool RefNextIntType()
			{
				static_assert(TypeTraits::TypeDispatch::is_same<typename TypeTraits::TypeNext::next_int_type<uint8_t>::type, int16_t>::value, "next_int_type<uint8_t>");
				static_assert(TypeTraits::TypeDispatch::is_same<typename TypeTraits::TypeNext::next_int_type<uint16_t>::type, int32_t>::value, "next_int_type<uint16_t>");
				static_assert(TypeTraits::TypeDispatch::is_same<typename TypeTraits::TypeNext::next_int_type<uint32_t>::type, int64_t>::value, "next_int_type<uint32_t>");
				static_assert(TypeTraits::TypeDispatch::is_same<typename TypeTraits::TypeNext::next_int_type<uint64_t>::type, int64_t>::value, "next_int_type<uint64_t>");
				static_assert(TypeTraits::TypeDispatch::is_same<typename TypeTraits::TypeNext::next_int_type<int8_t>::type, int16_t>::value, "next_int_type<int8_t>");
				static_assert(TypeTraits::TypeDispatch::is_same<typename TypeTraits::TypeNext::next_int_type<int16_t>::type, int32_t>::value, "next_int_type<int16_t>");
				static_assert(TypeTraits::TypeDispatch::is_same<typename TypeTraits::TypeNext::next_int_type<int32_t>::type, int64_t>::value, "next_int_type<int32_t>");
				static_assert(TypeTraits::TypeDispatch::is_same<typename TypeTraits::TypeNext::next_int_type<int64_t>::type, int64_t>::value, "next_int_type<int64_t>");

				bool pass = true;
				pass &= TypeTraits::TypeDispatch::is_same<typename TypeTraits::TypeNext::next_int_type<uint8_t>::type, int16_t>::value;
				pass &= TypeTraits::TypeDispatch::is_same<typename TypeTraits::TypeNext::next_int_type<int8_t>::type, int16_t>::value;
				pass &= TypeTraits::TypeDispatch::is_same<typename TypeTraits::TypeNext::next_int_type<uint16_t>::type, int32_t>::value;
				pass &= TypeTraits::TypeDispatch::is_same<typename TypeTraits::TypeNext::next_int_type<int16_t>::type, int32_t>::value;
				pass &= TypeTraits::TypeDispatch::is_same<typename TypeTraits::TypeNext::next_int_type<uint32_t>::type, int64_t>::value;
				pass &= TypeTraits::TypeDispatch::is_same<typename TypeTraits::TypeNext::next_int_type<int32_t>::type, int64_t>::value;
				pass &= TypeTraits::TypeDispatch::is_same<typename TypeTraits::TypeNext::next_int_type<uint64_t>::type, int64_t>::value;
				pass &= TypeTraits::TypeDispatch::is_same<typename TypeTraits::TypeNext::next_int_type<int64_t>::type, int64_t>::value;
				return pass;
			}

			static inline bool RefTypeDispatch()
			{
				static_assert(TypeTraits::TypeDispatch::is_same<uint8_t, uint8_t>::value, "is_same true");
				static_assert(!TypeTraits::TypeDispatch::is_same<uint8_t, int8_t>::value, "is_same false");
				static_assert(TypeTraits::TypeDispatch::is_same<typename TypeTraits::TypeDispatch::is_same<uint8_t, uint8_t>::type, TypeTraits::TypeDispatch::TrueType>::value, "is_same true type");
				static_assert(TypeTraits::TypeDispatch::is_same<typename TypeTraits::TypeDispatch::is_same<uint8_t, int8_t>::type, TypeTraits::TypeDispatch::FalseType>::value, "is_same false type");

				bool pass = true;
				pass &= TypeTraits::TypeDispatch::is_same<uint64_t, uint64_t>::value;
				pass &= !TypeTraits::TypeDispatch::is_same<uint64_t, int64_t>::value;
				return pass;
			}

			static inline bool RefTypeConditional()
			{
				static_assert(TypeTraits::TypeDispatch::is_same<typename TypeTraits::TypeConditional::conditional_type<uint8_t, uint16_t, true>::type, uint8_t>::value, "conditional_type true");
				static_assert(TypeTraits::TypeDispatch::is_same<typename TypeTraits::TypeConditional::conditional_type<uint8_t, uint16_t, false>::type, uint16_t>::value, "conditional_type false");
				static_assert(TypeTraits::TypeDispatch::is_same<typename TypeTraits::TypeConditional::larger_type<uint8_t, uint16_t>::type, uint16_t>::value, "larger_type");
				static_assert(TypeTraits::TypeDispatch::is_same<typename TypeTraits::TypeConditional::smaller_type<uint32_t, uint16_t>::type, uint16_t>::value, "smaller_type");
				static_assert(TypeTraits::TypeDispatch::is_same<typename TypeTraits::TypeConditional::larger_type<int16_t, uint16_t>::type, int16_t>::value, "larger_type equal size keeps first");
				static_assert(TypeTraits::TypeDispatch::is_same<typename TypeTraits::TypeConditional::smaller_type<int16_t, uint16_t>::type, uint16_t>::value, "smaller_type equal size keeps second");

				bool pass = true;
				pass &= TypeTraits::TypeConditional::larger_type<uint64_t, uint8_t>::value;
				pass &= !TypeTraits::TypeConditional::smaller_type<uint64_t, uint8_t>::value;
				return pass;
			}

			static inline bool RefTypeEnableIf()
			{
				static_assert(TypeTraits::TypeDispatch::is_same<typename TypeTraits::TypeEnableIf::enable_if<true, uint8_t>::type, uint8_t>::value, "enable_if true");
				static_assert(TypeTraits::TypeDispatch::is_same<typename TypeTraits::TypeEnableIf::disable_if<false, uint16_t>::type, uint16_t>::value, "disable_if false");
				static_assert(!HasNestedType<TypeTraits::TypeEnableIf::enable_if<false, uint8_t> >::value, "enable_if false has no type");
				static_assert(!HasNestedType<TypeTraits::TypeEnableIf::disable_if<true, uint8_t> >::value, "disable_if true has no type");

				bool pass = true;
				pass &= HasNestedType<TypeTraits::TypeEnableIf::enable_if<true, uint32_t> >::value;
				pass &= HasNestedType<TypeTraits::TypeEnableIf::disable_if<false, uint32_t> >::value;
				pass &= !HasNestedType<TypeTraits::TypeEnableIf::enable_if<false, uint32_t> >::value;
				pass &= !HasNestedType<TypeTraits::TypeEnableIf::disable_if<true, uint32_t> >::value;
				return pass;
			}

			static inline bool RefEvaluation()
			{
				static_assert(TypeTraits::TypeDispatch::is_same<typename TypeTraits::Evaluation::evaluation_tag<true>::type, TypeTraits::Evaluation::ConstantEvaluatedType>::value, "evaluation_tag<true>");
				static_assert(TypeTraits::TypeDispatch::is_same<typename TypeTraits::Evaluation::evaluation_tag<false>::type, TypeTraits::Evaluation::RuntimeEvaluatedType>::value, "evaluation_tag<false>");
				static_assert(TypeTraits::TypeDispatch::is_same<decltype(TypeTraits::Evaluation::GetEvaluationTag()), typename TypeTraits::Evaluation::evaluation_tag<TypeTraits::Evaluation::IsConstantEvaluated()>::type>::value, "GetEvaluationTag return type");

				bool pass = true;
				pass &= TypeTraits::TypeDispatch::is_same<typename TypeTraits::Evaluation::evaluation_tag<true>::type, TypeTraits::Evaluation::ConstantEvaluatedType>::value;
				pass &= TypeTraits::TypeDispatch::is_same<typename TypeTraits::Evaluation::evaluation_tag<false>::type, TypeTraits::Evaluation::RuntimeEvaluatedType>::value;
				pass &= TypeTraits::TypeDispatch::is_same<decltype(TypeTraits::Evaluation::GetEvaluationTag()), typename TypeTraits::Evaluation::evaluation_tag<TypeTraits::Evaluation::IsConstantEvaluated()>::type>::value;
				return pass;
			}

			// Reference functions for is_unsigned and is_signed
			static inline bool RefSignTraits()
			{
				static_assert(TypeTraits::TypeSign::is_unsigned<uint8_t>::value == true, "is_unsigned<uint8_t>");
				static_assert(TypeTraits::TypeSign::is_unsigned<uint16_t>::value == true, "is_unsigned<uint16_t>");
				static_assert(TypeTraits::TypeSign::is_unsigned<uint32_t>::value == true, "is_unsigned<uint32_t>");
				static_assert(TypeTraits::TypeSign::is_unsigned<uint64_t>::value == true, "is_unsigned<uint64_t>");
				static_assert(TypeTraits::TypeSign::is_signed<int8_t>::value == true, "is_signed<int8_t>");
				static_assert(TypeTraits::TypeSign::is_signed<int16_t>::value == true, "is_signed<int16_t>");
				static_assert(TypeTraits::TypeSign::is_signed<int32_t>::value == true, "is_signed<int32_t>");
				static_assert(TypeTraits::TypeSign::is_signed<int64_t>::value == true, "is_signed<int64_t>");

				bool pass = true;
				pass &= (TypeTraits::TypeSign::is_unsigned<uint8_t>::value == true);
				pass &= (TypeTraits::TypeSign::is_unsigned<uint16_t>::value == true);
				pass &= (TypeTraits::TypeSign::is_unsigned<uint32_t>::value == true);
				pass &= (TypeTraits::TypeSign::is_unsigned<int8_t>::value == false);
				pass &= (TypeTraits::TypeSign::is_unsigned<int16_t>::value == false);
				pass &= (TypeTraits::TypeSign::is_unsigned<int32_t>::value == false);
				pass &= (TypeTraits::TypeSign::is_unsigned<uint64_t>::value == true);
				pass &= (TypeTraits::TypeSign::is_unsigned<int64_t>::value == false);
				pass &= (TypeTraits::TypeSign::is_signed<int8_t>::value == true);
				pass &= (TypeTraits::TypeSign::is_signed<int16_t>::value == true);
				pass &= (TypeTraits::TypeSign::is_signed<int32_t>::value == true);
				pass &= (TypeTraits::TypeSign::is_signed<uint8_t>::value == false);
				pass &= (TypeTraits::TypeSign::is_signed<uint16_t>::value == false);
				pass &= (TypeTraits::TypeSign::is_signed<uint32_t>::value == false);
				pass &= (TypeTraits::TypeSign::is_signed<int64_t>::value == true);
				pass &= (TypeTraits::TypeSign::is_signed<uint64_t>::value == false);
				return pass;
			}

			static inline bool RefFitsIn()
			{
				static_assert(TypeTraits::TypeLimits::FitsIn<uint8_t>(uint8_t(0)), "FitsIn<uint8_t>(0)");
				static_assert(TypeTraits::TypeLimits::FitsIn<uint8_t>(uint8_t(255)), "FitsIn<uint8_t>(255)");
				static_assert(!TypeTraits::TypeLimits::FitsIn<uint8_t>(uint16_t(256)), "FitsIn<uint8_t>(256) false");
				static_assert(TypeTraits::TypeLimits::FitsIn<uint16_t>(int16_t(255)), "FitsIn<uint16_t>(255)");
				static_assert(!TypeTraits::TypeLimits::FitsIn<uint16_t>(int32_t(-1)), "FitsIn<uint16_t>(-1) false");
				static_assert(TypeTraits::TypeLimits::FitsIn<int8_t>(int8_t(-128)), "FitsIn<int8_t>(-128)");
				static_assert(TypeTraits::TypeLimits::FitsIn<int8_t>(int8_t(127)), "FitsIn<int8_t>(127)");
				static_assert(!TypeTraits::TypeLimits::FitsIn<int8_t>(int16_t(128)), "FitsIn<int8_t>(128) false");
				static_assert(!TypeTraits::TypeLimits::FitsIn<int8_t>(int16_t(-129)), "FitsIn<int8_t>(-129) false");
				static_assert(TypeTraits::TypeLimits::FitsIn<int16_t>(uint8_t(255)), "FitsIn<int16_t>(255)");
				static_assert(!TypeTraits::TypeLimits::FitsIn<int16_t>(uint32_t(65536)), "FitsIn<int16_t>(65536) false");

				bool pass = true;
				pass &= TypeTraits::TypeLimits::FitsIn<uint8_t>(uint8_t(0));
				pass &= TypeTraits::TypeLimits::FitsIn<uint8_t>(uint8_t(255));
				pass &= !TypeTraits::TypeLimits::FitsIn<uint8_t>(uint16_t(256));
				pass &= TypeTraits::TypeLimits::FitsIn<uint16_t>(int16_t(255));
				pass &= !TypeTraits::TypeLimits::FitsIn<uint16_t>(int32_t(-1));
				pass &= TypeTraits::TypeLimits::FitsIn<int8_t>(int8_t(-128));
				pass &= TypeTraits::TypeLimits::FitsIn<int8_t>(int8_t(127));
				pass &= !TypeTraits::TypeLimits::FitsIn<int8_t>(int16_t(128));
				pass &= !TypeTraits::TypeLimits::FitsIn<int8_t>(int16_t(-129));
				pass &= TypeTraits::TypeLimits::FitsIn<int16_t>(uint8_t(255));
				pass &= !TypeTraits::TypeLimits::FitsIn<int16_t>(uint32_t(65536));
				return pass;
			}

			// Reference functions for IsUnsignedType, make_unsigned, and make_signed
			static inline bool RefSignTransforms()
			{
				static_assert(TypeTraits::TypeDispatch::is_same<typename TypeTraits::TypeSign::make_unsigned<uint8_t>::type, uint8_t>::value, "make_unsigned<uint8_t>");
				static_assert(TypeTraits::TypeDispatch::is_same<typename TypeTraits::TypeSign::make_unsigned<uint16_t>::type, uint16_t>::value, "make_unsigned<uint16_t>");
				static_assert(TypeTraits::TypeDispatch::is_same<typename TypeTraits::TypeSign::make_unsigned<uint32_t>::type, uint32_t>::value, "make_unsigned<uint32_t>");
				static_assert(TypeTraits::TypeDispatch::is_same<typename TypeTraits::TypeSign::make_unsigned<uint64_t>::type, uint64_t>::value, "make_unsigned<uint64_t identity>");
				static_assert(TypeTraits::TypeDispatch::is_same<typename TypeTraits::TypeSign::make_unsigned<int8_t>::type, uint8_t>::value, "make_unsigned<int8_t>");
				static_assert(TypeTraits::TypeDispatch::is_same<typename TypeTraits::TypeSign::make_unsigned<int16_t>::type, uint16_t>::value, "make_unsigned<int16_t>");
				static_assert(TypeTraits::TypeDispatch::is_same<typename TypeTraits::TypeSign::make_unsigned<int32_t>::type, uint32_t>::value, "make_unsigned<int32_t>");
				static_assert(TypeTraits::TypeDispatch::is_same<typename TypeTraits::TypeSign::make_unsigned<int64_t>::type, uint64_t>::value, "make_unsigned<int64_t>");
				static_assert(TypeTraits::TypeDispatch::is_same<typename TypeTraits::TypeSign::make_signed<int8_t>::type, int8_t>::value, "make_signed<int8_t>");
				static_assert(TypeTraits::TypeDispatch::is_same<typename TypeTraits::TypeSign::make_signed<int16_t>::type, int16_t>::value, "make_signed<int16_t>");
				static_assert(TypeTraits::TypeDispatch::is_same<typename TypeTraits::TypeSign::make_signed<int32_t>::type, int32_t>::value, "make_signed<int32_t>");
				static_assert(TypeTraits::TypeDispatch::is_same<typename TypeTraits::TypeSign::make_signed<int64_t>::type, int64_t>::value, "make_signed<int64_t identity>");
				static_assert(TypeTraits::TypeDispatch::is_same<typename TypeTraits::TypeSign::make_signed<uint8_t>::type, int8_t>::value, "make_signed<uint8_t>");
				static_assert(TypeTraits::TypeDispatch::is_same<typename TypeTraits::TypeSign::make_signed<uint16_t>::type, int16_t>::value, "make_signed<uint16_t>");
				static_assert(TypeTraits::TypeDispatch::is_same<typename TypeTraits::TypeSign::make_signed<uint32_t>::type, int32_t>::value, "make_signed<uint32_t>");
				static_assert(TypeTraits::TypeDispatch::is_same<typename TypeTraits::TypeSign::make_signed<uint64_t>::type, int64_t>::value, "make_signed<uint64_t>");
				static_assert(TypeTraits::TypeDispatch::is_same<typename TypeTraits::TypeSign::IsUnsignedType<uint8_t>::type, TypeTraits::TypeDispatch::TrueType>::value, "IsUnsignedType<uint8_t>");
				static_assert(TypeTraits::TypeDispatch::is_same<typename TypeTraits::TypeSign::IsUnsignedType<uint16_t>::type, TypeTraits::TypeDispatch::TrueType>::value, "IsUnsignedType<uint16_t>");
				static_assert(TypeTraits::TypeDispatch::is_same<typename TypeTraits::TypeSign::IsUnsignedType<uint32_t>::type, TypeTraits::TypeDispatch::TrueType>::value, "IsUnsignedType<uint32_t>");
				static_assert(TypeTraits::TypeDispatch::is_same<typename TypeTraits::TypeSign::IsUnsignedType<uint64_t>::type, TypeTraits::TypeDispatch::TrueType>::value, "IsUnsignedType<uint64_t>");
				static_assert(TypeTraits::TypeDispatch::is_same<typename TypeTraits::TypeSign::IsUnsignedType<int8_t>::type, TypeTraits::TypeDispatch::FalseType>::value, "IsUnsignedType<int8_t>");
				static_assert(TypeTraits::TypeDispatch::is_same<typename TypeTraits::TypeSign::IsUnsignedType<int16_t>::type, TypeTraits::TypeDispatch::FalseType>::value, "IsUnsignedType<int16_t>");
				static_assert(TypeTraits::TypeDispatch::is_same<typename TypeTraits::TypeSign::IsUnsignedType<int32_t>::type, TypeTraits::TypeDispatch::FalseType>::value, "IsUnsignedType<int32_t>");
				static_assert(TypeTraits::TypeDispatch::is_same<typename TypeTraits::TypeSign::IsUnsignedType<int64_t>::type, TypeTraits::TypeDispatch::FalseType>::value, "IsUnsignedType<int64_t>");

				bool pass = true;
				pass &= TypeTraits::TypeDispatch::is_same<typename TypeTraits::TypeSign::IsUnsignedType<uint8_t>::type, TypeTraits::TypeDispatch::TrueType>::value;
				pass &= TypeTraits::TypeDispatch::is_same<typename TypeTraits::TypeSign::IsUnsignedType<uint16_t>::type, TypeTraits::TypeDispatch::TrueType>::value;
				pass &= TypeTraits::TypeDispatch::is_same<typename TypeTraits::TypeSign::IsUnsignedType<uint32_t>::type, TypeTraits::TypeDispatch::TrueType>::value;
				pass &= TypeTraits::TypeDispatch::is_same<typename TypeTraits::TypeSign::IsUnsignedType<int8_t>::type, TypeTraits::TypeDispatch::FalseType>::value;
				pass &= TypeTraits::TypeDispatch::is_same<typename TypeTraits::TypeSign::IsUnsignedType<int16_t>::type, TypeTraits::TypeDispatch::FalseType>::value;
				pass &= TypeTraits::TypeDispatch::is_same<typename TypeTraits::TypeSign::IsUnsignedType<int32_t>::type, TypeTraits::TypeDispatch::FalseType>::value;
				pass &= TypeTraits::TypeDispatch::is_same<typename TypeTraits::TypeSign::IsUnsignedType<int64_t>::type, TypeTraits::TypeDispatch::FalseType>::value;
				pass &= TypeTraits::TypeDispatch::is_same<typename TypeTraits::TypeSign::make_unsigned<uint8_t>::type, uint8_t>::value;
				pass &= TypeTraits::TypeDispatch::is_same<typename TypeTraits::TypeSign::make_unsigned<uint16_t>::type, uint16_t>::value;
				pass &= TypeTraits::TypeDispatch::is_same<typename TypeTraits::TypeSign::make_unsigned<uint32_t>::type, uint32_t>::value;
				pass &= TypeTraits::TypeDispatch::is_same<typename TypeTraits::TypeSign::make_unsigned<uint64_t>::type, uint64_t>::value;
				pass &= TypeTraits::TypeDispatch::is_same<typename TypeTraits::TypeSign::make_unsigned<int8_t>::type, uint8_t>::value;
				pass &= TypeTraits::TypeDispatch::is_same<typename TypeTraits::TypeSign::make_unsigned<int16_t>::type, uint16_t>::value;
				pass &= TypeTraits::TypeDispatch::is_same<typename TypeTraits::TypeSign::make_unsigned<int32_t>::type, uint32_t>::value;
				pass &= TypeTraits::TypeDispatch::is_same<typename TypeTraits::TypeSign::make_unsigned<int64_t>::type, uint64_t>::value;
				pass &= TypeTraits::TypeDispatch::is_same<typename TypeTraits::TypeSign::make_signed<uint8_t>::type, int8_t>::value;
				pass &= TypeTraits::TypeDispatch::is_same<typename TypeTraits::TypeSign::make_signed<uint16_t>::type, int16_t>::value;
				pass &= TypeTraits::TypeDispatch::is_same<typename TypeTraits::TypeSign::make_signed<uint32_t>::type, int32_t>::value;
				pass &= TypeTraits::TypeDispatch::is_same<typename TypeTraits::TypeSign::make_signed<uint64_t>::type, int64_t>::value;
				pass &= TypeTraits::TypeDispatch::is_same<typename TypeTraits::TypeSign::make_signed<int8_t>::type, int8_t>::value;
				pass &= TypeTraits::TypeDispatch::is_same<typename TypeTraits::TypeSign::make_signed<int16_t>::type, int16_t>::value;
				pass &= TypeTraits::TypeDispatch::is_same<typename TypeTraits::TypeSign::make_signed<int32_t>::type, int32_t>::value;
				pass &= TypeTraits::TypeDispatch::is_same<typename TypeTraits::TypeSign::make_signed<int64_t>::type, int64_t>::value;
				return pass;
			}

			static inline bool RunTests()
			{
				Serial.println(F("Starting TypeTraits tests..."));
				bool pass = true;

				pass &= RefTypeLimits();
				pass &= RefNextUintType();
				pass &= RefNextIntType();
				pass &= RefTypeDispatch();
				pass &= RefTypeConditional();
				pass &= RefTypeEnableIf();
				pass &= RefEvaluation();
				pass &= RefSignTraits();
				pass &= RefFitsIn();
				pass &= RefSignTransforms();

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