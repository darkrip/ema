#ifndef __CUSTOM_PACKER_HPP__
#define __CUSTOM_PACKER_HPP__



#include "packer.hpp"
#include "list_command_handler.hpp"
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
	typedef std::shared_ptr<CustomPacker> Ptr;
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
	bool       m_searchUglyDirs;
	bool 	   m_unixPath;
	bool       m_debug;
	
//	VariableProcessor m_variableProcessor;
	console::ConsoleCommand m_isArchive;
	//List section	
	console::ConsoleCommand m_listCommand;
	console::ListCommandHandler m_listCommandHandler;
	//Extract section
	console::ConsoleCommand m_extractCommand;
	console::ConsoleCommand m_extractWithoutPathCommand;

	console::ConsoleCommand m_testCommand;
	console::ConsoleCommand m_deleteCommand;
	console::ConsoleCommand m_addCommand;
	console::ConsoleCommand m_moveCommand;

	String m_inputString;
};




}//pack
}//ema






#endif//__CUSTOM_PACKER_HPP__