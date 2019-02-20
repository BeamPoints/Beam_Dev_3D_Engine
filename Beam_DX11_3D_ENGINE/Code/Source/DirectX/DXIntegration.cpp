#include "../../Include/DirectX/DXIntegration.h"

CDXIntegration::CDXIntegration(ID3D11Device *aDevice, ID3D11DeviceContext *aImmediateContext, IDXGISwapChain*aSwapChain)
	:mDevice(aDevice), mContext(aImmediateContext), mSwapChain(aSwapChain){}


std::shared_ptr<CDXIntegration> CDXIntegration::Create(std::shared_ptr<CWindow> aWindow)
{
	IDXGIAdapter   *pAdapter = nullptr;
	IDXGIOutput    *pOutput = nullptr;
	DXGI_MODE_DESC  outputMode = { };

	bool const adapterDetermined = CDXCommon::DetermineHardwareIntegration(aWindow, &pAdapter, &pOutput, &outputMode);
	assert(true == adapterDetermined);

	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	swapChainDesc.BufferCount = 2;
	swapChainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER | DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferDesc = outputMode;
	swapChainDesc.OutputWindow = aWindow->WindowHandle();
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Windowed = true;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Flags = 0;

	D3D_FEATURE_LEVEL FeatureLVL = D3D_FEATURE_LEVEL_11_0;

	ID3D11Device        *pDevice = nullptr;
	ID3D11DeviceContext *pContext = nullptr;
	IDXGISwapChain      *pSwapChain = nullptr;

	D3D_FEATURE_LEVEL suppFeatureLVL = {};

	D3D11CreateDeviceAndSwapChain(pAdapter,D3D_DRIVER_TYPE_UNKNOWN,0,D3D11_CREATE_DEVICE_DEBUG,&FeatureLVL,1,D3D11_SDK_VERSION,
		&swapChainDesc,&pSwapChain,&pDevice,&suppFeatureLVL,&pContext);

	CDXIntegration *temp = new CDXIntegration(pDevice,pContext,pSwapChain);

	std::shared_ptr<CDXIntegration> integration = std::shared_ptr<CDXIntegration>(temp);

	return std::move(integration);
}
