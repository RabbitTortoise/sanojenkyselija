#include <iostream>
#include <string>
#include <locale>
#include <Windows.h>
#include <fcntl.h>

#include "WordAsker.h"
#include "ErrorLogger.h"


/// <summary>
/// The main loop of the program
/// </summary>
/// <returns></returns>
int main()
{

    //Enable Finnish letters in the windows console.
    SetConsoleCP(1252);
    SetConsoleOutputCP(1252);
    //Sets the used locale settings to system default.
    setlocale(LC_ALL, "");

    //These two calls force wcout and wcin to work with unicode letters.
    _setmode(_fileno(stdout), _O_U16TEXT);
    _setmode(_fileno(stdin), _O_U16TEXT);
    
    //This is in case the operating system is in English. We have to use utf8 locale to get scandinavian letters to work.
    try
    {
        //Set writing and reading locale to "en_US.utf8"
        std::locale::global(std::locale("en_US.utf8"));
    }     
    catch(...)
    {
        std::cout << "Tietokoneelta ei l�ydy en_US.utf8 -kielipakettia! Asetetaan kielipaketiksi oletusasetus. ��kk�set eiv�t v�ltt�m�tt� toimi oikein!" << std::endl;
        std::cout << "HUOM! T�m� viesti siksi koska koodari k�ytt�� englanninkielist� windowsia. Jos sinulla on suomenkielinen k�ytt�j�rjestelm�, oletusasetuksen pit�isi olla oikein." << std::endl;
        std::locale::global(std::locale());
        std::locale the_global_locale;
        std::cout << "Asetettu kielipaketti: " << the_global_locale.name() << std::endl;
        std::cout << "Jos t�m� kielipaketti on fi_utf8 ohjelman pit�isi toimia oikein. (T�t� ei ole testattu)" << std::endl;
        std::cout << "Jos t�m� kielipaketti ei tue ��kk�si�, ohjelman toimimiseksi ��kk�sten kanssa sinun t�ytyy joko asentaa en_US.utf8 -kielipaketti tai saada jotenkin oletuskielipaketti (locale) sellaiseksi, ett� se tukee ��kk�si�." << std::endl;
        system("pause");
    }

    //Make a time entry to the log
    ErrorLogger::SaveErrorMessageToFile(CheckMethods::CurrentDateTime());

    //Creating the WordAsker object will automatically check for good files
    WordAsker wordAsker;

    //Loop the Menu until we get returned false.
    bool running = true;
    while (running)
    {
        running = wordAsker.StartMenu();
    }
    

    return 0;
}

