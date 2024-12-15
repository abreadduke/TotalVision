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
ISystemTimer* TimerStateReader::GetSystemTimer() {
	std::ifstream openedBinaryFile(this->filepath, std::ios::binary);
	time_t rate = 0;
	if (openedBinaryFile.is_open()) {
		std::unique_ptr<ISystemTimer> timer(new SystemTimer());
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
TimerStateSaver::TimerStateSaver(ISystemTimer* timer, std::string filepath) {
	this->filepath = filepath;
	this->timer = timer;
}
void TimerStateSaver::SaveTimer() {
	std::ofstream openedBinaryFile(filepath, std::ios::binary);
	if (openedBinaryFile.is_open()) {
		time_t rate = this->timer->GetTimerRate();
		openedBinaryFile.write((const char*)&rate, sizeof(rate));
		openedBinaryFile.close();
	}
	else {
		throw std::exception(FILE_OPENING_EXCEPTION_MESSAGE);
	}
}
void SystemTimer::SetTimerAction(ITimerAction* timerAction) {
	this->actiovationAction = timerAction;
}