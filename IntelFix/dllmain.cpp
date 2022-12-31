// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#define EXTERN_DLL_EXPORT extern "C" __declspec(dllexport)

PDWORD oldAffinity;
PDWORD oldSystemAffinity;
HANDLE curProcess;

DWORD WINAPI RestoreAffinityThread(LPVOID param)
{
    Sleep(5000);
    SetProcessAffinityMask(curProcess, (DWORD_PTR)&oldAffinity);
    FreeLibraryAndExitThread((HMODULE)param, 0);
    return 0;
}

EXTERN_DLL_EXPORT BOOL APIENTRY DllMain(HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        curProcess = GetCurrentProcess();
        GetProcessAffinityMask(curProcess, (PDWORD_PTR)&oldAffinity, (PDWORD_PTR)&oldSystemAffinity);
        SetProcessAffinityMask(curProcess, 1);
        CreateThread(0, 0, RestoreAffinityThread, hModule, 0, 0);
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

