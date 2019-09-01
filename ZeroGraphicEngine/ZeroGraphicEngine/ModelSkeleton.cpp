#include "stdafx.h"

#include <stdio.h>
#include "Model.h"
#include "ModelSkeleton.h"

//______________________________________________________________________ Model's Vertex Data Structure
IBone::IBone() noexcept
	: offset{}
	, index(0)
{}

#if defined(_DEBUG)
IBone::IBone(const char* boneName_) noexcept
	: offset{}
	, myname{ boneName_ }
	, name{ GenerateHash_ModelName(boneName_) }
	, index(0)
{}

IBone::IBone(size_t hash_boneName_) noexcept
	: offset{}
	, myname{ GetModelNameFromHash(hash_boneName_) }
	, name{ hash_boneName_ }
	, index(0)
{}
#endif
//______________________________________________________________________ Model's Bone Data Structure
INode::INode(const char* nodeName_) noexcept
	: matrix{}
	, childNodes{}
	, parent{ nullptr }
	, name{ GenerateHash_ModelName(nodeName_) }
#if defined(_DEBUG)
	, myname(nodeName_)
#endif
{
	matrix.SetIdentity();
}

INode::INode(size_t hash_nodeName_) noexcept
	: matrix{}
	, childNodes{}
	, parent{ nullptr }
	, name{ hash_nodeName_ }
#if defined(_DEBUG)
	, myname(GetModelNameFromHash(name))
#endif
{
	matrix.SetIdentity();
}

void INode::Print() {
	printf("%s\n", GetModelNameFromHash(name).c_str());
	for (const auto& elem : childNodes) {
		Printt(elem, 1);
	}
}

void INode::Printt(const INode& node_, int depth) {
	int d = depth;
	while (d--)
		printf(" ");

	printf("%s\n", GetModelNameFromHash(node_.name).c_str());
	for (const auto& elem : node_.childNodes) {
		Printt(elem, depth + 1);
	}
}


//______________________________________________________________________ Bone Animation Data Structure
//For storing Current Position or Scale at current time
Vector3Key::Vector3Key(float time_, const zg::Vector3& vector_) noexcept
	: time{ time_ }, value{ vector_ }
{}

//For storing Current Rotation at current duration
QuaternionKey::QuaternionKey(float time_, const zg::Quaternion& quaternion_) noexcept
	: time{ time_ }, value{ quaternion_ }
{}


//Current Animation Channel Data Structure, etc Idle, Attack ...
BoneAnimationChannel::BoneAnimationChannel(const char* boneName_) noexcept
	: positionKeys{}
	, scaleKeys{}
	, rotationKeys{}
	, numOfpositionKey(0)
	, numOfscaleKey(0)
	, numOfrotationKey(0)
	, boneName{ GenerateHash_ModelName(boneName_) }
{}

BoneAnimationChannel::BoneAnimationChannel(size_t hash_boneName_) noexcept
	: positionKeys{}
	, scaleKeys{}
	, rotationKeys{}
	, numOfpositionKey(0)
	, numOfscaleKey(0)
	, numOfrotationKey(0)
	, boneName{ hash_boneName_ }
{}

unsigned BoneAnimationChannel::FindPositionKey(float time_) const
{
	for (unsigned i = 0, end = numOfpositionKey - 1; i < end; ++i)
		if (time_ < positionKeys[i + 1].time)
			return i;
	return numOfpositionKey;
}

unsigned BoneAnimationChannel::FindScaleKey(float time_) const
{
	for (unsigned i = 0, end = numOfscaleKey - 1; i < end; ++i)
		if (time_ < scaleKeys[i + 1].time)
			return i;
	return numOfscaleKey;
}

unsigned BoneAnimationChannel::FindRotationKey(float time_) const
{
	for (unsigned i = 0, end = numOfrotationKey - 1; i < end; ++i)
		if (time_ < rotationKeys[i + 1].time)
			return i;
	return numOfrotationKey;
}

VQS BoneAnimationChannel::CalculateVQS(float time_) 
{
	VQS tmp;
	bool isposKey1 = numOfpositionKey == 1;
	bool isrotKey1 = numOfrotationKey == 1;
	bool isscaKey1 = numOfscaleKey == 1;

	tmp.pos = isposKey1 ? positionKeys[0].value : tmp.pos;
	tmp.rot = isrotKey1 ? rotationKeys[0].value : tmp.rot;
	tmp.sca = isscaKey1 ? scaleKeys[0].value : tmp.sca;

	unsigned posI = FindPositionKey(time_);
	unsigned rotI = FindRotationKey(time_);
	unsigned scaI = FindScaleKey(time_);

	if (!isposKey1)
	{
		if (posI == numOfpositionKey)
			tmp.pos = positionKeys.back().value;
		else 
		{
			//get total number of frame at this time
			float factor = (time_ - positionKeys[posI].time);
			tmp.pos = zg::Lerp(positionKeys[posI].value, positionKeys[posI + 1].value, factor);
		}
	}

	if(!isscaKey1)
	{
		if (scaI == numOfscaleKey)
		{
			tmp.sca = scaleKeys.back().value;
		}
		else
		{
			float factor = time_ - scaleKeys[scaI].time;
			tmp.sca = zg::Lerp(scaleKeys[scaI].value, scaleKeys[scaI + 1].value, factor);
		}
	}

	if(!isrotKey1)
	{
		if (rotI == numOfrotationKey)
		{
			tmp.rot = rotationKeys.back().value;
		}
		else
		{
			float factor = time_ - rotationKeys[rotI].time;
			tmp.rot = zg::nLerp(rotationKeys[rotI].value, rotationKeys[rotI + 1].value, factor);
			//tmp.rot = zg::Slerp(rotationKeys[rotI].value, rotationKeys[rotI + 1].value, factor);
		}
	}
	return tmp;
}

//______________________________________________________________________ Animation Data Structure
//Holds all Animation Channels
Animation::Animation(const char* animationTitle_, float duration_, float ticksPerSecond_) noexcept
	: listOfBones{}
	, name{ GenerateHash_ModelName(animationTitle_) }
	, duration{ duration_ }
	, ticksPerSecond{ ticksPerSecond_ }
{}

Animation::Animation(size_t hash_animationTitle_, float duration_, float ticksPerSecond_) noexcept 
	: listOfBones{}
	, name{ hash_animationTitle_ }
	, duration{ duration_ }
	, ticksPerSecond{ ticksPerSecond_ }
{}

bool  Animation::operator==(size_t hashname_) const 
{
	return name == hashname_;
}
//______________________________________________________________________ Meshes Data Structure
//Each Mesh in a Model/Scene Data Structure
IMesh::IMesh(unsigned int indices, unsigned int v, unsigned int i) noexcept
	: IndiceCount{ indices }
	, BaseVertex{ v }
	, BaseIndex{ i }
{}
