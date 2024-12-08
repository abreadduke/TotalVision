#pragma once
#include <thread>
class ThreadDistributor {
public:
	ThreadDistributor();
	bool GetNewThread(std::thread*& t);
	bool ClearThread();
	const bool isExist() const;
	std::thread* GetThread();
private:
	bool isThreadExist = false;
	std::thread* t = nullptr;
};