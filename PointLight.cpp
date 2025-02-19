#include "PointLight.h"
#include "imgui/imgui.h"

PointLight::PointLight(D3DApp& d3dApp, float radius)
	:
	mesh(d3dApp, radius),
	cbuf(d3dApp)
{}

void PointLight::SpawnControlWindow() noexcept
{
	if (ImGui::Begin("Light"))
	{
		ImGui::Text("Position");
		ImGui::SliderFloat("X", &pos.x, -60.0f, 60.0f, "%.1f");
		ImGui::SliderFloat("Y", &pos.y, -60.0f, 60.0f, "%.1f");
		ImGui::SliderFloat("Z", &pos.z, -60.0f, 60.0f, "%.1f");
		if (ImGui::Button("Reset"))
		{
			Reset();
		}
	}
	ImGui::End();
}

void PointLight::Reset() noexcept
{
	pos = { 0.0f,0.0f,0.0f };
}

void PointLight::Draw(D3DApp& d3dApp) const noexcept
{
	mesh.SetPos(pos);
	mesh.Draw(d3dApp);
}

void PointLight::Bind(D3DApp& d3dApp) const noexcept
{
	cbuf.Update(d3dApp, PointLightCBuf{ pos });
	cbuf.Bind(d3dApp);
}