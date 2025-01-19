#include "App.h"

namespace
{
	App* app = 0;
}

LRESULT CALLBACK
MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return app->WndProc(hwnd, msg, wParam, lParam);
}

App::App(HINSTANCE hInst)
	: hInstance(hInst)
{
	app = this;	

	using namespace DirectX;

	XMMATRIX I = XMMatrixIdentity();
	XMStoreFloat4x4(&mWorld, I);
	XMStoreFloat4x4(&mView, I);
	XMStoreFloat4x4(&mProj, I);

	XMMATRIX P = XMMatrixPerspectiveFovLH(0.25f * PI, AspectRatio(), 1.0f, 1000.0f);
	XMStoreFloat4x4(&mProj, P);
}

HWND App::GetHWnd() const
{
	return hMainWnd;
}

HINSTANCE App::GetInst() const
{
	return hInstance;
}

float App::AspectRatio() const
{
	return 800.0f / 600.0f;
}

void App::InitApp()
{
	if (!InitWin())
	{
		MessageBox(0, L"Window Initialization Failed", 0, 0);
		return;
	}
	if (!InitD3D())
	{
		MessageBox(0, L"D3D Initialization Failed", 0, 0);
		return;
	}

	BuildBuffers();
	BuildShaders();
	BuildInputLayout();
}

int App::Go()
{
	while (true)
	{
		if (const auto ecode = App::ProcessMessage())
		{
			return *ecode;
		}
		UpdateScene(gm.DeltaTime());
		ClearBuffer();
		Draw();
		EndFrame();
	}

	return 0;
}

void App::Shutdown()
{
	if (pDevice) { pDevice->Release(); pDevice = nullptr; }
	if (pImmediateContext) { pImmediateContext->Release(); pImmediateContext = nullptr; }
	if (pSwapChain) { pSwapChain->Release(); pSwapChain = nullptr; }
	if (pRVT) { pRVT->Release(); pRVT = nullptr; }

	UnregisterClass(L"D3DWndClassName", hInstance);
}

LRESULT App::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_LBUTTONDOWN:
		MessageBox(0, L"Hi", 0, 0);
		return 0;
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
			DestroyWindow(hwnd);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

bool App::InitWin()
{
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = MainWndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	wc.lpszMenuName = 0;
	wc.lpszClassName = L"D3DWndClassName";

	if (!RegisterClass(&wc))
	{
		MessageBox(0, L"RegisterClass Failed.", 0, 0);
		return false;
	}

	// Compute window rectangle dimensions based on requested client area dimensions.
	RECT R = { 0, 0, 800, 600 };
	AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, false);
	int width = R.right - R.left;
	int height = R.bottom - R.top;

	hMainWnd = CreateWindow(L"D3DWndClassName", L"Rifat Katranci",
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height, 0, 0, hInstance, 0);
	if (!hMainWnd)
	{
		MessageBox(0, L"CreateWindow Failed.", 0, 0);
		return false;
	}

	ShowWindow(hMainWnd, SW_SHOW);
	UpdateWindow(hMainWnd);

	return true;
}

bool App::InitD3D()
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
	sd.OutputWindow = hMainWnd;
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

void App::ClearBuffer()
{
	pImmediateContext->ClearRenderTargetView(pRVT, reinterpret_cast<const float*>(&Colors::blue));
}

void App::UpdateScene(float dt)
{
	using namespace DirectX;

	mTheta += dTheta * dt;
	mPhi += dTheta * dt;

	float x = mRadius * sinf(mPhi) * cosf(mTheta);
	float z = mRadius * sinf(mPhi) * sinf(mTheta);
	float y = mRadius * cosf(mPhi);

	// Build the view matrix
	XMVECTOR pos = XMVectorSet(x, y, z, 1.0f);
	XMVECTOR target = XMVectorZero();
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMMATRIX view = XMMatrixLookAtLH(pos, target, up);

	// Build the WVP matrix
	XMMATRIX world = XMLoadFloat4x4(&mWorld);
	XMMATRIX proj = XMLoadFloat4x4(&mProj);
	XMMATRIX wvp = world * view * proj;

	XMStoreFloat4x4(&mView, view);
	ConstantBuffer cb;
	XMStoreFloat4x4(&cb.worldViewProj, XMMatrixTranspose(wvp));

	// Update the constant buffer
	pImmediateContext->UpdateSubresource(pCB, 0, nullptr, &cb, 0, 0);
}


void App::Draw()
{
	// Bind vertex buffer to pipeline
	const UINT stride = sizeof(Vertex);
	const UINT offset = 0u;
	pImmediateContext->IASetVertexBuffers(0u, 1u, &pVB, &stride, &offset);
	pImmediateContext->IASetIndexBuffer(pIB, DXGI_FORMAT_R16_UINT, 0u);

	// bind pixel shader
	pImmediateContext->PSSetShader(pPixelShader, nullptr, 0u);

	// bind vertex shader
	pImmediateContext->VSSetShader(pVertexShader, nullptr, 0u);
	pImmediateContext->VSSetConstantBuffers(0u, 1u, &pCB);

	// bind vertex layout
	pImmediateContext->IASetInputLayout(pInputLayout);

	// bind render target
	pImmediateContext->OMSetRenderTargets(1u, &pRVT, nullptr);

	pImmediateContext->DrawIndexed(36u, 0u, 0u);
}

void App::EndFrame()
{
	pSwapChain->Present(0, 0);
}

void App::BuildBuffers()
{
	using namespace DirectX;
	// create vertex buffer (1 2d triangle at center of screen)
	const Vertex vertices[] =
	{
		{ XMFLOAT3(-1.0f, +1.0f, -1.0f), Colors::red },
		{ XMFLOAT3(-1.0f, -1.0f, -1.0f), Colors::gold },
		{ XMFLOAT3(+1.0f, +1.0f, -1.0f), Colors::lime },
		{ XMFLOAT3(+1.0f, -1.0f, -1.0f), Colors::white },
		{ XMFLOAT3(-1.0f, -1.0f, +1.0f), Colors::pink },
		{ XMFLOAT3(-1.0f, +1.0f, +1.0f), Colors::orange },
		{ XMFLOAT3(+1.0f, +1.0f, +1.0f), Colors::black },
		{ XMFLOAT3(+1.0f, -1.0f, +1.0f), Colors::magenta }
	};

	D3D11_BUFFER_DESC bd = {};
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0u;
	bd.MiscFlags = 0u;
	bd.ByteWidth = sizeof(vertices);
	bd.StructureByteStride = sizeof(Vertex);
	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = vertices;
	pDevice->CreateBuffer(&bd, &sd, &pVB);

	const unsigned short indices[] = {
		// Front face
		0, 2, 1, 2, 3, 1,

		// Back face
		5, 4, 6, 6, 4, 7,

		// Left face
		5, 0, 4, 4, 0, 1,

		// Right face
		2, 6, 3, 3, 6, 7,

		// Top face
		5, 2, 0, 5, 6, 2,

		// Bottom face
		1, 3, 4, 4, 3, 7
	};



	D3D11_BUFFER_DESC ibd = {};
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.CPUAccessFlags = 0u;
	ibd.MiscFlags = 0u;
	ibd.ByteWidth = sizeof(indices);
	ibd.StructureByteStride = sizeof(unsigned short);
	D3D11_SUBRESOURCE_DATA isd = {};
	isd.pSysMem = indices;
	pDevice->CreateBuffer(&ibd, &isd, &pIB);

	/*D3D11_BUFFER_DESC cbd = {};
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.Usage = D3D11_USAGE_DYNAMIC;
	cbd.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
	cbd.MiscFlags = 0u;
	cbd.ByteWidth = sizeof(ConstantBuffer);
	cbd.StructureByteStride = 0u;
	D3D11_SUBRESOURCE_DATA csd = {};
	csd.pSysMem = &cb;
	pDevice->CreateBuffer(&cbd, &csd, &pCB);*/

	D3D11_BUFFER_DESC cbd = {};
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.Usage = D3D11_USAGE_DEFAULT;
	cbd.ByteWidth = sizeof(ConstantBuffer);
	cbd.CPUAccessFlags = 0u;
	cbd.MiscFlags = 0u;
	pDevice->CreateBuffer(&cbd, nullptr, &pCB);
}

void App::BuildShaders()
{
	D3DReadFileToBlob(L"PixelShader.cso", &pBlob);
	pDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader);

	D3DReadFileToBlob(L"VertexShader.cso", &pBlob);
	pDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader);
}

void App::BuildInputLayout()
{
	const D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{ "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "COLOR",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0, 12,D3D11_INPUT_PER_VERTEX_DATA,0 },
	};
	pDevice->CreateInputLayout(
		ied, (UINT)std::size(ied),
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		&pInputLayout
	);
}

std::optional<int> App::ProcessMessage()
{
	MSG msg = {};

	while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
		{
			return (int)msg.wParam;
		}
		else
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return {};
}