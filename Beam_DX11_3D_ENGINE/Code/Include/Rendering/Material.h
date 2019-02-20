#pragma once

#include <vector>
#include <fstream>
#include <functional>

#include "../Base/Timer.h"
#include "../Base/Input.h"
#include "../DirectX/DXIntegration.h"

//--------------------------------------TEMPLATES--------------------------------------------------

template <typename TCBufferType> static bool CreateBuffer(ID3D11Device *aContext, ID3D11Buffer **aBuffer)
{
	assert(nullptr != aContext);
	assert(nullptr != aBuffer);

	D3D11_BUFFER_DESC bufferDesc{ };
	bufferDesc.ByteWidth = sizeof(TCBufferType);
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	aContext->CreateBuffer(&bufferDesc, nullptr, aBuffer);

	return true;
}

template <typename TCBufferType> static bool UpdateBuffer(ID3D11DeviceContext *aContext,
	ID3D11Buffer *aBuffer, std::function<bool(TCBufferType *)> aUpdate)
{
	assert(nullptr != aBuffer);
	assert(aUpdate);

	D3D11_MAPPED_SUBRESOURCE mappedResource{ };
	aContext->Map(aBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	bool const updated = aUpdate(static_cast<TCBufferType*>(mappedResource.pData));

	aContext->Unmap(aBuffer, 0);

	return updated;
}

//-------------------------------------------CMATERIAL---------------------------------------------

class CMaterial
{public:
//-------------------------------------CMATERIAL-STRUCTS-------------------------------------------

	struct MaterialInfo
	{
		static std::vector<D3D11_INPUT_ELEMENT_DESC> const sVertexLayout;

		std::string VSFilename;
		std::string PSFilename;
	};

	struct Shaders
	{
		ID3D11InputLayout  *inputLayout;
		ID3D11VertexShader *VS;
		ID3D11PixelShader  *PS;
	};

//-------------------------------------CMATERIAL-API-----------------------------------------------


	virtual ~CMaterial() = default;
	CMaterial(CMaterial const &) = delete;
	CMaterial(CMaterial &&) = delete;
	CMaterial& operator=(CMaterial const &) = delete;
	CMaterial& operator=(CMaterial &&) = delete;

	virtual bool Create(CMaterial::MaterialInfo const &) = 0;
	virtual bool Initialize(std::shared_ptr<CDXIntegration>) = 0;
	virtual bool Finalize() = 0;
	virtual bool Destroy() = 0;

	virtual bool Update(CTimer::State const &aTimerState, CInput const &aInputState) = 0;
	virtual bool Commit(std::shared_ptr<CDXIntegration>) = 0;

	virtual std::vector<ID3D11Buffer *> getDataBuffers() = 0;

	virtual CMaterial::MaterialInfo          const &getMaterialInfo()    const = 0;
	virtual CMaterial::Shaders               const &getShaders()         const = 0;
	virtual std::vector<ID3D11ShaderResourceView *> getShaderResources() = 0;
	virtual std::vector<ID3D11SamplerState       *> getSamplers() = 0;

//-------------------------------------CMATERIAL---------------------------------------------------


	static bool LoadShader(std::string const &aFilename, std::vector<char> &aByteCode);
	
protected:
	CMaterial() = default;
};

//-------------------------------------CMATERIALBASE-TEMPLATE--------------------------------------

template <typename TMaterialImplementation> 
class MaterialBase : public CMaterial
{
public:
	MaterialBase<TMaterialImplementation>() = default;
	virtual ~MaterialBase<TMaterialImplementation>() = default;

//-------------------------------------CMATERIALBASE-API-------------------------------------------

	bool Create(CMaterial::MaterialInfo const &aMaterialInfo)
	{
		mMaterialInfo = aMaterialInfo;

		bool const baseCreated = static_cast<TMaterialImplementation *>(this)->CreateImpl();
		return baseCreated;
	}

	bool Initialize(std::shared_ptr<CDXIntegration> aDirectX)
	{
		ID3D11Device *const device = aDirectX->myDevice();

		ID3D11VertexShader *VS = nullptr;
		ID3D11PixelShader  *PS = nullptr;

		std::vector<char> VSByteCode = { };
		std::vector<char> PSByteCode = { };

//-------------------------------------CMATERIALBASE-LOAD-VERTEX-SHADER----------------------------

		bool const vertexShaderLoaded = LoadShader(mMaterialInfo.VSFilename, VSByteCode);
		if (not vertexShaderLoaded){throw 22;}
		device->CreateVertexShader(VSByteCode.data(),VSByteCode.size(),nullptr, &VS);

//-------------------------------------CMATERIALBASE-LOAD-PIXEL-SHADER-----------------------------

		bool const pixelShaderLoaded = LoadShader(mMaterialInfo.PSFilename, PSByteCode);
		if (not pixelShaderLoaded){throw 23;}
		device->CreatePixelShader(PSByteCode.data(),PSByteCode.size(),nullptr,&PS);

		ID3D11InputLayout *CInputLayout = nullptr;
		device->CreateInputLayout(MaterialInfo::sVertexLayout.data(),MaterialInfo::sVertexLayout.size(),
			VSByteCode.data(),VSByteCode.size(),&CInputLayout);


		VSByteCode.clear();
		PSByteCode.clear();

		CMaterial::Shaders shaders{ };
		shaders.inputLayout = CInputLayout;
		shaders.VS = VS;
		shaders.PS = PS;

		mShaders = shaders;

		bool const baseInitialized = static_cast<TMaterialImplementation *>(this)->InitializeImpl(aDirectX);
		return baseInitialized;
	}

	bool Finalize()
	{
		bool const baseFinalize = static_cast<TMaterialImplementation *>(this)->FinalizeImpl();

		mShaders.inputLayout->Release();
		mShaders.VS->Release();
		mShaders.PS->Release();

		mShaders.inputLayout = nullptr;
		mShaders.VS = nullptr;
		mShaders.PS = nullptr;

		return baseFinalize;
	}

	bool Destroy()
	{
		bool const baseDestroyed = static_cast<TMaterialImplementation *>(this)->DestroyImpl();
		return baseDestroyed;
	}

	bool Update(CTimer::State const &aTimerState, CInput const &aInputState)
	{
		bool const baseUpdated = static_cast<TMaterialImplementation *>(this)->UpdateImpl(aTimerState, aInputState);
		return baseUpdated;
	}

	bool Commit(std::shared_ptr<CDXIntegration> aDirectX)
	{
		bool const baseCommitted = static_cast<TMaterialImplementation *>(this)->CommitImpl(aDirectX);
		return baseCommitted;
	}

	CMaterial::MaterialInfo const &getMaterialInfo() const { return mMaterialInfo; }
	CMaterial::Shaders      const &getShaders()      const { return mShaders; }

	std::vector<ID3D11Buffer *> getDataBuffers()
	{
		return static_cast<TMaterialImplementation *>(this)->DataBuffersImpl();
	}

	std::vector<ID3D11ShaderResourceView *> getShaderResources()
	{
		return static_cast<TMaterialImplementation *>(this)->ShaderResourcesImpl();
	}

	std::vector<ID3D11SamplerState *> getSamplers()
	{
		return static_cast<TMaterialImplementation *>(this)->SamplersImpl();
	}

private:
	CMaterial::MaterialInfo mMaterialInfo;
	CMaterial::Shaders mShaders;
};

//-------------------------------------CMATERIALBASE-END-------------------------------------------
