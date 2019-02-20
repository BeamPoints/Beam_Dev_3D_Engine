#include "../../Include/Engine/Camera.h"

 bool CCamera::Initialize(CameraParameters const & aParameters)
{
	mParameters = aParameters;
	updateProjectionMatrix();

	return true;
}

 bool CCamera::Update(CTimer::State const & aTimerState, CInput const & aInputState)
{
	static constexpr float const sRotateByPerSecond = 15.0f; // Degrees
	static constexpr float const sMoveByPerSecond = 5.0f; // Units

	float const rotateBy = (sRotateByPerSecond * (float)aTimerState.elapesed);
	float const moveBy = (sMoveByPerSecond   * (float)aTimerState.elapesed);
	static float rotation = 0;
	rotation += rotateBy;

	//CONTROLLS
	// Move camera along axes:    WASD
	// Rotate camera around axes: Ctrl + WASDQE
	bool const isWPressed = aInputState.getPressed(KeyCode::W);
	bool const isAPressed = aInputState.getPressed(KeyCode::A);
	bool const isSPressed = aInputState.getPressed(KeyCode::S);
	bool const isDPressed = aInputState.getPressed(KeyCode::D);
	bool const isQPressed = aInputState.getPressed(KeyCode::Q);
	bool const isEPressed = aInputState.getPressed(KeyCode::E);
	bool const isCtrlWPressed = aInputState.getPressedWithCtrl(KeyCode::W);
	bool const isCtrlAPressed = aInputState.getPressedWithCtrl(KeyCode::A);
	bool const isCtrlSPressed = aInputState.getPressedWithCtrl(KeyCode::S);
	bool const isCtrlDPressed = aInputState.getPressedWithCtrl(KeyCode::D);
	bool const isCtrlQPressed = aInputState.getPressedWithCtrl(KeyCode::Q);
	bool const isCtrlEPressed = aInputState.getPressedWithCtrl(KeyCode::E);

	if (isCtrlWPressed)
	{
		mTransform.pitchBy(-rotateBy);
	}
	if (isCtrlSPressed)
	{
		mTransform.pitchBy(rotateBy);
	}
	if (isCtrlAPressed)
	{
		mTransform.yawBy(-rotateBy);
	}
	if (isCtrlDPressed)
	{
		mTransform.yawBy(rotateBy);
	}
	if (isCtrlQPressed)
	{
		mTransform.rollBy(rotateBy);
	}
	if (isCtrlEPressed)
	{
		mTransform.rollBy(-rotateBy);
	}

	if (isWPressed and not isCtrlWPressed)
	{
		mTransform.translateDirectionalBy(moveBy);
	}
	if (isSPressed and not isCtrlSPressed)
	{
		mTransform.translateDirectionalBy(-moveBy);
	}
	if (isAPressed and not isCtrlAPressed)
	{
		mTransform.translateLateralBy(-moveBy);
	}
	if (isDPressed and not isCtrlDPressed)
	{
		mTransform.translateLateralBy(moveBy);
	}
	if (isQPressed and not isCtrlQPressed)
	{
		mTransform.translateVerticalBy(moveBy);
	}
	if (isEPressed and not isCtrlEPressed)
	{
		mTransform.translateVerticalBy(-moveBy);
	}

	updateViewMatrix();

	return false;
}

 void CCamera::updateViewMatrix()
{
	XMVECTOR const &position = mTransform.getTranslation();
	XMVECTOR const &forward = mTransform.getDirection();
	XMVECTOR const &up = mTransform.getUp();

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
