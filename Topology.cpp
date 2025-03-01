#include "Topology.h"
#include "BindableCodex.h"

namespace Bind
{
	Topology::Topology(D3DApp& d3dApp, D3D11_PRIMITIVE_TOPOLOGY type)
		:
		type(type)
	{}

	void Topology::Bind(D3DApp& d3dApp) noexcept
	{
		GetContext(d3dApp)->IASetPrimitiveTopology(type);
	}
	std::shared_ptr<Topology> Topology::Resolve(D3DApp& d3dApp, D3D11_PRIMITIVE_TOPOLOGY type)
	{
		return Codex::Resolve<Topology>(d3dApp, type);
	}
	std::string Topology::GenerateUID(D3D11_PRIMITIVE_TOPOLOGY type)
	{
		using namespace std::string_literals;
		return typeid(Topology).name() + "#"s + std::to_string(type);
	}
	std::string Topology::GetUID() const noexcept
	{
		return GenerateUID(type);
	}
}