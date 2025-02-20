#include "Skull.h"
#include "BindableBase.h"

Skull::Skull(D3DApp& d3dApp,
	std::mt19937& rng,
	std::uniform_real_distribution<float>& adist,
	std::uniform_real_distribution<float>& ddist,
	std::uniform_real_distribution<float>& odist,
	std::uniform_real_distribution<float>& rdist,
	DirectX::XMFLOAT3 material)
	: TestObject(d3dApp, rng, adist, ddist, odist, rdist)
{
	if (!IsStaticInitialized())
	{
		GeometryGenerator::MeshData mesh;
		GeometryGenerator gen;

		gen.CreateModel(mesh, "Models/Skull.txt");

		const std::vector<GeometryGenerator::Vertex> vertices = mesh.Vertices;
		AddStaticBind(std::make_unique<VertexBuffer>(d3dApp, vertices));

		auto pvs = std::make_unique<VertexShader>(d3dApp, L"PhongVS.cso");
		auto pvsbc = pvs->GetBytecode();
		AddStaticBind(std::move(pvs));

		AddStaticBind(std::make_unique<PixelShader>(d3dApp, L"PhongPS.cso"));

		const std::vector<UINT> indices = mesh.Indices;
		AddStaticIndexBuffer(std::make_unique<IndexBuffer>(d3dApp, indices));

		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
			{ "Normal",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0 },
		};
		AddStaticBind(std::make_unique<InputLayout>(d3dApp, ied, pvsbc));

		AddStaticBind(std::make_unique<Topology>(d3dApp, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	}
	else
	{
		SetIndexFromStatic();
	}

	AddBind(std::make_unique<TransformCbuf>(d3dApp, *this));

	colorConst.material = material;
	AddBind(std::make_unique<PixelConstantBuffer<PSMaterialConstant>>(d3dApp, colorConst, 1u));


	DirectX::XMStoreFloat3x3(&mt, DirectX::XMMatrixScaling(0.7f, 0.7f, 0.7f));
}

DirectX::XMMATRIX Skull::GetTransformXM() const noexcept
{
	namespace dx = DirectX;
	return dx::XMLoadFloat3x3(&mt) * TestObject::GetTransformXM();
}
