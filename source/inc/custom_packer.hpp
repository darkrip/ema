#ifndef __CUSTOM_PACKER_HPP__
#define __CUSTOM_PACKER_HPP__



#include "packer.hpp"
#include "list_command_handler.hpp"
#include "console_command.hpp"
#include "packer_id_list.hpp"
#include "variable_processor.hpp"

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
	typedef std::set<FileName> ExtensionList;
	typedef std::shared_ptr<CustomPacker> Ptr;
	
	bool isCorrectFile(const FileName&, PackDataStream&);
	PackFile::Ref open(const FileName&, PackDataStream&);
	void Upgrade(FileCache& file, FileCache::LoadStatus newRequestedStatus, bool readOnly = true);

protected:
	CustomPacker(StringRef packerName);
	void init();
private:
	friend class CustomPackerFactoryBase;
	enum CommandsId{ ciEmpty=0, ciIsArchive, ciList, ciExtract, ciExtractWithoutPath, ciTest, ciDelete, ciAdd, ciMove, ciLastCommand };

	void initVariables();
	int runCommand(CommandsId, var::ContextBase&, console::ConsoleCommandHandler& handler=console::EmptyHandler());

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
	
	var::VariableProcessor::Ptr m_variableProcessor;
	console::ConsoleCommand m_commands[ciLastCommand];
	console::ListCommandHandler m_listCommandHandler;
	String m_inputString;
};




}//pack
}//ema






#endif//__CUSTOM_PACKER_HPP__