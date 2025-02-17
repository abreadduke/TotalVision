#include "timings.hpp"

void SystemTimer::SetTimerRate(const tm &time) {
	this->timeRate = time.tm_sec + time.tm_min * 60 + time.tm_hour * 60 * 60;
}
void SystemTimer::SetTimerRate(const time_t& time)
{
	this->timeRate = time;
}
time_t SystemTimer::GetTimerRate() {
	return this->timeRate;
}
void SystemTimer::ActivateTimerAction() {
	if (actiovationAction != nullptr) {
		this->actiovationAction->Action();
	}
}
void SystemTimer::DiscountOneSecondFromTimer() {
	if (this->timer <= 0) {
		ActivateTimerAction();
		this->timer = this->timeRate;
	}
	else {
		this->timer--;
	}
}
TimerStateReader::TimerStateReader(std::string filename) {
	this->filepath = filename;
}
SystemTimer::SystemTimer() {

}
AbstractSystemTimer* TimerStateReader::GetSystemTimer() {
	std::ifstream openedBinaryFile(this->filepath, std::ios::binary);
	time_t rate = 0;
	if (openedBinaryFile.is_open()) {
		std::unique_ptr<AbstractSystemTimer> timer(new SystemTimer());
		openedBinaryFile.read((char*)&rate, sizeof(time_t));
		openedBinaryFile.close();
		timer->SetTimerRate(rate);
		return timer.get();
	}
	return nullptr;
}
MakeSnapshotAnalyze::MakeSnapshotAnalyze() {}
void MakeSnapshotAnalyze::SetVisioner(ProcessVisioner* visioner) {
	this->visioner = visioner;
}
void MakeSnapshotAnalyze::Action()
{
	if (timeAnalyzer == nullptr) return;
	visioner->makeSnapshot();
	timeAnalyzer->Analyze(*visioner);
	DataStorage* ds = new BinaryStorage();
	std::time_t time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	std::tm* nowDate = std::localtime(&time);
	std::string filepath = this->directory + "\\parsed_snapshot-" + std::to_string(nowDate->tm_sec) + "." + std::to_string(nowDate->tm_min) + "." + std::to_string(nowDate->tm_hour) + "." + std::to_string(nowDate->tm_mday) + "." + std::to_string(nowDate->tm_mon + 1) + "." + std::to_string(nowDate->tm_year + 1900) + ".psb";
	ds->SaveToFile(*timeAnalyzer, filepath);
	timeAnalyzer->ClearResults();
	visioner->closeProcs();
	delete ds;
}
void MakeSnapshotAnalyze::SetAnalyzer(TimeAnalyzer* timeAnalyzer)
{
	this->timeAnalyzer = timeAnalyzer;
}
void MakeSnapshotAnalyze::SetSavingDirectory(std::string directory)
{
	this->directory = directory;
}
TimerStateSaver::TimerStateSaver(AbstractSystemTimer* &timer, std::string filepath) {
	this->filepath = filepath;
	this->timer = &timer;
}
void TimerStateSaver::SaveTimer() {
	(*timer)->UseWriter(this);
	std::ofstream openedBinaryFile(filepath, std::ios::binary);
	if (openedBinaryFile.is_open()) {
		openedBinaryFile.write((const char*)&timeRate, sizeof(timeRate));
		openedBinaryFile.close();
	}
	else {
		throw std::exception(FILE_OPENING_EXCEPTION_MESSAGE);
	}
}
void TimerStateSaver::SaveAsYieldingSystemTimer(YieldingSystemTimer* yieldingSystemTimer)
{
	actionsCount = yieldingSystemTimer->GetActionCounts();
}
void TimerStateSaver::SaveAsSystemTimer(SystemTimer *systemtimer)
{
	timeRate = systemtimer->GetTimerRate();
}
void SystemTimer::SetTimerAction(ITimerAction* timerAction) {
	this->actiovationAction = timerAction;
}

ITimerAction* SystemTimer::GetTimerAction() const
{
	return this->actiovationAction;
}

bool SystemTimer::IsTimerCompleted()
{
	return this->timerCompleted;
}

void SystemTimer::Continue()
{
	this->timerCompleted = false;
}

void SystemTimer::UseWriter(TimerStateSaver* saver)
{
	saver->SaveAsSystemTimer(this);
}

void SystemTimer::UseReader(TimerStateReader* reader)
{
}

const std::string SystemTimer::GetType() const
{
	return TypeToString(SystemTimer);
}

YieldingSystemTimer::YieldingSystemTimer() : SystemTimer()
{
}
void YieldingSystemTimer::SetTimerRate(const tm& time)
{
	this->timeRate = this->timer = time.tm_sec + time.tm_min * 60 + time.tm_hour * 60 * 60;
}
void YieldingSystemTimer::SetTimerRate(const time_t& time)
{
	this->timeRate = this->timer = time;
}
void YieldingSystemTimer::SetActionCounts(const int counts)
{
	this->actionCounts = counts;
}
int YieldingSystemTimer::GetActionCounts() const
{
	return actionCounts;
}
void YieldingSystemTimer::DiscountOneSecondFromTimer()
{
	if (actionCounts && timeRate > 0) {
		this->timer--;
		if ((float)1 - (float)timer / (float)timeRate >= (float)(actionsCompleted) / (float)actionCounts) {
			ActivateTimerAction();
			actionsCompleted++;
		}
	}
	if(actionsCompleted == actionCounts){
		timerCompleted = true;
		this->timer = this->timeRate;
		this->actionsCompleted = 0;
	}
}

void YieldingSystemTimer::UseWriter(TimerStateSaver* saver)
{
	SystemTimer::UseWriter(saver);
	saver->SaveAsYieldingSystemTimer(this);
}

void YieldingSystemTimer::UseReader(TimerStateReader* reader)
{
}

const std::string YieldingSystemTimer::GetType() const
{
	return TypeToString(YieldingSystemTimer);
}

TimerTimeFileReader::TimerTimeFileReader(std::string filename) : TimerStateReader(filename)
{
}
#define GET_TIMER_TYPE_TEMPLATE(class_name) "\\[("#class_name")\\]"
#define GENERATE_TIMER_TYPE_CONDITION(class_name)	if (std::regex_match(line, match, std::regex(GET_TIMER_TYPE_TEMPLATE(class_name)))) {\
													if (timer != nullptr) delete timer;\
													timer = new class_name();\
													}
AbstractSystemTimer* TimerTimeFileReader::GetSystemTimer()
{
	std::ifstream openedTimeFile(this->filepath);
	time_t rate = 0;
	AbstractSystemTimer* timer = nullptr;
	if (openedTimeFile.is_open()) {
		while (!openedTimeFile.eof()) {
			std::string line;
			openedTimeFile >> line;
			std::string parameter;
			std::string value;
			std::smatch match;
			GENERATE_TIMER_TYPE_CONDITION(SystemTimer)
			else GENERATE_TIMER_TYPE_CONDITION(YieldingSystemTimer)
			else if (std::regex_match(line, match, std::regex("(\\S+)\\s*=\\s*(\\S+)"))) {
				parameter = match[1].str();
				value = match[2].str();
				if (parameter == "rate") {
					int rate = atoi(value.c_str());
					timer->SetTimerRate(rate);
				}
				else if (parameter == "actions_count") {
					int count = atoi(value.c_str());
					dynamic_cast<YieldingSystemTimer*>(timer)->SetActionCounts(count);
				}
			}
		}
	}
	return timer;
}
#undef GET_TIMER_TYPE_TEMPLATE
#undef GENERATE_TIMER_TYPE_CONDITION
TimerTimeFileSaver::TimerTimeFileSaver(AbstractSystemTimer* &timer, std::string filepath) : TimerStateSaver(timer, filepath) {

}
void TimerTimeFileSaver::SaveTimer()
{
	(*this->timer)->UseWriter((TimerStateSaver*)this);
	std::ofstream openedTimeFile(filepath);
	if ((*this->timer)->GetType() == UNDEFINED_TIMER_TYPE) return;
	if (openedTimeFile.is_open()) {
		time_t rate = timeRate;
		std::string data = "[" + (*this->timer)->GetType() + "]\n";
		data += "rate=" + std::to_string(rate) + "\n";
		if (actionsCount != 0)
			data += "actions_count=" + std::to_string(actionsCount) + "\n";
		openedTimeFile.write(data.c_str(), data.length());
		openedTimeFile.close();
	}
	else {
		throw std::exception(FILE_OPENING_EXCEPTION_MESSAGE);
	}
}

MakeXLSAnalyze::MakeXLSAnalyze()
{
	
}

MakeXLSAnalyze::MakeXLSAnalyze(TimeAnalyzer* timeAnalyzer, ProcessVisioner* visioner, AbstractSystemTimer* &timer, const std::string& directory)
{
	SetAnalyzer(timeAnalyzer);
	SetVisioner(visioner);
	SetSavingDirectory(directory);
	this->timer = &timer;
}

#define Type(type) #type
void MakeXLSAnalyze::Action()
{
	visioner->makeSnapshot();
	StorageReader* reader = new BinaryReader();
	std::vector<std::vector<TimeAnalyzedProcess>> parsedSnapshots;
	for (auto filepath : std::filesystem::directory_iterator(".")) {
		std::string filename = filepath.path().string();
		if (std::regex_match(filename, std::regex("\\.\\\\.+\\" DEFAULT_SNAPSHOT_FORMAT "$"))) {
			auto parsedSnapshot = reader->ReadStorage(filename);
			std::vector<TimeAnalyzedProcess> timeParsedSnapshot;
			for (auto parsedProcess : parsedSnapshot) {
				timeParsedSnapshot.push_back(TimeAnalyzedProcess(parsedProcess));
			}
			parsedSnapshots.push_back(timeParsedSnapshot);
		}
	}
	delete reader;
	XLSStorage ds;
	//midAnalyzer.Analyze(*visioner, parsedSnapshots);
	auto processInterfaceBuilder = TimeProcessInterfaceBuilder();
	if((*timer)->GetType() == Type(YieldingSystemTimer))
		processInterfaceBuilder.SetTimerRate((*timer)->GetTimerRate() / ((YieldingSystemTimer*)*timer)->GetActionCounts());
	else if((*timer)->GetType() == Type(SystemTimer))
		processInterfaceBuilder.SetTimerRate((*timer)->GetTimerRate());
	std::map<DWORD, TimeAnalyzedProcess> procs = MediateProcesses<TimeAnalyzedProcess>(parsedSnapshots, &processInterfaceBuilder);
	ds.SaveToFile(procs, directory);
	visioner->closeProcs();
}

inline const std::string UndefinedTimerType::GetType() const
{
	return UNDEFINED_TIMER_TYPE;
}
