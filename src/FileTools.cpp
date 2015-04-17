#include "FileTools.h"
#include "libs/Exceptions/Exceptions.h"

#include <string.h>

const std::string WORKING_MODE_WORDS = "words";
const std::string WORKING_MODE_CHECKSUM = "checksum";

CFileToolsApp::CFileToolsApp() : m_isInitialized(false)
{

}

CFileToolsApp::~CFileToolsApp()
{
	Clear();
}


void CFileToolsApp::PrintHelpMessage()
{
	std::cout << "File Tools utility. \n"
				"Abilities: \n"
				"\t- Count occurrences of the word in file. \n"
				"\t- Calculate checksum for file (checksum = word32_1 + word32_2 + ... + word32_N \n"

				"Usage:\n"
				"\tCount words:   FileTools -f <file> -m words -v <word>\n"
				"\tGet checksum:  FileTools -f <file> -m checksum -v <word>\n"
				"\tShow help:     FileTools -h\n";
}

void CFileToolsApp::Init( int argc, char* argv[] )
{
	Clear();
	m_cmdParser.Init(argc, argv);
	m_isInitialized = true;
}

void CFileToolsApp::Clear()
{
	m_isInitialized = false;

	m_cmdParser.Clear();

	m_workingMode.clear();

	m_inputFileName.clear();

	if (m_inputFileStream.is_open()) m_inputFileStream.close();

	m_wordForCounting.clear();

	m_wordCount = 0;

	m_showHelp = false;

	m_checksum32 = 0;
}


void CFileToolsApp::DoWork()
{
	if (!m_isInitialized) {
		EXCEPTION_THROW_MESSAGE("Not initialized. Please, call CFileToolsApp::Init() first.")
	}

	ParseParameters();

	if (m_showHelp) {
		PrintHelpMessage();
		return;
	}

	if (m_workingMode == WORKING_MODE_WORDS) {
		CountWords();
	} else if (m_workingMode == WORKING_MODE_CHECKSUM) {
		CalculateChecksum();
	} else {
		std::string msg = "Unsupported working mode: " + m_workingMode;
		EXCEPTION_THROW_MESSAGE(msg.c_str());
	}
}

bool CFileToolsApp::GetWordFromStream( std::istream& iStream, std::string& word )
{
	word.clear();

	if (!iStream.good()) {
		return false;
	}

	while (iStream.good()) {
		char sym = iStream.get();
		if (!std::isalnum(sym, iStream.getloc())) {
			break;
		}
		word+=sym;
	}

	return true;
}

void CFileToolsApp::ParseParameters()
{
	if (m_cmdParser.CheckParam("-h") || m_cmdParser.GetArgC() == 1/*only executable path*/) {
		m_showHelp = true;
	} else {
		m_showHelp = false;
	}

	m_cmdParser.GetParamValue("-f", m_inputFileName);
	m_cmdParser.GetParamValue("-m", m_workingMode);
	m_cmdParser.GetParamValue("-v", m_wordForCounting);

	CheckParameters();
}

void CFileToolsApp::CountWords()
{
	m_inputFileStream.open(m_inputFileName.c_str());

	if (!m_inputFileStream.good()) {
		EXCEPTION_THROW_MESSAGE("Can't open input file.");
	}

	std::string word;

	while (GetWordFromStream(m_inputFileStream, word)) {
		if (word.compare(m_wordForCounting) == 0) {
			m_wordCount++;
		}
	}

	m_inputFileStream.close();

	std::cout << "Words: " << m_wordCount << "\n";
}

void CFileToolsApp::CalculateChecksum()
{
	m_inputFileStream.open(m_inputFileName.c_str(), std::ios_base::binary);

	if (!m_inputFileStream.good()) {
		EXCEPTION_THROW_MESSAGE("Can't open input file.");
	}

	uint32_t checksum = 0;

	const int wordSizeBytes = 4;
	char word32[wordSizeBytes] = {0,0,0,0};

	while (m_inputFileStream.good()) {
		memset(word32, 0, wordSizeBytes);
		m_inputFileStream.read(word32, wordSizeBytes);
		checksum += *((uint32_t*)word32);
	}

	m_inputFileStream.close();

	std::cout << "Checksum: " << checksum << "\n";
}

void CFileToolsApp::CheckParameters()
{
	if (m_showHelp) { // just show help in DoWork()
		return;
	}

	if (!m_workingMode.empty()) {
		if (m_inputFileName.empty()) {
			EXCEPTION_THROW_MESSAGE("Please, set file path with -f <file>")
		}

		if (m_workingMode == WORKING_MODE_WORDS && m_wordForCounting.empty()) {
			EXCEPTION_THROW_MESSAGE("Please, set word for counting with -v <word>")
		}
	}

}

