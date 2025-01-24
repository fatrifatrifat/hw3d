#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <dxgi.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

class D3DApp
{
public:
	D3DApp();
	D3DApp(const D3DApp& rhs) = delete;
	D3DApp& operator=(const D3DApp& rhs) = delete;
	~D3DApp();

	bool InitD3D(HWND hWnd);
	void Shutdown();

	void BeginScene();
	void EndScene();

	ID3D11Device* GetDevice() const;
	ID3D11DeviceContext* GetDeviceContext() const;

	void GetProjectionMatrix(DirectX::XMFLOAT4X4&);
	void GetWorldMatrix(DirectX::XMFLOAT4X4&);
	void GetOrthoMatrix(DirectX::XMFLOAT4X4&);
	
private:
	ID3D11Device* pDevice;
	ID3D11DeviceContext* pImmediateContext;
	IDXGISwapChain* pSwapChain;
	ID3D11RenderTargetView* pRVT;
	D3D11_VIEWPORT vp;

	DirectX::XMFLOAT4X4 mProjectionMatrix;
	DirectX::XMFLOAT4X4 mWorldMatrix;
	DirectX::XMFLOAT4X4 mOrthoMatrix;
};

