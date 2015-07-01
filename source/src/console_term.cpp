#include "console_term.hpp"

#include "encoding_converter.hpp"

#include "handle_wraper_win.hpp"
#include "exit_scope.hpp"

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

		std::exception_ptr& getResult(){ return m_exeption; }

		void operator()()
		{
			try
			{
				while(m_function());
			}
			catch (...)
			{
				m_exeption = std::current_exception();
			}
		}
	private:
		std::function<bool()> m_function;
		std::exception_ptr    m_exeption;
	};
public:
	class AsyncHandler
	{
	public:
		typedef std::shared_ptr<AsyncHandler> Ptr;
		AsyncHandler(){}
		void join(){ m_threadGroup.join_all(); }
		void checkExeption(){
			if (m_writeTask.getResult())
				std::rethrow_exception(m_readTask.getResult());
			if (m_readTask.getResult())
				std::rethrow_exception(m_readTask.getResult());
		}
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

				return !!result;
			});

	
	return result;
}


void ConsoleCommandLinesHandler::flush()
{
	if (!m_writeCache.empty())
		writeLine(m_writeCache);
	m_writeCache.clear();
}

void ConsoleCommandLinesHandler::output(StringRef input)
{
	m_writeCache += input;

	String::size_type pos;
	while ((pos = m_writeCache.find_first_of(L'\n')) != String::npos)
	{
		String::size_type possend = pos;
		if (possend > 0 && m_writeCache[possend-1] == L'\r')
			--possend;

		String tosend = m_writeCache.substr(0, possend);
		m_writeCache.erase(0, pos + 1);
		writeLine(tosend);
	}
}

String ConsoleCommandLinesHandler::input()
{
	String result;
	if (m_readCache.empty())
		result = readLine();
	else
	{
		result = m_readCache;
		m_readCache.clear();
	}

	if (!result.empty())
	{
		String::size_type pos = result.find_first_of(L'\n');

		if (pos == String::npos)
		{
			result += L"\n";
		}
		else
		{
			String::size_type end_pos = pos+1;
			if (pos + 1 < result.size())
			{
				m_readCache = result.substr(pos + 1);
				result.erase(pos + 1);
			}
		}
	}
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
	using namespace hd::tools;

	size_t size = command.size() + 1;
	wchar_t* p_command = new wchar_t[size];
	wcscpy_s(p_command, size, command.c_str());
	EXIT_SCOPE({ delete[] p_command; });

	SECURITY_ATTRIBUTES sa = { 0 };
	sa.nLength = sizeof(sa);
	sa.bInheritHandle = TRUE;
	sa.lpSecurityDescriptor = NULL;

	HANDLE tmp_handle1, tmp_handle2;

	BOOST_VERIFY(CreatePipe(&tmp_handle1, &tmp_handle2, &sa, 0) != 0);
	HHANDLE inputReadPipe(tmp_handle1);
	HHANDLE inputWritePipe(tmp_handle2);

	BOOST_VERIFY(CreatePipe(&tmp_handle1, &tmp_handle2, &sa, 0) != 0);
	HHANDLE outputReadPipe(tmp_handle1);
	HHANDLE outputWritePipe(tmp_handle2);

	STARTUPINFOW        si = { 0 };
	PROCESS_INFORMATION pi = { 0 };

	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	si.wShowWindow = SW_HIDE;
	si.hStdInput = inputReadPipe.impl();
	si.hStdOutput = outputWritePipe.impl();
	si.hStdError = outputWritePipe.impl();

	auto operation_handler = m_asyncManager.createHandler(handler, m_data->m_converter, outputReadPipe.impl(), inputWritePipe.impl());

	if (!CreateProcessW(NULL, p_command, NULL, NULL, TRUE, 0/*CREATE_NEW_PROCESS_GROUP*/, NULL, NULL, &si, &pi))
	{
		throw RunCommandExpection();
	}

	HHANDLE command_thread(pi.hThread);
	HHANDLE command_process(pi.hProcess);

	DWORD wait_result = WaitForSingleObject(pi.hProcess, m_data->m_maxExecTime);

	if(wait_result != WAIT_OBJECT_0)
	{
		TerminateProcess(command_process.impl(), -1);
		throw RunCommandExpection();
	}

	ConsoleTerm::CommandResult result = -1;
	GetExitCodeProcess(command_process.impl(), &result);

	inputReadPipe.close();
	outputWritePipe.close();

	operation_handler->join();
	operation_handler->checkExeption();

	return result;

}

