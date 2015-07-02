#include "encoding_converter.hpp"

#include <boost/algorithm/string.hpp>
#include <boost/locale.hpp>

#include <algorithm>
#include <locale>
#include <codecvt>
#include <exception>

using namespace hd;
using namespace tools;



EncodingConverter::EncodingConverter(const std::wstring& encoding)
{
	m_encoding = std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>().to_bytes(encoding);
	detectEncodingType();
}


EncodingConverter::EncodingConverter(const std::string& encoding): m_encoding(encoding)
{
	detectEncodingType();
}

void EncodingConverter::detectEncodingType()
{
	std::string encoding = m_encoding;
	boost::erase_all(encoding, " ");
	boost::erase_all(encoding, "_");
	boost::erase_all(encoding, "-");
	boost::erase_all(encoding, ".");
	boost::algorithm::to_lower(encoding);

	if(encoding=="utf16" || encoding=="utf16le")
		m_coverType = ctMultyByte;
	else if(encoding=="utf16be")
		m_coverType = ctMultyByteRevers;
	else
		m_coverType = ctSingle;

	try
	{
		m_locale = std::locale(m_encoding);
		m_codecvt = &std::use_facet<Codecvt>(m_locale);
		m_useStdLocale = true;
	}catch(std::exception&)
	{
		m_useStdLocale = false;
		m_codecvt = NULL;
	}
}

void EncodingConverter::from(const EncodingConverter::BufferType& input, EncodingConverter::BufferType& output)
{ 
	if(m_coverType==ctMultyByte)
		output = input;
	else if(m_coverType==ctMultyByteRevers)
		std::copy(input.rbegin(), input.rend(), std::back_inserter(output));
	else
	{
		const char* begin = (char*)&input[0];
		const char* end = ((char*)&input[input.size()-1])+1;

		std::wstring buffer;
		
		if(m_useStdLocale)
		{
			std::mbstate_t state=0;

			const char* from_next;                            // from_next
			wchar_t* to_next;                              // to_next

			buffer.resize(input.size()+1);
			
			auto myresult = m_codecvt->in (state, begin, end, from_next,
				&buffer[0], &buffer[0]+buffer.size(), to_next);
			buffer.resize(to_next-&buffer[0]);
		}
		else
			buffer = boost::locale::conv::to_utf<wchar_t>(begin, end, m_encoding);
		output = ToBufferType(buffer);
	}
	
	
}


void EncodingConverter::to(const EncodingConverter::BufferType& input, EncodingConverter::BufferType& output)
{ 
	if(m_coverType==ctMultyByte)
		output = input;
	else if(m_coverType==ctMultyByteRevers)
		std::copy(input.rbegin(), input.rend(), std::back_inserter(output));
	else
	{
		const wchar_t* begin = (wchar_t*)&input[0];
		const wchar_t* end = ((wchar_t*)&input[input.size()-1])+1;
		std::string buffer;
		if(m_useStdLocale)
		{
			std::mbstate_t state=0;

			const wchar_t* from_next;                            // from_next
			char* to_next;                              // to_next

			buffer.resize(input.size()+1);

			auto myresult = m_codecvt->out (state, begin, end, from_next,
				&buffer[0], &buffer[0]+buffer.size(), to_next);
			buffer.resize(to_next-&buffer[0]);
		}
		else
			buffer = boost::locale::conv::from_utf(begin, end, m_encoding);



		output = ToBufferType(buffer);
	}
}
