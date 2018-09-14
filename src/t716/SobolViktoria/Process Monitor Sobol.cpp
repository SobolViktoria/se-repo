#include <windows.h>
#include <tlhelp32.h>
#include <tchar.h>
#include <stdio.h>
#include <iostream>
#include <iomanip> 
#include <psapi.h>
using namespace std;
#define MAX_NAME 256
#define MB 1048576
#define UNLEN 256
static float CalculateCPULoad();
static unsigned long long FileTimeToInt64();

float GetCPULoad();

void gotoxy(int x, int y)
{
	COORD cd;
	cd.X = x;
	cd.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cd);
}
//функция вывода процессов в консоль
char* getProcessUsername(HANDLE hSnapshot, PROCESSENTRY32 processList)
{
	DWORD dwSize = 256;
	HANDLE hProcess;
	TOKEN_USER *pUserInfo;
	static char staticName[MAX_NAME];
	char name[MAX_NAME];
	char domain[MAX_NAME];
	char *result;
	int iUse;

	hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processList.th32ProcessID);
	OpenProcessToken(hProcess, TOKEN_QUERY, &hSnapshot);
	pUserInfo = (PTOKEN_USER)GlobalAlloc(GPTR, dwSize);
	GetTokenInformation(hSnapshot, TokenUser, pUserInfo, dwSize, &dwSize);
	LookupAccountSidA(0, pUserInfo->User.Sid, name, &dwSize, domain, &dwSize, (PSID_NAME_USE)&iUse);
	strncpy_s(staticName, name, _TRUNCATE);
	result = staticName;
	return result;
}


void Out_Diagram_CPU()
{
	int get = GetCPULoad() * 100;
	cout << "CPU    " << get << "%[";
	for (int i = 0; i < get / 4; i++) {
		cout << "|";
	}
	for (int i = 0; i < 25 - get / 4; i++) {
		cout << ".";
	}
	gotoxy(36, 0);
	cout << "]";
}

void Out_Diagram_Mem()
{
	MEMORYSTATUSEX memStat;
	memStat.dwLength = sizeof(memStat);
	GlobalMemoryStatusEx(&memStat);

	cout << "\nMemory " << memStat.dwMemoryLoad << "%[";
	for (int i = 0; i<memStat.dwMemoryLoad / 4; i++)
		cout << "|";
	for (int i = 0; i<25 - memStat.dwMemoryLoad / 4; i++)
		cout << ".";
	gotoxy(36, 1);
	cout << "] " << (memStat.ullTotalPhys - memStat.ullAvailPhys) / MB << "MB/" << memStat.ullTotalPhys / MB << "MB";
}
void Out_Graphic_CPU()
{
	int get = GetCPULoad() * 100;
	for (int i = 0; i < get / 4; i++) {
		cout << "|";
	}
	for (int i = 0; i < 25 - get / 4; i++) {
		cout << ".";
	}
	cout <<get<< "%\n";
	Sleep(500);
}
void out_process(void) {
	HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (snap == INVALID_HANDLE_VALUE)
		return;
	PROCESSENTRY32 processList;
	HANDLE CONST hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	PROCESSENTRY32 pe;
	pe.dwSize = sizeof(pe);
	if (!Process32First(snap, &pe)) {
		CloseHandle(snap);
		return;
	}

	int count = 0;
	int cor = 3;
	gotoxy(0, 2);
	cout << "User\t\tpID\t\tProcess name\n\n";
	do {
		if (pe.th32ProcessID != 0 )
		{
			//printProcessList(hStdOut);
			//if (getProcessUsername(snap, pe)[1] != 'М'&&getProcessUsername(snap, pe)[0] != 'М')
			//	cout<<getProcessUsername(snap, pe);
			if (count < 50)
			{
				if (getProcessUsername(snap, pe)[1] != 'М'&&getProcessUsername(snap, pe)[0] != 'М')
				{
					gotoxy(0, cor);
					cout << getProcessUsername(snap, pe);
				}
				gotoxy(16, cor);
				cout << pe.th32ProcessID;
				gotoxy(32, cor);
				cout<< pe.szExeFile << "\n";
				cor++;
				count++;
			}

			if (count == 50)
			{
				cor = 2;
				gotoxy(70, cor);
				cout << "User\t\tpID\t\tProcess name\n";
				count++;
				cor++;
			}
			if (count > 50)
			{
				gotoxy(70, cor);
				if (getProcessUsername(snap, pe)[1] != 'М'&&getProcessUsername(snap, pe)[0] != 'М')
					cout << getProcessUsername(snap, pe);
				gotoxy(86, cor);
				cout << pe.th32ProcessID;
				gotoxy(102, cor);
				cout << pe.szExeFile << "\n";
				cor++;
			}

		}
	} while (Process32Next(snap, &pe));

	CloseHandle(snap);
}


int main(int argc, char* argv[])
{
	setlocale(0, "");
	int counter = 0;

	string as = argv[1];
	system("mode con cols=140 lines=200");
	
	if (as == "1")
	{
		while (counter < 4)
		{
			Out_Graphic_CPU();
			Sleep(500);
			system("cls");
		}
	}
	if (as == "2")
	{
		while (counter < 4)
		{
			Out_Diagram_CPU();
			Out_Diagram_Mem();
			Sleep(500);
			system("cls");
		}
	}
	if (as == "3")
	{
		while (counter < 4)
		{
			out_process();
			Sleep(500);
			system("cls");
		}
	}
	if (as == "4")
	{
		while (counter < 4)
		{
			Out_Diagram_CPU();
			Out_Diagram_Mem();
			out_process();
			Sleep(500);
			system("cls");
		}
	}
	system("pause");
	return 0;
}

static float CalculateCPULoad(unsigned long long idleTicks, unsigned long long totalTicks)
{
	static unsigned long long _previousTotalTicks = 0;
	static unsigned long long _previousIdleTicks = 0;

	unsigned long long totalTicksSinceLastTime = totalTicks - _previousTotalTicks;
	unsigned long long idleTicksSinceLastTime = idleTicks - _previousIdleTicks;


	float ret = 1.0f - ((totalTicksSinceLastTime > 0) ? ((float)idleTicksSinceLastTime) / totalTicksSinceLastTime : 0);

	_previousTotalTicks = totalTicks;
	_previousIdleTicks = idleTicks;
	return ret;
}

static unsigned long long FileTimeToInt64(const FILETIME & ft)
{
	return (((unsigned long long)(ft.dwHighDateTime)) << 32) | ((unsigned long long)ft.dwLowDateTime);
}

// Returns 1.0f for "CPU fully pinned", 0.0f for "CPU idle", or somewhere in between
// You'll need to call this at regular intervals, since it measures the load between
// the previous call and the current one.  Returns -1.0 on error.
float GetCPULoad()
{
	FILETIME idleTime, kernelTime, userTime;
	return GetSystemTimes(&idleTime, &kernelTime, &userTime) ? CalculateCPULoad(FileTimeToInt64(idleTime), FileTimeToInt64(kernelTime) + FileTimeToInt64(userTime)) : -1.0f;
}