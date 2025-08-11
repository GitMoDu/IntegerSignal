# IntegerSignal
A header-only C++ library for efficient integer-only signal processing and response curves, defined using templates for speed and (compile-time) flexibility.

## Features
- Integer-only Arithmetic: No floating point operations, compatible and fast even on 8 bit MCUs.

- Templates and Inheritance: Filters and Curves are compile time chainable, making it simple do define custom filter types.

- Filters: Includes implementations of EMA (Exponential Moving Average) and DEMA (Double Exponential Moving Average).

- Curves: Includes implementations for power^2, power^2, root^2. Square root operations are hardware accelerated on ARM architectures (_CLZ_).

- Colors: Native color models with optimized primitives, color interpolation and HSV color conversion.
  - (A)RGB 8:8:8
  - (A)RGB 10:10:10
  - Fraction 15:15:15 
