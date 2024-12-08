#include "distribute.hpp"

ThreadDistributor::ThreadDistributor() {}
bool ThreadDistributor::GetNewThread(std::thread*& t) {
	if (this->isThreadExist) return false;
	t = new std::thread;
	this->t = t;
	this->isThreadExist = true;
	return true;
}
const bool ThreadDistributor::isExist() const
{
	return this->isThreadExist;
}
bool ThreadDistributor::ClearThread() {
	if (!this->isThreadExist || !(this->t->joinable())) return false;
	this->isThreadExist = false;
	this->t->join();
	delete this->t;
	return true;
}
std::thread* ThreadDistributor::GetThread() {
	if (this->isThreadExist) return this->t;
	return nullptr;
}