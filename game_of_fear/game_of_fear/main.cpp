#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "fadma.h"


HWND g_HWND = NULL;
std::vector<unsigned> allKeys;

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
	FOR_ (i, 'A', 'Z'+1) {
		if (butdwn (i)) {
			cout << char (i) << endl;
			return FALSE;
		}
	}
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
	bool shift = (dialog[line][pos] == '?');
	INPUT input;

	input.type = INPUT_KEYBOARD;
	input.ki.wScan = input.ki.time = input.ki.dwExtraInfo = 0;

	char c;
	input.ki.wVk = LOBYTE(VkKeyScan(c = dialog[line][pos++]));
	input.ki.dwFlags = 0;

	if (shift) {
		setpressbut_plus (VK_SHIFT, SendInput(1, &input, sizeof(input)));
	} else {
		if (isalpha (c)) {
			setpressbut (toupper (c));
		} else {
			SendInput(1, &input, sizeof(input));
		}
	}
	return TRUE;
}

// èíèöèàëèçèðóåì ñòðîêè äèàëîãà
void __stdcall InitDialog(std::vector<std::wstring> &dialog) {
	dialog.resize(3);
	dialog[0] = L"Hello, man. What is your name?\n";
	dialog[1] = L"\nDo you like cats?\n";
	dialog[2] = L"\nDo you like darkness?\n";
}

void main()
{
	for (unsigned i = 0x41; i <= 0x5A; ++i) {
		allKeys.push_back (i);
	}

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
				int ms_to_sleep;
				if (rand () % 8 == 0) {
					ms_to_sleep = 500;
				} else {
					ms_to_sleep = 20 + rand() % 200; // íåáîëüøàÿ çàäåðæêà äëÿ ïðèäàíèÿ ðåàëèñòè÷íîñòè
				}
				ms_to_sleep /= 1.6;
				while (ms_to_sleep > 0) {
					ms_to_sleep -= 10;
					Sleep (10);
					if (!IsNothingPressed ()) {
						MessageBox(NULL, L"Dont do it!", L"My caption.", MB_ICONSTOP);
						SetForegroundWindow(g_HWND);
					}
				}
			}
			else {
				MessageBox(NULL, L"Dont do it!", L"My caption.", MB_ICONSTOP);
				SetForegroundWindow(g_HWND);
			}
		}
		Sleep(3000);				// ïîäîæäàòü
		TerminateProcess(pi.hProcess, NO_ERROR);	// óáðàòü ïðîöåññ
	}
}