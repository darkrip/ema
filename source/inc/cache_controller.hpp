#ifndef __CACHE_CONTROLLER_HPP__
#define __CACHE_CONTROLLER_HPP__



#include "defs.hpp"



namespace ema
{
namespace pack
{



class CacheController
{
public:
	typedef std::shared_ptr<CacheController> Ptr;

	String getCacheFolder(){ return L""; }
	Ptr createSub(){ return Ptr(new CacheController()); }
};









}//pack
}//ema














#endif//__CACHE_CONTROLLER_HPP__