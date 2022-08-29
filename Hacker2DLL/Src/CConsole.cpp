#include "CConsole.h"

FILE *CConsole::inStream = nullptr;
FILE *CConsole::outStream = nullptr;
FILE *CConsole::errStream = nullptr;

bool CConsole::Init()
{
    ::AllocConsole();
    ::freopen_s(&inStream, "conin$", "r", stdin );
	::freopen_s(&outStream, "conout$", "w", stdout );
	::freopen_s(&errStream, "conout$", "w", stderr );
    std::ios::sync_with_stdio(false);

	//SetConsoleOutputCP(65001);

    return true;
}

void CConsole::UnInit()
{
    ::FreeConsole();
}

void CConsole::WriteLine(const std::string &context)
{
    std::cout << context << '\n';
}

void CConsole::WriteLine(const char *context)
{
    std::cout << context << '\n';
}

void CConsole::WriteLine(const int context)
{
    std::cout << context << '\n';
}

void CConsole::WriteLine(const void *context)
{
    std::cout << context << '\n';
}