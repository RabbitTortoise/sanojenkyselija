#pragma once

#include <string>
#include <fstream>

/// <summary>
/// Simple error logger. Logs the given string to a file.
/// </summary>
class ErrorLogger
{
public:
	static void SaveErrorMessageToFile(std::wstring error);
	static void SaveErrorMessageToFile(std::string error);
	
private:
	static const std::wstring logPath;
};

