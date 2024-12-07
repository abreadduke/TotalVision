#include "commandhandler.hpp"

ThreadDistributor::ThreadDistributor(){}
bool ThreadDistributor::GetNewThread(std::thread* &t) {
	if (this->isThreadExist) return false;
	t = new std::thread;
	this->t = t;
	this->isThreadExist = true;
	return true;
}
bool ThreadDistributor::ClearThread() {
	if (!this->isThreadExist) return false;
	this->isThreadExist = false;
	delete this->t;
	return true;
}
std::thread* ThreadDistributor::GetThread() {
	if (this->isThreadExist) return this->t;
	return nullptr;
}
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
				ui.MakeCursorThread();
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
					if (distributor->GetThread() == nullptr) break;
				}
			});
			interfaceThread->detach();
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