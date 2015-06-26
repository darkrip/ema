#include "custom_packer.hpp"

#include "file_tools.hpp"

#include <boost/assert.hpp>
#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include <boost/locale.hpp>

using namespace ema;
using namespace pack;



CustomPacker::CustomPacker(StringRef packerName) : m_variableProcessor(new var::VariableProcessor), m_name(packerName)
{

}




void CustomPacker::init()
{
	initVariables();//Variable processor must be inited before commands

	BOOST_FOREACH( console::ConsoleCommand& cc, m_commands )
		cc.init(m_variableProcessor);

}



void CustomPacker::initVariables()
{
	BOOST_VERIFY(m_variableProcessor->registerVar<PackFile>(L"A", boost::bind(&PackFile::getPackFullPatch, _1)) != var::VariableProcessor::wrongIndex);
	BOOST_VERIFY(m_variableProcessor->registerVar<PackFile>(L"a", boost::bind(&hd::tools::shortPathCreator, boost::bind(&PackFile::getPackFullPatch, _1))) != var::VariableProcessor::wrongIndex);
	//TODO: maybe tmp folder must be unique for every command
	BOOST_VERIFY(m_variableProcessor->registerVar<PackFile>(L"W", boost::bind(&PackFile::getTmpFolder, _1)) != var::VariableProcessor::wrongIndex);
	//TODO: maybe should to move it into shared data
	BOOST_VERIFY(m_variableProcessor->registerVar<PackFile>(L"P", boost::bind(&PackFile::getPassword, _1)) != var::VariableProcessor::wrongIndex);

/*

	; %%R - Current archive folder.
		; %%L - Filelist name. Filelist is the file containing names of files
		; %%l - Filelist with short file names.
		; %%F - Names of one or more files to be processed. If all names
		; %%f - The name of a single file to process. The archiver will be
*/
}



bool CustomPacker::isCorrectFile(const FileName& fileName, PackDataStream& stream)
{
	FileName ext = hd::tools::extractFileExt(fileName);
	ext = boost::locale::to_lower(ext);

	if(!m_extensionList.empty() && m_extensionList.find(ext) == m_extensionList.end())
		return false;


	if(!m_ids.isCorrect(stream))
		return false;

	var::Context<console::ConsoleCommand> context(fileName);
	if(runCommand(ciIsArchive, context)!=0)
		return false;

	return true;
}



int CustomPacker::runCommand(CommandsId commandId, var::ContextBase& context, console::ConsoleCommandHandler& handler)
{
	int result = 0;
	SemaforeExecutorManager::getInstance().
		execute(pack, [&](){  result = m_commands[commandId].execute(context, handler);});	
	return result;
}