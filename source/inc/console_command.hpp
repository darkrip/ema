#ifndef __CONSOLE_COMMAND_HPP__
#define __CONSOLE_COMMAND_HPP__

#include "variable_processor.hpp"

#include <defs.hpp>
#include <memory>


namespace ema
{
namespace console
{


class ConsoleCommandHandler
{

};

class EmptyHandler : public ConsoleCommandHandler
{

};



class ConsoleCommand
{
public:
	typedef std::shared_ptr<ConsoleCommand> Ptr;
	typedef const ConsoleCommand&			Ref;
	ConsoleCommand();
	ConsoleCommand(StringRef strCommand);
	void init(var::VariableProcessor::Ptr);

};


}//console
}//ema


#endif//__CONSOLE_COMMAND_HPP__