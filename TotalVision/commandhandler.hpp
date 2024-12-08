#pragma once
#include <iostream>
#include <regex>
#include <list>
#include <thread>
#include "visioner.hpp"
#include "timings.hpp"
#include "consoleinterface.hpp"

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
class TimerCommand : public Command {
public:
	TimerCommand();
	virtual bool ExecuteCommand() override;
	virtual ISystemTimer* GetTimer();
	~TimerCommand();
private:
	ISystemTimer* timer = nullptr;
};