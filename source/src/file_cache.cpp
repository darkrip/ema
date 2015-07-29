#include "file_cache.hpp"

#include "packer.hpp"

using namespace ema;
using namespace pack;




void ema::pack::FileCache::innerUpgrade(LoadStatus status)
{
	m_status = m_pack->getPacker()->Upgrade(*this, *this, status);
}

void ema::pack::FileCache::ckeckLevel(LoadStatus status)
{
	if(m_status < status && isAutoUpgrade())
		innerUpgrade(status);
	if(m_status < status)
		throw FileCacheUpgradeExeption();
}
