#ifndef __FILE_CACHE_HPP__
#define __FILE_CACHE_HPP__

#include "file_attr.hpp"
#include "shared_data.hpp"

#include "defs.hpp"
#include <vector>
#include <memory>
#include <exception>

namespace ema
{
namespace pack
{

class PackFile;
class FileCache;

typedef std::shared_ptr<PackFile>  PackFilePtr;
typedef std::shared_ptr<FileCache> FileCachePtr;
typedef std::vector<FileCachePtr>  FileCacheChilds;




struct FileCacheData
{
	FileCacheData(const FileCacheData& ref) :
		m_pack(ref.m_pack), m_parent(ref.m_parent), m_name(ref.m_name), m_isFolder(ref.m_isFolder), m_childs(ref.m_childs){}

	FileCacheData(PackFilePtr pack, FileCachePtr parent, FileNameRef name, bool isFolder, const FileCacheChilds& childs) :
		m_pack(pack), m_parent(parent), m_name(name), m_isFolder(isFolder), m_childs(childs){}

	PackFilePtr	                m_pack;
	FileCachePtr				m_parent;
	FileName 		            m_name;
	FileName                    m_origName;
	bool	 	                m_isFolder;
	bool                        m_isDeleted;
	FileCacheChilds  		    m_childs;
	attr::FileAttr              m_attr;
};


class FileCacheUpgradeExeption : public std::exception
{

};


class FileCache : private  FileCacheData, public SharedDataStorage
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

	FileCache(const FileCacheData& data, LoadStatus status) : FileCacheData(data), m_status(status), m_autoUpgrade(false){}

	FileName getName(){ ckeckLevel(SratusLoadName); return m_name; }
	bool     isFolder(){ ckeckLevel(SratusLoadName); return m_isFolder; }

	attr::FileAttr& getAttr();
	FileName getExtractedName();

	void setAutoUpgrade(bool autoUpgrade){ m_autoUpgrade = autoUpgrade; }
	bool isAutoUpgrade()const{ return m_autoUpgrade; }

	void markDeleted();
	bool isDeleted()const{ return m_isDeleted; }

	PackDataReadStream&  getReadDataStream();
	PackDataWriteStream& getWriteDataStream();

	Childs& getChilds(){ ckeckLevel(StatusLoadStream); return m_childs; }
	Ptr getParent();

	PackFilePtr getPackFile()const{ return m_pack; }

	LoadStatus getStatus()const{ return m_status; }

	bool isDirty()const;

protected:
	void innerUpgrade(LoadStatus status);
	void ckeckLevel(LoadStatus status);
	void setDirty(){ m_dirty = true; }
	void clearDirty(){ m_dirty = false; }
	
private:
	FileCache();
	FileCache(const FileCache&);
	const FileCache& operator=(const FileCache&);

	LoadStatus  	m_status;
	bool            m_autoUpgrade;
	mutable bool    m_dirty;
};


}//pack
}//ema


#endif//__FILE_CACHE_HPP__