#include "consoleinterface.hpp"

ProcessPrinter::ProcessPrinter() {}
std::string ProcessPrinter::printProcess(TimeAnalyzer& timeanalyzer, std::string* staticinfo) {
	//ProcessAnalyzer analyzer(visioner);
	//CpuAnalyzer cpuanalyzer;
	//cpuanalyzer.MakeStatementSnapshot();
	//analyzer.MakeStatementSnapshot();
	//std::this_thread::sleep_for(std::chrono::milliseconds(160));
	//auto analyzedtimes = analyzer.AnalyzeTimes();
	//cpuanalyzer.AnalyzeCpuUsage();
	//unsigned long long cpuusagetimes = cpuanalyzer.GetKernel() + cpuanalyzer.GetUser();
	std::string buffer;
	//find max file path length to padding the parameters
	char filename[filenamelength];
	unsigned short maxfilename = 0;
	/*
	for (HANDLE process : visioner.getProcs()) {
		char filename[filenamelength];
		if (GetProcessImageFileNameA(process, filename, sizeof(filename))) {
			maxfilename = (unsigned short)std::fmaxf((float)std::regex_replace(filename, std::regex("\\\\Device\\\\HarddiskVolume3"), "C:").size(), (float)maxfilename);
		}
	}
	*/
	for (auto pinfo : timeanalyzer.GetAnalyzed()) {
		maxfilename = (unsigned short)std::fmaxf(pinfo.second.processName.length(), maxfilename);
	}
	/*
	unsigned long long allprocstimes = 0;
	for (auto analyzedtime : analyzedtimes) {
		allprocstimes += analyzedtime.second;
	}
	*/
	buffer = printHeaders(maxfilename);
	double allcpu = 0;
	/*
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
	*/
	for (auto pinfo : timeanalyzer.GetAnalyzed()) {
		std::string rfilename = pinfo.second.processName;
		buffer += rfilename;
		for (int i = 0; i < maxfilename - rfilename.size(); i++)
			buffer += ' ';
		for (int i = 0; i < this->indents; i++)
			#ifdef TABENABLE
			buffer += '\t';
			#else
			buffer += TAB;
			#endif
		buffer += std::to_string(pinfo.second.processMemoryUsage) + "MB";
		for (int i = 0; i < this->indents; i++)
			#ifdef TABENABLE
			buffer += '\t';
			#else
			buffer += TAB;
			#endif
		//get system time for define cpu usage
		buffer += std::to_string(pinfo.second.processCPUPersents) + "%\n";
		allcpu += pinfo.second.processCPUPersents;
	}
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
			#ifdef TABENABLE
			buffer += '\t';
			#else
			buffer += TAB;
			#endif
		#ifdef TABENABLE
		buffer += "Memory\tCPU\n";
		#else
		buffer += std::string("Memory") + TAB + "CPU\n";
		#endif
	}
	return buffer;
}
void ProcessPrinter::setIndents(int indents) {
	this->indents = indents;
}
ConsoleUI::ConsoleUI() {
	thisconsole = GetStdHandle(STD_OUTPUT_HANDLE);
	timeAnalyzer.SetBufferSize(50);
	timeAnalyzer.SetSnapshotsCount(15);
}
void ConsoleUI::SetOutputPrinter(const ProcessPrinter& printer) {
	this->printer = printer;
}
void ConsoleUI::SetVisioner(ProcessVisioner& visioner) {
	this->visioner = &visioner;
}
void ConsoleUI::MakeCursorThread() {
	std::thread cursorthread(&ConsoleUI::CursorScrolling, this);
	cursorthread.detach();
}
void ConsoleUI::DrawUI() {
	timeAnalyzerMutex.lock();
	//timeAnalyzer.ClearResults();
	timeAnalyzer.Analyze(*visioner);
	//timeAnalyzer.AnalyzeMidValues(*visioner, 10, 5);
	timeAnalyzerMutex.unlock();
	COORD cursorpos = { 0, 0 };
	size_t nextlineaddress = 0;
	int printerlimit = 30;
	SetConsoleCursorPosition(thisconsole, cursorpos);
	std::string staticinfo;
	std::string printedinfo = printer.printProcess(timeAnalyzer, &staticinfo);
	int scrolledpath = 0;
	int printedlimitcounter = 0;
	size_t eofdynamicpinfo = 0;
	//handle information
	cursormutex.lock();
	printercursor = printercursor >= 0 ? printercursor : 0;
	size_t bottomlimitedaddress = 0;
	size_t bottomcounter = 0;
	for (size_t i = printedinfo.length(); i != 0; i--) {
		if (printedinfo[i] == '\n') {
			bottomcounter++;
			if (bottomcounter == printerlimit) { bottomlimitedaddress = i; break; }
		}
	}
	for (size_t i = 0; i < printedinfo.length(); i++) {
		if (printedinfo[i] == '\n')
			scrolledpath++;
		if (scrolledpath == printercursor || i + 1 > bottomlimitedaddress) {
			printercursor = scrolledpath;
			nextlineaddress = i;
			break; 
		}
	}
	for (size_t i = nextlineaddress; i < printedinfo.length(); i++) {
		if (printedinfo[i] == '\n')
			printedlimitcounter++;
		if (printedlimitcounter == printerlimit) { eofdynamicpinfo = i - nextlineaddress + 1; break; }
	}
	cursormutex.unlock();
	nextlineaddress = nextlineaddress > 0 ? nextlineaddress : 0;
	//write out to the console buffer
	WriteConsoleA(thisconsole, printedinfo.c_str() + nextlineaddress, eofdynamicpinfo * sizeof(char), NULL, NULL);
	WriteConsoleA(thisconsole, staticinfo.c_str(), staticinfo.length() * sizeof(char), NULL, NULL);
	std::this_thread::sleep_for(std::chrono::milliseconds(250));
	//system("cls");
}
void ConsoleUI::CursorScrolling() {
	while (true) {
		char sym = _getch();
		switch (sym) {
		case 's':
			cursormutex.lock();
			printercursor++;
			cursormutex.unlock();
			continue;
		case 'w':
			cursormutex.lock();
			printercursor--;
			cursormutex.unlock();
			continue;
		case 'q': {
			StorageReader* reader = new BinaryReader();
			std::vector<std::vector<TimeAnalyzer::AnalyzedProcess>> parsedSnapshots;
			for (auto filepath : std::filesystem::directory_iterator(".")) {
				std::string filename = filepath.path().string();
				if (std::regex_match(filename, std::regex("\\.\\\\parsed_snapshot-.+"))) {
					auto parsedSnapshot = reader->ReadStorage(filename);
					parsedSnapshots.push_back(parsedSnapshot);
				}
			}
			delete reader;
			MidTimeAnalyzer midAnalyzer;
			DataStorage* ds = new XLSStorage();
			std::string filepath = "analitics.xlsx";
			midAnalyzer.Analyze(*visioner, parsedSnapshots);
			ds->SaveToFile(midAnalyzer, filepath);
			delete ds;
			continue;
		}
		case 'e':
			//XLSStorage ds;
			DataStorage* ds = new BinaryStorage();
			std::time_t time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
			std::tm* nowDate = std::localtime(&time);
			//
			std::string filepath = "parsed_snapshot-" + std::to_string(nowDate->tm_sec) + "." + std::to_string(nowDate->tm_min) + "." + std::to_string(nowDate->tm_hour) + "." + std::to_string(nowDate->tm_mday) + "." + std::to_string(nowDate->tm_mon + 1) + "." + std::to_string(nowDate->tm_year + 1900) + ".psb";
			timeAnalyzerMutex.lock();
			ds->SaveToFile(timeAnalyzer, filepath);
			timeAnalyzerMutex.unlock();
			delete ds;
			continue;
		}
	}
}