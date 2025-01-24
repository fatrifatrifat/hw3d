#pragma once
#include "Graphics.h"

class System
{
public:
	System(int width, int height, LPCWSTR caption);
	System(const System& rhs) = delete;
	System& operator=(const System& rhs) = delete;
	~System();
	

	bool Initialize(HINSTANCE hInst);
	void Go();
	void Shutdown();

	LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
	bool DoFrame();
	bool InitWin(HINSTANCE hInst);

private:
	HINSTANCE hInstance;
	HWND hMainWnd;

	LPCWSTR winCaption;
	int mWidth;
	int mHeight;

	std::unique_ptr<Graphics> gfx;
};

static LRESULT CALLBACK MainWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);