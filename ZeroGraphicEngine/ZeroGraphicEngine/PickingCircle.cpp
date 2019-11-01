
#include "stdafx.h"
#include "PickingCircle.h"

PickingCircle::PickingCircle() noexcept
	: owner{ nullptr }
	, radius(1.0f)
	, select(false)
{
}

PickingCircle::PickingCircle(GameObject* obj_)
	: owner{ obj_ }
	, radius(1.0f)
{}

PickingCircle::~PickingCircle(){}

void PickingCircle::Attach(GameObject*& obj_) {
	owner = obj_;
}
void PickingCircle::Draw(const zg::Color& color_) {
	//if (owner->transform.isBone)
	//	return;

	radius = MIN3(owner->transform.scale.x, owner->transform.scale.y, owner->transform.scale.z);
	Gizmo::Sphere(owner->transform.position, radius, color_);
}
bool PickingCircle::CheckPointCollide(const zg::Vector2& point_)
{
	//if (owner->transform.isBone)
	//  return false;
	return DistanceSq(owner->transform.position, point_) < radius * radius;
}
bool PickingCircle::CheckPointCollide(const zg::Vector3& point_)
{
	//if (owner->transform.isBone)
	//  return false;
	return DistanceSq(owner->transform.position, point_) < radius * radius;
}
void PickingCircle::Selected(bool select_) {
	select = select_;
}
