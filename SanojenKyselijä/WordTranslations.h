#pragma once

#include <string>
#include <iostream>

/// <summary>
/// This class contains all the words in a file in arrays. It is used for data storage.
/// </summary>
class WordTranslations
{
public:
	WordTranslations(int wordLines);
	~WordTranslations();

	std::wstring GetFinnishWord(int index) const;
	std::wstring GetSwedishWord(int index) const;
	std::wstring GetEnglishWord(int index) const;

	void SetFinnishWord(std::wstring word, int index);
	void SetSwedishWord(std::wstring word, int index);
	void SetEnglishWord(std::wstring word, int index);

private:
	
	std::wstring* finnishWords = nullptr;
	std::wstring* swedishWords = nullptr;
	std::wstring* englishWords = nullptr;
};

