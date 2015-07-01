#ifndef _HANDLE_WRAPER_H_
#define _HANDLE_WRAPER_H_

namespace hd
{
namespace tools
{
namespace impl
{
	class HHANDLE_Impl;
}

class HHANDLE
{
public:

	template< typename T >
	explicit HHANDLE( T value ){ Init(value); }

	HHANDLE( const HHANDLE& ref );

	HHANDLE(){ Init(0); }
	~HHANDLE(){ DropImpl(); }

	impl::HHANDLE_Impl& impl()const{ return *impl_; }

	HHANDLE& operator=(const HHANDLE& ref);

	operator bool()const{ return !!impl_; }  
	void close(){ DropImpl(); }

private:
	void DropImpl();
	template< typename T >
	void Init(T){ DropImpl(); impl_ = 0; }

	impl::HHANDLE_Impl* impl_;
};

}//tools
}//sd

#endif//_HANDLE_WRAPER_H_