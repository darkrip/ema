#ifndef __HDTOOLS_SINGLETON_HPP__
#define __HDTOOLS_SINGLETON_HPP__


namespace hd
{
namespace tools
{

template<class T>
class Singleton
{
public:
	static T& getInstance()
	{
		static T tmp;
		return tmp;
	}
};


}
}




#endif//__HDTOOLS_SINGLETON_HPP__