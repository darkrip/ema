#ifndef __HDTOOLS_FILE_TOOLS_HPP__
#define __HDTOOLS_FILE_TOOLS_HPP__

#include <string>

namespace hd
{
namespace tools
{


std::wstring shortPathCreator(const std::wstring& path);
std::wstring extractFileExt(const std::wstring& fileName);



}//tools
}//hd






#endif //__HDTOOLS_FILE_TOOLS_HPP__
