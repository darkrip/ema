#include "console_term.hpp"

#include "encoding_converter.hpp"

#include <boost/assert.hpp>
#include <boost/thread.hpp>

#include <windows.h>

using namespace ema;
using namespace console;


class ema::console::ConsoleAsyncManager
{
	class Task
	{
	public:
		Task(){}
		Task(const std::function<bool()>& function):m_function(function){}

		void operator()()
		{
			while(m_function());
		}
	private:
		std::function<bool()> m_function;
	};
public:
	class AsyncHandler
	{
	public:
		typedef std::shared_ptr<AsyncHandler> Ptr;
		AsyncHandler(){}
		void join(){ m_threadGroup.join_all(); }
	private:
		friend class ConsoleAsyncManager;
		boost::thread_group m_threadGroup;
		Task m_readTask;
		Task m_writeTask;

	};
	AsyncHandler::Ptr createHandler(ConsoleCommandHandler& handler, hd::tools::EncodingConverter& converter, HANDLE outputReadPipe, HANDLE inputWritePipe);
private:

	template<typename Functor>
	Task createTask(AsyncHandler& ahandler, Functor & functor)
	{
		Task task(functor);
		ahandler.m_threadGroup.create_thread(task);
		return task;
	}
};

ConsoleAsyncManager ConsoleTerm::m_asyncManager;


struct ema::console::ConsoleTermData
{
	ConsoleTermData(StringRef encoding): m_converter(encoding){}
	bool m_isDebug;
	hd::tools::EncodingConverter m_converter;
	int m_maxExecTime;
};

#include <iostream>
ConsoleAsyncManager::AsyncHandler::Ptr
	ConsoleAsyncManager::createHandler(ConsoleCommandHandler& handler, hd::tools::EncodingConverter& converter, HANDLE outputReadPipe, HANDLE inputWritePipe)
{
	AsyncHandler::Ptr result(new AsyncHandler);

	result->m_readTask = createTask(*result, [outputReadPipe, &handler, &converter]()->bool
			{
				const DWORD bufferSize=1024*10;
				DWORD reallyReaded = 0;
				std::string buffer;
				buffer.resize(bufferSize);
				char* p_buffer = &buffer[0];
				BOOL result = ReadFile(outputReadPipe, p_buffer, bufferSize, &reallyReaded, NULL);
				std::cout << "Error:" << GetLastError() << std::endl;
				if(!result)
					return false;
				buffer.resize(reallyReaded);
				handler.output(converter.from<String>(buffer));
				return true;
			});

		result->m_readTask = createTask(*result, [inputWritePipe, &handler, &converter]()->bool
			{
				String str = handler.input();
				if(str.empty())
					return false;

				std::string buffer = converter.to<std::string>(str);

				DWORD length;
				BOOL result = WriteFile(inputWritePipe, buffer.c_str(), sizeof(buffer[0])*buffer.size(), &length, NULL);

				std::cout << "Error:" << GetLastError() << std::endl;

				return !!result;
			});

	
	return result;
}




ConsoleTerm::ConsoleTerm()
{
}


void ConsoleTerm::init(StringRef locale, bool isDebug, int max_exec_time)
{
	m_data.reset(new ConsoleTermData(locale));
	m_data->m_isDebug = isDebug;
	m_data->m_maxExecTime = max_exec_time;
}



ConsoleTerm::CommandResult ConsoleTerm::execute(StringRef command, ConsoleCommandHandler& handler /* = EmptyHandler() */)
{
	BOOST_ASSERT(!!m_data);

	size_t size = command.size() + 1;
	wchar_t* p_command = new wchar_t[size];
	wcscpy_s(p_command, size, command.c_str());

	HANDLE m_inputReadPipe;
	HANDLE m_inputWritePipe;

	HANDLE m_outputReadPipe;
	HANDLE m_outputWritePipe;

	SECURITY_ATTRIBUTES sa = { 0 };
	sa.nLength = sizeof(sa);
	sa.bInheritHandle = TRUE;
	sa.lpSecurityDescriptor = NULL;


	BOOST_VERIFY(CreatePipe(&m_inputReadPipe, &m_inputWritePipe, &sa, 0) != 0);
	BOOST_VERIFY(CreatePipe(&m_outputReadPipe, &m_outputWritePipe, &sa, 0) != 0);

	STARTUPINFOW        si = { 0 };
	PROCESS_INFORMATION pi = { 0 };

	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	si.wShowWindow = SW_HIDE;
	si.hStdInput = m_inputReadPipe;
	si.hStdOutput = m_outputWritePipe;
	si.hStdError = m_outputWritePipe;

	auto operation_handler = m_asyncManager.createHandler(handler, m_data->m_converter, m_outputReadPipe, m_inputWritePipe);

	if (!CreateProcessW(NULL, p_command, NULL, NULL, TRUE, 0/*CREATE_NEW_PROCESS_GROUP*/, NULL, NULL, &si, &pi))
	{
		throw RunCommandExpection();
	}

	DWORD wait_result = WaitForSingleObject(pi.hProcess, m_data->m_maxExecTime);

	if(wait_result != WAIT_OBJECT_0)
	{
		TerminateProcess(pi.hProcess, -1);
		throw RunCommandExpection();
	}

	ConsoleTerm::CommandResult result = -1;
	GetExitCodeProcess(pi.hProcess, &result);

	CloseHandle(m_inputReadPipe);
	CloseHandle(m_outputWritePipe);

	operation_handler->join();

	CloseHandle(m_outputReadPipe);
	CloseHandle(m_inputWritePipe);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);


	


	return result;

}

