#include "App.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
	PSTR cmdLine, int showCmd)
{
	App app = App(hInstance);
	app.InitApp();
	app.Go();
	/*System sys(800, 600, L"Rifat");
	sys.Initialize(hInstance);
	sys.Go();*/
}