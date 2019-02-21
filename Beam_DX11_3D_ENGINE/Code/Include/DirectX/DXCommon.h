#pragma once

#include <d3d.h>
#include <dxgi.h>
#include <vector>
#include <d3d11.h>
#include <assert.h>
#include <Windows.h>
#include <d3dcommon.h>
#include <d3dcompiler.h>

// Include Maths

#define _USE_MATH_DEFINES 
#include <cmath>
#include <math.h>

#define _XM_NO_INTRINSICS_
#include <DirectXMath.h>

#define VEC_X(v) v.vector4_f32[0]
#define VEC_Y(v) v.vector4_f32[1]
#define VEC_Z(v) v.vector4_f32[2]
#define VEC_W(v) v.vector4_f32[3]

// Include DirectX11 

#include <dxgi.h>
#include <d3d.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <d3dcommon.h>

#ifndef __DX11_LINKED__
#define __DX11_LINKED__
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d10.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")
#endif

#include "../Base/Window.h"

using namespace DirectX;

struct STextureState2D
{
	uint32_t                  width;
	uint32_t                  height;
	uint32_t                  depth;
	DXGI_FORMAT               format;
	ID3D11Texture2D          *texture;
	ID3D11SamplerState       *sampler;
	ID3D11RenderTargetView   *rtv;
	ID3D11ShaderResourceView *srv;

};

class CDXCommon
{
public:

	static bool DetermineHardwareIntegration(
		std::shared_ptr<CWindow> aWindow, IDXGIAdapter **aSelectedAdapter,
		IDXGIOutput **aSelectedOutput, DXGI_MODE_DESC *aSelectedMode);

	static STextureState2D CreateTexture2D(ID3D11Device *aDevice, uint32_t const &aWidth, uint32_t const &aHeight,
		uint32_t const &aDepth, DXGI_FORMAT const &aFormat, D3D11_BIND_FLAG const &aBindFlag, std::vector<std::vector<uint8_t>> const &aData, bool const &aIsCubeMap);
};

