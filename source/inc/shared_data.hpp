#ifndef __SHARED_DATA_HPP__
#define __SHARED_DATA_HPP__

#include <memory>
#include <map>
#include <vector>

namespace ema
{
namespace pack
{


class BaseSharedData
{
public:
	struct _SDI;
	typedef _SDI *SDI;

	virtual ~BaseSharedData(){}

	template<typename T>
	static SDI getSharedDataId()
	{
		auto ptr = static_cast<SDI(*)()>(&BaseSharedData::getSharedDataId<T>);
		return (SDI)(void*)ptr;
	}
};


template<typename T>
class SharedData : public BaseSharedData
{
public:
	T& get(){ return m_data; }
private:
	T m_data;
};




class SharedDataStorage
{
public:
	template<typename T>
	T& getSharedData(size_t index=0)
	{
		BaseSharedData::SDI sdi = BaseSharedData::getSharedDataId<T>();
		BaseSharedData& bsd =  getBSD<T>(sdi, index);
		return ((SharedData<T>&)bsd).get();
	}
private:
	typedef std::unique_ptr<BaseSharedData> BSD;
	typedef std::vector<BSD> BSDItems;
	typedef std::map<BaseSharedData::SDI, BSDItems> SharedDataMap;

	template<typename T>
	BaseSharedData& getBSD(BaseSharedData::SDI sdi, size_t index)
	{
		auto it = m_data.find(sdi);
		if(it==m_data.end())
			it = m_data.insert(std::make_pair(sdi, BSDItems())).first;

		while(it->second.size() >= index)
		{
			it->second.push_back(BSD(new SharedData<T>()));
		}

		return *it->second[index].get();
	}

	SharedDataMap m_data;
};



}//pack
}//ema


#endif//__SHARED_DATA_HPP__
