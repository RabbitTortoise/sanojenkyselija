#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <io.h>
#include <vector>

#include "CheckMethods.h"
#include "WordTranslations.h"
#include "ErrorLogger.h"


/// <summary>
/// This class contains all the information about opened file.
/// </summary>
class WordFile
{
public:
	WordFile(std::wstring path);
	~WordFile();

	bool GetFileStructureCorrect() const { return fileStructureCorrect; };
	bool GetReadingSuccessful() const { return readingSuccessful; };
	WordTranslations* GetFileWords() const { return wordTranslations; };
	int GetWordLines() const { return wordLines; };
	std::wstring GetFilePath() const { return filePath; };

private:
	std::wstring filePath;

	bool fileStructureCorrect = false;
	bool readingSuccessful = false;
	int wordLines = -1;
	std::vector<int> languageOrder;

	bool hasFinnishWords = false;
	bool hasSwedishWords = false;
	bool hasEnglishWords = false;

	WordTranslations* wordTranslations;
	

	bool CheckFileContents();
	bool ReadWordsFromFile();
};

