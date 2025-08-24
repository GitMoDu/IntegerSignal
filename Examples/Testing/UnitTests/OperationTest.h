#ifndef _OPERATION_TEST_h
#define _OPERATION_TEST_h

#include <IntegerSignalTesting.h>

namespace IntegerSignal::Operation::Test
{
	static bool TestMinValue()
	{
		bool pass = true;
		pass &= (MinValue(5, 10) == 5);
		pass &= (MinValue(-3, 2) == -3);
		pass &= (MinValue(0u, 255u) == 0u);
		pass &= (MinValue(100, 100) == 100);
		return pass;
	}

	static bool TestMaxValue()
	{
		bool pass = true;
		pass &= (MaxValue(5, 10) == 10);
		pass &= (MaxValue(-3, 2) == 2);
		pass &= (MaxValue(0u, 255u) == 255u);
		pass &= (MaxValue(100, 100) == 100);
		return pass;
	}

	static bool TestLimitValue()
	{
		bool pass = true;
		pass &= (LimitValue(5, 0, 10) == 5);
		pass &= (LimitValue(-5, 0, 10) == 0);
		pass &= (LimitValue(15, 0, 10) == 10);
		pass &= (LimitValue(10, 0, 10) == 10);
		pass &= (LimitValue(0, 0, 10) == 0);
		return pass;
	}

	static bool TestAbsValue()
	{
		bool pass = true;
		pass &= (AbsValue(5) == 5);
		pass &= (AbsValue(-5) == 5);
		pass &= (AbsValue(0) == 0);
		pass &= (AbsValue(-128) == 128);
		return pass;
	}

	static bool TestGetBitShifts()
	{
		bool pass = true;
		pass &= (GetBitShifts(0) == 0);
		pass &= (GetBitShifts(1) == 0);
		pass &= (GetBitShifts(2) == 1);
		pass &= (GetBitShifts(8) == 3);
		pass &= (GetBitShifts(255) == 7);
		return pass;
	}

	static bool TestSignedRightShift()
	{
		bool pass = true;
		pass &= (SignedRightShift(8, 1) == 4);
		pass &= (SignedRightShift(-8, 1) == -4);
		pass &= (SignedRightShift(7, 2) == 1);
		pass &= (SignedRightShift(-7, 2) == -1);
		pass &= (SignedRightShift(0, 3) == 0);
		return pass;
	}

	static bool TestSignedRightShiftTemplate()
	{
		bool pass = true;
		pass &= (SignedRightShift<int16_t, 2>(8) == 2);
		pass &= (SignedRightShift<int16_t, 2>(-8) == -2);
		pass &= (SignedRightShift<int16_t, 0>(5) == 5);
		pass &= (SignedRightShift<int16_t, 3>(-16) == -2);
		return pass;
	}

	static bool RunTests()
	{
		Serial.println(F("Starting Operation tests..."));
		bool pass = true;
		pass &= TestMinValue();
		pass &= TestMaxValue();
		pass &= TestLimitValue();
		pass &= TestAbsValue();
		pass &= TestGetBitShifts();
		pass &= TestSignedRightShift();
		pass &= TestSignedRightShiftTemplate();

		if (pass)
			Serial.println(F("Operation tests PASSED."));
		else
			Serial.println(F("Operation tests FAILED."));

		return pass;
	}
}

#endif

