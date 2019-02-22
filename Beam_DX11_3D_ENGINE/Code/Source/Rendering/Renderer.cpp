#include "../../Include/Rendering/Renderer.h"
#include "../../Include/Engine/DefaultMaterial.h"

//-------------------------------------CRENDER-CODE------------------------------------------------
//-------------------------------------INITIALIZE--------------------------------------------------

bool CRenderer::Initialize(std::shared_ptr<CDXIntegration> const & aDirectX)
{
	return CreateRenderStates(aDirectX);
}

//-------------------------------------RENDER------------------------------------------------------

void CRenderer::Render(std::shared_ptr<CDXIntegration> const &aDirectX, std::vector<std::shared_ptr<CEntity>> const &aEntities)
{
	D3D11_VIEWPORT viewPort{};
	viewPort.TopLeftX = 0;
	viewPort.TopLeftY = 0;
	viewPort.Width = 1920;
	viewPort.Height = 1080;
	viewPort.MinDepth = 0.1f;
	viewPort.MaxDepth = 1.0f;

	ID3D11Device * const device = aDirectX->myDevice();
	ID3D11DeviceContext *const context = aDirectX->myContext();
	IDXGISwapChain *const swapChain = aDirectX->mySwapChain();

	DXGI_SWAP_CHAIN_DESC swapChainDescriptor;
	swapChain->GetDesc(&swapChainDescriptor);

	ID3D11Texture2D *BackBufferTex = nullptr;
	swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&BackBufferTex);

	D3D11_RENDER_TARGET_VIEW_DESC backBufferDesc = {};
	backBufferDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	backBufferDesc.Texture2D.MipSlice = 0;
	backBufferDesc.Format = swapChainDescriptor.BufferDesc.Format;

	ID3D11RenderTargetView *backBufferRTV = nullptr;
	device->CreateRenderTargetView(BackBufferTex, &backBufferDesc, &backBufferRTV);

	BackBufferTex->Release();
	BackBufferTex = nullptr;

	//RENDERER START

	float color[4] = { 0.2f , 0.2f, 0.2f, 1.0f };
	context->ClearRenderTargetView(backBufferRTV, color);
	context->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0xFF);

	for (std::shared_ptr<CEntity> const &entity : aEntities)
	{
		CTransform &transform = entity->transform();
		transform.worldMatrix(XMMatrixIdentity(), nullptr);

		std::shared_ptr<CMesh>     const  mesh = entity->mesh();
		std::shared_ptr<CMaterial>        material = entity->material();

		std::shared_ptr<CDefaultMaterial> defaultMaterial = std::static_pointer_cast<CDefaultMaterial>(material);
		defaultMaterial->setWorldMatrix(transform.composedWorldMatrix());

		material->Commit(aDirectX); // Push data to GPU.

		std::vector<ID3D11Buffer *> vertexBuffers = mesh->getMeshData().vertexBuffers;
		std::vector<UINT> strides =
		{
			sizeof(XMVECTOR),
			sizeof(XMFLOAT3),
			sizeof(XMFLOAT3),
			sizeof(XMVECTOR)
		};
		std::vector<UINT> offsets =
		{
			0, 0, 0, 0
		};

		CMaterial::Shaders const &shaders = material->getShaders();

		context->IASetInputLayout(shaders.inputLayout);
		context->IASetVertexBuffers(0, vertexBuffers.size(), vertexBuffers.data(), strides.data(), offsets.data());
		context->IASetIndexBuffer(mesh->getMeshData().indexBuffer, DXGI_FORMAT_R32_UINT, 0);
		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		context->VSSetShader(shaders.VS, nullptr, 0);

		std::vector<ID3D11Buffer *> buffers = material->getDataBuffers();
		ID3D11Buffer** dataBuffers = buffers.data();
		uint64_t const dataBufferCount = material->getDataBuffers().size();
		context->VSSetConstantBuffers(0, dataBufferCount, dataBuffers);

		std::vector<ID3D11ShaderResourceView*> resources = material->getShaderResources();
		std::vector<ID3D11SamplerState*>       samplers = material->getSamplers();

		context->PSSetShaderResources(0, resources.size(), resources.data());
		context->PSSetSamplers(0, samplers.size(), samplers.data());

		context->RSSetState(mRasterizer);
		context->RSSetViewports(1, &viewPort);

		context->PSSetShader(shaders.PS, nullptr, 0);

		context->OMSetRenderTargets(1, &backBufferRTV, mDepthStencilView);
		context->OMSetDepthStencilState(mDepthStencilState, 0);

		context->DrawIndexed(mesh->getMeshData().indexCount, 0, 0);
	}

	swapChain->Present(0, 0);

	context->ClearState();

	// RENDER END

	backBufferRTV->Release();
	backBufferRTV = nullptr;
}

//-------------------------------------CRENDER-FINALIZE--------------------------------------------

void CRenderer::Finalize()
{

	mDepthStencilState->Release();
	mDepthStencilView->Release();
	mDepthTextureState.texture->Release();
	mRasterizer->Release();
}

//-------------------------------------CRENDER-RENDERSTATES----------------------------------------

bool CRenderer::CreateRenderStates(std::shared_ptr<CDXIntegration> const & aDirectX)
{
	ID3D11Device *device = aDirectX->myDevice();

	D3D11_DEPTH_STENCIL_DESC desc{};
	desc.DepthEnable = true;
	desc.DepthFunc = D3D11_COMPARISON_LESS;
	desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	desc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	desc.StencilEnable = true;
	desc.StencilReadMask = 0xFF;
	desc.StencilWriteMask = 0xFF;

	ID3D11DepthStencilState *depthStencilState = nullptr;
	device->CreateDepthStencilState(&desc, &depthStencilState);

	IDXGISwapChain *swapChain = aDirectX->mySwapChain();

	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	swapChain->GetDesc(&swapChainDesc);

	STextureState2D textureState = CDXCommon::CreateTexture2D(
		device,
		swapChainDesc.BufferDesc.Width,
		swapChainDesc.BufferDesc.Height,
		1,
		DXGI_FORMAT_D24_UNORM_S8_UINT,
		D3D11_BIND_DEPTH_STENCIL,
		{}, false);

	mDepthTextureState = textureState;

	D3D11_DEPTH_STENCIL_VIEW_DESC depthViewDesc{};
	depthViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthViewDesc.Texture2D.MipSlice = 0;
	depthViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthViewDesc.Flags = 0;

	ID3D11DepthStencilView *depthStencilView = nullptr;
	device->CreateDepthStencilView(textureState.texture, &depthViewDesc, &depthStencilView);

	ID3D11RasterizerState *rasterizer = nullptr;

	D3D11_RASTERIZER_DESC rasterizerDesc = { };
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_BACK;
	rasterizerDesc.FrontCounterClockwise = false;
	rasterizerDesc.AntialiasedLineEnable = true;
	rasterizerDesc.MultisampleEnable = false;
	rasterizerDesc.ScissorEnable = false;
	rasterizerDesc.DepthClipEnable = false;

	device->CreateRasterizerState(&rasterizerDesc, &rasterizer);

	mViewPort.TopLeftX = 0;
	mViewPort.TopLeftY = 0;
	mViewPort.Width = swapChainDesc.BufferDesc.Width;
	mViewPort.Height = swapChainDesc.BufferDesc.Height;
	mViewPort.MinDepth = 0.0;
	mViewPort.MaxDepth = 1.0;

	mDepthStencilView = depthStencilView;
	mDepthStencilState = depthStencilState;
	mRasterizer = rasterizer;

	return true;
}

//-------------------------------------CRENDER-CODE-END--------------------------------------------
