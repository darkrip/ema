#ifndef __EMA_PACKER_HPP__
#define __EMA_PACKER_HPP__

#include "hdtools/singleton.hpp"


namespace ema
{
namespace reg
{

typedef std::wstring FileName;
/*
Apply all registre changes as single transaction. Store all changes with possibility to discard all changes
*/
class RegStorage : public hd::tools::Singleton<ServiceRegistrator>
{
public:
	typedef std::wstring KeyType;
	typedef int			 IntValue;
	typedef std::wstring StringValue;
	
	void setStorageFile(const FileName&);

	void addKey(const KeyType&);
	void addToRegistry(HKEY, const KeyType&, IntValue);
	void addToRegistry(HKEY, const KeyType&, const StringValue&);
	
	bool Apply(); //Applay all added query to system registry, remove all loses query. Return true when any changes was be
	void Revert(); //Remove all changes from registry
};






class ServiceRegistrator : public hd::tools::Singleton<ServiceRegistrator>
{
public:
	void setStorageFile(const FileName&);

	hd::tools::Guid addExtension(const FileName&);
	FileName getExtension(const hd::tools::Guid&);
	
	void Apply();
	void Revert();
}











}
}



#endif//__EMA_PACKER_HPP__