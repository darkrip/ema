#ifndef __CUSTOM_PACKER_FACTORY_HPP__
#define __CUSTOM_PACKER_FACTORY_HPP__

#include "defs.hpp"
#include "custom_packer.hpp"
#include "console_command.hpp"


namespace ema
{
namespace pack
{



class CustomPackerFactoryBase
{
protected:
	void construct(StringRef packName);
	void registration();


	void setPackArchiverEncoding(StringRef encoding){ m_packer->m_encoding = encoding; }
	void setPackDescription(StringRef description){ m_packer->m_description = description; }
	void setArchiver(StringRef archiver){ m_packer->m_archiver = archiver; }
	void setPackIconPath(StringRef iconPath){ m_packer->m_iconPath = iconPath; }
private:
	CustomPacker::Ref m_packer;
};





}//pack
}//ema


#endif//__CUSTOM_PACKER_FACTORY_HPP__