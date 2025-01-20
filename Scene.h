#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include "Utils.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

class Scene
{
public:
	Scene(ID3D11Device* pD, ID3D11DeviceContext* pDC)
		: pDevice(pD)
		, pImmediateContext(pDC)
	{}

	virtual void UpdateScene() = 0;
	virtual void Draw() = 0;

protected:
	virtual void BuildBuffers() = 0;
	virtual void BuildShaders() = 0;
	virtual void BuildInputLayout() = 0;

protected:
	ID3D11Device* pDevice = nullptr;
	ID3D11DeviceContext* pImmediateContext = nullptr;

	ID3D11Buffer* pVB = nullptr;
	ID3D11Buffer* pIB = nullptr;
	ID3D11Buffer* pCB = nullptr;

	ID3D11InputLayout* pInputLayout = nullptr;

	ID3D11VertexShader* pVertexShader = nullptr;
	ID3D11PixelShader* pPixelShader = nullptr;
	ID3DBlob* pBlob = nullptr;

};