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
typedef std::shared_ptr<PackFile> PackRef;


class FileCache
{
public:
	enum LoadStatus
	{
		StatusLoadEmpty, // Only Name and IsFolder
		StatusLoadAttrOnly, // And attr's
		StatusLoadStream, // And read/write stream
		StatusLoadInFile, // Unpacked file exist in temporary folder
		StatusMAX
	};

	typedef std::shared_ptr<FileCache> Ref;

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
	PackRef  		m_pack;
	Ref				m_parent;
	FileName 		m_name;

	LoadStatus  	m_status;

	bool	 		m_isFolder;
	Childs  		m_childs;
};


}//pack
}//ema


#endif//__FILE_CACHE_HPP__