#include "D3DApp.h"
#include "Utils.h"

D3DApp::D3DApp()
{
	pSwapChain = 0;
	pDevice = 0;
	pImmediateContext = 0;
	pRVT = 0;
}

D3DApp::~D3DApp()
{
}

bool D3DApp::InitD3D(HWND hWnd)
{
	D3D_FEATURE_LEVEL featureLevel;

	DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width = 800;
	sd.BufferDesc.Height = 600;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;

	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = hWnd;
	sd.Windowed = true;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;

	HRESULT hr = D3D11CreateDeviceAndSwapChain(0, D3D_DRIVER_TYPE_HARDWARE, 0, 0, 0, 0, D3D11_SDK_VERSION, &sd, &pSwapChain, &pDevice, &featureLevel, &pImmediateContext);

	if (FAILED(hr))
	{
		MessageBox(0, L"CreateDeviceAndSwapChain Failed", 0, 0);
		return false;
	}

	ID3D11Texture2D* pBackBuffer;
	pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pBackBuffer));
	pDevice->CreateRenderTargetView(pBackBuffer, 0, &pRVT);
	pBackBuffer->Release();

	// bind render target
	pImmediateContext->OMSetRenderTargets(1u, &pRVT, nullptr);


	// Set primitive topology to triangle list (groups of 3 vertices)
	pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	// configure viewport
	vp.Width = 800;
	vp.Height = 600;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	pImmediateContext->RSSetViewports(1u, &vp);

	return true;
}

void D3DApp::Shutdown()
{
	if (pDevice)
		pDevice->Release();

	if (pSwapChain)
		pSwapChain->Release();

	if (pImmediateContext)
		pImmediateContext->Release();

	if (pRVT)
		pRVT->Release();
}

void D3DApp::BeginScene()
{
	pImmediateContext->ClearRenderTargetView(pRVT, reinterpret_cast<const float*>(&Colors::blue));
}

void D3DApp::EndScene()
{
	pSwapChain->Present(0, 0);
}

ID3D11Device* D3DApp::GetDevice() const
{
	return pDevice;
}

ID3D11DeviceContext* D3DApp::GetDeviceContext() const
{
	return pImmediateContext;
}

void D3DApp::GetProjectionMatrix(DirectX::XMFLOAT4X4&)
{
}

void D3DApp::GetWorldMatrix(DirectX::XMFLOAT4X4&)
{
}

void D3DApp::GetOrthoMatrix(DirectX::XMFLOAT4X4&)
{
}
