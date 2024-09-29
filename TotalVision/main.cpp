#include <windows.h>
#include <iostream>
#include <vector>
#include <tlhelp32.h>
#include <psapi.h>
#include <cmath>
#include <regex>
#include <string>
#include <thread>
#include <mutex>
#include <map>
#include <conio.h>

#define BYTENEXTDIS 1024

std::vector<HANDLE> getAllProcs();
class ProcessVisioner {
public:
	ProcessVisioner() {}
	void makeSnapshot() {
		procs = getAllProcs();
	}
	void closeProcs() {
		for (HANDLE process : this->procs) {
			if(process != NULL && GetProcessId(process) != 0)
				CloseHandle(process);
		}
		this->procs.clear();
	}
	std::vector<HANDLE> getProcs() {
		return this->procs;
	}
private:
	std::vector<HANDLE> procs;
};
class ProcessAnalyzer {
public:
	ProcessAnalyzer(ProcessVisioner& visioner)
	{
		this->visioner = visioner;
	}
	void MakeStatementSnapshot() {
		this->processTimes.clear();
		unsigned long long time = 0;
		for (HANDLE process : visioner.getProcs()) {
			if (QueryProcessCycleTime(process, &time) != 0) {
				processTimes.insert(std::make_pair<DWORD, LONG64>(GetProcessId(process), time));
			}
		}
	}
	std::map<DWORD, LONG64> AnalyzeTimes() {
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
private:
	std::map<DWORD, LONG64> processTimes;
	ProcessVisioner visioner;
};
class CpuAnalyzer {
public:
	CpuAnalyzer() {};
	void MakeStatementSnapshot() {
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
	void AnalyzeCpuUsage() {
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
	unsigned long long GetIdle() {
		return this->cpuidletimes;
	}
	unsigned long long GetKernel() {
		return this->cpukerneltimes;
	}
	unsigned long long GetUser() {
		return this->cpuusertimes;
	}
private:
	unsigned long long cpukerneltimes;
	unsigned long long cpuusertimes;
	unsigned long long cpuidletimes;
};
std::vector<HANDLE> getAllProcs() {
	std::vector<HANDLE> procs;
	//PROCESSENTRY32 peProcessEntry;
	//HANDLE CONST hSnapshot = CreateToolhelp32Snapshot(
	//	TH32CS_SNAPPROCESS | TH32CS_SNAPALL, 0);
	//if (INVALID_HANDLE_VALUE == hSnapshot) {
	//	throw std::runtime_error("Невозможно сделать снапшот процессов");
	//}
	//
	//peProcessEntry.dwSize = sizeof(PROCESSENTRY32);
	//Process32First(hSnapshot, &peProcessEntry);
	//do {
	//	procs.push_back(OpenProcess(PROCESS_QUERY_INFORMATION, true, peProcessEntry.th32ProcessID));
	//} while (Process32Next(hSnapshot, &peProcessEntry));
	//CloseHandle(hSnapshot);
	DWORD* procsids = new DWORD[1024];
	DWORD procssize;
	EnumProcesses(procsids, 1024, &procssize);
	for (int i = 0; i < procssize; i++) {
		procs.push_back(OpenProcess(PROCESS_QUERY_INFORMATION, true, procsids[i]));
	}
	delete[] procsids;
	return procs;
}
class ProcessPrinter {
public:
	ProcessPrinter() {};
	std::string printHeaders(int memoryindent) {
		std::string buffer;
		buffer = "PROCESS";
		if (memoryindent >= (sizeof("PROCESS") / sizeof(char) - 1)) {
			for (int i = 0; i < (memoryindent - (sizeof("PROCESS") / sizeof(char) - 1)); i++)
				buffer += ' ';
			for (int i = 0; i < this->indents; i++)
				buffer += '\t';
			buffer += "Memory\n";
		}
		return buffer;
	}
	std::string printProcess(ProcessVisioner &visioner, std::string *staticinfo = nullptr) {
		ProcessAnalyzer analyzer(visioner);
		CpuAnalyzer cpuanalyzer;
		cpuanalyzer.MakeStatementSnapshot();
		analyzer.MakeStatementSnapshot();
		std::this_thread::sleep_for(std::chrono::milliseconds(160));
		auto analyzedtimes = analyzer.AnalyzeTimes();
		cpuanalyzer.AnalyzeCpuUsage();
		unsigned long long cpuusagetimes = cpuanalyzer.GetKernel() + cpuanalyzer.GetUser();
		std::string buffer;
		//find max file path length to padding the parameters
		char filename[filenamelength];
		unsigned short maxfilename = 0;
		for (HANDLE process : visioner.getProcs()) {
			char filename[filenamelength];
			if (GetProcessImageFileNameA(process, filename, sizeof(filename))) {
				maxfilename = (unsigned short)std::fmaxf((float)std::regex_replace(filename, std::regex("\\\\Device\\\\HarddiskVolume3"), "C:").size(), (float)maxfilename);
			}
		}
		unsigned long long allprocstimes = 0;
		for (auto analyzedtime : analyzedtimes) {
			allprocstimes += analyzedtime.second;
		}
		//std::cout << maxfilename;
		buffer = printHeaders(maxfilename);
		double allcpu = 0;
		for (HANDLE process : visioner.getProcs()) {
			if (GetProcessImageFileNameA(process, filename, sizeof(filename))) {
				std::string rfilename = std::regex_replace(filename, std::regex("\\\\Device\\\\HarddiskVolume3"), "C:");
				buffer += rfilename;
				for (int i = 0; i < maxfilename - rfilename.size(); i++)
					buffer += ' ';
				for (int i = 0; i < this->indents; i++)
					buffer += '\t';
				PROCESS_MEMORY_COUNTERS_EX memorycounters = { 0 };
				memorycounters.cb = sizeof(memorycounters);
				K32GetProcessMemoryInfo(process, (PROCESS_MEMORY_COUNTERS*)&memorycounters, memorycounters.cb);
				buffer += std::to_string(memorycounters.WorkingSetSize / std::pow(BYTENEXTDIS, 2)) + "MB";
				for (int i = 0; i < this->indents; i++)
					buffer += '\t';
				//get system time for define cpu usage
				auto analyzedtimesfound = analyzedtimes.find(GetProcessId(process));
				unsigned long long calculatedpubliccputimes = allprocstimes + cpuanalyzer.GetIdle() * 100;
				if (analyzedtimesfound != analyzedtimes.end() && calculatedpubliccputimes != 0)
					buffer += std::to_string(((double)analyzedtimesfound->second / calculatedpubliccputimes)
						 * 100) + "%\n";
				else buffer += "0%\n";
				allcpu += ((double)analyzedtimesfound->second / calculatedpubliccputimes) * 100;
			}
		}
		//std::cout << allcpu << '%' << std::endl;
		if (staticinfo != nullptr)
			for (int i = 0; i < maxfilename; i++)
				*staticinfo += '=';
			*staticinfo += "\nGlobal CPU usage: " + std::to_string(allcpu) + "%\n";
		return buffer;
	}
	void setIndents(int indents){
		this->indents = indents;
	}
private:
	static const unsigned short filenamelength = 1024;
	unsigned short indents = 1;
};
class ConsoleUI {
public:
	ConsoleUI(){
		thisconsole = GetStdHandle(STD_OUTPUT_HANDLE);
	}
	void SetOutputPrinter(const ProcessPrinter &printer) {
		this->printer = printer;
	}
	void SetVisioner(ProcessVisioner &visioner) {
		this->visioner = &visioner;
	}
	void MakeCursorThread() {
		std::thread cursorthread(&CursorScrolling, &cursormutex, &printercursor);
		cursorthread.detach();
	}
	void DrawUI() {
		COORD cursorpos = { 0, 0 };
		size_t nextlineaddress = 0;
		int printerlimit = 30;
		SetConsoleCursorPosition(thisconsole, cursorpos);
		std::string staticinfo;
		std::string printedinfo = printer.printProcess(*visioner, &staticinfo);
		int scrolledpath = 0;
		int printedlimitcounter = 0;
		size_t eofdynamicpinfo = 0;
		//handle information
		cursormutex.lock();
		do {
			for (size_t i = 0; i < printedinfo.length(); i++) {
				if (printedinfo[i] == '\n')
					scrolledpath++;
				if (scrolledpath == printercursor) { nextlineaddress = i; break; }
			}
			for (size_t i = nextlineaddress; i < printedinfo.length(); i++) {
				if (printedinfo[i] == '\n')
					printedlimitcounter++;
				if (printedlimitcounter == printerlimit ) { eofdynamicpinfo = i - nextlineaddress + 1; break; }
			}
			if (printedlimitcounter != printerlimit) {
				printercursor--;
				int scrolledpath = 0;
				int printedlimitcounter = 0;
				size_t nextlineaddress = 0;
				size_t eofdynamicpinfo = 0;
			}
		} while (nextlineaddress == printedinfo.length() - 1);
		cursormutex.unlock();
		nextlineaddress = nextlineaddress > 0 ? nextlineaddress : 0;
		//write out to the console buffer
		WriteConsoleA(thisconsole, printedinfo.c_str() + nextlineaddress, eofdynamicpinfo * sizeof(char), NULL, NULL);
		WriteConsoleA(thisconsole, staticinfo.c_str(), staticinfo.length() * sizeof(char), NULL, NULL);
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		system("cls");
	}
private:
	static void CursorScrolling(std::mutex *m, int *printercursor) {
		while (true) {
			char sym = _getch();
			switch (sym) {
			case 's':
				m->lock();
				(*printercursor)++;
				m->unlock();
				continue;
			case 'w':
				m->lock();
				(*printercursor)--;
				m->unlock();
				continue;
			}
		}
	}
	std::mutex cursormutex;
	ProcessPrinter printer;
	ProcessVisioner *visioner;
	HANDLE thisconsole;
	int printercursor = 0;
};
int main() {
	setlocale(LC_ALL, "ru");
	ProcessVisioner visioner;
	ProcessPrinter printer;
	ConsoleUI ui;
	//HANDLE thisconsole = GetStdHandle(STD_OUTPUT_HANDLE);
	ui.SetOutputPrinter(printer);
	ui.SetVisioner(visioner);
	ui.MakeCursorThread();
	while (true) {
		try {
			visioner.makeSnapshot();
		}
		catch (std::runtime_error& error) {
			std::cout << error.what() << std::endl;
			return -1;
		}
		ui.DrawUI();
		visioner.closeProcs();
		//clear buffer
		//size_t printedinfosize = printedinfo.length();
		//for (size_t i = 0; i < printedinfosize; i++) {
		//	if (printedinfo[i] == '\t') {
		//		for (int j = 0; j < 8; j++)
		//			printedinfo[i + j] = ' ';
		//		i += 8;
		//	}
		//	else printedinfo[i] = ' ';
		//}
		//CONSOLE_SCREEN_BUFFER_INFO cbi;
		//bool result = GetConsoleScreenBufferInfo(thisconsole, &cbi);
		//COORD bufferedcursorpos = cbi.dwCursorPosition;
		//SetConsoleCursorPosition(thisconsole, cursorpos);
		//WriteConsoleA(thisconsole, printedinfo.c_str(), printedinfo.length() * sizeof(char), NULL, NULL);
		//SetConsoleCursorPosition(thisconsole, bufferedcursorpos);
	}
	return 0;
}