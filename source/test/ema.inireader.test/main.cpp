


#include "ini_parser.hpp"

#include <string>
#include <iostream>
#include <fstream>
#include <stdexcept>

#include <boost/config.hpp>
#include <boost/range.hpp>
#include <boost/locale.hpp>




bool test_ini(int id, const std::locale& locale, const std::wstring& multibyte_example, bool linux_like_line_ending,  bool error1, bool error2, bool error3, bool error4)
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

	std::wstring file_name = std::wstring(L"ini_test") + std::to_wstring(id);
	{
		std::wfstream file(file_name, std::wfstream::out | std::wfstream::trunc);

		file.imbue(locale);

		file << cendl;
		file << L";comment1"  << cendl;
		if(error1)
			file << L"key1="  << multibyte_example<< cendl;
		file << L"[section1"  << (error2 ? L"" : L"]")  << cendl;
		file << L";comment2"  << cendl;
		file << cendl;
		file << L"key1=value1"<< cendl;
		file << L"key2  "     << (error4 ? L"" : L"=") << multibyte_example<< cendl;
		file << cendl;
		file << L";comment3"  << cendl;
		file << L"[section2]" << (error3 ? L"w" : L"")  << cendl;
		file << L"key1=value3"<< cendl;
		file << L"key2=key2"  << multibyte_example << cendl;
		file << L";comment4"  << multibyte_example << cendl;
		file << cendl;
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
	std::locale locale;
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
		std::cout << "test1 passed" << std::endl;
		return 0;
	}
	else
	{
		std::cout << "test1 failed" << std::endl;
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

	boost::locale::generator gen;

	std::locale current(gen("ru_RU.1251"));
	std::locale current_utf8(gen("en_US.UTF-8"));
	std::locale current_utf16(gen("en_US.UTF-16"));
	std::locale jap_utf8(gen("en_US.UTF-8"));
	std::locale jap_utf16(gen("en_US.UTF-16"));


//	boost::locale::info inf;


	boost::locale::generator gen1;
	std::locale l = gen1("");
	std::cout << std::use_facet<boost::locale::info>(l).name() << std::endl;

//	std::cout << std::locale::global().c_str();
	
	boost::locale::info const &inf=std::use_facet<boost::locale::info>(std::locale(""));
	std::locale loc= boost::locale::generator().generate("ru_RU.windows-1252");
	std::ofstream file;
	//file.imbue(loc);
	file.open("hello.txt");
	

	file << boost::locale::conv::from_utf(L"Теория струн",inf.language()) << std::endl;

	//file << L"Теория струн" << std::endl;
	file.close();
	return 0;




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
		//{current,       L"test1",  false, false, false, false, false},
		//{current_utf8,  L"test2",  false, false, false, false, false},
		//{current_utf16, L"test3",  false, false, false, false, false},
		{current,       L"тест4",  false, false, false, false, false},
		{current_utf8,  L"тест5",  false, false, false, false, false},
		{current_utf16, L"тест6",  false, false, false, false, false},
		//{jap_utf8,      L"試験7",  false, false, false, false, false},
		//{jap_utf16,     L"試験8",  false, false, false, false, false},
		//{current_utf8,  L"тест9",  true,  false, false, false, false},
		//{current_utf8,  L"тест10", false, true,  false, false, false},
		//{current_utf8,  L"тест11", false, false, true,  false, false},
		//{current_utf8,  L"тест12", false, false, false, true,  false},
		//{current_utf8,  L"тест13", false, false, false, false, true},
		//{current_utf16, L"тест14", true,  false, false, false, false},
		//{current_utf16, L"тест15", false, true,  false, false, false},
		//{current_utf16, L"тест16", false, false, true,  false, false},
		//{current_utf16, L"тест17", false, false, false, true,  false},
		//{current_utf16, L"тест18", false, false, false, false, true}
	};

	for(size_t i=0; i < boost::size(testConfigs); ++i)
		result|=run_test((int)i, testConfigs[i]);

	return result;
}


