#pragma once
#include <memory>
#include "Renderer.h"
#include "Material.h"

class CEntity
{
public:
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

	inline std::shared_ptr<CMesh>     const &mesh()     const { return mMesh; }
	inline std::shared_ptr<CMaterial> const &material() const { return mMaterial; }

	inline void Update(CTimer::State const &aTimerState, CInput const &aInputState)
	{
		mMaterial->Update(aTimerState, aInputState);
	}

private:
	std::shared_ptr<CMesh>     mMesh;
	std::shared_ptr<CMaterial> mMaterial;
};

