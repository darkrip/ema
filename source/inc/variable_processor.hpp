#ifndef __VARIABLE_PROCESSOR_HPP__
#define __VARIABLE_PROCESSOR_HPP__


#include <memory>


namespace ema
{
namespace console
{

typedef String VariableValue;

class ContextBase
{
public:
	VariableValue getVariable


};



template<typename ContextHolder>
class Context : public ContextBase
{

};



class VariableProcessor
{
public:
	typedef std::shared_ptr<VariableProcessor> Ptr;
	typedef size_t VariableId;
	typedef String VariableName;
	typedef StringRef VariableNameRef;

	void init();

	template< typename ContextHolder >
	VariableId addVariable(VariableNameRef, );
	




};








}//console
}//ema








#endif//__VARIABLE_PROCESSOR_HPP__