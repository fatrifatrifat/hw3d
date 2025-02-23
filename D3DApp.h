#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <wrl\client.h>
#include <dxgi.h>
#include <memory>
#include <vector>
#include <string>
#include <random>
#include "GameTimer.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

using namespace Microsoft::WRL;

class D3DApp
{
	friend class Bindable;

public:
	D3DApp(int width, int height);
	D3DApp(const D3DApp& rhs) = delete;
	D3DApp& operator=(const D3DApp& rhs) = delete;
	~D3DApp();

	bool InitD3D(HWND hWnd);

	void BeginScene();
	void DrawIndexed(UINT count);
	void EndScene();

	ID3D11Device* GetDevice() const;
	ID3D11DeviceContext* GetDeviceContext() const;

	DirectX::XMMATRIX GetCamera() const;
	DirectX::XMMATRIX GetWorldMatrix() const;
	DirectX::XMMATRIX GetProjMatrix() const;
	DirectX::XMMATRIX GetViewMatrix() const;

	void SetCamera(DirectX::FXMMATRIX camera) noexcept;
	void SetProjection(DirectX::FXMMATRIX proj) noexcept;

	void EnableImgui() noexcept;
	void DisableImgui() noexcept;
	bool IsImguiEnabled() const noexcept;

private:
	int width;
	int height;

	bool imguiEnabled = true;

	ComPtr<ID3D11Device> pDevice;
	ComPtr<ID3D11DeviceContext> pImmediateContext;
	ComPtr<IDXGISwapChain> pSwapChain;
	ComPtr<ID3D11RenderTargetView> pRVT;
	ComPtr<ID3D11DepthStencilView> pDSV;

	ComPtr<ID3D11RasterizerState> pRasterWireFrame;
	ComPtr<ID3D11RasterizerState> pRasterSolidState;

	DirectX::XMFLOAT4X4 mCamera;
	DirectX::XMFLOAT4X4 mWorldMatrix;
	DirectX::XMFLOAT4X4 mProjectionMatrix;
	DirectX::XMFLOAT4X4 mViewMatrix;
};

