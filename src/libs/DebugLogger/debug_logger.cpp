// debug_logger.cpp : Defines the entry point for the console application.
//
#include "debug_logger.h"

#ifdef DEBUG_LOGGER_INCLUDE_IN_BUILD

#include <ctime>
#include <string>
#include "windows.h"

#include "..\Exceptions\Exceptions.h"

#define USE_BOOST_LIB

#ifdef USE_BOOST_LIB
#include "boost/date_time/posix_time/posix_time.hpp"
#include <boost/filesystem.hpp>
#include <boost/filesystem/path.hpp>
#endif

// globals
int CDebugLogger::mg_logDepth = 0;

HANDLE CDebugLogger::mg_logDepthMutex = 0;
HANDLE CDebugLogger::mg_logFileMutex = 0;

//bool CDebugLogger::mg_isDebugLoggingEnabled = false;

std::ofstream CDebugLogger::mg_logOutput;

bool CDebugLogger::mg_logOnOffState = false;

void CDebugLogger::InitDebugLogger(const char* logPath )
{

	//CDebugLogger::mg_isDebugLoggingEnabled = true;

	CDebugLogger::mg_logOnOffState = false;

	CDebugLogger::mg_logDepth = 0;
	CDebugLogger::mg_logDepthMutex = CreateMutex(NULL, FALSE, NULL);

	if (CDebugLogger::mg_logDepthMutex == INVALID_HANDLE_VALUE) {
		EXCEPTION_THROW_MESSAGE("InitDebugLogger: can't create log depth mutex.")
	}

	CDebugLogger::mg_logFileMutex = CreateMutex(NULL, FALSE, NULL);

	if (CDebugLogger::mg_logFileMutex == INVALID_HANDLE_VALUE) {
		EXCEPTION_THROW_MESSAGE("InitDebugLogger: can't create file mutex.")
	}

	SetOutputFile(logPath);
}


CDebugLogger::CDebugLogger(const char* message ) : m_message(message)
{
	m_isWritesAllowed = mg_logOnOffState;

	if (m_isWritesAllowed == false) {
		return;
	}
	
	WaitForSingleObject(mg_logDepthMutex, INFINITE);
	m_logDepthCurrent = mg_logDepth;
	mg_logDepth++;
	ReleaseMutex(mg_logDepthMutex);

	for (int i = 0; i < m_logDepthCurrent; i++) {
		m_depthMark+=".\t";
	}

	WaitForSingleObject(mg_logFileMutex, INFINITE);

#ifdef USE_BOOST_LIB
	mg_logOutput<< m_depthMark << "> " << m_message << " "<< GetCurrentThreadId() << " " << boost::posix_time::microsec_clock::local_time().time_of_day() << std::endl;
#else 
	mg_logOutput<< m_depthMark << "> " << m_message << " "<< GetCurrentThreadId() << " " << std::time(0)  << std::endl;
#endif

	mg_logOutput.flush();
	ReleaseMutex(mg_logFileMutex);
}

CDebugLogger::~CDebugLogger()
{
	if (m_isWritesAllowed == false) {
		return;
	}
	
	WaitForSingleObject(mg_logDepthMutex, INFINITE);
	mg_logDepth--;
	ReleaseMutex(mg_logDepthMutex);

	WaitForSingleObject(mg_logFileMutex, INFINITE);


#ifdef USE_BOOST_LIB
	mg_logOutput << m_depthMark << "< " << m_message << " "<< GetCurrentThreadId() << " " << boost::posix_time::microsec_clock::local_time().time_of_day() << std::endl;
#else 
	mg_logOutput << m_depthMark << "< " << m_message << " "<< GetCurrentThreadId() << " " << std::time(0) << std::endl;
#endif

	mg_logOutput.flush();
	ReleaseMutex(mg_logFileMutex);
}

void CDebugLogger::WriteToLog( char* message )
{
	if (m_isWritesAllowed == false) {
		return;
	}

	WaitForSingleObject(mg_logFileMutex, INFINITE);

#ifdef USE_BOOST_LIB
	mg_logOutput << m_depthMark << "! " << message << " "<< GetCurrentThreadId() << " " << boost::posix_time::microsec_clock::local_time().time_of_day() << std::endl;
#else 
	mg_logOutput << m_depthMark << "! " << message << " "<< GetCurrentThreadId() << " " << std::time(0) << std::endl;
#endif

	mg_logOutput.flush();
	ReleaseMutex(mg_logFileMutex);
	
}

void CDebugLogger::CloseDebugLogger()
{
	WaitForSingleObject(mg_logFileMutex, INFINITE);
	
	if (mg_logOutput.is_open()) {
		mg_logOutput.close();
	} 

	ReleaseMutex(mg_logFileMutex);
}

void CDebugLogger::SetOutputFile(const char* path )
{
	WaitForSingleObject(mg_logFileMutex, INFINITE);

	CDebugLogger::mg_logOutput.exceptions(0); /* disable exceptions */

	if (mg_logOutput.is_open()) {
		mg_logOutput.close();
	} 
	
#ifdef USE_BOOST_LIB
	/* create directory if not exists */
	try {
		boost::filesystem::path dirPath(path);
		dirPath.remove_filename();
		boost::filesystem::create_directories(dirPath);
	} catch (...) {
		std::cerr  << "Exception on debug log directories creation.\n";
	}
#endif

	CDebugLogger::mg_logOutput.open(path);

	if (CDebugLogger::mg_logOutput.fail() ) {
		ReleaseMutex(mg_logFileMutex);
		EXCEPTION_THROW_MESSAGE_WITH_INFO("Failed to open debug log file.")
	}

	ReleaseMutex(mg_logFileMutex);
}

void CDebugLogger::SetOnOff( bool bOnOff )
{
	CDebugLogger::mg_logOnOffState = bOnOff;
}
#endif //DEBUG_LOGGER_INCLUDE_IN_BUILD



//#define DEBUG_LOGGER_BUILD_TEST_APP

#ifdef DEBUG_LOGGER_BUILD_TEST_APP

void Test3() {
	DEBUG_LOGGER_DECLARE
	Sleep(1000);
	DEBUG_LOGGER_WRITE("End of Test3()")
	DEBUG_LOGGER_WRITE("End of Test3() again")
}

void Test2() {
	DEBUG_LOGGER_DECLARE
	Sleep(1000);
	Test3();
	//DEBUG_LOGGER_TURN_ON
}

void Test1() {
	DEBUG_LOGGER_DECLARE
	Sleep(1000);
	Test2();
}

DWORD WINAPI thread1(void *param) {
	Test1();
	return 0;
}


int _tmain(int argc, _TCHAR* argv[])
{
	DEBUG_LOGGER_INIT("c:\\test_log.txt")

	//DEBUG_LOGGER_TURN_OFF
	//DEBUG_LOGGER_TURN_ON

	DEBUG_LOGGER_DECLARE

	DEBUG_LOGGER_WRITE("test")
	Test1();

	HANDLE m_hThread = CreateThread(NULL, 0, thread1, 0, 0, NULL);
	HANDLE m_hThread2 = CreateThread(NULL, 0, thread1, 0, 0, NULL);
	HANDLE m_hThread3 = CreateThread(NULL, 0, thread1, 0, 0, NULL);
	WaitForSingleObject(m_hThread3, INFINITE);

	//DEBUG_LOGGER_CLOSE

	return 0;
}
#endif //DEBUG_LOGGER_BUILD_TEST_APP