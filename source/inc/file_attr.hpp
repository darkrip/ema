#ifndef __FILE_ATTR_HPP__
#define __FILE_ATTR_HPP__


#include <boost/type_traits/remove_pointer.hpp>
#include "defs.hpp"
/*
_Out_opt_ LPFILETIME lpCreationTime,
_Out_opt_ LPFILETIME lpLastAccessTime,
_Out_opt_ LPFILETIME lpLastWriteTime


_P ;  n - им€ файла
_P ;  z - размер не сжатого файла
_P ;  c - описание файла
_P ;  p - размер сжатого файла
_P ;  d - день
_P ;  t - мес€ц
_P ;  TTT - трехбуквенное им€ мес€ца (Jan, Feb, Mar, ...)
_P ;  y - год
_P ;  h - часы
_P ;  H - буква - модификатор часов (a - врем€ до полудн€ (a.m. time),
_P                                   p - врем€ после полудн€ (p.m. time))
_P ;  m - минуты
_P ;  s - секунды
_P ;  a - атрибуты
_P ;  e - расширение файла
_P ;  * - пропускать до первого пробела или конца строки

*/

namespace ema
{
namespace attr
{


enum PreferedAttrs
{
	paUnknown,
	paIsDirectory,
	paIsReadOnly,
	paIsArhived,
	paIsHidden,
	paIsSystem,
	paSize,
	paCommpressedSize,
	paDescription,
	paCreationTime,
	paLastModificationTime,
	paLastOpenTime,
	paLastPreferedId
};


template< int DataTypeId >
class BaseAttr
{
public:
	enum{ attr_index=DataTypeId };
};



template< int DataTypeId >
class BaseBoolAttr : public BaseAttr<DataTypeId>
{
};

template< int DataTypeId >
class BaseStringAttr : public BaseAttr<DataTypeId>
{
};

template< int DataTypeId >
class BaseIntAttr : public BaseAttr<DataTypeId>
{
};

template< int DataTypeId >
class BaseDateTimeAttr : public BaseAttr<DataTypeId>
{
};



class DataBuffer
{
public:
	typedef unsigned char* DataType;
	DataBuffer(): m_size(0), m_data(nullptr){}
	DataBuffer(size_t size): m_size(size), m_data(new boost::remove_pointer<DataType>::type[size]){}
	//DataBuffer(size_t size, const DataType buffer) : m_size(size), 
	DataBuffer (DataBuffer && ref): m_size(ref.m_size), m_data(ref.m_data){ ref.m_data=nullptr;}

	bool isEmpty()const;
private:	
	DataBuffer(const DataBuffer&);

	size_t m_size;
	DataType m_data;
};

typedef const char* AttrNameType;

template< int DataTypeId >
class AttrData
{
public:
	AttrNameType getName(){ return "unknown"; }
	enum{ attr_index=DataTypeId };
};


#define EMA_ATTR_REGISTER_ATTR_TYPE(name, base)			\
	template < >										\
	class AttrData<base::attr_index>  : public base		\
	{													\
		static AttrNameType getName(){ return name; }	\
	}





EMA_ATTR_REGISTER_ATTR_TYPE("directory", BaseBoolAttr<paIsDirectory>);
EMA_ATTR_REGISTER_ATTR_TYPE("read only", BaseBoolAttr<paIsReadOnly>);
EMA_ATTR_REGISTER_ATTR_TYPE("archived", BaseBoolAttr<paIsArhived>);
EMA_ATTR_REGISTER_ATTR_TYPE("hidden", BaseBoolAttr<paIsHidden>);
EMA_ATTR_REGISTER_ATTR_TYPE("size", BaseIntAttr<paSize>);
EMA_ATTR_REGISTER_ATTR_TYPE("compressed size", BaseIntAttr<paCommpressedSize>);
EMA_ATTR_REGISTER_ATTR_TYPE("description", BaseStringAttr<paIsSystem>);
EMA_ATTR_REGISTER_ATTR_TYPE("creation time", BaseDateTimeAttr<paCreationTime>);
EMA_ATTR_REGISTER_ATTR_TYPE("last modified time", BaseDateTimeAttr<paLastModificationTime>);
EMA_ATTR_REGISTER_ATTR_TYPE("last open time", BaseDateTimeAttr<paLastOpenTime>);



class FileAttr
{
public:
	typedef int AttrIndexType;

	template<int DataTypeId>
	String getAttrName()
	{
		return AttrData<DataTypeId>::name;
	}

	template<int DataTypeId>
	AttrData<DataTypeId> getAttr()
	{
		AttrIndexType index = AttrData<DataTypeId>::attr_index;
		static_assert(index!=AttrIndexType(-1));
		AttrIndexType innerIndex = getInnerIndex(index);
		if(index == AttrIndexType(-1))
		{
			auto guard = boost::make_lock_guard(m_mutex);
			m_databuffer.push_back(DataBuffer());
			index = m_databuffer.size()-1;
		}
		return AttrData<DataTypeId>(index);
	}	

	bool isEmpty()const;
	bool isDirty()const;
	void clearDirty();
};









}//attr
}//ema















#endif//__FILE_ATTR_HPP__