
#include "p2p/base/momortcconfig.h"
#include "p2p/base/momosingleton.h"

MomoRTCConfig* MomoRTCConfig::getInstance()
{
	return MomoSingleton<MomoRTCConfig>::getInstance(); 
}

MomoRTCConfig::MomoRTCConfig()
{
	mStunKeepAliveDelay = 60 * 1000;
	mStunRetryTimeout = 120 * 1000; 
}

MomoRTCConfig::~MomoRTCConfig()
{

}

int MomoRTCConfig::getStunKeepAliveDelay()
{
	UniqueLock lock(mMutex);
	return mStunKeepAliveDelay;
}

void MomoRTCConfig::setStunKeepAliveDelay(int stunKeepAliveDelay)
{
	UniqueLock lock(mMutex);
	mStunKeepAliveDelay = stunKeepAliveDelay;
}

int MomoRTCConfig::getStunRetryTimeout()
{
	UniqueLock lock(mMutex);
	return mStunRetryTimeout;
}

void MomoRTCConfig::setStunRetryTimeout(int stunRetryTimeout)
{
	UniqueLock lock(mMutex);
	mStunRetryTimeout = stunRetryTimeout;
}

