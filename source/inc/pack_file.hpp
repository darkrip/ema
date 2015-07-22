#ifndef __PACK_FILE_HPP__
#define __PACK_FILE_HPP__


#include "shared_data.hpp"
#include "cache_controller.hpp"
#include "file_cache.hpp"
#include "defs.hpp"

namespace ema
{
namespace pack
{

class PackerBase;
typedef std::shared_ptr<PackerBase> PackerBasePtr;


class PackFile : public SharedDataStorage
{
public:
	typedef std::shared_ptr<PackFile> Ptr;

	PackFile(FileNameRef fullFileName, PackerBasePtr packer, const CacheController::Ptr& cache_controller);

	FileName getPackName()const;
	FileName getPackFullPatch()const;
	FileName getTmpFolder()const;
	FileName getPassword()const;
	FileCache::Ptr getRoot(){ return m_root; }
	PackerBasePtr getPacker();
private:
	FileCache::Ptr       m_root;
	FileName             m_fullName;
	PackerBasePtr        m_packer;
	CacheController::Ptr m_cacheConstoller;
};


}//pack
}//ema





#endif//__PACK_FILE_HPP__