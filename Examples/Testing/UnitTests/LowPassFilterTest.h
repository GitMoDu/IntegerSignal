#ifndef _LOW_PASS_FILTER_TEST_h
#define _LOW_PASS_FILTER_TEST_h

#include <IntegerSignalTesting.h>
#include <IntegerSignal.h>

namespace IntegerSignal
{
	namespace Filters
	{
		namespace LowPass
		{
			namespace Test
			{
				// Helpers
				template<typename T>
				static T AbsDiff(T a, T b) { return (a > b) ? (a - b) : (b - a); }

				// Verify Clear initializes output to the given value and holds DC when input equals that value.
				template<typename FilterT, typename ValueT>
				static bool TestClearAndDC(const ValueT init, const uint32_t steps = 16)
				{
					FilterT f;
					f.Clear(init);

					uint32_t errors = 0;
					if (f.Get() != init)
					{
						Serial.print(F("LowPass Clear mismatch: got="));
						Serial.print(f.Get());
						Serial.print(F(" expected="));
						Serial.println(init);
						errors++;
					}

					for (uint32_t i = 0; i < steps; i++)
					{
						f.Set(init);
						f.Step();
						if (f.Get() != init)
						{
							Serial.print(F("LowPass DC hold mismatch at step "));
							Serial.print(i);
							Serial.print(F(": got="));
							Serial.print(f.Get());
							Serial.print(F(" expected="));
							Serial.println(init);
							errors++;
							break;
						}
					}
					return errors == 0;
				}

				// Verify step response is monotonic and converges to the target.
				template<typename FilterT, typename ValueT>
				static bool TestStepMonotonic(const ValueT start, const ValueT target, const uint32_t steps, const bool expectIncrease)
				{
					FilterT f;
					f.Clear(start);

					ValueT prev = f.Get();
					uint32_t errors = 0;

					for (uint32_t i = 0; i < steps; i++)
					{
						f.Set(target);
						f.Step();
						const ValueT y = f.Get();

						if (expectIncrease)
						{
							if (y < prev || y > target)
							{
								Serial.print(F("LowPass monotonic up violated at step "));
								Serial.print(i);
								Serial.print(F(" prev=")); Serial.print(prev);
								Serial.print(F(" y=")); Serial.print(y);
								Serial.print(F(" target=")); Serial.println(target);
								errors++; break;
							}
						}
						else
						{
							if (y > prev || y < target)
							{
								Serial.print(F("LowPass monotonic down violated at step "));
								Serial.print(i);
								Serial.print(F(" prev=")); Serial.print(prev);
								Serial.print(F(" y=")); Serial.print(y);
								Serial.print(F(" target=")); Serial.println(target);
								errors++; break;
							}
						}
						prev = y;
					}

					// After enough steps, it should reach the exact target for dyadic alpha
					if (errors == 0 && f.Get() != target)
					{
						Serial.print(F("LowPass did not settle exactly to target. y="));
						Serial.print(f.Get());
						Serial.print(F(" target="));
						Serial.println(target);
						errors++;
					}

					return errors == 0;
				}

				// Linearity (approximate under quantization): filter(k*x) ~= k*filter(x)
				// Allow tolerance to account for independent truncation in both runs.
				template<typename FilterT, typename ValueT>
				static bool TestLinearity(const ValueT maxInput, const uint8_t k, const uint8_t tolerance = 0)
				{
					const uint8_t tol = (tolerance == 0) ? static_cast<uint8_t>(k ? (k - 1) : 0) : tolerance;

					FilterT f1, f2;
					f1.Clear(0);
					f2.Clear(0);

					uint32_t errors = 0;

					for (ValueT x = 0; x <= maxInput; x++)
					{
						const ValueT x1 = x;
						const ValueT x2 = static_cast<ValueT>(x * k);

						f1.Set(x1); f1.Step();
						f2.Set(x2); f2.Step();

						const ValueT y1 = f1.Get();
						const ValueT y2 = f2.Get();

						const ValueT ky1 = static_cast<ValueT>(y1 * k);
						const ValueT diff = (ky1 > y2) ? (ky1 - y2) : (y2 - ky1);

						if (diff > tol)
						{
							Serial.print(F("LowPass linearity failed at x="));
							Serial.print(x);
							Serial.print(F(" k*y1=")); Serial.print(ky1);
							Serial.print(F(" y2=")); Serial.print(y2);
							Serial.print(F(" tol=")); Serial.println(tol);
							errors++; break;
						}
					}

					return errors == 0;
				}

				// Saturation: max input should reach and hold max output without overflow or decrease.
				template<typename FilterT, typename ValueT>
				static bool TestSaturation(const ValueT vmax, const uint32_t steps)
				{
					FilterT f;
					f.Clear(0);
					ValueT prev = 0;
					uint32_t errors = 0;

					for (uint32_t i = 0; i < steps; i++)
					{
						f.Set(vmax);
						f.Step();
						const ValueT y = f.Get();
						if (y < prev || y > vmax)
						{
							Serial.print(F("LowPass saturation monotonicity violated at step "));
							Serial.print(i);
							Serial.print(F(" prev=")); Serial.print(prev);
							Serial.print(F(" y=")); Serial.print(y);
							Serial.print(F(" vmax=")); Serial.println(vmax);
							errors++; break;
						}
						prev = y;
					}

					if (errors == 0 && prev != vmax)
					{
						Serial.print(F("LowPass saturation did not settle to vmax. y="));
						Serial.print(prev);
						Serial.print(F(" vmax="));
						Serial.println(vmax);
						errors++;
					}

					return errors == 0;
				}

				// Factor=0 should be exact pass-through after one Step.
				template<typename FilterT, typename ValueT>
				static bool TestPassThroughFactor0(const ValueT values[], const size_t count)
				{
					FilterT f;
					f.Clear(0);
					uint32_t errors = 0;

					for (size_t i = 0; i < count; i++)
					{
						f.Set(values[i]);
						f.Step();
						const ValueT y = f.Get();
						if (y != values[i])
						{
							Serial.print(F("LowPass factor=0 pass-through failed at index "));
							Serial.print(i);
							Serial.print(F(" in=")); Serial.print(values[i]);
							Serial.print(F(" y=")); Serial.println(y);
							errors++; break;
						}
					}
					return errors == 0;
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

				// -----------------------------------------------------------------------
				// Exhaustive and Sampled tests (like other suites)
				// -----------------------------------------------------------------------

				// Exhaustive one-step correctness: for all (y0, x) pairs, check the next output.
				template<uint8_t Factor>
				static bool TestLowPassU8ExhaustiveOneStep()
				{
					Serial.print(F("Starting exhaustive LowPassU8<"));
					Serial.print(Factor);
					Serial.println(F("> one-step tests..."));

					using F = IntegerSignal::Filters::LowPassU8<Factor>;

					uint32_t errorCount = 0;

					for (uint16_t y0 = 0; y0 <= UINT8_MAX; y0++)
					{
						for (uint16_t x = 0; x <= UINT8_MAX; x++)
						{
							F f; f.Clear((uint8_t)y0);
							f.Set((uint8_t)x);
							f.Step();
							const uint8_t y = f.Get();

							// Reference (done in a wider type)
							uint32_t H = ((uint32_t)y0) << Factor;
							H = H - (H >> Factor) + (uint32_t)x;
							const uint8_t yRef = (uint8_t)(H >> Factor);

							if (y != yRef)
							{
								Serial.print(F("LowPassU8<"));
								Serial.print(Factor);
								Serial.print(F("> one-step error: y0="));
								Serial.print((uint8_t)y0);
								Serial.print(F(" x="));
								Serial.print((uint8_t)x);
								Serial.print(F(" got="));
								Serial.print(y);
								Serial.print(F(" ref="));
								Serial.println(yRef);
								errorCount++;
							}
						}

						if ((y0 % 16) == 0)
						{
							Serial.print(F("LowPassU8<"));
							Serial.print(Factor);
							Serial.print(F("> progress: y0="));
							Serial.println((uint8_t)y0);
						}
					}

					if (errorCount == 0)
						Serial.println(F("All LowPassU8 one-step exhaustive tests PASSED."));
					else
					{
						Serial.print(F("LowPassU8 one-step exhaustive tests FAILED. Errors: "));
						Serial.println(errorCount);
					}

					return errorCount == 0;
				}

				// Sampled multi-step sequences: sweep inputs and compare to integer reference simulation.
				template<uint8_t Factor, uint32_t MaxIterations = 50000>
				static bool TestLowPassU8Sample()
				{
					const uint32_t iterations = MinValue<uint32_t>(MaxIterations, UINT16_MAX);
					Serial.print(F("Starting sampled LowPassU8<"));
					Serial.print(Factor);
					Serial.println(F("> tests..."));

					using F = IntegerSignal::Filters::LowPassU8<Factor>;

					const uint8_t initialY[] = { 0, 1, 64, 127, 128, 200, 255 };
					uint32_t errorCount = 0;

					for (uint8_t y0 : initialY)
					{
						F f; f.Clear(y0);

						// Reference accumulator (wide)
						uint32_t H = ((uint32_t)y0) << Factor;

						for (uint32_t i = 0; i <= iterations; i++)
						{
							const uint8_t x = (uint8_t)((i * UINT8_MAX) / (iterations ? (iterations) : 1));
							f.Set(x);
							f.Step();
							const uint8_t y = f.Get();

							// Reference step
							H = H - (H >> Factor) + (uint32_t)x;
							const uint8_t yRef = (uint8_t)(H >> Factor);

							if (y != yRef)
							{
								Serial.print(F("LowPassU8<"));
								Serial.print(Factor);
								Serial.print(F("> sampled error: y0="));
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
								Serial.print(F("LowPassU8<"));
								Serial.print(Factor);
								Serial.print(F("> progress: "));
								Serial.print(i);
								Serial.println(F(" samples..."));
							}
						}
					}

					if (errorCount == 0)
						Serial.println(F("All LowPassU8 sampled tests PASSED."));
					else
					{
						Serial.print(F("LowPassU8 sampled tests FAILED. Errors: "));
						Serial.println(errorCount);
					}

					return errorCount == 0;
				}

				// Sampled U16 multi-step sequences (edge-biased initial values and inputs).
				template<uint8_t Factor, uint32_t MaxIterations = 10000>
				static bool TestLowPassU16Sample()
				{
					const uint32_t iterations = MinValue<uint32_t>(MaxIterations, (uint32_t)UINT16_MAX);
					Serial.print(F("Starting sampled LowPassU16<"));
					Serial.print(Factor);
					Serial.println(F("> tests..."));

					using F = IntegerSignal::Filters::LowPassU16<Factor>;

					const uint16_t initialY[] = { 0u, 1u, 255u, 256u, 32767u, 32768u, 65535u };
					const uint16_t inputs[] = { 0u, 1u, 255u, 256u, 32767u, 32768u, 65535u };

					uint32_t errorCount = 0;

					for (uint16_t y0 : initialY)
					{
						for (uint16_t xEdge : inputs)
						{
							F f; f.Clear(y0);
							uint64_t H = ((uint64_t)y0) << Factor;

							for (uint32_t i = 0; i <= iterations; i += MaxValue<uint32_t>(1, iterations / 64))
							{
								// Mix between edge x and a sweep
								const uint16_t xSweep = (uint16_t)((i * UINT16_MAX) / (iterations ? iterations : 1));
								const uint16_t x = (uint16_t)((xSweep >> 1) + (xEdge >> 1)); // average to cover both

								f.Set(x);
								f.Step();
								const uint16_t y = f.Get();

								H = H - (H >> Factor) + (uint64_t)x;
								const uint16_t yRef = (uint16_t)(H >> Factor);

								if (y != yRef)
								{
									Serial.print(F("LowPassU16<"));
									Serial.print(Factor);
									Serial.print(F("> sampled error: y0="));
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

					if (errorCount == 0)
						Serial.println(F("All LowPassU16 sampled tests PASSED."));
					else
					{
						Serial.print(F("LowPassU16 sampled tests FAILED. Errors: "));
						Serial.println(errorCount);
					}

					return errorCount == 0;
				}

				// Sampled U32 multi-step sequences (edge-biased initial values and inputs).
				template<uint8_t Factor, uint32_t MaxIterations = 5000>
				static bool TestLowPassU32Sample()
				{
					const uint32_t iterations = MinValue<uint32_t>(MaxIterations, (uint32_t)65536);
					Serial.print(F("Starting sampled LowPassU32<"));
					Serial.print(Factor);
					Serial.println(F("> tests..."));

					using F = IntegerSignal::Filters::LowPassU32<Factor>;

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
							F f; f.Clear(y0);
							uint64_t H = ((uint64_t)y0) << Factor;

							for (uint32_t i = 0; i <= iterations; i += MaxValue<uint32_t>(1, iterations / 128))
							{
								// Mix between an edge x and a sweep across full range (downsampled)
								const uint32_t xSweep = (uint32_t)((uint64_t)i * 4294967295ULL / (iterations ? iterations : 1));
								const uint32_t x = (xSweep >> 1) + (xEdge >> 1);

								f.Set(x);
								f.Step();
								const uint32_t y = f.Get();

								H = H - (H >> Factor) + (uint64_t)x;
								const uint32_t yRef = (uint32_t)(H >> Factor);

								if (y != yRef)
								{
									Serial.print(F("LowPassU32<"));
									Serial.print(Factor);
									Serial.print(F("> sampled error: y0="));
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

					if (errorCount == 0)
						Serial.println(F("All LowPassU32 sampled tests PASSED."));
					else
					{
						Serial.print(F("LowPassU32 sampled tests FAILED. Errors: "));
						Serial.println(errorCount);
					}

					return errorCount == 0;
				}

				// -----------------------------------------------------------------------
				// Parametric run groups
				// -----------------------------------------------------------------------

				template<uint32_t MaxIterations, uint8_t Tol>
				static bool RunLowPassU8()
				{
					bool pass = true;

					// Exhaustive one-step verification for representative factors
					pass &= TestLowPassU8ExhaustiveOneStep<0>();
					pass &= TestLowPassU8ExhaustiveOneStep<4>();
					pass &= TestLowPassU8ExhaustiveOneStep<8>();

					// Sampled multi-step verification
					constexpr uint32_t Iter4 = MaxIterations;
					constexpr uint32_t Iter8 = (MaxIterations > 1) ? (MaxIterations / 2) : 1;
					pass &= TestLowPassU8Sample<4, Iter4>();
					pass &= TestLowPassU8Sample<8, Iter8>();

					// Factor=4 (typical)
					{
						using F = IntegerSignal::Filters::LowPassU8<4>;
						pass &= TestClearAndDC<F, uint8_t>(123);
						pass &= TestStepMonotonic<F, uint8_t>(0, 200, 1024, true);
						pass &= TestLinearity<F, uint8_t>(85, 3, Tol);     // 0..85 scaled by 3 fits in 8-bit
						pass &= TestSaturation<F, uint8_t>(UINT8_MAX, 1024);
					}

					// Factor=0 (pass-through)
					{
						using F0 = IntegerSignal::Filters::LowPassU8<0>;
						const uint8_t vec[] = { 0, 1, 7, 128, 200, 255 };
						pass &= TestPassThroughFactor0<F0, uint8_t>(vec, sizeof(vec) / sizeof(vec[0]));
					}

					// Factor=8 (max documented)
					{
						using F8 = IntegerSignal::Filters::LowPassU8<8>;
						pass &= TestStepMonotonic<F8, uint8_t>(0, 255, 4096, true);
						pass &= TestSaturation<F8, uint8_t>(UINT8_MAX, 4096);
					}

					return pass;
				}

				template<uint32_t MaxIterations, uint8_t Tol>
				static bool RunLowPassI8()
				{
					bool pass = true;

					// Factor=4 signed wrapper
					{
						using SF = IntegerSignal::Filters::LowPassI8<4>;
						pass &= TestSignedBehavior<SF, int8_t>(-64, 64, 1024);
					}

					// Factor=0 signed wrapper (pass-through)
					{
						using SF0 = IntegerSignal::Filters::LowPassI8<0>;
						SF0 f; f.Clear();
						const int8_t inputs[] = { -128, -64, -1, 0, 1, 64, 127 };
						bool ok = true;
						for (size_t i = 0; i < sizeof(inputs) / sizeof(inputs[0]); i++)
						{
							f.Set(inputs[i]); f.Step();
							if (f.Get() != inputs[i]) { ok = false; break; }
						}
						if (!ok) Serial.println(F("LowPassI8<0> pass-through failed."));
						pass &= ok;
					}

					return pass;
				}

				template<uint32_t MaxIterations, uint8_t Tol>
				static bool RunLowPassU16()
				{
					bool pass = true;

					// Sampled multi-step verification
					constexpr uint32_t IterU16 = (MaxIterations > 4) ? (MaxIterations / 5) : 1;
					pass &= TestLowPassU16Sample<8, IterU16>();

					// A lightweight sanity for wider type
					{
						using F = IntegerSignal::Filters::LowPassU16<8>;
						pass &= TestClearAndDC<F, uint16_t>(43210);
						pass &= TestStepMonotonic<F, uint16_t>(0, 60000, 4096, true);
						pass &= TestSaturation<F, uint16_t>(UINT16_MAX, 4096);
					}

					return pass;
				}

				template<uint32_t MaxIterations, uint8_t Tol>
				static bool RunLowPassU32()
				{
					bool pass = true;

					// Sampled multi-step verification for U32
					constexpr uint32_t IterU32 = (MaxIterations > 10) ? (MaxIterations / 10) : 1;
					pass &= TestLowPassU32Sample<8, IterU32>();

					// Basic sanity for U32
					{
						using F = IntegerSignal::Filters::LowPassU32<8>;
						pass &= TestClearAndDC<F, uint32_t>(3000000000u);
						pass &= TestStepMonotonic<F, uint32_t>(0u, 4294967295u, 8192, true);
						pass &= TestSaturation<F, uint32_t>(UINT32_MAX, 8192);
					}

					// Factor=0 (pass-through) U32
					{
						using F0 = IntegerSignal::Filters::LowPassU32<0>;
						const uint32_t vec[] = { 0u, 1u, 255u, 256u, 65535u, 65536u, 2147483647u, 2147483648u, 4294967295u };
						pass &= TestPassThroughFactor0<F0, uint32_t>(vec, sizeof(vec) / sizeof(vec[0]));
					}

					return pass;
				}

				template<uint32_t MaxIterations, uint8_t Tol>
				static bool RunLowPassI32()
				{
					bool pass = true;

					// Factor=4 signed wrapper
					{
						using SF = IntegerSignal::Filters::LowPassI32<4>;
						pass &= TestSignedBehavior<SF, int32_t>(-1000000000, 1000000000, 8192);
					}

					// Factor=0 signed wrapper (pass-through)
					{
						using SF0 = IntegerSignal::Filters::LowPassI32<0>;
						SF0 f; f.Clear();
						const int32_t inputs[] = { INT32_MIN, -1000000000, -1, 0, 1, 1000000000, INT32_MAX };
						bool ok = true;
						for (size_t i = 0; i < sizeof(inputs) / sizeof(inputs[0]); i++)
						{
							f.Set(inputs[i]); f.Step();
							if (f.Get() != inputs[i]) { ok = false; break; }
						}
						if (!ok) Serial.println(F("LowPassI32<0> pass-through failed."));
						pass &= ok;
					}

					return pass;
				}

				// Main entry, templated like other test files.
				template<uint32_t MaxIterations = 50000, uint8_t LinearityTolerance = 0>
				static bool RunTests()
				{
					Serial.println(F("Starting LowPass filter tests..."));
					bool pass = true;

					pass &= RunLowPassU8<MaxIterations, LinearityTolerance>();
					pass &= RunLowPassI8<MaxIterations, LinearityTolerance>();
					pass &= RunLowPassU16<MaxIterations, LinearityTolerance>();
					pass &= RunLowPassU32<MaxIterations, LinearityTolerance>();
					pass &= RunLowPassI32<MaxIterations, LinearityTolerance>();

					if (pass) Serial.println(F("LowPass filter tests PASSED."));
					else      Serial.println(F("LowPass filter tests FAILED."));

					return pass;
				}
			}
		}
	}
}
#endif