#include "Rasterizer.h"
#include "BindableCodex.h"

namespace Bind
{
	Rasterizer::Rasterizer(D3DApp& d3dApp, bool twoSided)
		:
		twoSided(twoSided)
	{
		D3D11_RASTERIZER_DESC rasterDesc = CD3D11_RASTERIZER_DESC(CD3D11_DEFAULT{});
		rasterDesc.CullMode = twoSided ? D3D11_CULL_NONE : D3D11_CULL_BACK;

		GetDevice(d3dApp)->CreateRasterizerState(&rasterDesc, &pRasterizer);
	}

	void Rasterizer::Bind(D3DApp& d3dApp) noexcept
	{
		GetContext(d3dApp)->RSSetState(pRasterizer.Get());
	}

	std::shared_ptr<Rasterizer> Rasterizer::Resolve(D3DApp& d3dApp, bool twoSided)
	{
		return Codex::Resolve<Rasterizer>(d3dApp, twoSided);
	}
	std::string Rasterizer::GenerateUID(bool twoSided)
	{
		using namespace std::string_literals;
		return typeid(Rasterizer).name() + "#"s + (twoSided ? "2s" : "1s");
	}
	std::string Rasterizer::GetUID() const noexcept
	{
		return GenerateUID(twoSided);
	}
}