// ICurve.h

#ifndef _I_CURVE_h
#define _I_CURVE_h

#include <stdint.h>

namespace IntegerSignal::Curves
{
	/// <summary>
	/// Template interface for curves.
	/// </summary>
	/// <typeparam name="value_t"></typeparam>
	template<typename value_t>
	struct ICurve
	{
		virtual const value_t Get(const value_t input) const { return input; }
	};

	namespace Template
	{
		/// <summary>
		/// Base template for curves.
		/// Curves of the same value_t can be template-chained to execute each step in line.
		/// </summary>
		/// <typeparam name="value_t"></typeparam>
		template<typename value_t>
		struct Curve
		{
			static constexpr value_t Get(const value_t input) { return input; }
		};

		/// <summary>
		/// Turns a Curve into a ICurve interface implemementer.
		/// </summary>
		/// <typeparam name="CurveType"></typeparam>
		/// <typeparam name="value_t"></typeparam>
		template<typename CurveType,
			typename value_t>
		struct Interface : public ICurve<value_t>
		{
			Interface() : ICurve<value_t>() {}

			virtual const value_t Get(const value_t input) const final
			{
				return CurveType::Get(input);
			}
		};
	}
}

#endif