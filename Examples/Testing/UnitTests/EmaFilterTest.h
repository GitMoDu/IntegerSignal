#ifndef _EMA_FILTER_TEST_h
#define _EMA_FILTER_TEST_h

#include <IntegerSignalTesting.h>
#include <IntegerSignal.h>

namespace IntegerSignal
{
	namespace Filters
	{
		namespace Ema
		{
			namespace Test
			{
				// Helpers
				template<typename T>
				static T AbsDiff(T a, T b) { return (a > b) ? (a - b) : (b - a); }

				// EMA reference step: given residual R (post-step accumulator), input x and factor,
				// compute next output y and update residual R: S = R + x; y = round(S/2^f); R' = S - y.
				template<typename ResidualT, typename InputT, uint8_t Factor>
				static inline InputT EmaRefStep(ResidualT& residual, const InputT x)
				{
					const ResidualT half = (ResidualT)1 << (Factor - 1);
					const ResidualT s = residual + (ResidualT)x;
					const InputT y = (InputT)((s + half) >> Factor);
					residual = s - (ResidualT)y;
					return y;
				}

				// Clear check (EMA-specific semantics):
				// - After Clear(initRaw), Output == round(initRaw / 2^factor)
				// - Internal residual becomes initRaw - Output
				template<typename FilterT, typename ValueT, uint8_t Factor>
				static bool TestClearSemantics(const ValueT initRaw)
				{
					FilterT f;
					f.Clear(initRaw);

					const ValueT expected = (ValueT)((((uint64_t)initRaw) + ((uint64_t)1 << (Factor - 1))) >> Factor);
					const ValueT got = f.Get();
					if (got != expected)
					{
						Serial.print(F("EMA Clear mismatch: raw="));
						IntegerSignal::Testing::PrintUInt64(initRaw);
						Serial.print(F(" got="));
						IntegerSignal::Testing::PrintUInt64(got);
						Serial.print(F(" expected="));
						IntegerSignal::Testing::PrintUInt64(expected);
						Serial.println();
						return false;
					}
					return true;
				}

				// DC convergence: starting from Clear(0), with constant input 'dc', output must converge to 'dc'.
				template<typename FilterT, typename ValueT>
				static bool TestDCConvergence(const ValueT dc, const uint32_t steps)
				{
					FilterT f;
					f.Clear(0);

					ValueT prev = f.Get();
					for (uint32_t i = 0; i < steps; i++)
					{
						f.Set(dc);
						f.Step();
						const ValueT y = f.Get();
						// No overshoot
						if ((y > dc) || (y < prev))
						{
							Serial.print(F("EMA DC convergence violated at step "));
							Serial.print(i);
							Serial.print(F(" prev=")); Serial.print(prev);
							Serial.print(F(" y=")); Serial.print(y);
							Serial.print(F(" dc=")); Serial.println(dc);
							return false;
						}
						prev = y;
					}

					if (prev != dc)
					{
						Serial.print(F("EMA DC did not settle to target. y="));
						IntegerSignal::Testing::PrintUInt64(prev);
						Serial.print(F(" dc="));
						IntegerSignal::Testing::PrintUInt64(dc);
						Serial.println();
						return false;
					}
					return true;
				}

				// Monotonic step response (non-decreasing if stepping up, non-increasing if stepping down).
				template<typename FilterT, typename ValueT, uint8_t Factor>
				static bool TestStepMonotonic(const ValueT start, const ValueT target, const uint32_t steps, const bool expectIncrease)
				{
					FilterT f;
					// Put filter into steady-state output=start:
					// EMA steady state for output=y0 is achieved by Clear(y0 << Factor).
					f.Clear((ValueT)(start << Factor));

					ValueT prev = f.Get();
					if (prev != start)
					{
						Serial.print(F("EMA precondition failed (steady-state init). start="));
						Serial.print(start);
						Serial.print(F(" got="));
						Serial.println(prev);
						return false;
					}

					for (uint32_t i = 0; i < steps; i++)
					{
						f.Set(target);
						f.Step();
						const ValueT y = f.Get();

						if (expectIncrease)
						{
							if (y < prev || y > target)
							{
								Serial.print(F("EMA monotonic up violated at step "));
								Serial.print(i);
								Serial.print(F(" prev=")); Serial.print(prev);
								Serial.print(F(" y=")); Serial.print(y);
								Serial.print(F(" target=")); Serial.println(target);
								return false;
							}
						}
						else
						{
							if (y > prev || y < target)
							{
								Serial.print(F("EMA monotonic down violated at step "));
								Serial.print(i);
								Serial.print(F(" prev=")); Serial.print(prev);
								Serial.print(F(" y=")); Serial.print(y);
								Serial.print(F(" target=")); Serial.println(target);
								return false;
							}
						}
						prev = y;
					}

					if (prev != target)
					{
						Serial.print(F("EMA did not settle exactly to target. y="));
						IntegerSignal::Testing::PrintUInt64(prev);
						Serial.print(F(" target="));
						IntegerSignal::Testing::PrintUInt64(target);
						Serial.println();
						return false;
					}

					return true;
				}

				// Linearity (approximate under quantization): filter(k*x) ~= k*filter(x) with bounded tolerance
				template<typename FilterT, typename ValueT>
				static bool TestLinearity(const ValueT maxInput, const uint8_t k, const uint8_t tolerance = 0)
				{
					const uint8_t tol = (tolerance == 0) ? (uint8_t)(k ? (k - 1) : 0) : tolerance;

					FilterT f1, f2;
					f1.Clear(0);
					f2.Clear(0);

					for (ValueT x = 0; x <= maxInput; x++)
					{
						f1.Set(x); f1.Step();
						f2.Set((ValueT)(x * k)); f2.Step();

						const ValueT y1 = f1.Get();
						const ValueT y2 = f2.Get();
						const ValueT ky1 = (ValueT)(y1 * k);

						const ValueT diff = (ky1 > y2) ? (ky1 - y2) : (y2 - ky1);
						if (diff > tol)
						{
							Serial.print(F("EMA linearity failed at x="));
							IntegerSignal::Testing::PrintUInt64(x);
							Serial.print(F(" k*y1=")); IntegerSignal::Testing::PrintUInt64(ky1);
							Serial.print(F(" y2=")); IntegerSignal::Testing::PrintUInt64(y2);
							Serial.print(F(" tol=")); Serial.println(tol);
							return false;
						}
					}
					return true;
				}

				// Saturation: max input should reach and hold max output without overflow or decrease.
				template<typename FilterT, typename ValueT>
				static bool TestSaturation(const ValueT vmax, const uint32_t steps)
				{
					FilterT f;
					f.Clear(0);
					ValueT prev = 0;

					for (uint32_t i = 0; i < steps; i++)
					{
						f.Set(vmax);
						f.Step();
						const ValueT y = f.Get();
						if (y < prev || y > vmax)
						{
							Serial.print(F("EMA saturation monotonicity violated at step "));
							Serial.print(i);
							Serial.print(F(" prev=")); IntegerSignal::Testing::PrintUInt64(prev);
							Serial.print(F(" y=")); IntegerSignal::Testing::PrintUInt64(y);
							Serial.print(F(" vmax=")); IntegerSignal::Testing::PrintUInt64(vmax);
							Serial.println();
							return false;
						}
						prev = y;
					}
					if (prev != vmax)
					{
						Serial.print(F("EMA saturation did not settle to vmax. y="));
						IntegerSignal::Testing::PrintUInt64(prev);
						Serial.print(F(" vmax="));
						IntegerSignal::Testing::PrintUInt64(vmax);
						Serial.println();
						return false;
					}
					return true;
				}

				// -----------------------------------------------------------------------
				// Exhaustive and Sampled tests (like LowPass)
				// -----------------------------------------------------------------------

				// Exhaustive one-step correctness for U8 EMA:
				// For each output y0 and input x, initialize to steady state with Clear(y0 << Factor),
				// step once with x, and compare to reference integer EMA.
				template<uint8_t Factor>
				static bool TestEmaU8ExhaustiveOneStep()
				{
					static_assert(Factor >= 1, "EMA requires factor >= 1");
					Serial.print(F("Starting exhaustive EmaU8<"));
					Serial.print(Factor);
					Serial.println(F("> one-step tests..."));

					using F = IntegerSignal::Filters::EmaU8<Factor>;

					uint32_t errorCount = 0;

					for (uint16_t y0 = 0; y0 <= UINT8_MAX; y0++)
					{
						for (uint16_t x = 0; x <= UINT8_MAX; x++)
						{
							F f; f.Clear((uint8_t)y0); // was y0 << Factor
							f.Set((uint8_t)x);
							f.Step();
							const uint8_t y = f.Get();

							uint32_t residual = (uint32_t)y0 * ((1u << Factor) - 1u);
							const uint8_t yRef = EmaRefStep<uint32_t, uint8_t, Factor>(residual, (uint8_t)x);

							if (y != yRef)
							{
								Serial.print(F("EmaU8<")); Serial.print(Factor); Serial.print(F("> one-step error: y0="));
								Serial.print((uint8_t)y0);
								Serial.print(F(" x=")); Serial.print((uint8_t)x);
								Serial.print(F(" got=")); Serial.print(y);
								Serial.print(F(" ref=")); Serial.println(yRef);
								errorCount++;
							}
						}
						if ((y0 % 16) == 0)
						{
							Serial.print(F("EmaU8<")); Serial.print(Factor); Serial.print(F("> progress: y0="));
							Serial.println((uint8_t)y0);
						}
					}

					if (errorCount == 0)
						Serial.println(F("All EmaU8 one-step exhaustive tests PASSED."));
					else
					{
						Serial.print(F("EmaU8 one-step exhaustive tests FAILED. Errors: "));
						Serial.println(errorCount);
					}
					return errorCount == 0;
				}

				// Sampled multi-step sequences: sweep inputs and compare to integer EMA reference.
				template<uint8_t Factor, uint32_t MaxIterations = 50000>
				static bool TestEmaU8Sample()
				{
					static_assert(Factor >= 1, "EMA requires factor >= 1");
					const uint32_t iterations = MinValue<uint32_t>(MaxIterations, UINT16_MAX);
					Serial.print(F("Starting sampled EmaU8<")); Serial.print(Factor); Serial.println(F("> tests..."));

					using F = IntegerSignal::Filters::EmaU8<Factor>;
					const uint8_t initialY[] = { 0, 1, 64, 127, 128, 200, 255 };

					uint32_t errorCount = 0;

					for (uint8_t y0 : initialY)
					{
						F f; f.Clear(y0); // was y0 << Factor

						uint32_t residual = (uint32_t)y0 * ((1u << Factor) - 1u);

						for (uint32_t i = 0; i <= iterations; i++)
						{
							const uint8_t x = (uint8_t)((i * UINT8_MAX) / (iterations ? iterations : 1));
							f.Set(x);
							f.Step();
							const uint8_t y = f.Get();

							const uint8_t yRef = EmaRefStep<uint32_t, uint8_t, Factor>(residual, x);

							if (y != yRef)
							{
								Serial.print(F("EmaU8<")); Serial.print(Factor); Serial.print(F("> sampled error: y0="));
								Serial.print(y0);
								Serial.print(F(" i=")); Serial.print(i);
								Serial.print(F(" x=")); Serial.print(x);
								Serial.print(F(" y=")); Serial.print(y);
								Serial.print(F(" ref=")); Serial.println(yRef);
								errorCount++;
								break;
							}

							if ((i % (MaxValue<uint32_t>(1, iterations / 10))) == 0)
							{
								Serial.print(F("EmaU8<")); Serial.print(Factor); Serial.print(F("> progress: "));
								Serial.print(i); Serial.println(F(" samples..."));
							}
						}
					}

					if (errorCount == 0) Serial.println(F("All EmaU8 sampled tests PASSED."));
					else { Serial.print(F("EmaU8 sampled tests FAILED. Errors: ")); Serial.println(errorCount); }
					return errorCount == 0;
				}

				// Sampled U16 multi-step sequences (edges + sweep).
				template<uint8_t Factor, uint32_t MaxIterations = 10000>
				static bool TestEmaU16Sample()
				{
					static_assert(Factor >= 1, "EMA requires factor >= 1");
					const uint32_t iterations = MinValue<uint32_t>(MaxIterations, (uint32_t)UINT16_MAX);
					Serial.print(F("Starting sampled EmaU16<")); Serial.print(Factor); Serial.println(F("> tests..."));

					using F = IntegerSignal::Filters::EmaU16<Factor>;

					const uint16_t initialY[] = { 0u, 1u, 255u, 256u, 32767u, 32768u, 65535u };
					const uint16_t inputs[] = { 0u, 1u, 255u, 256u, 32767u, 32768u, 65535u };

					uint32_t errorCount = 0;

					for (uint16_t y0 : initialY)
					{
						for (uint16_t xEdge : inputs)
						{
							F f; f.Clear(y0); // was y0 << Factor
							uint64_t residual = (uint64_t)y0 * ((1u << Factor) - 1u);

							for (uint32_t i = 0; i <= iterations; i += MaxValue<uint32_t>(1, iterations / 64))
							{
								const uint16_t xSweep = (uint16_t)((i * UINT16_MAX) / (iterations ? iterations : 1));
								const uint16_t x = (uint16_t)((xSweep >> 1) + (xEdge >> 1));

								f.Set(x);
								f.Step();
								const uint16_t y = f.Get();

								const uint16_t yRef = EmaRefStep<uint64_t, uint16_t, Factor>(residual, x);

								if (y != yRef)
								{
									Serial.print(F("EmaU16<")); Serial.print(Factor); Serial.print(F("> sampled error: y0="));
									Serial.print(y0);
									Serial.print(F(" x=")); Serial.print(x);
									Serial.print(F(" y=")); Serial.print(y);
									Serial.print(F(" ref=")); Serial.println(yRef);
									errorCount++;
									break;
								}
							}
						}
					}

					if (errorCount == 0) Serial.println(F("All EmaU16 sampled tests PASSED."));
					else { Serial.print(F("EmaU16 sampled tests FAILED. Errors: ")); Serial.println(errorCount); }
					return errorCount == 0;
				}

				// Sampled U32 multi-step sequences (edges + sweep).
				template<uint8_t Factor, uint32_t MaxIterations = 5000>
				static bool TestEmaU32Sample()
				{
					static_assert(Factor >= 1, "EMA requires factor >= 1");
					const uint32_t iterations = MinValue<uint32_t>(MaxIterations, (uint32_t)65536);
					Serial.print(F("Starting sampled EmaU32<")); Serial.print(Factor); Serial.println(F("> tests..."));

					using F = IntegerSignal::Filters::EmaU32<Factor>;

					const uint32_t initialY[] = {
						0u, 1u, 255u, 256u, 65535u, 65536u, 2147483647u, 2147483648u, 4294967295u
					};
					const uint32_t inputs[] = {
						0u, 1u, 255u, 256u, 65535u, 65536u, 2147483647u, 2147483648u, 4294967295u
					};

					uint32_t errorCount = 0;

					for (uint32_t y0 : initialY)
					{
						for (uint32_t xEdge : inputs)
						{
							F f; f.Clear(y0); // was y0 << Factor
							uint64_t residual = (uint64_t)y0 * ((1u << Factor) - 1u);

							for (uint32_t i = 0; i <= iterations; i += MaxValue<uint32_t>(1, iterations / 128))
							{
								const uint32_t xSweep = (uint32_t)((uint64_t)i * 4294967295ULL / (iterations ? iterations : 1));
								const uint32_t x = (xSweep >> 1) + (xEdge >> 1);

								f.Set(x);
								f.Step();
								const uint32_t y = f.Get();

								const uint32_t yRef = EmaRefStep<uint64_t, uint32_t, Factor>(residual, x);

								if (y != yRef)
								{
									Serial.print(F("EmaU32<")); Serial.print(Factor); Serial.print(F("> sampled error: y0="));
									IntegerSignal::Testing::PrintUInt64(y0);
									Serial.print(F(" x=")); IntegerSignal::Testing::PrintUInt64(x);
									Serial.print(F(" y=")); IntegerSignal::Testing::PrintUInt64(y);
									Serial.print(F(" ref=")); IntegerSignal::Testing::PrintUInt64(yRef);
									Serial.println();
									errorCount++;
									break;
								}
							}
						}
					}

					if (errorCount == 0) Serial.println(F("All EmaU32 sampled tests PASSED."));
					else { Serial.print(F("EmaU32 sampled tests FAILED. Errors: ")); Serial.println(errorCount); }
					return errorCount == 0;
				}


				// -----------------------------------------------------------------------
				// Parametric run groups
				// -----------------------------------------------------------------------

				template<uint32_t MaxIterations, uint8_t Tol>
				static bool RunEmaU8()
				{
					bool pass = true;

					// Exhaustive one-step verification for representative factors (>=1)
					pass &= TestEmaU8ExhaustiveOneStep<1>();
					pass &= TestEmaU8ExhaustiveOneStep<4>();
					pass &= TestEmaU8ExhaustiveOneStep<8>();

					// Sampled multi-step verification
					pass &= TestEmaU8Sample<4, MaxIterations>();
					pass &= TestEmaU8Sample<8, (MaxIterations > 1 ? MaxIterations / 2 : 1)>();

					// DC/monotonic/saturation & linearity (factor=4)
					{
						using F = IntegerSignal::Filters::EmaU8<4>;
						pass &= TestClearSemantics<F, uint8_t, 4>(0);
						pass &= TestDCConvergence<F, uint8_t>(200, 2048);
						pass &= TestStepMonotonic<F, uint8_t, 4>(0, 200, 2048, true);
						pass &= TestLinearity<F, uint8_t>(85, 3, Tol);
						pass &= TestSaturation<F, uint8_t>(UINT8_MAX, 4096);
					}

					return pass;
				}

				// Signed wrapper tests: symmetric behavior around 0, both directions monotonic.
				template<typename SFilterT, typename ValueT>
				static bool TestSignedBehavior(const ValueT neg, const ValueT pos, const uint32_t steps)
				{
					SFilterT f;
					f.Clear();

					uint32_t errors = 0;

					// Step to positive
					{
						ValueT prev = f.Get();
						for (uint32_t i = 0; i < steps; i++)
						{
							f.Set(pos);
							f.Step();
							const ValueT y = f.Get();
							if (y < prev || y > pos) { errors++; break; }
							prev = y;
						}
						if (errors == 0 && f.Get() != pos) errors++;
					}

					// Step to negative
					if (errors == 0)
					{
						ValueT prev = f.Get();
						for (uint32_t i = 0; i < steps; i++)
						{
							f.Set(neg);
							f.Step();
							const ValueT y = f.Get();
							if (y > prev || y < neg) { errors++; break; }
							prev = y;
						}
						if (errors == 0 && f.Get() != neg) errors++;
					}

					return errors == 0;
				}

				template<uint32_t MaxIterations, uint8_t Tol>
				static bool RunEmaI8()
				{
					bool pass = true;

					// Factor=4 signed wrapper monotonicity both ways
					{
						using SF = IntegerSignal::Filters::EmaI8<4>;
						pass &= TestSignedBehavior<SF, int8_t>(-64, 64, 2048);
					}
					return pass;
				}

				template<uint32_t MaxIterations, uint8_t Tol>
				static bool RunEmaU16()
				{
					bool pass = true;

					// Sampled multi-step verification
					pass &= TestEmaU16Sample<8, (MaxIterations > 4 ? MaxIterations / 5 : 1)>();

					// Basic sanity
					{
						using F = IntegerSignal::Filters::EmaU16<8>;
						pass &= TestClearSemantics<F, uint16_t, 8>(0);
						pass &= TestDCConvergence<F, uint16_t>(60000, 4096);
						pass &= TestStepMonotonic<F, uint16_t, 8>(0, 60000, 4096, true);
						pass &= TestSaturation<F, uint16_t>(UINT16_MAX, 8192);
					}

					return pass;
				}

				template<uint32_t MaxIterations, uint8_t Tol>
				static bool RunEmaU32()
				{
					bool pass = true;

					// Sampled multi-step verification
					pass &= TestEmaU32Sample<8, (MaxIterations > 10 ? MaxIterations / 10 : 1)>();

					// Basic sanity
					{
						using F = IntegerSignal::Filters::EmaU32<8>;
						pass &= TestClearSemantics<F, uint32_t, 8>(0u);
						pass &= TestDCConvergence<F, uint32_t>(4294967295u, 8192);
						pass &= TestStepMonotonic<F, uint32_t, 8>(0u, 4294967295u, 8192, true);
						pass &= TestSaturation<F, uint32_t>(UINT32_MAX, 8192);
					}

					return pass;
				}


				template<uint32_t MaxIterations, uint8_t Tol>
				static bool RunEmaI32()
				{
					bool pass = true;

					// Factor=4 signed wrapper monotonicity both ways
					{
						using SF = IntegerSignal::Filters::EmaI32<4>;
						pass &= TestSignedBehavior<SF, int32_t>(-1000000000, 1000000000, 8192);
					}
					return pass;
				}


				// Main entry (templated like other suites)
				template<uint32_t MaxIterations = 50000, uint8_t LinearityTolerance = 0>
				static bool RunTests()
				{
					Serial.println(F("Starting EMA filter tests..."));
					bool pass = true;

					pass &= RunEmaU8<MaxIterations, LinearityTolerance>();
					pass &= RunEmaI8<MaxIterations, LinearityTolerance>();
					pass &= RunEmaU16<MaxIterations, LinearityTolerance>();
					pass &= RunEmaU32<MaxIterations, LinearityTolerance>();
					pass &= RunEmaI32<MaxIterations, LinearityTolerance>();

					if (pass) Serial.println(F("EMA filter tests PASSED."));
					else      Serial.println(F("EMA filter tests FAILED."));

					return pass;
				}
			}
		}
	}
}
#endif