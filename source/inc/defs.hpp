#ifndef __DEFS_HPP__
#define __DEFS_HPP__

#include <string>
#include <iostream>

namespace ema
{
typedef std::wstring	 String;
typedef const String&	 StringRef;
typedef String			 FileName;
typedef std::wiostream   PackDataStream;
typedef std::wostream    PackDataReadStream;
typedef std::wistream    PackDataWriteStream;

}


#endif//__DEFS_HPP__