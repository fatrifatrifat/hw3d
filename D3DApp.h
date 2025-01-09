#pragma once

#include <Windows.h>
#include <d3d11.h>
#include 

class D3DApp
{
public:
	D3DApp(HINSTANCE hInstance);
	virtual ~D3DApp();

	HINSTANCE AppInst() const;
	HWND MainWnd() const;
	float AspectRation() const;

	int Run();

	virtual bool Init();
	virtual void OnResize();
	virtual void UpdateScene(float dt) = 0;
	virtual void DrawScene() = 0;
	virtual LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	virtual void OnMouseDown(WPARAM wParam btnState, int x, int y){}
	virtual void OnMouseUp(WPARAM wParam btnState, int x, int y){}
	virtual void OnMouseMove(WPARAM wParam btnState, int x, int y){}

protected:
	bool InitMainWindow();
	bool InitDirect3D();

	void CalculateFrameStats();

protected:
	HINSTANCE mhAppInst;
	HWND mhMainWnd;
	bool mAppPaused;
	bool mMinimized;
	bool mMaximized;
	bool mResizing;
	UINT m4xMsaaQuality;

	
};