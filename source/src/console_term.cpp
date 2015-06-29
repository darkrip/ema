#include "console_term.hpp"

#include "encoding_converter.hpp"

#include <boost/assert.hpp>

#include <windows.h>

using namespace ema;
using namespace console;


class ema::console::ConsoleAsyncManager
{
public:
	class AsyncHandler
	{
	public:
		typedef std::shared_ptr<AsyncHandler> Ptr;

	};
	AsyncHandler::Ptr createHandler(ConsoleCommandHandler& handler, HANDLE outputReadPipe, HANDLE inputWritePipe);
private:

};

ConsoleAsyncManager ConsoleTerm::m_asyncManager;


struct ema::console::ConsoleTermData
{
	ConsoleTermData(StringRef encoding): m_converter(encoding){}
	bool m_isDebug;
	hd::tools::EncodingConverter m_converter;
	int m_maxExecTime;


	HANDLE m_inputReadPipe;
	HANDLE m_inputWritePipe;

	HANDLE m_outputReadPipe;
	HANDLE m_outputWritePipe;
};



ConsoleTerm::ConsoleTerm()
{
}


void ConsoleTerm::init(StringRef locale, bool isDebug, int max_exec_time)
{
	m_data.reset(new ConsoleTermData(locale));
	m_data->m_isDebug = isDebug;
	m_data->m_maxExecTime = max_exec_time;

	SECURITY_ATTRIBUTES sa = { 0 };
	sa.nLength = sizeof(sa);
	sa.bInheritHandle = TRUE;
	sa.lpSecurityDescriptor = NULL;

	BOOST_VERIFY(CreatePipe(&m_data->m_inputReadPipe, &m_data->m_inputWritePipe, &sa, 0) != 0);
	BOOST_VERIFY(CreatePipe(&m_data->m_outputReadPipe, &m_data->m_outputWritePipe, &sa, 0) != 0);


}



ConsoleTerm::CommandResult ConsoleTerm::execute(StringRef command, ConsoleCommandHandler& handler /* = EmptyHandler() */)
{
	BOOST_ASSERT(!!m_data);

	size_t size = command.size() + 1;
	wchar_t* p_command = new wchar_t[size];
	wcscpy_s(p_command, size, command.c_str());

	STARTUPINFOW        si = { 0 };
	PROCESS_INFORMATION pi = { 0 };

	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	si.wShowWindow = SW_HIDE;
	si.hStdInput = m_data->m_inputReadPipe;
	si.hStdOutput = m_data->m_outputWritePipe;
	si.hStdError = m_data->m_outputWritePipe;

	auto operation_handler = m_asyncManager.createHandler(handler, m_data->m_outputReadPipe, m_data->m_inputWritePipe);
	
	if (!CreateProcessW(NULL, p_command, NULL, NULL, TRUE, 0/*CREATE_NEW_PROCESS_GROUP*/, NULL, NULL, &si, &pi))
	{
		throw RunCommandExpection();
	}

	WaitForSingleObject(pi.hProcess, m_data->m_maxExecTime);


	


	return 0;

}

