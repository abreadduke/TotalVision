#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <ctime>
#include <string>
#include <fstream>
#include "datastorage.hpp"
#include "visioner.hpp"
#include "analyzer.hpp"
#include "consoleinterface.hpp"

#define FILE_OPENING_EXCEPTION_MESSAGE "File open error"
#define FINAL_EXEL_TABLE_PATH "result_table.xls"
class TimerStateSaver;
class TimerStateReader;
class ITimerAction {
public:
	virtual void Action() = 0;
};
class MakeSnapshotAnalyze : public ITimerAction {
public:
	MakeSnapshotAnalyze();
	virtual void Action() override;
	void SetAnalyzer(TimeAnalyzer* timeAnalyzer);
	void SetVisioner(ProcessVisioner* visioner);
	void SetSavingDirectory(std::string directory);
protected:
	std::string directory = ".";
	ProcessVisioner* visioner = nullptr;
	TimeAnalyzer* timeAnalyzer = nullptr;
};
class MakeXLSAnalyze : public MakeSnapshotAnalyze {
public:
	MakeXLSAnalyze();
	MakeXLSAnalyze(TimeAnalyzer* timeAnalyzer, ProcessVisioner* visioner, const std::string& directory);
	virtual void Action() override;
};
class ITimerType {
public:
	inline virtual const std::string GetType() const = 0;
};
#define UNDEFINED_TIMER_TYPE "UNDEFINED_TYPE"
#define TypeToString(type) #type
class UndefinedTimerType : public ITimerType{
public:
	inline virtual const std::string GetType() const override;
};
class AbstractSystemTimer : public UndefinedTimerType{
public:
	virtual void SetTimerRate(const tm& time) = 0;
	virtual void SetTimerRate(const time_t& time) = 0;
	virtual void DiscountOneSecondFromTimer() = 0;
	virtual void ActivateTimerAction() = 0;
	virtual void SetTimerAction(ITimerAction* timerAction) = 0;
	virtual ITimerAction* GetTimerAction() const = 0;
	virtual bool IsTimerCompleted() = 0;
	virtual void UseWriter(TimerStateSaver* saver) = 0;
	virtual void UseReader(TimerStateReader* reader) = 0;
	virtual void Continue() = 0;
	virtual time_t GetTimerRate() = 0;
};
class SystemTimer : public AbstractSystemTimer {
public:
	SystemTimer();
	virtual time_t GetTimerRate() override;
	virtual void SetTimerRate(const tm& time) override;
	virtual void SetTimerRate(const time_t& time) override;
	virtual void DiscountOneSecondFromTimer() override;
	virtual void ActivateTimerAction() override;
	virtual inline void SetTimerAction(ITimerAction* timerAction) override;
	virtual ITimerAction* GetTimerAction() const override;
	virtual inline bool IsTimerCompleted() override;
	virtual inline void Continue() override;
	virtual void UseWriter(TimerStateSaver* saver) override;
	virtual void UseReader(TimerStateReader* reader) override;
	inline virtual const std::string GetType() const override;
protected:
	bool timerCompleted = false;
	ITimerAction *actiovationAction = nullptr;
	time_t timeRate = 0;
	time_t timer = 0;
};
class YieldingSystemTimer : public SystemTimer {
public:
	YieldingSystemTimer();
	virtual inline void SetTimerRate(const tm& time) override;
	virtual inline void SetTimerRate(const time_t& time) override;
	virtual inline void SetActionCounts(const int counts);
	virtual inline int GetActionCounts() const;
	virtual void DiscountOneSecondFromTimer() override;
	virtual void UseWriter(TimerStateSaver* saver) override;
	virtual void UseReader(TimerStateReader* reader) override;
	inline virtual const std::string GetType() const override;
protected:
	int actionCounts = 0;
private:
	int actionsCompleted = 0;
};
class ITimerStateReader {
public:
	virtual AbstractSystemTimer* GetSystemTimer() = 0;
};
class ITimerStateSaver {
public:
	virtual void SaveTimer() = 0;
	virtual void SaveAsYieldingSystemTimer(YieldingSystemTimer* yieldingSystemTimer) = 0;
	virtual void SaveAsSystemTimer(SystemTimer* systemtimer) = 0;
};
class TimerStateSaver : public ITimerStateSaver {
public:
	TimerStateSaver(AbstractSystemTimer* &timer, std::string filepath);
	virtual void SaveTimer() override;
	virtual void SaveAsYieldingSystemTimer(YieldingSystemTimer* yieldingSystemTimer) override;
	virtual void SaveAsSystemTimer(SystemTimer* systemtimer) override;
protected:
	std::time_t timeRate = 3;
	int actionsCount = 0;
	std::string filepath = "";
	AbstractSystemTimer** timer = nullptr;
};
class TimerStateReader : public ITimerStateReader {
public:
	TimerStateReader(std::string filename);
	virtual AbstractSystemTimer* GetSystemTimer() override;
protected:
	std::string filepath = "";
};
class TimerTimeFileReader : public TimerStateReader {
public:
	TimerTimeFileReader(std::string filename);
	virtual AbstractSystemTimer* GetSystemTimer() override;
};
class TimerTimeFileSaver : public TimerStateSaver {
public:
	TimerTimeFileSaver(AbstractSystemTimer* &timer, std::string filepath);
	virtual void SaveTimer() override;
};