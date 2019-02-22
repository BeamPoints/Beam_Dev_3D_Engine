#pragma once
#include <memory>
#include "Mesh.h"
#include "Material.h"
#include "../Engine/Transform.h"

class CEntity
{
public:
	inline CEntity()
		: mMesh(nullptr), mMaterial(nullptr)
	{}

	inline CEntity(std::shared_ptr<CMesh> aMesh,std::shared_ptr<CMaterial> aMaterial)
		: mMesh(aMesh) , mMaterial(aMaterial)
	{
		assert(nullptr != aMesh);
		assert(nullptr != aMaterial);
	}

	inline void Initialize()
	{

	}
	inline void Finalize()
	{
		mMaterial->Finalize();
	}

	CTransform &getTransform() { return mTransform; }

	inline std::shared_ptr<CMesh>     const &mesh()     const { return mMesh; }
	inline std::shared_ptr<CMaterial> const &material() const { return mMaterial; }

	virtual inline bool Update(CTimer::State const &aTimerState, CInput const &aInputState)
	{

		static constexpr float const sRotateByPerSecond = 150.0f; // Degrees
		static constexpr float const sMoveByPerSecond = 10.0f; // Units

		float const rotateBy = (sRotateByPerSecond * (float)aTimerState.elapesed);
		float const moveBy = (sMoveByPerSecond   * (float)aTimerState.elapesed);
		//static float rotation = 0;
		//rotation += rotateBy;

		//CONTROLLS
		//USE CAMERA		Ctrl + Y
		//USE CUBE			Ctrl + 1 | D1 not DNum1
		//USE Kick Stand	Ctrl + 2 | D2 not DNum2
		//USE Knife			Ctrl + 3 | D3 not DNum3

		//ASSETS BY Domenic Wamser
		// The Knife is wrong exported the controlls maybe Weared
		// The Kick Stand is wrong exported the controlls maybe Weared
		// The Cube Works Fine

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

	
		

		if (nullptr != mMaterial)
		{
			mMaterial->Update(aTimerState, aInputState);
		}
		return true;
	}

	CTransform &transform()
	{
		return mTransform;
	};

	

private:
	std::shared_ptr<CMesh>     mMesh;
	std::shared_ptr<CMaterial> mMaterial;
	CTransform mTransform;
};

