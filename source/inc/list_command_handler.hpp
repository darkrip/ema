#ifndef __LIST_COMMAND_HANDLER_HPP__
#define __LIST_COMMAND_HANDLER_HPP__


#include "console_term.hpp"

#include "defs.hpp"


namespace ema
{
namespace console
{


class ListCommandHandler : public ConsoleCommandLinesHandler
{
public:
	ListCommandHandler()0
	ListCommandHandler(StringListRef format, StringRef start, StringRef end, StringListRef ignoreStrings);
protected:
	virtual String readLine();
	virtual void writeLine(StringRef);
};



}//console
}//ema



#endif