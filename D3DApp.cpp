#include "D3DApp.h"
#include "Utils.h"
#include <algorithm>
#include "imgui\imgui_impl_dx11.h"
#include "imgui\imgui_impl_win32.h"
#include <fstream>

D3DApp::D3DApp()
{
	//input = std::make_unique<Input>();

	using namespace DirectX;

	XMMATRIX I = XMMatrixIdentity();
	XMStoreFloat4x4(&mWorldMatrix, I);

	XMStoreFloat4x4(&mViewMatrix, I);
	XMStoreFloat4x4(&mProjectionMatrix, I);
	

	XMMATRIX P = XMMatrixPerspectiveFovLH(0.25f * PI, 800.f/600.f, 1.0f, 1000.0f);
	XMStoreFloat4x4(&mProjectionMatrix, P);
}

D3DApp::~D3DApp()
{
	ImGui_ImplDX11_Shutdown();
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

	ComPtr<ID3D11Resource> pBackBuffer;
	pSwapChain->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer);
	pDevice->CreateRenderTargetView(pBackBuffer.Get(), 0, &pRVT);
	pBackBuffer->Release();

	D3D11_DEPTH_STENCIL_DESC dsd = {};
	dsd.DepthEnable = true;
	dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsd.DepthFunc = D3D11_COMPARISON_LESS;
	ComPtr<ID3D11DepthStencilState> pDSState;
	pDevice->CreateDepthStencilState(&dsd, &pDSState);

	pImmediateContext->OMSetDepthStencilState(pDSState.Get(), 1u);

	ComPtr<ID3D11Texture2D> pDepthStencil;
	D3D11_TEXTURE2D_DESC descDepth = {};
	descDepth.Width = 800u;
	descDepth.Height = 600u;
	descDepth.MipLevels = 1u;
	descDepth.ArraySize = 1u;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count = 1u;
	descDepth.SampleDesc.Quality = 0u;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	pDevice->CreateTexture2D(&descDepth, nullptr, &pDepthStencil);

	D3D11_DEPTH_STENCIL_VIEW_DESC ddsv = {};
	ddsv.Format = DXGI_FORMAT_D32_FLOAT;
	ddsv.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	ddsv.Texture2D.MipSlice = 0u;

	pDevice->CreateDepthStencilView(pDepthStencil.Get(), &ddsv, &pDSV);
	pImmediateContext->OMSetRenderTargets(1u, pRVT.GetAddressOf(), pDSV.Get());

	// Set primitive topology to triangle list (groups of 3 vertices)
	pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	D3D11_RASTERIZER_DESC rd = {};
	rd.FillMode = D3D11_FILL_SOLID;
	rd.FrontCounterClockwise = false;
	rd.DepthClipEnable = true;
	pDevice->CreateRasterizerState(&rd, &pRasterSolidState);

	rd.FillMode = D3D11_FILL_WIREFRAME;
	rd.CullMode = D3D11_CULL_NONE;
	pDevice->CreateRasterizerState(&rd, &pRasterWireFrame);

	// configure viewport
	D3D11_VIEWPORT vp;
	vp.Width = 800.0f;
	vp.Height = 600.0f;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	pImmediateContext->RSSetViewports(1u, &vp);

	ImGui_ImplDX11_Init(pDevice.Get(), pImmediateContext.Get());

	return true;
}

void D3DApp::BeginScene()
{
	if (imguiEnabled)
	{
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
	}

	pImmediateContext->ClearRenderTargetView(pRVT.Get(), reinterpret_cast<const float*>(&Colors::black));
	pImmediateContext->ClearDepthStencilView(pDSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
}

void D3DApp::DrawIndexed(UINT count)
{
	pImmediateContext->DrawIndexed(count, 0u, 0u);
}

void D3DApp::EndScene()
{
	if (imguiEnabled)
	{
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}

	pSwapChain->Present(0, 0);
}

ID3D11Device* D3DApp::GetDevice() const
{
	return pDevice.Get();
}

ID3D11DeviceContext* D3DApp::GetDeviceContext() const
{
	return pImmediateContext.Get();
}

DirectX::XMMATRIX D3DApp::GetCamera() const
{
	return XMLoadFloat4x4(&mCamera);
}

DirectX::XMMATRIX D3DApp::GetWorldMatrix() const
{
	return XMLoadFloat4x4(&mWorldMatrix);
}

DirectX::XMMATRIX D3DApp::GetProjMatrix() const
{
	return XMLoadFloat4x4(&mProjectionMatrix);
}

DirectX::XMMATRIX D3DApp::GetViewMatrix() const
{
	return XMLoadFloat4x4(&mViewMatrix);
}

void D3DApp::SetCamera(DirectX::FXMMATRIX camera) noexcept
{
	XMStoreFloat4x4(&mCamera, camera);
}

void D3DApp::SetProjection(DirectX::FXMMATRIX proj) noexcept
{
	XMStoreFloat4x4(&mProjectionMatrix, proj);
}

void D3DApp::EnableImgui() noexcept
{
	imguiEnabled = true;
}

void D3DApp::DisableImgui() noexcept
{
	imguiEnabled = false;
}

bool D3DApp::IsImguiEnabled() const noexcept
{
	return imguiEnabled;
}