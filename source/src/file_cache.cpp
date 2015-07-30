#include "file_cache.hpp"

#include "packer.hpp"

#include <boost/foreach.hpp>

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


bool ema::pack::FileCache::isDirty()const
{
	if (m_dirty)
		return true;

	m_dirty =
		m_name != m_origName ||
		isDeleted()          ||
		m_attr.isDirty();

	if (!m_childs.empty() && !m_dirty)
	{
		BOOST_FOREACH(const ema::pack::FileCache::Ptr& fc, m_childs)
		{
			m_dirty = fc->m_name != fc->m_origName ||
				fc->isDeleted();
			if (m_dirty)
				break;
		}
	}

	return m_dirty;
}