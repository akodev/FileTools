#pragma once
#include <stdio.h>

#include "libs/CommandLineParser/CommandLineParser.h"

#include <iostream>
#include <fstream>
#include <locale>

#include <stdint.h>

class CFileToolsApp {

public:
	CFileToolsApp();
	~CFileToolsApp();

	void Init(int argc, char* argv[]);
	void Clear();

	void DoWork();

private:
	void CalculateChecksum();
	void CountWords();

	void ParseParameters();
	void CheckParameters();

	void PrintHelpMessage();

	bool GetWordFromStream(std::istream& iStream, std::string& word);

	CCommandLineParser m_cmdParser;

	std::string m_workingMode; // words or checksum

	std::string m_inputFileName;
	std::ifstream m_inputFileStream;

	std::string m_wordForCounting;

	unsigned int m_wordCount;

	bool m_showHelp;

	uint32_t m_checksum32;

	bool m_isInitialized;
};
