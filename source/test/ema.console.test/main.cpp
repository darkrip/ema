

#include "console_command.hpp"
#include "console_term.hpp"

#include "defs.hpp"

#include <boost/range.hpp>

#include <string>
#include <iostream>
#include "windows.h"

class TestConsoleCommandHandler : public ema::console::ConsoleCommandHandler
{
public:
	virtual void output(ema::StringRef str)
	{
		if (m_result.empty())
			m_result = str.substr(0, str.length()-1);
		else
			m_result += str;
	}
	virtual ema::String input(){ return L""; }
	ema::String getResult(){ return m_result; }
private:
	ema::String m_result;
};


bool test(const std::wstring& locale, const std::wstring& command, const std::wstring& expectedOutput, int expectedResult, bool wrongCommand)
{
	bool is_have_expection = false;
	std::wstring output_result;
	int result = -1;

	try
	{
		ema::console::ConsoleTerm console;
		console.init(locale, false, 10000);
		TestConsoleCommandHandler handler;
		result = console.execute(command, handler);
		output_result = handler.getResult();
	}
	catch (std::exception&)
	{
		is_have_expection = true;
	}

	return (wrongCommand && is_have_expection) || (!wrongCommand && !is_have_expection && output_result == expectedOutput && result == expectedResult);
}


struct TestConfig
{
	std::wstring locale;
	std::wstring command;
	std::wstring expectedOutput;
	int expectedResult;
	bool wrongCommand;
};


bool run_test(int id, const TestConfig& config)
{
	std::stringstream s;
	bool result = false;

	if (test(config.locale, config.command, config.expectedOutput, config.expectedResult, config.wrongCommand))
	{
		s << "test " << id << " passed" << std::endl;
		result = true;
	}
	else
	{
		s << "test " << id << " failed" << std::endl;
	}
	OutputDebugStringA(s.str().c_str());
	std::cout << s.str();
	return result;
}


int main(int, char**)
//int __stdcall WinMain( _In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd )
{

	wchar_t buff[1024];
	GetCurrentDirectoryW(1024, buff);

	/*
	consoleTest1.bat
		Tegra Tagra [args]
	consoleTest2.bat - OEM
		теорма аксиома [args]
	consoleTest3.bat - Utf-16
		定理公理 [args]
	consoleTest4.bat - Utf-8
		定理公理 [args]
	consoleTest5.bat [args] set error level args
	*/


	TestConfig testConfigs[] = {
		{ L"C",      L"echo For the KKK",      L"For the KKK",             0,  false },
		{ L"C",      L"consoleTest1.bat Tygra",    L"Tegra Tagra Tygra",       0,  false },
		{ L"C",      L"consoleTest2.bat гипотеза", L"теорма аксиома гипотеза", 0,  false },
		{ L"UTF-16", L"consoleTest3.bat гипотеза", L"теорма аксиома гипотеза", 0,  false },
		{ L"UTF-16", L"consoleTest3.bat 仮説",     L"定理公理 仮説",            0,  false },
		{ L"UTF-8",  L"consoleTest4.bat 仮説",     L"定理公理 仮説",            0,  false },
		{ L"UTF-99", L"consoleTest4.bat 仮説",     L"定理公理 仮説",            0,  true  },
		{ L"C",      L"consoleTest5.bat 17",       L"",                        17, false },
		{ L"C",      L"wrongcommand17157sx.bat",   L"",                        0,  true  },
	};

	int result = 0;
	for (size_t i = 0; i < boost::size(testConfigs); ++i)
		if (!run_test((int)i, testConfigs[i]))
			--result;
	return result;
}