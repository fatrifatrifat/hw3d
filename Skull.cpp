#include "Skull.h"
#include "BindableBase.h"

Skull::Skull(D3DApp& d3dApp,
	std::mt19937& rng,
	std::uniform_real_distribution<float>& rdist,
	std::uniform_real_distribution<float>& adist,
	std::uniform_real_distribution<float>& ddist,
	std::uniform_real_distribution<float>& odist)
	: r(rdist(rng)),
	droll(ddist(rng)),
	dpitch(ddist(rng)),
	dyaw(ddist(rng)),
	dphi(odist(rng)),
	dtheta(odist(rng)),
	dchi(odist(rng)),
	chi(adist(rng)),
	theta(adist(rng)),
	phi(adist(rng))
{
	if (!IsStaticInitialized())
	{
		GeometryGenerator::MeshData mesh;
		GeometryGenerator gen;

		gen.CreateModel(mesh, "Models/Skull.txt");

		const std::vector<GeometryGenerator::Vertex> vertices = mesh.Vertices;
		AddStaticBind(std::make_unique<VertexBuffer>(d3dApp, vertices));

		auto pvs = std::make_unique<VertexShader>(d3dApp, L"VertexShader.cso");
		auto pvsbc = pvs->GetBytecode();
		AddStaticBind(std::move(pvs));

		AddStaticBind(std::make_unique<PixelShader>(d3dApp, L"PixelShader.cso"));

		const std::vector<UINT> indices = mesh.Indices;
		AddStaticIndexBuffer(std::make_unique<IndexBuffer>(d3dApp, indices));

		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{ "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
			{ "COLOR",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0 },
		};
		AddStaticBind(std::make_unique<InputLayout>(d3dApp, ied, pvsbc));

		AddStaticBind(std::make_unique<Topology>(d3dApp, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	}
	else
	{
		SetIndexFromStatic();
	}

	AddBind(std::make_unique<TransformCbuf>(d3dApp, *this));
}

void Skull::Update(float dt) noexcept
{
	roll += droll * dt;
	pitch += dpitch * dt;
	yaw += dyaw * dt;
	theta += dtheta * dt;
	phi += dphi * dt;
	chi += dchi * dt;
}

DirectX::XMMATRIX Skull::GetTransformXM() const noexcept
{
	return DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll) *
		DirectX::XMMatrixTranslation(r, 0.0f, 0.0f) *
		DirectX::XMMatrixRotationRollPitchYaw(theta, phi, chi);
}
