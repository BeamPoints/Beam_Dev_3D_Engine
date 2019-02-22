#include "../../Include/Engine/Transform.h"

void CTransform::PrintMatrixRow(XMVECTOR p)
{
	XMFLOAT4 c;
	XMStoreFloat4(&c, p);
}

void CTransform::PrintMatrix(XMMATRIX * m)
{
	for (int i = 0; i < 4; i++)
	{
		PrintMatrixRow(m->r[i]);
	}
}

void CTransform::setScale(const float & x, const float & y, const float & z)
{
	setScaleX(x);
	setScaleY(y);
	setScaleZ(z);
	invalidate();
}

 void CTransform::rotateAroundAxisBy(const XMVECTOR & axis, float angle)
{
	XMMATRIX rot = XMMatrixRotationAxis(axis, angle);
	m_rotation = XMMatrixMultiply(m_rotation, rot);
	invalidate();
}

 void CTransform::setRotation(const XMVECTOR & vec)
{
	m_rotation = XMMatrixIdentity();
	m_rotation *= XMMatrixRotationY(RAD(VEC_Y(vec)));
	m_rotation *= XMMatrixRotationX(RAD(VEC_X(vec)));
	m_rotation *= XMMatrixRotationZ(RAD(VEC_Z(vec)));

	invalidate();
}

 const XMVECTOR CTransform::getRotation()
{
	return 
	{
				DEG(atan2(VEC_X(m_rotation.r[1]),  VEC_X(m_rotation.r[0]))),
				DEG(atan2(-VEC_X(m_rotation.r[2]), sqrt(pow(VEC_Y(m_rotation.r[2]), 2) + pow(VEC_Z(m_rotation.r[2]),2)))),
				DEG(atan2(VEC_Y(m_rotation.r[2]),  VEC_Z(m_rotation.r[2]))),
				0.0f
	};
}

 void CTransform::setTranslation(const float & x, const float & y, const float & z)
{
	setTranslationX(x);
	setTranslationY(y);
	setTranslationZ(z);
	invalidate();
}

 void CTransform::worldMatrix(const XMMATRIX & parent, XMMATRIX * pCombined)
{
	m_composed = XMMatrixMultiply(m_local, parent);

	if (pCombined)
	{
		*pCombined = composedWorldMatrix();
	}
}

 XMMATRIX CTransform::composedWorldMatrix() const
{
	return m_composed;
}

 void CTransform::invalidate()
{
	static XMVECTOR rgt = { 1.0f, 0.0f, 0.0f, 0.0f };
	static XMVECTOR up = { 0.0f, 1.0f, 0.0f, 0.0f };
	static XMVECTOR fwd = { 0.0f, 0.0f, 1.0f, 0.0f };

	XMMATRIX T = XMMatrixTranslation(VEC_X(m_translation), VEC_Y(m_translation), VEC_Z(m_translation));
	XMMATRIX R = m_rotation;
	XMMATRIX S = XMMatrixScaling(VEC_X(m_scale), VEC_Y(m_scale), VEC_Z(m_scale));

	m_local = XMMatrixMultiply(XMMatrixMultiply(S, R), T);

	// Recalculate the axis vectors
	m_right = R.r[0];
	m_up = R.r[1];
	m_direction = R.r[2];
}
