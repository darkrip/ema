#ifndef __EMA_PACKER_HPP__
#define __EMA_PACKER_HPP__

#include "file_cache.hpp"
#include "pack_file.hpp"
#include "singleton.hpp"
#include <memory>

namespace ema
{
namespace pack
{





class PackerBase
{
public:
	typedef std::shared_ptr<PackerBase> Ptr;
	virtual bool isCorrectFile(const FileName&, PackDataStream&) = 0;
	virtual PackFile::Ref open(const FileName&, PackDataStream&) = 0;
	virtual void Upgrade(FileCache& file, FileCache::LoadStatus newRequestedStatus, bool readOnly = true) = 0;
};






class PackerManager : public hd::tools::AutoSingleton<PackerManager>
{
public:
	typedef struct PACKER_ID_T{} *PACKER_ID;

	PACKER_ID registerPacker(PackerBase::Ptr&);
	PackerBase::Ptr findPacker(const FileName&, PackDataStream&);
};






}
}

#endif//__EMA_PACKER_HPP__