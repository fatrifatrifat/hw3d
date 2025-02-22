#pragma once
#include "Bindable.h"

namespace Bind
{
	class PixelShader : public Bindable
	{
	public:
		PixelShader(D3DApp& d3dApp, const std::wstring& filename);
		void Bind(D3DApp& d3dApp) noexcept override;

	protected:
		Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader;
	};
}