#ifndef __CUSTOM_PACKER_HPP__
#define __CUSTOM_PACKER_HPP__



#include "packer.hpp"
#include "list_command_handler.hpp"
#include "console_command.hpp"
#include "console_term.hpp"
#include "packer_id_list.hpp"
#include "variable_processor.hpp"
#include "cache_controller.hpp"
#include "status_chain.hpp"

#include "defs.hpp"

#include <set>
#include <list>


namespace ema
{
namespace pack
{

typedef StatusChain < FileCache::LoadStatus, FileCache::LoadStatus(FileCache&, FileCacheData&, bool)> PackerStatusChain;


class CustomPacker : public PackerBase
{
public:
	typedef std::set<FileName> ExtensionList;
	typedef std::shared_ptr<CustomPacker> Ptr;
	
	bool isCorrectFile(const FileName&, PackDataStream&);
	PackFile::Ptr open(const FileName&, PackDataStream&);
	FileCache::LoadStatus Upgrade(FileCache& file, FileCacheData& fileData, FileCache::LoadStatus newRequestedStatus, bool readOnly = true);

	const ExtensionList& getExtensions()const{ return m_extensionList; }


protected:
	CustomPacker(StringRef packerName);
	void init(const LPtr& self);
private:
	friend class CustomPackerFactoryBase;
	enum CommandsId{ ciEmpty=0, ciIsArchive, ciList, ciExtract, ciExtractWithoutPath, ciTest, ciDelete, ciAdd, ciMove, ciLastCommand };

	void initVariables();
	int runCommand(CommandsId, var::ContextBase&, console::ConsoleCommandHandler& handler=console::EmptyHandler());
	var::ContextBase::Ptr& getContext(){ return m_context; }

	FileCache::LoadStatus loadFileName(FileCache& file, FileCacheData& fileData, bool readOnly);
	FileCache::LoadStatus unloadFileName(FileCache& file, FileCacheData& fileData, bool readOnly);

	FileCache::LoadStatus loadFileAttr(FileCache& file, FileCacheData& fileData, bool readOnly);
	FileCache::LoadStatus unloadFileAttr(FileCache& file, FileCacheData& fileData, bool readOnly);

	FileCache::LoadStatus loadFileStream(FileCache& file, FileCacheData& fileData, bool readOnly);
	FileCache::LoadStatus unloadFileStream(FileCache& file, FileCacheData& fileData, bool readOnly);

	FileCache::LoadStatus loadFileToTmp(FileCache& file, FileCacheData& fileData, bool readOnly);
	FileCache::LoadStatus unloadFileFromTmp(FileCache& file, FileCacheData& fileData, bool readOnly);

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
	var::ConsoleCommand m_commands[ciLastCommand];
	console::ListCommandHandler m_listCommandHandler;
	String m_inputString;

	console::ConsoleTerm  m_console;
	var::ContextBase::Ptr m_context;
	CacheController       m_cacheController;
	PackerStatusChain     m_statusChain;
};




}//pack
}//ema






#endif//__CUSTOM_PACKER_HPP__