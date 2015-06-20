#ifndef __LIST_COMMAND_HANDLER_HPP__
#define __LIST_COMMAND_HANDLER_HPP__

#include "defs.hpp"


namespace ema
{
namespace console
{


class ListCommandHandler
{
public:
	ListCommandHandler(StringListRef format, StringRef start, StringRef end, StringListRef ignoreStrings);

};



}//console
}//ema



#endif