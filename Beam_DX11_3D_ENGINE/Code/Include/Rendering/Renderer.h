#pragma once
#include "../DirectX/DXCommon.h"
#include "../DirectX/DXIntegration.h"
#include "Mesh.h"
#include "Material.h"
#include "Entity.h"


class CRenderer
{public:

	bool Initialize(std::shared_ptr<CDXIntegration> const &aDirectX);
	void Update() {};
	void Render(std::shared_ptr<CDXIntegration> const &aDirectX, std::vector<std::shared_ptr<CEntity>> const &aEntities);
	void Finalize();

private:


	bool CreateRenderStates(std::shared_ptr<CDXIntegration> const &aDirectX);

	STextureState2D mDepthTextureState;
	ID3D11DepthStencilView *mDepthStencilView;
	ID3D11DepthStencilState *mDepthStencilState;
	ID3D11RasterizerState *mRasterizer;
	D3D11_VIEWPORT mViewPort;
};
