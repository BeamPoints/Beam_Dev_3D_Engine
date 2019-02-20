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

	material = std::make_shared<CDefaultMaterial>(); //!ERROR C2440
	material->Create(initInfo);
	material->Initialize(mDirectX);

	CDefaultMaterial::STextureCollection textures{ };
	textures.diffuse = "resources/textures/low_default_AlbedoTransparency.png";
	textures.specular = "resources/textures/low_default_MetallicSmoothness.png";
	textures.normal = "resources/textures/low_default_Normal.png";
	textures.reflectionMap =
	{
		"resources/textures/sor_cwd/cwd_lt.png",
		"resources/textures/sor_cwd/cwd_rt.png"
		"resources/textures/sor_cwd/cwd_.png"
		"resources/textures/sor_cwd/cwd_.png"
		"resources/textures/sor_cwd/cwd_.png"
		"resources/textures/sor_cwd/cwd_bk.png"


	}
	std::shared_ptr < CDefaultMaterial> defaultMaterial;
	defaultMaterial = std::static_pointer_cast<CDefaultMaterial>(material);
	defaultMaterial->setTextures(mDirectX->myDevice(), textures);

	mEntity = std::make_shared<CEntity>(mesh, material); //!ERROR C2440

	CCamera::CameraParameters cameraParameters;
	
	cameraParameters.projectionType = CCamera::ECameraProjectionType::Perspective;
	cameraParameters.fovY = RAD(60);
	cameraParameters.aspectRatio = (1920.0 / 1080.0);
	cameraParameters.minDepth = 0.01;
	cameraParameters.maxDepth = 100;
	
	mCamera.Initialize(cameraParameters);
}

void CEngine::Update(CTimer::State const & aTimerState, CInput const & aInputState)
{
	mCamera.Update(aTimerState, aInputState);
	XMMATRIX const &viewMatrix = mCamera.viewMatrix();
	XMMATRIX const &projectionMatrix = mCamera.projectionMatrix();
	
	float scaleFactor = 0.05;
	XMMATRIX scale = XMMatrixScaling(scaleFactor, scaleFactor, scaleFactor);
	std::shared_ptr<CDefaultMaterial> material;
	material = std::static_pointer_cast<CDefaultMaterial>(mEntity->material()); // 
	material->setWorldMatrix(scale);
	material->setViewMatrix(viewMatrix);
	material->setProjectionMatrix(projectionMatrix);
	
	mEntity->Update(aTimerState, aInputState);
}

void CEngine::Render()
{
	mRenderer->Render(mDirectX, { mEntity });
}

void CEngine::Finalize()
{
	mCamera.Finalize();
	mRenderer->Finalize();
	mDirectX->Finalize();
	mEntity->Finalize();

	mRenderer = nullptr;
	mDirectX = nullptr;
}
