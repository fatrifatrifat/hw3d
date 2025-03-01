#pragma once
#include "Bindable.h"

namespace Bind
{
	class PixelShader : public Bindable
	{
	public:
		PixelShader(D3DApp& d3dApp, const std::string& path);
		void Bind(D3DApp& d3dApp) noexcept override;
		static std::shared_ptr<PixelShader> Resolve(D3DApp& d3dApp, const std::string& path);
		static std::string GenerateUID(const std::string& path);
		std::string GetUID() const noexcept override;
	protected:
		std::string path;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader;
	};
}