#include "analyzer.hpp"

std::map<DWORD, size_t> ProcessAnalyzer::AnalyzeMemory() {
	std::map<DWORD, size_t> analyzedmemory;
	for (HANDLE process : visioner->getProcs()) {
		PROCESS_MEMORY_COUNTERS_EX memorycounters = { 0 };
		memorycounters.cb = sizeof(memorycounters);
		K32GetProcessMemoryInfo(process, (PROCESS_MEMORY_COUNTERS*)&memorycounters, memorycounters.cb);
		if (DWORD processid = GetProcessId(process)) {
			analyzedmemory.insert(std::make_pair(processid, memorycounters.WorkingSetSize / std::pow(1024, 2)));
		}
	}
	return analyzedmemory;
}
ProcessAnalyzer::ProcessAnalyzer(ProcessVisioner& visioner) {
	this->visioner = &visioner;
}
void ProcessAnalyzer::MakeStatementSnapshot() {
	this->processTimes.clear();
	unsigned long long time = 0;
	for (HANDLE process : visioner->getProcs()) {
		if (QueryProcessCycleTime(process, &time) != 0) {
			processTimes.insert(std::make_pair<DWORD, LONG64>(GetProcessId(process), time));
		}
	}
}
std::map<DWORD, LONG64> ProcessAnalyzer::AnalyzeTimes() {
	unsigned long long time = 0;
	for (HANDLE process : visioner->getProcs()) {
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
	this->cpuidletimes = cpuidle.dwLowDateTime;
	this->cpukerneltimes = cpukernel.dwLowDateTime;
	this->cpuusertimes = cpuuser.dwLowDateTime;
}
void CpuAnalyzer::AnalyzeCpuUsage() {
	FILETIME cpukernel = { 0 };
	FILETIME cpuuser = { 0 };
	FILETIME cpuidle = { 0 };
	GetSystemTimes(&cpuidle, &cpukernel, &cpuuser);
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
void TimeAnalyzer::Analyze(ProcessVisioner &visioner){
	ProcessAnalyzer panalyzer(visioner);
	cpuanalyzer.MakeStatementSnapshot();
	panalyzer.MakeStatementSnapshot();
	std::this_thread::sleep_for(std::chrono::milliseconds(160));
	auto times = panalyzer.AnalyzeTimes();
	auto memory = panalyzer.AnalyzeMemory();
	cpuanalyzer.AnalyzeCpuUsage();
	ULONG64 cpuidle = cpuanalyzer.GetIdle();
	auto procs = visioner.getProcs();
	unsigned long long allprocstimes = 0;
	for (auto analyzedtime : times) {
		allprocstimes += analyzedtime.second;
	}
	for (HANDLE process : procs) {
		char filename[1024];
		char disks[] = { 'C', 'D', 'E', 'F' };
		if (GetProcessImageFileNameA(process, filename, sizeof(filename))) {
			DWORD processid = GetProcessId(process);
			auto itertimes = times.find(processid);
			LONG64 processtimes = 0;
			if (itertimes != times.end())
				processtimes = itertimes->second;
			auto itermemory = memory.find(processid);
			size_t processmemory = 0;
			if(itermemory != memory.end())
				processmemory = itermemory->second;
			double parcedtimes = 0;
			if (allprocstimes != 0)
				parcedtimes = ((double)processtimes / (double)(allprocstimes + cpuidle * 100)) * 100;
			std::string rfilename = "";
			for (int diskname = 3; diskname < sizeof(disks) / sizeof(char); diskname++) {
				rfilename = std::regex_replace(filename, std::regex(std::string("\\\\Device\\\\HarddiskVolume") + std::to_string(diskname)), disks[diskname - 3] + std::string(":"));
				if (rfilename != "") break;
			}
			TimeAnalyzer::AnalyzedProcess analyzedprocess = {
				processid, rfilename, processmemory, parcedtimes
			};
			this->analyzedProcs.push_back(analyzedprocess);
		}
	}
}
void TimeAnalyzer::ClearResults() {
	analyzedProcs.clear();
}
const std::vector<TimeAnalyzer::AnalyzedProcess> TimeAnalyzer::GetAnalyzed() {
	return (const std::vector<TimeAnalyzer::AnalyzedProcess>)(this->analyzedProcs);
}
MidTimeAnalyzer::MidTimeAnalyzer() : TimeAnalyzer::TimeAnalyzer(){}
void MidTimeAnalyzer::Analyze(ProcessVisioner& visioner){
	//ClearResults();
	//if (analyzedProcs.size() == 0) {
	//	TimeAnalyzer::Analyze(visioner);
	//}
	TimeAnalyzer hiddenAnalyzer;
	//hiddenAnalyzer.Analyze(visioner);
	//std::vector<TimeAnalyzer::AnalyzedProcess> newAnalyzed = hiddenAnalyzer.GetAnalyzed();
	std::vector<TimeAnalyzer::AnalyzedProcess> recombinatedAnalyzedProcs;
	//for (auto newAp : newAnalyzed) {
	//	bool hasNotNewAp = true;
	//	for (auto oldAp : analyzedProcs) {
	//		if (newAp.processId == oldAp.processId) {
	//			recombinatedAnalyzedProcs.push_back({newAp.processId, newAp.processName, 
	//				(newAp.processMemoryUsage + oldAp.processMemoryUsage) / 2,
	//				(newAp.processCPUPersents + oldAp.processCPUPersents) / 2});
	//			hasNotNewAp = false;
	//			break;
	//		}
	//	}
	//	if(hasNotNewAp)
	//		recombinatedAnalyzedProcs.push_back(newAp);
	//}
	for (unsigned short b = 0; b < snapshotsCount; b++) {
		hiddenAnalyzer.ClearResults();
		std::this_thread::sleep_for(std::chrono::milliseconds(analyzerOffset));
		hiddenAnalyzer.Analyze(visioner);
		while (analyzedProcessesBuffer.size() >= bufferSize) {
			analyzedProcessesBuffer.pop();
		}
		std::map<DWORD, TimeAnalyzer::AnalyzedProcess> mappedAnalyzedProcs;
		for (auto ap : hiddenAnalyzer.GetAnalyzed()) {
			mappedAnalyzedProcs.insert(std::make_pair(ap.processId, ap));
		}
		analyzedProcessesBuffer.push(mappedAnalyzedProcs);
	}
	hiddenAnalyzer.ClearResults();
	std::this_thread::sleep_for(std::chrono::milliseconds(analyzerOffset));
	hiddenAnalyzer.Analyze(visioner);
	for (auto ap : hiddenAnalyzer.GetAnalyzed()) {
		bool hasNotNewAp = false;
		std::vector<TimeAnalyzer::AnalyzedProcess> procs;
		std::queue<std::map<DWORD, TimeAnalyzer::AnalyzedProcess>> bufferBackup;
		while (!analyzedProcessesBuffer.empty()) {
			std::map<DWORD, TimeAnalyzer::AnalyzedProcess> analyzedSnapshot = analyzedProcessesBuffer.front();
			analyzedProcessesBuffer.pop();
			bufferBackup.push(analyzedSnapshot);
			auto processInfoIterator = analyzedSnapshot.find(ap.processId);
			if (processInfoIterator != analyzedSnapshot.end()) {
				procs.push_back(processInfoIterator->second);
			}
		}
		size_t processMemoryUsage = 0;
		double processCPUPersents = 0;
		for (auto apFromProcs : procs) {
			processMemoryUsage += apFromProcs.processMemoryUsage;
			processCPUPersents += apFromProcs.processCPUPersents;
		}
		size_t procsLength = procs.size();
		recombinatedAnalyzedProcs.push_back({ap.processId, ap.processName,
											processMemoryUsage / procsLength,
											processCPUPersents / procsLength});
		analyzedProcessesBuffer = bufferBackup;
	}
	analyzedProcs = recombinatedAnalyzedProcs;
}
void MidTimeAnalyzer::SetBufferSize(unsigned short bufferSize) {
	this->bufferSize = bufferSize;
}
unsigned short MidTimeAnalyzer::GetBufferSize() {
	return this->bufferSize;
}
void MidTimeAnalyzer::SetAnalyzerOffset(size_t analyzerOffset) {
	this->analyzerOffset = analyzerOffset;
}
size_t MidTimeAnalyzer::GetAnalyzerOffset() {
	return this->analyzerOffset;
}
void MidTimeAnalyzer::SetSnapshotsCount(unsigned short snapshotsCount) {
	this->snapshotsCount = snapshotsCount;
}
unsigned short MidTimeAnalyzer::GetSnapshotsCount() {
	return this->snapshotsCount;
}