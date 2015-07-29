#ifndef __STATUS_CHAIN_HPP__
#define __STATUS_CHAIN_HPP__


#include <boost/function_types/parameter_types.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/equal.hpp>
#include <boost/fusion/functional/invocation/invoke.hpp>
#include <boost/fusion/include/make_vector.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/type_traits/remove_reference.hpp>

namespace ema
{

class StatusChainExpection : public std::exception
{};

template< typename StatusType, typename FunctorSpec >
class StatusChain
{
public:
	typedef std::function<FunctorSpec> Functor;

	typedef typename boost::function_types::parameter_types< FunctorSpec > ParamMplListWithRef;
	typedef typename boost::mpl::transform< ParamMplListWithRef, boost::remove_reference<boost::mpl::_1> >::type ParamMplList;

	template< typename ...Args >
	StatusType run(StatusType newStatus, StatusType oldStatus, Args& ... args)
	{
		typedef boost::mpl::vector<Args...> MplArgs;
		static_assert(boost::mpl::equal<ParamMplList, MplArgs>::type::value, "Wrong arguments");
		auto fargs = boost::fusion::vector<Args&...>(args...);

		BOOST_ASSERT(newStatus >= 0 && newStatus < (int)m_items.size());

		StatusType realStatus = oldStatus;
		if (newStatus == oldStatus)
		{
			//TODO??????			
			BOOST_ASSERT(0);
		}
		else
		{
			if (newStatus > oldStatus)
			{
				StatusType stepStatus = oldStatus;
				do
				{
					stepStatus = (StatusType)(stepStatus + 1);
					realStatus =
						boost::fusion::invoke(m_items[stepStatus].m_upgradeFunctor, fargs);
					if (realStatus < stepStatus)
						throw StatusChainExpection();
				} while (realStatus < newStatus);
			}
			else
			{
				StatusType stepStatus = oldStatus;
				do
				{
					stepStatus = (StatusType)(stepStatus - 1);
					realStatus =
						boost::fusion::invoke(m_items[stepStatus].m_downgradeFunctor, fargs);
					if (realStatus > stepStatus)
						throw StatusChainExpection();
				} while (realStatus > newStatus);
			}
		}
		return realStatus;
	}


	struct Item
	{
		Item() : m_status(StatusType()){}
		Item(StatusType status, const Functor& upgradeFunctor, const Functor& downgradeFunctor) :
			m_status(status), m_upgradeFunctor(upgradeFunctor), m_downgradeFunctor(downgradeFunctor){}

		StatusType m_status;
		Functor m_upgradeFunctor;
		Functor m_downgradeFunctor;
	};

	struct Creator
	{
		typedef std::vector<Item> Items;
		Creator& operator<<(Item&& item)
		{
			m_items.push_back(item);
			return *this;
		}

		Items m_items;
	};


	void init(Creator& creator)
	{
		auto maxIt = std::max_element(creator.m_items.begin(), creator.m_items.end(), [](const Item& itemLeft, const Item& itemRight)->bool{ return itemLeft.m_status < itemRight.m_status; });
		m_items.resize(maxIt->m_status + 1);
		std::for_each(creator.m_items.begin(), creator.m_items.end(), [&](Item& item){ std::swap(m_items[item.m_status], item); });		
	}
private:
	typedef std::vector<Item> Items;
	Items m_items;
};










}//ema













#endif//__STATUS_CHAIN_HPP__