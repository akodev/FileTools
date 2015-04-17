#pragma once

#include <exception> 
#include <string>

#include <iostream>
#include <sstream>

#include <stdio.h>


/* exception class for use anywhere */
class CCommonException : public std::exception
{	
public:
	CCommonException(const char* message = 0, const char* file = 0, int line = 0, const char* function = 0);

	~CCommonException() throw();

	virtual const char* what() const throw();

private:
	std::string m_exceptionText;
};

/* some convenient macro */

#define EXCEPTION_THROW throw CCommonException();
#define EXCEPTION_THROW_MESSAGE(message) throw CCommonException(message);

#define EXCEPTION_THROW_WITH_INFO throw CCommonException(0, __FILE__,  __LINE__, __FUNCTION__);
#define EXCEPTION_THROW_MESSAGE_WITH_INFO(message) throw CCommonException(message, __FILE__,  __LINE__, __FUNCTION__);


