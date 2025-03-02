#include "timings_test.hpp"

void TestTimings() {
	std::filesystem::create_directory("temp");

	SystemTimer timer;
	tm timeRate = { 0 };
	timeRate.tm_sec = 3;
	timer.SetTimerRate(timeRate);
	MakeSnapshotAnalyze timerAction;
	timer.SetTimerAction(&timerAction);
	TimerTimeFileReader reader("temp\\time.bin");
	AbstractSystemTimer* timer_ptr = &timer;
	TimerTimeFileSaver saver(timer_ptr, "temp\\time.bin");
	LOG("testing timer saving/reading");
	saver.SaveTimer();
	SystemTimer timer2;
	MakeSnapshotAnalyze timerAction2;
	TimeAnalyzer* analyzer = new TimeAnalyzer();
	ProcessVisioner visioner;
	visioner.makeSnapshot();
	analyzer->Analyze(visioner);
	timerAction2.SetAnalyzer(analyzer);
	timerAction2.SetSavingDirectory("temp");
	timer2.SetTimerAction(&timerAction2);
	timer2.SetTimerRate(timeRate);
	//testing for null value
	auto readerTimerPtr = reader.GetSystemTimer();
	assert(readerTimerPtr != NULL);
	//testing for read validity
	assert(readerTimerPtr->GetTimerRate() == timeRate.tm_sec);
	assert(readerTimerPtr->GetType() == "SystemTimer");
	LOG("saving / reading - SUCCES");
	LOG("checking timers work");
	for (int i = 0; i < 9; i++) {
		std::this_thread::sleep_for(std::chrono::seconds(1));
		timer.DiscountOneSecondFromTimer();
		timer2.DiscountOneSecondFromTimer();
	}
	LOG("checking timers work - SUCCES");
	delete analyzer;
	std::filesystem::directory_iterator diter(".\\temp");
	bool hasSaveTimerFile = false;
	bool isActionWorked = false;
	for (auto file : diter) {
		hasSaveTimerFile |= std::regex_match(file.path().string(), std::regex("\\.\\\\temp\\\\time.bin"));
		isActionWorked |= std::regex_match(file.path().string(), std::regex("\\.\\\\temp\\\\parsed_snapshot-.+"));
	}
	LOG("checking for timers generated files");
	//testing for file existing
	assert(hasSaveTimerFile | isActionWorked);
	LOG("checking for timers generated files - SUCCES");
	LOG("testing timer sets");
	//testing timer date
	TimerCommand timerCommand;
	timerCommand.SetTimer(timer_ptr);
	timerCommand.SetCommand("timer 1000:1000:1000");
	assert(timerCommand.ExecuteCommand());
	assert(timer.GetTimerRate() == (time_t)(59+59*60+1000*60*60));
	timerCommand.SetCommand("timer -1000:-1000:-1000");
	assert(!timerCommand.ExecuteCommand());
	timerCommand.SetCommand("timer 10.5:-7:1");
	assert(!timerCommand.ExecuteCommand());
	timerCommand.SetCommand("timer s:m:h");
	assert(!timerCommand.ExecuteCommand());
	LOG("testing timer sets - SUCCES");
}