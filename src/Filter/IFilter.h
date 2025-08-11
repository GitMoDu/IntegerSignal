// IFilter.h

#ifndef _I_FILTER_h
#define _I_FILTER_h

#include <stdint.h>

namespace IntegerSignal
{
	namespace Filter
	{
		/// <summary>
		/// Templated interface for filters.
		/// </summary>
		/// <typeparam name="value_t"></typeparam>
		template<typename value_t>
		struct IFilter
		{
			IFilter() {}

			/// <summary>
			/// Clear the state.
			/// </summary>
			virtual void Clear() {}

			/// <summary>
			/// Set the input value.
			/// </summary>
			/// <param name="value"></param>
			virtual void Set(const value_t value) {}

			/// <summary>
			/// Step the current input to update the output.
			/// </summary>
			virtual void Step() {}

			/// <summary>
			/// Get the latest output value.
			/// </summary>
			/// <returns></returns>
			virtual const value_t Get() const { return 0; }
		};

		template<typename value_t>
		class AbstractFilter : public IFilter<value_t>
		{
		protected:
			value_t Input = 0;

		public:
			AbstractFilter() : IFilter<value_t>() {}

			virtual void Set(const value_t value)
			{
				Input = value;
			}

			virtual void Clear(const value_t value = 0)
			{
				Input = value;
			}
		};
	}
}
#endif