#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <dxgi.h>
#include <algorithm>
#include <optional>
#include <wrl\client.h>
#include <cmath>
#include <vector>
#include <memory>
#include "GeometryGenerator.h"
#include "Scene.h"
#include "GameTimer.h"
#include "Utils.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

class App
{
public:
	App(HINSTANCE hInst);
	App(const App& rhs) = delete;
	App& operator=(const App& rhs) = delete;

	HWND GetHWnd() const;
	HINSTANCE GetInst() const;
	float AspectRatio() const;

	virtual void InitApp();
	int Go();
	void Shutdown();

	LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

protected:
	bool InitWin();
	bool InitD3D();

	virtual void ClearBuffer();
	virtual void UpdateScene(float dt);
	virtual void Draw();
	virtual void EndFrame();

	virtual void BuildBuffers();
	virtual void BuildShaders();
	virtual void BuildInputLayout();

	static std::optional<int> ProcessMessage();

protected:
	HWND hMainWnd;
	HINSTANCE hInstance;
	GameTimer gm;

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

private:
	float GetHeight(float x, float z) const;

private:
	std::vector<std::unique_ptr<Scene>> scenes;
	std::vector<std::unique_ptr<Scene>>::iterator currScene;

	struct Vertex
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT4 color;
	};

	struct ConstantBuffer
	{
		DirectX::XMFLOAT4X4 worldViewProj;
	};
	
	DirectX::XMFLOAT4X4 mGridWorld;

	UINT mGridIndexCount;

	DirectX::XMFLOAT4X4 mWorld;
	DirectX::XMFLOAT4X4 mView;
	DirectX::XMFLOAT4X4 mProj;

	float mTheta = PI * 1.5f;
	float mPhi = PI * 0.25f;
	float mRadius = 5.0f;

	float dTheta = PI * 0.0001f;

};

