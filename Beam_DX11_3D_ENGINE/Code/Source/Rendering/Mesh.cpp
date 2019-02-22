#pragma once
#include "../../Include/Rendering/Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/vector3.h>
#include <assimp/cimport.h>

#pragma comment (lib, "assimp-vc140-mtd.lib")
#pragma comment (lib, "assimp-vc140-mt.lib")

//-------------------------------------TEMPLATES---------------------------------------------------

template <typename TComponentType> static auto const createVertexComponentBuffer = []
	(
	ID3D11Device *device, std::vector<TComponentType> const &data, ID3D11Buffer **outBuffer
	) -> bool {

	D3D11_BUFFER_DESC vertexBufferDesc{ };
	vertexBufferDesc.ByteWidth = data.size() * sizeof(TComponentType);
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vertexData{ };
	vertexData.pSysMem = data.data();
	vertexData.SysMemPitch = sizeof(TComponentType);
	vertexData.SysMemSlicePitch = 0;

	device->CreateBuffer(&vertexBufferDesc, &vertexData, outBuffer);
	return true;
};

template <typename TVector>
struct Converter
{
	static TVector convert(const aiVector3D& in, const float w = 0.0f, const float factor = 1.0f);
};


template <>
struct Converter<XMVECTOR>
{
	static XMVECTOR convert(const aiVector3D& in, const float w = 0.0f, const float factor = 1.0f)
	{
		return XMVECTOR({ in.x * factor, in.y * factor, in.z * factor, w });
	}
};

template <>
struct Converter<XMFLOAT3>
{
	static XMFLOAT3 convert(const aiVector3D& in, const float factor = 1.0f)
	{
		return XMFLOAT3({ in.x * factor, in.y * factor, in.z * factor });
	}
};



//-------------------------------------CMESH-CODE--------------------------------------------------

CMesh::CMesh(MeshData const &aMeshData) : mMeshData(aMeshData) { }

//-------------------------------------CREATE-TRIANGLE---------------------------------------------


std::shared_ptr<CMesh> CMesh::CreateTriangle(std::shared_ptr<CDXIntegration> directX)
{
	Vertices vertices =
	{
		{ { +.5, -.5, +.5, 1.0 }, { -.5, -.5, +.5, 1.0 }, { 0.0, +.5f, +.5, 1.0 } },
		{ { 0.0, 0.0, -1.0     }, { 0.0, 0.0, -1.0     }, { 0.0, 0.0, -1.0      } },
		{ { 0.0, 1.0,  0.0     }, { 0.0, 1.0,  0.0     }, { 0.0, 1.0,  0.0      } },
		{ { 1.0, 1.0, 0.0, 0.0 }, { 0.0, 1.0, 0.0, 0.0 }, { +.5, 0.0,  0.0, 0.0 } },
	};

	std::vector<uint32_t> indices =
	{
		0, 1, 2
	};

	return CMesh::CreateMesh(directX, vertices, indices);
}


//-------------------------------------CREATE-QUAD-------------------------------------------------

std::shared_ptr<CMesh> CMesh::CreateQuad(std::shared_ptr<CDXIntegration> aDirectX, uint32_t const & aScale)
{
	Vertices vertices =
	{
		{
			{ -.5, -.5, +.5, 1.0 }, 
			{ +.5, -.5, +.5, 1.0 }, 
			{ +.5, +.5, +.5, 1.0 }, 
			{ -.5, +.5, +.5, 1.0 }  
		},
		{
			{ 0.0, 0.0, -1.0 },
			{ 0.0, 0.0, -1.0 },
			{ 0.0, 0.0, -1.0 },
			{ 0.0, 0.0, -1.0 }
		},
		{
			{ 0.0, 1.0,  0.0 },
			{ 0.0, 1.0,  0.0 },
			{ 0.0, 1.0,  0.0 },
			{ 0.0, 1.0,  0.0 }
		},
		{
			{ 0.0, 1.0, 0.0, 0.0 },
			{ 1.0, 1.0, 0.0, 0.0 },
			{ 1.0, 0.0, 0.0, 0.0 },
			{ 0.0, 0.0, 0.0, 0.0 }
		}
	};

	std::vector<uint32_t> indices =
	{
		0, 2, 1,
		2, 0, 3
	};

	return CMesh::CreateMesh(aDirectX, vertices, indices);
}

//-------------------------------------CREATE-MESH-FROM-FILE---------------------------------------

std::shared_ptr<CMesh> CMesh::CreateMeshFromFile(std::shared_ptr<CDXIntegration> aDirectX, std::string const & aFilename)
{
	unsigned int flags = 0
		| aiProcess_GenUVCoords
		| aiProcess_GenNormals
		| aiProcess_CalcTangentSpace
		| aiProcess_Triangulate
	 // | aiProcess_FlipWindingOrder
		;

	Assimp::Importer importer;

	const aiScene*pScene = importer.ReadFile(aFilename, flags);

	if (not pScene)
	{
		const std::string err = importer.GetErrorString();
		return false;
	}

	pScene = importer.ApplyPostProcessing(flags);

	if (not pScene->HasMeshes())
	{
		return false;
	}

	unsigned int totalVertexCount = 0;
	unsigned int totalFaceCount = 0;
	unsigned int totalIndexCount = 0;

	for (unsigned int k = 0; k < pScene->mNumMeshes; ++k)
	{
		aiMesh *pMesh = pScene->mMeshes[k];

		totalVertexCount += pMesh->mNumVertices;
		totalFaceCount += pMesh->mNumFaces;
		totalIndexCount += pMesh->mNumFaces * 3;
	}

	Vertices vertices;
	vertices.positions.resize(totalVertexCount);
	vertices.normals.resize(totalVertexCount);
	vertices.tangents.resize(totalVertexCount);
	vertices.texpos.resize(totalVertexCount);

	std::vector<uint32_t> indices;
	indices.resize(totalIndexCount);

	uint64_t currentVertexOffset = 0;
	uint64_t currentIndexOffset = 0;

	uint32_t lastIndex = 0;

	for (unsigned int k = 0; k < pScene->mNumMeshes; ++k)
	{
		lastIndex = k;
		aiMesh *pMesh = pScene->mMeshes[k];

		for (unsigned int v = 0; v < pMesh->mNumVertices; ++v)
		{
			vertices.positions[currentVertexOffset + v] = Converter<XMVECTOR>::convert(pMesh->mVertices[v], 1.0f);
			vertices.normals[currentVertexOffset + v] = Converter<XMFLOAT3>::convert(pMesh->mNormals[v]);
			vertices.tangents[currentVertexOffset + v] = Converter<XMFLOAT3>::convert(pMesh->mTangents[v]);
			vertices.texpos[currentVertexOffset + v] = Converter<XMVECTOR>::convert((pMesh->mTextureCoords[0][v]));
		}

		for (unsigned int j = 0; j < pMesh->mNumFaces; ++j)
		{
			aiFace *pFace = pMesh->mFaces + j;
			for (unsigned int i = 0; i < pFace->mNumIndices; ++i)
			{
				indices[currentIndexOffset + ((3 * j) + i)] = (currentVertexOffset + *(pFace->mIndices + i));
			}
		}

		currentVertexOffset += pMesh->mNumVertices;
		currentIndexOffset += pMesh->mNumFaces * 3;
	}

	return CMesh::CreateMesh(aDirectX, vertices, indices);
}

//-------------------------------------CREATE-MESH---------------------------------------------

std::shared_ptr<CMesh> CMesh::CreateMesh(std::shared_ptr<CDXIntegration> aDirectX, Vertices const & aVertice, std::vector<uint32_t> const & aIndice)
{
	ID3D11Device *device = aDirectX->myDevice();

	ID3D11Buffer *vertexPositionBuffer = nullptr;
	ID3D11Buffer *vertexNormalBuffer = nullptr;
	ID3D11Buffer *vertexTangentBuffer = nullptr;
	ID3D11Buffer *vertexTexPosBuffer = nullptr;
	ID3D11Buffer *indexBuffer = nullptr;

	createVertexComponentBuffer<VertexPosition>(device, aVertice.positions, &vertexPositionBuffer);
	createVertexComponentBuffer<VertexNormal>(device, aVertice.normals, &vertexNormalBuffer);
	createVertexComponentBuffer<VertexTangent>(device, aVertice.tangents, &vertexTangentBuffer);
	createVertexComponentBuffer<VertexTexPos>(device, aVertice.texpos, &vertexTexPosBuffer);

	D3D11_BUFFER_DESC indexBufferDesc{ };
	indexBufferDesc.ByteWidth = aIndice.size() * sizeof(uint32_t);
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA indexData{ };
	indexData.pSysMem = aIndice.data();
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	device->CreateBuffer(&indexBufferDesc, &indexData, &indexBuffer);

	CMesh::MeshData data{ };
	data.vertexBuffers =
	{
		vertexPositionBuffer,
		vertexNormalBuffer,
		vertexTangentBuffer,
		vertexTexPosBuffer
	};
	data.indexBuffer = indexBuffer;
	data.indexCount = aIndice.size();

	CMesh *temp = new CMesh(data);

	std::shared_ptr<CMesh> mesh = std::shared_ptr<CMesh>(temp);
	return mesh;
}

//-------------------------------------CMESH-CODE-END----------------------------------------------
