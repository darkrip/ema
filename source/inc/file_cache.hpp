#ifndef __FILE_CACHE_HPP__
#define __FILE_CACHE_HPP__

#include "file_attr.hpp"

#include "defs.hpp"
#include <vector>
#include <memory>

namespace ema
{
namespace pack
{

class PackFile;
typedef std::shared_ptr<PackFile>  PackRef;
typedef std::shared_ptr<FileCache> FileCachePtr;
typedef std::vector<FileCachePtr>  FileCacheChilds;




struct FileCacheData
{
	PackRef  	                m_pack;
	FileCachePtr				m_parent;
	FileName 		            m_name;
	bool	 	                m_isFolder;
	FileCacheChilds  		    m_childs;
};



class FileCache : private  FileCacheData
{
public:
	enum LoadStatus
	{
		StatusLoadEmpty, 
		SratusLoadName, 	// Only Name and IsFolder
		StatusLoadAttrOnly, // And attr's
		StatusLoadStream,   // And read/write stream, childs list avalible
		StatusLoadInFile,   // Unpacked file exist in temporary folder
		StatusMAX
	};
	typedef std::shared_ptr<FileCache> Ptr;
	typedef std::vector<Ptr>           Childs;

	FileCache(const FileCacheData& data, LoadStatus status): FileCacheData(data), m_status(status){}

	FileName getName()const{ ckeckLevel(SratusLoadName); return m_name; }
	bool     isFolder()const;

	attr::FileAttr& getAttr();
	FileName getExtractedName();

	void setAutoUpgrade();
	bool isAutoUpgrade()const;

	void markDeleted();
	bool isDeleted()const;

	PackDataReadStream&  getReadDataStream();
	PackDataWriteStream& getWriteDataStream();

	Childs& getChilds();
	Ptr getParent();


protected:
	void innerUpgrade(LoadStatus);
	void ckeckLevel(LoadStatus);
	bool isDirty()const;
	void setDirty();
	void clearDirty();
private:
	FileCache();
	FileCache(const FileCache&);
	const FileCache& operator=(const FileCache&);

	LoadStatus  	m_status;
};


}//pack
}//ema


#endif//__FILE_CACHE_HPP__