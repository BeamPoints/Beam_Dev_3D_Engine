#pragma once
#include "Entity.h"
#include "../Base/Input.h"
#include "../Base/Timer.h"

class CLight : public CEntity
{public:
	enum ELightType
	{
		Directional = 0,
		Point,
		Spot
	};

	struct SLightParameters
	{
		ELightType lightType;
		XMVECTOR   position;
		XMVECTOR   direction;
		XMVECTOR   color;
		float      intensity;
		float      distance;
		float      hotspotAngle;
		float      falloffAngle;
	};

	inline CLight() : CEntity(){}

	inline bool initialize(SLightParameters const &aParameters)
	{
		mParameters = aParameters;

		return true;
	}

	inline bool deinitialize()
	{
		return true;
	}

	inline bool update(CTimer::State const &aTimerState, CInput const &aInputState)
	{
		CEntity::Update(aTimerState, aInputState);

		mParameters.position = mTransform.getTranslation();
		mParameters.direction = mTransform.getDirection();

		return true;
	}

	inline SLightParameters const &getLightParameters() const
	{
		return mParameters;
	}

private:
	SLightParameters mParameters;
	CTransform        mTransform;
};

