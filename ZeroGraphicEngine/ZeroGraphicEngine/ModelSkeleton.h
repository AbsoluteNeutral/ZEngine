
#ifndef _MODEL_SKELETON_H
#define _MODEL_SKELETON_H

#define MAX_BONES 100
#include "ZGMath.h"

//______________________________________________________________________ Model's Vertex Data Structure
struct ZG_API IVertex {
	zg::Vector3 position;
	zg::Vector3 normal;
	zg::Vector3 tangent;
	zg::Vector2 uv;
	zg::Vector4 weights;
	zg::Vector4 boneIndex;
};

//______________________________________________________________________ Model's Bone Data Structure
struct ZG_API IBone {
	IBone()						noexcept;
#if defined(_DEBUG)
	IBone(const char* boneName_)noexcept;
	IBone(size_t hash_boneName_)noexcept;
#endif

	zg::Matrix44 offset;
#if defined(_DEBUG)
	std::string myname;
	size_t name;
#endif
	unsigned index;
};

//______________________________________________________________________ Model's Bone Data Structure
struct ZG_API INode {
	INode		(const char* nodeName_ = "")	noexcept;
	INode		(size_t hash_nodeName_)			noexcept;

	void Print	();

	zg::Matrix44		matrix;
	std::vector<INode>	childNodes;
	INode*				parent;
	size_t				name;

#if defined(_DEBUG)
	std::string myname;
#endif
	
private:
	void Printt(const INode& node_, int depth);
};

//______________________________________________________________________ Bone Animation Channel Data Structure
//For storing Current Position or Scale at current time
struct ZG_API Vector3Key {
	Vector3Key(float time_ = 0.0f, const zg::Vector3& vector_ = zg::Vector3{}) noexcept;

	zg::Vector3 value;	//value of the Vector3
	float time;			//current key frame
};

//For storing Current Rotation at current duration
struct ZG_API QuaternionKey {
	QuaternionKey(float time_ = 0.0f, const zg::Quaternion& quaternion_ = zg::Quaternion{}) noexcept;

	zg::Quaternion value;	//value of the Quaternion
	float time;				//current key frame
};

//Current Animation Channel Data Structure, etc Idle, Attack ...
struct ZG_API VQS {
	zg::Vector3 pos;
	zg::Vector3 sca;
	zg::Quaternion rot;
};

struct ZG_API BoneAnimationChannel {

	BoneAnimationChannel(const char* boneName_ = "")	noexcept;
	BoneAnimationChannel(size_t hash_boneName_)			noexcept;

	unsigned FindPositionKey	(float time_) const;
	unsigned FindScaleKey		(float time_) const;
	unsigned FindRotationKey	(float time_) const;
	VQS CalculateVQS			(float time_);

	std::vector<Vector3Key>		positionKeys;
	std::vector<Vector3Key>		scaleKeys;
	std::vector<QuaternionKey>	rotationKeys;
	size_t boneName;
	unsigned numOfpositionKey;
	unsigned numOfscaleKey;
	unsigned numOfrotationKey;
};

//______________________________________________________________________ Animation Data Structure
//Holds all Animation Channels
struct ZG_API Animation {
	Animation	(const char* animationTitle_ = "", float duration_ = 0.0f, float ticksPerSecond_ = 0.0f) noexcept;
	Animation	(size_t hash_animationTitle_, float duration_, float ticksPerSecond_) noexcept;
	
	bool operator==(size_t hash) const;

	std::map<size_t, BoneAnimationChannel> listOfBones;
	size_t name;			//animation name
	float duration;			//total number of keyframe
	float ticksPerSecond;	//framerate that is exported
};

//______________________________________________________________________ Meshes Data Structure
//Each Mesh in a Model/Scene Data Structure
struct ZG_API IMesh {
public:
	IMesh(unsigned int indices = 0, unsigned int v = 0, unsigned int i = 0) noexcept;
	unsigned int IndiceCount;	//total number of indices in this mesh
	unsigned int BaseVertex;	//stating vector<> index of the Vertex
	unsigned int BaseIndex;		//stating vector<> index of the indicies 
};

#endif
