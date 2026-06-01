#ifndef _INTEGER_SIGNAL_LINEAR_INTERPOLATE_CURVE_h
#define _INTEGER_SIGNAL_LINEAR_INTERPOLATE_CURVE_h

#include "ICurve.h"
#include "../Base/ByteSize.h"

namespace IntegerSignal
{
	namespace Curves
	{
		namespace LinearInterpolate
		{
			template<typename value_t>
			struct TemplatePoint
			{
				value_t Input;
				value_t Output;
			};

			using upoint32_t = TemplatePoint<uint32_t>;
			using upoint16_t = TemplatePoint<uint16_t>;
			using upoint8_t = TemplatePoint<uint8_t>;

			using ipoint32_t = TemplatePoint<int32_t>;
			using ipoint16_t = TemplatePoint<int16_t>;
			using ipoint8_t = TemplatePoint<int8_t>;

			namespace Template
			{
				template<typename value_t, typename unsigned_intermediate_t, typename signed_intermediate_t>
				struct ValueMath
				{
					using magnitude_t = typename IntegerSignal::TypeTraits::TypeSign::make_unsigned<value_t>::type;

					static magnitude_t DifferenceMagnitude(const value_t a, const value_t b)
					{
						return DifferenceMagnitude(a, b, typename IntegerSignal::TypeTraits::TypeSign::IsUnsignedType<value_t>::type());
					}

					static magnitude_t DifferenceMagnitude(const value_t a, const value_t b, IntegerSignal::TypeTraits::TypeDispatch::TrueType)
					{
						const unsigned_intermediate_t aWide = (unsigned_intermediate_t)a;
						const unsigned_intermediate_t bWide = (unsigned_intermediate_t)b;

						return aWide >= bWide
							? (magnitude_t)(aWide - bWide)
							: (magnitude_t)(bWide - aWide);
					}

					static magnitude_t DifferenceMagnitude(const value_t a, const value_t b, IntegerSignal::TypeTraits::TypeDispatch::FalseType)
					{
						const signed_intermediate_t aWide = (signed_intermediate_t)a;
						const signed_intermediate_t bWide = (signed_intermediate_t)b;

						return aWide >= bWide
							? (magnitude_t)(aWide - bWide)
							: (magnitude_t)(bWide - aWide);
					}

					static value_t ApplyDelta(const value_t value, const magnitude_t delta, const bool increase)
					{
						return ApplyDelta(value, delta, increase, typename IntegerSignal::TypeTraits::TypeSign::IsUnsignedType<value_t>::type());
					}

					static value_t ApplyDelta(const value_t value, const magnitude_t delta, const bool increase, IntegerSignal::TypeTraits::TypeDispatch::TrueType)
					{
						const unsigned_intermediate_t valueWide = (unsigned_intermediate_t)value;
						const unsigned_intermediate_t deltaWide = (unsigned_intermediate_t)delta;

						return increase
							? (value_t)(valueWide + deltaWide)
							: (value_t)(valueWide - deltaWide);
					}

					static value_t ApplyDelta(const value_t value, const magnitude_t delta, const bool increase, IntegerSignal::TypeTraits::TypeDispatch::FalseType)
					{
						const signed_intermediate_t valueWide = (signed_intermediate_t)value;
						const signed_intermediate_t deltaWide = (signed_intermediate_t)delta;

						return increase
							? (value_t)(valueWide + deltaWide)
							: (value_t)(valueWide - deltaWide);
					}

					static value_t Interpolate(
						const value_t fromInput,
						const value_t fromOutput,
						const value_t toInput,
						const value_t toOutput,
						const value_t input)
					{
						const magnitude_t inputRange = DifferenceMagnitude(toInput, fromInput);

						if (inputRange == 0)
						{
							return toOutput;
						}

						const magnitude_t inputOffset = DifferenceMagnitude(input, fromInput);
						const magnitude_t outputRange = DifferenceMagnitude(toOutput, fromOutput);
						const magnitude_t outputOffset = (magnitude_t)(((unsigned_intermediate_t)inputOffset * (unsigned_intermediate_t)outputRange) / inputRange);

						return ApplyDelta(fromOutput, outputOffset, toOutput >= fromOutput);
					}
				};

				template<typename value_t, value_t... ValuePairs>
				struct ValidateValuePairOrder;

				template<typename value_t>
				struct ValidateValuePairOrder<value_t>
				{
					enum { value = true };
				};

				template<typename value_t, value_t input0, value_t output0>
				struct ValidateValuePairOrder<value_t, input0, output0>
				{
					enum { value = true };
				};

				template<typename value_t, value_t input0, value_t output0, value_t input1, value_t output1, value_t... remainingValuePairs>
				struct ValidateValuePairOrder<value_t, input0, output0, input1, output1, remainingValuePairs...>
				{
					enum { value = (input0 < input1) && ValidateValuePairOrder<value_t, input1, output1, remainingValuePairs...>::value };
				};

				template<typename value_t, bool hasEvenValuePairValues, value_t... ValuePairs>
				struct ValidateValuePairs
				{
					enum { value = true };
				};

				template<typename value_t, value_t... ValuePairs>
				struct ValidateValuePairs<value_t, true, ValuePairs...>
				{
					enum { value = ValidateValuePairOrder<value_t, ValuePairs...>::value };
				};

				template<typename value_t, typename unsigned_intermediate_t, typename signed_intermediate_t, value_t... ValuePairs>
				class ValuePairCurve : public ICurve<value_t>
				{
				public:
					ValuePairCurve() : ICurve<value_t>()
					{}

					~ValuePairCurve()
					{}

					virtual value_t Get(const value_t input) const override
					{
						return GetByPairCount(input);
					}

				private:
					using math_t = ValueMath<value_t, unsigned_intermediate_t, signed_intermediate_t>;
					using pair_count_t = typename IntegerSignal::ByteSize::uint_type<(sizeof...(ValuePairs) / 2)>::type;
					using pair_value_count_t = typename IntegerSignal::ByteSize::uint_type<sizeof...(ValuePairs)>::type;
					static constexpr pair_count_t PairCount = (pair_count_t)(sizeof...(ValuePairs) / 2);
					static constexpr pair_value_count_t PairValueCount = (pair_value_count_t)sizeof...(ValuePairs);

					static_assert((sizeof...(ValuePairs) % 2) == 0,
						"LinearInterpolate requires input/output pairs.");
					static_assert(ValidateValuePairs<value_t, ((sizeof...(ValuePairs) % 2) == 0), ValuePairs...>::value,
						"LinearInterpolate points must be in strictly increasing input order.");

					template<pair_count_t pairCount = PairCount>
					static typename IntegerSignal::TypeTraits::TypeEnableIf::enable_if<(pairCount == 0), value_t>::type GetByPairCount(const value_t input)
					{
						return input;
					}

					template<pair_count_t pairCount = PairCount>
					static typename IntegerSignal::TypeTraits::TypeEnableIf::enable_if<(pairCount != 0), value_t>::type GetByPairCount(const value_t input)
					{
						const value_t curveValuePairs[] = { ValuePairs... };

						if (input <= curveValuePairs[0])
						{
							return curveValuePairs[1];
						}

						for (pair_value_count_t i = 2; i < PairValueCount; i += 2)
						{
							if (input <= curveValuePairs[i])
							{
								return math_t::Interpolate(curveValuePairs[i - 2], curveValuePairs[i - 1], curveValuePairs[i], curveValuePairs[i + 1], input);
							}
						}

						return curveValuePairs[PairValueCount - 1];
					}
				};

				template<typename value_t, typename unsigned_intermediate_t, typename signed_intermediate_t>
				class RuntimeCurve : public ICurve<value_t>
				{
				public:
					using point_t = TemplatePoint<value_t>;

					RuntimeCurve()
						: CurvePoints(nullptr), CurvePointCount(0)
					{}

					RuntimeCurve(const point_t* curvePoints, const size_t curvePointCount)
						: CurvePoints(curvePoints), CurvePointCount(curvePointCount)
					{}

					template<size_t curvePointCount>
					RuntimeCurve(const point_t(&curvePoints)[curvePointCount])
						: CurvePoints(curvePoints), CurvePointCount(curvePointCount)
					{}

					virtual value_t Get(const value_t input) const override
					{
						if (CurvePoints == nullptr || CurvePointCount == 0)
						{
							return input;
						}

						if (input <= CurvePoints[0].Input)
						{
							return CurvePoints[0].Output;
						}

						for (size_t i = 1; i < CurvePointCount; ++i)
						{
							if (input <= CurvePoints[i].Input)
							{
								return ValueMath<value_t, unsigned_intermediate_t, signed_intermediate_t>::Interpolate(
									CurvePoints[i - 1].Input, CurvePoints[i - 1].Output,
									CurvePoints[i].Input, CurvePoints[i].Output,
									input);
							}
						}

						return CurvePoints[CurvePointCount - 1].Output;
					}

					void SetCurvePoints(const point_t* curvePoints, const size_t curvePointCount)
					{
						CurvePoints = curvePoints;
						CurvePointCount = curvePointCount;
					}

					template<size_t curvePointCount>
					void SetCurvePoints(const point_t(&curvePoints)[curvePointCount])
					{
						CurvePoints = curvePoints;
						CurvePointCount = curvePointCount;
					}

					bool IsValid() const
					{
						if (CurvePointCount == 0)
						{
							return true;
						}

						if (CurvePoints == nullptr)
						{
							return false;
						}

						for (size_t i = 1; i < CurvePointCount; ++i)
						{
							if (!(CurvePoints[i - 1].Input < CurvePoints[i].Input))
							{
								return false;
							}
						}

						return true;
					}

				private:
					const point_t* CurvePoints;
					size_t CurvePointCount;
				};
			}

			template<uint8_t... ValuePairs>
			using LinearInterpolateTemplateU8 = Template::ValuePairCurve<uint8_t, uint16_t, int16_t, ValuePairs...>;

			template<uint16_t... ValuePairs>
			using LinearInterpolateTemplateU16 = Template::ValuePairCurve<uint16_t, uint32_t, int32_t, ValuePairs...>;

			template<uint32_t... ValuePairs>
			using LinearInterpolateTemplateU32 = Template::ValuePairCurve<uint32_t, uint64_t, int64_t, ValuePairs...>;

			template<int8_t... ValuePairs>
			using LinearInterpolateTemplateI8 = Template::ValuePairCurve<int8_t, uint16_t, int16_t, ValuePairs...>;

			template<int16_t... ValuePairs>
			using LinearInterpolateTemplateI16 = Template::ValuePairCurve<int16_t, uint32_t, int32_t, ValuePairs...>;

			template<int32_t... ValuePairs>
			using LinearInterpolateTemplateI32 = Template::ValuePairCurve<int32_t, uint64_t, int64_t, ValuePairs...>;

			using LinearInterpolateRuntimeU8 = Template::RuntimeCurve<uint8_t, uint16_t, int16_t>;
			using LinearInterpolateRuntimeU16 = Template::RuntimeCurve<uint16_t, uint32_t, int32_t>;
			using LinearInterpolateRuntimeU32 = Template::RuntimeCurve<uint32_t, uint64_t, int64_t>;
			using LinearInterpolateRuntimeI8 = Template::RuntimeCurve<int8_t, uint16_t, int16_t>;
			using LinearInterpolateRuntimeI16 = Template::RuntimeCurve<int16_t, uint32_t, int32_t>;
			using LinearInterpolateRuntimeI32 = Template::RuntimeCurve<int32_t, uint64_t, int64_t>;
		}
	}
}

#endif
