


#include "ini_parser.hpp"

#include <string>
#include <iostream>
#include <fstream>
#include <stdexcept>

#include <boost/config.hpp>
#include <boost/range.hpp>
#include <boost/locale.hpp>




bool test_ini(int id, const std::string& locale, const std::wstring& multibyte_example, bool linux_like_line_ending,  bool error1, bool error2, bool error3, bool error4)
{
	
	std::wstring cendl = linux_like_line_ending ? L"\n" : L"\r\n";



	/*

	;comment
	[section1]
	;comment

	key1=value1
	key2=multibyte_example
	;comment

	[section2]
	key1=value3
	key2=key2+multibyte_example
	;comment+multibyte_example

	*/

	std::wstring file_name = std::wstring(L"ini_test") + std::to_wstring(id) + std::wstring(L".ini");
	{
		std::wstringstream out;

		out << cendl;
		out << L";comment1" << cendl;
		if(error1)
			out << L"key1=" << multibyte_example << cendl;
		out << L"[section1" << (error2 ? L"" : L"]") << cendl;
		out << L";comment2" << cendl;
		out << cendl;
		out << L"key1=value1" << cendl;
		out << L"key2  " << (error4 ? L"" : L"=") << multibyte_example << cendl;
		out << cendl;
		out << L";comment3" << cendl;
		out << L"[section2]" << (error3 ? L"w" : L"") << cendl;
		out << L"key1=value3" << cendl;
		out << L"key2=key2" << multibyte_example << cendl;
		out << L";comment4" << multibyte_example << cendl;
		out << cendl;
		
		std::string str;
		if (locale == "UTF-16")
		{
			std::fstream file(file_name, std::fstream::out | std::fstream::trunc | std::fstream::binary);
			file.write((char*)out.str().c_str(), out.str().length()*sizeof(out.str()[0]));
		}
		else
		{
			str = boost::locale::conv::from_utf(out.str(), locale);
			if (locale == "UTF-8")
			{
				char bom[] = { (char)0xEF, (char)0xBB, (char)0xBF, 0 };
				str = std::string(bom) + str;
			}
			std::fstream file(file_name, std::wfstream::out | std::wfstream::trunc);
			file << str;
		}
	}


	bool is_parse_expection=false;
	bool is_parse_correct=true;

	try
	{
		ema::config::IniParser parser(file_name);

		is_parse_correct&=parser.getString(L"section1", L"key1")==L"value1";
		is_parse_correct&=parser.getString(L"section1", L"key2")==multibyte_example;
		is_parse_correct&=parser.getString(L"section2", L"key1")==L"value3";
		is_parse_correct&=parser.getString(L"section2", L"key2")==std::wstring(L"key2")+multibyte_example;
		is_parse_correct&=parser.getString(L"section2", L"key3", L"value5")==L"value5";
	}
	catch (const std::exception&)
	{
		is_parse_expection=true;
	}

	bool result=false;
	if(is_parse_expection)
	{
		result = error1 || error2 || error3 || error4;
	}else
	{
		result = !(error1 || error2 || error3 || error4) && is_parse_correct;
	}

	return result;
}




struct TestConfig
{
	std::string locale;
	std::wstring multibyte_example;
	bool linux_like_line_ending;
	bool key_without_section;
	bool section_name_incorrect;
	bool text_in_section_line;
	bool key_format_invalid;
};

int run_test(int id, const TestConfig& config)
{
	if (test_ini(id, config.locale, config.multibyte_example, config.linux_like_line_ending, config.key_without_section, config.section_name_incorrect, config.text_in_section_line, config.key_format_invalid))
	{
		std::cout << "test " << id << " passed" << std::endl;
		return 0;
	}
	else
	{
		std::cout << "test " << id << " failed" << std::endl;
		return -id;
	}
}


#include <windows.h>

int main(int, char**)
{
	int result = 0;
	/*
	english
	english unicode
	
	russian(current system locale)
	russian utf-8
	russian utf-16
	japan   utf-8
	japan   utf-16

	bool linux like line ending

	error1 - key bofore section
	error2 - wrong section name(without close bracket)
	error3 - wrong section name(other text in section line)
	error4 - wrong key(without =)
*/

	UINT langid = GetACP();
	CPINFOEX info;
	//GetCPInfo(langid, &info);

	GetCPInfoEx(CP_ACP, 0, &info);

	std::string current("cp1251");
	std::string current_utf8("UTF-8");
	std::string current_utf16("UTF-16");
	std::string jap_utf8("UTF-8");
	std::string jap_utf16("UTF-16");



/*

	std::locale current(gen(""));
	std::locale current_utf8(gen("rus_RUS.utf-8"));
	std::locale current_utf16(gen("ru_RU.UTF16"));
	std::locale jap_utf8(gen("ja_JP.UTF8"));
	std::locale jap_utf16(gen("ja_JP.UTF16"));

	std::cout << "current" << current.c_str() << std::endl;
	std::cout << "current_utf8" << current_utf8.c_str() << std::endl;
	std::cout << "current_utf16" << current_utf16.c_str() << std::endl;
	std::cout << "jap_utf8" << jap_utf8.c_str() << std::endl;
	std::cout << "jap_utf16" << jap_utf16.c_str() << std::endl;
*/


	TestConfig testConfigs[]={
		{current,       L"test1",  false, false, false, false, false},
		{current_utf8,  L"test2",  false, false, false, false, false},
		{current_utf16, L"test3",  false, false, false, false, false},
		{current,       L"тест4",  false, false, false, false, false},
		{current_utf8,  L"тест5",  false, false, false, false, false},
		{current_utf16, L"тест6",  false, false, false, false, false},
		{jap_utf8,      L"試験7",  false, false, false, false, false},
		{jap_utf16,     L"試験8",  false, false, false, false, false},
		{current_utf8,  L"тест9",  true,  false, false, false, false},
		{current_utf8,  L"тест10", false, true,  false, false, false},
		{current_utf8,  L"тест11", false, false, true,  false, false},
		{current_utf8,  L"тест12", false, false, false, true,  false},
		{current_utf8,  L"тест13", false, false, false, false, true},
		{current_utf16, L"тест14", true,  false, false, false, false},
		{current_utf16, L"тест15", false, true,  false, false, false},
		{current_utf16, L"тест16", false, false, true,  false, false},
		{current_utf16, L"тест17", false, false, false, true,  false},
		{current_utf16, L"тест18", false, false, false, false, true}
	};

	for(size_t i=0; i < boost::size(testConfigs); ++i)
		result|=run_test((int)i, testConfigs[i]);

	return result;
}


