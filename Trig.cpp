#include "stdafx.h"
#include "Trig.h"

using namespace std;

MemUtil mem;

Trig::Trig()
{
}


Trig::~Trig()
{
}

bool Trig::initialize() {
	
	if (mem.FindProcess()) {
		if (mem.ClientModuleBaseAddress()) {
			LOCAL_PLAYER = mem.GetLocalPlayer();
			LOCAL_TEAM = mem.GetTeam(LOCAL_PLAYER);
			LOCAL_POSITION = mem.GetPosition(LOCAL_PLAYER);

			cout << "INFO: Trig::Initialize success" << endl;
			cout << "INFO: LOCAL_TEAM = " << LOCAL_TEAM << endl;
			return true;
		}
	}
	
	cout << "ERR: Trig::initialize failed" << endl;
	return false;
}

void Trig::activate() {
	LOCAL_XHAIR = mem.GetCrossHair(LOCAL_PLAYER);
	LOCAL_POSITION = mem.GetPosition(LOCAL_PLAYER);

	TARGET_PLAYER = mem.GetTarget(LOCAL_XHAIR);
	TARGET_HEALTH = mem.GetTargetHealth(TARGET_PLAYER);
	TARGET_TEAM = mem.GetTeam(TARGET_PLAYER);

	if (LOCAL_TEAM != TARGET_TEAM && TARGET_HEALTH > 0) {
		input.type = INPUT_MOUSE;
		input.mi.mouseData = 0;
		input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
		SendInput(1, &input, sizeof(input));

		input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
		SendInput(1, &input, sizeof(input));
	}
}