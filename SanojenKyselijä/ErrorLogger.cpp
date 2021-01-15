#include "ErrorLogger.h"

const std::wstring ErrorLogger::logPath = L"ErrorLog.txt";

void ErrorLogger::SaveErrorMessageToFile(std::wstring error)
{
	std::wofstream outFile;
	outFile.open(logPath, std::wofstream::app);
	outFile << error << std::endl;
	outFile.close();
}

void ErrorLogger::SaveErrorMessageToFile(std::string error)
{
	std::ofstream outFile;
	outFile.open(logPath, std::ofstream::app);
	outFile << error << std::endl;
	outFile.close();
}

