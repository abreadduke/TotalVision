#pragma once
#include <windows.h>
#include <vector>
#include <psapi.h>
#include <tlhelp32.h>
#include <iostream>
std::vector<HANDLE> getAllProcs(DWORD openFlags = 0);
class ProcessVisioner {
public:
	ProcessVisioner();
	void makeSnapshot(DWORD openFlags = 0);
	void closeProcs();
	std::vector<HANDLE> getProcs()const;
private:
	std::vector<HANDLE> procs;
};