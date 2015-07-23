#include "pack_file.hpp"


using namespace ema;
using namespace pack;

PackFile::PackFile(FileNameRef fullFileName, PackerBasePtr packer, const CacheController::Ptr& cache_controller) :
m_fullName(fullFileName),
m_packer(packer),
m_cacheConstoller(cache_controller)
{
}

void ema::pack::PackFile::init(LPtr ptr)
{
	m_self = ptr;
	FileCacheData fileData={
		getSelf(),
		FileCachePtr(),
		L"\\",
		true,
		FileCacheChilds()
	};
	m_root.reset(new FileCache(fileData, FileCache::SratusLoadName));
}
