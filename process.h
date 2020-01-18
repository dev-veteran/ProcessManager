#include <iostream>
#include <Windows.h>

class process
{
public:
	DWORD FindProcessId(const std::wstring& processName);

	void TerminateById(DWORD processId);
	void SuspendById(DWORD processId);
	void ResumeById(DWORD processId);
};