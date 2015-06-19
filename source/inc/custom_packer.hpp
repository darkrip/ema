#ifndef __CUSTOM_PACKER_HPP__
#define __CUSTOM_PACKER_HPP__



#include "packer.hpp"
#include "console_command.hpp"
#include "packer_id_list.hpp"
#include "defs.hpp"
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
	typedef std::shared_ptr<CustomPacker> Ref;
	friend class CustomPackerFactoryBase;

	String		  m_name;
	String		  m_description;
	ExtensionList m_extensionList;
	PackerIdList  m_ids;
	String        m_encoding;
	String        m_archiver;
	String        m_iconPath;

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