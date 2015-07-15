#include "variable_processor.hpp"

#include <boost/assert.hpp>
#include <boost/foreach.hpp>


using namespace ema;
using namespace var;



VariableBase::Value ContextBase::get(ContextBase::Id id, const ModificatorsList& mod_list, bool ignore_mod_for_empty)
{
	auto owner = getOwner();
	BOOST_ASSERT(!!owner);
	if(!owner)
		return VariableBase::Value();

	if(!isExist(id))
	{
		auto parent = getParent();
		if(!parent)
			throw VariableNotExistInCurrentContext(id, getOwner());
		return parent->get(id, mod_list, ignore_mod_for_empty);
	}

	VariableBase::Value result = internalGet(id);

	if(!ignore_mod_for_empty || !result.empty())
	{
		for(auto it = mod_list.begin(); it != mod_list.end(); ++it)
			owner->getModificator(*it).modify(result, id, *this);
	}
	return result;
}


bool ContextBase::registerGetter(ContextBase::Id id, const Getter& getter)
{
	BOOST_ASSERT(!isExist(id));
	m_data[id] = getter;
	return true;
}


bool ContextBase::isExist(ContextBase::Id id)const
{
	return m_data.find(id)!=m_data.end();
}


VariableBase::Value ContextBase::internalGet(ContextBase::Id id)
{
	BOOST_ASSERT(isExist(id));
	return m_data[id]();
}




Context::ContextCreator Context::create(ContextBase::Ptr baseContext)
{
	BOOST_ASSERT(!!baseContext);
	return ContextCreator(baseContext->getOwner(), baseContext);
}

Context::ContextCreator Context::create(VariableProcessorPtr owner)
{
	BOOST_ASSERT(!!owner);
	return ContextCreator(owner, ContextBase::Ptr());
}

ContextBase::Ptr Context::ContextCreator::create()
{
	std::shared_ptr<Context> context(new Context(m_owner, m_parent));

	BOOST_FOREACH( const Var& var, m_vars )
	{
		VariableBase::Id id = m_owner->findVar(var.getName());
		if(id == VariableProcessor::wrongIndex)
			throw VariableNotRegistred(var.getName());
		context->registerGetter(id, var.getGetter());
	}

	return context;
}