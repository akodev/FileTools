#include "Exceptions.h"
#include "../DebugLogger/debug_logger.h"

CCommonException::CCommonException(const char* message, const char* file, int line, const char* function) {

	DEBUG_LOGGER_DECLARE
	m_exceptionText.append( "exception message: ");
	if (message) {
		m_exceptionText.append(message);
	} else {
		m_exceptionText.append("Not specified.");
	}

	if (file) {
		m_exceptionText.append("\nfile: ");
		m_exceptionText.append(file);
	}

	if (line) {
		m_exceptionText.append("\nline: ");
		std::stringstream strStream;
		strStream << line;
		m_exceptionText.append(strStream.str());
	}

	if (function) {
		m_exceptionText.append("\nfunction: ");
		m_exceptionText.append(function);
	}
}

CCommonException::~CCommonException() throw() {
	DEBUG_LOGGER_DECLARE
}


const char* CCommonException::what() const throw()
{
	DEBUG_LOGGER_DECLARE
	return m_exceptionText.c_str();
}
