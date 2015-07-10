#include "variable_processor.hpp"

#include <boost/assert.hpp>


using namespace ema;
using namespace var;



VariableBase::Value ContextBase::get(VariableBase::Id id, const ModificatorsList& mod_list, bool ignore_mod_for_empty)
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
			owner->getModificator(*it)->modify(result, id, *this);
	}
	return result;
}