#pragma once
#include "D3DApp.h"

class Bindable
{
public:
	virtual void Bind(D3DApp& d3dApp) noexcept = 0;
	virtual ~Bindable() = default;

protected:
	static ID3D11DeviceContext* GetContext(D3DApp& d3dApp) noexcept;
	static ID3D11Device* GetDevice(D3DApp& d3dApp) noexcept;
};