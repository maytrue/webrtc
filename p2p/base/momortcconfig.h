#ifndef MOMO_RTC_CONFIG_H_
#define MOMO_RTC_CONFIG_H_

#include <thread>

class MomoRTCConfig
{
public:

	using UniqueLock = std::unique_lock<std::mutex>;

	MomoRTCConfig();

	~MomoRTCConfig();

	static MomoRTCConfig *getInstance();

	int getStunKeepAliveDelay();

	void setStunKeepAliveDelay(int stunKeepAliveDelay);

	int getStunRetryTimeout();

	void setStunRetryTimeout(int stunRetryTimeout);

private:
	std::mutex mMutex;

	int mStunKeepAliveDelay;

	int mStunRetryTimeout;
};

#endif
