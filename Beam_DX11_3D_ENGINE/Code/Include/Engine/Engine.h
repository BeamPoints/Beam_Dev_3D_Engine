#pragma once
#include "../Rendering/Renderer.h"
#include "../Base/Input.h"
#include "../Base/Timer.h"
#include "Camera.h"
#include "DefaultMaterial.h"


class CEngine
{public:
	
	CEngine(std::shared_ptr<CRenderer> const &aRenderer,std::shared_ptr<CDXIntegration> const &aDirectX);

	void Initialize();
	void Update(CTimer::State const &aTimerState, CInput const &aInputState);
	void Render();
	void Finalize();

private:
	std::shared_ptr<CDXIntegration> mDirectX;
	std::shared_ptr<CRenderer> mRenderer;
	std::shared_ptr<CEntity> mEntity;
	
	CCamera mCamera;

};


