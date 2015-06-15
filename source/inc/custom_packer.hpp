#ifndef __CUSTOM_PACKER_HPP__
#define __CUSTOM_PACKER_HPP__


#include "defs.hpp"
#include "packer.hpp"
#include "console_command.hpp"
#include <set>
#include <list>


namespace ema
{
namespace pack
{


class CustomPacker : public PackerBase
{
public:
	CustomPacker(StringRef packerName);

private:
	typedef std::set<FileName> ExtensionList;
//	typedef std::list<PackID>  PackerIdList;
	friend class CustomPackerFactoryBase;

	String		  m_name;
	String		  m_description;
	ExtensionList m_extensionList;
//	PackerIdList  m_ids;

//	ErrorLevel m_errorLevel;
	bool 	   m_batchUnpack;
	bool	   m_skipEmpty;
	bool 	   m_skipDirsInFileList;
	bool 	   m_unixPath;

//	VariableProcessor m_variableProcessor;
	//List section	
	ConsoleCommand m_listCommand;
	String  m_startListParsing;
	String  m_endListParsing;
	String  m_ignoreStrings;
//	ListFormat m_listFormat;
	//Extract section
	ConsoleCommand m_extractCommand;
	ConsoleCommand m_extractWithoutPathCommand;

	ConsoleCommand m_testCommand;
	ConsoleCommand m_deleteCommand;
	ConsoleCommand m_addCommand;
	ConsoleCommand m_moveCommand;
};




}//pack
}//ema






#endif//__CUSTOM_PACKER_HPP__