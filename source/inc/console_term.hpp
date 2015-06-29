#ifndef __CONSOLE_TERM_HPP__
#define __CONSOLE_TERM_HPP__

#include "defs.hpp"
#include <memory>

namespace ema
{
namespace console
{


struct ConsoleTermData;
class ConsoleAsyncManager;

class RunCommandExpection : public std::exception
{

};


class ConsoleCommandHandler
{
public:
	virtual void output(StringRef) = 0;
	virtual String input() = 0;
};

class EmptyHandler : public ConsoleCommandHandler
{
	virtual void output(StringRef){}
	virtual String input(){ return L""; }
};

class ConsoleTerm
{
public:
	typedef int	CommandResult;
	ConsoleTerm();
	void init(StringRef locale, bool is_debug, int max_exec_time);
	CommandResult execute(StringRef command, ConsoleCommandHandler& hadler = EmptyHandler());
private:
	std::shared_ptr<ConsoleTermData> m_data;
	static ConsoleAsyncManager m_asyncManager;
};





}//console
}//ema



#endif // __CONSOLE_TERM_HPP__
