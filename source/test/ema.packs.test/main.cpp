
#include "custom_packs_ini_configurator.hpp"
#include "packer.hpp"

#include <fstream>





void print_file(const ema::pack::FileCache::Ptr& file, const std::wstring& tab=L"")
{
	std::wcout << tab << file->getName() << std::endl;
	for(size_t i=0; i < file->getChilds().size(); ++i)
		print_file(file->getChilds()[i], tab+L"\t");

}








int main(int argc, char** argv)
{
	ema::config::CustomPackerIniConfigurator configurator(L"test.pack.ini");
	std::wstring fileName = L"TestPacks1.testpack";
	std::fstream file(fileName, std::fstream::in);
	ema::pack::PackerBase::Ptr packer = ema::pack::PackerManager::getInstance().findPacker(fileName, file);
	ema::pack::PackFile::Ptr pack_file = packer->open(fileName, file);
	ema::pack::FileCache::Ptr root_folder = pack_file->getRoot();
}