#include "..\..\Include\Engine\DefaultMaterial.h"

void CDefaultMaterial::setWorldMatrix(XMMATRIX aMatrix)
{
	mBufferData.world = aMatrix;
}

void CDefaultMaterial::setViewMatrix(XMMATRIX aMatrix)
{
	mBufferData.view = aMatrix;
}

void CDefaultMaterial::setProjectionMatrix(XMMATRIX aMatrix)
{
	mBufferData.projection = aMatrix;

}

void CDefaultMaterial::setTextures(ID3D11Device * aDevice, STextureCollection const & aTextures)
{
	mTextures = aTextures;

	if (not mTextures.diffuse.empty())
	{
		CTextur::Texture2DDescriptor diffuseTextureData{ };
		CTextur::LoadTextureFromFile(aTextures.diffuse.c_str(), diffuseTextureData);
		mDiffuseTexture
			= CDXCommon::CreateTexture2D(
				aDevice,
				diffuseTextureData.width,
				diffuseTextureData.height,
				diffuseTextureData.depth,
				DXGI_FORMAT_R8G8B8A8_UNORM,
				D3D11_BIND_SHADER_RESOURCE,
				diffuseTextureData.inData);
	}

	if (not mTextures.specular.empty())
	{
		CTextur::Texture2DDescriptor specularTextureData{ };
		CTextur::LoadTextureFromFile(mTextures.specular.c_str(), specularTextureData);
		mSpecularTexture
			= CDXCommon::CreateTexture2D(
				aDevice,
				specularTextureData.width,
				specularTextureData.height,
				specularTextureData.depth,
				DXGI_FORMAT_R8G8B8A8_UNORM,
				D3D11_BIND_SHADER_RESOURCE,
				specularTextureData.inData);
	}

	if (not mTextures.normal.empty())
	{
		CTextur::Texture2DDescriptor normalTextureData{ };
		CTextur::LoadTextureFromFile(mTextures.normal.c_str(), normalTextureData);
		mNormalTexture
			= CDXCommon::CreateTexture2D(
				aDevice,
				normalTextureData.width,
				normalTextureData.height,
				normalTextureData.depth,
				DXGI_FORMAT_R8G8B8A8_UNORM,
				D3D11_BIND_SHADER_RESOURCE,
				normalTextureData.inData);
	}
	if (not mTextures.normal.empty())
	{
		bool valid = true;
		for (std::string const &Filename : )
		{
			valid = valid && not Filename.empty();
		}
		if (valid)
		{
			CTextur::Texture2DDescriptor ReflectionMap{ };
			CTextur::LoadTextureArrayFromFiles(mTextures.reflectionMap, ReflectionMap);
			mReflectionMap
				= CDXCommon::CreateTexture2D(
					aDevice,
					ReflectionMap.width,
					ReflectionMap.height,
					ReflectionMap.depth,
					DXGI_FORMAT_R8G8B8A8_UNORM,
					D3D11_BIND_SHADER_RESOURCE,
					normalTextureData.inData[0]);

		}
	}
}

 bool CDefaultMaterial::InitializeImpl(std::shared_ptr<CDXIntegration> aDirectX)
{
	bool const successful = CreateBuffer<SMatrixBuffer>(aDirectX->myDevice(), &mBuffer);
	return successful;
}

 bool CDefaultMaterial::CreateImpl()
{
	mBuffer = nullptr;

	return true;
}

 bool CDefaultMaterial::CommitImpl(std::shared_ptr<CDXIntegration> aDirectX)
{
	auto const &updater = [this](SMatrixBuffer *pData) -> bool
	{
		memcpy(pData, &mBufferData, sizeof(SMatrixBuffer));
		return true;
	};

	bool const updated = UpdateBuffer<SMatrixBuffer>(
		aDirectX->myContext(),
		mBuffer,
		updater);

	return updated;
}

 bool CDefaultMaterial::FinalizeImpl()
{
	if (mDiffuseTexture.sampler) { mDiffuseTexture.sampler->Release(); mDiffuseTexture.sampler = nullptr; }
	if (mDiffuseTexture.rtv) { mDiffuseTexture.rtv->Release(); mDiffuseTexture.rtv = nullptr; }
	if (mDiffuseTexture.srv) { mDiffuseTexture.srv->Release(); mDiffuseTexture.srv = nullptr; }
	if (mDiffuseTexture.texture) { mDiffuseTexture.texture->Release(); mDiffuseTexture.texture = nullptr; }

	if (mSpecularTexture.sampler) { mSpecularTexture.sampler->Release(); mSpecularTexture.sampler = nullptr; }
	if (mSpecularTexture.rtv) { mSpecularTexture.rtv->Release(); mSpecularTexture.rtv = nullptr; }
	if (mSpecularTexture.srv) { mSpecularTexture.srv->Release(); mSpecularTexture.srv = nullptr; }
	if (mSpecularTexture.texture) { mSpecularTexture.texture->Release(); mSpecularTexture.texture = nullptr; }

	if (mNormalTexture.sampler) { mNormalTexture.sampler->Release(); mNormalTexture.sampler = nullptr; }
	if (mNormalTexture.rtv) { mNormalTexture.rtv->Release(); mNormalTexture.rtv = nullptr; }
	if (mNormalTexture.srv) { mNormalTexture.srv->Release(); mNormalTexture.srv = nullptr; }
	if (mNormalTexture.texture) { mNormalTexture.texture->Release(); mNormalTexture.texture = nullptr; }

	if (mBuffer)
	{
		mBuffer->Release();
	}

	return true;
}

 std::vector<ID3D11Buffer*> CDefaultMaterial::DataBuffersImpl()
{
	std::vector<ID3D11Buffer *> buffers;
	buffers.push_back(mBuffer);
	return buffers;
}

 std::vector<ID3D11ShaderResourceView*> CDefaultMaterial::ShaderResourcesImpl()
{
	return {mDiffuseTexture.srv,mSpecularTexture.srv,mNormalTexture.srv};
}

 std::vector<ID3D11SamplerState*> CDefaultMaterial::SamplersImpl()
{
	return {mDiffuseTexture.sampler,mSpecularTexture.sampler,mNormalTexture.sampler};
}
