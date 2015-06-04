#ifndef __HDTOOLS_SINGLETON_HPP__
#define __HDTOOLS_SINGLETON_HPP__


namespace hd
{
namespace tools
{

template<class T>
class AutoSingleton
{
public:
	static T& getInstance()
	{
		static T tmp;
		return tmp;
	}
};



template<class T>
class ManualSingleton
{
public:
	static T& getInstance()
	{
		static T tmp;
		return tmp;
	}
protected:
	static void create();
	static void destroy();
};





}
}




#endif//__HDTOOLS_SINGLETON_HPP__