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
	String getCacheFolder(){ return L""; }
	CacheController createSub(){ return CacheController(); }
};









}//pack
}//ema














#endif//__CACHE_CONTROLLER_HPP__