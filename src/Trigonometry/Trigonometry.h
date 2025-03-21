#ifndef _INTEGER_TRIGONOMETRY_h
#define _INTEGER_TRIGONOMETRY_h

#include <stdint.h>


// Define INTEGER_TRIGONOMETRY_LUT as INTEGER_TRIGONOMETRY_LUT_TINY to use the 64 items LUT.
#define INTEGER_TRIGONOMETRY_LUT_TINY 1

// Define INTEGER_TRIGONOMETRY_LUT as INTEGER_TRIGONOMETRY_LUT_DEFAULT to use the 256 items LUT.
#define INTEGER_TRIGONOMETRY_LUT_DEFAULT 2


// No LUT size defined, use best guess according to platform.
#if !defined(INTEGER_TRIGONOMETRY_LUT)
#if defined(__SIZEOF_INT__) && (__SIZEOF_INT__ > 2)
#define INTEGER_TRIGONOMETRY_LUT INTEGER_TRIGONOMETRY_LUT_DEFAULT
#else
#define INTEGER_TRIGONOMETRY_LUT INTEGER_TRIGONOMETRY_LUT_TINY
#endif
#endif

namespace IntegerSignal::Trigonometry
{
	/// <summary>
	/// Represents an abstract angle in the range [0; UINT16_MAX]
	/// Corresponding to [0; 360] degrees of rotation.
	/// </summary>
	typedef uint16_t angle_t;

	static constexpr angle_t ANGLE_RANGE = UINT16_MAX;

	/// <summary>
	/// Converts degrees to the abstract angle representation.
	/// </summary>
	/// <param name="degrees">>[-360;360] degrees of rotation.</param>
	/// <returns>Abstract angle in the range [0; UINT16_MAX]</returns>
	static constexpr angle_t GetAngle(const int16_t degrees)
	{
		return ((int32_t)degrees * ((int32_t)ANGLE_RANGE + 1)) / 360;
	}

	static constexpr angle_t ANGLE_90 = GetAngle(90);
	static constexpr angle_t ANGLE_45 = GetAngle(45);
	static constexpr angle_t ANGLE_180 = GetAngle(180);
	static constexpr angle_t ANGLE_270 = GetAngle(270);
}

#endif