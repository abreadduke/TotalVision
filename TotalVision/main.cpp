//#define TEST

#ifndef TEST
#include <windows.h>
#include <iostream>
#include "visioner.hpp"
#include "analyzer.hpp"
#include "consoleinterface.hpp"
#include "commandhandler.hpp"

int main() {
	setlocale(LC_ALL, "ru");
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
	std::string command;
	ConsoleReadCommand reader;
	VisualCommand visc;
	ThreadDistributor *ProgrammInterfaceThreadDistr = new ThreadDistributor();
	visc.SetDistributor(ProgrammInterfaceThreadDistr);
	reader.AddCommandHandler(&visc);
	while (true) {
		if (ProgrammInterfaceThreadDistr->GetThread() == nullptr) {
			std::getline(std::cin, command);
			reader.SetCommand(command);
			if (!reader.ExecuteCommand()) {
				std::cout << "Неизвестная команда " << command << std::endl;
			}
		}
	}
	delete ProgrammInterfaceThreadDistr;
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