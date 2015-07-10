#ifndef __CONSOLE_COMMAND_HPP__
#define __CONSOLE_COMMAND_HPP__

#include "variable_processor.hpp"

#include "defs.hpp"

#include <boost/interprocess/sync/interprocess_mutex.hpp>


#include <memory>


namespace ema
{
namespace var
{

class ConsoleCommand
{
public:
	typedef std::shared_ptr<ConsoleCommand> Ptr;
	typedef const ConsoleCommand&			Ref;
	ConsoleCommand();
	ConsoleCommand(StringRef strCommand);
	void init(var::VariableProcessor::Ptr);
	void init();
	String str(var::ContextBase& context = var::EmptyContext());
	String work_dir(var::ContextBase& context = var::EmptyContext());
};


class CommandContext : public ContextBase
{
public:
	CommandContext(FileNameRef);
};
}//var
}//ema


#endif//__CONSOLE_COMMAND_HPP__