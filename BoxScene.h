#pragma once
#include "Scene.h"
#include <DirectXMath.h>

class BoxScene : public Scene
{
public:
	BoxScene(ID3D11Device* pD, ID3D11DeviceContext* pDC)
		: Scene(pD, pDC)
	{
		BuildBuffers();
		BuildShaders();
		BuildInputLayout();
	}

	virtual void UpdateScene() override
	{

	}

	virtual void Draw() override
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

		pImmediateContext->DrawIndexed(36u, 0u, 0u);
	}

private:
	virtual void BuildBuffers() override
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

		/*D3D11_BUFFER_DESC cbd = {};
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd.Usage = D3D11_USAGE_DEFAULT;
		cbd.ByteWidth = sizeof(ConstantBuffer);
		cbd.CPUAccessFlags = 0u;
		cbd.MiscFlags = 0u;
		pDevice->CreateBuffer(&cbd, nullptr, &pCB);*/
	}

	virtual void BuildShaders() override
	{
		D3DReadFileToBlob(L"PixelShader.cso", &pBlob);
		pDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader);

		D3DReadFileToBlob(L"VertexShader.cso", &pBlob);
		pDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader);
	}

	virtual void BuildInputLayout() override
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

	struct Vertex
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT4 color;
	};

	struct ConstantBuffer
	{
		DirectX::XMFLOAT4X4 worldViewProj;
	};

	DirectX::XMFLOAT4X4 mWorld;
	DirectX::XMFLOAT4X4 mView;
	DirectX::XMFLOAT4X4 mProj;

	float mTheta = PI * 1.5f;
	float mPhi = PI * 0.25f;
	float mRadius = 5.0f;

	float dTheta = PI * 0.0001f;
};