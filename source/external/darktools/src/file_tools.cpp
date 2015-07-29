#include "file_tools.hpp"

#include <boost/filesystem.hpp>



std::wstring hd::tools::shortPathCreator(const std::wstring& path)
{
	return path;//TODO
}



std::wstring hd::tools::extractFileExt(const std::wstring& fileName)
{
	return boost::filesystem::path(fileName).extension().wstring();
}
