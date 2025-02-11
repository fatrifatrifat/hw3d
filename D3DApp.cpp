#include "D3DApp.h"
#include "Utils.h"
#include <algorithm>
#include "GeometryGenerator.h"
#include <fstream>

D3DApp::D3DApp()
{
	/*pSwapChain = 0;
	pDevice = 0;
	pImmediateContext = 0;
	pRVT = 0;*/
	
	input = std::make_unique<Input>();

	using namespace DirectX;

	XMMATRIX I = XMMatrixIdentity();
	XMStoreFloat4x4(&mWorldMatrixCube1, I);
	XMStoreFloat4x4(&mWorldMatrixCube2, I);

	XMStoreFloat4x4(&mViewMatrix, I);
	XMStoreFloat4x4(&mProjectionMatrix, I);

	XMMATRIX P = XMMatrixPerspectiveFovLH(0.25f * PI, 800.f/600.f, 1.0f, 1000.0f);
	XMStoreFloat4x4(&mProjectionMatrix, P);
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
	vp.Width = 800;
	vp.Height = 600;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	pImmediateContext->RSSetViewports(1u, &vp);

	gt.Start();

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
	pImmediateContext->ClearRenderTargetView(pRVT.Get(), reinterpret_cast<const float*>(&Colors::black));
	pImmediateContext->ClearDepthStencilView(pDSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
}

void D3DApp::UpdateScene(float dt)
{
	input->Update();
	using namespace DirectX;

	if (input->IsKeyPressed('W')) vz = -0.001;
	else if (input->IsKeyPressed('S')) vz = 0.001;
	else vz = 0.f;

	if (input->IsKeyPressed('A')) vx = 0.001;
	else if (input->IsKeyPressed('D')) vx = -0.001;
	else vx = 0.f;

	if (input->IsKeyPressed('Q')) vy = 0.001;
	else if (input->IsKeyPressed('E')) vy = -0.001;
	else vy = 0.f;

	if (input->IsKeyPressed('P'))
		currRenderState = true;
	else if(input->IsKeyPressed('O'))
		currRenderState = false;

	x += vx * dt;
	y += vy * dt;
	z += vz * dt;

	if (input->IsKeyPressed('R'))
		cube2Rotation += 0.001 * dt;


	// Build the view matrix
	XMVECTOR pos = XMVectorSet(x, y, -5.0f, 1.0f);
	XMVECTOR target = XMVectorZero();
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	// Build the WVP matrix
	XMMATRIX worldCube1 = XMLoadFloat4x4(&mWorldMatrixCube1);
	XMMATRIX worldCube2 = XMLoadFloat4x4(&mWorldMatrixCube2);

	XMMATRIX view = XMMatrixLookAtLH(pos, target, up);
	XMStoreFloat4x4(&mViewMatrix, view);

	XMMATRIX proj = XMLoadFloat4x4(&mProjectionMatrix);

	XMMATRIX wvpCube1 = worldCube1 * view * proj;
	XMMATRIX wvpCube2 = worldCube2 * view * proj;

	struct ConstantBuffer2
	{
		struct
		{
			XMFLOAT4 color;
		} face_colors[6];
	};

	const ConstantBuffer2 cb2 =
	{
		{
			Colors::blue,
			Colors::red,
			Colors::green,
			Colors::yellow,
			Colors::magenta,
			Colors::purple
		}
	};

	//pImmediateContext->UpdateSubresource(pCB2, 0, nullptr, &cb2, 0, 0);
}

void D3DApp::DrawScene()
{
	// Bind vertex buffer to pipeline
	const UINT stride = sizeof(Vertex);
	const UINT offset = 0u;
	pImmediateContext->IASetVertexBuffers(0u, 1u, pVB.GetAddressOf(), &stride, &offset);
	pImmediateContext->IASetIndexBuffer(pIB.Get(), DXGI_FORMAT_R32_UINT, 0u);

	// bind pixel shader
	pImmediateContext->PSSetShader(pPixelShader.Get(), nullptr, 0u);
	//pImmediateContext->PSSetConstantBuffers(0u, 1u, &pCB2);

	// bind vertex shader
	pImmediateContext->VSSetShader(pVertexShader.Get(), nullptr, 0u);
	pImmediateContext->VSSetConstantBuffers(0u, 1u, pCB.GetAddressOf());

	// bind vertex layout
	pImmediateContext->IASetInputLayout(pInputLayout.Get());

	if(currRenderState)
		pImmediateContext->RSSetState(pRasterWireFrame.Get());
	else
		pImmediateContext->RSSetState(pRasterSolidState.Get());

	ConstantBuffer cb;
	XMStoreFloat4x4(&cb.worldViewProj, XMMatrixTranspose(XMLoadFloat4x4(&mWorldMatrixCube1) * XMLoadFloat4x4(&mViewMatrix) * XMLoadFloat4x4(&mProjectionMatrix)));
	pImmediateContext->UpdateSubresource(pCB.Get(), 0, nullptr, &cb, 0, 0);
	pImmediateContext->DrawIndexed((UINT)mIndexCount, 0u, 0u);

	// Draw Cube 2 (Rotating Cube)
	XMStoreFloat4x4(&cb.worldViewProj, XMMatrixTranspose(XMLoadFloat4x4(&mWorldMatrixCube2) * XMMatrixRotationY(cube2Rotation) * XMMatrixTranslation(0, 0, z) * XMLoadFloat4x4(&mViewMatrix) * XMLoadFloat4x4(&mProjectionMatrix)));
	pImmediateContext->UpdateSubresource(pCB.Get(), 0, nullptr, &cb, 0, 0);
	pImmediateContext->DrawIndexed((UINT)mIndexCount, 0u, 0u);
}

void D3DApp::EndScene()
{
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

void D3DApp::BuildBuffers()
{
	using namespace DirectX;

	GeometryGenerator geoGen;
	GeometryGenerator::MeshData mesh;

	float width = 2.0f;
	float height = 2.0f;
	float depth = 2.0f;
	//geoGen.CreateBox(width, height, depth, mesh);
	//geoGen.CreateSphere(1, 75, 75, mesh);
	geoGen.CreateModel(mesh, "Models/Skull.txt");
	//geoGen.CreateModel(mesh, "Models/Car.txt");
	//geoGen.CreateGrid(160.0f, 160.0f, 50, 50, mesh);

	D3D11_BUFFER_DESC vbd = {};
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex) * mesh.Vertices.size();
	//vbd.ByteWidth = sizeof(XMFLOAT3) * 8;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vinitData = {};
	vinitData.pSysMem = mesh.Vertices.data();
	//vinitData.pSysMem = vertices;

	HRESULT hr = pDevice->CreateBuffer(&vbd, &vinitData, &pVB);
	if (FAILED(hr))
	{
		MessageBox(0, L"Failed to create vertex buffer", 0, 0);
		return;
	}

	mIndexCount = mesh.Indices.size();
	//mIndexCount = 36;

	D3D11_BUFFER_DESC ibd = {};
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * mIndexCount;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA iinitData = {};
	iinitData.pSysMem = mesh.Indices.data();
	//iinitData.pSysMem = indices;


	hr = pDevice->CreateBuffer(&ibd, &iinitData, &pIB);
	if (FAILED(hr))
	{
		MessageBox(0, L"Failed to create index buffer", 0, 0);
		return;
	}

	D3D11_BUFFER_DESC cbd = {};
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.Usage = D3D11_USAGE_DEFAULT;
	cbd.ByteWidth = sizeof(ConstantBuffer);
	cbd.CPUAccessFlags = 0u;
	cbd.MiscFlags = 0u;

	hr = pDevice->CreateBuffer(&cbd, nullptr, &pCB);
	if (FAILED(hr))
	{
		MessageBox(0, L"Failed to create constant buffer", 0, 0);
		return;
	}

	struct ConstantBuffer2
	{
		struct
		{
			XMFLOAT4 color;
		} face_colors[6];
	};

	const ConstantBuffer2 cb2 =
	{
		{
			Colors::blue,
			Colors::red,
			Colors::green,
			Colors::yellow,
			Colors::magenta,
			Colors::purple
		}
	};

	D3D11_BUFFER_DESC cbd2 = {};
	cbd2.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd2.Usage = D3D11_USAGE_DEFAULT;
	cbd2.ByteWidth = sizeof(ConstantBuffer2);
	cbd2.CPUAccessFlags = 0u;
	cbd2.MiscFlags = 0u;

	hr = pDevice->CreateBuffer(&cbd2, nullptr, &pCB2);
	if (FAILED(hr))
	{
		MessageBox(0, L"Failed to create constant buffer 2", 0, 0);
		return;
	}

}


void D3DApp::BuildShaders()
{
	D3DReadFileToBlob(L"PixelShader.cso", pBlob.GetAddressOf());
	//D3DReadFileToBlob(L"BoxPS.cso", &pBlob);
	pDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader);

	D3DReadFileToBlob(L"VertexShader.cso", pBlob.GetAddressOf());
	//D3DReadFileToBlob(L"BoxVS.cso", &pBlob);
	pDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader);
}

void D3DApp::BuildInputLayout()
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