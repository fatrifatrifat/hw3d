#pragma once
#include "DrawableCommon.h"
#include "BindableBase.h"
#include "Vertex.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Mesh : public DrawableBase<Mesh>
{
public:
	Mesh(D3DApp& d3dApp, std::vector<std::unique_ptr<Bind::Bindable>> bindPtrs);
	void Draw(D3DApp& d3dApp, DirectX::FXMMATRIX accumulatedTransform) const;
	DirectX::XMMATRIX GetTransformXM() const noexcept override;
private:
	mutable DirectX::XMFLOAT4X4 transform;
};

class Node
{
	friend class Model;
public:
	Node(std::vector<Mesh*> meshPtrs, const DirectX::XMMATRIX& transform);
	void Draw(D3DApp& d3dApp, DirectX::FXMMATRIX accumulatedTransform) const;
private:
	void AddChild(std::unique_ptr<Node> pChild);
private:
	std::vector<std::unique_ptr<Node>> childPtrs;
	std::vector<Mesh*> meshPtrs;
	DirectX::XMFLOAT4X4 transform;
};

class Model
{
public:
	Model(D3DApp& d3dApp, const std::string fileName);
	void Draw(D3DApp& d3dApp, DirectX::FXMMATRIX transform) const;
private:
	static std::unique_ptr<Mesh> ParseMesh(D3DApp& d3dApp, const aiMesh& mesh);
	std::unique_ptr<Node> ParseNode(const aiNode& node);
private:
	std::unique_ptr<Node> pRoot;
	std::vector<std::unique_ptr<Mesh>> meshPtrs;
};