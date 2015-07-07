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

class ConsoleCommandLinesHandler : public ConsoleCommandHandler
{
protected:
	virtual String readLine()=0;
	virtual void writeLine(StringRef) = 0;
	void flush();
	~ConsoleCommandLinesHandler(){ flush(); }
private:
	virtual void output(StringRef);
	virtual String input();

	String m_readCache;
	String m_writeCache;
};



class ConsoleTerm
{
public:
	typedef unsigned long CommandResult;
	ConsoleTerm();
	void init(StringRef locale, StringListRef additional_path, bool is_debug, int max_exec_time);
	CommandResult execute(StringRef command, StringRef work_dir, ConsoleCommandHandler& hadler = EmptyHandler());
private:
	std::shared_ptr<ConsoleTermData> m_data;
	static ConsoleAsyncManager       m_asyncManager;
	StringList					     m_additionalPath;
};





}//console
}//ema



#endif // __CONSOLE_TERM_HPP__
