#pragma once
#include <Windows.h>
#include <memory>
#include "D3DApp.h"

class Graphics
{
public:
	Graphics();
	Graphics(const Graphics& gfx) = delete;
	Graphics& operator=(const Graphics& gfx) = delete;
	~Graphics();

	bool Initialize(int width, int height, HWND hWnd);
	void Shutdown();
	bool DoFrame();

private:
	bool Render();

private:
	std::unique_ptr<D3DApp> d3dApp;
};

