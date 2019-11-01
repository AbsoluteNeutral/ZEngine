
#ifndef _TRANSFORM_H
#define _TRANSFORM_H

#include "XMLWrap.h"
#include "Reals.h"

enum ZG_API TRANSFORM_FLAG_ENUM {
	T_DIRTY				= 0x00000001,	//track wheather the matrix have been concatenated
	T_STATIC			= 0x00000002,	//track to see if is a static transform, no update is needed throughout
	T_YET_TOUPDATE		= 0x00000004,	//track wheather world coordinates have been updated, only applies to child transform
	T_ISBONE			= 0x00000008	//track wheather if is a bone object, internal use only
};

class ZG_API Transform {
public:
	Transform	() noexcept;
	Transform	(const zg::Vector3& position_, const zg::Quaternion& quaternion_, const zg::Vector3& scale_) noexcept;
	~Transform	() = default;

	// ____________________________________________________________static 
	static Transform Identity;

	// ____________________________________________________________ setters
	void SetFacingVector		(const zg::Vector3& zfacing);
	void SetAsBoneObject		(bool isBoneObject_);
	void SetStatic				(bool static_);
	void SetWorldFromMatrix		(const zg::Matrix44& mat);
	void SetLocalFromMatrix		(const zg::Matrix44& mat);

	// ____________________________________________________________ World
	void SetPosition			(float x_);
	void SetPosition			(float x_, float y_);
	void SetPosition			(float x_, float y_ , float z_);
	void SetPosition			(const zg::Vector3& position_);
	void SetRotation			(float x_);
	void SetRotation			(float x_, float y_);
	void SetRotation			(float x_, float y_, float z_);
	void SetRotation			(const zg::Vector3& eular_);
	void SetRotation			(const zg::Quaternion& rotation_);
	void SetScale				(float x_);
	void SetScale				(float x_, float y_);
	void SetScale				(float x_, float y_ , float z_);
	void SetScale				(const zg::Vector3& scale_);
	void SetUniformWorldScale2D	(float xy_);
	void SetUniformWorldScale3D	(float xyz_);

	// ____________________________________________________________ Local
	void SetLocalPosition		(float x_);
	void SetLocalPosition		(float x_, float y_);
	void SetLocalPosition		(float x_, float y_, float z_);
	void SetLocalPosition		(const zg::Vector3& position_);
	void SetLocalRotation		(float x_);
	void SetLocalRotation		(float x_, float y_);
	void SetLocalRotation		(float x_, float y_, float z_);
	void SetLocalRotation		(const zg::Vector3& localEular__);
	void SetLocalRotation		(const zg::Quaternion& rotation_);
	void SetLocalScale			(float x_);
	void SetLocalScale			(float x_, float y_);
	void SetLocalScale			(float x_, float y_, float z_);
	void SetLocalScale			(const zg::Vector3& scale_);
	void SetUniformLocalScale2D	(float xy_);
	void SetUniformLocalScale3D	(float xyz_);

#ifdef USING_SOL2
	//world
	void Translate		(float x_, float y_, float z_);
	void Translate		(const zg::Vector3& translate_);
	void Rotate			(float x_, float y_, float z_);
	void Rotate			(const zg::Vector3& rotation_);
	void Scale			(float x_, float y_, float z_);
	void Scale			(const zg::Vector3& scaling_);
	//local
	void LocalTranslate	(float x_, float y_, float z_);
	void LocalTranslate	(const zg::Vector3& translate_);
	void LocalRotate	(float x_, float y_, float z_);
	void LocalRotate	(const zg::Vector3& rotation_);
	void LocalScale		(float x_, float y_, float z_);
	void LocalScale		(const zg::Vector3& scaling_);
	static void BindLua	(sol::state& lua);
#else
	// ____________________________________________________________ world
	void Translate			(float x_, float y_ = 0.0f, float z_ = 0.0f);
	void Translate			(const zg::Vector3& translate_);
	void Rotate				(float x_, float y_ = 0.0f, float z_ = 0.0f);
	void Rotate				(const zg::Vector3& eular_);
	void Rotate				(const zg::Quaternion& rotation_);
	void Scale				(float x_, float y_ = 0.0f, float z_ = 0.0f);
	void Scale				(const zg::Vector3& scaling_);
	void ScaleMultiply		(float x_, float y_ = 0.0f, float z_ = 0.0f);
	void ScaleMultiply		(const zg::Vector3& scaling_);
	// ____________________________________________________________ local
	void LocalTranslate		(float x_, float y_ = 0.0f, float z_ = 0.0f);
	void LocalTranslate		(const zg::Vector3& translate_);
	void LocalRotate		(float x_, float y_ = 0.0f, float z_ = 0.0f);
	void LocalRotate		(const zg::Vector3& eular_);
	void LocalRotate		(const zg::Quaternion& rotation_);
	void LocalScale			(float x_, float y_ = 0.0f, float z_ = 0.0f);
	void LocalScale			(const zg::Vector3& scaling_);
	void LocalScaleMultiply	(float x_, float y_ = 0.0f, float z_ = 0.0f);
	void LocalScaleMultiply	(const zg::Vector3& scaling_);
#endif

	// ____________________________________________________________ getters
	Transform			GetConcatTransform	() const;
	Transform			GetInverse			() const;
	const zg::Matrix44&	GetMatrix			();
	zg::Matrix44		GetLocalMatrix		() const;
	const zg::Matrix44&	GetWorldMatrix		() const;
	bool				HasUpdated			() const;
	bool				IsBone				() const;
	bool				IsStatic			() const;
	zg::Vector3			Forward				() const;
	zg::Vector3			Up					() const;
	zg::Vector3			Right				() const;

	zg::Vector3			GetWorldPosition	() const;
	zg::Quaternion		GetWorldRotation	() const;
	zg::Vector3			GetWorldScale		() const;

	//etc function
	void Attach						(Transform* parent);	//attach to a parent transform
	void Detach						();

	void CameraLookAt				(const Transform& target_);	
	void CameraLookAt				(const zg::Vector3& target_);
	void LookAt						(const Transform& target_);
	void LookAt						(const zg::Vector3& target_);
	void LookAt						(const zg::Vector3& target_, const zg::Vector3& facing_, const zg::Vector3& up_);

	void Reset						();
	void ResetFlag					();
	void SetUpdateFlag				();

	void RotateTowards				(const Transform& target_, float rate);
	void RotateTowards				(const zg::Vector3& target_, float rate);
	void RotateTowardsIgnoreHeight	(const Transform& target_, float rate);
	void RotateTowardsIgnoreHeight	(const zg::Vector3& target_, float rate);
	void Slerp						(const zg::Quaternion& q, float deltatime_);

	void ForceUpdate				();
	void Update						();
	void dump						() const;

	// ____________________________________________________________ serial
#ifdef TINYXML2_EXPORT
	void Serial		(FXMLDocument* doc, FXMLNode* pElem);
	void Unserial	(FXMLElement* pElem);
#endif

	//____________________________________________________________serial friend
	ZG_API friend Transform ConcatVQS		(const Transform& t2_, const Transform& t1_);
	ZG_API friend Transform ConcatLocalVQS	(const Transform& t2_, const Transform& t1_);

	zg::Vector3		position;		//read-only
	zg::Vector3		localPosition;	//read-only
	zg::Vector3		scale;			//read-only
	zg::Vector3		localScale;		//read-only
	zg::Quaternion	rotation;		//read-only
	zg::Quaternion	localRotation;	//read-only

private:
	std::vector<Transform*> child;
	int flags;
	zg::Vector3 facing;
	Transform* parent;
	zg::Matrix44 worldMatrix;

	void SignalChildDirtyBit();
};

// ____________________________________________________________ extern function
ZG_API Transform InterpolateTransforms(const Transform& oldTransform, const Transform& newTransform, float factor);
ZG_API void RotateTowards(Transform& transform_, const Transform& target, float rate);
ZG_API void RotateTowards(Transform& transform_, const zg::Vector3& target, float rate);

#endif //_TRANSFORM_H
