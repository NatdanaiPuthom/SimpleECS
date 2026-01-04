#include "Console.hpp"
#include <iostream>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace Simple
{
	Console::Console()
	{
	}

	Console::~Console()
	{
#pragma warning(push)
#pragma warning(disable : 4996)
		fclose(stdin);
		fclose(stdout);
		fclose(stderr);
#pragma warning(pop)
		FreeConsole();
	}

	void Console::Init()
	{
#pragma warning(push)
#pragma warning( disable : 4996 )
		AllocConsole();
		freopen_s((FILE**)stdin, "CONIN$", "r", stdin);
		freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
		freopen_s((FILE**)stdout, "CONOUT$", "w", stderr);

		setbuf(stdin, NULL);
		setbuf(stdout, NULL);
		setbuf(stderr, NULL);

		SetConsoleTitle(L"Simple, it's just that easy");
#pragma warning( pop )
	}

	void Console::Print(const char* aText, const bool aShouldEndLine)
	{
		std::cout << aText;

		if (aShouldEndLine)
		{
			std::cout << std::endl;
		}
	}

	void Console::SetTextColor(const ConsoleTextColor aColor)
	{
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

		switch (aColor)
		{
		case ConsoleTextColor::Red:
			SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
			break;
		case ConsoleTextColor::Green:
			SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
			break;
		case ConsoleTextColor::Blue:
			SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
			break;
		case ConsoleTextColor::Yellow:
			SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
			break;
		case ConsoleTextColor::White:
			SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
			break;
		case ConsoleTextColor::Default:
			SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
			break;
		default:
			SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
			break;
		}
	}
}
