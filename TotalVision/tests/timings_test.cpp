#include "timings_test.hpp"

void TestTimings() {
	std::filesystem::create_directory("temp");

	SystemTimer timer;
	tm timeRate = { 0 };
	timeRate.tm_sec = 3;
	timer.SetTimerRate(timeRate);
	MakeSnapshotAnalyze timerAction;
	timer.SetTimerAction(&timerAction);
	TimerStateReader reader("temp\\time.bin");
	TimerStateSaver saver(&timer, "temp\\time.bin");
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
	assert(reader.GetSystemTimer() != NULL);
	//testing for read validity
	assert(reader.GetSystemTimer()->GetTimerRate() == timer.GetTimerRate());
	for (int i = 0; i < 9; i++) {
		std::this_thread::sleep_for(std::chrono::seconds(1));
		try {
			timer.DiscountOneSecondFromTimer();
			timer2.DiscountOneSecondFromTimer();
		}
		catch(std::exception &ex){
			assert(false);
		}
	}
	delete analyzer;
	std::filesystem::directory_iterator diter(".\\temp");
	bool hasSaveTimerFile = false;
	bool isActionWorked = false;
	for (auto file : diter) {
		hasSaveTimerFile |= std::regex_match(file.path().string(), std::regex("\\.\\\\temp\\\\time.bin"));
		isActionWorked |= std::regex_match(file.path().string(), std::regex("\\.\\\\temp\\\\parsed_snapshot-.+"));
	}
	//testing for file existing
	assert(hasSaveTimerFile);
	//checking timer2 action working
	assert(isActionWorked);
}