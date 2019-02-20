#pragma once
#include "Window.h"

class CTimer
{

public:

	struct State
	{
		double
			elapesed,
			totalElapsed;

		State
		(
			double aElapsed,
			double aTotal
		);
	};

	CTimer();
	bool initialize();
	bool update();
	bool finalize();
	
	
	
	State state() const
	{
		return State(elapsed(), totalElapsed());
	}

	inline double elapsed()      const
	{
		return mElapsed;
	}

	inline double totalElapsed() const
	{
		return mTotalElapsed;
	}

private:
	LARGE_INTEGER
		mFrequency,
		mFirstTimestamp,
		mPreviousTimestamp,
		mCurrentTimestamp;
	double
		mElapsed,
		mTotalElapsed;
};

