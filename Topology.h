#pragma once
#include "Bindable.h"

namespace Bind
{
	class Topology : public Bindable
	{
	public:
		Topology(D3DApp& d3dApp, D3D11_PRIMITIVE_TOPOLOGY type);
		void Bind(D3DApp& d3dApp) noexcept override;
		static std::shared_ptr<Topology> Resolve(D3DApp& d3dApp, D3D11_PRIMITIVE_TOPOLOGY type);
		static std::string GenerateUID(D3D11_PRIMITIVE_TOPOLOGY type);
		std::string GetUID() const noexcept override;
	protected:
		D3D11_PRIMITIVE_TOPOLOGY type;
	};
}