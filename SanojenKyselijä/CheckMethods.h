#pragma once

#include <string>
#include <time.h>


/// <summary>
/// Has some helper functions so they can be used easily in many classes
/// </summary>
class CheckMethods
{
public:
	static std::wstring ReturnLanguageString(int word);
	static int ReturnLanguageCode(std::wstring word);
	static bool CheckIfLegalLanguageIndex(int index);
	static std::string CurrentDateTime();
};

