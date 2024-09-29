#include <windows.h>
#include <iostream>
#include "visioner.h"
#include "analyzer.h"
#include "consoleinterface.h"

int main() {
	setlocale(LC_ALL, "ru");
	ProcessVisioner visioner;
	ProcessPrinter printer;
	ConsoleUI ui;
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
	}
	return 0;
}