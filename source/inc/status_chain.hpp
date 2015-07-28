#ifndef __STATUS_CHAIN_HPP__
#define __STATUS_CHAIN_HPP__



namespace ema
{
namespace pack
{



template< typename StatusType, typename Functor >
class StatusChain
{
public:
	StatusType run(StatusType newStatus, StatusType oldStatus, );

	struct Creator
	{
		typedef std::function<void(int, int)>::
	};


	struct Item
	{
		Item()

	};



};










}//pack
}//ema













#endif//__STATUS_CHAIN_HPP__