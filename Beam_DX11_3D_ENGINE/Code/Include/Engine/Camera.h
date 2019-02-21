#pragma once
#include "../DirectX/DXCommon.h"
#include "../Base/Input.h"
#include "../Base/Timer.h"
#include "Transform.h"
#include "../Rendering/Entity.h"

class CCamera : public CEntity
{public:

	enum ECameraProjectionType
	{
		Perspective = 0,
		Orthogonal
	};

	struct CameraParameters
	{
		float minDepth;    // nearplane
		float maxDepth;    // farplane
		float fovY;
		float aspectRatio;

		ECameraProjectionType projectionType;
	};

	using CEntity::CEntity;

	inline CCamera(): CEntity(){}

    bool Initialize(CameraParameters const &aParameters);
    bool Update(CTimer::State const &aTimerState, CInput const &aInputState);
    bool Finalize(){return true;}

	inline XMMATRIX const &viewMatrix()       const { return mViewMatrix; }
	inline XMMATRIX const &projectionMatrix() const { return mProjectionMatrix; }

private:

    void updateViewMatrix();
    void updateProjectionMatrix();

	CameraParameters mParameters;
	XMMATRIX         mViewMatrix;
	XMMATRIX         mProjectionMatrix;
};

