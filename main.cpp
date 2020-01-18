#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>

#include "process.h"

const std::wstring TargetProcess = L"notepad.exe";

int main()
{
    process Process;

	while (true)
	{
        	SetConsoleTitleA("Debug Console");
        	DWORD processId = Process.FindProcessId(TargetProcess);

        	if (processId)
        	{
            		std::cout << "Catched PID: " << processId << std::endl;
            		Process.TerminateById(processId);
        	}
		Sleep(50);
	}
}
