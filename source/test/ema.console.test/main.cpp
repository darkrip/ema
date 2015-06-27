

#include "console_command.hpp"

#include "defs.hpp"

#include <boost/range.hpp>

#include <string>
#include <iostream>


class TestConsoleCommandHandler : public ema::console::ConsoleCommandHandler
{
public:
	virtual void output(ema::StringRef str)
	{
		if (!m_result.empty())m_result += L"\n";
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
		console.init(locale, false);
		ema::console::ConsoleCommand command(command);
		command.init();
		TestConsoleCommandHandler handler;
		ema::var::EmptyContext context;
		result = console.execute(command, context, handler);
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
	if (test(config.locale, config.command, config.expectedOutput, config.expectedResult, config.wrongCommand))
	{
		std::cout << "test " << id << " passed" << std::endl;
		return true;
	}
	else
	{
		std::cout << "test " << id << " failed" << std::endl;
		return false;
	}
}



int main(int, char**)
{
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
		{ L"C",      L"consoleTest1 Tygra",    L"Tegra Tagra Tygra",       0,  false },
		{ L"C",      L"consoleTest2 гипотеза", L"теорма аксиома гипотеза", 0,  false },
		{ L"UTF-16", L"consoleTest3 гипотеза", L"теорма аксиома гипотеза", 0,  false },
		{ L"UTF-16", L"consoleTest3 仮説",     L"定理公理 仮説",            0,  false },
		{ L"UTF-8",  L"consoleTest4 仮説",     L"定理公理 仮説",            0,  false },
		{ L"UTF-99", L"consoleTest4 仮説",     L"定理公理 仮説",            0,  true  },
		{ L"C",      L"consoleTest5 17",       L"",                        17, false },
		{ L"C",      L"wrongcommand17157sx",   L"",                        0,  true  },
	};

	int result = 0;
	for (size_t i = 0; i < boost::size(testConfigs); ++i)
		if (!run_test((int)i, testConfigs[i]))
			--result;
	return result;
}