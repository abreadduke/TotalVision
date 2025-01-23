#include "visioner.hpp"

std::vector<HANDLE> getAllProcs(DWORD openFlags) {
	std::vector<HANDLE> procs;
	PROCESSENTRY32 peProcessEntry;
	HANDLE CONST hSnapshot = CreateToolhelp32Snapshot(
		TH32CS_SNAPPROCESS | TH32CS_SNAPALL, 0);
	if (INVALID_HANDLE_VALUE == hSnapshot) {
		throw std::runtime_error("Невозможно сделать снапшот процессов");
	}
	peProcessEntry.dwSize = sizeof(PROCESSENTRY32);
	Process32First(hSnapshot, &peProcessEntry);
	do {
		procs.push_back(OpenProcess(PROCESS_QUERY_INFORMATION | openFlags, true, peProcessEntry.th32ProcessID));
	} while (Process32Next(hSnapshot, &peProcessEntry));
	CloseHandle(hSnapshot);
	//DWORD* procsids = new DWORD[1024];
	//DWORD procssize;
	//EnumProcesses(procsids, 1024, &procssize);
	//for (int i = 0; i < procssize; i++) {
	//	procs.push_back(OpenProcess(PROCESS_QUERY_INFORMATION, true, procsids[i]));
	//}
	//delete[] procsids;
	return procs;
}
ProcessVisioner::ProcessVisioner() {}
void ProcessVisioner::makeSnapshot(DWORD openFlags) {
	procs = getAllProcs(openFlags);
}
void ProcessVisioner::closeProcs() {
	for (HANDLE process : this->procs) {
		if (process != NULL && GetProcessId(process) != 0)
			CloseHandle(process);
	}
	this->procs.clear();
}
std::vector<HANDLE> ProcessVisioner::getProcs() const{
	return this->procs;
}