#include <windows.h>
#include <iostream>
#include <vector>
#include <tlhelp32.h>
#include <psapi.h>
#include <conio.h>

typedef void (&PROCESSHANDLER)(HANDLE);

std::vector<HANDLE> getAllProcs() {
	std::vector<HANDLE> procs;
	PROCESSENTRY32 peProcessEntry;
	TCHAR szBuff[1024];
	DWORD dwTemp;
	HANDLE CONST hSnapshot = CreateToolhelp32Snapshot(
		TH32CS_SNAPPROCESS, 0);
	if (INVALID_HANDLE_VALUE == hSnapshot) {
		throw std::runtime_error("Невозможно сделать снапшот процессов");
	}

	peProcessEntry.dwSize = sizeof(PROCESSENTRY32);
	Process32First(hSnapshot, &peProcessEntry);
	do {
		procs.push_back(OpenProcess(PROCESS_QUERY_INFORMATION | READ_CONTROL, true, peProcessEntry.th32ProcessID));
	} while (Process32Next(hSnapshot, &peProcessEntry));
	CloseHandle(hSnapshot);
	return procs;
}
class ProcessPrinter {
public:
	ProcessPrinter() {};
	static void printProcess(HANDLE process) {
		char filename[1024];
		if(GetProcessImageFileNameA(process, filename, sizeof(filename)))
			std::cout << filename << std::endl;
	}
};
class ProcessVisioner {
public:
	ProcessVisioner() {}
	void makeSnapshot() {
		procs = getAllProcs();
	}
	void sendProcsNames(PROCESSHANDLER prochandler) {
		for (HANDLE process : this->procs)
			prochandler(process);
	}
private:
	std::vector<HANDLE> procs;
};
int main() {
	setlocale(LC_ALL, "ru");
	ProcessVisioner visioner;
	try {
		visioner.makeSnapshot();
	}
	catch (std::runtime_error& error) {
		std::cout << error.what() << std::endl;
		return -1;
	}
	visioner.sendProcsNames(ProcessPrinter::printProcess);
	_getch();
	return 0;
}