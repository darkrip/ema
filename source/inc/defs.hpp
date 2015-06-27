#ifndef __DEFS_HPP__
#define __DEFS_HPP__

#include <string>
#include <iostream>
#include <vector>

namespace ema
{
typedef std::wstring	    String;
typedef const String&	    StringRef;
typedef String			    FileName;
typedef const FileName&	    FileNameRef;
typedef std::vector<String> StringList;
typedef const StringList&   StringListRef;
typedef std::wiostream      PackDataStream;
typedef std::wostream       PackDataReadStream;
typedef std::wistream       PackDataWriteStream;

}


#endif//__DEFS_HPP__