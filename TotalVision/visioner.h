#pragma once
#include <windows.h>
#include <vector>
#include <psapi.h>
#include <tlhelp32.h>
#include <iostream>
std::vector<HANDLE> getAllProcs();
class ProcessVisioner {
public:
	ProcessVisioner();
	void makeSnapshot();
	void closeProcs();
	std::vector<HANDLE> getProcs();
private:
	std::vector<HANDLE> procs;
};