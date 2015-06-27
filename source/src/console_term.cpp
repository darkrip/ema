#include "console_term.hpp"


using namespace ema;
using namespace console;


struct ema::console::ConsoleTermData
{
	ConsoleTermData(){}
	bool m_isDebug;

};



ConsoleTerm::ConsoleTerm()
{
}


void ConsoleTerm::init(StringRef locale, bool isDebug)
{
	m_data.reset(new ConsoleTermData());
	m_data->m_isDebug = isDebug;
}





