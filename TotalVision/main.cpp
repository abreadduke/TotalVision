//#define TEST

#ifndef TEST
#include "defined_paths.h"
#include "commandhandler.hpp"
#include <windows.h>
#include <iostream>
#include "visioner.hpp"
#include "analyzer.hpp"
#include "consoleinterface.hpp"

TimeFacadeSystem* timeFacadeSystem = nullptr;
ThreadDistributor* ProgrammInterfaceThreadDistr = nullptr;
std::thread timerSecondsUpdate;
void Closing() {
	timerSecondsUpdate.detach();
	delete timeFacadeSystem;
	delete ProgrammInterfaceThreadDistr;
}
BOOL WINAPI ConsoleSignalReceiver(_In_ DWORD dwCtrlType) {
	if (dwCtrlType == CTRL_CLOSE_EVENT || dwCtrlType == CTRL_SHUTDOWN_EVENT) {
		Closing();
	}
	return true;
}
int main() {
	setlocale(LC_ALL, "ru");
	SetConsoleOutputCP(1251);
	SetConsoleCP(1251);
	//ProcessVisioner visioner;
	//ProcessPrinter printer;
	//ConsoleUI ui;
	//ui.SetOutputPrinter(printer);
	//ui.SetVisioner(visioner);
	//ui.MakeCursorThread();
	//while (true) {
	//	try {
	//		visioner.makeSnapshot();
	//	}
	//	catch (std::runtime_error& error) {
	//		std::cout << error.what() << std::endl;
	//		return -1;
	//	}
	//	ui.DrawUI();
	//	visioner.closeProcs();
	//}

	//Bind console exit actions to the procedure
	SetConsoleCtrlHandler(ConsoleSignalReceiver, true);

	//Setup programm console commands
	ConsoleReadCommand reader;
	VisualCommand visc;
	TimerCommand timerCommand;
	OpenSnapshotCommand openSnapshotCommand;
	GetSnapshotListCommand getSnapshotListCommand;
	ClearShanpshotsCommand clearSnapshotsCommand;
	HelpCommand helpCommand;
	AbstractSystemTimer* timer = new SystemTimer();
	timerCommand.SetTimer(timer);
	std::mutex mainThreadLocker;
	ThreadDistributor *ProgrammInterfaceThreadDistr = new ThreadDistributor();
	ProgrammInterfaceThreadDistr->lockOtherThreads = &mainThreadLocker;
	visc.SetDistributor(ProgrammInterfaceThreadDistr);
	reader.AddCommandHandler(&visc);
	reader.AddCommandHandler(&timerCommand);
	reader.AddCommandHandler(&openSnapshotCommand);
	reader.AddCommandHandler(&getSnapshotListCommand);
	reader.AddCommandHandler(&clearSnapshotsCommand);
	reader.AddCommandHandler(&helpCommand);
	//
	//Setup TimeSystems
	const std::string timerPath = TIMER_IO_FILE_PATH;
	//ISystemTimer* systemTimer = new SystemTimer();
	TimeAnalyzer timerAnalyzer;
	ProcessVisioner visioner;
	MakeSnapshotAnalyze timerAction;
	MakeXLSAnalyze endignTimerAction(&timerAnalyzer, &visioner, timer, FINAL_EXEL_TABLE_PATH);
	timerAction.SetAnalyzer(&timerAnalyzer);
	timerAction.SetVisioner(&visioner);
	timerAction.SetSavingDirectory(".");
	timeFacadeSystem = new TimeFacadeSystem(timerPath, timer, &timerAction, &endignTimerAction);
	timeFacadeSystem->Setup();
	TimeFacadeSystem* fassys = timeFacadeSystem;
	timerSecondsUpdate = std::thread([&fassys]() {
		while (true) {
			if (DynamicSettings::IsTimerActive()) {
				fassys->UpdateSeconds();
			}
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
	});
	while (true) {
		const int unknown_command_buffer = 150;
		char unknown_command_message[unknown_command_buffer];
		LoadStringA(nullptr, UNKNOWN_COMMAND_MESSAGE, unknown_command_message, unknown_command_buffer);
		std::string command;
		if (ProgrammInterfaceThreadDistr->GetThread() == nullptr) {
			std::getline(std::cin, command);
			reader.SetCommand(command);
			if (!reader.ExecuteCommand()) {
				std::cout << unknown_command_message << command << std::endl;
			}
		}
		else {
			try {
				std::lock_guard<std::mutex> thread_locker(mainThreadLocker);
			}
			catch (std::system_error &error){
				std::cout << error.what() << std::endl;
			}
		}
	}
	//Exiting
	Closing();
	return 0;
}
#else
#include "tests/timings_test.hpp"
//#define Q() "
//#define TEST_INCLUDE(test_path) Q()tests/##test_path##.hpp"
int main() {
	TestTimings();
	return 0;
}
#endif