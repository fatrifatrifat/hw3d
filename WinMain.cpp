#include "App.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
	PSTR cmdLine, int showCmd)
{
	App app(hInstance);
	app.InitApp();
	app.Go();
	app.Shutdown();
}