#include "WordFile.h"



/// <summary>
/// Reads the file from the given path.
/// </summary>
/// <param name="path"></param>
WordFile::WordFile(std::wstring path)
{
    std::wcout << "Wordfile start" << std::endl;
    filePath = path;

    //Check if the file has a valid format for the program to work. The function also saves all required information to fileContents.
    fileStructureCorrect = CheckFileContents();

    //Only progress if we could read the file successfully.
    if (fileStructureCorrect)
    {
        wordTranslations = new WordTranslations(wordLines);
        readingSuccessful = ReadWordsFromFile();
    }
}


WordFile::~WordFile()
{
    if (wordTranslations != nullptr)
    {
        delete wordTranslations;
        wordTranslations = nullptr;
    }
}




/// <summary>
/// Reads the file to find out what language it uses and how many words it has. Returns false if something went wrong.
/// TODO: Currently does not check if file gives two of the same language words, ie. if you are translating from Finnish to Finnish.
/// </summary>
/// <param name="fileContents"></param>
/// <param name="fileName"></param>
/// <returns>Returns true if nothing goes wrong</returns>
bool WordFile::CheckFileContents()
{
    bool error = false;

    std::wstring line;
    std::wifstream myFile;

    //Try to open the file
    myFile.open(filePath, std::ios::binary);
    if (myFile.is_open())
    {
        ErrorLogger::SaveErrorMessageToFile(L"Avataan tiedosto " + filePath);
        //Just to make sure that we are at the beginning of the file and no eof-flags are set.
        myFile.clear();
        myFile.seekg(0);

        //First line should always be formatted like this: [Sanakyselijä][Translate From Language][Translate To Language];
        getline(myFile, line);

        //DEBUG
        //wcout << line << endl;

        //Variables needed in intrepreting the line.
        std::wstring word = L"";
        std::wstring c = L"";
        bool readingWord = false;

        //Things that should be in the header
        std::wstring header = L"";
        std::wstring translateFromLanguage = L"";
        std::wstring translateToLanguage = L"";
        //Track how many header parts have been read.
        int headerCheck = 0;

        //Process the line character by character
        for (int i = 0; i < line.length(); i++)
        {
            //Read one letter to check what to do next.
            c = line.substr(i, 1);

            //DEBUG
            //wcout << c << endl;

            //If letter is '[' it means that a new word is next thing on the line.
            if (c == L"[")
            {
                readingWord = true;
                //Skip to the next character
                continue;
            }
            //If letter is '[' it means that we have finished reading the word.
            else if (c == L"]")
            {
                readingWord = false;
                //In this case where we are reading a header we should get 3 things. The file identification, Language to translate from and Language to translate to.
                if (headerCheck == 0)
                {
                    //DEBUG
                    //wcout << L"headerCheck 0: " << word << endl;

                    //In case the file identification is not "Sanakyselijä" there is an error.
                    if (word != L"Sanakyselijä")
                    {
                        ErrorLogger::SaveErrorMessageToFile(L"Tiedoston ensimmäisen rivin ensimmäinen sana ei ollut \"Sanakyselijä\". Hylätään tiedosto. Sana oli: " + word);

                        error = true;
                        break;
                    }

                    word = L"";
                    headerCheck++;
                }
                else if (headerCheck > 0)
                {
                    //DEBUG
                    //wcout << L"headerCheck 1: " << word << endl;

                    //The next thing should be the first language. If it is not supported, we canno't use the file
                    int language = CheckMethods::ReturnLanguageCode(word);
                    if (language == -1)
                    {
                        ErrorLogger::SaveErrorMessageToFile(L"Tiedostosta löytyvää kieltä ei pystytty tulkitsemaan. Pyyntö oli: " + word);
                        error = true;
                        break;
                    }

                    //Store the next language to langu
                    languageOrder.push_back(language);

                    if (language == 0)
                    {
                        hasFinnishWords = true;
                    }
                    else if (language == 1)
                    {
                        hasSwedishWords = true;
                    }
                    else if (language == 2)
                    {
                        hasEnglishWords = true;
                    }

                    word = L"";
                    headerCheck++;
                } 
                continue;
            }
            //We have reached the end of line character, stop processing the line.
            else if (c == L";")
            {
                //Make sure all the header checks are done
                if (headerCheck < 2)
                {
                    ErrorLogger::SaveErrorMessageToFile(L"Otsikon luku päättyi ennen kuin saatiin kahta kieltä luettua! Seassa saattaa olla ';'-merkki väärässä kohdassa. Kyseinen rivi: " + line);
                    error = true;
                }
                break;
            }

            //Add to the word while it's reading is still in proggress.
            if (readingWord)
            {
                word += line.substr(i, 1);
            }

        }

        //Make sure all the header checks are done
        if (headerCheck < 2)
        {
            ErrorLogger::SaveErrorMessageToFile(L"Otsikon kaikkia tietoja ei onnistuttu lukemaan! Kyseinen rivi: " + line);
            error = true;
        }


        //Header checks are done. Only continue if we had no errors.
        //From this point on all the lines should have translatable words. So we just loop through all the remaining lines.
        if (!error)
        {
            //Keep track how many words there are to read in the file
            int wordLinesInTotal = 0;
            //Loop through all the remaining lines
            while (getline(myFile, line) && !error)
            {
                //DEBUG
                //wcout << line << endl;

                //Variables needed in intrepreting the line.
                std::wstring word = L"";
                std::wstring c = L"";
                bool readingWord = false;
                //Variables we can read from the translate line.
                int language = 0;
                std::wstring readWord = L"";
                //This is used to keep track how many words we have read successfully:
                int wordsRead = 0;
                //Check used in the logic
                bool nextIsLanguageIndex = true;

                //Process the line character by character
                for (int i = 0; i < line.length(); i++)
                {
                    //Read one letter to check what to do next.
                    c = line.substr(i, 1);

                    //DEBUG
                    //wcout << c << endl;;

                    //If letter is '[' it means that a new word is next thing on the line. If readingWord is still true it means that we missed "]"
                    if (c == L"[" && readingWord)
                    {
                        ErrorLogger::SaveErrorMessageToFile(L"Sanan luvun lopetusmerkkiä ']' ei tullut vastaan!");
                        error = true;
                        break;
                    }
                    else if (c == L"[")
                    {
                        readingWord = true;
                        //Skip to the next character
                        continue;
                    }

                    //If letter is '[' it means that we have finished reading the word.
                    if (c == L"]")
                    {
                        readingWord = false;

                        //DEBUG
                        //wcout << word << endl;

                        //Check word validity
                        if (nextIsLanguageIndex)
                        {
                            nextIsLanguageIndex = false;
                            int index = stoi(word);
                            if (CheckMethods::CheckIfLegalLanguageIndex(index))
                            {
                                language = index;
                            }
                            else
                            {
                                
                                ErrorLogger::SaveErrorMessageToFile(L"Sanan kieli-indeksi ei ole hyväksyttävä! Indeksi oli: " + std::to_wstring(index));
                                error = true;
                                break;
                            }
                        }
                        //If this part is the actual word just store it and continue.
                        else
                        {
                            readWord = word;
                            wordsRead++;
                            nextIsLanguageIndex = true;
                        }

                        //Once the word is processed clear it.
                        word = L"";
                        continue;
                    }
                    //We have reached the end of line character, stop processing the line.
                    if (c == L";")
                    {
                        //Check that we have read the both words in the line.
                        if (wordsRead < 2)
                        {
                            //DEBUG
                            //wcout << wordsRead << endl;
                            ErrorLogger::SaveErrorMessageToFile(L"Sanarivin luku keskeytyi ennen kuin kaikki sanat saatiin luettua! Seassa saattaa olla ';'-merkki väärässä kohdassa. Kyseinen rivi: " + line);
                            error = true;
                            break;
                        }
                        //This line is read, skip to next line:
                        continue;
                    }

                    //Add to the word while it's reading is still in proggress.
                    if (readingWord)
                    {
                        word += line.substr(i, 1);
                    }
                }

                //Make sure the line actually had words to read:
                if (wordsRead < 2)
                {
                    //DEBUG
                    //wcout << wordsRead << endl;
                    ErrorLogger::SaveErrorMessageToFile(L"Rivistä ei jostain syystä saatu kaikkia sanoja luettua. Kyseinen rivi: " + line);
                    error = true;
                }

                //If there was no error, count how many word lines there has been in total
                if (!error)
                {
                    wordLinesInTotal++;
                }
                else
                {
                    ErrorLogger::SaveErrorMessageToFile(L"Jokin meni pieleen sanarivin lukemisessa! Kyseinen rivi: " + line);
                    break;
                }
            }

            //If there was no error, store the word line count to fileContents
            if (!error)
            {
                wordLines = wordLinesInTotal;

                //DEBUG
                //wcout << "Sanoja käännettäväksi löytyi yhteensä: " << wordLinesInTotal << endl;

            }
            if (error)
            {
                ErrorLogger::SaveErrorMessageToFile(L"JOKU MENI PIELEEN TIEDOSTON SISÄLLÖN LUKEMISESSA!");
            }
        }
    }
    //IF there was a problem in opening the file, print an error message
    else
    {
        error = true;
        ErrorLogger::SaveErrorMessageToFile(L"Jokin meni pieleen tiedoston " + filePath + L" avaamisessa!");
    }

    ErrorLogger::SaveErrorMessageToFile(L"Suljetaan tiedosto " + filePath);

    //Close the file if it is still open
    if (!myFile.is_open())
    {
        myFile.close();
    }

    //If there was an error, return false
    if (error)
    {
        readingSuccessful = false;
        return false;
    }

    readingSuccessful = true;
    return true;
}

/// <summary>
/// Reads all translateble words from the file and stores them in the arrays
/// </summary>
/// <param name="wordstoTranslate"></param>
/// <param name="correctAnswers"></param>
/// <param name="wordsInTotal"></param>
/// <param name="fileName"></param>
/// <returns></returns>
bool WordFile::ReadWordsFromFile()
{
    bool error = false;

    std::wstring line;
    std::wifstream myFile;

    //Try to open the file
    myFile.open(filePath, std::ios::binary);
    if (myFile.is_open())
    {
        //Just to make sure that we are at the beginning of the file and no eof-flags are set.
        myFile.clear();
        myFile.seekg(0);

        //First line is always the header. We have checked it before so we can just skip it now.
        getline(myFile, line);

        //We know how many lines there are in the file so we can just loop through them.
        for (int lineNumber = 0; lineNumber < wordLines; lineNumber++)
        {
            //Process every line
            getline(myFile, line);

            //DEBUG
            //wcout << line << endl;;

            //Variables needed in intrepreting the line.
            std::wstring word = L"";
            std::wstring c = L"";
            bool readingWord = false;
            //Variables we can read from the translate line.
            int language = 0;
            std::wstring readWord = L"";
            //This is used to keep track how many words we have read successfully:
            int wordsRead = 0;
            //Check used in the logic
            bool nextIsLanguageIndex = true;

            //Process the line character by character. We no longer need to check for errors as we have done that previously.
            for (int character = 0; character < line.length(); character++)
            {
                //Read one letter to check what to do next.
                c = line.substr(character, 1);

                //DEBUG
                //wcout << c << endl;;

                //If letter is '[' it means that a new word is next thing on the line.
                if (c == L"[")
                {
                    readingWord = true;
                    //Skip to the next character
                    continue;
                }
                //If letter is '[' it means that we have finished reading the word.
                if (c == L"]")
                {
                    readingWord = false;

                    //DEBUG
                    //wcout << word << endl;


                    //Stores the Language index and text of the line
                    if (nextIsLanguageIndex)
                    {
                        nextIsLanguageIndex = false;
                        int index = stoi(word);
                        language = index;
                    }
                    else
                    {
                        readWord = word;
                        wordsRead++;
                        nextIsLanguageIndex = true;

                        //Finnish word
                        if (language == 0)
                        {
                            wordTranslations->SetFinnishWord(readWord, lineNumber);
                        }
                        //Swedish word
                        else if (language == 1)
                        {
                            wordTranslations->SetSwedishWord(readWord, lineNumber);
                        }
                        //English word
                        else if (language == 2)
                        {
                            wordTranslations->SetEnglishWord(readWord, lineNumber);
                        }
                    }

                    word = L"";
                    continue;
                    
                }
                //We have reached the end of line character, stop processing the line.
                if (c == L";")
                {
                    //This line is read, skip to next line:
                    continue;
                }
                //Add to the word while it's reading is still in proggress.
                if (readingWord)
                {
                    word += line.substr(character, 1);
                }
            }

            
        }
    }
    //If the file could not be opened, print an error message.
    else
    {
        error = true;
        ErrorLogger::SaveErrorMessageToFile(L"Jokin meni pieleen tiedoston " + filePath + L" avaamisessa!");
    }
    //Close the file if it's still open.
    if (!myFile.is_open())
    {
        myFile.close();
    }
    //Return false if there was an error with file opening.
    if (error)
    {
        return false;
    }

    return true;
}


