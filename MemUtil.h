#pragma once
#include <Windows.h>
#include <iostream>
#include <string>
#include <TlHelp32.h>

class MemUtil
{

public:

#pragma region Offsets
	/* Offsets */
	// nice ones
	const DWORD dwEntLoopDistanceOffset = 0x10;
	const DWORD dwTeamOffset = 0xF0;
	const DWORD dwHealthOffset = 0xFC;

	// fucky ones...
	const DWORD dwCrossHairOffset = 0xA950;
	const DWORD dwEntityListOffset = 0x4A98174;
	const DWORD dwLocalPlayerOffset = 0xA7D4CC;
	const DWORD dwVecOrigin = 0x134;
#pragma endregion

#pragma region Base Variables
	/* Process Iteration Vars*/
	HANDLE hProcess; // csgo process handle
	DWORD dwPID;	// csgo process ID
	DWORD dwClientBase; // client.dll base addr

#pragma endregion

#pragma region Player Values
	INPUT input;

	DWORD LOCAL_PLAYER;
	int LOCAL_TEAM;
	int LOCAL_XHAIR;
	float* LOCAL_POSITION;

	DWORD TARGET_PLAYER;
	int TARGET_TEAM;
	int TARGET_HEALTH;
#pragma endregion

	MemUtil();
	~MemUtil();

	template <class cData>
	cData MemRead(DWORD dwAddress) {
		cData cValue;
		ReadProcessMemory(hProcess, (LPVOID)dwAddress, &cValue, sizeof(cData), NULL);
		return cValue;
	}

	virtual bool FindProcess();
	virtual bool ClientModuleBaseAddress();

	virtual DWORD GetLocalPlayer();
	virtual int GetTeam(DWORD player);
	virtual int GetCrossHair(DWORD player);

	virtual DWORD GetTarget(int xhair);
	virtual int GetTargetHealth(DWORD target);

	virtual float* GetPosition(DWORD player);
};