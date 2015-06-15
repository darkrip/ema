#ifndef __CUSTOM_PACKS_INI_CONFIGURATOR_HPP__
#define __CUSTOM_PACKS_INI_CONFIGURATOR_HPP__


#include "defs.hpp"
#include "custom_packer_factory.hpp"
#include "ini_parser.hpp"


namespace ema
{
namespace config
{




class CustomPackerIniConfigurator : public ema::pack::CustomPackerFactoryBase
{
public:
	CustomPackerIniConfigurator(const FileName& inifilename);
private:
	IniParser m_iniParser;
};














}//config
}//ema

#endif//__CUSTOM_PACKS_INI_CONFIGURATOR_HPP__