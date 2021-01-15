#include "WordTranslations.h"

/// <summary>
/// Initializes the arrays to be the correct size.
/// </summary>
/// <param name="wordLines"></param>
WordTranslations::WordTranslations(int wordLines)
{

	std::wcout << "WordTranslations " << wordLines << std::endl;

	//Allocate memory for the arrays
	finnishWords = new std::wstring[wordLines];
	swedishWords = new std::wstring[wordLines];;
	englishWords = new std::wstring[wordLines];;

	
	// Initialize all elements to be empty.
	for (int i = 0; i < wordLines; i++) 
	{
		finnishWords[i] = L"";
		swedishWords[i] = L"";
		englishWords[i] = L"";
	}
}

WordTranslations::~WordTranslations()
{
	if (finnishWords != nullptr)
	{
		delete[] finnishWords;
		finnishWords = nullptr;
	}
	if (swedishWords != nullptr)
	{
		delete[] swedishWords;
		swedishWords = nullptr;
	}
	if (englishWords != nullptr)
	{
		delete[] englishWords;
		englishWords = nullptr;
	}
}

std::wstring WordTranslations::GetFinnishWord(int index) const
{
	if (finnishWords != nullptr)
	{
		return finnishWords[index];
	}
}

std::wstring WordTranslations::GetSwedishWord(int index) const
{
	if (swedishWords != nullptr)
	{
		return swedishWords[index];
	}
}


std::wstring WordTranslations::GetEnglishWord(int index) const
{
	if (englishWords != nullptr)
	{
		return englishWords[index];
	}
}

void WordTranslations::SetFinnishWord(std::wstring word, int index)
{
	if (finnishWords != nullptr) 
	{
		finnishWords[index] = word;
	}
}

void WordTranslations::SetSwedishWord(std::wstring word, int index)
{	 
	if (swedishWords != nullptr)
	{
		swedishWords[index] = word;
	}
}

void WordTranslations::SetEnglishWord(std::wstring word, int index)
{
	if (englishWords != nullptr)
	{
		englishWords[index] = word;
	}
}


