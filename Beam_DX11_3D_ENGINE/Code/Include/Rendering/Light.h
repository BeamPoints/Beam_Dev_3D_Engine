#pragma once
#include "../DirectX/DXCommon.h"
#include "Entity.h"
#include "../Base/Input.h"
#include "../Base/Timer.h"
/*			WIRD VERWORFEN WEGEN ZU VIELEN BUGGS
class CLight : public CEntity
{public:
	enum ELightType
	{
		Directional = 0,
		Point = 1,
		Spot = 2,
	};

	struct SLightParameters
	{
		ELightType lightType;
		XMVECTOR   position;
		XMVECTOR   direction;
		XMVECTOR   color;
		XMVECTOR   idhf;
		XMVECTOR   falloffCoefficients;
		XMVECTOR   lp3;
		float      intensity;
		float      distance;
		float      hotspotAngle;
		float      falloffAngle;
	};

	inline CLight() : CEntity(){}

	inline bool Initialize(SLightParameters const &aParameters)
	{
		mParameters = aParameters;

		return true;
	}

	inline bool Finalize()
	{
		return true;
	}

	inline bool Update(CTimer::State const &aTimerState, CInput const &aInputState)
	{
		CEntity::Update(aTimerState, aInputState);

		mParameters.position = transform().getTranslation();
		mParameters.direction = transform().getDirection();

		return true;
	}

	inline SLightParameters const &getLightParameters() const
	{
		return mParameters;
	}

private:
	SLightParameters mParameters;
};
*/
