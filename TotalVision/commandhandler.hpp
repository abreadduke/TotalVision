#pragma once
#include <iostream>
#include <regex>
#include <list>
#include <thread>
#include "consoleinterface.hpp"
#include "visioner.hpp"

class ThreadDistributor {
public:
	ThreadDistributor();
	bool GetNewThread(std::thread* &t);
	bool ClearThread();
	std::thread* GetThread();
private:
	bool isThreadExist = false;
	std::thread* t = nullptr;
};
class ICommand {
public:
	virtual void SetCommand(std::string command) = 0;
	virtual bool ExecuteCommand() = 0;
};
class Command : public ICommand{
public:
	Command();
	virtual void SetCommand(std::string command) override;
	virtual bool ExecuteCommand() override;
protected:
	std::string command;
};
class ConsoleReadCommand : public Command {
public:
	ConsoleReadCommand();
	ConsoleReadCommand(std::string command);
	virtual bool ExecuteCommand() override;
	virtual void AddCommandHandler(ICommand* commandhandler);
private:
	std::list<ICommand*> commandHandlers;
};
class VisualCommand : public Command {
public:
	VisualCommand();
	virtual bool ExecuteCommand() override;
	virtual bool SetDistributor(ThreadDistributor* distributor);
private:
	ThreadDistributor* distributor = nullptr;
};