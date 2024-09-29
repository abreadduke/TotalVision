#include "analyzer.h"
ProcessAnalyzer::ProcessAnalyzer(ProcessVisioner& visioner) {
	this->visioner = visioner;
}
void ProcessAnalyzer::MakeStatementSnapshot() {
	this->processTimes.clear();
	unsigned long long time = 0;
	for (HANDLE process : visioner.getProcs()) {
		if (QueryProcessCycleTime(process, &time) != 0) {
			processTimes.insert(std::make_pair<DWORD, LONG64>(GetProcessId(process), time));
		}
	}
}
std::map<DWORD, LONG64> ProcessAnalyzer::AnalyzeTimes() {
	unsigned long long time = 0;
	for (HANDLE process : visioner.getProcs()) {
		if (QueryProcessCycleTime(process, &time) != 0) {
			auto procsfound = processTimes.find(GetProcessId(process));
			if (procsfound != processTimes.end()) {
				procsfound->second = time - procsfound->second;
			}
		}
	}
	return this->processTimes;
}
CpuAnalyzer::CpuAnalyzer() {}
void CpuAnalyzer::MakeStatementSnapshot() {
	FILETIME cpukernel = { 0 };
	FILETIME cpuuser = { 0 };
	FILETIME cpuidle = { 0 };
	GetSystemTimes(&cpuidle, &cpukernel, &cpuuser);
	//this->cpuidletimes = cpuidle.dwLowDateTime + (cpuidle.dwHighDateTime << (sizeof(DWORD) * 8));
	//this->cpukerneltimes = cpukernel.dwLowDateTime + (cpukernel.dwHighDateTime << (sizeof(DWORD) * 8));
	//this->cpuusertimes = cpuuser.dwLowDateTime + (cpuuser.dwHighDateTime << (sizeof(DWORD) * 8));
	this->cpuidletimes = cpuidle.dwLowDateTime;
	this->cpukerneltimes = cpukernel.dwLowDateTime;
	this->cpuusertimes = cpuuser.dwLowDateTime;
}
void CpuAnalyzer::AnalyzeCpuUsage() {
	FILETIME cpukernel = { 0 };
	FILETIME cpuuser = { 0 };
	FILETIME cpuidle = { 0 };
	GetSystemTimes(&cpuidle, &cpukernel, &cpuuser);
	//this->cpuidletimes = cpuidle.dwLowDateTime + (cpuidle.dwHighDateTime << (sizeof(DWORD) * 8)) - this->cpuidletimes;
	//this->cpukerneltimes = cpukernel.dwLowDateTime + (cpukernel.dwHighDateTime << (sizeof(DWORD) * 8)) - this->cpukerneltimes;
	//this->cpuusertimes = cpuuser.dwLowDateTime + (cpuuser.dwHighDateTime << (sizeof(DWORD) * 8)) - this->cpuusertimes;
	this->cpuidletimes = cpuidle.dwLowDateTime - this->cpuidletimes;
	this->cpukerneltimes = cpukernel.dwLowDateTime - this->cpukerneltimes;
	this->cpuusertimes = cpuuser.dwLowDateTime - this->cpuusertimes;
}
unsigned long long CpuAnalyzer::GetIdle() {
	return this->cpuidletimes;
}
unsigned long long CpuAnalyzer::GetKernel() {
	return this->cpukerneltimes;
}
unsigned long long CpuAnalyzer::GetUser() {
	return this->cpuusertimes;
}
TimeAnalyzer::TimeAnalyzer() {}