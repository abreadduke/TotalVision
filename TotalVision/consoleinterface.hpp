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
#define BYTENEXTDIS 1024
#define TAB "    "
#define complete_string_to_console(buffer)  buffer += std::string(10, ' ') + '\n'
#define PRINTER_LIMIT 30

//#define TABENABLE
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
	const TimeAnalyzer* GetAnalyzer();
	const ProcessVisioner* GetVisioner();
	std::mutex timeAnalyzerMutex;
	std::mutex consoleMutex;
	void AddKeyBindAction(const char key, IExecutableProcedure* action);
	~ConsoleUI();
protected:
	void MakeActionHandlerThread();
	void MakeProcessAnalyzingThread();
private:
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