#ifndef __EMA_PACKER_HPP__
#define __EMA_PACKER_HPP__


namespace ema
{
namespace pack
{

class VariablesList
{
};



class Command
{
};



class PackerBase
{
public:
	virtual bool isCorrectFile(const FileName&, PackDataStream&);
	virtual PackFileRef open(const FileName&, PackDataStream&);
	virtual void Upgrade(FileCache& file, FileCache::State newState, bool readOnly=true);
};




class CustomPacker
{
public:
	CustomPacker(const std::wstring& packerName);

	void addExtension(const FileName&);
	
	
private:
	typedef std::set<FileName> ExtensionList;
	typedef std::list<PackID>  PackerIdList;
		
		
	String		  m_name;	
	String		  m_description;
	ExtensionList m_extensionList;
	PackerIdList  m_ids;
	
	ErrorLevel m_errorLevel;
	bool 	   m_batchUnpack;
	bool	   m_skipEmpty;
	bool 	   m_skipDirsInFileList;
	bool 	   m_unixPath;
	
//List section	
	Command m_listCommand;
	String  m_startListParsing;
	String  m_endListParsing;
	String  m_ignoreStrings;
	ListFormat m_listFormat;
//Extract section
	Command m_extractCommand;
	Command m_extractWithoutPathCommand;
	
	Command m_testCommand;
	Command m_deleteCommand;
	Command m_addCommand;
	Command m_moveCommand;
};


class PackerManager
{
public:
	typedef struct PACKER_ID_T{} *PACKER_ID;

	static PackerManager& getInstance();
	PACKER_ID registerPacker(Packer::Ref&);
	Packer::Ref findPacker(const FileName&, PackDataStream&);	
};

class PackManager
{
};



class Pack
{
public:
	FileName getPackName()const;
	FileCache& getRoot();
	Packer::Ref getPacker();
};



class FileCache
{
public:
	FileName getName()const;
	bool     isFolder()const;
	
	FileAttr& getAttr();
	FileName getExtractedName();
	
	void setAutoUpgrade();
	bool isAutoUpgrade()const;
	
	void markDeleted();
	bool isDeleted()const;
	
	PackDataReadStream&  getReadDataStream();
	PackDataWriteStream& getWriteDataStream();	
	
	
protected:
	bool isDirty()const;
	void setDirty();	
	void clearDirty();
private:
	typedef std::vector<FileCache*> Childs;

	enum LoadStatus
	{
		StatusLoadEmpty, // Only Name and IsFolder
		StatusLoadAttrOnly,
		StatusLoadStream, 
		StatusLoadInFile,
		StatusMAX
	};

	PackRef  		m_pack;
	FileCacheRef	m_parent;
	FileName 		m_name;
	
	Status  		m_status;
	
	bool	 		m_isFolder;
	Childs  		m_childs;
};









}
}

#endif//__EMA_PACKER_HPP__