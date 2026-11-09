#ifndef _FIXED_POINT_FRACTION_TEST_h
#define _FIXED_POINT_FRACTION_TEST_h

#include "FixedPointFractionApiConsistencyTest.h"
#include "FixedPointFractionScalarTest.h"
#include "FixedPointFractionSignedTest.h"
#include "FixedPointFractionUnsignedTest.h"
#include "FixedPointFractionInterpolateTest.h"

namespace IntegerSignal
{
	namespace FixedPoint
	{
		namespace ScalarFraction
		{
			namespace Test
			{
				template<uint32_t MaxIterations = 50000, uint8_t maxError32 = 0>
				static inline bool RunTests()
				{
					bool pass = true;

					pass &= TestGetScalarU8ExhaustiveValidation();
					pass &= TestGetScalarU8SignedInputs();
					pass &= TestGetScalarFastUnsignedRegression();
					pass &= TestSplitScalarApiConsistency();
					pass &= TestGetFraction8Exhaustive();
					pass &= TestGetScalarWideIntermediateRegression();
					pass &= TestUFraction8Types();

					pass &= TestUInterpolate8();
					pass &= TestSFraction8EdgeCases();

					pass &= TestFraction8Exhaustive();
					pass &= TestUFraction8Exhaustive();

					pass &= TestUFraction16Edges();

					pass &= TestFraction8Sample<MaxIterations>();
					pass &= TestUFraction8Sample<MaxIterations>();

					pass &= TestFraction16Sample<MaxIterations>();
					pass &= TestUFraction16Sample<MaxIterations>();

					pass &= TestFraction32Sample<MaxIterations>();
					pass &= TestUFraction32Sample<MaxIterations>();

					if (!pass)
					{
						Serial.println(F("FractionScale tests FAILED."));
					}

					return pass;
				}

				static inline bool RunExhaustive()
				{
					bool pass = true;
					pass &= TestFraction16Exhaustive();
					pass &= TestUFraction16Exhaustive();

					return pass;
				}
			}
		}
	}
}

#endif
