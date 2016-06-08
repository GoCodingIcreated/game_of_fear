#include <windows.h>
#include <iostream>
#include <string>
#include <vector>


HWND g_HWND = NULL;

BOOL CALLBACK EnumWindowsProcMy(HWND hwnd, LPARAM lParam)
{
	DWORD lpdwProcessId;
	GetWindowThreadProcessId(hwnd, &lpdwProcessId);
	if (lpdwProcessId == lParam)
	{
		g_HWND = hwnd;
		return FALSE;
	}
	return TRUE;
}

void __stdcall WriteTextInNotepad(const std::string &str)
{
	SetForegroundWindow(g_HWND);
	Sleep(1000);
	for (size_t i = 0; i < str.size(); ++i) {
		INPUT input;

		input.type = INPUT_KEYBOARD;
		input.ki.wScan = input.ki.time = input.ki.dwExtraInfo = 0;

		input.ki.wVk = LOBYTE(VkKeyScan(str[i]));
		input.ki.dwFlags = 0;
		SendInput(1, &input, sizeof(input));
		Sleep(50 + rand() % 300); // небольшая задержка для придания реалистичности
	}
}

// вывод диалога в блокнот с интервалом
void __stdcall PrintDialog(std::vector<std::string> &dialog) {
	for (auto sentence : dialog) {
		WriteTextInNotepad(sentence);
		Sleep(5000);
	}
}

// инициализируем строки диалога
void __stdcall InitDialog(std::vector<std::string> &dialog) {
	dialog.resize(3);
	dialog[0] = "Hello, man. What is your name?\n";
	dialog[1] = "\nDo you like cats?\n";
	dialog[2] = "\nDo you like dark?\n";
}

void main()
{
	STARTUPINFO cif;
	ZeroMemory(&cif, sizeof(STARTUPINFO));
	PROCESS_INFORMATION pi;


	if (CreateProcess(L"c:\\windows\\notepad.exe", NULL,
		NULL, NULL, FALSE, NULL, NULL, NULL, &cif, &pi) == TRUE)
	{

		EnumWindows(EnumWindowsProcMy, pi.dwProcessId);
		RECT rect;
		GetWindowRect(g_HWND, &rect);

		std::vector<std::string> my_dialog;
		InitDialog(my_dialog);
		PrintDialog(my_dialog);

		Sleep(3000);				// подождать
		TerminateProcess(pi.hProcess, NO_ERROR);	// убрать процесс
	}
}
