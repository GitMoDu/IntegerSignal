# ![A toolkit for doing Digital Signal Processing on chips that are bad at math](https://github.com/GitMoDu/IntegerSignal/blob/master/Media/banner.jpg)

`A toolkit for doing Digital Signal Processing on chips that are bad at math.`

Header-only C++ library for fast, deterministic, integer-only signal processing and math utilities for microcontrollers. No floating point required.

- Core embedded utilities for constrained targets: lightweight integer math, traits, bit/byte helpers, conversions, mixing, integer square root
- Fixed-point Fraction [-1; 1] and Scale [1/N; N] primitives as first-class DSP features
- Composable Filters and Curves (EMA/DEMA/LowPass, Power/SquareRoot/Clamp...)
- Color models (RGB8 / RGB10 / Fraction 15:15:15) with HSV conversion and interpolation helpers
- Trigonometry: integer sine, cosine, and tangent using lookup tables (8-bit and 16-bit fractional angles) and non–lookup-table variants for minimal footprint

## Why IntegerSignal?

- Up to ~10× faster on MCUs without floating point
- Efficient bitwise/branchless code paths due to template specialization
- No external dependencies
- Requires only C++11 or later

## Key features

1) Fixed-point Fraction scalars (0..1 and −1..1)
- Unsigned: UQ0.7, UQ0.15, UQ0.31
- Signed: Q0.6, Q0.14, Q0.30
- Typed constants for 1.0 and −1.0
- Operations: multiply by a fractional scalar, interpolate with a fractional scalar, compute scalars from numerator/denominator with clamping

2) Fixed-point Scale factors (unbounded above 1.0)
- Widths: 8-bit, 16-bit, 32-bit with power-of-two units
- Operations: multiply by a scale factor, compute scale factors from numerator/denominator

Additional modules
- Filters: EMA, DEMA, LowPass, and helpers (composable, template-based)
- Curves: Power, Root (square root), Limit/Clamp, ScaleDown, Signed, Strength
- Color models: RGB8 (8 bits per channel), RGB10 (10 bits per channel), Fraction 15:15:15 for high-precision intermediate math; HSV conversion; integer-precise interpolation
- Trigonometry: integer implementations of sine, cosine, and tangent for fractional angles; headers with and without lookup tables to trade accuracy, speed, and size

## Quick start

Top-level includes (only headers directly in `src/`):
```cpp
#include <IntegerSignal.h>        // Umbrella header: Base, FixedPoint, Filters, Curves, Colors
#include <IntegerTrigonometry.h>  // Trigonometry without lookup tables (minimal footprint)
#include <IntegerTrigonometry8.h> // Trigonometry with 8-bit fractional-angle lookup tables (fast, small tables)
#include <IntegerTrigonometry16.h>// Trigonometry with 16-bit fractional-angle lookup tables (higher precision)
```

### Fractions and scales (minimal usage)

```cpp
#include <IntegerSignal.h>
using namespace IntegerSignal;

// Fraction (unsigned 0..1): compute 3/4 and apply
auto s = FixedPoint::ScalarFraction::GetScalarU16(3, 4);
uint16_t scaled = FixedPoint::ScalarFraction::Fraction<uint16_t>(s, 400); // -> 300

// Interpolate between 100 and 500 at 3/4
uint16_t y = FixedPoint::ScalarFraction::Interpolate<uint16_t>(s, 100, 500); // -> 400

// Scale factor (> 1.0 and down to 1/unit): map 0..1023 to 0..255
auto k = FixedPoint::FactorScale::GetFactor16(255, 1023);
uint16_t pwm = FixedPoint::FactorScale::Scale<uint16_t>(k, /*adc*/ 512); // ≈ 128
```

Notes
- Unsigned fractions clamp to [0,1]; signed fractions clamp to [-1,1].
- Scale factors use multiply-and-shift with power-of-two units (no divisions in steady state).

## Examples

- Benchmark and feature demos are provided in `Examples/Benchmark` and `Examples/Testing` (Arduino IDE: File → Examples → IntegerSignal).

## Performance notes

- Integer square root may use ARM CLZ acceleration where available
- Fraction/Scale helpers avoid intermediate overflows via widened math and power-of-two units

## Directory structure

- `src/FixedPoint/` — Fraction scalars and Scale factors (plus Q-format helpers)
- `src/Base/` — core embedded utilities (traits, operations, mix/interpolate, convert, bit/byte helpers, integer square root)
- `src/Filter/` — EMA, DEMA, LowPass, and helpers
- `src/Curve/` — Power, Root, Limit/Clamp, ScaleDown, etc.
- `src/Color/` — RGB8, RGB10, Fraction16, HSV
- `src/Trigonometry/` — integer trigonometry helpers
- `Examples/` — Arduino sketches

## Compatibility

- C++11 or later
- MCUs: 8‑bit AVR, ARM Cortex‑M, and desktop builds for testing
- Arduino-compatible (library.properties provided)
