#pragma once
#include <iostream>
#include <codecvt>
#include <io.h>
#include <fcntl.h>
#include "Parser.h"

int main()
{
    //set utf-8 for the whole program
    //use only wide input and output
    std::locale::global(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));
    auto mode = _setmode(_fileno(stdout), _O_U8TEXT);

    try
    {
        JSONParser parser;

        wstring command;

        while (true)
        {
            wcout << ">";
            getline(wcin, command, L'\n');

            //quits program if q or Q is passed to the command
            if (command == L"q")
                break;
            if (command == L"cls")
            {
                system("cls");
                continue;
            }

            parser.parseCommand(command);
        }
    }

    catch (const exception& e)
    {
        wcerr << e.what();
    }
}
