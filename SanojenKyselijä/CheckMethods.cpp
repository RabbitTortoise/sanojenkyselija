#include "CheckMethods.h"



/// <summary>
/// This function returns language index from the given string.  0 = Finnish, 1 = Swedish, 2 = English
/// </summary>
/// <param name="word"></param>
/// <returns>Language Index. -1 Means something went wrong.</returns>
std::wstring CheckMethods::ReturnLanguageString(int word)
{
    if (word == 0)
    {
        return L"Suomi";
    }
    else if (word == 1)
    {
        return L"Ruotsi";
    }
    else if (word == 2)
    {
        return L"Englanti";
    }
    else
    {
        return L"";
    }
}


/// <summary>
/// This function returns language index from the given string.  0 = Finnish, 1 = Swedish, 2 = English
/// </summary>
/// <param name="word"></param>
/// <returns>Language Index. -1 Means something went wrong.</returns>
int CheckMethods::ReturnLanguageCode(std::wstring word)
{
    if (word == L"Suomi")
    {
        return 0;
    }
    else if (word == L"Ruotsi")
    {
        return 1;
    }
    else if (word == L"Englanti")
    {
        return 2;
    }
    else
    {
        return -1;
    }
}

/// <summary>
/// Checks if the given index is valid.
/// </summary>
/// <param name="index"></param>
/// <returns></returns>
bool CheckMethods::CheckIfLegalLanguageIndex(int index)
{
    //Currently only 2 languages are supported. 0 = Finnish, 1 = Swedish, 2 = English
    if (index >= 0 && index <= 2)
    {
        return true;
    }
    return false;
}

/// <summary>
/// Returns the current date, format is YYYY-MM-DD HH:mm:ss
/// </summary>
/// <returns></returns>
std::string CheckMethods::CurrentDateTime() {
    struct tm newtime;
    time_t now = time(0);
    localtime_s(&newtime, &now);
    std::string date = std::to_string(newtime.tm_year + 1900) + "-" +
        std::to_string(newtime.tm_mon + 1) + "-" +
        std::to_string(newtime.tm_mday) + " " +
        std::to_string(newtime.tm_hour) + ":" +
        std::to_string(newtime.tm_min) + ":" +
        std::to_string(newtime.tm_sec);
    return date;
}
