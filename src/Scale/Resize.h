#ifndef _INTEGER_SIGNAL_RESIZE_h
#define _INTEGER_SIGNAL_RESIZE_h

#include <stdint.h>

namespace IntegerSignal
{
	namespace Resize
	{
		/// <summary>
		/// [RESIZE8_1_16X; RESIZE8_15X].
		/// Corresponds to [1/16 ; 1x15] times resize.
		/// </summary>
		typedef uint8_t resize8_t;

		/// <summary>
		/// [RESIZE16_1_256X; RESIZE16_255X].
		/// Corresponds to [1/256 ; 1x255] times resize.
		/// </summary>
		typedef uint16_t resize16_t;

		/// <summary>
		/// [RESIZE32_1_65536X; RESIZE32_65535X].
		/// Corresponds to [1/65536 ; 1x65535] times resize.
		/// </summary>
		typedef uint32_t resize32_t;


		static constexpr resize8_t RESIZE8_1X = (resize8_t)(UINT8_MAX >> 4) + 1;
		static constexpr resize8_t RESIZE8_1_16X = 1;
		static constexpr resize8_t RESIZE8_15X = UINT8_MAX;
		static constexpr resize8_t RESIZE8_FACTOR_MIN = RESIZE8_1X;
		static constexpr uint8_t RESIZE8_FACTOR_MAX = UINT8_MAX / RESIZE8_1X;

		static constexpr resize16_t RESIZE16_1X = (resize16_t)UINT8_MAX + 1;
		static constexpr resize16_t RESIZE16_1_256X = 1;
		static constexpr resize16_t RESIZE16_255X = UINT16_MAX;
		static constexpr resize16_t RESIZE16_FACTOR_MIN = RESIZE16_1X;
		static constexpr uint8_t RESIZE16_FACTOR_MAX = UINT16_MAX / RESIZE16_1X;

		static constexpr resize32_t RESIZE32_1X = (resize32_t)UINT16_MAX + 1;
		static constexpr resize32_t RESIZE32_1_65536X = 1;
		static constexpr resize32_t RESIZE32_65535X = UINT16_MAX;
		static constexpr resize32_t RESIZE32_FACTOR_MIN = RESIZE32_1X;
		static constexpr uint16_t RESIZE32_FACTOR_MAX = UINT32_MAX / RESIZE32_1X;

		/// <summary>
		/// Calculator for resize based on numerator/denominator ratio.
		/// </summary>
		/// <param name="numerator">Ratio multiplier.</param>
		/// <param name="denominator">Ratio divider.</param>
		/// <returns>Resize scale to be used with ResizeN().</returns>
		namespace ResizeCalculator
		{
			static constexpr resize8_t GetResize8(const uint8_t numerator, const uint8_t denominator = 1)
			{
				return ((uint16_t)numerator * RESIZE8_1X) / denominator;
			}

			static constexpr resize8_t GetResize8(const uint16_t numerator, const uint16_t denominator = 1)
			{
				return ((uint32_t)numerator * RESIZE8_1X) / denominator;
			}

			static constexpr resize8_t GetResize8(const uint32_t numerator, const uint32_t denominator = 1)
			{
				return resize8_t(((uint64_t)numerator * RESIZE8_1X) / denominator);
			}

			static constexpr resize16_t GetResize16(const uint16_t numerator, const uint16_t denominator = 1)
			{
				return ((uint32_t)numerator * RESIZE16_1X) / denominator;
			}

			static constexpr resize16_t GetResize16(const uint8_t numerator, const uint8_t denominator = 1)
			{
				return GetResize16((uint16_t)numerator, (uint16_t)denominator);
			}

			static constexpr resize16_t GetResize16(const uint32_t numerator, const uint32_t denominator = 1)
			{
				return resize16_t(((uint64_t)numerator * RESIZE16_1X) / denominator);
			}

			static constexpr resize32_t GetResize32(const uint32_t numerator, const uint32_t denominator = 1)
			{
				return ((uint64_t)numerator * RESIZE32_1X) / denominator;
			}

			static constexpr resize32_t GetResize32(const uint16_t numerator, const uint16_t denominator = 1)
			{
				return GetResize32((uint32_t)numerator, (uint32_t)denominator);
			}

			static constexpr resize32_t GetResize32(const uint8_t numerator, const uint8_t denominator = 1)
			{
				return GetResize32((uint32_t)numerator, (uint32_t)denominator);
			}
		}

		using namespace ResizeCalculator;

		static constexpr uint8_t Scale(const resize8_t resize, const uint8_t value)
		{
			return uint8_t(MinValue((uint16_t)UINT8_MAX, (uint16_t)(((uint16_t)value * resize) / RESIZE8_1X)));
		}

		static constexpr int8_t Scale(const resize8_t resize, const int8_t value)
		{
			return int8_t(MaxValue((int16_t)INT8_MIN, MinValue((int16_t)INT8_MAX, (int16_t)(((int16_t)value * resize) / RESIZE8_1X))));
		}

		static constexpr uint16_t Scale(const resize8_t resize, const uint16_t value)
		{
			return uint16_t(MinValue((uint32_t)UINT16_MAX, ((uint32_t)value * resize) / RESIZE8_1X));
		}

		static constexpr int16_t Scale(const resize8_t resize, const int16_t value)
		{
			return int16_t(MaxValue((int32_t)INT16_MIN, MinValue((int32_t)INT16_MAX, (int32_t)(((int32_t)value * resize) / RESIZE8_1X))));
		}

		static constexpr uint32_t Scale(const resize8_t resize, const uint32_t value)
		{
			return uint32_t(MinValue((uint64_t)UINT32_MAX, (uint64_t)(((uint64_t)value * resize) / RESIZE8_1X)));
		}

		static constexpr int32_t Scale(const resize8_t resize, const int32_t value)
		{
			return int32_t(MaxValue((int64_t)INT32_MIN, MinValue((int64_t)INT32_MAX, ((int64_t)value * resize) / RESIZE8_1X)));
		}

		static constexpr uint8_t Scale(const resize16_t resize, const uint8_t value)
		{
			return uint8_t(MinValue((uint16_t)UINT8_MAX, (uint16_t)(((uint16_t)value * resize) / RESIZE16_1X)));
		}

		static constexpr int8_t Scale(const resize16_t resize, const int8_t value)
		{
			return int8_t(MaxValue((int16_t)INT8_MIN, MinValue((int16_t)INT8_MAX, (int16_t)(((int32_t)value * resize) / RESIZE16_1X))));
		}

		static constexpr uint16_t Scale(const resize16_t resize, const uint16_t value)
		{
			return uint16_t(MinValue((uint32_t)UINT16_MAX, (uint32_t)(((uint32_t)value * resize) / RESIZE16_1X)));
		}

		static constexpr int16_t Scale(const resize16_t resize, const int16_t value)
		{
			return int16_t(MaxValue((int32_t)INT16_MIN, MinValue((int32_t)INT16_MAX, (int32_t)(((int32_t)value * resize) / RESIZE16_1X))));
		}

		static constexpr uint32_t Scale(const resize16_t resize, const uint32_t value)
		{
			return uint32_t(MinValue((uint64_t)UINT32_MAX, (uint64_t)(((uint64_t)value * resize) / RESIZE16_1X)));
		}

		static constexpr int32_t Scale(const resize16_t resize, const int32_t value)
		{
			return int32_t(MaxValue((int64_t)INT32_MIN, MinValue((int64_t)INT32_MAX, (int64_t)(((int64_t)value * resize) / RESIZE16_1X))));
		}

		static constexpr uint8_t Scale(const resize32_t resize, const uint8_t value)
		{
			return uint8_t(MinValue((uint16_t)UINT8_MAX, (uint16_t)(((uint32_t)value * resize) / RESIZE32_1X)));
		}

		static constexpr int8_t Scale(const resize32_t resize, const int8_t value)
		{
			return int8_t(MaxValue((int16_t)INT8_MIN, MinValue((int16_t)INT8_MAX, (int16_t)(((int32_t)value * resize) / RESIZE32_1X))));
		}

		static constexpr uint16_t Scale(const resize32_t resize, const uint16_t value)
		{
			return uint16_t(MinValue((uint32_t)UINT16_MAX, (uint32_t)(((uint32_t)value * resize) / RESIZE32_1X)));
		}

		static constexpr int16_t Scale(const resize32_t resize, const int16_t value)
		{
			return int16_t(MaxValue((int32_t)INT16_MIN, MinValue((int32_t)INT16_MAX, (int32_t)(((int32_t)value * resize) / RESIZE32_1X))));
		}

		static constexpr uint32_t Scale(const resize32_t resize, const uint32_t value)
		{
			return uint32_t(MinValue((uint64_t)UINT32_MAX, (uint64_t)(((uint64_t)value * resize) / RESIZE32_1X)));
		}

		static constexpr int32_t Scale(const resize32_t resize, const int32_t value)
		{
			return int32_t(MaxValue((int64_t)INT32_MIN, MinValue((int64_t)INT32_MAX, (int64_t)(((int64_t)value * resize) / RESIZE32_1X))));
		}
	}
}

#endif