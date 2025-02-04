#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <dxgi.h>
#include <memory>
#include "InputClass.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

class D3DApp
{
private:
	struct Vertex
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT4 color;
	};

	struct ConstantBuffer
	{
		DirectX::XMFLOAT4X4 worldViewProj;
	};

public:
	D3DApp();
	D3DApp(const D3DApp& rhs) = delete;
	D3DApp& operator=(const D3DApp& rhs) = delete;
	~D3DApp();

	bool InitD3D(HWND hWnd);
	void Shutdown();

	void BeginScene();
	void UpdateScene(float dt);
	void DrawScene();
	void EndScene();

	ID3D11Device* GetDevice() const;
	ID3D11DeviceContext* GetDeviceContext() const;

	void BuildBuffers();
	void BuildShaders();
	void BuildInputLayout();

private:
	std::unique_ptr<Input> input;

	unsigned int mIndexCount;

	float x = 0.f;
	float y = 0.f;
	float z = -5.f;

	float vx = 0.f;
	float vy = 0.f;
	float vz = 0.f;

	ID3D11Device* pDevice;
	ID3D11DeviceContext* pImmediateContext;
	IDXGISwapChain* pSwapChain;
	ID3D11RenderTargetView* pRVT;
	D3D11_VIEWPORT vp;

	ID3D11Buffer* pVB;
	ID3D11Buffer* pIB;
	ID3D11Buffer* pCB;

	ID3D11InputLayout* pInputLayout;

	ID3D11VertexShader* pVertexShader;
	ID3D11PixelShader* pPixelShader;
	ID3DBlob* pBlob;

	DirectX::XMFLOAT4X4 mProjectionMatrix;
	DirectX::XMFLOAT4X4 mWorldMatrix;
	DirectX::XMFLOAT4X4 mViewMatrix;
};

