#include "packer.hpp"

#include "file_tools.hpp"

#include <boost/assert.hpp>
#include <boost/algorithm/string.hpp>    

using namespace ema;
using namespace pack;



PackerManager::PACKER_ID PackerManager::registerPacker(PackerBase::Ptr& packer, const ExtensionList& extensions)
{
	BOOST_ASSERT(!extensions.empty());

	m_packers.push_back(packer);
	PACKER_ID id = m_packers.size() - 1;

	for (auto it = extensions.begin(); it != extensions.end(); ++it)
	{
		FileName ext = *it;
		boost::algorithm::to_lower(ext);
		auto fit = m_extensions.find(ext);

		if (fit == m_extensions.end())
			m_extensions.insert(std::make_pair(ext, id));
		else
			m_extensions.insert(fit, std::make_pair(ext, id));
	}

	return id;
}

PackerBase::Ptr PackerManager::findPacker(const FileName& name, PackDataStream& stream)
{
	String ext = hd::tools::extractFileExt(name);
	boost::algorithm::to_lower(ext);

	auto range = m_extensions.equal_range(ext);

	for (int i = 0; i < 2; ++i)
	{
		if (i)
			range = m_extensions.equal_range(L"");

		for (auto it = range.first; it != range.second; ++it)
		{
			PackerBase::Ptr packer = m_packers[it->second];
			if (!packer)
				continue;
			if (packer->isCorrectFile(name, stream))
				return packer;
		}
	}

	return PackerBase::Ptr();
}
