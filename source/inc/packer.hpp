#ifndef __EMA_PACKER_HPP__
#define __EMA_PACKER_HPP__

#include "file_cache.hpp"
#include "pack_file.hpp"
#include "singleton.hpp"
#include <memory>
#include <map>

namespace ema
{
namespace pack
{





class PackerBase
{
public:
	typedef std::shared_ptr<PackerBase> Ptr;
	typedef std::weak_ptr<PackerBase>   LPtr;
	void init(const LPtr& self){ m_self = self; }
	LPtr getSelf(){ return m_self; }


	virtual bool isCorrectFile(const FileName&, PackDataStream&) = 0;
	virtual PackFile::Ptr open(const FileName&, PackDataStream&) = 0;
	virtual void Upgrade(FileCache& file, FileCache::LoadStatus newRequestedStatus, bool readOnly = true) = 0;
private:
	LPtr m_self;
};






class PackerManager : public hd::tools::AutoSingleton<PackerManager>
{
public:
	typedef size_t PACKER_ID;
	typedef std::vector<FileName> ExtensionList;

	PACKER_ID registerPacker(PackerBase::Ptr&, const ExtensionList&);
	PackerBase::Ptr findPacker(const FileName&, PackDataStream&);
private:
	typedef std::vector<PackerBase::Ptr> Packers;
	typedef std::multimap<String, PACKER_ID> Extensions;

	Packers m_packers;
	Extensions m_extensions;
};






}
}

#endif//__EMA_PACKER_HPP__