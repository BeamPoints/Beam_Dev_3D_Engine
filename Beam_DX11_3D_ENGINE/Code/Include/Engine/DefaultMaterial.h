#pragma once
#include "../Rendering/Material.h"
#include "../Rendering/Light.h"
#include "Textur.h"

class CDefaultMaterial : public MaterialBase<CDefaultMaterial>
{
	struct SMatrixBuffer
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

public:
	struct STextureCollection
	{
		std::string diffuse;
		std::string specular;
		std::string normal;
		std::string reflectivity;
		std::vector<std::string> reflectionMap;
	};

	CDefaultMaterial() = default;
	virtual ~CDefaultMaterial() = default;

	void setWorldMatrix(XMMATRIX aMatrix);
	void setViewMatrix(XMMATRIX aMatrix);
	void setProjectionMatrix(XMMATRIX aMatrix);

	void setTextures(ID3D11Device *aDevice, STextureCollection const &aTextures);

	inline STextureState2D const &getDiffuseTexture() const { return mDiffuseTexture; }
	inline STextureState2D const &getSpecularTexture() const { return mSpecularTexture; }
	inline STextureState2D const &getNormalTexture() const { return mNormalTexture; }
	inline STextureState2D const &getmReflectionMap() const { return mReflectionMap; }
	inline STextureState2D const &getReflectionMap() const { return mReflectionMap; }

    bool InitializeImpl(std::shared_ptr<CDXIntegration> aDirectX);
    bool CreateImpl();
    bool UpdateImpl(CTimer::State const &aTimerState, CInput const &aInputState) { return true; }
    bool CommitImpl(std::shared_ptr<CDXIntegration> aDirectX);
    bool DestroyImpl() { return true; }
    bool FinalizeImpl();
   
    std::vector<ID3D11Buffer *> DataBuffersImpl();
    std::vector<ID3D11ShaderResourceView *> ShaderResourcesImpl();
    std::vector<ID3D11SamplerState *> SamplersImpl();

private:
	STextureCollection  mTextures;
	SMatrixBuffer       mBufferData;
	ID3D11Buffer       *mBuffer;


	STextureState2D mDiffuseTexture;
	STextureState2D mSpecularTexture;
	STextureState2D mNormalTexture;
	STextureState2D mReflectionMap;
	STextureState2D mReflectivityTexture;
};

