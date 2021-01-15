#include "WordAsker.h"


WordAsker::WordAsker()
{
    FindFiles();
}

WordAsker::~WordAsker()
{
    //Remember to free all memory allocations. We have to use delete[] to delete the whole array
    if (wordsToTranslate != nullptr)
    {
        delete[] wordsToTranslate;
        wordsToTranslate = nullptr;
    }
    if (answers != nullptr)
    {
        delete[] answers;
        answers = nullptr;
    }

    for (int i = 0; i < wordFiles.size(); i++)
    {
        if (wordFiles[i] != nullptr)
        {
            delete wordFiles[i];
            wordFiles[i] = nullptr;
        }
    }
    wordFiles.clear();
}

/// <summary>
/// Prints the start menu to the console. User can choose what file to open. Asks the words from the chosen file.
/// </summary>
/// <returns></returns>
bool WordAsker::StartMenu()
{
    bool incorrectInput = false;
    bool fileNotReadable = false;
    std::wstring a = L"";
    int ans = 0;
    int previousAns = 0;
    std::wstring path = L"";

    //Ask from the user which file we want to open.
    do
    {

        //Empty windows console
        system("cls");

        std::wcout << "|| Sanojenkyselijä (versio 0.2) ||" << std::endl << std::endl;
        std::wcout << "Valitse seuraavista vaihtoehdoista yksi sanakoe tehtäväksi: " << std::endl;

        for (int i = 0; i < filesTotal; i++)
        {
            std::wcout << "(" << (i + 1) << "): \"" << wordFiles[i]->GetFilePath() << "\"";
            if (!wordFiles[i]->GetReadingSuccessful())
            {
                std::wcout << "  [VIRHE TIEDOSTON LUKEMISESSA]";
            }
            std::wcout << std::endl;
        }
        std::wcout << std::endl;
        std::wcout << "(0) Poistu ohjelmasta" << std::endl;
        std::wcout << std::endl;

        if (fileNotReadable)
        {
            std::wcout << "\"" << wordFiles[ans]->GetFilePath() << "\"" << " Tätä tiedostoa ei pystytä lukemaan, valitse jokin toinen tiedosto!" << std::endl << std::endl;
        }

        if (incorrectInput && !fileNotReadable)
        {
            std::wcout << "Anna syötteeksi numero väliltä 0-" << filesTotal << "!" << std::endl << std::endl;
        }

        a = L"";

        incorrectInput = false;
        std::getline(std::wcin, a);
        try 
        {
            ans = std::stoi(a) - 1;
        }
        catch (...)
        {
            ans = -9999;
        }
        

        fileNotReadable = false;

        //Check if answer was in the range of files found. If so and the file was read successfully, continues out of the loop
        if (ans >= 0 && ans < filesTotal)
        {
            if (wordFiles[ans]->GetReadingSuccessful())
            {
                incorrectInput = false;
            }
            else
            {
                previousAns = ans;
                incorrectInput = true;
                fileNotReadable = true;
            }
        }
        else
        {
            incorrectInput = true;
        }
        if (ans == -1)
        {
            return false;
        }

    } while (incorrectInput);



    //As the program may come to this part more than once and the files may be of different sizes, delete the pointer arrays just in case.
    if (wordsToTranslate != nullptr)
    {
        delete[] wordsToTranslate;
        wordsToTranslate = nullptr;
    }

    if (answers != nullptr)
    {
        delete[] answers;
        answers = nullptr;
    }

    //Asks all the words found from the file
    if (wordFiles[ans]->GetReadingSuccessful())
    {
        //Allocate memory for the arrays
        int wordsInTotal = wordFiles[ans]->GetWordLines();
        wordsToTranslate = new std::wstring[wordsInTotal];
        answers = new std::wstring[wordsInTotal];

        // Initialize all elements. Currently this always makes the user translate from Finnish to Swedish.
        for (int i = 0; i < wordsInTotal; i++)
        {
            wordsToTranslate[i] = wordFiles[ans]->GetFileWords()->GetFinnishWord(i);
            answers[i] = wordFiles[ans]->GetFileWords()->GetSwedishWord(i);
        }



        int correctAnswers = 0;
        //Go trough the arrays and ask the words
        AskWords(wordsInTotal, wordsToTranslate, answers, correctAnswers);
        //Empty windows console
        system("cls");

        //Prints the results
        std::wcout << "Tuloksesi: " << correctAnswers << "/" << wordsInTotal << std::endl;
        if (correctAnswers == wordsInTotal)
        {
            std::wcout << std::endl << "Onneksi olkoon! Sait kaikki oikein!" << std::endl;
        }

        std::wcout << std::endl << "Kirjoita jotain jatkaaksesi:" << std::endl;
        std::wstring temp = L"";
        std::getline(std::wcin, temp);
    }
    else
    {
        std::wcout << "Tiedoston lukemisessa tapahtui virhe!" << std::endl;
    }


    //Returns true so the menu loop keeps going.
    return true;
}


/// <summary>
/// Finds all the files inside "Sanakokeet/" and checks their contents
/// </summary>
void WordAsker::FindFiles()
{
    //Finding all files inside /Sanakokeet This ignores all subfolders

    for (std::filesystem::directory_entry p : std::filesystem::directory_iterator(folderName))
    {
        if (p.exists())
        {
            if (!std::filesystem::is_directory(p.path().c_str()))
            {
                filePaths.push_back(p.path().c_str());
                filesTotal++;
            }
        }
    }

    //Check all files in the directory and try to read them
    for (int i = 0; i < filesTotal; i++)
    {
        wordFiles.push_back(new WordFile(filePaths[i]));
    }
}


/// <summary>
/// This function asks all the words found from the chosen file
/// </summary>
/// <param name="wordsInTotal"></param>
/// <param name="fileContents"></param>
/// <param name="wordsToTranslate"></param>
/// <param name="answers"></param>
/// <param name="correctAnswers"></param>
void WordAsker::AskWords(int wordsInTotal, std::wstring* wordsToTranslate, std::wstring* answers, int& correctAnswers)
{
    std::wstring answer = L"";
    std::wstring previousAnswer = L"";

    for (int i = 0; i < wordsInTotal; i++)
    {
        //Empty windows console
        system("cls");

        //DEBUG
        //wcout << "Käännettävä sana: " << wordsToTranslate[i] << " :: Vastaus: " << answers[i] << endl;


        //Printing the UI
        if (i > 0)
        {
            std::wcout << "Oikeita vastauksia tähän mennessä: " << correctAnswers << "/" << wordsInTotal << std::endl;
            std::wcout << "Edellinen sana: " << wordsToTranslate[i - 1] << ", Vastauksesi oli: " << previousAnswer << ", Oikea vastaus oli: " << answers[i - 1] << std::endl << std::endl;
        }

        //ERROR
        //std::wcout << "Kääntäminen  kielestä " << HelperClass::ReturnLanguageString(fileContents.translateFromLanguage) << " kieleen " << HelperClass::ReturnLanguageString(fileContents.tranfslateToLanguage) << std::endl;
        std::wcout << "Käännä sana: " << wordsToTranslate[i] << std::endl << std::endl;

        answer = L"";
        std::getline(std::wcin, answer);
        previousAnswer = answer;


        //Check if the answer was correct
        if (answer == answers[i])
        {
            std::wcout << "Oikein meni!" << std::endl;
            correctAnswers++;

            std::wcout << "Kirjoita jotain jatkaaksesi:" << std::endl;
            std::wstring temp = L"";
            std::getline(std::wcin, temp);
        }
        //If the answer wasn't correct, ask the user if he thinks it's close enough to get a point.
        else
        {
            std::wstring a = L"";
            bool incorrectInput = false;

            //Ask for input until we get a usable answer
            do
            {
                //Empty windows console
                system("cls");
                std::wcout << answer << "  Oli väärä vastaus! Oikea vastaus: " << answers[i] << std::endl;
                std::wcout << "Jos vastauksesi oli mielestäsi riittävän lähellä oikeaa vastausta, voit antaa itsellesi pisteen vaihtoehdolla (1)." << std::endl;
                std::wcout << "Vaihtoehtoisesti kirjoita (2) jatkaaksesi sanojen kääntämistä." << std::endl << std::endl;
                std::wcout << "(1): Kyllä" << std::endl;
                std::wcout << "(2): Ei" << std::endl;

                if (incorrectInput)
                {
                    std::wcout << "Anna syötteeksi numero (1) tai (2) !" << std::endl;
                }
                std::wcout << std::endl;

                a = L"";
                incorrectInput = false;
                std::getline(std::wcin, a);

                if (a == L"1") {
                    correctAnswers++;
                    incorrectInput = false;
                }
                else if (a == L"2")
                {
                    incorrectInput = false;
                }
                else
                {
                    incorrectInput = true;
                }
            } while (incorrectInput);
        }
    }
}

