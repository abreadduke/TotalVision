#pragma once
#include <windows.h>
#include <vector>
#include <map>
#include <regex>
#include <string>
#include <tlhelp32.h>
#include <psapi.h>
#include "visioner.h"
class ProcessAnalyzer {
public:
	ProcessAnalyzer(ProcessVisioner& visioner);
	void MakeStatementSnapshot();
	std::map<DWORD, LONG64> AnalyzeTimes();
private:
	std::map<DWORD, LONG64> processTimes;
	ProcessVisioner visioner;
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
	TimeAnalyzer();
};