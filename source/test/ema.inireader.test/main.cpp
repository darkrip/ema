


#include <string>
#include <iostream>
#include <fstream>









bool test_ini(const std::locale& locale, const std::wstring& multibyte_example, bool linux_like_line_ending,  bool error1, bool error2, bool error3, bool error4)
{
	


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

	static int i = 0;
	std::wstring file_name = std::wstring(L"ini_test") + std::to_wstring(i++);

	std::wfstream file(file_name, std::wfstream::out | std::wfstream::trunc);

	file << ";comment";






	try
	{

	}
	catch (const std::exception& e)
	{

	}

















}







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

	std::locale current;


	std::string language = current.name().c_str();
	if (language.find(L'.') != std::string::npos)
		language.erase(language.find(L'.'));

	std::locale current_utf8(language + ".UTF-8");
	std::locale current_utf16(language + ".UTF-16");

	std::string language = "ja_JP";

	std::locale jap_utf8(language + ".UTF-8");
	std::locale jap_utf16(language + ".UTF-16");


	if (test_ini(current, L"test1", false, false, false, false, false))
		std::cout << "test1 passed" << std::endl;
	else
		std::cout << "test1 failed" << std::endl;
//-----------------------------------------------------------------------------------------------------
	if (test_ini(current_utf16, L"test2", false, false, false, false, false))
		std::cout << "test2 passed" << std::endl;
	else
		std::cout << "test2 failed" << std::endl;
//----------------------------------------------------------------------------------------------------
	if (test_ini(current_utf16, L"test2", false, false, false, false, false))
		std::cout << "test2 passed" << std::endl;
	else
		std::cout << "test2 failed" << std::endl;
//----------------------------------------------------------------------------------------------------
	if (test_ini(current, L"тест 3", false, false, false, false, false))
		std::cout << "test3 passed" << std::endl;
	else
		std::cout << "test3 failed" << std::endl;
//----------------------------------------------------------------------------------------------------
	if (test_ini(current_utf8, L"тест4", false, false, false, false, false))
		std::cout << "test4 passed" << std::endl;
	else
		std::cout << "test4 failed" << std::endl;
//----------------------------------------------------------------------------------------------------
	if (test_ini(current_utf16, L"тест5", false, false, false, false, false))
		std::cout << "test5 passed" << std::endl;
	else
		std::cout << "test5 failed" << std::endl;
//----------------------------------------------------------------------------------------------------
	if (test_ini(current_utf8, L"試験6", false, false, false, false, false))
		std::cout << "test6 passed" << std::endl;
	else
		std::cout << "test6 failed" << std::endl;
//----------------------------------------------------------------------------------------------------
	if (test_ini(current_utf16, L"試験7", false, false, false, false, false))
		std::cout << "test7 passed" << std::endl;
	else
		std::cout << "test7 failed" << std::endl;
//----------------------------------------------------------------------------------------------------
	if (test_ini(current_utf8, L"тест8", true, false, false, false, false))
		std::cout << "test8 passed" << std::endl;
	else
		std::cout << "test8 failed" << std::endl;
	//----------------------------------------------------------------------------------------------------
	if (test_ini(current_utf8, L"тест9", false, true, false, false, false))
		std::cout << "test9 passed" << std::endl;
	else
		std::cout << "test9 failed" << std::endl;
	//----------------------------------------------------------------------------------------------------
	if (test_ini(current_utf8, L"тест10", false, false, true, false, false))
		std::cout << "test10 passed" << std::endl;
	else
		std::cout << "test10 failed" << std::endl;
	//----------------------------------------------------------------------------------------------------
	if (test_ini(current_utf8, L"тест11", false, false, false, true, false))
		std::cout << "test11 passed" << std::endl;
	else
		std::cout << "test11 failed" << std::endl;
	//----------------------------------------------------------------------------------------------------
	if (test_ini(current_utf8, L"тест12", false, false, false, false, true))
		std::cout << "test12 passed" << std::endl;
	else
		std::cout << "test12 failed" << std::endl;
	//----------------------------------------------------------------------------------------------------
	if (test_ini(current_utf16, L"тест13", true, false, false, false, false))
		std::cout << "test13 passed" << std::endl;
	else
		std::cout << "test13 failed" << std::endl;
	//----------------------------------------------------------------------------------------------------
	if (test_ini(current_utf16, L"тест14", false, true, false, false, false))
		std::cout << "test14 passed" << std::endl;
	else
		std::cout << "test14 failed" << std::endl;
	//----------------------------------------------------------------------------------------------------
	if (test_ini(current_utf16, L"тест15", false, false, true, false, false))
		std::cout << "test15 passed" << std::endl;
	else
		std::cout << "test15 failed" << std::endl;
	//----------------------------------------------------------------------------------------------------
	if (test_ini(current_utf16, L"тест116", false, false, false, true, false))
		std::cout << "test16 passed" << std::endl;
	else
		std::cout << "test16 failed" << std::endl;
	//----------------------------------------------------------------------------------------------------
	if (test_ini(current_utf16, L"тест17", false, false, false, false, true))
		std::cout << "test17 passed" << std::endl;
	else
		std::cout << "test17 failed" << std::endl;
	//----------------------------------------------------------------------------------------------------




}


