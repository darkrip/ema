#ifndef __HDTOOLS_GUID_HPP__
#define __HDTOOLS_GUID_HPP__

namespace hd
{
namespace tools
{



class Guid
{
public:
	void Guid(REFGUID);
	void Guid(const std::wstring&);
	
	operator const REFGUID()const;
	operator std::wstring()const;
	
	static Guid& create();	
};





}
}




#endif//__HDTOOLS_GUID_HPP__