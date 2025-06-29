#ifndef _INTEGER_SIGNAL_BIT_SIZE_h
#define _INTEGER_SIGNAL_BIT_SIZE_h

#include <stdint.h>

namespace IntegerSignal
{
	/// <summary>
	/// BitSize provides utilities for compile-time bit-level calculations and type selection.
	/// It contains helpers for representing constant values, returning types, and determining
	/// the number of bits required to store a given value at compile time.
	/// </summary>
	namespace BitSize
	{
		/// <summary>
		/// BitType contains internal helpers for representing compile-time constants
		/// and for returning types in a template context.
		/// </summary>
		namespace BitType
		{
			/// <summary>
			/// Compile-time constant value.
			/// </summary>
			/// <typeparam name="N">The constant value.</typeparam>
			template<uint64_t N>
			struct constant
			{
				enum { value = N };
			};

			/// <summary>
			/// Helper for returning a type.
			/// </summary>
			/// <typeparam name="T">The type to return.</typeparam>
			template<typename T>
			struct return_type
			{
				typedef T type;
			};
		}

		/// <summary>
		/// Computes the number of bits required to represent N.
		/// </summary>
		/// <typeparam name="N">The value to analyze.</typeparam>
		template<uint64_t N>
		struct bit_count : BitType::constant<1 + bit_count<(N >> 1)>::value> {};

		/// <summary>
		/// Specialization for N = 0.
		/// </summary>
		template<>
		struct bit_count<0> : BitType::constant<1> {};

		/// <summary>
		/// Specialization for N = 1.
		/// </summary>
		template<>
		struct bit_count<1> : BitType::constant<1> {};
	}

	using namespace BitSize;
}
#endif