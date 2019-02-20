#pragma once
#include <iostream>
#include "../DirectX/DXCommon.h"

#define DEG(rad) XMConvertToDegrees(rad)
#define RAD(deg) XMConvertToRadians(deg)

static void PrintMatrixRow(XMVECTOR p);
static void PrintMatrix(XMMATRIX *m);

class CTransform
{public:

	inline CTransform()
		: m_scale({ 1.0f, 1.0f, 1.0f })
		, m_rotation(XMMatrixIdentity())
		, m_translation({ 0.0f, 0.0f, 0.0f })
		, m_direction({ 0.0f, 0.0f, 1.0f })
		, m_right({ 1.0f, 0.0f, 0.0f })
		, m_up({ 0.0f, 1.0f, 0.0f })
		, m_local(XMMatrixIdentity())
		, m_composed(XMMatrixIdentity())
	{}

	inline const XMVECTOR& getDirection() const { return m_direction; };
	inline const XMVECTOR& getUp()        const { return m_up; };
	inline const XMVECTOR& getRight()     const { return m_right; };

	inline void setScaleX(const float& factor) { VEC_X(m_scale) = factor; invalidate(); }
	inline void setScaleY(const float& factor) { VEC_Y(m_scale) = factor; invalidate(); }
	inline void setScaleZ(const float& factor) { VEC_Z(m_scale) = factor; invalidate(); }
	inline void setScale(const float& x, const float& y, const float& z);
	inline void setScale(const XMVECTOR& vec) { m_scale = vec; invalidate(); }
	inline const XMVECTOR& getScale() const { return m_scale; }
	void rotateAroundAxisBy(const XMVECTOR& axis, float angle);

	inline void rotateXBy(const float& angle) { rotateAroundAxisBy({ 1.0f, 0.0f, 0.0f, 0.0f }, RAD(angle)); }
	inline void rotateYBy(const float& angle) { rotateAroundAxisBy({ 0.0f, 1.0f, 0.0f, 0.0f }, RAD(angle)); }
	inline void rotateZBy(const float& angle) { rotateAroundAxisBy({ 0.0f, 0.0f, 1.0f, 0.0f }, RAD(angle)); }

	inline void rollBy(const float& angle) { rotateAroundAxisBy(m_direction, RAD(angle)); }
	inline void pitchBy(const float& angle) { rotateAroundAxisBy(m_right, RAD(angle)); }
	inline void yawBy(const float& angle) { rotateAroundAxisBy(m_up, RAD(angle)); }

	inline void setRotationX(const float& angle) { setRotation(angle, 0, 0); }
	inline void setRotationY(const float& angle) { setRotation(0, angle, 0); }
	inline void setRotationZ(const float& angle) { setRotation(0, 0, angle); }
	inline void setRotation(const float& x, const float& y, const float& z) { setRotation({ x, y, z, 0.0f }); }
	inline void setRotation(const XMVECTOR& vec);
	const XMVECTOR getRotation();

	inline void translateXBy(const float& offset) { VEC_X(m_translation) += offset; invalidate(); }
	inline void translateYBy(const float& offset) { VEC_Y(m_translation) += offset; invalidate(); }
	inline void translateZBy(const float& offset) { VEC_Z(m_translation) += offset; invalidate(); }

	inline void translateDirectionalBy(const float& offset) { m_translation += (offset * m_direction); invalidate(); }
	inline void translateVerticalBy(const float& offset) { m_translation += (offset * m_up);        invalidate(); }
	inline void translateLateralBy(const float& offset) { m_translation += (offset * m_right);     invalidate(); }

	inline void setTranslationX(const float& offset) { VEC_X(m_translation) = offset; invalidate(); }
	inline void setTranslationY(const float& offset) { VEC_Y(m_translation) = offset; invalidate(); }
	inline void setTranslationZ(const float& offset) { VEC_Z(m_translation) = offset; invalidate(); }
	void setTranslation(const float& x, const float& y, const float& z);

	inline void setTranslation(const XMVECTOR& vec) { m_translation = vec; invalidate(); }
	inline const XMVECTOR& getTranslation() const { return m_translation; }
	inline void worldMatrix(const XMMATRIX& parent, XMMATRIX *pCombined);
	XMMATRIX composedWorldMatrix() const;

private:
	void invalidate();

	XMVECTOR m_scale;
	XMMATRIX m_rotation;
	XMVECTOR m_translation;

	XMVECTOR m_direction;
	XMVECTOR m_right;
	XMVECTOR m_up;

	XMMATRIX m_local;
	XMMATRIX m_composed;
};

