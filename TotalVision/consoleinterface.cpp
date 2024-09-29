#include <cmath>
#include <regex>
#include <string>
#include <thread>
#include <mutex>
#include <conio.h>
#include "visioner.h"
#include "analyzer.h"
#include "consoleinterface.h"
#define BYTENEXTDIS 1024

ProcessPrinter::ProcessPrinter() {}
std::string ProcessPrinter::printProcess(ProcessVisioner& visioner, std::string* staticinfo) {
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
std::string ProcessPrinter::printHeaders(int memoryindent) {
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
void ProcessPrinter::setIndents(int indents) {
	this->indents = indents;
}
ConsoleUI::ConsoleUI() {
	thisconsole = GetStdHandle(STD_OUTPUT_HANDLE);
}
void ConsoleUI::SetOutputPrinter(const ProcessPrinter& printer) {
	this->printer = printer;
}
void ConsoleUI::SetVisioner(ProcessVisioner& visioner) {
	this->visioner = &visioner;
}
void ConsoleUI::MakeCursorThread() {
	std::thread cursorthread(&CursorScrolling, &cursormutex, &printercursor);
	cursorthread.detach();
}
void ConsoleUI::DrawUI() {
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
			if (printedlimitcounter == printerlimit) { eofdynamicpinfo = i - nextlineaddress + 1; break; }
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
void ConsoleUI::CursorScrolling(std::mutex* m, int* printercursor) {
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