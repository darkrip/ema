#ifndef __ENCODING_CONVERTER_HPP__
#define __ENCODING_CONVERTER_HPP__



#include <string>



namespace hd
{
namespace tools
{


class EncodingConverter
{
public:
	EncodingConverter(const std::string& encoding){}
	EncodingConverter(const std::wstring& encoding){}

	template<typename Output, typename Input>
	Output from(const Input& input);   // Convert from "encoding" to UTF-16LE (common for C++)

	template<typename Output, typename Input>
	Output to(const Input& input);   // Convert from UTF-16LE (common for C++) to "encoding"
};





}//tools
}//hd




#endif//__ENCODING_CONVERTER_HPP__