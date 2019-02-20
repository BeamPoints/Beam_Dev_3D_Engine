#pragma once
#include "../DirectX/DXIntegration.h"
#include <DirectXMath.h>

class CMesh
{public:

	using VertexPosition = XMVECTOR;
	using VertexNormal = XMFLOAT3;
	using VertexTangent = XMFLOAT3;
	using VertexTexPos = XMVECTOR;

	struct Vertices
	{
		std::vector<VertexPosition> positions;
		std::vector<VertexNormal> normals;
		std::vector<VertexTangent> tangents;
		std::vector<VertexTexPos> texpos;
	};

	struct MeshData
	{
		std::vector<ID3D11Buffer*> vertexBuffers;
		ID3D11Buffer              *indexBuffer;
		uint32_t                   indexCount;
	};

	static std::shared_ptr<CMesh> CreateTriangle(std::shared_ptr<CDXIntegration> directX);

	static std::shared_ptr<CMesh> CreateQuad(std::shared_ptr<CDXIntegration> aDirectX, uint32_t const &aScale = 1.0);
	static std::shared_ptr<CMesh> CreateMeshFromFile(std::shared_ptr<CDXIntegration> aDirectX,std::string const &aFilename);

	MeshData const &getMeshData() const { return mMeshData; }

private:
	CMesh(MeshData const &aMeshData);

	static std::shared_ptr<CMesh> CreateMesh(std::shared_ptr<CDXIntegration> aDirectX, Vertices const &aVertice, std::vector<uint32_t> const &aIndice);

	MeshData mMeshData;
};

