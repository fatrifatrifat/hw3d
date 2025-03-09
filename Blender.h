#pragma once
#include "Bindable.h"
#include <array>
#include <optional>

namespace Bind
{
	class Blender : public Bindable
	{
	public:
		Blender(D3DApp& d3dApp, bool blending, std::optional<float> factor = {});
		void Bind(D3DApp& d3dApp) noexcept override;
		void SetFactor(float factor);
		float GetFactor() const;
		static std::shared_ptr<Blender> Resolve(D3DApp& d3dApp, bool blending, std::optional<float> factor = {});
		static std::string GenerateUID(bool blending, std::optional<float> factor);
		std::string GetUID() const noexcept override;
	protected:
		Microsoft::WRL::ComPtr<ID3D11BlendState> pBlender;
		bool blending;
		std::optional<std::array<float, 4>> factors;
	};
}