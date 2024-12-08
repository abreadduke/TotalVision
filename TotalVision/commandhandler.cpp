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
				ui.MakeActionHandlerThread();
				FinalAnalyzeProcedure finalAnalyzeProcedure;
				MakeBinaryAnalyzedFile binaryMakerAction;
				ThreadCloseProcedure threadCloseProcedure;
				threadCloseProcedure.SetThreadDistrubutor(distributor);
				ui.AddKeyBindAction('p', &finalAnalyzeProcedure);
				ui.AddKeyBindAction('e', &binaryMakerAction);
				ui.AddKeyBindAction('q', &threadCloseProcedure);
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
					if (!distributor->isExist())
						break;
				}
			});
			//interfaceThread->detach();
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
	if (std::regex_match(command, match, std::regex("timer +(\\d*):(\\d*):(\\d*)"))) {
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
		this->timer->SetTimerRate(time);
		std::cout << time.tm_sec << time.tm_min << time.tm_hour << std::endl;
		return true;
	}
	return false;
}
ISystemTimer* TimerCommand::GetTimer() {
	return this->timer;
}
TimerCommand::TimerCommand() {
	this->timer = new SystemTimer();
	timer->SetTimerAction(nullptr);
}
TimerCommand::~TimerCommand() {
	delete this->timer;
}