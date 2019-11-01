
#ifndef _PICKING_BOX_H
#define _PICKING_BOX_H

#include "ZGMath.h"
#include "GameObject.h"

class ZG_API PickingCircle
{
public:
	PickingCircle() noexcept;
	PickingCircle(GameObject* obj_);
	~PickingCircle();

	//etc
	void Attach				(GameObject*& obj_);
	void Draw				(const zg::Color& color_ = CC_DARKOLIVEGREEN_1);
	bool CheckPointCollide	(const zg::Vector2& point_);
	bool CheckPointCollide	(const zg::Vector3& point_);
	void Selected			(bool select_);

	zg::Vector3 GetViewCoord(const zg::Matrix44& xform) {
		return xform * owner->transform.position;
	}
	GameObject* owner;
	float radius;
private:
	bool select;
};

#endif //_PICKING_BOX_H
