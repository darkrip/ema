#ifndef __VARIABLE_PROCESSOR_HPP__
#define __VARIABLE_PROCESSOR_HPP__


#include "defs.hpp"

#include <memory>
#include <functional>
#include <map>
#include <list>

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

class VariableNotRegistred : public std::exception
{
public:
	VariableNotRegistred(VariableBase::NameRef){}
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
	ContextBase(VariableProcessorLPtr owner, LPtr parent);
	virtual ~ContextBase(){}
private:
	typedef std::map<Id, Getter> Data;

	VariableBase::Value internalGet(VariableBase::Id);
	LPtr m_parent;
	VariableProcessorLPtr m_owner;
	Data m_data;
};


class Context: public ContextBase
{
public:
	friend class ContextCreator;

	class Var
	{
	public:
		Var(VariableBase::NameRef name, const Getter& getter):
			m_name(name), m_getter(getter){}
		const VariableBase::Name& getName()const{ return m_name; }
		const Getter& getGetter()const{ return m_getter; }
	private:
		VariableBase::Name m_name;
		Getter             m_getter;
	};

	class ContextCreator
	{
	public:
		ContextCreator(VariableProcessorPtr owner, ContextBase::Ptr parent);
		ContextCreator& operator << (const Var& var){ addVar(var); return *this; }
		operator ContextBase::Ptr(){ return create(); }
	private:
		typedef std::list<Var> Vars;
		void addVar(const Var& var){ m_vars.push_back(var); }
		ContextBase::Ptr create();
		VariableProcessorPtr m_owner;
		ContextBase::Ptr     m_parent;
		Vars                 m_vars;
	};

	Context(VariableProcessorLPtr owner, LPtr parent):
		ContextBase(owner, parent){}
	static ContextCreator create(ContextBase::Ptr baseContext);
	static ContextCreator create(VariableProcessorPtr owner);
	
};







class EmptyContext : public ContextBase
{
public:
	EmptyContext(): ContextBase(VariableProcessorPtr(), ContextBase::Ptr()){}
};


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