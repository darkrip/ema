#include "custom_packer_factory.hpp"


#include <cassert>


using namespace ema;
using namespace pack;






void CustomPackerFactoryBase::construct(StringRef packName)
{
	assert(!m_packer);
	m_packer.reset(new CustomPacker(packName));
}

void CustomPackerFactoryBase::registration()
{
	if (!m_packer)
	{
		assert(0);
		return;
	}

	ema::pack::PackerBase::Ptr ptr = m_packer;
	m_packer->init(ptr);
	PackerManager::ExtensionList extensions(m_packer->getExtensions().begin(), m_packer->getExtensions().end());
	m_packer.reset();
	PackerManager::getInstance().registerPacker(ptr, extensions);
}
