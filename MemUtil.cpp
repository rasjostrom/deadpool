#include "stdafx.h"
#include "MemUtil.h"
using namespace std;

#pragma region Constructors
MemUtil::MemUtil()
{
}


MemUtil::~MemUtil()
{
	CloseHandle(hProcess);
}

#pragma endregion

#pragma region Get Process & ClientModule

bool MemUtil::FindProcess() {

	/* Create a snapshot of all processes & declare process entry*/
	HANDLE hProcSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 pEntry32;
	pEntry32.dwSize = sizeof(PROCESSENTRY32);
	char * cTargetProc = "csgo.exe";

	/* Check if snapshot is valid */
	if (hProcSnap == INVALID_HANDLE_VALUE) {
		cout << "ERR: MemUtils::FindProcess() => INVALID_HANDLE_VALUE" << endl;
		return false;
	}

	/* Iterate through all processes*/
	cout << "SEARCHING FOR " << cTargetProc << "..." << endl;
	do
		/* If the current process name matches target process name */
		if (!strcmp(pEntry32.szExeFile, cTargetProc)) {
			cout << "\n* PROCESS '" << cTargetProc << "' FOUND! \n" << endl;

			dwPID = pEntry32.th32ProcessID; // Get the PID
			CloseHandle(hProcSnap); // Close the handle
			hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID); // Allow read access

			cout << "PROCESS READ ACCESS SUCCESSFUL..." << endl;
			return true;
		}
		while (Process32Next(hProcSnap, &pEntry32));

		cout << "ERR: MemUtils::FindProcess() => PROCESS_NOT_FOUND" << endl;
		return false;
}

bool MemUtil::ClientModuleBaseAddress() {
	/* See comments for FindProcess(), they're basically the same*/
	HANDLE hModSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPID);
	MODULEENTRY32 modEntry32;
	modEntry32.dwSize = sizeof(MODULEENTRY32);
	char * cTargetModule = "client.dll";

	if (hModSnapshot == INVALID_HANDLE_VALUE) {
		cout << "ERR: MemUtils::ClientModuleBaseAddress() => INVALID_HANDLE_VALUE" << endl;
		return 0;
	}

	cout << "INFO: SEARCHING FOR " << cTargetModule << "..." << endl;
	do
		if (!strcmp(modEntry32.szModule, cTargetModule)) {
			CloseHandle(hModSnapshot);
			dwClientBase = (DWORD)modEntry32.modBaseAddr; // Save the address in dwClientBase
			cout << "\n* MODULE '" << cTargetModule << "' FOUND!\n" << endl;
			cout << "ClientBase: " << dwClientBase << endl;
			return true;
		}
		while (Module32Next(hModSnapshot, &modEntry32));

		cout << "ERR: MemUtils::ClientModuleBaseAddress() => MODULE_NOT_FOUND" << endl;
		return false;
}

#pragma endregion

#pragma region Read Memory
/* Get Local Information */
DWORD MemUtil::GetLocalPlayer() { return MemRead<DWORD>(dwClientBase + dwLocalPlayerOffset); }

int MemUtil::GetTeam(DWORD player) { return MemRead<int>(player + dwTeamOffset); }

int MemUtil::GetCrossHair(DWORD player) { return MemRead<int>(player + dwCrossHairOffset); }

/* Get Target Information */
DWORD MemUtil::GetTarget(int xhair) {
	return MemRead<DWORD>(dwClientBase +
		dwEntityListOffset +
		((xhair - 1) *
			dwEntLoopDistanceOffset));
}
int MemUtil::GetTargetHealth(DWORD target) { return MemRead<int>(target + dwHealthOffset); }

float* MemUtil::GetPosition(DWORD player)
{
	static float Position[3];
	ReadProcessMemory(hProcess, (PBYTE*)(player + dwVecOrigin), &Position, sizeof(float[3]), 0);
	printf("GET_POSITION: %4.2f, %4.2f, %4.2f \n\n", Position[0], Position[1], Position[2]);
	return Position;
}
#pragma endregion