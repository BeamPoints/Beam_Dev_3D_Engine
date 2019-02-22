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
	std::shared_ptr<CMesh> mesh2 = nullptr;
	std::shared_ptr<CMesh> mesh3 = nullptr;
	std::shared_ptr<CMaterial> material = nullptr;
	std::shared_ptr<CMaterial> material2 = nullptr;
	std::shared_ptr<CMaterial> material3 = nullptr;

	//mMesh = CMesh::CreateTriangle(aDirectX);
	mesh = CMesh::CreateMeshFromFile(mDirectX, "resources/meshes/Box_low.obj");
	mesh2 = CMesh::CreateMeshFromFile(mDirectX, "Resources/meshes/KickStand.fbx");
	mesh3 = CMesh::CreateMeshFromFile(mDirectX, "resources/meshes/Tanto-Knife.obj");

	CMaterial::MaterialInfo initInfo{ };
	initInfo.VSFilename = "standard_vs.cso";
	initInfo.PSFilename = "standard_ps.cso";

	material = std::make_shared<CDefaultMaterial>(); 
	material->Create(initInfo);
	material->Initialize(mDirectX);

	material2 = std::make_shared<CDefaultMaterial>();
	material2->Create(initInfo);
	material2->Initialize(mDirectX);

	material3 = std::make_shared<CDefaultMaterial>();
	material3->Create(initInfo);
	material3->Initialize(mDirectX);

	CDefaultMaterial::STextureCollection textures{ };
	textures.diffuse = "Resources/textures/low_default_AlbedoTransparency.png";
	textures.specular = "Resources/textures/tiedye.jpg";
	textures.normal = "Resources/textures/low_default_Normal.png";
	textures.reflectivity = "Resources/textures/tiedye.jpg";
	textures.reflectionMap = {
		"Resources/textures/sor_cwd/cwd_rt.jpg",
		"Resources/textures/sor_cwd/cwd_lf.jpg",
		"Resources/textures/sor_cwd/cwd_up.jpg",
		"Resources/textures/sor_cwd/cwd_dn.jpg",
		"Resources/textures/sor_cwd/cwd_ft.jpg",
		"Resources/textures/sor_cwd/cwd_bk.jpg",
	};
	CDefaultMaterial::STextureCollection textures2{ };
	textures2.diffuse = "Resources/textures/KickStand/KickStand_Diff.png";
	textures2.specular = "Resources/textures/KickStand/KickStand_Reflection.png";
	textures2.normal = "Resources/textures/KickStand/KickStand_N.png";
	textures2.reflectivity = "Resources/textures/KickStand/KickStand_Gloss.png";
	textures2.reflectionMap = textures.reflectionMap;

	CDefaultMaterial::STextureCollection textures3{ };
	textures3.diffuse = "Resources/textures/TantoKnife/Knife_Diff.png";
	textures3.specular = "Resources/textures/TantoKnife/Knife_Gloss.png";
	textures3.normal = "Resources/textures/TantoKnife/Knife_N.png";
	textures3.reflectivity = "Resources/textures/TantoKnife/Knife_Reflection.png";
	textures3.reflectionMap = textures.reflectionMap;

	std::shared_ptr <CDefaultMaterial> defaultMaterial;
	defaultMaterial = std::static_pointer_cast<CDefaultMaterial>(material);
	defaultMaterial->setTextures(mDirectX->myDevice(), textures);

	std::shared_ptr <CDefaultMaterial> defaultMaterial2;
	defaultMaterial2 = std::static_pointer_cast<CDefaultMaterial>(material2);
	defaultMaterial2->setTextures(mDirectX->myDevice(), textures2);

	std::shared_ptr <CDefaultMaterial> defaultMaterial3;
	defaultMaterial3 = std::static_pointer_cast<CDefaultMaterial>(material3);
	defaultMaterial3->setTextures(mDirectX->myDevice(), textures3);

	mEntity = std::make_shared<CEntity>(mesh, material);
	mEntity->getTransform().setTranslationX(10);
	mEntity->getTransform().setTranslationY(-2);
	mEntity->getTransform().setTranslationZ(20);
	mEntity->getTransform().setScale(0.02, 0.02, 0.02);
	mEntity->getTransform().setTranslationY(0);

	mEntity2 = std::make_shared<CEntity>(mesh2, material2);
	mEntity2->getTransform().setTranslationX(0);
	mEntity2->getTransform().setTranslationY(-8);
	mEntity2->getTransform().setTranslationZ(20);
	mEntity2->getTransform().setScale(0.5, 0.5, 0.5);
	mEntity2->getTransform().setRotationX(-90);

	mEntity3 = std::make_shared<CEntity>(mesh3, material3);
	mEntity3->getTransform().setTranslationX(0);
	mEntity3->getTransform().setTranslationY(1);
	mEntity3->getTransform().setTranslationZ(20);
	mEntity3->getTransform().setScale(0.2, 0.2, 0.2);
	mEntity3->getTransform().setRotationX(0);
	mEntity3->getTransform().setRotationY(0);
	mEntity3->getTransform().setRotationZ(90);

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
	static CEntity *inputReceiver = nullptr;
	if (aInputState.getPressedWithCtrl(KeyCode::Y))
	{
		inputReceiver = &mCamera;
	}
	else
	if (aInputState.getPressedWithCtrl(KeyCode::D1))
	{
		inputReceiver = &*mEntity;
	}
	else
	if (aInputState.getPressedWithCtrl(KeyCode::D2))
	{
		inputReceiver = &*mEntity2;
	}
	else
	if (aInputState.getPressedWithCtrl(KeyCode::D3))
	{
		inputReceiver = &*mEntity3;
	}
	else{}

	if (nullptr != inputReceiver)
	{
		inputReceiver->Update(aTimerState, aInputState);
	}


	mCamera.Update(aTimerState, {});

	XMMATRIX const &viewMatrix = mCamera.viewMatrix();
	XMMATRIX const &projectionMatrix = mCamera.projectionMatrix();

	mEntity->Update(aTimerState, {});
	mEntity->getTransform().worldMatrix(XMMatrixIdentity(), nullptr);
	mEntity2->Update(aTimerState, {});
	mEntity2->getTransform().worldMatrix(XMMatrixIdentity(), nullptr);
	mEntity3->Update(aTimerState, {});
	mEntity3->getTransform().worldMatrix(XMMatrixIdentity(), nullptr);

	mEntity->transform().worldMatrix(XMMatrixIdentity(), nullptr);
	
	std::shared_ptr<CDefaultMaterial> material = std::static_pointer_cast<CDefaultMaterial>(mEntity->material());
	material->setWorldMatrix(mEntity->transform().composedWorldMatrix());
	material->setViewMatrix(viewMatrix);
	material->setProjectionMatrix(projectionMatrix);

	mEntity2->transform().worldMatrix(XMMatrixIdentity(), nullptr);

	std::shared_ptr<CDefaultMaterial> material2 = std::static_pointer_cast<CDefaultMaterial>(mEntity2->material());
	material2->setWorldMatrix(mEntity->transform().composedWorldMatrix());
	material2->setViewMatrix(viewMatrix);
	material2->setProjectionMatrix(projectionMatrix);

	mEntity3->transform().worldMatrix(XMMatrixIdentity(), nullptr);

	std::shared_ptr<CDefaultMaterial> material3 = std::static_pointer_cast<CDefaultMaterial>(mEntity3->material());
	material3->setWorldMatrix(mEntity->transform().composedWorldMatrix());
	material3->setViewMatrix(viewMatrix);
	material3->setProjectionMatrix(projectionMatrix);
}

void CEngine::Render()
{
	mRenderer->Render(mDirectX, { mEntity , mEntity2, mEntity3 });
}

void CEngine::Finalize()
{
	mCamera.Finalize();
	mRenderer->Finalize();
	mDirectX->Finalize();
	mEntity->Finalize();
	mEntity2->Finalize();
	mEntity3->Finalize();

	mEntity3 = nullptr;
	mEntity2 = nullptr;
	mEntity = nullptr;
	mRenderer = nullptr;
	mDirectX = nullptr;

}
