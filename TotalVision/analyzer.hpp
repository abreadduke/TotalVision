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
#include <filesystem>
#include "defined_paths.h"

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
//class TimeAnalyzerAdapter : TimeAnalyzer {
//public:
//	TimeAnalyzerAdapter(std::map<DWORD, TimeAnalyzer::AnalyzedProcess> analyzedProcs);
//};
struct TimeAnalyzedProcess : TimeAnalyzer::AnalyzedProcess {
	TimeAnalyzedProcess();
	TimeAnalyzedProcess(const TimeAnalyzer::AnalyzedProcess& ap);
	time_t time = 0;
};
template<class AnalyzedFormat>
class ProcessInterfaceBuilder {
public:
	virtual void Build(std::map<DWORD, AnalyzedFormat>& procs, const DWORD& processId, std::map<DWORD, int>& fixedCounts) = 0;
};
template<class AnalyzedFormat>
class AnalyzerProcessInterfaceBuilder : public ProcessInterfaceBuilder<AnalyzedFormat>{
public:
	AnalyzerProcessInterfaceBuilder(){}
	virtual void Build(std::map<DWORD, AnalyzedFormat>& procs, const DWORD& processId, std::map<DWORD, int>& fixedCounts) override {
		procs[processId].processMemoryUsage /= fixedCounts[processId];
		procs[processId].processCPUPersents /= fixedCounts[processId];
	}
};
class TimeProcessInterfaceBuilder : public AnalyzerProcessInterfaceBuilder<TimeAnalyzedProcess> {
public:
	TimeProcessInterfaceBuilder();
	virtual void SetTimerRate(time_t rate);
	virtual void Build(std::map<DWORD, TimeAnalyzedProcess>& procs, const DWORD& processId, std::map<DWORD, int>& fixedCounts) override;
private:
	time_t rate = 0;
};
template<class AnalyzedFormat>
std::map<DWORD, AnalyzedFormat> MediateProcesses(std::vector<std::vector<AnalyzedFormat>>& buffer, ProcessInterfaceBuilder<AnalyzedFormat>* ib) {
	std::map<DWORD, AnalyzedFormat> combinedProcs;
	std::map<DWORD, int> countProcessesInBuffer;
	std::hash<std::string> stringHash;
	for (int snapshotIndex = 0; snapshotIndex < buffer.size(); snapshotIndex++) {
		for (auto process : buffer[snapshotIndex]) {
			DWORD hashcode = stringHash(process.processName);
			if (combinedProcs.find(hashcode) == combinedProcs.end()) {
				combinedProcs[hashcode] = process;
				countProcessesInBuffer[hashcode] = 0;
			}
			else {
				combinedProcs[hashcode].processMemoryUsage += process.processMemoryUsage;
				combinedProcs[hashcode].processCPUPersents += process.processCPUPersents;
			}
			countProcessesInBuffer[hashcode]++;
		}
	}
	for (auto processInfoForMidiation : combinedProcs) {
		//----
		//processInfoForMidiation.second.processMemoryUsage /= countProcessesInBuffer[processInfoForMidiation.first];
		//processInfoForMidiation.second.processCPUPersents /= countProcessesInBuffer[processInfoForMidiation.first];
		ib->Build(combinedProcs, processInfoForMidiation.first, countProcessesInBuffer);
	}
	return combinedProcs;
}
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