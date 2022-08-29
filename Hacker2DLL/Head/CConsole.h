#pragma once

#define WIN32_LEAN_AND_MEAN

#include <iostream>
#include <string>

#include <windows.h>

class CConsole
{
private:
	static FILE *inStream;
	static FILE *outStream;
	static FILE *errStream;
public:
    static bool Init();
    static void UnInit();
    static void WriteLine(const std::string &context);
    static void WriteLine(const char *context);
    static void WriteLine(const int context);
	static void WriteLine(const void *context);
};