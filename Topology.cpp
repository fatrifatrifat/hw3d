#include "Topology.h"

namespace Bind
{
	Topology::Topology(D3DApp& d3dApp, D3D11_PRIMITIVE_TOPOLOGY type)
		: type(type)
	{}

	void Topology::Bind(D3DApp& d3dApp) noexcept
	{
		GetContext(d3dApp)->IASetPrimitiveTopology(type);
	}
}