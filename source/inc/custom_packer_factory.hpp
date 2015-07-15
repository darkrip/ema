#ifndef __CUSTOM_PACKER_FACTORY_HPP__
#define __CUSTOM_PACKER_FACTORY_HPP__

#include "custom_packer.hpp"
#include "list_command_handler.hpp"
#include "console_command.hpp"
#include "packer_id_list.hpp"

#include "defs.hpp"

#include <iterator>

namespace ema
{
namespace pack
{



class CustomPackerFactoryBase
{
protected:
	void construct(StringRef packName);
	void registration();

	void setPackIdList(const PackerIdList& pil){ m_packer->m_ids = pil; }
	void setPackExtensions(StringListRef exts){ std::copy(exts.begin(), exts.end(), std::inserter(m_packer->m_extensionList, m_packer->m_extensionList.end())); }

	void setPackListCommandHandler(const console::ListCommandHandler& handler){ m_packer->m_listCommandHandler = handler; }

	void setPackIsArchiveCommand(var::ConsoleCommand::Ref command){ m_packer->m_commands[CustomPacker::ciIsArchive] = command; }
	void setPackListCommand(var::ConsoleCommand::Ref command){ m_packer->m_commands[CustomPacker::ciList] = command; }
	void setPackExtractCommand(var::ConsoleCommand::Ref command){ m_packer->m_commands[CustomPacker::ciExtract] = command; }
	void setPackExtractWithPathCommand(var::ConsoleCommand::Ref command){ m_packer->m_commands[CustomPacker::ciExtractWithoutPath] = command; }
	void setPackTestCommand(var::ConsoleCommand::Ref command){ m_packer->m_commands[CustomPacker::ciTest] = command; }
	void setPackDeleteCommand(var::ConsoleCommand::Ref command){ m_packer->m_commands[CustomPacker::ciDelete] = command; }
	void setPackAddCommand(var::ConsoleCommand::Ref command){ m_packer->m_commands[CustomPacker::ciAdd] = command; }
	void setPackMoveCommand(var::ConsoleCommand::Ref command){ m_packer->m_commands[CustomPacker::ciMove] = command; }


	void setPackInputString(StringRef inputString){ m_packer->m_inputString = inputString; }

	void setPackArchiverEncoding(StringRef encoding){ m_packer->m_encoding = encoding; }
	void setPackDescription(StringRef description){ m_packer->m_description = description; }
	void setArchiver(StringRef archiver){ m_packer->m_archiver = archiver; }
	void setPackIconPath(StringRef iconPath){ m_packer->m_iconPath = iconPath; }

	void setPackBatchUnpack(bool bacthUnpack){ m_packer->m_batchUnpack = bacthUnpack; }
	void setPackSkipEmpty(bool skipEmpty){ m_packer->m_skipEmpty = skipEmpty;  }
	void setPackSkipDirs(bool skipDirs){ m_packer->m_skipDirsInFileList = skipDirs; }
	void setPackSearchForUglyDirs(bool searchUglyDirs){ m_packer->m_searchUglyDirs = searchUglyDirs; }
	void setPackUnixPath(bool unixPath){ m_packer->m_unixPath = unixPath; }
	void setPackDebug(bool debug){ m_packer->m_debug = debug; }

private:
	CustomPacker::Ptr m_packer;
};





}//pack
}//ema


#endif//__CUSTOM_PACKER_FACTORY_HPP__