#include "SolidSphere.h"
#include "BindableCommon.h"
#include "Vertex.h"
#include "Sphere.h"


SolidSphere::SolidSphere(D3DApp& d3dApp, float radius)
{
	using namespace Bind;
	namespace dx = DirectX;

	auto model = Sphere::Make();
	model.Transform(dx::XMMatrixScaling(radius, radius, radius));
	const auto geometryTag = "$sphere." + std::to_string(radius);
	AddBind(VertexBuffer::Resolve(d3dApp, geometryTag, model.vertices));
	AddBind(IndexBuffer::Resolve(d3dApp, geometryTag, model.indices));

	auto pvs = VertexShader::Resolve(d3dApp, "SolidVS.cso");
	auto pvsbc = pvs->GetBytecode();
	AddBind(std::move(pvs));

	AddBind(PixelShader::Resolve(d3dApp, "SolidPS.cso"));

	struct PSColorConstant
	{
		dx::XMFLOAT3 color = { 1.0f,1.0f,1.0f };
		float padding;
	} colorConst;
	AddBind(PixelConstantBuffer<PSColorConstant>::Resolve(d3dApp, colorConst, 1u));

	AddBind(InputLayout::Resolve(d3dApp, model.vertices.GetLayout(), pvsbc));

	AddBind(Topology::Resolve(d3dApp, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	AddBind(std::make_shared<TransformCbuf>(d3dApp, *this));

	AddBind(Blender::Resolve(d3dApp, false));

	AddBind(Rasterizer::Resolve(d3dApp, false));
}

void SolidSphere::SetPos(DirectX::XMFLOAT3 pos) noexcept
{
	this->pos = pos;
}

DirectX::XMMATRIX SolidSphere::GetTransformXM() const noexcept
{
	return DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);
}