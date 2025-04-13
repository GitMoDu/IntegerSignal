// Signed.h

#ifndef _INTEGER_SIGNAL_SINED_CURVE_h
#define _INTEGER_SIGNAL_SINED_CURVE_h

#include "ICurve.h"

namespace IntegerSignal
{
	namespace Curves
	{
		namespace Template
		{
			/// <summary>
			/// Wraps an unsigned Curve with a signed input/output.
			/// </summary>
			/// <typeparam name="CurveType">Unsigned value ICurve type.</typeparam>
			/// <typeparam name="signed_t">The matching signed type, of the same bitwidth.</typeparam>
			template<typename CurveType,
				typename unsigned_t,
				typename signed_t>
			struct Signed
			{
				static const signed_t Get(const signed_t value)
				{
					switch (value)
					{
					case 0:
						return (signed_t)(CurveType::Get(0) / 2);
						break;
					case Max():
						return (signed_t)(CurveType::Get(UMax()) / 2);
						break;
					case Min():
						return (-(signed_t)(CurveType::Get(UMax()) / 2)) - 1;
						break;
					default:
						if (value > 0)
						{
							return (signed_t)(CurveType::Get((unsigned_t)value << 1) / 2);
						}
						else
						{
							return -(signed_t)(CurveType::Get(((unsigned_t)-value << 1)) / 2);
						}
						break;
					}
				}

			private:
				static constexpr unsigned_t UMax()
				{
					return (((UINT8_MAX * (sizeof(unsigned_t) == sizeof(uint8_t)))
						| (UINT16_MAX * (sizeof(unsigned_t) == sizeof(uint16_t)))
						| (UINT32_MAX * (sizeof(unsigned_t) == sizeof(uint32_t)))
						| (UINT64_MAX * (sizeof(unsigned_t) == sizeof(uint64_t)))));
				}

				static constexpr signed_t Max()
				{
					return (((INT8_MAX * (sizeof(signed_t) == sizeof(int8_t)))
						| (INT16_MAX * (sizeof(signed_t) == sizeof(int16_t)))
						| (INT32_MAX * (sizeof(signed_t) == sizeof(int32_t)))
						| (INT64_MAX * (sizeof(signed_t) == sizeof(int64_t)))));
				}

				static constexpr signed_t Min()
				{
					return (((INT8_MIN * (sizeof(signed_t) == sizeof(int8_t)))
						| (INT16_MIN * (sizeof(signed_t) == sizeof(int16_t)))
						| (INT32_MIN * (sizeof(signed_t) == sizeof(int32_t)))
						| (INT64_MIN * (sizeof(signed_t) == sizeof(int64_t)))));
				}
			};

			/// <summary>
			/// Wraps an unsigned Curve with a signed input/output.
			/// </summary>
			/// <typeparam name="CurveType">Unsigned value ICurve type.</typeparam>
			template<typename CurveType>
			using SignedI8 = Template::Signed<CurveType, uint8_t, int8_t>;

			/// <summary>
			/// Wraps an unsigned Curve with a signed input/output.
			/// </summary>
			/// <typeparam name="CurveType">Unsigned value ICurve type.</typeparam>
			template<typename CurveType>
			using SignedI16 = Template::Signed<CurveType, uint16_t, int16_t>;

			/// <summary>
			/// Wraps an unsigned Curve with a signed input/output.
			/// </summary>
			/// <typeparam name="CurveType">Unsigned value ICurve type.</typeparam>
			template<typename CurveType>
			using SignedI32 = Template::Signed<CurveType, uint32_t, int32_t>;
		}
	}
}

#endif