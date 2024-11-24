#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <cmath>
#include <regex>
#include <string>
#include <thread>
#include <mutex>
#include <conio.h>
#include "visioner.hpp"
#include "analyzer.hpp"
#include <cmath>
#include <regex>
#include <string>
#include <thread>
#include <mutex>
#include <conio.h>
#include "visioner.hpp"
#include "analyzer.hpp"
#include "datastorage.hpp"
#include <filesystem>
#include <ctime>
#define BYTENEXTDIS 1024
#define TAB "    "

//#define TABENABLE

class ProcessPrinter {
public:
	ProcessPrinter();
	std::string printHeaders(int memoryindent);
	std::string printProcess(TimeAnalyzer& timeanalyzer, std::string* staticinfo = nullptr);
	void setIndents(int indents);
private:
	static const unsigned short filenamelength = 1024;
	unsigned short indents = 1;
};
class ConsoleUI {
public:
	ConsoleUI();
	void SetOutputPrinter(const ProcessPrinter& printer);
	void SetVisioner(ProcessVisioner& visioner);
	void MakeCursorThread();
	void DrawUI();
private:
	void CursorScrolling();
	std::mutex cursormutex;
	std::mutex timeAnalyzerMutex;
	ProcessPrinter printer;
	ProcessVisioner* visioner;
	HANDLE thisconsole;
	//TimeAnalyzer timeAnalyzer;
	MidTimeAnalyzer timeAnalyzer;
	int printercursor = 0;
};