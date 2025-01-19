#pragma once
#include "App.h"

class Box : public App
{
public:
	Box(HINSTANCE hInstance);

	virtual void InitApp();

	virtual void ClearBuffer();
	virtual void UpdateScene(float dt);
	virtual void Draw();
	virtual void EndFrame();

private:
	void BuildBuffers();
	void BuildShaders();
	void BuildInputLayout();

private:
	ID3D11Buffer* pVB;
	ID3D11Buffer* pIB;

	ID3D11VertexShader* pVertexShader;
	ID3D11PixelShader* pPixelShader;
	ID3DBlob* pBlob;

	ID3D11InputLayout* pInputLayout;

	DirectX::XMFLOAT4X4 mWorld;
	DirectX::XMFLOAT4X4 mView;
	DirectX::XMFLOAT4X4 mProj;

	float mTheta = PI * 1.5f;
	float mPhi = PI * 0.25f;
	float mRadius = 5.0f;
};

