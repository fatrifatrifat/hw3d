#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <wrl\client.h>
#include <dxgi.h>
#include <memory>
#include "InputClass.h"
#include "GameTimer.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

using namespace Microsoft::WRL;

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

	float GetHeight(float x, float z) const;

private:
	std::unique_ptr<Input> input;

	UINT mIndexCount;
	GameTimer gt;

	float x = 0.f;
	float y = 0.f;
	float z = -5.f;

	float vx = 0.f;
	float vy = 0.f;
	float vz = 0.f;

	float roll = 0.0f;
	float pitch = 0.0f;
	float yaw = 0.0f;

	bool currRenderState = true;

	ComPtr<ID3D11Device> pDevice;
	ComPtr<ID3D11DeviceContext> pImmediateContext;
	ComPtr<IDXGISwapChain> pSwapChain;
	ComPtr<ID3D11RenderTargetView> pRVT;
	ComPtr<ID3D11DepthStencilView> pDSV;

	ComPtr<ID3D11RasterizerState> pRasterWireFrame;
	ComPtr<ID3D11RasterizerState> pRasterSolidState;

	ComPtr<ID3D11Buffer> pVB;
	ComPtr<ID3D11Buffer> pIB;
	ComPtr<ID3D11Buffer> pCB;
	ComPtr<ID3D11Buffer> pCB2;
	ComPtr<ID3D11InputLayout> pInputLayout;
	
	ComPtr<ID3D11VertexShader> pVertexShader;
	ComPtr<ID3D11PixelShader> pPixelShader;
	ComPtr<ID3DBlob> pBlob;

	/*ID3D11Device* pDevice;
	ID3D11DeviceContext* pImmediateContext;
	IDXGISwapChain* pSwapChain;
	ID3D11RenderTargetView* pRVT;
	ID3D11RasterizerState* pRasterWireFrame;
	ID3D11RasterizerState* pRasterSolidState;
	ID3D11DepthStencilView* pDSV;
	D3D11_VIEWPORT vp;

	ID3D11Buffer* pVB;
	ID3D11Buffer* pIB;
	ID3D11Buffer* pCB;
	ID3D11Buffer* pCB2;

	ID3D11InputLayout* pInputLayout;

	ID3D11VertexShader* pVertexShader;
	ID3D11PixelShader* pPixelShader;
	ID3DBlob* pBlob;*/

	DirectX::XMFLOAT4X4 mProjectionMatrix;
	DirectX::XMFLOAT4X4 mWorldMatrixCube1;
	DirectX::XMFLOAT4X4 mWorldMatrixCube2;
	DirectX::XMFLOAT4X4 mViewMatrix;
};

