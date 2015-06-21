#include "custom_packer.hpp"



using namespace ema;
using namespace pack;



CustomPacker::CustomPacker(StringRef packerName) : m_variableProcessor(new console::VariableProcessor), m_name(packerName)
{

}




void CustomPacker::init()
{
	initVariables();//Variable processor must be inited before commands
	//TODO: Can be create commands storage?
	m_isArchive.init(m_variableProcessor);
	m_listCommand.init(m_variableProcessor);
	m_extractCommand.init(m_variableProcessor);
	m_extractWithoutPathCommand.init(m_variableProcessor);
	m_testCommand.init(m_variableProcessor);
	m_deleteCommand.init(m_variableProcessor);
	m_addCommand.init(m_variableProcessor);
	m_moveCommand.init(m_variableProcessor);
}



void CustomPacker::initVariables()
{


}