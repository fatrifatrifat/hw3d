#include "GeometryGenerator.h"
#include <cmath>

void GeometryGenerator::CreateBox(float width, float height, float depth, MeshData& meshData)
{
    // Clear the mesh data.
    meshData.Vertices.clear();
    meshData.Indices.clear();

    Vertex v[8];

    float w2 = 0.5f * width;
    float h2 = 0.5f * height;
    float d2 = 0.5f * depth;

    // Front face
    
    meshData.Vertices.emplace_back(XMFLOAT3(-w2, +h2, -d2), Colors::red); // Red
    meshData.Vertices.emplace_back(XMFLOAT3(-w2, -h2, -d2), Colors::green); // Green
    meshData.Vertices.emplace_back(XMFLOAT3(+w2, +h2, -d2), Colors::blue); // Blue
    meshData.Vertices.emplace_back(XMFLOAT3(+w2, -h2, -d2), Colors::yellow); // Yellow

    // Back face
    meshData.Vertices.emplace_back(XMFLOAT3(-w2, -h2, +d2), Colors::magenta); // Magenta
    meshData.Vertices.emplace_back(XMFLOAT3(-w2, +h2, +d2), Colors::cyan); // Cyan
    meshData.Vertices.emplace_back(XMFLOAT3(+w2, +h2, +d2), Colors::white); // White
    meshData.Vertices.emplace_back(XMFLOAT3(+w2, -h2, +d2), Colors::gray); // Gray

    unsigned short i[36] = {
        // Front face
        0, 2, 1, 2, 3, 1,
        // Back face
        5, 4, 6, 6, 4, 7,
        // Left face
        5, 0, 4, 4, 0, 1,
        // Right face
        2, 6, 3, 3, 6, 7,
        // Top face
        5, 2, 0, 5, 6, 2,
        // Bottom face
        1, 3, 4, 4, 3, 7
    };

    for (int j = 0; j < 36; j++)
    {
        meshData.Indices.push_back(i[j]);
    }

}

void GeometryGenerator::CreateSphere(float radius, UINT sliceCount, UINT stackCount, MeshData& meshData)
{
    // Clear mesh data.
    meshData.Vertices.clear();
    meshData.Indices.clear();

    // Compute vertices.
    Vertex topVertex(0.0f, +radius, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
    Vertex bottomVertex(0.0f, -radius, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);

    meshData.Vertices.push_back(topVertex);

    float phiStep = XM_PI / stackCount;
    float thetaStep = 2.0f * XM_PI / sliceCount;

    for (UINT i = 1; i <= stackCount - 1; ++i)
    {
        float phi = i * phiStep;

        for (UINT j = 0; j <= sliceCount; ++j)
        {
            float theta = j * thetaStep;

            Vertex v;

            v.position.x = radius * sinf(phi) * cosf(theta);
            v.position.y = radius * cosf(phi);
            v.position.z = radius * sinf(phi) * sinf(theta);

            v.color = XMFLOAT4(
                v.position.x / radius + 0.5f,
                v.position.y / radius + 0.5f,
                v.position.z / radius + 0.5f,
                1.0f);

            meshData.Vertices.push_back(v);
        }
    }

    meshData.Vertices.push_back(bottomVertex);

    // Compute indices for top stack.
    for (UINT i = 1; i <= sliceCount; ++i)
    {
        meshData.Indices.push_back(0);
        meshData.Indices.push_back(i + 1);
        meshData.Indices.push_back(i);
    }

    // Compute indices for inner stacks.
    UINT baseIndex = 1;
    UINT ringVertexCount = sliceCount + 1;

    for (UINT i = 0; i < stackCount - 2; ++i)
    {
        for (UINT j = 0; j < sliceCount; ++j)
        {
            meshData.Indices.push_back(baseIndex + i * ringVertexCount + j);
            meshData.Indices.push_back(baseIndex + i * ringVertexCount + j + 1);
            meshData.Indices.push_back(baseIndex + (i + 1) * ringVertexCount + j);

            meshData.Indices.push_back(baseIndex + (i + 1) * ringVertexCount + j);
            meshData.Indices.push_back(baseIndex + i * ringVertexCount + j + 1);
            meshData.Indices.push_back(baseIndex + (i + 1) * ringVertexCount + j + 1);
        }
    }

    // Compute indices for bottom stack.
    UINT southPoleIndex = (UINT)meshData.Vertices.size() - 1;

    baseIndex = southPoleIndex - ringVertexCount;

    for (UINT i = 0; i < sliceCount; ++i)
    {
        meshData.Indices.push_back(southPoleIndex);
        meshData.Indices.push_back(baseIndex + i);
        meshData.Indices.push_back(baseIndex + i + 1);
    }
}

void GeometryGenerator::CreateGrid(float width, float depth, UINT m, UINT n, MeshData& meshData)
{
    meshData.Vertices.clear();
    meshData.Indices.clear();

    UINT vertexCount = m * n;
    UINT faceCount = (m - 1) * (n - 1) * 2;

    float halfWidth = 0.5f * width;
    float halfDepth = 0.5f * depth;

    float dx = width / (n - 1);
    float dz = depth / (m - 1);

    float du = 1.0f / (n - 1);
    float dv = 1.0f / (m - 1);

    for (UINT i = 0; i < m; ++i)
    {
        float z = halfDepth - i * dz;
        for (UINT j = 0; j < n; ++j)
        {
            float x = -halfWidth + j * dx;

            meshData.Vertices.push_back(Vertex(x, 0.0f, z, 0.0f, 1.0f, 0.0f, 1.0f));
        }
    }

    for (UINT i = 0; i < m - 1; ++i)
    {
        for (UINT j = 0; j < n - 1; ++j)
        {
            meshData.Indices.push_back(i * n + j);
            meshData.Indices.push_back(i * n + j + 1);
            meshData.Indices.push_back((i + 1) * n + j);

            meshData.Indices.push_back((i + 1) * n + j);
            meshData.Indices.push_back(i * n + j + 1);
            meshData.Indices.push_back((i + 1) * n + j + 1);
        }
    }
}
