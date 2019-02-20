#include "../../Include/DirectX/DXCommon.h"

bool CDXCommon::DetermineHardwareIntegration(std::shared_ptr<CWindow> aWindow, IDXGIAdapter **aSelectedAdapter,IDXGIOutput **aSelectedOutput, DXGI_MODE_DESC *aSelectedMode)
{
	assert(nullptr != aSelectedAdapter);
	assert(nullptr != aSelectedOutput);
	
	IDXGIFactory *Factory = nullptr;
	void         *FactoryVoid = nullptr;

	CreateDXGIFactory(__uuidof(IDXGIFactory), &FactoryVoid);
	Factory = static_cast<IDXGIFactory *>(FactoryVoid);
	uint64_t adapterIndex = 0;
	IDXGIAdapter *Adapter = nullptr;
	while (S_OK == Factory->EnumAdapters(adapterIndex, &Adapter))
	{
		DXGI_ADAPTER_DESC adapterDesc = { };
		Adapter->GetDesc(&adapterDesc);
		uint64_t     outputIndex = 0;
		IDXGIOutput *Output = nullptr;
		while (S_OK == Adapter->EnumOutputs(outputIndex, &Output))
		{
			DXGI_OUTPUT_DESC outputDesc = { };
			Output->GetDesc(&outputDesc);

			DXGI_FORMAT desiredFormat = DXGI_FORMAT_R8G8B8A8_UNORM;

			uint32_t                    modeCount = 0;
			std::vector<DXGI_MODE_DESC> modes{ };
			Output->GetDisplayModeList(desiredFormat, 0, &modeCount, nullptr);
			modes.resize(modeCount);
			Output->GetDisplayModeList(desiredFormat, 0, &modeCount, modes.data());

			for (auto const &mode : modes)
			{
				uint64_t const width = mode.Width;
				uint64_t const height = mode.Height;

				if (aWindow->display().width == width && aWindow->display().height == height)
				{
					*aSelectedAdapter = Adapter;
					*aSelectedOutput = Output;
					*aSelectedMode = mode;

					Factory->Release();

					return true;
				}
			}

			Output->Release();
			++outputIndex;
		}

		Adapter->Release();
		++adapterIndex;
	}

	Factory->Release();
	return false;
}

 STextureState2D CDXCommon::CreateTexture2D(ID3D11Device * aDevice, uint32_t const & aWidth, uint32_t const & aHeight,
	uint32_t const & aDepth,DXGI_FORMAT const & aFormat, D3D11_BIND_FLAG const & aBindFlag, std::vector<std::vector<uint8_t>> const & aData, bool const &aIsCubeMap)
{
	D3D11_TEXTURE2D_DESC textureDesc{ };
	textureDesc.Width = aWidth;
	textureDesc.Height = aHeight;
	textureDesc.ArraySize = aDepth;
	textureDesc.MipLevels = 1;
	textureDesc.Format = aFormat;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = aBindFlag;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = (aIsCubeMap ? D3D11_RESOURCE_MISC_TEXTURECUBE:0);

	std::vector<D3D11_SUBRESOURCE_DATA> inputData;
	inputData.resize(aDepth);
	if(not(aData.empty() || aData.size() < aDepth))
	{
	
		for (uint32_t k = 0; k < aDepth; k++)
		{
			D3D11_SUBRESOURCE_DATA  data{ };
	
			if (not aData[k].empty())
			{
				data.pSysMem = aData[k].data();
				data.SysMemPitch = aWidth * sizeof(uint8_t) * 4;
				data.SysMemSlicePitch = 0;
	
			}
			inputData[k] = data;
		}
	}
	D3D11_SUBRESOURCE_DATA * input = inputData.empty() ? nullptr : inputData.data();

	ID3D11Texture2D *textureHandle = nullptr;
	aDevice->CreateTexture2D(&textureDesc, inputData.data(), &textureHandle);
	ID3D11SamplerState *sampler = nullptr;
	D3D11_SAMPLER_DESC samplerDesc{};
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	samplerDesc.MipLODBias = 0;
	samplerDesc.MaxAnisotropy = 0;
	samplerDesc.BorderColor[0] = 0.0f;
	samplerDesc.BorderColor[1] = 0.0f;
	samplerDesc.BorderColor[2] = 0.0f;
	samplerDesc.BorderColor[3] = 0.0f;

	aDevice->CreateSamplerState(&samplerDesc, &sampler);

	ID3D11RenderTargetView *rtv = nullptr;
	if (D3D11_BIND_RENDER_TARGET == aBindFlag)
	{
		D3D11_RENDER_TARGET_VIEW_DESC rtvDesc{};
		rtvDesc.Format = aFormat;

		if (1 == aDepth)
		{
			rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
			rtvDesc.Texture2D.MipSlice = 0;
		}
		else
		{
			rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
			rtvDesc.Texture2DArray.MipSlice = 0;
			rtvDesc.Texture2DArray.FirstArraySlice = 0;
			rtvDesc.Texture2DArray.ArraySize = aDepth;
		}


		aDevice->CreateRenderTargetView(textureHandle, &rtvDesc, &rtv);
	}

	ID3D11ShaderResourceView *srv = nullptr;
	if (D3D11_BIND_SHADER_RESOURCE == aBindFlag)
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{ };
		srvDesc.Format = aFormat;
		if (1 == aDepth)
		{
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			srvDesc.Texture2D.MipLevels = 1;
			srvDesc.Texture2D.MostDetailedMip = 0;
		}
		else
		{
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
			srvDesc.Texture2DArray.MostDetailedMip =
			srvDesc.Texture2DArray.MipLevels =
			srvDesc.Texture2DArray.FirstArraySlice =
			srvDesc.Texture2DArray.ArraySize = aDepth;
		}
		aDevice->CreateShaderResourceView(textureHandle, &srvDesc, &srv);
	}

	STextureState2D texture{ };
	texture.width = aWidth;
	texture.height = aHeight;
	texture.depth = aDepth;
	texture.format = aFormat;
	texture.texture = textureHandle;
	texture.sampler = sampler;
	texture.rtv = rtv;
	texture.srv = srv;

	return texture;
}

