#include "process.h"

#include <TlHelp32.h>

DWORD process::FindProcessId(const std::wstring& processName)
{
    PROCESSENTRY32 processInfo;
    processInfo.dwSize = sizeof(processInfo);

    HANDLE processSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
    if (processSnapshot == INVALID_HANDLE_VALUE) {
        return 0;
    }

    Process32First(processSnapshot, &processInfo);
    if (!processName.compare(processInfo.szExeFile))
    {
        CloseHandle(processSnapshot);
        return processInfo.th32ProcessID;
    }

    while (Process32Next(processSnapshot, &processInfo))
    {
        if (!processName.compare(processInfo.szExeFile))
        {
            CloseHandle(processSnapshot);
            return processInfo.th32ProcessID;
        }
    }
    CloseHandle(processSnapshot);
    return 0;
}

void process::TerminateById(DWORD processId)
{
    HANDLE Handle = OpenProcess(SYNCHRONIZE | PROCESS_TERMINATE, TRUE, processId);
    TerminateProcess(Handle, 0);
}

void process::SuspendById(DWORD processId)
{
    HANDLE hThreadSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);

    THREADENTRY32 threadEntry;
    threadEntry.dwSize = sizeof(THREADENTRY32);

    Thread32First(hThreadSnapshot, &threadEntry);

    do
    {
        if (threadEntry.th32OwnerProcessID == processId)
        {
            HANDLE Handle = OpenThread(THREAD_ALL_ACCESS, FALSE,
                threadEntry.th32ThreadID);

            SuspendThread(Handle);
            CloseHandle(Handle);
        }
    } while (Thread32Next(hThreadSnapshot, &threadEntry));
    CloseHandle(hThreadSnapshot);
}

void process::ResumeById(DWORD processId)
{
    HANDLE processSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);

    THREADENTRY32 threadEntry;
    threadEntry.dwSize = sizeof(THREADENTRY32);

    Thread32First(processSnapshot, &threadEntry);

    do
    {
        if (threadEntry.th32OwnerProcessID == processId)
        {
            HANDLE Handle = OpenThread(THREAD_ALL_ACCESS, FALSE,
                threadEntry.th32ThreadID);

            ResumeThread(Handle);
            CloseHandle(Handle);
        }
    } while (Thread32Next(processSnapshot, &threadEntry));
    CloseHandle(processSnapshot);
}