#pragma once
#include "Bindable.h"
#include <array>

namespace Bind
{
	class Rasterizer : public Bindable
	{
	public:
		Rasterizer(D3DApp& d3dApp, bool twoSided);
		void Bind(D3DApp& d3dApp) noexcept override;
		static std::shared_ptr<Rasterizer> Resolve(D3DApp& d3dApp, bool twoSided);
		static std::string GenerateUID(bool twoSided);
		std::string GetUID() const noexcept override;
	protected:
		Microsoft::WRL::ComPtr<ID3D11RasterizerState> pRasterizer;
		bool twoSided;
	};
}