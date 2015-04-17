//#include "stdafx.h"
#include "CommandLineParser.h"

#include <algorithm>

#include "../Exceptions/Exceptions.h"
#include "../DebugLogger/debug_logger.h"

#include "stdlib.h"
#include "tchar.h"

using namespace std;

CCommandLineParser::CCommandLineParser(void)
{
	DEBUG_LOGGER_DECLARE
	m_argPointersVector.resize(0);
}

CCommandLineParser::~CCommandLineParser(void)
{
	DEBUG_LOGGER_DECLARE
}

bool CCommandLineParser::Init( int argc,char* argv[] )
{
	DEBUG_LOGGER_DECLARE

	m_params.clear();
	for (int i=0/*1*/; i< argc; ++i ) {
		m_params.push_back(argv[i]);
	}

	UpdateArgPointersVector();
	
	return true;
}


bool CCommandLineParser::Init()
{
	DEBUG_LOGGER_DECLARE
	return Init(__argc, __argv);
}

void CCommandLineParser::Clear()
{
	DEBUG_LOGGER_DECLARE
	m_argPointersVector.clear();
	m_params.clear();
}

bool CCommandLineParser::CheckParam( const char* paramName ) const
{
	DEBUG_LOGGER_DECLARE
	std::list<std::string>::const_iterator it;
	return FindParam(paramName, it);
}

bool CCommandLineParser::GetParamValue( const char* paramName, string &retValue ) const
{
	DEBUG_LOGGER_DECLARE
	std::list<std::string>::const_iterator it;
	if ( FindParam(paramName, it) ) {
		++it; 
		if (it != m_params.end()) {
			retValue = *it;
		} else return false;
		return true;
	}
	return false;
}

bool CCommandLineParser::FindParam( const char* paramName, std::list<std::string>::const_iterator& it ) const
{
	DEBUG_LOGGER_DECLARE
	it = find(m_params.begin(),m_params.end(),paramName);
	if (it != m_params.end())
		return true;
	else 
		return false;
}

void CCommandLineParser::UpdateArgPointersVector()
{
	DEBUG_LOGGER_DECLARE
	m_argPointersVector.resize(0);
	
	for (auto it = m_params.begin(); it != m_params.end(); ++it) {
		m_argPointersVector.push_back((char*)it->c_str());
	}
}

void CCommandLineParser::AddParam( const char* paramName, const char* paramValue /*= 0*/ )
{
	DEBUG_LOGGER_DECLARE
	m_params.push_back(paramName);
	m_argPointersVector.push_back((char*)m_params.back().c_str());
	
	if (paramValue) {
		m_params.push_back(paramValue);
		m_argPointersVector.push_back((char*)m_params.back().c_str());
	}
}

void CCommandLineParser::AddParamsFromString( const char* paramsString )
{
	DEBUG_LOGGER_DECLARE
	string paramsLine(paramsString);
	string paramsElement;
	
	size_t i = 0;
	while(i < paramsLine.length()) {
		if (paramsLine[i] == L' ') { /* skip spaces */
			++i;
			continue;
		}

		if (paramsLine[i] == L'"' || paramsLine[i] == L'\'') { /* get element enclosed in double or single quotes */
			size_t beg = i+1;
			char quotesSymbol = paramsLine[i]; 
			i = paramsLine.find(quotesSymbol, beg); /* find closing quotes */
			
			if (i == string::npos) {
				EXCEPTION_THROW_MESSAGE("Closing single or double quotes is not found in the parameter string.");
			}

			paramsElement.clear();
			paramsElement.assign(paramsLine, beg, i-beg);

			m_params.push_back(paramsElement);
			++i;
			continue;
		}

		/* get space-separated element */
		size_t beg = i;
		i = paramsLine.find(L' ', beg); /* find next space */
		if (i == string::npos) { /* no more spaces to the end */
			i = paramsLine.length();
		}

		paramsElement.clear();
		paramsElement.assign(paramsLine, beg, i-beg);

		m_params.push_back(paramsElement);
		++i;
		continue;

	}

	UpdateArgPointersVector();
}

int CCommandLineParser::GetArgC() const
{
	DEBUG_LOGGER_DECLARE
	return m_argPointersVector.size();
}

char** CCommandLineParser::GetArgV()
{
	DEBUG_LOGGER_DECLARE
	return m_argPointersVector.data();
}

std::string CCommandLineParser::GetExecutablePath() const
{
	DEBUG_LOGGER_DECLARE
	std::string exePath;

	if (m_params.size() > 0) {
		exePath = m_params.front();
	}

	return exePath;
}

std::string CCommandLineParser::GetExecutableName() const
{
	DEBUG_LOGGER_DECLARE
	std::string exeName;
	exeName = GetExecutablePath();

	if (!exeName.empty()) {
		size_t pos = exeName.find_last_of("/\\");
		if (string::npos != pos) {
			exeName = exeName.substr(pos+1);
		}
	}
	
	return exeName;
}


