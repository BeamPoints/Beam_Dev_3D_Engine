#include "../../Include/Engine/Camera.h"

 bool CCamera::Initialize(CameraParameters const & aParameters)
{
	mParameters = aParameters;
	updateProjectionMatrix();

	return true;
}

 bool CCamera::Update(CTimer::State const & aTimerState, CInput const & aInputState)
{
	 CEntity::Update(aTimerState, aInputState);
	updateViewMatrix();

	return false;
}

 void CCamera::updateViewMatrix()
{
	XMVECTOR const &position = this->transform().getTranslation();
	XMVECTOR const &forward = this->transform().getDirection();
	XMVECTOR const &up = this->transform().getUp();

	mViewMatrix = XMMatrixLookToLH(position, forward, up);
}

 void CCamera::updateProjectionMatrix()
{
	 float const &fovY = mParameters.fovY;
	 float const &aspect = mParameters.aspectRatio;
	 float const &minDepth = mParameters.minDepth;
	 float const &maxDepth = mParameters.maxDepth;

	 mProjectionMatrix = XMMatrixPerspectiveFovLH(fovY, aspect, minDepth, maxDepth);
}
