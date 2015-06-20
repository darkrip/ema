#ifndef __CONSOLE_COMMAND_HPP__
#define __CONSOLE_COMMAND_HPP__

#include <defs.hpp>
#include <memory>


namespace ema
{
namespace console
{



class ConsoleCommand
{
public:
	typedef std::shared_ptr<ConsoleCommand> Ptr;
	typedef const ConsoleCommand&			Ref;
	ConsoleCommand(StringRef strCommand);

};


}//console
}//ema


#endif//__CONSOLE_COMMAND_HPP__