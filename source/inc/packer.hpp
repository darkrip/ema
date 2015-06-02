#ifndef __EMA_PACKER_HPP__
#define __EMA_PACKER_HPP__

namespace ema
{
namespace pack
{

typedef std::iowstream PackDataStream;
typedef std::wstring FileName;


class VariablesList
{
};



class Command
{
};



class Packer
{
public:
	Packer(const std::wstring& packerName);

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

};



class FileCache
{
public:

};









}
}

#endif//__EMA_PACKER_HPP__