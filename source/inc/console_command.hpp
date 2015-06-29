#ifndef __CONSOLE_COMMAND_HPP__
#define __CONSOLE_COMMAND_HPP__

#include "variable_processor.hpp"

#include "defs.hpp"

#include <boost/interprocess/sync/interprocess_mutex.hpp>


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
	ConsoleCommand();
	ConsoleCommand(StringRef strCommand);
	void init(var::VariableProcessor::Ptr);
	void init();
	String str(var::ContextBase& context = var::EmptyContext());
};



}//console
namespace var
{
template<>
class Context<console::ConsoleCommand> : public ContextBase
{
public:
	Context(FileNameRef);
	virtual VariableBase::Value get(VariableBase::Id, const ModificatorsList& mod_list = ModificatorsList(), bool ignore_mod_for_empty = true){ return L""; }
};
}//var
}//ema


#endif//__CONSOLE_COMMAND_HPP__