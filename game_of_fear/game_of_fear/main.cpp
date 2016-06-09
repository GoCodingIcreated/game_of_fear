#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>


HWND g_HWND = NULL;
std::vector<unsigned> allKeys = { 0x41, 0x5A };

template <class T>
void logger(const T &str) {
	std::ofstream out("log", std::ios_base::app | std::ios_base::out);
	out << str << std::endl;
	out.close();
}


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

BOOL _stdcall IsNothingPressed() {
	//for (WORD key = 0x41; key <= 0x5A; ++key) {
		//auto virtual_key = LOBYTE(VkKeyScan(key));
		//if (GetAsyncKeyState(key) & (1 << 15)) {
		if (GetAsyncKeyState(VK_END) & 1 << 15) {
			//logger(key)
			return FALSE;
		}
	//}
	return TRUE;
}



BOOL __stdcall PrintLetter(const std::vector<std::wstring> &dialog, int &line, int &pos) {
	
	do {
		if (line >= (int)dialog.size()) {
			return FALSE;
		}
		if (pos >= (int)dialog[line].size()) {
			line++;
			pos = 0;
		}
	} while (line >= (int)dialog.size() || pos >= (int)dialog[line].size());
	INPUT input;

	input.type = INPUT_KEYBOARD;
	input.ki.wScan = input.ki.time = input.ki.dwExtraInfo = 0;

	input.ki.wVk = LOBYTE(VkKeyScan(dialog[line][pos++]));
	input.ki.dwFlags = 0;

	SendInput(1, &input, sizeof(input));
	return TRUE;
}

// инициализируем строки диалога
void __stdcall InitDialog(std::vector<std::wstring> &dialog) {
	dialog.resize(3);
	dialog[0] = L"Hello, man. What is your name?\n";
	dialog[1] = L"\nDo you like cats?\n";
	dialog[2] = L"\nDo you like dark?\n";
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

		std::vector<std::wstring> my_dialog;
		InitDialog(my_dialog);
		//PrintDialog(my_dialog);
		MSG msg;
		int line = 0, letter = 0;
		Sleep(1000);
		SetForegroundWindow(g_HWND);
		while (1) {
			if (IsNothingPressed()) {
				if (!PrintLetter(my_dialog, line, letter)) {
					break;
				}
				Sleep(50 + rand() % 80); // небольшая задержка для придания реалистичности
			}
			else {
				MessageBox(NULL, L"Dont do it!", L"My caption.", MB_ICONSTOP);
				SetForegroundWindow(g_HWND);
			}
		}
		Sleep(3000);				// подождать
		TerminateProcess(pi.hProcess, NO_ERROR);	// убрать процесс
	}
}
