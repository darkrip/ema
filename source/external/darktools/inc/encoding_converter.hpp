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
	/*
	template<typedef TInput, typename TOutput>
	std::basic_string<TOutput, std::char_traits<TOutput>, std::allocator<TOutput> >
		from(const std::basic_string<TInput, std::char_traits<TInput>, std::allocator<TInput> >& input);   // Convert from "encoding" to UTF-16LE (common for C++)

	template<typedef Input, typename Output>
	basic_string<Output, char_traits<Output>, allocator<Output> >
		to(const basic_string<Input, char_traits<Input>, allocator<Input> >& input);   // Convert from UTF-16LE (common for C++) to "encoding"

		*/
};





}//tools
}//hd




#endif//__ENCODING_CONVERTER_HPP__