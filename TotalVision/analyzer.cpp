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
				rfilename, processmemory, parcedtimes
			};
			//this->analyzedProcs.push_back(analyzedprocess);
			this->analyzedProcs[processid] = analyzedprocess;
		}
	}
}
void TimeAnalyzer::ClearResults() {
	analyzedProcs.clear();
}
const std::map<DWORD, TimeAnalyzer::AnalyzedProcess> TimeAnalyzer::GetAnalyzed() {
	return (const std::map<DWORD, TimeAnalyzer::AnalyzedProcess>)(this->analyzedProcs);
}
MidTimeAnalyzer::MidTimeAnalyzer() : TimeAnalyzer::TimeAnalyzer(){}
void MidTimeAnalyzer::Analyze(ProcessVisioner& visioner){
	for (unsigned short b = 0; b < snapshotsCount; b++) {
		hiddenAnalyzer.ClearResults();
		std::this_thread::sleep_for(std::chrono::milliseconds(analyzerOffset));
		hiddenAnalyzer.Analyze(visioner);
		while (analyzedProcessesBuffer.size() >= bufferSize) {
			analyzedProcessesBuffer.pop();
		}
		analyzedProcessesBuffer.push(hiddenAnalyzer.GetAnalyzed());
	}
	AnalyzeWithGivenBuffer(visioner, analyzedProcessesBuffer);
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
void MidTimeAnalyzer::Analyze(ProcessVisioner& visioner, std::vector<std::vector<TimeAnalyzer::AnalyzedProcess>>& buffer) {
	AnalyzeWithGivenBuffer(visioner, buffer);
}
void MidTimeAnalyzer::AnalyzeWithGivenBuffer(ProcessVisioner& visioner, std::queue<std::map<DWORD, AnalyzedProcess>>& buffer) {
	std::map<DWORD, TimeAnalyzer::AnalyzedProcess> recombinatedAnalyzedProcs;
	hiddenAnalyzer.ClearResults();
	std::this_thread::sleep_for(std::chrono::milliseconds(analyzerOffset));
	hiddenAnalyzer.Analyze(visioner);
	for (auto ap : hiddenAnalyzer.GetAnalyzed()) {
		bool hasNotNewAp = false;
		std::vector<TimeAnalyzer::AnalyzedProcess> procs;
		std::queue<std::map<DWORD, TimeAnalyzer::AnalyzedProcess>> bufferBackup;
		while (!buffer.empty()) {
			std::map<DWORD, TimeAnalyzer::AnalyzedProcess> analyzedSnapshot = buffer.front();
			buffer.pop();
			bufferBackup.push(analyzedSnapshot);
			auto processInfoIterator = analyzedSnapshot.find(ap.first);
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
		if (procsLength != 0) {
			recombinatedAnalyzedProcs[ap.first] = { ap.second.processName,
												processMemoryUsage / procsLength,
												processCPUPersents / procsLength };
		}
		buffer = bufferBackup;
	}
	analyzedProcs = recombinatedAnalyzedProcs;
}
void MidTimeAnalyzer::AnalyzeWithGivenBuffer(ProcessVisioner& visioner, std::vector<std::vector<TimeAnalyzer::AnalyzedProcess>>& buffer) {
	std::map<DWORD, TimeAnalyzer::AnalyzedProcess> combinedProcs;
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
		combinedProcs[processInfoForMidiation.first].processMemoryUsage /= countProcessesInBuffer[processInfoForMidiation.first];
		combinedProcs[processInfoForMidiation.first].processCPUPersents /= countProcessesInBuffer[processInfoForMidiation.first];
	}
	analyzedProcs = combinedProcs;
}