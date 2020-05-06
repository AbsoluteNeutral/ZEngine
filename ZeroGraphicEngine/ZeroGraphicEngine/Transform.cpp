
#include "stdafx.h"
#include "Transform.h"
#if defined(_DEBUG)
	#include "Logging.h"
#endif


#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif

Transform Transform::Identity{};

Transform::Transform() noexcept
	: position{}
	, localPosition{}
	, scale{ 1.0f, 1.0f, 1.0f } 
	, localScale{ 1.0f, 1.0f, 1.0f }
	, rotation{}
	, localRotation{}
	, flags(0)
	, facing{ 0.0f, 0.0f, 1.0f }
	, parent{ nullptr }
	, worldMatrix{}
{
	worldMatrix.SetIdentity();
}
Transform::Transform(const zg::Vector3& position_, const zg::Quaternion& quaternion_, const zg::Vector3& scale_) noexcept
	: position{ position_ }
	, localPosition{}
	, scale{ scale_ }
	, localScale{ 1.0f, 1.0f, 1.0f }
	, rotation{ quaternion_ }
	, localRotation{}
	, flags(0)
	, facing{ 0.0f, 0.0f, 1.0f }
	, parent{ nullptr }
	, worldMatrix{}
{}

//____________________________________________________________ setters
void Transform::SetFacingVector(const zg::Vector3& zfacing) 
{
	facing = zfacing;
}
void Transform::SetAsBoneObject(bool isBoneObject_) 
{
	flags = isBoneObject_ ? (flags | T_ISBONE) : (flags & ~T_ISBONE);
}
void Transform::SetStatic(bool static_)
{
	flags = static_ ? (flags | T_STATIC) : (flags & ~T_STATIC);
	worldMatrix = BuildTRS4x4(position, rotation, scale);
}
void Transform::SetWorldFromMatrix(const zg::Matrix44& matrix_)
{
	worldMatrix = matrix_;
	position	= zg::GetPosition(matrix_);
	rotation	= zg::GetRotation(matrix_);
	scale		= zg::GetScale(matrix_);
}
void Transform::SetLocalFromMatrix(const zg::Matrix44& matrix_)
{
	localPosition	= zg::GetPosition(matrix_);
	localRotation	= zg::GetRotation(matrix_);
	localScale		= zg::GetScale(matrix_);
	flags |= T_DIRTY;
}

//____________________________________________________________ World
void Transform::SetPosition(float x_) {
	position.x = x_;
	flags |= T_DIRTY;
}
void Transform::SetPosition(float x_, float y_) {
	position.x = x_;
	position.y = y_;
	flags |= T_DIRTY;
}
void Transform::SetPosition(float x_, float y_, float z_) {
	position.x = x_;
	position.y = y_;
	position.z = z_;
	flags |= T_DIRTY;
}
void Transform::SetPosition(const zg::Vector3& position_) {
	position.x = position_.x;
	position.y = position_.y;
	position.z = position_.z;
	flags |= T_DIRTY;
}
void Transform::SetRotation(float x_) {
	zg::Vector3 eular{ zg::ToEularDegree(rotation) };
	rotation.SetFromEulerAngles(zg::Vector3{ x_, eular.y, eular.z });
	flags |= T_DIRTY;
}
void Transform::SetRotation(float x_, float y_) {
	rotation.SetFromEulerAngles(zg::Vector3{ x_, y_, zg::ToEularDegree(rotation).z });
	flags |= T_DIRTY;
}
void Transform::SetRotation(float x_, float y_, float z_) {
	rotation.SetFromEulerAngles(zg::Vector3{ x_, y_, z_ });
	flags |= T_DIRTY;
}
void Transform::SetRotation(const zg::Vector3& eular_) {
	rotation.SetFromEulerAngles(eular_);
	flags |= T_DIRTY;
}
void Transform::SetRotation(const zg::Quaternion& rotation_) {
	rotation = rotation_;
	flags |= T_DIRTY;
}
void Transform::SetScale(float x_) {
	scale.x = x_;
	flags |= T_DIRTY;
}
void Transform::SetScale(float x_, float y_) {
	scale.x = x_;
	scale.y = y_;
	flags |= T_DIRTY;
}
void Transform::SetScale(float x_, float y_, float z_) {
	scale.x = x_;
	scale.y = y_;
	scale.z = z_;
	flags |= T_DIRTY;
}
void Transform::SetScale(const zg::Vector3& scale_) {
	scale = scale_;
	flags |= T_DIRTY;
}
void Transform::SetUniformWorldScale2D(float xy_) {
	scale.y = scale.x = xy_;
	flags |= T_DIRTY;
}
void Transform::SetUniformWorldScale3D(float xyz_) {
	scale.z = scale.y = scale.x = xyz_;
	flags |= T_DIRTY;
}

//______________________________________________________________________ Local
void Transform::SetLocalPosition(float x_) {
	localPosition.x = x_;
	flags |= T_DIRTY | T_YET_TOUPDATE;
}
void Transform::SetLocalPosition(float x_, float y_) {
	localPosition.x = x_;
	localPosition.y = y_;
	flags |= T_DIRTY | T_YET_TOUPDATE;
}
void Transform::SetLocalPosition(float x_, float y_, float z_) {
	localPosition.x = x_;
	localPosition.y = y_;
	localPosition.z = z_;
	flags |= T_DIRTY | T_YET_TOUPDATE;
}
void Transform::SetLocalPosition(const zg::Vector3& position_) {
	localPosition.x = position_.x;
	localPosition.y = position_.y;
	localPosition.z = position_.z;
	flags |= T_DIRTY | T_YET_TOUPDATE;
}
void Transform::SetLocalRotation(float x_) {
	zg::Vector3 eular{ zg::ToEularDegree(localRotation) };
	localRotation.SetFromEulerAngles(zg::Vector3{ x_, eular.y, eular.z });
	flags |= T_DIRTY | T_YET_TOUPDATE;
}
void Transform::SetLocalRotation(float x_, float y_) {
	localRotation.SetFromEulerAngles(zg::Vector3{ x_, y_, zg::ToEularDegree(localRotation).z });
	flags |= T_DIRTY | T_YET_TOUPDATE;
}
void Transform::SetLocalRotation(float x_, float y_, float z_) {
	localRotation.SetFromEulerAngles(zg::Vector3{ x_, y_, z_ });
	flags |= T_DIRTY | T_YET_TOUPDATE;
}
void Transform::SetLocalRotation(const zg::Vector3& eular_) {
	localRotation.SetFromEulerAngles(eular_);
	flags |= T_DIRTY | T_YET_TOUPDATE;
}
void Transform::SetLocalRotation(const zg::Quaternion& rotation_) {
	localRotation = rotation_;
	flags |= T_DIRTY | T_YET_TOUPDATE;
}
void Transform::SetLocalScale(float x_) {
	localScale.x = x_;
	flags |= T_DIRTY | T_YET_TOUPDATE;
}
void Transform::SetLocalScale(float x_, float y_) {
	localScale.x = x_;
	localScale.y = y_;
	flags |= T_DIRTY | T_YET_TOUPDATE;
}
void Transform::SetLocalScale(float x_, float y_, float z_) {
	localScale.x = x_;
	localScale.y = y_;
	localScale.z = z_;
	flags |= T_DIRTY | T_YET_TOUPDATE;
}
void Transform::SetLocalScale(const zg::Vector3& scale_) {
	localScale = scale_;
	flags |= T_DIRTY | T_YET_TOUPDATE;
}
void Transform::SetUniformLocalScale2D(float xy_) {
	localScale.y = localScale.x = xy_;
	flags |= T_DIRTY | T_YET_TOUPDATE;
}
void Transform::SetUniformLocalScale3D(float xyz_) {
	localScale.z = localScale.y = localScale.x = xyz_;
	flags |= T_DIRTY | T_YET_TOUPDATE;
}

//____________________________________________________________ World
void Transform::Translate(float x_, float y_, float z_) {
	position.x += x_;
	position.y += y_;
	position.z += z_;
	flags |= T_DIRTY;
}
void Transform::Translate(const zg::Vector3& translate_) {
	position += translate_;
	flags |= T_DIRTY;
}
void Transform::Rotate(float x_, float y_, float z_) {
	rotation *= zg::ToQuaternion(zg::Vector3{ x_, y_, z_ });
	flags |= T_DIRTY;
}
void Transform::Rotate(const zg::Vector3& eular_) {
	rotation *= zg::ToQuaternion(eular_);
	flags |= T_DIRTY;
}
void Transform::Rotate(const zg::Quaternion& rotation_) {
	rotation *= rotation_;
	flags |= T_DIRTY;
}
void Transform::Scale(float x_, float y_, float z_) {
	scale.x += x_;
	scale.y += y_;
	scale.z += z_;
	flags |= T_DIRTY;
}
void Transform::Scale(const zg::Vector3& scaling_) {
	scale.x += scaling_.x;
	scale.y += scaling_.y;
	scale.z += scaling_.z;
	flags |= T_DIRTY;
}
void Transform::ScaleMultiply(float x_, float y_, float z_) {
	scale.x *= x_;
	scale.y *= y_;
	scale.z *= z_;
	flags |= T_DIRTY;
}
void Transform::ScaleMultiply(const zg::Vector3& scaling_) {
	scale.x *= scaling_.x;
	scale.y *= scaling_.y;
	scale.z *= scaling_.z;
	flags |= T_DIRTY;
}

//______________________________________________________________________ Local
void Transform::LocalTranslate(float x_, float y_, float z_) {
	localPosition.x += x_;
	localPosition.y += y_;
	localPosition.z += z_;
	flags |= T_DIRTY | T_YET_TOUPDATE;
}
void Transform::LocalTranslate(const zg::Vector3& translate_) {
	localPosition += translate_;
	flags |= T_DIRTY | T_YET_TOUPDATE;
}
void Transform::LocalRotate(float x_, float y_, float z_) {
	localRotation *= zg::ToQuaternion(zg::Vector3{ x_, y_, z_ });
	flags |= T_DIRTY | T_YET_TOUPDATE;
}
void Transform::LocalRotate(const zg::Vector3& eular__) {
	localRotation *= zg::ToQuaternion(eular__);
	flags |= T_DIRTY | T_YET_TOUPDATE;
}
void Transform::LocalRotate(const zg::Quaternion& rotation_) {
	localRotation *= rotation_;
	flags |= T_DIRTY | T_YET_TOUPDATE;
}
void Transform::LocalScale(float x_, float y_, float z_) {
	localScale.x += x_;
	localScale.y += y_;
	localScale.z += z_;
	flags |= T_DIRTY | T_YET_TOUPDATE;
}
void Transform::LocalScale(const zg::Vector3& scaling_) {
	localScale.x += scaling_.x;
	localScale.y += scaling_.y;
	localScale.z += scaling_.z;
	flags |= T_DIRTY | T_YET_TOUPDATE;
}
void Transform::LocalScaleMultiply(float x_, float y_, float z_) {
	localScale.x *= x_;
	localScale.y *= y_;
	localScale.z *= z_;
	flags |= T_DIRTY | T_YET_TOUPDATE;
}
void Transform::LocalScaleMultiply(const zg::Vector3& scaling_) {
	localScale.x *= scaling_.x;
	localScale.y *= scaling_.y;
	localScale.z *= scaling_.z;
	flags |= T_DIRTY | T_YET_TOUPDATE;
}

//______________________________________________________________________ getters
Transform Transform::GetConcatTransform() const
{
	if (parent)
	{
		return ConcatLocalVQS(parent->GetConcatTransform(), *this);
	}
	return *this;
}

Transform Transform::GetInverse() const {
	const zg::Quaternion& invQuaternion = rotation.GetInverted();
	return Transform(invQuaternion * (-position), invQuaternion, zg::Vector3::One);
}

const zg::Matrix44& Transform::GetMatrix() {
	
	//if flag is dirty, concat the latest matrix
	if (flags & T_DIRTY) 
	{
		//should only be called by a transform with no parent
		worldMatrix = BuildTRS4x4(position, rotation, scale);
		//turn bit off
		flags &= ~T_DIRTY;
	}
	return worldMatrix;
}

zg::Matrix44 Transform::GetLocalMatrix() const 
{ 
	return BuildTRS4x4(localPosition, localRotation, localScale);
}
const zg::Matrix44&	Transform::GetWorldMatrix	() const { return worldMatrix; }
bool				Transform::HasUpdated		() const { return !(flags & T_DIRTY); }
bool				Transform::IsBone			() const { return (flags & T_ISBONE); }
bool				Transform::IsStatic			() const { return (flags & T_STATIC); }
zg::Vector3			Transform::Forward			() const { return rotation.GetRotatedVector(facing); }
zg::Vector3			Transform::Up				() const { return rotation.GetRotatedVector(zg::Vector3::Vector3Y); }
zg::Vector3			Transform::Right			() const { return Forward().Cross(zg::Vector3::Vector3Y); } //rotation.GetRotatedVector(zg::Vector3::Vector3X); }

// ______________________________________________________________________ etc function
void Transform::Attach(Transform* parent_) {
	//if parent is nullptr
	//if parent is itself
	//if parent is same
	if (!parent_ || parent_ == this || parent == parent_)
	{
#if defined(_DEBUG)
		Warn("Transform.cpp No Parent to be attached");
#endif
		return;
	}

	if (parent)
	{
		//delete child from old parent
		auto b = parent->child.begin();
		auto e = parent->child.end();
		std::vector<Transform*>::iterator itrToRemove = std::find(b, e, this);
		parent->child.erase(itrToRemove);
	}

	//assign to the new parent
	parent = parent_;
	zg::Vector3 rec = (1.0f / parent->scale);
	zg::Quaternion con = parent->rotation.GetConjugated();

	//localPosition = (con * (rec * position)) + (con * (rec * -parent->position)) ; //zg::GaussJordanInverse(parent->GetMatrix()) * position;
	localPosition = zg::GaussJordanInverse(parent->GetMatrix()) * position;
	localScale		= rec * scale;
	localRotation	= con * rotation;

	//attach child
	parent->child.push_back(this);

	SignalChildDirtyBit();
	flags |= T_DIRTY | T_YET_TOUPDATE;
}
void Transform::Detach() {
	if (!parent) return;

	position = ((parent->rotation * (parent->scale * localPosition))) + parent->position;
	rotation = parent->rotation * localRotation;
	scale	 = parent->scale * localScale;

	localPosition.SetZero();
	localScale		= zg::Vector3::One;
	localRotation	= zg::Quaternion{};

	//remove child
	auto b = parent->child.begin();
	auto e = parent->child.end();
	std::vector<Transform*>::iterator itrToRemove = std::find(b, e, this);
	parent->child.erase(itrToRemove);

	parent = nullptr;

	SignalChildDirtyBit();
	flags |= T_DIRTY;
}

void Transform::CameraLookAt(const Transform& target_)
{
	CameraLookAt(target_.position);
}
void Transform::CameraLookAt(const zg::Vector3& target_position_)
{
	rotation = zg::LookRotation(position - target_position_, Up(), facing);
	if (parent) {
		localRotation = rotation;
	}
	flags |= T_DIRTY;
}
void Transform::LookAt(const Transform& target_) {
	LookAt(target_.position);
}
void Transform::LookAt(const zg::Vector3& target_) 
{
	rotation = zg::LookRotation(target_ - position, zg::Vector3::Vector3Y, facing);
	if (parent) 
	{
		//zg::Quaternion con = parent->rotation.GetConjugated();
		//localRotation = con * rotation;

		localRotation = rotation;
	}
	flags |= T_DIRTY;
}
void Transform::LookAt(const zg::Vector3& target_, const zg::Vector3& facing_, const zg::Vector3& up_)
{
	zg::Quaternion getRot = zg::LookRotation(target_ - position, up_, facing_);
	if (parent)
	{
		//Detach();
		//rotation = getRot;
		//Attach(parent);

		//zg::Quaternion con = parent->rotation.GetConjugated();
		//localRotation = con * rotation;
		localRotation = rotation;
	}
	else
		rotation = getRot;

	flags |= T_DIRTY;
}
void Transform::Reset() {
	if (parent) 
	{
		localPosition	= zg::Vector3{};
		localScale		= zg::Vector3::One;
		localRotation	= zg::Quaternion{};
		flags |= T_DIRTY | T_YET_TOUPDATE;
	}
	else 
	{
		position	= zg::Vector3{};
		scale		= zg::Vector3::One;
		rotation	= zg::Quaternion{};
		worldMatrix.SetIdentity();
		flags |= T_DIRTY;
	}
}
void Transform::ResetFlag() 
{
	flags &= ~T_YET_TOUPDATE;
	
	if (parent) 
	{
		zg::Vector3 rec = (1.0f / parent->scale);
		zg::Quaternion con = parent->rotation.GetConjugated();

		localPosition	= zg::GaussJordanInverse(parent->GetMatrix()) * position;
		//localPosition	= (con * (rec * position)) + (con * (rec * -parent->position)); //zg::GaussJordanInverse(parent->GetMatrix()) * position;
		localScale		= rec * scale;
		localRotation	= con * rotation;

		SignalChildDirtyBit();
	}
}
void Transform::SetUpdateFlag()
{
	flags |= T_DIRTY | T_YET_TOUPDATE;
}

void Transform::RotateTowards(const Transform& target, float rate)
{
	RotateTowards(target.position, rate);
}
void Transform::RotateTowards(const zg::Vector3& target, float rate)
{
	if (parent)
	{
		auto targetPos = target;

		auto tRot = zg::LookRotation(targetPos - localPosition, zg::Vector3::Vector3Y, Forward());

		auto dot = localRotation.Dot(tRot);

		if (std::abs(dot) > 1.f - EPSILON)
			dot = 0.f;
		else
			dot = TO_DEG(acos(std::min(abs(dot), 1.f)) * 2.f);

		if (dot == 0.f)
			SetLocalRotation(tRot);
		else
			SetLocalRotation(zg::Slerp(localRotation, tRot, std::min(1.f, rate / dot)));
	}
	else
	{
		auto targetPos = target;

		auto tRot = zg::LookRotation(targetPos - position, zg::Vector3::Vector3Y, Forward());

		auto dot = rotation.Dot(tRot);

		if (std::abs(dot) > 1.f - EPSILON)
			dot = 0.f;
		else
			dot = TO_DEG(acos(std::min(abs(dot), 1.f)) * 2.f);

		if (dot == 0.f)
			SetRotation(tRot);
		else
			SetRotation(zg::Slerp(rotation, tRot, std::min(1.f, rate / dot)));
	}
}
void Transform::RotateTowardsIgnoreHeight(const Transform & target, float rate)
{
	RotateTowardsIgnoreHeight(target.position, rate);
}
void Transform::RotateTowardsIgnoreHeight(const zg::Vector3 & target, float rate)
{
	auto targetPos = target;
	targetPos.y = position.y;

	auto tRot = zg::LookRotation(targetPos - position, zg::Vector3::Vector3Y, Forward());

	auto dot = rotation.Dot(tRot);

	if (std::abs(dot) > 1.f - EPSILON)
		dot = 0.f;
	else
		dot = TO_DEG(acos(std::min(abs(dot), 1.f)) * 2.f);

	if (dot == 0.f)
		SetRotation(tRot);
	else
		SetRotation(zg::Slerp(rotation, tRot, std::min(1.f, rate / dot)));
}
void Transform::Slerp(const zg::Quaternion& q, float deltatime_) {
	rotation = zg::Slerp(rotation, q, deltatime_);
	flags |= T_DIRTY;
}

void Transform::ForceUpdate()
{
	if (!parent)
		return;

	//if parent have yet to update, update upwards
	if ((parent->flags & T_YET_TOUPDATE))
		parent->Update();

	//VQS implmentation, update using latest value
	position = ((parent->rotation * (parent->scale * localPosition))) + parent->position;
	rotation = parent->rotation * localRotation;
	scale	 = parent->scale * localScale;

	//updated, reset flag
	//concat matrix
	worldMatrix = BuildTRS4x4(position, rotation, scale);
	flags &= ~T_DIRTY;			//reset dirty bit
	flags &= ~T_YET_TOUPDATE;	//reset T_YET_TOUPDATE bit
}
void Transform::Update() 
{
	//if is static
	if ((flags & T_STATIC))
	{
		flags &= ~T_DIRTY;			//reset dirty bit
		flags &= ~T_YET_TOUPDATE;	//reset T_YET_TOUPDATE bit
		return;
	}
	
	//if no parent 
	if (!parent)
	{
		//but flag bit is dirty
		//signal all child to dirty bit 
		if ((flags & T_DIRTY))
			SignalChildDirtyBit();
		return;
	}

	//reaching here means there is a parent
	if( (flags & T_DIRTY) || (flags & T_YET_TOUPDATE) )
	{
		//signal all child to dirty bit 
		//Update
		SignalChildDirtyBit();	
		ForceUpdate();
	}
}

zg::Vector3 Transform::GetWorldPosition() const {
	if (parent)
		return ((parent->GetWorldRotation() * (parent->GetWorldScale() * localPosition)) ) + parent->GetWorldPosition();
	return position;
}
zg::Quaternion Transform::GetWorldRotation() const {
	if (parent) 
		return parent->GetWorldRotation() * localRotation;
	return rotation;
}
zg::Vector3 Transform::GetWorldScale() const {
	if (parent) 
		return parent->GetWorldScale() * localScale;
	return scale;
}


void Transform::dump() const {
	zg::Vector3 eular = zg::ToEularDegree(rotation);
	zg::Vector3 localEular = zg::ToEularDegree(localRotation);
	printf("worldT: %15.6f %15.6f %15.6f \nworldR: %15.6f %15.6f %15.6f \n\
worldS: %15.6f %15.6f %15.6f \n\
localT: %15.6f %15.6f %15.6f \n\
localR: %15.6f %15.6f %15.6f \n\
localS: %15.6f %15.6f %15.6f \n\n"
		, position.x, position.y, position.z
		, eular.x, eular.y, eular.z
		, scale.x, scale.y, scale.z
		, localPosition.x, localPosition.y, localPosition.z
		, localEular.x, localEular.y, localEular.z
		, localScale.x, localScale.y, localScale.z
	);
}

void Transform::SignalChildDirtyBit()
{
	for (auto& elem : child)
	{
		elem->flags |= T_YET_TOUPDATE;
		elem->SignalChildDirtyBit();
	}
}

#ifdef TINYXML2_EXPORT
// ____________________________________________________________ serial
void Transform::Serial(FXMLDocument* doc, FXMLNode* root){
	FXMLElement* pElem = doc->NewElement(IS_TRANSFORM);

	if (parent) 
	{
		position = ((parent->rotation * (parent->scale * localPosition))) + parent->position;
		rotation = parent->rotation * localRotation;
		scale = parent->scale * localScale;
	}

	FXMLElement* pSubElem = doc->NewElement("position");
	pSubElem->SetAttribute("x", position.x);
	pSubElem->SetAttribute("y", position.y);
	pSubElem->SetAttribute("z", position.z);
	pElem->InsertEndChild(pSubElem);

	pSubElem = doc->NewElement("scale");
	pSubElem->SetAttribute("x", scale.x);
	pSubElem->SetAttribute("y", scale.y);
	pSubElem->SetAttribute("z", scale.z);
	pElem->InsertEndChild(pSubElem);
	
	zg::Vector3 getEuler = rotation.ToEularDegree();
	pSubElem = doc->NewElement("rotation");
	pSubElem->SetAttribute("x", getEuler.x);
	pSubElem->SetAttribute("y", getEuler.y);
	pSubElem->SetAttribute("z", getEuler.z);
	pElem->InsertEndChild(pSubElem);

	pSubElem = doc->NewElement("misc");
	pSubElem->SetAttribute("isStatic", (flags & T_STATIC));
	pSubElem->SetAttribute("isBone", (flags & T_ISBONE));
	pElem->InsertEndChild(pSubElem);

	root->InsertEndChild(pElem);
}
void Transform::Unserial(FXMLElement* pElem){
	FXMLElement* pSubElem = pElem->FirstChildElement("position");
	position.x = pSubElem->FloatAttribute("x");
	position.y = pSubElem->FloatAttribute("y");
	position.z = pSubElem->FloatAttribute("z");

	pSubElem = pSubElem->NextSiblingElement();
	scale.x = pSubElem->FloatAttribute("x");
	scale.y = pSubElem->FloatAttribute("y");
	scale.z = pSubElem->FloatAttribute("z");

	zg::Vector3 getEuler;
	pSubElem = pSubElem->NextSiblingElement();
	getEuler.x = pSubElem->FloatAttribute("x");
	getEuler.y = pSubElem->FloatAttribute("y");
	getEuler.z = pSubElem->FloatAttribute("z");
	rotation = zg::ToQuaternion(getEuler);

	pSubElem = pSubElem->NextSiblingElement();

	bool getBool = pSubElem->BoolAttribute("isStatic");
	SetStatic(getBool);
	getBool = pSubElem->BoolAttribute("isBone");
	SetAsBoneObject(getBool);
}

#endif

// ____________________________________________________________ friend
Transform ConcatVQS(const Transform& t2_, const Transform& t1_) {
	return Transform{
		t2_.rotation * (t2_.scale * t1_.position) + t2_.position,
		t2_.rotation * t1_.rotation,
		t2_.scale * t1_.scale
	};
}
Transform ConcatLocalVQS(const Transform& t2_, const Transform& t1_) {
	return Transform{
		t2_.rotation * (t2_.scale * t1_.localPosition) + t2_.position,
		t2_.rotation * t1_.localRotation,
		t2_.scale * t1_.localScale
	};
	//VQM
	//zg::Quaternion q1{ t1_.localRotation };
	//zg::Quaternion q1c{ t1_.localRotation.GetConjugated() };
	//return Transform{
	//	t2_.rotation * (t2_.scale * t1_.localPosition) * t2_.rotation.GetConjugated(),
	//	t2_.rotation * q1,
	//	(q1c * t2_.scale) * (q1 * t1_.localScale)
	//};
}

// ____________________________________________________________ extern function
Transform InterpolateTransforms(const Transform& oldTransform,
	const Transform& newTransform,
	float factor) {

	zg::Vector3 interPosition = oldTransform.position * (float(1.0) - factor) +
		newTransform.position * factor;
	zg::Quaternion interOrientation = zg::Slerp(oldTransform.rotation,
		newTransform.rotation, factor);
	return Transform(interPosition, interOrientation, zg::Vector3::One);
}
void RotateTowards(Transform& transform_, const Transform& target, float rate)
{
	transform_.RotateTowards(target, rate);
}
void RotateTowards(Transform& transform_, const zg::Vector3& target, float rate)
{
	transform_.RotateTowards(target, rate);
}

#ifdef USING_SOL2
void Transform::BindLua(sol::state & lua)
{
	lua.new_usertype<Transform>("Transform",
		sol::constructors <Transform(), Transform(const zg::Vector3&, const zg::Vector3&, const zg::Vector3&)>(),

		"Attach", &Transform::Attach,
		"Detach", &Transform::Detach,
		"SetStatic", &Transform::SetStatic,
		"HasUpdated", &Transform::HasUpdated,
		"GetMatrix", &Transform::GetMatrix,
		"GetLocalMatrix", &Transform::GetLocalMatrix,
		"GetWorldMatrix", &Transform::GetWorldMatrix,

		"LookAt", sol::overload(
			static_cast<void (Transform::*)(const Transform&)>(&Transform::LookAt),
			static_cast<void (Transform::*)(const zg::Vector3&)>(&Transform::LookAt)
		),

		"RotateTowards", sol::overload(
			static_cast<void (Transform::*)(const Transform&, float)>(&Transform::RotateTowards),
			static_cast<void (Transform::*)(const zg::Vector3&, float)>(&Transform::RotateTowards)
		),

		"RotateTowardsIgnoreHeight", sol::overload(
			static_cast<void (Transform::*)(const Transform&, float)>(&Transform::RotateTowardsIgnoreHeight),
			static_cast<void (Transform::*)(const zg::Vector3&, float)>(&Transform::RotateTowardsIgnoreHeight)
		),

		"Slerp", &Transform::Slerp,

		"Forward", &Transform::Forward,
		"Up", &Transform::Up,
		"Right", &Transform::Right,

		"SetPosition", sol::overload(
			static_cast<void (Transform::*)(float)>(&Transform::SetPosition),
			static_cast<void (Transform::*)(float, float)>(&Transform::SetPosition),
			static_cast<void (Transform::*)(float, float, float)>(&Transform::SetPosition),
			static_cast<void (Transform::*)(const zg::Vector3&)>(&Transform::SetPosition)
		),
		"SetRotation", sol::overload(
			static_cast<void (Transform::*)(float)>(&Transform::SetRotation),
			static_cast<void (Transform::*)(float, float)>(&Transform::SetRotation),
			static_cast<void (Transform::*)(float, float, float)>(&Transform::SetRotation),
			static_cast<void (Transform::*)(const zg::Vector3&)>(&Transform::SetRotation),
			static_cast<void (Transform::*)(const zg::Quaternion&)>(&Transform::SetRotation)
		),
		"SetScale", sol::overload(
			static_cast<void (Transform::*)(float)>(&Transform::SetScale),
			static_cast<void (Transform::*)(float, float)>(&Transform::SetScale),
			static_cast<void (Transform::*)(float, float, float)>(&Transform::SetScale),
			static_cast<void (Transform::*)(const zg::Vector3&)>(&Transform::SetScale)
		),
		"Translate", sol::overload(
			static_cast<void (Transform::*)(float)>(&Transform::Translate),
			static_cast<void (Transform::*)(float, float)>(&Transform::Translate),
			static_cast<void (Transform::*)(float, float, float)>(&Transform::Translate),
			static_cast<void (Transform::*)(const zg::Vector3&)>(&Transform::Translate)
		),
		"Rotate", sol::overload(
			static_cast<void (Transform::*)(float)>(&Transform::Rotate),
			static_cast<void (Transform::*)(float, float)>(&Transform::Rotate),
			static_cast<void (Transform::*)(float, float, float)>(&Transform::Rotate),
			static_cast<void (Transform::*)(const zg::Vector3&)>(&Transform::Rotate)
		),
		"Scale", sol::overload(
			static_cast<void (Transform::*)(float, float, float)>(&Transform::Scale),
			static_cast<void (Transform::*)(const zg::Vector3&)>(&Transform::Scale)
		),
		"SetUniformWorldScale2D", &Transform::SetUniformWorldScale2D,
		"SetUniformWorldScale3D", &Transform::SetUniformWorldScale3D,

		"SetLocalPosition", sol::overload(
			static_cast<void (Transform::*)(float)>(&Transform::SetLocalPosition),
			static_cast<void (Transform::*)(float, float)>(&Transform::SetLocalPosition),
			static_cast<void (Transform::*)(float, float, float)>(&Transform::SetLocalPosition),
			static_cast<void (Transform::*)(const zg::Vector3&)>(&Transform::SetLocalPosition)
		),
		"SetLocalRotation", sol::overload(
			static_cast<void (Transform::*)(float)>(&Transform::SetLocalRotation),
			static_cast<void (Transform::*)(float, float)>(&Transform::SetLocalRotation),
			static_cast<void (Transform::*)(float, float, float)>(&Transform::SetLocalRotation),
			static_cast<void (Transform::*)(const zg::Vector3&)>(&Transform::SetLocalRotation),
			static_cast<void (Transform::*)(const zg::Quaternion&)>(&Transform::SetLocalRotation)
		),
		"SetLocalScale", sol::overload(
			static_cast<void (Transform::*)(float)>(&Transform::SetLocalScale),
			static_cast<void (Transform::*)(float, float)>(&Transform::SetLocalScale),
			static_cast<void (Transform::*)(float, float, float)>(&Transform::SetLocalScale),
			static_cast<void (Transform::*)(const zg::Vector3&)>(&Transform::SetLocalScale)
		),
		"SetUniformWorldScale2D", &Transform::SetUniformLocalScale2D,
		"SetUniformWorldScale3D", &Transform::SetUniformLocalScale3D,
		"LocalTranslate", sol::overload(
			static_cast<void (Transform::*)(float, float, float)>(&Transform::LocalTranslate),
			static_cast<void (Transform::*)(const zg::Vector3&)>(&Transform::LocalTranslate)
		),
		"LocalRotate", sol::overload(
			static_cast<void (Transform::*)(float, float, float)>(&Transform::LocalRotate),
			static_cast<void (Transform::*)(const zg::Vector3&)>(&Transform::LocalRotate)
		),
		"LocalScale", sol::overload(
			static_cast<void (Transform::*)(float, float, float)>(&Transform::LocalScale),
			static_cast<void (Transform::*)(const zg::Vector3&)>(&Transform::LocalScale)
		),

		//read-only varibles
		"position", &Transform::position,
		"scale", &Transform::scale,
		"rotation", &Transform::rotation,
		"rotation", &Transform::rotation,
		"localPosition", &Transform::localPosition,
		"localScale", &Transform::localScale,
		"localRotation", &Transform::localRotation,
		"localRotation", &Transform::localRotation,

		//test
		"ForceUpdate", &Transform::ForceUpdate,
		"UpdateWithParent", &Transform::UpdateWithParent
		);
}
#endif
