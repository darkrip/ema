#ifndef __EMA_PACKER_HPP__
#define __EMA_PACKER_HPP__

#include "file_cache.hpp"
#include "pack_file.hpp"
#include <memory>

namespace ema
{
namespace pack
{





class PackerBase
{
public:
	typedef std::shared_ptr<PackerBase> Ref;
	virtual bool isCorrectFile(const FileName&, PackDataStream&) = 0;
	virtual PackFile::Ref open(const FileName&, PackDataStream&) = 0;
	virtual void Upgrade(FileCache& file, FileCache::LoadStatus newRequestedStatus, bool readOnly = true) = 0;
};






class PackerManager
{
public:
	typedef struct PACKER_ID_T{} *PACKER_ID;

	static PackerManager& getInstance();
	PACKER_ID registerPacker(PackerBase::Ref&);
	PackerBase::Ref findPacker(const FileName&, PackDataStream&);
};

class PackManager
{
};















}
}

#endif//__EMA_PACKER_HPP__