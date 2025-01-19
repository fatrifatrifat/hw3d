#include "Box.h"

Box::Box(HINSTANCE hInstance)
	: App(hInstance)
{
	using namespace DirectX;
	
	XMMATRIX I = XMMatrixIdentity();
	XMStoreFloat4x4(&mWorld, I);
	XMStoreFloat4x4(&mView, I);
	XMStoreFloat4x4(&mProj, I);
}

void Box::InitApp()
{
	App::InitApp();

	BuildBuffers();
	BuildShaders();
	BuildInputLayout();
}

void Box::ClearBuffer()
{
	pImmediateContext->ClearRenderTargetView(pRVT, reinterpret_cast<const float*>(&Colors::blue));
}

void Box::UpdateScene(float dt)
{
}

void Box::Draw()
{
	ClearBuffer();

	pImmediateContext->IASetInputLayout(pInputLayout);
	pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	UINT stride = 28u;
	UINT offset = 0u;
	pImmediateContext->IASetVertexBuffers(0, 1, &pVB, &stride, &offset);

	pImmediateContext->PSSetShader(pPixelShader, nullptr, 0u);
	pImmediateContext->VSSetShader(pVertexShader, nullptr, 0u);

	pImmediateContext->OMSetRenderTargets(1u, &pRVT, nullptr);

	// configure viewport
	D3D11_VIEWPORT vp;
	vp.Width = 800;
	vp.Height = 600;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	pImmediateContext->RSSetViewports(1u, &vp);

	EndFrame();
}

void Box::EndFrame()
{
	pSwapChain->Present(0u, 0u);
}

void Box::BuildBuffers()
{
	using namespace DirectX;
	struct Vertex
	{
		XMFLOAT3 pos;
		XMFLOAT4 color;
	};

	// create vertex buffer (1 2d triangle at center of screen)
	const Vertex vertices[] =
	{
		{XMFLOAT3(0.0f,0.5f, 0.0f), Colors::blue},
		{XMFLOAT3(0.5f,-0.5f, 0.0f), Colors::green},
		{XMFLOAT3(-0.5f,-0.5f, 0.0f), Colors::red},
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
}

void Box::BuildShaders()
{
	// create pixel shader
	D3DReadFileToBlob(L"PixelShader.cso", &pBlob);
	pDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader);

	// create vertex shader
	D3DReadFileToBlob(L"VertexShader.cso", &pBlob);
	pDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader);
}

void Box::BuildInputLayout()
{
	// input (vertex) layout (2d position only)
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
