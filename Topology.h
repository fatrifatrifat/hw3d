#pragma once
#include "Bindable.h"

namespace Bind
{
	class Topology : public Bindable
	{
	public:
		Topology(D3DApp& d3dApp, D3D11_PRIMITIVE_TOPOLOGY type);
		void Bind(D3DApp& d3dApp) noexcept override;

	protected:
		D3D11_PRIMITIVE_TOPOLOGY type;
	};
}