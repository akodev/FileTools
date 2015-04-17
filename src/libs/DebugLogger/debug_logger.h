#pragma once


// define this symbol if you want to use debug logger:
//#define DEBUG_LOGGER_INCLUDE_IN_BUILD


#ifdef DEBUG_LOGGER_INCLUDE_IN_BUILD

#define DEBUG_LOGGER_INIT(logFilePath) CDebugLogger::InitDebugLogger(logFilePath);
#define DEBUG_LOGGER_CLOSE CDebugLogger::CloseDebugLogger();

#define DEBUG_LOGGER_LOCAL_OBJECT_NAME __debugLogger
#define DEBUG_LOGGER_DECLARE CDebugLogger DEBUG_LOGGER_LOCAL_OBJECT_NAME(__FUNCTION__);
#define DEBUG_LOGGER_WRITE(message) DEBUG_LOGGER_LOCAL_OBJECT_NAME.WriteToLog(message);

#define DEBUG_LOGGER_TURN_ON CDebugLogger::SetOnOff(true);
#define DEBUG_LOGGER_TURN_OFF CDebugLogger::SetOnOff(false);


#include <iostream>
#include <fstream>
#include <string>


class CDebugLogger {
public:
	explicit CDebugLogger(const char* message);
	~CDebugLogger();

	void WriteToLog(char* message);

	static void InitDebugLogger(const char* logPath);
	static void CloseDebugLogger();
	static void SetOutputFile(const char* path);
	static void SetOnOff(bool bOnOff);

	std::string m_message;

	int m_logDepthCurrent;
	std::string m_depthMark;

	bool m_isWritesAllowed;

	// globals 
	static int mg_logDepth;

	static void* mg_logDepthMutex;
	static void* mg_logFileMutex;

	//static bool mg_isDebugLoggingEnabled;

	static std::ofstream mg_logOutput;

	static bool mg_logOnOffState;
};


#else

#define DEBUG_LOGGER_INIT(logFilePath) 
#define DEBUG_LOGGER_CLOSE

#define DEBUG_LOGGER_LOCAL_OBJECT_NAME
#define DEBUG_LOGGER_DECLARE
#define DEBUG_LOGGER_WRITE(message)

#define DEBUG_LOGGER_TURN_ON
#define DEBUG_LOGGER_TURN_OFF

#endif //DEBUG_LOGGER_INCLUDE_IN_BUILD

