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
#include "distribute.hpp"
#include "defined_paths.h"
#include "utils/path.hpp"
#include "resource.h"
#define BYTENEXTDIS 1024
#ifdef TABENABLE
#define TAB '\t'
#else
#define TAB "    "
#endif 
#define complete_string_to_console(buffer)  buffer += std::string(10, ' ') + '\n'
#define PRINTER_LIMIT 30

//#define TABENABLE
typedef unsigned char digits;
digits getDigitsFromNumber(unsigned int number);
class ConsoleUI;
class IExecutableProcedure;
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
	void DrawUI();
	void LockDrawing();
	void UnlockDrawing();
	void ClearConsoleBuffer();
	const TimeAnalyzer* GetAnalyzer() const;
	ProcessVisioner* GetVisioner() const;
	std::mutex timeAnalyzerMutex;
	std::mutex consoleMutex;
	void AddKeyBindAction(const char key, IExecutableProcedure* action);
	~ConsoleUI();
protected:
	void MakeActionHandlerThread();
	void MakeProcessAnalyzingThread();
private:
	bool canDraw = true;
	bool forceDraw = false;
	bool running = true;
	std::thread actionHandling;
	std::thread processAnalyzingThread;
	std::map<const char, IExecutableProcedure*> keyBindActions;
	void ButtonActionsHandler();
	void ProcessAnalyzing();
	std::mutex cursormutex;
	ProcessPrinter printer;
	ProcessVisioner* visioner;
	HANDLE thisconsole;
	//TimeAnalyzer timeAnalyzer;
	MidTimeAnalyzer timeAnalyzer;
	int printercursor = 0;
};
class IExecutableProcedure {
public:
	virtual void Execute(ConsoleUI* consoleui) = 0;
};
class ThreadCloseProcedure : public IExecutableProcedure {
public:
	ThreadCloseProcedure();
	virtual void Execute(ConsoleUI* consoleui) override;
	virtual void SetThreadDistrubutor(ThreadDistributor* distributor);
protected:
	ThreadDistributor* distributor = nullptr;
};
class FinalAnalyzeProcedure : public IExecutableProcedure {
public:
	FinalAnalyzeProcedure();
	static void AnalyzeProcedure(ProcessVisioner& visioner, const std::string& filepath);
	virtual void Execute(ConsoleUI* consoleui) override;
protected:
	ProcessVisioner* visioner;
};
class MakeAnalyzedFile : public IExecutableProcedure {
public:
	virtual void SaveToFile(TimeAnalyzer& timeAnalyzer, DataStorage* dataStorage, std::string filename);
protected:
	std::string filename;
};
class KillProcessProcedure : public IExecutableProcedure {
public:
	KillProcessProcedure();
	virtual void Execute(ConsoleUI* consoleui) override;
};
class MakeBinaryAnalyzedFile : public MakeAnalyzedFile {
public:
	MakeBinaryAnalyzedFile();
	virtual void Execute(ConsoleUI* consoleui) override;
};
class MakeXLSAnalyzedFile : public MakeAnalyzedFile {
public:
	MakeXLSAnalyzedFile();
	virtual void Execute(ConsoleUI* consoleui) override;
};