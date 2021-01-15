#pragma once


#include <string>
#include <iostream>
#include <vector>
#include <filesystem>
#include "WordFile.h"

/// <summary>
/// This class contains functions for the main menu and tests.
/// </summary>
class WordAsker
{
public:
	WordAsker();
	~WordAsker();

	bool StartMenu();

private:
	std::vector<WordFile*> wordFiles;
	int filesTotal = 0;
	std::wstring folderName = L"Sanakokeet/";
	std::vector<std::wstring> filePaths;
	std::vector<bool> fileReadable;
	std::wstring* wordsToTranslate = nullptr;
	std::wstring* answers = nullptr;

	void FindFiles();
	void AskWords(int wordsInTotal, std::wstring* wordsToTranslate, std::wstring* answers, int& correctAnswers);

};