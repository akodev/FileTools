#include "FileTools.h"
#include "libs/Exceptions/Exceptions.h"


int main(int argc, char* argv[])
{

	try {
		CFileToolsApp fileTools;

		fileTools.Init(argc, argv);
		fileTools.DoWork();
		fileTools.Clear();
	} catch (const CCommonException& ex) {
		std::cout << "Error: " << ex.what() << "\n";
	} catch (...) {
		std::cout << "Error: unknown exception \n"; 
	}

	return 0;
}

