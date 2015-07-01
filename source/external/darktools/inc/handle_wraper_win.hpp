#ifndef _HANDLE_WRAPER_IMPL_H_
#define _HANDLE_WRAPER_IMPL_H_


#include <Windows.h>
#include "handle_wraper.hpp"



namespace hd
{
namespace tools
{
namespace impl
{



class HHANDLE_Impl
{
public:
	~HHANDLE_Impl(){ CloseHandle(handle_); }
	HHANDLE_Impl( HANDLE handle ):handle_(handle){}
	HHANDLE_Impl( const HHANDLE_Impl&  ref )
	{
		DuplicateHandle(GetCurrentProcess(), 
			ref.handle_, 
						GetCurrentProcess(),
						&handle_, 
						0,
						FALSE,
						DUPLICATE_SAME_ACCESS);
	}

	HANDLE get()const
	{
		return handle_;
	}

	operator HANDLE()const
	{
		return handle_;
	}
private:
	HANDLE handle_;	
};


}//impl





template<>
inline void HHANDLE::Init< HANDLE >( HANDLE handle )
{	
	impl_ = new impl::HHANDLE_Impl(handle);
}


template<>
inline void HHANDLE::Init< const HHANDLE& >( const HHANDLE& ref )
{	
	if( ref.impl_ )
		impl_ = new impl::HHANDLE_Impl(*ref.impl_);
	else
		impl_ = 0;
}


inline void HHANDLE::DropImpl()
{
	delete impl_;
	impl_ = 0;
}


inline HHANDLE::HHANDLE( const HHANDLE& ref )
{ 
	Init<const HHANDLE&>(ref); 
}


inline HHANDLE& HHANDLE::operator=(const HHANDLE& ref)
{
	DropImpl();
	Init<const HHANDLE&>(ref);
	return *this;
}


}//tools
}//hd






#endif//_HANDLE_WRAPER_IMPL_H_