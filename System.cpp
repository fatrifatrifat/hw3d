#include "System.h"

namespace
{
	System* sys = 0;
}

static LRESULT CALLBACK MainWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return sys->WndProc(hWnd, msg, wParam, lParam);
}

System::System(int width, int height, LPCWSTR caption)
	: winCaption(caption)
	, mWidth(width)
	, mHeight(height)
{
	sys = this;
	gfx = std::make_unique<Graphics>();
}

System::~System()
{
}

bool System::Initialize(HINSTANCE hInst)
{
	if (!InitWin(hInst))
	{
		MessageBox(0, L"Windows Init Failed", 0, 0);
		return false;
	}

	if (!gfx->Initialize(mWidth, mHeight, hMainWnd))
	{
		MessageBox(0, L"Graphics Init Failed", 0, 0);
		return false;
	}

	return true;
}

void System::Go()
{
	MSG msg;
	bool done;
	bool res;

	done = false;
	while (!done)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			res = DoFrame();
			if (!res)
				done = true;
		}
	}

	return;
}

void System::Shutdown()
{
}

LRESULT System::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
			DestroyWindow(hWnd);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_LBUTTONDOWN:
		MessageBox(0, L"Hi", 0, 0);
		return 0;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

bool System::DoFrame()
{
	return gfx->DoFrame();
}

bool System::InitWin(HINSTANCE hInst)
{
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = MainWndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInst;
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	wc.lpszMenuName = 0;
	wc.lpszClassName = L"D3DWndClassName";

	if (!RegisterClass(&wc))
	{
		MessageBox(0, L"RegisterClass Failed.", 0, 0);
		return false;
	}

	// Compute window rectangle dimensions based on requested client area dimensions.
	RECT R = { 0, 0, mWidth, mHeight };
	AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, false);
	int width = R.right - R.left;
	int height = R.bottom - R.top;

	hMainWnd = CreateWindow(L"D3DWndClassName", winCaption,
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height, 0, 0, hInstance, 0);
	if (!hMainWnd)
	{
		MessageBox(0, L"CreateWindow Failed.", 0, 0);
		return false;
	}

	ShowWindow(hMainWnd, SW_SHOW);
	UpdateWindow(hMainWnd);

	return true;
}