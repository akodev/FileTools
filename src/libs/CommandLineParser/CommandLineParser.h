#pragma once

/* Simple command line parser. Works with space separated arguments passed to application. UNICODE ready.*/

#include <string>
#include <list>
#include <vector>

class CCommandLineParser
{
public:
	CCommandLineParser(void);
	~CCommandLineParser(void);

	bool Init(int argc, char* argv[]);

	bool Init(); // try to find current process arguments

	void Clear();

	bool CheckParam(const char* paramName) const;

	bool GetParamValue(const char* paramName, std::string &retValue) const;

	int GetArgC() const;
	char** GetArgV();

	void AddParam(const char* paramName, const char* paramValue = 0);

	void AddParamsFromString(const char* paramsString);

	std::string GetExecutablePath() const;
	std::string GetExecutableName() const;;
private:

	void UpdateArgPointersVector();

	bool FindParam(const char* paramName, std::list<std::string>::const_iterator& it) const;

	std::list<std::string> m_params;

	std::vector<char*> m_argPointersVector;
};

