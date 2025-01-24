#include "App.h"
#include "System.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
	PSTR cmdLine, int showCmd)
{
	/*App app(hInstance);
	app.InitApp();
	app.Go();
	app.Shutdown();*/

	System sys(800, 600, L"Rifat");
	sys.Initialize(hInstance);
	sys.Go();
}