#include "consoleinterface.hpp"

ProcessPrinter::ProcessPrinter() {}
digits getDigitsFromNumber(unsigned int number) {
	digits c = 0;
	while (true) {
		if (number == 0) return c;
		else {
			c++;
			//divide a number by the number system
			number /= 10;
		}
	}
}
std::string ProcessPrinter::printProcess(TimeAnalyzer& timeanalyzer, std::string* staticinfo) {
	std::string buffer;
	char filename[filenamelength];
	unsigned short maxfilename = 0;
	unsigned int maxPIDSize = 0;
	for (auto pinfo : timeanalyzer.GetAnalyzed()) {
		maxfilename = (unsigned short)std::fmaxf(pinfo.second.processName.length(), maxfilename);
		maxPIDSize = max(maxPIDSize, pinfo.first);
	}
	buffer = printHeaders(maxfilename);
	double allcpu = 0;
	for (auto pinfo : timeanalyzer.GetAnalyzed()) {
		buffer += std::to_string(pinfo.first) + TAB;
		std::string rfilename = pinfo.second.processName;
		digits PIDMaxDigitsCount = getDigitsFromNumber(maxPIDSize);
		for (digits i = 0; i < PIDMaxDigitsCount - getDigitsFromNumber(pinfo.first); i++)
			buffer += ' ';
		buffer += rfilename;
		for (int i = 0; i < maxfilename - rfilename.size(); i++)
			buffer += ' ';
		for (int i = 0; i < this->indents; i++)
			buffer += TAB;
		buffer += std::to_string(pinfo.second.processMemoryUsage) + "MB";
		for (int i = 0; i < this->indents; i++)
			buffer += TAB;
		//get system time for define cpu usage
		buffer += std::to_string(pinfo.second.processCPUPersents) + '%';
		complete_string_to_console(buffer);
		allcpu += pinfo.second.processCPUPersents;
	}
	if (staticinfo != nullptr) {
		for (int i = 0; i < maxfilename; i++)
			*staticinfo += '=';
		*staticinfo += "\nGlobal CPU usage: " + std::to_string(allcpu);
		complete_string_to_console((*staticinfo));
	}
	return buffer;
}
std::string ProcessPrinter::printHeaders(int memoryindent) {
	std::string buffer;
	buffer = "PID" + std::string(TAB) + "PROCESS";
	if (memoryindent >= (sizeof("PROCESS") / sizeof(char) - 1)) {
		for (int i = 0; i < (memoryindent - (sizeof("PROCESS") / sizeof(char) - 1)); i++)
			buffer += ' ';
		for (int i = 0; i < this->indents; i++)
			buffer += TAB;
		buffer += std::string("Memory") + TAB + "CPU";
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
	timeAnalyzer.SetAnalyzerOffset(2);
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
	if (!canDraw) return;
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
			if (bottomcounter++ == PRINTER_LIMIT) { bottomlimitedaddress = i - 1; break; }
		}
	}
	if (bottomlimitedaddress == 0)
		bottomlimitedaddress - printedinfo.length();
	for (size_t i = 0; i < printedinfo.length(); i++) {
		if (printedinfo[i] == '\n')
			scrolledpath++;
		if (scrolledpath == printercursor || i + 1 > bottomlimitedaddress) {
			printercursor = scrolledpath;
			if (printedinfo[i] == '\n' && i+1 < printedinfo.length()) nextlineaddress = i+1;
			else nextlineaddress = i;
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
	std::lock_guard<std::mutex> consoleLock(consoleMutex);
	SetConsoleCursorPosition(thisconsole, cursorpos);
	WriteConsoleA(thisconsole, printedinfo.c_str() + nextlineaddress * sizeof(char), eofdynamicpinfo * sizeof(char), NULL, NULL);
	WriteConsoleA(thisconsole, staticinfo.c_str(), staticinfo.length() * sizeof(char), NULL, NULL);
	SetConsoleCursorPosition(thisconsole, cursorpos);
}
void ConsoleUI::LockDrawing()
{
	canDraw = false;
	consoleMutex.lock();
}
void ConsoleUI::UnlockDrawing()
{
	canDraw = true;
	consoleMutex.unlock();
}
void ConsoleUI::ClearConsoleBuffer()
{
	SetConsoleCursorPosition(thisconsole, {0, 0});
	CONSOLE_SCREEN_BUFFER_INFO consoleBuffer;
	GetConsoleScreenBufferInfo(thisconsole, &consoleBuffer);
	DWORD d;
	FillConsoleOutputCharacterA(thisconsole, ' ', consoleBuffer.dwSize.X * PRINTER_LIMIT * 2, { 0, 0 }, &d);
}
const TimeAnalyzer* ConsoleUI::GetAnalyzer() const
{
	return &(this->timeAnalyzer);
}
ProcessVisioner* ConsoleUI::GetVisioner() const
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
	ClearConsoleBuffer();
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
	}
}

void ConsoleUI::ProcessAnalyzing()
{
	while (running) {
		if (this->visioner) {
			try {
				visioner->makeSnapshot();
				timeAnalyzerMutex.lock();
				timeAnalyzer.Analyze(*visioner);
				timeAnalyzerMutex.unlock();
				
			}
			catch (std::runtime_error& error) {
				std::cout << error.what() << std::endl;
			}
			visioner->closeProcs();
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(20));
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
		if (std::regex_match(filename, std::regex("\\.\\\\.+\\" DEFAULT_SNAPSHOT_FORMAT "$"))) {
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
	std::string filepath = "parsed_snapshot-" + std::to_string(nowDate->tm_sec) + "." + std::to_string(nowDate->tm_min) + "." + std::to_string(nowDate->tm_hour) + "." + std::to_string(nowDate->tm_mday) + "." + std::to_string(nowDate->tm_mon + 1) + "." + std::to_string(nowDate->tm_year + 1900) + DEFAULT_SNAPSHOT_FORMAT;
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

KillProcessProcedure::KillProcessProcedure()
{
}

#define UNLOCK_AND_RETURN do{consoleui->ClearConsoleBuffer();consoleui->UnlockDrawing();return;}while(0)
void KillProcessProcedure::Execute(ConsoleUI* consoleui)
{
	consoleui->ClearConsoleBuffer();
	consoleui->LockDrawing();
	const int bufferSize = 150;
	char killMessage[bufferSize];
	LoadStringA(nullptr, PROCESS_KILL_MESSAGE, killMessage, bufferSize);
	std::cout << killMessage;
	std::string rawProcessPath;
	std::getline(std::cin, rawProcessPath);
	std::smatch match;
	if (std::regex_match(rawProcessPath, match, std::regex("\\d+"))) {
		HANDLE process = OpenProcess(PROCESS_TERMINATE, true, std::stoi(match[0].str()));
		if (process != NULL) {
			TerminateProcess(process, 1);
			CloseHandle(process);
		}
	}
	else {
		if (rawProcessPath == "") UNLOCK_AND_RETURN;
		auto visioner = consoleui->GetVisioner();
		visioner->closeProcs();
		visioner->makeSnapshot(PROCESS_TERMINATE);
		auto procs = visioner->getProcs();
		for (auto process : procs) {
			char imageFileName[1024];
			if (process != NULL &&
				GetProcessImageFileNameA(process, imageFileName, sizeof(imageFileName)) != NULL 
				&& RawPathToPolished(std::string(imageFileName)) == rawProcessPath) {
				TerminateProcess(process, 1);
				CloseHandle(process);
			}
		}
		visioner->closeProcs();
		visioner->makeSnapshot();
	}
	UNLOCK_AND_RETURN;
}
#undef UNLOCK_AND_RETURN
