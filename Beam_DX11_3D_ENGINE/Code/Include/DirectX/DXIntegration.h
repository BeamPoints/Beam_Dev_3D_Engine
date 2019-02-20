#pragma once
#include "DXCommon.h"
#include "../Base/Window.h"

class CDXIntegration
{
public:
	static std::shared_ptr<CDXIntegration> Create(std::shared_ptr<CWindow> aEnvironment);

	
	ID3D11Device        *const myDevice() const { return mDevice; }
	inline ID3D11DeviceContext *const myContext() const	{ return mContext; }
	inline IDXGISwapChain      *const mySwapChain() const { return mSwapChain; }

	inline void Finalize()
	{
		mSwapChain->Release();
		mContext->Release();
		mDevice->Release();
	}

private:
	CDXIntegration(ID3D11Device *aDevice, ID3D11DeviceContext *aContext, IDXGISwapChain*aSwapChain);

	ID3D11Device        *mDevice;
	ID3D11DeviceContext *mContext; 
	IDXGISwapChain      *mSwapChain;
};

