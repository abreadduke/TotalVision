#pragma once
#include <thread>
#include <mutex>
class ThreadDistributor {
public:
	ThreadDistributor();
	bool GetNewThread(std::thread*& t);
	bool ClearThread();
	const bool isExist() const;
	std::thread* GetThread();
	std::mutex* lockOtherThreads = nullptr;
private:
	bool isThreadExist = false;
	std::thread* t = nullptr;
};