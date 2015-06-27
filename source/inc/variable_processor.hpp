#ifndef __VARIABLE_PROCESSOR_HPP__
#define __VARIABLE_PROCESSOR_HPP__


#include "defs.hpp"

#include <memory>


namespace ema
{
namespace var
{



class VariableBase
{
public:
	typedef size_t Id;
	typedef String Name;
	typedef String Value;
	typedef StringRef NameRef;
};



template<typename ContextHolder>
class Variable : public VariableBase
{
};

class VariableModificatorBase
{
public:
	typedef size_t Id;
};


typedef std::vector<VariableModificatorBase::Id> ModificatorsList;


class ContextBase
{
public:
	virtual VariableBase::Value get(VariableBase::Id, const ModificatorsList& mod_list=ModificatorsList(), bool ignore_mod_for_empty=true)=0;
};


class EmptyContext : public ContextBase
{
public:
	virtual VariableBase::Value get(VariableBase::Id, const ModificatorsList& mod_list = ModificatorsList(), bool ignore_mod_for_empty = true){ return L""; }
};


template<typename ContextHolder>
class Context : public ContextBase
{
public:
};




class VariableProcessor
{
public:
	typedef std::shared_ptr<VariableProcessor> Ptr;
	static const VariableBase::Id wrongIndex = (VariableBase::Id)-1;

	void init();

	template< typename ContextHolder, typename Getter >
	VariableBase::Id registerVar(VariableBase::NameRef, const Getter& );
	VariableBase::Id findVar(VariableBase::NameRef);
	const VariableBase&      getVariable(VariableBase::Id);
};
//-----------------------------------------------------------------------------------

template< typename ContextHolder, typename Getter >
VariableBase::Id VariableProcessor::registerVar(VariableBase::NameRef name, const Getter& getter )
{
	return 	wrongIndex;
}









}//var
}//ema








#endif//__VARIABLE_PROCESSOR_HPP__