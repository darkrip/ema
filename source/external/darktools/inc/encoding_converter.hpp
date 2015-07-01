#ifndef __ENCODING_CONVERTER_HPP__
#define __ENCODING_CONVERTER_HPP__



#include <string>
#include <vector>


namespace hd
{
namespace tools
{


class EncodingConverter
{
	typedef std::vector<unsigned char> BufferType;
public:
	EncodingConverter(const std::string& encoding);
	EncodingConverter(const std::wstring& encoding);

	template<typename Output, typename Input>
	Output from(const Input& input)// Convert from "encoding" to UTF-16LE (common for C++)
	{
		BufferType boutput;
		from(ToBufferType(input), boutput);
		return FromBufferType<Output>(boutput); 
	}
	

	template<typename Output, typename Input>
	Output to(const Input& input) // Convert from UTF-16LE (common for C++) to "encoding"
	{
		BufferType boutput;
		from(ToBufferType(input), boutput);
		return FromBufferType<Output>(boutput); 
	}
	
private:

	template<typename Input>
	BufferType ToBufferType(const Input& input)
	{
		if(input.size()==0)
			return BufferType();
		const unsigned char* ubegin = (unsigned char*)input.c_str();
		const unsigned char* uend = ubegin + input.size()*sizeof(input[0]);
		return BufferType(ubegin, uend);
	}

	template<typename Output>
	Output FromBufferType(const BufferType& input)
	{
		if(input.size()==0)
			return Output();
		const unsigned char* ubegin = &input[0];
		const unsigned char* uend = ubegin + input.size()*sizeof(BufferType::value_type);

		while ((uend - ubegin) % sizeof(Output::value_type))
			--uend;

		return Output((Output::value_type*)ubegin, (Output::value_type*)uend);
	}

	void from(const BufferType& input, BufferType& output);
	void to(const BufferType& input, BufferType& output);
	void detectEncodingType();
	enum ConvertType{ ctSingle, ctMultyByte, ctMultyByteRevers } m_coverType;
	bool m_useStdLocale;
	std::locale m_locale;
	std::string m_encoding;
};





}//tools
}//hd




#endif//__ENCODING_CONVERTER_HPP__