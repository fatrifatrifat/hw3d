#include "GeometryGenerator.h"
#include <cmath>

void GeometryGenerator::CreateModel(MeshData& meshData, const std::string& fileName)
{
    std::ifstream fin(fileName);

    if (!fin)
    {
        MessageBox(0, L"Models/Skull.txt not found.", 0, 0);
        return;
    }

    UINT vcount = 0;
    UINT tcount = 0;
    std::string ignore;

    fin >> ignore >> vcount;
    fin >> ignore >> tcount;
    fin >> ignore >> ignore >> ignore >> ignore;

    float nx, ny, nz;
    XMFLOAT4 black(0.0f, 0.0f, 0.0f, 1.0f);

    meshData.Vertices.resize(vcount);
    for (UINT i = 0; i < vcount; ++i)
    {
        fin >> meshData.Vertices[i].pos.x >> meshData.Vertices[i].pos.y >> meshData.Vertices[i].pos.z;
        fin >> meshData.Vertices[i].normal.x >> meshData.Vertices[i].normal.y >> meshData.Vertices[i].normal.z;
    }

    fin >> ignore;
    fin >> ignore;
    fin >> ignore;

    UINT mSkullIndexCount = 3 * tcount;

    meshData.Indices.resize(mSkullIndexCount);
    for (UINT i = 0; i < tcount; ++i)
    {
        fin >> meshData.Indices[i * 3 + 0] >> meshData.Indices[i * 3 + 1] >> meshData.Indices[i * 3 + 2];
    }

    fin.close();
}

float GeometryGenerator::GetHeight(float x, float z) const
{
    return 0.3f * (z * sinf(0.1f * x) + x * cosf(0.1f * z));
}

