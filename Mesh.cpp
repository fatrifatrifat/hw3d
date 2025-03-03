#include "Mesh.h"
#include "imgui/imgui.h"
#include "Surface.h"
#include <unordered_map>
#include <sstream>

namespace dx = DirectX;

// Mesh
Mesh::Mesh(D3DApp& d3dApp, std::vector<std::shared_ptr<Bind::Bindable>> bindPtrs)
{
	AddBind(Bind::Topology::Resolve(d3dApp, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	for (auto& pb : bindPtrs)
	{
		AddBind(std::move(pb));
	}

	AddBind(std::make_shared<Bind::TransformCbuf>(d3dApp, *this));
}
void Mesh::Draw(D3DApp& d3dApp, DirectX::FXMMATRIX accumulatedTransform) const
{
	DirectX::XMStoreFloat4x4(&transform, accumulatedTransform);
	Drawable::Draw(d3dApp);
}
DirectX::XMMATRIX Mesh::GetTransformXM() const noexcept
{
	return DirectX::XMLoadFloat4x4(&transform);
}


// Node
Node::Node(int id, const std::string& name, std::vector<Mesh*> meshPtrs, const DirectX::XMMATRIX& transform_in)
	:
id(id),
meshPtrs(std::move(meshPtrs)),
name(name)
{
	dx::XMStoreFloat4x4(&transform, transform_in);
	dx::XMStoreFloat4x4(&appliedTransform, dx::XMMatrixIdentity());
}

void Node::Draw(D3DApp& d3dApp, DirectX::FXMMATRIX accumulatedTransform) const
{
	const auto built =
		dx::XMLoadFloat4x4(&appliedTransform) *
		dx::XMLoadFloat4x4(&transform) *
		accumulatedTransform;
	for (const auto pm : meshPtrs)
	{
		pm->Draw(d3dApp, built);
	}
	for (const auto& pc : childPtrs)
	{
		pc->Draw(d3dApp, built);
	}
}

void Node::AddChild(std::unique_ptr<Node> pChild)
{
	assert(pChild);
	childPtrs.push_back(std::move(pChild));
}

void Node::ShowTree(Node*& pSelectedNode) const noexcept
{
	// if there is no selected node, set selectedId to an impossible value
	const int selectedId = (pSelectedNode == nullptr) ? -1 : pSelectedNode->GetId();
	// build up flags for current node
	const auto node_flags = ImGuiTreeNodeFlags_OpenOnArrow
		| ((GetId() == selectedId) ? ImGuiTreeNodeFlags_Selected : 0)
		| ((childPtrs.size() == 0) ? ImGuiTreeNodeFlags_Leaf : 0);
	// render this node
	const auto expanded = ImGui::TreeNodeEx(
		(void*)(intptr_t)GetId(), node_flags, name.c_str()
	);
	// processing for selecting node
	if (ImGui::IsItemClicked())
	{
		pSelectedNode = const_cast<Node*>(this);
	}
	// recursive rendering of open node's children
	if (expanded)
	{
		for (const auto& pChild : childPtrs)
		{
			pChild->ShowTree(pSelectedNode);
		}
		ImGui::TreePop();
	}
}

void Node::SetAppliedTransform(DirectX::FXMMATRIX transform) noexcept
{
	dx::XMStoreFloat4x4(&appliedTransform, transform);
}

int Node::GetId() const noexcept
{
	return id;
}


// Model
class ModelWindow // pImpl idiom, only defined in this .cpp
{
public:
	void Show(D3DApp& d3dApp, const char* windowName, const Node& root) noexcept
	{
		// window name defaults to "Model"
		windowName = windowName ? windowName : "Model";
		// need an ints to track node indices and selected node
		int nodeIndexTracker = 0;
		if (ImGui::Begin(windowName))
		{
			ImGui::Columns(2, nullptr, true);
			root.ShowTree(pSelectedNode);

			ImGui::NextColumn();
			if (pSelectedNode != nullptr)
			{
				auto& transform = transforms[pSelectedNode->GetId()];
				ImGui::Text("Orientation");
				ImGui::SliderAngle("Roll", &transform.roll, -180.0f, 180.0f);
				ImGui::SliderAngle("Pitch", &transform.pitch, -180.0f, 180.0f);
				ImGui::SliderAngle("Yaw", &transform.yaw, -180.0f, 180.0f);
				ImGui::Text("Position");
				ImGui::SliderFloat("X", &transform.x, -20.0f, 20.0f);
				ImGui::SliderFloat("Y", &transform.y, -20.0f, 20.0f);
				ImGui::SliderFloat("Z", &transform.z, -20.0f, 20.0f);

				if (!pSelectedNode->ControlMeDaddy(d3dApp, skinMaterial))
				{
					pSelectedNode->ControlMeDaddy(d3dApp, ringMaterial);
				}
			}
		}
		ImGui::End();
	}
	dx::XMMATRIX GetTransform() const noexcept
	{
		assert(pSelectedNode != nullptr);
		const auto& transform = transforms.at(pSelectedNode->GetId());
		return
			dx::XMMatrixRotationRollPitchYaw(transform.roll, transform.pitch, transform.yaw) *
			dx::XMMatrixTranslation(transform.x, transform.y, transform.z);
	}
	Node* GetSelectedNode() const noexcept
	{
		return pSelectedNode;
	}
private:
	Node* pSelectedNode;
	struct TransformParameters
	{
		float roll = 0.0f;
		float pitch = 0.0f;
		float yaw = 0.0f;
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;
	};
	Node::PSMaterialConstantFullmonte skinMaterial;
	Node::PSMaterialConstantNotex ringMaterial;
	std::unordered_map<int, TransformParameters> transforms;
};

Model::Model(D3DApp& d3dApp, const std::string fileName)
	:
	pWindow(std::make_unique<ModelWindow>())
{
	Assimp::Importer imp;
	const auto pScene = imp.ReadFile(fileName.c_str(),
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_ConvertToLeftHanded |
		aiProcess_GenNormals |
		aiProcess_CalcTangentSpace
	);

	for (size_t i = 0; i < pScene->mNumMeshes; i++)
	{
		meshPtrs.push_back(ParseMesh(d3dApp, *pScene->mMeshes[i], pScene->mMaterials));
	}

	int nextId = 0;
	pRoot = ParseNode(nextId, *pScene->mRootNode);
}

void Model::Draw(D3DApp& d3dApp) const
{
	if (auto node = pWindow->GetSelectedNode())
	{
		node->SetAppliedTransform(pWindow->GetTransform());
	}
	pRoot->Draw(d3dApp, dx::XMMatrixIdentity());
}

void Model::ShowWindow(D3DApp& d3dApp, const char* windowName) noexcept
{
	pWindow->Show(d3dApp, windowName, *pRoot);
}

void Model::SetRootTransform(DirectX::FXMMATRIX tf) noexcept
{
	pRoot->SetAppliedTransform(tf);
}

Model::~Model() noexcept
{}

std::unique_ptr<Mesh> Model::ParseMesh(D3DApp& d3dApp, const aiMesh& mesh, const aiMaterial* const* pMaterials)
{
	using namespace std::string_literals;
	using Dvtx::VertexLayout;
	using namespace Bind;

	std::vector<std::shared_ptr<Bind::Bindable>> bindablePtrs;

	const auto base = "Models\\gobber\\"s;

	bool hasSpecularMap = false;
	bool hasAlphaGloss = false;
	bool hasNormalMap = false;
	bool hasDiffuseMap = false;
	float shininess = 2.0f;
	dx::XMFLOAT4 specularColor = { 0.18f,0.18f,0.18f,1.0f };
	dx::XMFLOAT4 diffuseColor = { 0.45f,0.45f,0.85f,1.0f };
	if (mesh.mMaterialIndex >= 0)
	{
		auto& material = *pMaterials[mesh.mMaterialIndex];

		aiString texFileName;

		if (material.GetTexture(aiTextureType_DIFFUSE, 0, &texFileName) == aiReturn_SUCCESS)
		{
			bindablePtrs.push_back(Texture::Resolve(d3dApp, base + texFileName.C_Str()));
			hasDiffuseMap = true;
		}
		else
		{
			material.Get(AI_MATKEY_COLOR_DIFFUSE, reinterpret_cast<aiColor3D&>(diffuseColor));
		}

		if (material.GetTexture(aiTextureType_SPECULAR, 0, &texFileName) == aiReturn_SUCCESS)
		{
			auto tex = Texture::Resolve(d3dApp, base + texFileName.C_Str(), 1);
			hasAlphaGloss = tex->HasAlpha();
			bindablePtrs.push_back(std::move(tex));
			hasSpecularMap = true;
		}
		else
		{
			material.Get(AI_MATKEY_COLOR_SPECULAR, reinterpret_cast<aiColor3D&>(specularColor));
		}
		if (!hasAlphaGloss)
		{
			material.Get(AI_MATKEY_SHININESS, shininess);
		}

		if (material.GetTexture(aiTextureType_NORMALS, 0, &texFileName) == aiReturn_SUCCESS)
		{
			auto tex = Texture::Resolve(d3dApp, base + texFileName.C_Str(), 2);
			hasAlphaGloss = tex->HasAlpha();
			bindablePtrs.push_back(std::move(tex));
			hasNormalMap = true;
		}

		if (hasDiffuseMap || hasSpecularMap || hasNormalMap)
		{
			bindablePtrs.push_back(Bind::Sampler::Resolve(d3dApp));
		}
	}

	const auto meshTag = base + "%" + mesh.mName.C_Str();

	const float scale = 6.0f;

	if (hasDiffuseMap && hasNormalMap && hasSpecularMap)
	{
		Dvtx::VertexBuffer vbuf(std::move(
			VertexLayout{}
			.Append(VertexLayout::Position3D)
			.Append(VertexLayout::Normal)
			.Append(VertexLayout::Tangent)
			.Append(VertexLayout::Bitangent)
			.Append(VertexLayout::Texture2D)
		));

		for (unsigned int i = 0; i < mesh.mNumVertices; i++)
		{
			vbuf.EmplaceBack(
				dx::XMFLOAT3(mesh.mVertices[i].x * scale, mesh.mVertices[i].y * scale, mesh.mVertices[i].z * scale),
				*reinterpret_cast<dx::XMFLOAT3*>(&mesh.mNormals[i]),
				*reinterpret_cast<dx::XMFLOAT3*>(&mesh.mTangents[i]),
				*reinterpret_cast<dx::XMFLOAT3*>(&mesh.mBitangents[i]),
				*reinterpret_cast<dx::XMFLOAT2*>(&mesh.mTextureCoords[0][i])
			);
		}

		std::vector<unsigned int> indices;
		indices.reserve(mesh.mNumFaces * 3);
		for (unsigned int i = 0; i < mesh.mNumFaces; i++)
		{
			const auto& face = mesh.mFaces[i];
			assert(face.mNumIndices == 3);
			indices.push_back(face.mIndices[0]);
			indices.push_back(face.mIndices[1]);
			indices.push_back(face.mIndices[2]);
		}

		bindablePtrs.push_back(VertexBuffer::Resolve(d3dApp, meshTag, vbuf));

		bindablePtrs.push_back(IndexBuffer::Resolve(d3dApp, meshTag, indices));

		auto pvs = VertexShader::Resolve(d3dApp, "PhongVSNormalMap.cso");
		auto pvsbc = pvs->GetBytecode();
		bindablePtrs.push_back(std::move(pvs));

		bindablePtrs.push_back(PixelShader::Resolve(d3dApp, "PhongPSSpecNormalMap.cso"));

		bindablePtrs.push_back(InputLayout::Resolve(d3dApp, vbuf.GetLayout(), pvsbc));

		Node::PSMaterialConstantFullmonte pmc;
		pmc.specularPower = shininess;
		pmc.hasGlossMap = hasAlphaGloss ? TRUE : FALSE;
		// this is CLEARLY an issue... all meshes will share same mat const, but may have different
		// Ns (specular power) specified for each in the material properties... bad conflict
		bindablePtrs.push_back(PixelConstantBuffer<Node::PSMaterialConstantFullmonte>::Resolve(d3dApp, pmc, 1u));
	}
	else if (hasDiffuseMap && hasNormalMap)
	{
		Dvtx::VertexBuffer vbuf(std::move(
			VertexLayout{}
			.Append(VertexLayout::Position3D)
			.Append(VertexLayout::Normal)
			.Append(VertexLayout::Tangent)
			.Append(VertexLayout::Bitangent)
			.Append(VertexLayout::Texture2D)
		));

		for (unsigned int i = 0; i < mesh.mNumVertices; i++)
		{
			vbuf.EmplaceBack(
				dx::XMFLOAT3(mesh.mVertices[i].x * scale, mesh.mVertices[i].y * scale, mesh.mVertices[i].z * scale),
				*reinterpret_cast<dx::XMFLOAT3*>(&mesh.mNormals[i]),
				*reinterpret_cast<dx::XMFLOAT3*>(&mesh.mTangents[i]),
				*reinterpret_cast<dx::XMFLOAT3*>(&mesh.mBitangents[i]),
				*reinterpret_cast<dx::XMFLOAT2*>(&mesh.mTextureCoords[0][i])
			);
		}

		std::vector<unsigned int> indices;
		indices.reserve(mesh.mNumFaces * 3);
		for (unsigned int i = 0; i < mesh.mNumFaces; i++)
		{
			const auto& face = mesh.mFaces[i];
			assert(face.mNumIndices == 3);
			indices.push_back(face.mIndices[0]);
			indices.push_back(face.mIndices[1]);
			indices.push_back(face.mIndices[2]);
		}

		bindablePtrs.push_back(VertexBuffer::Resolve(d3dApp, meshTag, vbuf));

		bindablePtrs.push_back(IndexBuffer::Resolve(d3dApp, meshTag, indices));

		auto pvs = VertexShader::Resolve(d3dApp, "PhongVSNormalMap.cso");
		auto pvsbc = pvs->GetBytecode();
		bindablePtrs.push_back(std::move(pvs));

		bindablePtrs.push_back(PixelShader::Resolve(d3dApp, "PhongPSNormalMap.cso"));

		bindablePtrs.push_back(InputLayout::Resolve(d3dApp, vbuf.GetLayout(), pvsbc));

		struct PSMaterialConstantDiffnorm
		{
			float specularIntensity;
			float specularPower;
			BOOL  normalMapEnabled = TRUE;
			float padding[1];
		} pmc;
		pmc.specularPower = shininess;
		pmc.specularIntensity = (specularColor.x + specularColor.y + specularColor.z) / 3.0f;
		// this is CLEARLY an issue... all meshes will share same mat const, but may have different
		// Ns (specular power) specified for each in the material properties... bad conflict
		bindablePtrs.push_back(PixelConstantBuffer<PSMaterialConstantDiffnorm>::Resolve(d3dApp, pmc, 1u));
	}
	else if (hasDiffuseMap)
	{
		Dvtx::VertexBuffer vbuf(std::move(
			VertexLayout{}
			.Append(VertexLayout::Position3D)
			.Append(VertexLayout::Normal)
			.Append(VertexLayout::Texture2D)
		));

		for (unsigned int i = 0; i < mesh.mNumVertices; i++)
		{
			vbuf.EmplaceBack(
				dx::XMFLOAT3(mesh.mVertices[i].x * scale, mesh.mVertices[i].y * scale, mesh.mVertices[i].z * scale),
				*reinterpret_cast<dx::XMFLOAT3*>(&mesh.mNormals[i]),
				*reinterpret_cast<dx::XMFLOAT2*>(&mesh.mTextureCoords[0][i])
			);
		}

		std::vector<unsigned int> indices;
		indices.reserve(mesh.mNumFaces * 3);
		for (unsigned int i = 0; i < mesh.mNumFaces; i++)
		{
			const auto& face = mesh.mFaces[i];
			assert(face.mNumIndices == 3);
			indices.push_back(face.mIndices[0]);
			indices.push_back(face.mIndices[1]);
			indices.push_back(face.mIndices[2]);
		}

		bindablePtrs.push_back(VertexBuffer::Resolve(d3dApp, meshTag, vbuf));

		bindablePtrs.push_back(IndexBuffer::Resolve(d3dApp, meshTag, indices));

		auto pvs = VertexShader::Resolve(d3dApp, "PhongVS.cso");
		auto pvsbc = pvs->GetBytecode();
		bindablePtrs.push_back(std::move(pvs));

		bindablePtrs.push_back(PixelShader::Resolve(d3dApp, "PhongPS.cso"));

		bindablePtrs.push_back(InputLayout::Resolve(d3dApp, vbuf.GetLayout(), pvsbc));

		struct PSMaterialConstantDiffuse
		{
			float specularIntensity;
			float specularPower;
			float padding[2];
		} pmc;
		pmc.specularPower = shininess;
		pmc.specularIntensity = (specularColor.x + specularColor.y + specularColor.z) / 3.0f;
		// this is CLEARLY an issue... all meshes will share same mat const, but may have different
		// Ns (specular power) specified for each in the material properties... bad conflict
		bindablePtrs.push_back(PixelConstantBuffer<PSMaterialConstantDiffuse>::Resolve(d3dApp, pmc, 1u));
	}
	else if (!hasDiffuseMap && !hasNormalMap && !hasSpecularMap)
	{
		Dvtx::VertexBuffer vbuf(std::move(
			VertexLayout{}
			.Append(VertexLayout::Position3D)
			.Append(VertexLayout::Normal)
		));

		for (unsigned int i = 0; i < mesh.mNumVertices; i++)
		{
			vbuf.EmplaceBack(
				dx::XMFLOAT3(mesh.mVertices[i].x * scale, mesh.mVertices[i].y * scale, mesh.mVertices[i].z * scale),
				*reinterpret_cast<dx::XMFLOAT3*>(&mesh.mNormals[i])
			);
		}

		std::vector<unsigned int> indices;
		indices.reserve(mesh.mNumFaces * 3);
		for (unsigned int i = 0; i < mesh.mNumFaces; i++)
		{
			const auto& face = mesh.mFaces[i];
			assert(face.mNumIndices == 3);
			indices.push_back(face.mIndices[0]);
			indices.push_back(face.mIndices[1]);
			indices.push_back(face.mIndices[2]);
		}

		bindablePtrs.push_back(VertexBuffer::Resolve(d3dApp, meshTag, vbuf));

		bindablePtrs.push_back(IndexBuffer::Resolve(d3dApp, meshTag, indices));

		auto pvs = VertexShader::Resolve(d3dApp, "PhongVSNotex.cso");
		auto pvsbc = pvs->GetBytecode();
		bindablePtrs.push_back(std::move(pvs));

		bindablePtrs.push_back(PixelShader::Resolve(d3dApp, "PhongPSNotex.cso"));

		bindablePtrs.push_back(InputLayout::Resolve(d3dApp, vbuf.GetLayout(), pvsbc));

		Node::PSMaterialConstantNotex pmc;
		pmc.specularPower = shininess;
		pmc.specularColor = specularColor;
		pmc.materialColor = diffuseColor;
		// this is CLEARLY an issue... all meshes will share same mat const, but may have different
		// Ns (specular power) specified for each in the material properties... bad conflict
		bindablePtrs.push_back(PixelConstantBuffer<Node::PSMaterialConstantNotex>::Resolve(d3dApp, pmc, 1u));
	}
	else
	{
		throw std::runtime_error("terrible combination of textures in material smh");
	}

	return std::make_unique<Mesh>(d3dApp, std::move(bindablePtrs));
}

std::unique_ptr<Node> Model::ParseNode(int& nextId, const aiNode& node) noexcept
{
	namespace dx = DirectX;
	const auto transform = dx::XMMatrixTranspose(dx::XMLoadFloat4x4(
		reinterpret_cast<const dx::XMFLOAT4X4*>(&node.mTransformation)
	));

	std::vector<Mesh*> curMeshPtrs;
	curMeshPtrs.reserve(node.mNumMeshes);
	for (size_t i = 0; i < node.mNumMeshes; i++)
	{
		const auto meshIdx = node.mMeshes[i];
		curMeshPtrs.push_back(meshPtrs.at(meshIdx).get());
	}

	auto pNode = std::make_unique<Node>(nextId++, node.mName.C_Str(), std::move(curMeshPtrs), transform);
	for (size_t i = 0; i < node.mNumChildren; i++)
	{
		pNode->AddChild(ParseNode(nextId, *node.mChildren[i]));
	}

	return pNode;
}