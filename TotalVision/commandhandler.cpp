#include "commandhandler.hpp"

ConsoleReadCommand::ConsoleReadCommand() {}
ConsoleReadCommand::ConsoleReadCommand(std::string command) {
	SetCommand(command);
}
bool ConsoleReadCommand::ExecuteCommand() {
	bool handlersWorked = false;
	for (auto handler : this->commandHandlers) {
		handler->SetCommand(command);
		handlersWorked |= handler->ExecuteCommand();
	}
	return handlersWorked;
}
void ConsoleReadCommand::AddCommandHandler(ICommand* commandhandler) {
	commandHandlers.push_back(commandhandler);
}
void Command::SetCommand(std::string command) {
	this->command = command;
}
Command::Command() {}
bool Command::ExecuteCommand() { return true; }

VisualCommand::VisualCommand()
{
}

bool VisualCommand::ExecuteCommand()
{
	if (std::regex_match(command, std::regex("visual"))) {
		std::thread* interfaceThread = nullptr;
		if (this->distributor != nullptr && this->distributor->GetNewThread(interfaceThread)) {
			*interfaceThread = std::thread([this]() {
				ProcessVisioner visioner;
				ProcessPrinter printer;
				ConsoleUI ui;
				ui.SetOutputPrinter(printer);
				ui.SetVisioner(visioner);
				FinalAnalyzeProcedure finalAnalyzeProcedure;
				MakeBinaryAnalyzedFile binaryMakerAction;
				ThreadCloseProcedure threadCloseProcedure;
				MakeXLSAnalyzedFile xlsMakerAction;
				threadCloseProcedure.SetThreadDistrubutor(distributor);
				//key actions
				ui.AddKeyBindAction('p', &finalAnalyzeProcedure);
				ui.AddKeyBindAction('e', &binaryMakerAction);
				ui.AddKeyBindAction('q', &threadCloseProcedure);
				ui.AddKeyBindAction('x', &xlsMakerAction);
				while (true) {
					if (!distributor->isExist()) {
						distributor->ClearThread();
						break;
					}
					ui.DrawUI();
					if (!distributor->isExist())
						break;
					std::this_thread::sleep_for(std::chrono::milliseconds(20));
					std::this_thread::yield();
				}
			});
		}
		return true;
	}
	return false;
}
bool VisualCommand::SetDistributor(ThreadDistributor* distributor) {
	if (this->distributor != nullptr) return false;
	this->distributor = distributor;
	return true;
}

bool TimerCommand::ExecuteCommand() {
	std::smatch match;
	if (std::regex_search(command, match, std::regex("timer +(\\d*):(\\d*):(\\d*)"))) {
		//timer flags
		std::smatch timerModeMatch;
		if (std::regex_search(command, timerModeMatch, std::regex(" +/mode +(auto|manual) *"))) {
			std::string mode = timerModeMatch[1].str();
			ITimerAction* timerAction = (*this->timer)->GetTimerAction();
			if (mode == "auto") {
				if (*this->timer) {
					delete *this->timer;
					(*this->timer) = new YieldingSystemTimer();
				}
			}
			else if (mode == "manual") {
				if (*this->timer) {
					delete *this->timer;
					(*this->timer) = new SystemTimer();
				}
			}
			(*this->timer)->SetTimerAction(timerAction);
		}
		int timerCounts = 1;
		std::smatch yieldingTimerCountsMatch;
		if (std::regex_search(command, yieldingTimerCountsMatch, std::regex(" +\\/c +(\\d+) *"))) {
			timerCounts = atoi(yieldingTimerCountsMatch[1].str().c_str());
		}
		char seconds = atoi(match[1].str().c_str());
		char minutes = atoi(match[2].str().c_str());
		char hours = atoi(match[3].str().c_str());
		seconds = 59 < seconds ?  59 : (0 > seconds ? 0 : seconds);
		minutes = 59 < minutes ? 59 : (0 > minutes ? 0 : minutes);
		hours = 23 < hours ? 23 : (0 > hours ? 0 : hours);
		tm time;
		time.tm_sec = seconds;
		time.tm_min = minutes;
		time.tm_hour = hours;
		(*this->timer)->SetTimerRate(time);
		if ((*this->timer)->GetType() == TypeToString(YieldingSystemTimer))
			dynamic_cast<YieldingSystemTimer*>(*this->timer)->SetActionCounts(timerCounts);
		return true;
	}
	else if (std::regex_match(command, match, std::regex("timer +activate"))) {
		time_t timerRateFuse = 3;
		if ((*this->timer)->GetTimerRate() < timerRateFuse) {
			(*this->timer)->SetTimerRate(timerRateFuse);
		}
		DynamicSettings::SetTimerActiveMode(true);
		return true;
	}
	else if (std::regex_match(command, match, std::regex("timer +diactivate"))) {
		DynamicSettings::SetTimerActiveMode(false);
		return true;
	}
	return false;
}
AbstractSystemTimer* TimerCommand::GetTimer() const{
	return (*this->timer);
}
void TimerCommand::SetTimer(AbstractSystemTimer* &timer)
{
	if (!this->timer) {
		delete this->timer;
	}
	MakeSnapshotAnalyze timerAction;
	this->timer = &timer;
	(*this->timer)->SetTimerAction(&timerAction);
}
TimerCommand::TimerCommand() {
	if (!this->timer) {
		this->timer = new AbstractSystemTimer*;
		(*this->timer) = new YieldingSystemTimer();
		MakeSnapshotAnalyze timerAction;
		(*this->timer)->SetTimerAction(&timerAction);
	}
}
TimerCommand::~TimerCommand() {
	delete this->timer;
}
TimeFacadeSystem::TimeFacadeSystem()
{
}
TimeFacadeSystem::TimeFacadeSystem(std::string timerPath, AbstractSystemTimer* &timer, ITimerAction* timerAction)
{
	this->timerPath = timerPath;
	this->timer = &timer;
	this->timerAction = timerAction;
}

TimeFacadeSystem::TimeFacadeSystem(std::string timerPath, AbstractSystemTimer* &timer, ITimerAction* timerAction, ITimerAction* endingTimerAction) : TimeFacadeSystem(timerPath, timer, timerAction)
{
	this->endingTimerAction = endingTimerAction;
}

bool TimeFacadeSystem::Setup()
{
	bool returnValue = true;
	timeReader = new TimerTimeFileReader(timerPath);
	timeWriter = new TimerTimeFileSaver(*timer, timerPath);
	AbstractSystemTimer* receivedSystemTimer = (timeReader->GetSystemTimer());
	if (receivedSystemTimer != nullptr && timer != nullptr) {
		time_t timeRate = receivedSystemTimer->GetTimerRate();
		if (receivedSystemTimer->GetType() == TypeToString(YieldingSystemTimer)) {
			if (timer != nullptr) {
				delete *timer;
				*timer = new YieldingSystemTimer();
			}
			int actionCount = dynamic_cast<YieldingSystemTimer*>(receivedSystemTimer)->GetActionCounts();
			dynamic_cast<YieldingSystemTimer*>(*this->timer)->SetActionCounts(actionCount);
		}
		(*this->timer)->SetTimerRate(timeRate);
		delete receivedSystemTimer;
	}
	else {
		returnValue = false;
	}
	if(timer)
		(*this->timer)->SetTimerAction(this->timerAction);
	return returnValue;
}

void TimeFacadeSystem::UpdateSeconds()
{
	if (*this->timer != nullptr) {
		(*this->timer)->DiscountOneSecondFromTimer();
		if ((*this->timer)->IsTimerCompleted()) {
			DynamicSettings::SetTimerActiveMode(false);
			(*this->timer)->Continue();
			if (this->endingTimerAction != nullptr) {
				endingTimerAction->Action();
			}
		}
	}
}

TimeFacadeSystem::~TimeFacadeSystem()
{
	timeWriter->SaveTimer();
	delete timeReader;
	delete timeWriter;
	timeReader = nullptr;
	timeWriter = nullptr;
}

