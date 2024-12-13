// ICurve.h

#ifndef _I_CURVE_h
#define _I_CURVE_h

#include <stdint.h>

namespace IntegerSignal::Curve
{
	/// <summary>
	/// Base template interface for curves.
	/// Curves of the same value_t can be template-chained to execute each step in line.
	/// </summary>
	/// <typeparam name="value_t"></typeparam>
	template<typename value_t>
	struct ICurve
	{
		virtual const value_t Get(const value_t input) const { return input; }
	};
}

#endif