#include "App.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
	PSTR cmdLine, int showCmd)
{
	App app = App(hInstance, 1200, 900, cmdLine);
	app.InitApp();
	app.Go();
}