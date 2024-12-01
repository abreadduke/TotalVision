#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <ctime>
#include <string>
#include <fstream>
#include "datastorage.hpp"
#include "visioner.hpp"
#include "analyzer.hpp"

#define FILE_OPENING_EXCEPTION_MESSAGE "File open error"

class ITimerAction {
public:
	virtual void Action() = 0;
};
class MakeSnapshotAnalyze : public ITimerAction {
public:
	MakeSnapshotAnalyze();
	virtual void Action() override;
	void SetAnalyzer(TimeAnalyzer* timeAnalyzer) {
		this->timeAnalyzer = timeAnalyzer;
	}
	void SetSavingDirectory(std::string directory);
private:
	std::string directory = ".";
	TimeAnalyzer* timeAnalyzer = nullptr;
};
class ISystemTimer {
public:
	virtual void SetTimerRate(tm& time) = 0;
	virtual void SetTimerRate(time_t& time) = 0;
	virtual void DiscountOneSecondFromTimer() = 0;
	virtual void ActivateTimerAction() = 0;
	virtual void SetTimerAction(ITimerAction* timerAction) = 0;
	virtual time_t GetTimerRate() = 0;
};
class SystemTimer : public ISystemTimer {
public:
	SystemTimer();
	virtual time_t GetTimerRate() override;
	virtual void SetTimerRate(tm& time) override;
	virtual void SetTimerRate(time_t& time) override;
	virtual void DiscountOneSecondFromTimer() override;
	virtual void ActivateTimerAction() override;
	virtual void SetTimerAction(ITimerAction* timerAction) override;
private:
	ITimerAction *actiovationAction = nullptr;
	time_t timeRate = 0;
	time_t timer = 0;
};
class ITimerStateReader {
public:
	virtual ISystemTimer* GetSystemTimer() = 0;
};
class ITimerStateSaver {
public:
	virtual void SaveTimer() = 0;
};
class TimerStateSaver : public ITimerStateSaver {
public:
	TimerStateSaver(ISystemTimer* timer, std::string filepath);
	virtual void SaveTimer() override;
private:
	std::string filepath;
	ISystemTimer* timer;
};
class TimerStateReader : public ITimerStateReader {
public:
	TimerStateReader(std::string filename);
	virtual ISystemTimer* GetSystemTimer() override;
private:
	std::string filepath;
};