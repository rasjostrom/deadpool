// DeadPool.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Trig.h"
#include "Aim.h"
using namespace std;

Trig tBoy;
Aim aBoy;

bool isTrig = false;
bool isAim = false;



int main()
{

	
	//aBoy.initialize();
	
	while (true) {

		// NumLock toggled?
		while ((GetKeyState(VK_NUMLOCK) & 0x8001) != 0) {
		
			if (!isTrig) { isTrig = tBoy.initialize(); }
			if (!isAim) { isAim = aBoy.initialize(); }
			// Mouse Right pressed?
			if ((GetKeyState(VK_RBUTTON) & 0x100) != 0)
				tBoy.activate();
			
			//tBoy.printPos();
			Sleep(50);
		}

		if (isTrig) {
			cout << "INFO: Deactivated Trigger" << endl;
			isTrig = false; 
		}
	}
	
	cin.get();
    return 0;
}

