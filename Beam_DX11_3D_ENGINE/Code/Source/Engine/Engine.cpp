#include "../../Include/Engine/Engine.h"

CEngine::CEngine(std::shared_ptr<CRenderer> const &aRenderer, std::shared_ptr<CDXIntegration> const &aDirectX)
	: mRenderer(aRenderer), mDirectX(aDirectX)
{
	assert(nullptr != aRenderer);
	assert(nullptr != aDirectX);
}

void CEngine::Initialize()
{
	std::shared_ptr<CMesh> mesh = nullptr;
	std::shared_ptr<CMaterial> material = nullptr;

	//mMesh = CMesh::CreateTriangle(aDirectX);
	mesh = CMesh::CreateMeshFromFile(mDirectX, "resources/meshes/Box_low.obj");

	CMaterial::MaterialInfo initInfo{ };
	initInfo.VSFilename = "standard_vs.cso";
	initInfo.PSFilename = "standard_ps.cso";

	material = std::make_shared<CDefaultMaterial>(); 
	material->Create(initInfo);
	material->Initialize(mDirectX);

	CDefaultMaterial::STextureCollection textures{ };
	textures.diffuse = "resources/textures/low_default_AlbedoTransparency.png";
	textures.specular = "resources/textures/low_default_MetallicSmoothness.png";
	textures.normal = "resources/textures/low_default_Normal.png";
	textures.reflectivity = "resources/textures/low_default_AO.png";
	//textures.reflectionMap = 
	//{
	//	"Resources/textures/sor_cwd/cwd_rt.png",
	//	"Resources/textures/sor_cwd/cwd_lf.png",
	//	"Resources/textures/sor_cwd/cwd_up.png",
	//	"Resources/textures/sor_cwd/cwd_dn.png",
	//	"Resources/textures/sor_cwd/cwd_ft.png",
	//	"Resources/textures/sor_cwd/cwd_bk.png",
	//};
	std::shared_ptr <CDefaultMaterial> defaultMaterial;
	defaultMaterial = std::static_pointer_cast<CDefaultMaterial>(material);
	defaultMaterial->setTextures(mDirectX->myDevice(), textures);

	mEntity = std::make_shared<CEntity>(mesh, material);
	mEntity->getTransform().setTranslationZ(20);
	mEntity->getTransform().setScale(0.05, 0.05, 0.05);

	CCamera::CameraParameters cameraParameters;
	
	cameraParameters.projectionType = CCamera::ECameraProjectionType::Perspective;
	cameraParameters.fovY = RAD(60);
	cameraParameters.aspectRatio = (1920.0 / 1080.0);
	cameraParameters.minDepth = 0.01;
	cameraParameters.maxDepth = 100;
	
	mCamera.Initialize(cameraParameters);

	CLight::SLightParameters lightParameters;
	lightParameters.lightType = CLight::ELightType::Spot;
	lightParameters.position = { 0.0f, 20.0f, 0.0f, 1.0f };
	lightParameters.direction = { 0.0f, -1.0f, 1.0f, 0.0f };
	lightParameters.color = { 1.0f, 0.5f, 0.5f, 1.0f };
	lightParameters.intensity = 1;
	lightParameters.distance = 20.0f;
	lightParameters.hotspotAngle = 25.0f;
	lightParameters.falloffAngle = 10.0f;
	mLight.initialize(lightParameters);
}

void CEngine::Update(CTimer::State const & aTimerState, CInput const & aInputState)
{
	static CEntity *inputReceiver = nullptr;
	if (aInputState.getPressedWithCtrl(KeyCode::Y))
	{
		inputReceiver = &mCamera;
	}
	else
		if (aInputState.getPressedWithCtrl(KeyCode::X))
		{
			inputReceiver = &*mEntity;
		}
		else
			if (aInputState.getPressedWithCtrl(KeyCode::C))
			{
				inputReceiver = &mLight;
			}

	if (nullptr != inputReceiver)
	{
		inputReceiver->Update(aTimerState, aInputState);
	}

	mCamera.Update(aTimerState, {});
	XMMATRIX const &viewMatrix = mCamera.viewMatrix();
	XMMATRIX const &projectionMatrix = mCamera.projectionMatrix();

	mEntity->Update(aTimerState, aInputState);
	mEntity->getTransform().setScale(0.05, 0.05, 0.05);
	mEntity->getTransform().worldMatrix(XMMatrixIdentity(), nullptr);
	
	std::shared_ptr<CDefaultMaterial> material;
	material = std::static_pointer_cast<CDefaultMaterial>(mEntity->material()); // 
	material->setWorldMatrix(mEntity->getTransform().composedWorldMatrix());
	material->setViewMatrix(viewMatrix);
	material->setProjectionMatrix(projectionMatrix);
}

void CEngine::Render()
{
	mRenderer->Render(mDirectX, { mEntity });
}

void CEngine::Finalize()
{
	mLight.deinitialize();
	mCamera.Finalize();
	mRenderer->Finalize();
	mDirectX->Finalize();
	mEntity->Finalize();

	mEntity = nullptr;
	mRenderer = nullptr;
	mDirectX = nullptr;
}
