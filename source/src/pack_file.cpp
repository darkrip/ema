#include "pack_file.hpp"


using namespace ema;
using namespace pack;

PackFile::PackFile(FileNameRef fullFileName, PackerBasePtr packer, const CacheController::Ptr& cache_controller) :
m_fullName(fullFileName),
m_packer(packer),
m_cacheConstoller(cache_controller)
{
}
