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
		//buffer += std::to_string(pinfo.second.processCPUPersents) + "%\n";
		buffer += std::to_string(pinfo.second.processCPUPersents) + '%';
		complete_string_to_console(buffer);
		allcpu += pinfo.second.processCPUPersents;
	}
	if (staticinfo != nullptr) {
		for (int i = 0; i < maxfilename; i++)
			*staticinfo += '=';
		//*staticinfo += "\nGlobal CPU usage: " + std::to_string(allcpu) + "%\n";
		*staticinfo += "\nGlobal CPU usage: " + std::to_string(allcpu);
		complete_string_to_console((*staticinfo));
	}
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
		buffer += "Memory\tCPU";
		#else
		buffer += std::string("Memory") + TAB + "CPU";
		#endif
		complete_string_to_console(buffer);
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
	MakeActionHandlerThread();
	MakeProcessAnalyzingThread();
}
void ConsoleUI::SetOutputPrinter(const ProcessPrinter& printer) {
	this->printer = printer;
}
void ConsoleUI::SetVisioner(ProcessVisioner& visioner) {
	this->visioner = &visioner;
}
void ConsoleUI::MakeActionHandlerThread() {
	actionHandling = std::thread(&ConsoleUI::ButtonActionsHandler, this);
}
void ConsoleUI::MakeProcessAnalyzingThread()
{
	processAnalyzingThread = std::thread(&ConsoleUI::ProcessAnalyzing, this);
}
void ConsoleUI::DrawUI() {
	COORD cursorpos = { 0, 0 };
	size_t nextlineaddress = 0;
	std::string staticinfo;
	std::string printedinfo;
	if (forceDraw) {
		printedinfo = printer.printProcess(timeAnalyzer, &staticinfo);
	}
	else {
		timeAnalyzerMutex.lock();
		printedinfo = printer.printProcess(timeAnalyzer, &staticinfo);
		timeAnalyzerMutex.unlock();
	}
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
			if (bottomcounter == PRINTER_LIMIT) { bottomlimitedaddress = i; break; }
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
		if (printedlimitcounter == PRINTER_LIMIT) { eofdynamicpinfo = i - nextlineaddress + 1; break; }
	}
	cursormutex.unlock();
	nextlineaddress = nextlineaddress > 0 ? nextlineaddress : 0;
	//write out to the console buffer
	//DWORD d;
	//CONSOLE_SCREEN_BUFFER_INFO consoleBuffer;
	//GetConsoleScreenBufferInfo(thisconsole, &consoleBuffer);
	//FillConsoleOutputCharacter(thisconsole, ' ', consoleBuffer.dwSize.X * consoleBuffer.dwSize.Y, cursorpos, &d);
	std::lock_guard<std::mutex> consoleLock(consoleMutex);
	SetConsoleCursorPosition(thisconsole, cursorpos);
	WriteConsoleA(thisconsole, printedinfo.c_str() + nextlineaddress, eofdynamicpinfo * sizeof(char), NULL, NULL);
	WriteConsoleA(thisconsole, staticinfo.c_str(), staticinfo.length() * sizeof(char), NULL, NULL);
	SetConsoleCursorPosition(thisconsole, cursorpos);
}
const TimeAnalyzer* ConsoleUI::GetAnalyzer()
{
	return &(this->timeAnalyzer);
}
const ProcessVisioner* ConsoleUI::GetVisioner()
{
	return this->visioner;
}
void ConsoleUI::AddKeyBindAction(const char key, IExecutableProcedure* action)
{
	this->keyBindActions.insert(std::make_pair(key, action));
}
ConsoleUI::~ConsoleUI()
{
	this->running = false;
	CONSOLE_SCREEN_BUFFER_INFO consoleBuffer;
	GetConsoleScreenBufferInfo(thisconsole, &consoleBuffer);
	DWORD d;
	FillConsoleOutputCharacterA(thisconsole, ' ', consoleBuffer.dwSize.X * PRINTER_LIMIT * 2, { 0, 0 }, &d);
	if (this->actionHandling.joinable())
		actionHandling.join();
	if (this->processAnalyzingThread.joinable())
		processAnalyzingThread.join();
}
void ConsoleUI::ButtonActionsHandler() {
	while (running) {
		char sym = _getch();
		switch (sym) {
		case 's':
			cursormutex.lock();
			printercursor++;
			cursormutex.unlock();
			forceDraw = true;
			DrawUI();
			forceDraw = false;
			continue;
		case 'w':
			cursormutex.lock();
			printercursor--;
			cursormutex.unlock();
			forceDraw = true;
			DrawUI();
			forceDraw = false;
			continue;
		default: {
			auto actionPair = this->keyBindActions.find(sym);
			if (actionPair != this->keyBindActions.end()) {
				auto action = actionPair->second;
				action->Execute(this);
			}
			break;
		}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(20));
		std::this_thread::yield();
	}
}

void ConsoleUI::ProcessAnalyzing()
{
	while (running) {
		if (this->visioner) {
			try {
				visioner->makeSnapshot();
				timeAnalyzerMutex.lock();
				//timeAnalyzer.ClearResults();
				timeAnalyzer.Analyze(*visioner);
				//timeAnalyzer.AnalyzeMidValues(*visioner, 10, 5);
				timeAnalyzerMutex.unlock();
				
			}
			catch (std::runtime_error& error) {
				std::cout << error.what() << std::endl;
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(250));
			visioner->closeProcs();
		}
	}
}

ThreadCloseProcedure::ThreadCloseProcedure()
{
}

void ThreadCloseProcedure::Execute(ConsoleUI* consoleui)
{
	if(this->distributor)
		this->distributor->ClearThread();
}

void ThreadCloseProcedure::SetThreadDistrubutor(ThreadDistributor* distributor)
{
	this->distributor = distributor;
}
FinalAnalyzeProcedure::FinalAnalyzeProcedure()
{
}
void FinalAnalyzeProcedure::AnalyzeProcedure(ProcessVisioner &visioner, const std::string& filepath)
{
	StorageReader* reader = new BinaryReader();
	std::vector<std::vector<TimeAnalyzer::AnalyzedProcess>> parsedSnapshots;
	for (auto filepath : std::filesystem::directory_iterator(".")) {
		std::string filename = filepath.path().string();
		if (std::regex_match(filename, std::regex("\\.\\\\.+\\.psb$"))) {
			auto parsedSnapshot = reader->ReadStorage(filename);
			parsedSnapshots.push_back(parsedSnapshot);
		}
	}
	delete reader;
	MidTimeAnalyzer midAnalyzer;
	DataStorage* ds = new XLSStorage();
	midAnalyzer.Analyze(visioner, parsedSnapshots);
	ds->SaveToFile(midAnalyzer, filepath);
	delete ds;
	ds = nullptr;
}
void FinalAnalyzeProcedure::Execute(ConsoleUI* consoleui)
{
	ProcessVisioner visioner = *(consoleui->GetVisioner());
	AnalyzeProcedure(visioner, "analitics.xls");
}

MakeBinaryAnalyzedFile::MakeBinaryAnalyzedFile()
{

}
void MakeBinaryAnalyzedFile::Execute(ConsoleUI* consoleui) {
	DataStorage* ds = new BinaryStorage();
	std::time_t time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	std::tm* nowDate = std::localtime(&time);
	//
	std::string filepath = "parsed_snapshot-" + std::to_string(nowDate->tm_sec) + "." + std::to_string(nowDate->tm_min) + "." + std::to_string(nowDate->tm_hour) + "." + std::to_string(nowDate->tm_mday) + "." + std::to_string(nowDate->tm_mon + 1) + "." + std::to_string(nowDate->tm_year + 1900) + ".psb";
	consoleui->timeAnalyzerMutex.lock();
	SaveToFile((TimeAnalyzer&)*(consoleui->GetAnalyzer()), ds, filepath);
	consoleui->timeAnalyzerMutex.unlock();
	delete ds;
}

void MakeAnalyzedFile::SaveToFile(TimeAnalyzer& timeAnalyzer, DataStorage* dataStorage, std::string filename)
{
	dataStorage->SaveToFile(timeAnalyzer, filename);
}

MakeXLSAnalyzedFile::MakeXLSAnalyzedFile()
{
}

void MakeXLSAnalyzedFile::Execute(ConsoleUI* consoleui)
{
	DataStorage* ds = new XLSStorage();
	std::string filepath = "readable_list.xlsx";
	consoleui->timeAnalyzerMutex.lock();
	SaveToFile((TimeAnalyzer&)*(consoleui->GetAnalyzer()), ds, filepath);
	consoleui->timeAnalyzerMutex.unlock();
	delete ds;
}
