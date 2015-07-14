#ifndef __VARIABLE_PROCESSOR_HPP__
#define __VARIABLE_PROCESSOR_HPP__


#include "defs.hpp"

#include <memory>
#include <functional>
#include <map>

namespace ema
{
namespace var
{

class VariableProcessor;
class ContextBase;


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
	typedef std::shared_ptr<VariableModificatorBase> Ptr;
	typedef const VariableModificatorBase&           Ref;
	typedef size_t Id;
	virtual void modify(VariableBase::Value, VariableBase::Id, const ContextBase&)const=0;
};


typedef std::vector<VariableModificatorBase::Id> ModificatorsList;
typedef std::weak_ptr<VariableProcessor> VariableProcessorLPtr;
typedef std::shared_ptr<VariableProcessor> VariableProcessorPtr;


class VariableNotExistInCurrentContext : public std::exception
{
public:
	VariableNotExistInCurrentContext(VariableBase::Id, const VariableProcessorPtr&){}
};


class ContextBase
{
public:
	typedef std::shared_ptr<ContextBase> Ptr;
	typedef std::weak_ptr<ContextBase>   LPtr;
	typedef std::function<String(void)>    Getter;
	typedef VariableBase::Id             Id;

	VariableBase::Value get(Id, const ModificatorsList& mod_list=ModificatorsList(), bool ignore_mod_for_empty=true);
	ContextBase::Ptr getParent(){ return m_parent.lock(); }
	VariableProcessorPtr getOwner(){ return m_owner.lock(); }
	bool isExist(Id)const;
protected:
	bool registerGetter(Id, const Getter&);
private:
	typedef std::map<Id, Getter> Data;

	VariableBase::Value internalGet(VariableBase::Id);
	LPtr m_parent;
	VariableProcessorLPtr m_owner;
	Data m_data;
};


template<typename Obj>
class Context: public ContextBase
{
public:
	ContextBase::Ptr create();
	
};







class EmptyContext : public ContextBase
{};


class VariableProcessor
{
public:
	typedef std::shared_ptr<VariableProcessor> Ptr;
	static const VariableBase::Id wrongIndex = (VariableBase::Id)-1;

	void init();

	VariableBase::Id    registerVar(VariableBase::NameRef);
	VariableBase::Id    findVar(VariableBase::NameRef);

	VariableModificatorBase::Id registerModificator(VariableBase::NameRef, VariableModificatorBase::Ptr);
	VariableModificatorBase::Id findModificator(VariableBase::NameRef);
	VariableModificatorBase::Ref getModificator(VariableModificatorBase::Id);	
};
//-----------------------------------------------------------------------------------




}//var
}//ema








#endif//__VARIABLE_PROCESSOR_HPP__