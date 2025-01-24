#include "Graphics.h"

Graphics::Graphics()
{
}

Graphics::~Graphics()
{
}

bool Graphics::Initialize(int width, int height, HWND hWnd)
{
	d3dApp = std::make_unique<D3DApp>();

	if (!d3dApp->InitD3D(hWnd))
	{
		MessageBox(0, L"D3D Initialization Failed", 0, 0);
		return false;
	}

	return true;
}

void Graphics::Shutdown()
{
	d3dApp->Shutdown();
}

bool Graphics::DoFrame()
{
	bool result;

	result = Render();
	if (!result)
	{
		return false;
	}

	return true;
}

bool Graphics::Render()
{
	d3dApp->BeginScene();



	d3dApp->EndScene();
	return true;
}
