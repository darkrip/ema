#ifndef __PACK_FILE_HPP__
#define __PACK_FILE_HPP__


#include "shared_data.hpp"


namespace ema
{
namespace pack
{

class PackerBase;


class PackFile : public SharedDataStorage
{
public:
	typedef std::shared_ptr<PackFile> Ptr;
	FileName getPackName()const;
	FileName getPackFullPatch()const;
	FileName getTmpFolder()const;
	FileName getPassword()const;
	FileCache& getRoot();
	std::shared_ptr<PackerBase> getPacker();
};




}//pack
}//ema





#endif//__PACK_FILE_HPP__