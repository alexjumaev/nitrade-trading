// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"

#include <iostream>

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}



void ab()
{
	std::cout << "Hello World" << std::endl;
}

/*
class YenSquared
{
public:
	YenSquared() {}
	~YenSquared() {}

	void onInit()
	{

	}

	void onBar()
	{
		std::cout << "Yen Squared on Bar" << std::endl;
	}
};*/