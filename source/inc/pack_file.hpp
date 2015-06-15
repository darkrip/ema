#ifndef __PACK_FILE_HPP__
#define __PACK_FILE_HPP__


namespace ema
{
namespace pack
{

class PackerBase;


class PackFile
{
public:
	typedef std::shared_ptr<PackFile> Ref;
	FileName getPackName()const;
	FileCache& getRoot();
	std::shared_ptr<PackerBase> getPacker();
};




}//pack
}//ema





#endif//__PACK_FILE_HPP__