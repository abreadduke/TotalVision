#pragma once
#include <windows.h>
#include <vector>
#include <map>
#include <regex>
#include <string>
#include <tlhelp32.h>
#include <psapi.h>
#include <thread>
#include <algorithm>
#include <queue>
#include <map>
#include "visioner.hpp"
#include "utils/path.hpp"

class ProcessAnalyzer {
public:
	ProcessAnalyzer(ProcessVisioner& visioner);
	void MakeStatementSnapshot();
	std::map<DWORD, size_t> AnalyzeMemory();
	std::map<DWORD, LONG64> AnalyzeTimes();
private:
	std::map<DWORD, LONG64> processTimes;
	std::map<DWORD, size_t> processMemory;
	ProcessVisioner *visioner;
};
class CpuAnalyzer {
public:
	CpuAnalyzer();
	void MakeStatementSnapshot();
	void AnalyzeCpuUsage();
	unsigned long long GetIdle();
	unsigned long long GetKernel();
	unsigned long long GetUser();
private:
	unsigned long long cpukerneltimes;
	unsigned long long cpuusertimes;
	unsigned long long cpuidletimes;
};
class TimeAnalyzer {
public:
	struct AnalyzedProcess {
		//DWORD processId;
		std::string processName;
		size_t processMemoryUsage;
		double processCPUPersents;
	};
	TimeAnalyzer();
	virtual void Analyze(ProcessVisioner& visioner);
	void ClearResults();
	const std::map<DWORD, TimeAnalyzer::AnalyzedProcess> GetAnalyzed();
private:
	CpuAnalyzer cpuanalyzer;
protected:
	std::map<DWORD, TimeAnalyzer::AnalyzedProcess> analyzedProcs;
};
class MidTimeAnalyzer : public TimeAnalyzer {
public:
	MidTimeAnalyzer();
	virtual void Analyze(ProcessVisioner& visioner) override;
	virtual void Analyze(ProcessVisioner& visioner, std::vector<std::vector<TimeAnalyzer::AnalyzedProcess>>& buffer);
	void SetBufferSize(unsigned short bufferSize);
	unsigned short GetBufferSize();
	void SetAnalyzerOffset(size_t analyzerOffset);
	size_t GetAnalyzerOffset();
	void SetSnapshotsCount(unsigned short snapshotsCount);
	unsigned short GetSnapshotsCount();
protected:
	virtual void AnalyzeWithGivenBuffer(ProcessVisioner& visioner, std::queue<std::map<DWORD, TimeAnalyzer::AnalyzedProcess>>& buffer);
	virtual void AnalyzeWithGivenBuffer(ProcessVisioner& visioner, std::vector<std::vector<TimeAnalyzer::AnalyzedProcess>>& buffer);
private:
	TimeAnalyzer hiddenAnalyzer;
	size_t analyzerOffset = 10;
	unsigned short bufferSize = 10;
	//must be more than 1
	unsigned short snapshotsCount = 3;
	std::queue<std::map<DWORD, TimeAnalyzer::AnalyzedProcess>> analyzedProcessesBuffer;
};