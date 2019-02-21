#include "../../Include/Base/Timer.h"

CTimer::State::State(double aElapsed, double aTotal) : elapesed(aElapsed), totalElapsed(aTotal) {}

CTimer::CTimer() : mFrequency({}), mFirstTimestamp({}), mPreviousTimestamp({}), mCurrentTimestamp({}), mElapsed(0.0), mTotalElapsed(0.0){}

bool CTimer::initialize()
{
	bool const frequencyFetchOk = QueryPerformanceFrequency(&mFrequency);
	if (!frequencyFetchOk){return false; }

	bool const fetchCountOk = QueryPerformanceCounter(&mFirstTimestamp);
	if (!fetchCountOk){ return false; }

	mCurrentTimestamp = mFirstTimestamp;

	return true;
}

bool CTimer::update()
{
	mPreviousTimestamp = mCurrentTimestamp;

	bool const fetchCountOk = QueryPerformanceCounter(&mCurrentTimestamp);

	if (!fetchCountOk){return false;}

	mElapsed = (((mCurrentTimestamp.QuadPart - mPreviousTimestamp.QuadPart)) / ((double)mFrequency.QuadPart));
	mTotalElapsed = (((mCurrentTimestamp.QuadPart - mFirstTimestamp.QuadPart)) / ((double)mFrequency.QuadPart));

	return true;
}

bool CTimer::finalize()
{
	return false;
}

