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




void CustomPacker::init(const LPtr& self)
{
	PackerBase::init(self);
	initVariables();//Variable processor must be inited before commands

	BOOST_FOREACH( var::ConsoleCommand& cc, m_commands )
		cc.init(m_variableProcessor);

	m_statusChain.init(
		PackerStatusChain::Creator() 
		<< PackerStatusChain::Item(FileCache::LoadStatus::SratusLoadName,     boost::bind(&CustomPacker::loadFileName,   this, _1, _2, _3), boost::bind(&CustomPacker::unloadFileName, this,    _1, _2, _3))
		<< PackerStatusChain::Item(FileCache::LoadStatus::StatusLoadAttrOnly, boost::bind(&CustomPacker::loadFileAttr,   this, _1, _2, _3), boost::bind(&CustomPacker::unloadFileAttr, this,    _1, _2, _3))
		<< PackerStatusChain::Item(FileCache::LoadStatus::StatusLoadStream,   boost::bind(&CustomPacker::loadFileStream, this, _1, _2, _3), boost::bind(&CustomPacker::unloadFileStream, this,  _1, _2, _3))
		<< PackerStatusChain::Item(FileCache::LoadStatus::StatusLoadInFile,   boost::bind(&CustomPacker::loadFileToTmp,  this, _1, _2, _3), boost::bind(&CustomPacker::unloadFileFromTmp, this, _1, _2, _3))
		);
}

PackFile::Ptr CustomPacker::open(const FileName& name, PackDataStream&)
{
	PackFile::Ptr pack_file(
		new PackFile(name, getSelf().lock(), m_cacheController.createSub()));
	pack_file->init(pack_file);
	return pack_file;
}

void CustomPacker::initVariables()
{
/*
BOOST_VERIFY(m_variableProcessor->registerVar<PackFile>(L"A", boost::bind(&PackFile::getPackFullPatch, _1)) != var::VariableProcessor::wrongIndex);
BOOST_VERIFY(m_variableProcessor->registerVar<PackFile>(L"a", boost::bind(&hd::tools::shortPathCreator, boost::bind(&PackFile::getPackFullPatch, _1))) != var::VariableProcessor::wrongIndex);
//TODO: maybe tmp folder must be unique for every command
BOOST_VERIFY(m_variableProcessor->registerVar<PackFile>(L"W", boost::bind(&PackFile::getTmpFolder, _1)) != var::VariableProcessor::wrongIndex);
//TODO: maybe should to move it into shared data
BOOST_VERIFY(m_variableProcessor->registerVar<PackFile>(L"P", boost::bind(&PackFile::getPassword, _1)) != var::VariableProcessor::wrongIndex);

*/



	BOOST_VERIFY(m_variableProcessor->registerVar(L"A") != var::VariableProcessor::wrongIndex);
	BOOST_VERIFY(m_variableProcessor->registerVar(L"a") != var::VariableProcessor::wrongIndex);
	//TODO: maybe tmp folder must be unique for every command
	BOOST_VERIFY(m_variableProcessor->registerVar(L"W") != var::VariableProcessor::wrongIndex);
	//TODO: maybe should to move it into shared data
	BOOST_VERIFY(m_variableProcessor->registerVar(L"P") != var::VariableProcessor::wrongIndex);

/*

	; %%R - Current archive folder.
	; %%L - Filelist name. Filelist is the file containing names of files
	; %%l - Filelist with short file names.
	; %%F - Names of one or more files to be processed. If all names
	; %%f - The name of a single file to process. The archiver will be
*/
}


FileCache::LoadStatus CustomPacker::loadFileName(FileCache& file, FileCacheData& fileData, bool readOnly)
{
	//TODO we can't load file afte full unload
	return FileCache::StatusLoadEmpty;
		/*		,
		SratusLoadName, 	// Only Name and IsFolder
		StatusLoadAttrOnly, // And attr's
		StatusLoadStream,   // And read/write stream, childs list avalible
		StatusLoadInFile,   // Unpacked file exist in temporary folder*/
}

FileCache::LoadStatus CustomPacker::unloadFileName(FileCache& file, FileCacheData& fileData, bool readOnly)
{
	if (!readOnly)
	{
		if (file.isDeleted())
			deleteFile(file, fileData);
		else if (file.isDirty())
			renameFile(file, fileData);
	}
	fileData.m_name.clear();
	fileData.m_parent.reset();

	return FileCache::StatusLoadEmpty;
}

FileCache::LoadStatus CustomPacker::loadFileAttr(FileCache& file, FileCacheData& fileData, bool readOnly)
{
	return FileCache::StatusLoadEmpty;
}

FileCache::LoadStatus CustomPacker::unloadFileAttr(FileCache& file, FileCacheData& fileData, bool readOnly)
{
	return FileCache::StatusLoadEmpty;
}

FileCache::LoadStatus CustomPacker::loadFileStream(FileCache& file, FileCacheData& fileData, bool readOnly)
{
	if (file.isFolder())
		return loadDirContent(file, fileData);
	else
		return loadFileToTmp(file, fileData, readOnly);
}

FileCache::LoadStatus CustomPacker::unloadFileStream(FileCache& file, FileCacheData& fileData, bool readOnly)
{
	return FileCache::StatusLoadEmpty;
}

FileCache::LoadStatus CustomPacker::loadFileToTmp(FileCache& file, FileCacheData& fileData, bool readOnly)
{
	return FileCache::StatusLoadEmpty;
}

FileCache::LoadStatus CustomPacker::unloadFileFromTmp(FileCache& file, FileCacheData& fileData, bool readOnly)
{
	return FileCache::StatusLoadEmpty;
}



FileCache::LoadStatus CustomPacker::deleteFile(FileCache& file, FileCacheData& fileData)
{
	if (!isImplemented(ciMove))
		throw CustomPackerOperationNotImplemented(ciMove, getSelf().lock(), file.getPackFile());

	//TODO
	FileName fileName = fileData.m_origName;

	var::ContextBase::Ptr context = var::Context::create(file.getSharedData<var::ContextBase::Ptr>())
		<< var::Context::Var(L"A", [fileName]()->String{ return fileName; })
		<< var::Context::Var(L"a", boost::bind(&hd::tools::shortPathCreator, fileName));

	return FileCache::StatusLoadEmpty;
}

FileCache::LoadStatus CustomPacker::renameFile(FileCache& file, FileCacheData& fileData)
{
	return FileCache::StatusLoadEmpty;
}

FileCache::LoadStatus CustomPacker::loadDirContent(FileCache& file, FileCacheData& fileData)
{
	return FileCache::StatusLoadEmpty;
}



bool CustomPacker::isCorrectFile(const FileName& fileName, PackDataStream& stream)
{
	FileName ext = hd::tools::extractFileExt(fileName);
	ext = boost::locale::to_lower(ext);

	if(!m_extensionList.empty() && m_extensionList.find(ext) == m_extensionList.end())
		return false;


	if(!m_ids.isCorrect(stream))
		return false;

	var::ContextBase::Ptr context = var::Context::create(getContext())
		<< var::Context::Var(L"A", [fileName]()->String{ return fileName; } )
		<< var::Context::Var(L"a", boost::bind(&hd::tools::shortPathCreator, fileName))
		<< var::Context::Var(L"W", boost::bind(&CacheController::getCacheFolder, m_cacheController.createSub()));
	
	if(runCommand(ciIsArchive, *context)!=0)
		return false;

	return true;
}

int CustomPacker::runCommand(CommandsId commandId, var::ContextBase& context, console::ConsoleCommandHandler& handler)
{
	int result = 0;
	String str_command = m_commands[commandId].str(context);
	String str_work_dir =m_commands[commandId].work_dir();
	result = m_console.execute(str_command, str_work_dir, handler);
	return result;
}

FileCache::LoadStatus ema::pack::CustomPacker::Upgrade(FileCache& file, FileCacheData& fileData, FileCache::LoadStatus newRequestedStatus, bool readOnly /*= true*/)
{
	FileCache::LoadStatus oldStatus = file.getStatus();
	FileCache::LoadStatus realStatus = m_statusChain.run(newRequestedStatus, oldStatus, file, fileData, readOnly);
	return realStatus;
}
