#pragma once
#include <cmath>
#include <regex>
#include <string>
#include <thread>
#include <mutex>
#include <conio.h>
#include "visioner.h"
#include "analyzer.h"
#define BYTENEXTDIS 1024

class ProcessPrinter {
public:
	ProcessPrinter();
	std::string printHeaders(int memoryindent);
	std::string printProcess(ProcessVisioner& visioner, std::string* staticinfo = nullptr);
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
	static void CursorScrolling(std::mutex* m, int* printercursor);
	std::mutex cursormutex;
	ProcessPrinter printer;
	ProcessVisioner* visioner;
	HANDLE thisconsole;
	int printercursor = 0;
};