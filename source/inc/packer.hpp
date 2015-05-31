#ifndef __EMA_PACKER_HPP__
#define __EMA_PACKER_HPP__

namespace ema
{
namespace pack
{

typedef std::iowstream PackDataStream;
typedef std::wstring FileName;

class Packer
{
public:

};


class PackerManager
{
public:
	typedef struct PACKER_ID_T{} *PACKER_ID;

	static PackerManager& getInstance();
	PACKER_ID registerPacker(Packer::Ref&);
	Packer::Ref findPacker(const FileName&, PackDataStream&);	
};

class PackManager
{
};



class Pack
{
public:

};



class FileCache
{
public:

};









}
}

#endif//__EMA_PACKER_HPP__