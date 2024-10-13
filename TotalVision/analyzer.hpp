#pragma once
#include <windows.h>
#include <vector>
#include <map>
#include <regex>
#include <string>
#include <tlhelp32.h>
#include <psapi.h>
#include <thread>
#include "visioner.hpp"
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
		DWORD processId;
		std::string processName;
		size_t processMemoryUsage;
		double processCPUPersents;
	};
	TimeAnalyzer();
	void Analyze(ProcessVisioner& visioner);
	void ClearResults();
	const std::vector<AnalyzedProcess> GetAnalyzed();
private:
	std::vector<AnalyzedProcess> analyzedProcs;
	CpuAnalyzer cpuanalyzer;
};