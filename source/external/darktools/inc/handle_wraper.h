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

private:
	void DropImpl();
	template< typename T >
	void Init( T ){ impl_ = 0; }

	impl::HHANDLE_Impl* impl_;
};

}//tools
}//hd

#endif//_HANDLE_WRAPER_H_