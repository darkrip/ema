#ifndef __PACKER_ID_LIST_HPP__
#define __PACKER_ID_LIST_HPP__


#include "defs.hpp"

namespace ema
{
namespace pack
{

class PackerIdList
{
public:
	bool isCorrect(PackDataReadStream&);
};


}//pack
}//ema



#endif//__PACKER_ID_LIST_HPP__