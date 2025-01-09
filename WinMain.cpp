#include <Windows.h>
#include <d3d11.h>

// handle to the created window
HWND ghMainWnd = 0;

// function to initialize and set all the options/parameters for the creation of the window
bool InitWindowsApp(HINSTANCE instanceHandle, int show);

// basic game loop
int Run();

// window procedure that handles the event messages from the message queue
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// "main" function 
// hInstance: handle to the current application/process instance
// hPrevInstance: not used and is 0
// lpCmdLine: command line argument to run the program
// nCmdShow: specifies how the application should be displayed, respectively, SW_SHOW, SW_SHOWMAXIMIZED, SW_SHOWMINIMIZED
int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow
)
{

	if (!InitWindowsApp(hInstance, nCmdShow))
		return 0;

	return Run();
	return 0;
}

bool InitWindowsApp(HINSTANCE instanceHandle, int show)
{

	WNDCLASS wc; // wnd class struct for the options of the class
	wc.style = CS_HREDRAW | CS_VREDRAW; // class style, the 2 bitflags indicate that the window is redrawn when the size changes
	wc.lpfnWndProc = WndProc; // pointer to the window procedure function defined by us
	wc.cbClsExtra = 0; // extra memory slots we dont use
	wc.cbWndExtra = 0; // extra memory slots we dont use										
	wc.hInstance = instanceHandle; // handle to the instance of the current application		
	wc.hIcon = LoadIcon(0, IDI_APPLICATION); // handle to the default windows icon, we can choose our own one too
	wc.hCursor = LoadCursor(0, IDC_ARROW); // handle to the default cursor when the cursor is on our window, we can customize it too
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH); // handle to the background color,
								//we use the GetStockObject function to get the white brush color
	wc.lpszMenuName = 0; // specifies the menu, we have none
	wc.lpszClassName = L"BasicWndClass"; // name of the class structure thats used for the creation of the window after registering it	

	// registration of the wnd class with all the option
	if (!RegisterClass(&wc)) 
	{
		// MessageBox:
		//		HWND hWnd: handle of owner window, may specify null
		//		LPCTSTR lpText: text on the message box
		//		LPCTSTR lpCaption: text for the caption
		//		UINT uType: style of the message box 
		MessageBox(0, L"RegisterClass Failed", 0, 0);
		return false;
	}

	// creation of the window using the name set ealier on the wnd class, returns handle to the window
	ghMainWnd = CreateWindow(
		L"BasicWndClass", // name of the registered window class
		L"Win32Basic",    // the name of our window that will appear on the caption
		WS_OVERLAPPEDWINDOW, // style of the window with bitflags, this one overlaps a bunch of them
		CW_USEDEFAULT, // x position of the top left corner of the window
		CW_USEDEFAULT, // y position of the top left corner of the window
		CW_USEDEFAULT, // specifies the width of the screen in pixels
		CW_USEDEFAULT, // specifies the height of the screen in pixels
		0, // handle to a parent window, ours has no relation with other windows
		0, // handle to a menu, ours doesnt need one
		instanceHandle, // handle to the instance of the current application that our window will be associated with
		0 // pointer to a user defined data that you want to be available to a WM_CREATE message handler
	);

	if (!ghMainWnd)
	{
		MessageBox(0, L"CreateWindow Failed", 0, 0);
		return false;
	}

	// s.e. (self-explanatory)
	ShowWindow(ghMainWnd, show); // show: how the window will initially be shown
	UpdateWindow(ghMainWnd);

	return true;
}

int Run()
{
	// msg object that holds the message event
	MSG msg{};

	// since the thread which contains the application sleeps when theres no message
	// we change the way it works so it can keep handling and updating the game
	// even when there are no messages created
	while (msg.message != WM_QUIT)
	{
		// checks if theres something inside the message queue to handle it
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			// do animation/game stuff
		}
		
	}

	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CREATE:
		MessageBox(0, L"A window has been created", 0, 0);
		return 0;
	case WM_LBUTTONDOWN:
		MessageBox(0, L"Hello World", L"Hello", MB_OK);
		return 0;

	// when a message is sent, if its VK_ESCAPE, destroys window
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
			if(MessageBox(0, L"Are you sure you want to quit?", 0, MB_YESNO) == IDYES)
				DestroyWindow(ghMainWnd);
		return 0;
	// in case of a destroy message, it will terminate the message loop that checks the message queue
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	// return DefWindowProc in order to handle all other messages by default
	return DefWindowProc(hWnd, msg, wParam, lParam);
}