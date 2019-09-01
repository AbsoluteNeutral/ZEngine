/*****************************************************************************/
/*!
\file			MeshRenderer.cpp
\brief			MeshRenderer Component of Game Object
*/
/*****************************************************************************/
#include "stdafx.h"
#include "MeshRenderer.h"
#include "Gizmo.h"
#include "Logging.h"

MeshRenderer::MeshRenderer(Model* mesh_, IShader* material_, const zg::Color& color_) noexcept
  : //CommonDrawComponent{ DRAW_MESHRENDERER },
	Component{}
	, model{ mesh_ }
	, material{ material_ }
	, timeElapsed{ 0.f }
	, animationSpeed{ 1.f }
	//, currentAnimation{ 0 }
	, boneLocations{}
	, color{ color_ }
	, isVisible{ true }
	//, castShadows{ true }
	//, revert{ false }
	, toggleLoop{ true }
	, prevLoop{ false }
	, finishedAnim{ false }
	//, blending{ false }
	//, animating{ true }
	//, receiveShadows{ true }
	//, cullFront{ false }
	//, postProcess{ false }
	//, justDraw{ false }
	//, bloom{ true }
	//, prevAnim{ 0 }
	, currFrame{ 0 }
	, prevFrame{ 0 }
	//, uvOffset{}
	//, blendTime{ 0.f }
	//, blendTimer{ 0.f }
	, prevTimeElapsed{ 0.f }
	//, intensity{ 1.f }
	//, prevAnimation{ 0 }
	, textureMaps{}
	, boneTransforms{}
	, boneObjects{}
	//, callbacks{}
	//, uniformData{}
	, modelAnimataion(nullptr)
{
  textureMaps[0] = nullptr;
  textureMaps[1] = nullptr;
  textureMaps[2] = nullptr;
  textureMaps[3] = nullptr;
  boneTransforms.reserve(MAX_BONES);
}

MeshRenderer::MeshRenderer(const MeshRenderer& rhs)
	: 
	//CommonDrawComponent{ rhs }, 
	Component{},
    model{ rhs.model },
    material{ rhs.material },
    timeElapsed{ rhs.timeElapsed },
    animationSpeed{ rhs.animationSpeed },
    //currentAnimation{ rhs.currentAnimation },
    boneLocations{},
    color{ rhs.color },
    isVisible{ rhs.isVisible },
    //castShadows{ rhs.castShadows },
    //revert{ rhs.revert },
    toggleLoop{ rhs.toggleLoop },
    prevLoop{ rhs.prevLoop },
    finishedAnim{ rhs.finishedAnim },
    //blending{ rhs.blending },
    //animating{ rhs.animating },
    //receiveShadows{ rhs.receiveShadows },
    //cullFront{ rhs.cullFront },
    //postProcess{ rhs.postProcess },
    //justDraw{ rhs.justDraw },
    //bloom{ rhs.bloom },
    //prevAnim{ rhs.prevAnim },
    currFrame{ rhs.currFrame },
    prevFrame{ rhs.prevFrame },
    //uvOffset{ rhs.uvOffset },
    //blendTime{ rhs.blendTime },
   // blendTimer{ rhs.blendTimer },
    prevTimeElapsed{ rhs.prevTimeElapsed },
    //intensity{ rhs.intensity },
   // prevAnimation{ rhs.prevAnimation },
    textureMaps{},
    boneTransforms{},
    boneObjects{}
    //uniformData{ rhs.uniformData }
    //callbacks{ rhs.callbacks }
{
  SetIShader(rhs.material);
  boneTransforms.reserve(MAX_BONES);
  InitBone();
  InitBoneToShader();

  for(int i = 0; i < 4; ++i)
	  textureMaps[i] = rhs.textureMaps[i];
}

MeshRenderer& MeshRenderer::operator=(const MeshRenderer& rhs) 
{
	Component::operator=(rhs);
	model = rhs.model;
	material = rhs.material;
	timeElapsed = rhs.timeElapsed;
	animationSpeed = rhs.animationSpeed;
	//currentAnimation = rhs.currentAnimation;
	color = rhs.color;
	isVisible = rhs.isVisible;
	//castShadows = rhs.castShadows;
	//revert = rhs.revert;
	toggleLoop = rhs.toggleLoop;
	prevLoop = rhs.prevLoop;
	finishedAnim = rhs.finishedAnim;
	//blending = rhs.blending;
	//animating = rhs.animating;
	//receiveShadows = rhs.receiveShadows;
	//cullFront = rhs.cullFront;
	//postProcess = rhs.postProcess;
	//justDraw = rhs.justDraw;
	// bloom = rhs.bloom;
	//prevAnim = rhs.prevAnim;
	currFrame = rhs.currFrame;
	prevFrame = rhs.prevFrame;
	//uvOffset = rhs.uvOffset;
	// blendTime = rhs.blendTime;
	// blendTimer = rhs.blendTimer;
	prevTimeElapsed = rhs.prevTimeElapsed;
	//intensity = rhs.intensity;
	//prevAnimation = rhs.prevAnimation;
	
	InitBone();
	//InitBoneToShader();
	
	for (int i = 0; i < 4; ++i)
		textureMaps[i] = rhs.textureMaps[i];
	return *this;
}

MeshRenderer::~MeshRenderer() 
{}
//MeshRenderer* MeshRenderer::Clone() 
//{
//#ifdef USE_RAWNEW
//	return new MeshRenderer{ *this };
//#else
//	MeshRenderer* tmp = Allocate<MeshRenderer>(1);
//	new(tmp) MeshRenderer{ *this };
//	return tmp;
//#endif
//}

// ____________________________________________________________ init
void MeshRenderer::Attach(GameObject* obj_, size_t arrayIndex_, const char* modelname_, const char* diffuseName_, const char* glsl_, bool generateBones_, const zg::Color& color_)
{	
	owner = obj_;
	obj_->AddComponent<MeshRenderer>(arrayIndex_);

	material = FindShader(glsl_);
	color = color_;
	textureMaps[0] = FindTexture(diffuseName_);
	textureMaps[1] = FindTexture("null.dds");
	textureMaps[2] = FindTexture("n_null.dds");
	textureMaps[3] = FindTexture("black.dds");

	SetModel(modelname_, generateBones_);
	//InitBoneToShader();
}

void MeshRenderer::Init(const char* modelname_, const char* diffuseName_, const char* glsl_, bool generateBones_, const zg::Color& color_)
{
	material = FindShader(glsl_);
	color = color_;
	textureMaps[0] = FindTexture(diffuseName_);
	textureMaps[1] = FindTexture("null.dds");
	textureMaps[2] = FindTexture("n_null.dds");
	textureMaps[3] = FindTexture("black.dds");
	SetModel(modelname_, generateBones_);
	//InitBoneToShader();
}

void MeshRenderer::InitBone()
{
	if (model->boneCount)
	{
		boneTransforms.clear();
		boneTransforms = std::vector<zg::Matrix44>(model->boneCount);
	}
}

void MeshRenderer::InitBoneToShader()
{
	int sz = MIN2(model->boneCount, MAX_BONES);
	for (int i = 0; i < sz; ++i)
	{
		char name[128];
		memset(name, 0, 128);
		sprintf_s(name, 128, "gBones[%d]", i);
		boneLocations[i] = glGetUniformLocation(material->programID, name);
	}
}

void MeshRenderer::CreateBoneGameObject(const INode* node, GameObject*& parent)
{
#if defined(_EDITOR_ON) || !defined(_EDITOR_ON)
	if (!node) return;

	if (GetModelNameFromHash(node->name).find("$Assimp") == std::string::npos)
	{
		if (GetModelNameFromHash(node->name) == "RootNode")
		{
			boneObjects[node->name] = parent;

			for (auto& elem : node->childNodes)
				CreateBoneGameObject(&elem, parent);
		}
		else
		{
			GameObject* obj = RM::CreateGameObject(GetModelNameFromHash(node->name).c_str());
			obj->transform.SetAsBoneObject(true);
			boneObjects[node->name] = obj;
			obj->Attach(parent);

			for (auto& elem : node->childNodes)
				CreateBoneGameObject(&elem, obj);
		}
	}
	else
	{
		GameObject* obj = RM::CreateGameObject(GetModelNameFromHash(node->name).c_str());
		obj->transform.SetAsBoneObject(true);
		boneObjects[node->name] = obj;
		obj->Attach(parent);

		//precompute for $Assimp objects
		//zg::Vector3 extrascale{ model->extra_calefare_scalingmatrix.m0, model->extra_calefare_scalingmatrix.m0, model->extra_calefare_scalingmatrix.m0 };
		//zg::Matrix44 out = model->extra_calefare_scalingmatrix * node->matrix;
		obj->transform.localPosition = model->scalingFactor * zg::GetPosition(node->matrix);
		obj->transform.localRotation = zg::GetRotation(node->matrix);
		obj->transform.localScale = zg::GetScale(node->matrix);
		obj->transform.SetUpdateFlag();

		for (auto& elem : node->childNodes)
			CreateBoneGameObject(&elem, obj);
	}
#else
	UNREFERENCED_PARAMETER(node);
	UNREFERENCED_PARAMETER(parent);
#endif
}

// ____________________________________________________________ etc function

//Etc
//void MeshRenderer::ShadowDraw(const IShader* m)
//{
//  if (!owner->IsActive() || !isVisible || !castShadows || color.a <= 0.0f) return;
//
//  if (model->hasAnims)
//  {
//    m->SendBool("anim", true);
//
//    for (auto i = 0u; i < boneTransforms.size(); ++i)
//      m->SendMat4(("gBones[" + std::to_string(i) + "]").c_str(), boneTransforms[i]);
//  }
//  else
//    m->SendBool("anim", false);
//
//  model->ShadowDraw(this, m);
//}


//void MeshRenderer::Draw(const Camera* cam, LightSource* ls)
void MeshRenderer::Draw(const Camera* camera_)
{
	//static auto defaultTagHash = GetHashFromString("default");
	material->Use();

	//auto s = material;
	//for (auto& data : uniformData)
	//{
	//  switch (s->uniforms[data.first])
	//  {
	//  case UNIFORM_FLOAT:
	//  {
	//    static float f;
	//    sscanf_s(data.second.c_str(), "%f", &f);
	//    material->SendFloat(GetStringFromHash(data.first).c_str(), f);
	//    break;
	//  }
	//  case UNIFORM_FLOAT_VEC2:
	//  {
	//    static zg::Vector2 v;
	//    sscanf_s(data.second.c_str(), "%f %f", &v.x, &v.y);
	//    material->SendVec2(GetStringFromHash(data.first).c_str(), v);
	//    break;
	//  }
	//  case UNIFORM_FLOAT_VEC3:
	//  {
	//    static zg::Vector3 v;
	//    sscanf_s(data.second.c_str(), "%f %f %f", &v.x, &v.y, &v.z);
	//    material->SendVec3(GetStringFromHash(data.first).c_str(), v);
	//    break;
	//  }
	//  case UNIFORM_FLOAT_VEC4:
	//  {
	//    static zg::Vector4 v;
	//    sscanf_s(data.second.c_str(), "%f %f %f %f", &v.x, &v.y, &v.z, &v.w);
	//    material->SendVec4(GetStringFromHash(data.first).c_str(), v);
	//    break;
	//  }
	//  case UNIFORM_INT:
	//  {
	//    static int i;
	//    sscanf_s(data.second.c_str(), "%d", &i);
	//    material->SendInt(GetStringFromHash(data.first).c_str(), i);
	//    break;
	//  }    
	//  case UNIFORM_BOOL:
	//  {
	//    static int i;
	//    sscanf_s(data.second.c_str(), "%d", &i);
	//    material->SendBool(GetStringFromHash(data.first).c_str(), (bool)i);
	//    break;
	//  }
	//  }
	//}
	
	if (model->hasAnims) 
	{
		float invsc = 1.0f / model->scalingFactor;
		zg::Vector3 extrascale{ invsc,  invsc, invsc };
		for (const auto& elem : model->iNode.childNodes)
			UpdateBonesIK(&elem, zg::Matrix44::Identity, extrascale);
	}
	

	//activate textures
	for (int i = 0; i < 4; ++i)
	{
	  glActiveTexture(GL_TEXTURE0 + i);
	  glUniform1i(glGetUniformLocation(material->programID, TEXTURE_TYPE_NAME[i]), i);
	  glBindTexture(GL_TEXTURE_2D, textureMaps[i]->id);
	}
	
	InitBoneToShader();
	size_t sz = boneTransforms.size();
	for (size_t i = 0; i < sz ; ++i)
	  material->SendMat4(boneLocations[i], boneTransforms[i]);
	
	//material->SendMat4("rot", owner->transform.GetRotMatrix());
	//material->SendFloat("mIntensity", intensity);
	//if (cullFront)
	//  glCullFace(GL_FRONT);
	  glCullFace(GL_BACK);
	//model->Draw(cam, this, ls);
	//model->Draw(cam, this);
	
	material->SendInt("anim", model->hasAnims ? 1 : 0);
	material->SendFloat("scalingFactor", model->scalingFactor);
	material->SendMat4("projview", camera_->ProjViewMatrix());
	material->SendMat4("model", owner->transform.GetMatrix());
	material->SendVec4("color", color);
	//material->SendBool("receiveShadows", s->receiveShadows);
	//material->SendBool("bloom", s->bloom);

	model->Draw();
	
	//if (cullFront)
	//  glCullFace(GL_BACK);
}

void MeshRenderer::DrawWireFrame(const Camera* camera_, const zg::Color& color_)
{
	material->Use();
	
	if (model->hasAnims)
	{
		float invsc = 1.0f / model->scalingFactor;
		zg::Vector3 extrascale{ invsc,  invsc, invsc };
		for (const auto& elem : model->iNode.childNodes)
			UpdateBonesIK(&elem, zg::Matrix44::Identity, extrascale);
	}

	//activate textures
	static Texture2D* whitedds = FindTexture("white.dds");
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(material->programID, TEXTURE_TYPE_NAME[0]), 0);
	glBindTexture(GL_TEXTURE_2D, whitedds->id);

	InitBoneToShader();
	size_t sz = boneTransforms.size();
	for (size_t i = 0; i < sz; ++i)
		material->SendMat4(boneLocations[i], boneTransforms[i]);

	glCullFace(GL_BACK);

	material->SendInt("anim", model->hasAnims ? 1 : 0);
	material->SendFloat("scalingFactor", model->scalingFactor);
	material->SendMat4("projview", camera_->ProjViewMatrix());
	material->SendMat4("model", owner->transform.GetMatrix());
	material->SendVec4("color", color_);
	//material->SendFloat("scalingFactor", model->extra_calefare_scalingmatrix.m0);
	model->Draw();
}

void MeshRenderer::RestartAnimation()
{
	finishedAnim	= false;
	timeElapsed		= 0.0f;
	//prevTimeElapsed = 0.0f;
	//currFrame		= 0;
	//prevFrame		= 0;
}

void MeshRenderer::RenderSkeleton()
{
	for(const auto& elem : owner->GetAllChildren())
		elem->RenderParentAndChild();
}

void MeshRenderer::Update()
{
	if (finishedAnim) return;

	if (modelAnimataion && model->hasAnims)
	{
		{
			//Compute Time elapsed have past in the curren time frame of the current animation
			timeElapsed += FrameRateController::dttimescale * animationSpeed;

			//const auto& anims = model->animations;
			//float dur = anims[currentAnimation].duration;
			////std::cout << dur << std::endl;
			//float timeInTicks = timeElapsed * (anims[currentAnimation].ticksPerSecond != 0 ? anims[currentAnimation].ticksPerSecond : 25.f);
			//float animTime = false ? fmod(timeInTicks, dur) : timeInTicks > dur ? dur : timeInTicks;

			float totalKeyFrame = modelAnimataion->duration;
			float currentKeyFrame_time = timeElapsed * modelAnimataion->ticksPerSecond;
			currentKeyFrame_time = currentKeyFrame_time >= totalKeyFrame ? totalKeyFrame : currentKeyFrame_time;

			//check if animation have done
			if (currentKeyFrame_time >= totalKeyFrame)
			{
				currentKeyFrame_time = totalKeyFrame;
				timeElapsed = 0.0f;
				finishedAnim = toggleLoop ? false : true;
			}

			//zg::Vector3 extrascale{ extra_calefare_scalingmatrix_.m0, extra_calefare_scalingmatrix_.m0, extra_calefare_scalingmatrix_.m0 };
			zg::Vector3 extrascale{ model->scalingFactor,  model->scalingFactor, model->scalingFactor };
			for (const auto& elem : model->iNode.childNodes)
				UpdateBonesAnimation(currentKeyFrame_time, &elem, zg::Matrix44::Identity, extrascale);
				//UpdateBones(currentKeyFrame_time, &elem, zg::Matrix44::Identity, model->extra_calefare_scalingmatrix);
			//prevFrame = currFrame;
		}
	}
}

//dosen't work 
void MeshRenderer::PostUpdate() 
{
	//for (auto& elem : model->boneOffsetMap) 
	//{
	//	boneTransforms[elem.second.index] = 
	//		zg::GaussJordanInverse(owner->transform.GetMatrix()) * boneObjects[elem.second.name]->transform.GetMatrix() * elem.second.offset;
	//}
	//if (model->boneOffsetMap.find(node->name) != model->boneOffsetMap.end())
	//{
	//	IBone& getBone = model->boneOffsetMap[node->name];
	//	boneTransforms[getBone.index] = gTrans * getBone.offset;
	//}
}

void MeshRenderer::Pause() {
	finishedAnim = true;
}
void MeshRenderer::Play() {
	finishedAnim = false;
}
void MeshRenderer::Stop() {
	finishedAnim = true;
	timeElapsed = 0.0f;

	if (model->hasAnims)
	{
		zg::Vector3 extrascale{ model->scalingFactor,  model->scalingFactor, model->scalingFactor };
		for (const auto& elem : model->iNode.childNodes)
			UpdateBonesAnimation(0.0f, &elem, zg::Matrix44::Identity, extrascale);
	}
}

void MeshRenderer::UpdateBonesAnimation(float time_, const INode* node, const zg::Matrix44& pTrans, zg::Vector3& scaleFactor_)
{
	//Find it's bone channel affected in this animation 
	BoneAnimationChannel* channel = nullptr;
	auto boneBegin = modelAnimataion->listOfBones.find(node->name);
	if (boneBegin != modelAnimataion->listOfBones.end())
		channel = &(*boneBegin).second;

	//If Current Bone is affected
	zg::Matrix44 gTrans{};
	if (channel)
	{
		//Find it's current frame corresponding keys and compute it's value
		VQS tmp = channel->CalculateVQS(time_);

		//Concat it's matrix for bone transformation in shader
		gTrans = pTrans * zg::BuildTRS4x4(tmp.pos, tmp.rot, tmp.sca);

		//if there is a Bone's Gameobject, update the gameobject as well
		//update it's local's value directly 
		auto b = boneObjects.find(node->name);
		if (b != boneObjects.end())
		{
			GameObject* getboneGameObject = (*b).second;
			getboneGameObject->transform.localPosition	= scaleFactor_ * tmp.pos;
			getboneGameObject->transform.localRotation	= tmp.rot;
			getboneGameObject->transform.localScale		= tmp.sca;
			getboneGameObject->transform.SetUpdateFlag();
		}
	}
	//else 
	//{
	//	//if current bone is not affected
	//	//concat it's matrix
	//	gTrans = pTrans * node->matrix;
	//}
	//
	////concat with it's offset bones matrix if exist
	//auto f = model->boneOffsetMap.find(node->name);
	//if (f != model->boneOffsetMap.end())
	//{
	//	IBone& getBone = (*f).second;
	//	boneTransforms[getBone.index] = gTrans * getBone.offset;
	//}

	//update it's child
	for (const auto& elem : node->childNodes)
		UpdateBonesAnimation(time_, &elem, gTrans, scaleFactor_);
}

void MeshRenderer::UpdateBonesIK(const INode* node, const zg::Matrix44& pTrans, zg::Vector3& scaleFactor_)
{
	//Find it's bone channel affected in this animation 
	BoneAnimationChannel* channel = nullptr;
	auto boneBegin = modelAnimataion->listOfBones.find(node->name);
	if (boneBegin != modelAnimataion->listOfBones.end())
		channel = &(*boneBegin).second;

	//If Current Bone is affected
	zg::Matrix44 gTrans{};
	if (channel)
	{
		//if there is a Bone's Gameobject, update the gameobject as well
		//update it's local's value directly 
		auto b = boneObjects.find(node->name);
		if (b != boneObjects.end())
		{
			GameObject* getboneGameObject = (*b).second;
			gTrans = pTrans * zg::BuildTRS4x4(
				getboneGameObject->transform.localPosition * scaleFactor_,
				getboneGameObject->transform.localRotation,
				getboneGameObject->transform.localScale
			);
		}
	}
	else
	{
		//if current bone is not affected
		//concat it's matrix
		gTrans = pTrans * node->matrix;
	}

	//concat with it's offset bones matrix if exist
	auto f = model->boneOffsetMap.find(node->name);
	if (f != model->boneOffsetMap.end())
	{
		IBone& getBone = (*f).second;
		boneTransforms[getBone.index] = gTrans * getBone.offset;
	}

	//update it's child
	for (const auto& elem : node->childNodes)
		UpdateBonesIK(&elem, gTrans, scaleFactor_);
}

//void MeshRenderer::DrawPost(const Camera * cam)
//{
//  auto s = material->shader;
//
//  glActiveTexture(GL_TEXTURE2);
//  glUniform1i(glGetUniformLocation(s->programID, "filterTexture"), 2);
//  glBindTexture(GL_TEXTURE_2D, textureMaps[0]->id);
//
//  for (auto& data : uniformData)
//  {
//    switch (s->uniforms[data.first])
//    {
//    case UNIFORM_FLOAT:
//    {
//      static float f;
//      sscanf_s(data.second.c_str(), "%f", &f);
//      material->SendFloat(GetStringFromHash(data.first).c_str(), f);
//      break;
//    }
//    case UNIFORM_FLOAT_VEC2:
//    {
//      static zg::Vector2 v;
//      sscanf_s(data.second.c_str(), "%f %f", &v.x, &v.y);
//      material->SendVec2(GetStringFromHash(data.first).c_str(), v);
//      break;
//    }
//    case UNIFORM_FLOAT_VEC3:
//    {
//      static zg::Vector3 v;
//      sscanf_s(data.second.c_str(), "%f %f %f", &v.x, &v.y, &v.z);
//      material->SendVec3(GetStringFromHash(data.first).c_str(), v);
//      break;
//    }
//    case UNIFORM_FLOAT_VEC4:
//    {
//      static zg::Vector4 v;
//      sscanf_s(data.second.c_str(), "%f %f %f %f", &v.x, &v.y, &v.z, &v.w);
//      material->SendVec4(GetStringFromHash(data.first).c_str(), v);
//      break;
//    }
//    case UNIFORM_INT:
//    {
//      static int i;
//      sscanf_s(data.second.c_str(), "%d", &i);
//      material->SendInt(GetStringFromHash(data.first).c_str(), i);
//      break;
//    }
//    case UNIFORM_BOOL:
//    {
//      static int i;
//      sscanf_s(data.second.c_str(), "%d", &i);
//      material->SendBool(GetStringFromHash(data.first).c_str(), (bool)i);
//      break;
//    }
//    }
//  }
//
//  model->Draw(cam, this, nullptr);
//}

std::string MeshRenderer::GetAnimationTitle() const {
	if (!modelAnimataion) return "";
	return GetModelNameFromHash(modelAnimataion->name);
}
Model* MeshRenderer::GetModel() {
	return model; 
}
IShader* MeshRenderer::GetIShader() {
	return material; 
}
int	MeshRenderer::GetAnimationCycles() const {
	return static_cast<int>(modelAnimataion->duration);
}
int	MeshRenderer::GetAnimationCycles(const char* animationTitle_) const {
	size_t hashTitle = GetHashFromString(animationTitle_);
	auto b = model->animations.find(hashTitle);
	bool found = model->animations.find(hashTitle) != model->animations.end();
	//bool found = model->IsAnimationExist(hashTitle);

	if (found)
	{
		return static_cast<int>((*b).second.duration);
		//modelAnimataion = &(model->animations[hashTitle]);
	}
	return 0;
}
bool MeshRenderer::IsCurrentAnim(const char* animationName_) const
{
	if (!modelAnimataion) return true;

	return modelAnimataion->name == GetHashFromModelName(animationName_);
}
bool MeshRenderer::IsCurrentAnim(const std::string& animationName_) const
{
	return IsCurrentAnim(animationName_.c_str());
}

// ____________________________________________________________ getters
void MeshRenderer::SetAnimation(const char* animationTitle_)
{
	size_t hashTitle = GetHashFromString(animationTitle_);

	auto b = model->animations.find(hashTitle);
	bool found = model->animations.find(hashTitle) != model->animations.end();
	//bool found = model->IsAnimationExist(hashTitle);

	if (found)
	{
		RestartAnimation();
		modelAnimataion = &(*b).second;
		//modelAnimataion = &(model->animations[hashTitle]);
	}
	else
	{
		Warn("Failed to find animation %s.\n", animationTitle_);
	}
		
}
void MeshRenderer::SetAnimation(const std::string& animationTitle_)
{
	SetAnimation(animationTitle_.c_str());
}

void MeshRenderer::SetAnimationSpeed(float animationSpeed_)
{
	animationSpeed = animationSpeed_;
}

void MeshRenderer::SetModel(Model* model_, bool generateBones_)
{
	//if model exist, delete all bone gameobject and it's hirachacy
	if (model)
	{
		std::vector<GameObject*> getBone;

		for (auto& elem : owner->GetAllChildren())
		{
			if (elem->transform.IsBone())
				getBone.push_back(elem);
		}
		for (auto& elem : getBone)
		{
			//if (GetModelNameFromHash(elem.first) != "RootNode")
				//RM::RemoveGameObject(elem.second);
				RM::RemoveGameObject(elem);
		}
		boneObjects.clear();
	}

	model = model_;
	InitBone();

	if (generateBones_ && model && model->boneCount) {
		//set a default animation to the first one
		if (model->animations.size())
			modelAnimataion = &(*model->animations.begin()).second;

		//create gameobject bones
		CreateBoneGameObject(&model->iNode, owner);
	}

	RestartAnimation();
	//callbacks.clear();
	//callbacks.reserve(100);
	//auto size = model->animations.size();
	//for (unsigned i = 0; i < size; ++i)
	//  callbacks.push_back(std::map<unsigned, std::vector<ScriptCallback>>());
}
void MeshRenderer::SetModel(const char* modelname_models, bool generateBones_)
{
	Model*& modl = FindModel(modelname_models);
	SetModel(modl, generateBones_);
}
void MeshRenderer::SetModel(const std::string& modelname_models, bool generateBones_)
{
	SetModel(modelname_models.c_str(), generateBones_);
}

//void MeshRenderer::SetModelRuntime(Model * model_)
//{
//  model = model_;
//
//  timeElapsed = 0.f;
//  currentAnimation = 0;
//
//  callbacks.clear();
//  callbacks.reserve(100);
//  auto size = model->animations.size();
//  for (unsigned i = 0; i < size; ++i)
//    callbacks.push_back(std::map<unsigned, std::vector<ScriptCallback>>());
//
//}

void MeshRenderer::SetIShader(IShader* mat_)
{
	material = mat_;

	//uniformData.clear();
	//for (auto& uniform : material->->uniforms)
	//{
	//  std::string data;
	//  switch (uniform.second)
	//  {
	//  case UNIFORM_FLOAT:
	//    data = "0.0";
	//    break;
	//  case UNIFORM_FLOAT_VEC2:
	//    data = "0.0 0.0";
	//    break;
	//  case UNIFORM_FLOAT_VEC3:
	//    data = "0.0 0.0 0.0";
	//    break;
	//  case UNIFORM_FLOAT_VEC4:
	//    data = "0.0 0.0 0.0 0.0";
	//    break;
	//  case UNIFORM_INT:
	//    data = "1";
	//    break;
	//  case UNIFORM_BOOL:
	//    data = "0";
	//    break;
	//  }
	//
	//  uniformData[uniform.first] = data;
	//}
	
	timeElapsed = 0.f;
	//InitBoneToShader();
}
void MeshRenderer::SetIShader(const char* shader_glsl_) {
	SetIShader(FindShader(shader_glsl_));
}
void MeshRenderer::SetIShader(const std::string& shader_glsl_) {
	SetIShader(shader_glsl_.c_str());
}


//void MeshRenderer::SetAnimationByID(unsigned int id)
//{
//	finishedAnim = false;
//	prevTimeElapsed = timeElapsed;
//	timeElapsed = 0.f;
//	prevAnimation = currentAnimation;
//	currentAnimation = id;
//	currFrame = 0;
//	prevFrame = 0;
//
//	//if (blendTime > 0.f)
//	//{
//	//  blendTimer  = 0.f;
//	//  blending    = true;
//	//}
//
//	//auto maxFrame = model->animations[currentAnimation].channels[0].posKeyCount;
//	
//	//for (auto i = 0u; i < maxFrame; ++i)
//	//{
//	//  if (callbacks[currentAnimation].find(i) != callbacks[currentAnimation].end())
//	//  {
//	//    auto& callbackGroup = callbacks[currentAnimation][i];
//	//
//	//    for (auto& callback : callbackGroup)
//	//      if (!callback.triggerOnce)
//	//        callback.triggered = false;
//	//  }
//	//}
//}
//void MeshRenderer::SetModelSR(const std::string & model_)
//{
//  auto modl = FindModel(model_);
//
//  if (modl)
//    SetModelRuntime(modl);
//}

void MeshRenderer::SetDiffuse(const char* textureName_dds_) 
{
	textureMaps[0] = FindTexture(textureName_dds_);
}
void MeshRenderer::SetDiffuse(const std::string& textureName_dds_)
{
	SetDiffuse(textureName_dds_.c_str());
}

/*
void MeshRenderer::SetUniformData(const std::string & name, bool val)
{
	UNREFERENCED_PARAMETER(name);
	UNREFERENCED_PARAMETER(val);
 // unsigned hashedName = GetHashFromString(name);

  //auto it = uniformData.find(hashedName);
  //
  //if (it != uniformData.end())
  //{
  //  auto& uniforms = material->uniforms;
  //
  //  if (uniforms[hashedName] == UNIFORM_BOOL)
  //  {
  //    static char buf[BUFSIZ];
  //    ZeroMemory(buf, BUFSIZ);
  //
  //    sprintf_s(buf, BUFSIZ, "%d", val);
  //
  //    uniformData[hashedName] = buf;
  //  }
  //  else
  //  {
  //    printf("Invalid type! %s is not a bool!\n", name.c_str());
  //  }
  //}
  //else
  //  printf("Invalid uniform name! %s is not a material property!\n", name.c_str());
}

void MeshRenderer::SetUniformData(const std::string & name, float val)
{
	UNREFERENCED_PARAMETER(name);
	UNREFERENCED_PARAMETER(val);
  //unsigned hashedName = GetHashFromString(name);
  //
  //auto it = uniformData.find(hashedName);
  //
  //if (it != uniformData.end())
  //{
  //  auto& uniforms = material->uniforms;
  //
  //  if (uniforms[hashedName] == UNIFORM_FLOAT)
  //  {
  //    static char buf[BUFSIZ];
  //    ZeroMemory(buf, BUFSIZ);
  //
  //    sprintf_s(buf, BUFSIZ, "%f", val);
  //
  //    uniformData[hashedName] = buf;
  //  }
  //  else
  //  {
  //    printf("Invalid type! %s is not a float!\n", name.c_str());
  //  }
  //}
  //else
  //  printf("Invalid uniform name! %s is not a material property!\n", name.c_str());
}

void MeshRenderer::SetUniformData(const std::string & name, zg::Vector2 val)
{
	UNREFERENCED_PARAMETER(name);
	UNREFERENCED_PARAMETER(val);
  //unsigned hashedName = GetHashFromString(name);
  //
  //auto it = uniformData.find(hashedName);
  //
  //if (it != uniformData.end())
  //{
  //  auto& uniforms = material->shader->uniforms;
  //
  //  if (uniforms[hashedName] == UNIFORM_FLOAT_VEC2)
  //  {
  //    static char buf[BUFSIZ];
  //    ZeroMemory(buf, BUFSIZ);
  //
  //    sprintf_s(buf, BUFSIZ, "%f %f", val.x, val.y);
  //
  //    uniformData[hashedName] = buf;
  //  }
  //  else
  //  {
  //    printf("Invalid type! %s is not a zg::Vector2!\n", name.c_str());
  //  }
  //}
  //else
  //  printf("Invalid uniform name! %s is not a material property!\n", name.c_str());
}

void MeshRenderer::SetUniformData(const std::string & name, zg::Vector3 val)
{
	UNREFERENCED_PARAMETER(name);
	UNREFERENCED_PARAMETER(val);
  //unsigned hashedName = GetHashFromString(name);
  //
  //auto it = uniformData.find(hashedName);
  //
  //if (it != uniformData.end())
  //{
  //  auto& uniforms = material->shader->uniforms;
  //
  //  if (uniforms[hashedName] == UNIFORM_FLOAT_VEC3)
  //  {
  //    static char buf[BUFSIZ];
  //    ZeroMemory(buf, BUFSIZ);
  //
  //    sprintf_s(buf, BUFSIZ, "%f %f %f", val.x, val.y, val.z);
  //
  //    uniformData[hashedName] = buf;
  //  }
  //  else
  //  {
  //    printf("Invalid type! %s is not a zg::Vector3!\n", name.c_str());
  //  }
  //}
  //else
  //  printf("Invalid uniform name! %s is not a material property!\n", name.c_str());
}

void MeshRenderer::SetUniformData(const std::string & name, zg::Vector4 val)
{
	UNREFERENCED_PARAMETER(name);
	UNREFERENCED_PARAMETER(val);
  //unsigned hashedName = GetHashFromString(name);
  //
  //auto it = uniformData.find(hashedName);
  //
  //if (it != uniformData.end())
  //{
  //  auto& uniforms = material->shader->uniforms;
  //
  //  if (uniforms[hashedName] == UNIFORM_FLOAT_VEC4)
  //  {
  //    static char buf[BUFSIZ];
  //    ZeroMemory(buf, BUFSIZ);
  //
  //    sprintf_s(buf, BUFSIZ, "%f %f %f %f", val.x, val.y, val.z, val.w);
  //
  //    uniformData[hashedName] = buf;
  //  }
  //  else
  //  {
  //    printf("Invalid type! %s is not a zg::Vector4!\n", name.c_str());
  //  }
  //}
  //else
  //  printf("Invalid uniform name! %s is not a material property!\n", name.c_str());
}

bool MeshRenderer::GetUniformBool(const std::string & name)
{
	UNREFERENCED_PARAMETER(name);
  //int val = 0;
  //unsigned hashedName = GetHashFromString(name);
  //
  //auto it = uniformData.find(hashedName);
  //
  //if (it != uniformData.end())
  //{
  //  auto& uniforms = material->shader->uniforms;
  //
  //  if (uniforms[hashedName] == UNIFORM_BOOL)
  //  {
  //    static char buf[BUFSIZ];
  //    ZeroMemory(buf, BUFSIZ);
  //
  //    sscanf_s(it->second.c_str(), "%d", &val);
  //    return (bool)val;
  //  }
  //  else
  //  {
  //    printf("Invalid type! %s is not a bool!\n", name.c_str());
  //  }
  //}
  //else
  //  printf("Invalid uniform name! %s is not a material property!\n", name.c_str());

  return false;
}

float MeshRenderer::GetUniformFloat(const std::string & name)
{
	UNREFERENCED_PARAMETER(name);
  //float val = std::numeric_limits<float>().min();
  //unsigned hashedName = GetHashFromString(name);
  //
  //auto it = uniformData.find(hashedName);
  //
  //if (it != uniformData.end())
  //{
  //  auto& uniforms = material->shader->uniforms;
  //
  //  if (uniforms[hashedName] == UNIFORM_FLOAT)
  //  {
  //    static char buf[BUFSIZ];
  //    ZeroMemory(buf, BUFSIZ);
  //
  //    sscanf_s(it->second.c_str(), "%f", &val);
  //    return val;
  //  }
  //  else
  //  {
  //    printf("Invalid type! %s is not a float!\n", name.c_str());
  //  }
  //}
  //else
  //  printf("Invalid uniform name! %s is not a material property!\n", name.c_str());

  return std::numeric_limits<float>().min();
}

zg::Vector2 MeshRenderer::GetUniformVec2(const std::string & name)
{
	UNREFERENCED_PARAMETER(name);
  zg::Vector2 val{ std::numeric_limits<float>().min(), std::numeric_limits<float>().min() };
  //unsigned hashedName = GetHashFromString(name);
  //
  //auto it = uniformData.find(hashedName);
  //
  //if (it != uniformData.end())
  //{
  //  auto& uniforms = material->shader->uniforms;
  //
  //  if (uniforms[hashedName] == UNIFORM_FLOAT_VEC2)
  //  {
  //
  //    static char buf[BUFSIZ];
  //    ZeroMemory(buf, BUFSIZ);
  //
  //    sscanf_s(it->second.c_str(), "%f %f", &val.x, &val.y);
  //  }
  //  else
  //  {
  //    printf("Invalid type! %s is not a zg::Vector2!\n", name.c_str());
  //  }
  //}
  //else
  //  printf("Invalid uniform name! %s is not a material property!\n", name.c_str());

  return val;
}

zg::Vector3 MeshRenderer::GetUniformVec3(const std::string & name)
{
	UNREFERENCED_PARAMETER(name);
  zg::Vector3 val{ std::numeric_limits<float>().min(),
               std::numeric_limits<float>().min(),
               std::numeric_limits<float>().min()};

  //unsigned hashedName = GetHashFromString(name);
  //
  //auto it = uniformData.find(hashedName);
  //
  //if (it != uniformData.end())
  //{
  //  auto& uniforms = material->shader->uniforms;
  //
  //  if (uniforms[hashedName] == UNIFORM_FLOAT_VEC3)
  //  {
  //    static char buf[BUFSIZ];
  //    ZeroMemory(buf, BUFSIZ);
  //
  //    sscanf_s(it->second.c_str(), "%f %f %f", &val.x, &val.y, &val.z);
  //  }
  //  else
  //  {
  //    printf("Invalid type! %s is not a zg::Vector3!\n", name.c_str());
  //  }
  //}
  //else
  //  printf("Invalid uniform name! %s is not a material property!\n", name.c_str());

  return val;
}

zg::Vector4 MeshRenderer::GetUniformVec4(const std::string & name)
{
	UNREFERENCED_PARAMETER(name);
  zg::Vector4 val{ std::numeric_limits<float>().min(),
               std::numeric_limits<float>().min(),
               std::numeric_limits<float>().min(),
               std::numeric_limits<float>().min() };

  //unsigned hashedName = GetHashFromString(name);
  //
  //auto it = uniformData.find(hashedName);
  //
  //if (it != uniformData.end())
  //{
  //  auto& uniforms = material->shader->uniforms;
  //
  //  if (uniforms[hashedName] == UNIFORM_FLOAT_VEC4)
  //  {
  //    static char buf[BUFSIZ];
  //    ZeroMemory(buf, BUFSIZ);
  //
  //    sscanf_s(it->second.c_str(), "%f %f %f %f", &val.x, &val.y, &val.z, &val.w);
  //  }
  //  else
  //  {
  //    printf("Invalid type! %s is not a zg::Vector4!\n", name.c_str());
  //  }
  //}
  //else
  //  printf("Invalid uniform name! %s is not a material property!\n", name.c_str());

  return val;
}

*/

//void MeshRenderer::PlayAnimationOnce(const std::string & anim)
//{
//  prevAnim = currentAnimation;
//  prevLoop = toggleLoop;
//  toggleLoop = false;
//  revert = true;
//
//  SetAnimation(anim);
//}

//void MeshRenderer::BlendAnimation(float factor, const INode* node, const zg::Matrix44 & pTrans)
//{
//  const BoneAnimationChannel* channel = nullptr;
//  const BoneAnimationChannel* pChannel = nullptr;
//
//  if (model->channelMap[currentAnimation].find(node->name) != model->channelMap[currentAnimation].end())
//    channel = model->channelMap[currentAnimation][node->name];
//
//  if (model->channelMap[prevAnimation].find(node->name) != model->channelMap[prevAnimation].end())
//    pChannel = model->channelMap[prevAnimation][node->name];
//
//  auto nTrans = node->transform;
//
//  if (channel && pChannel)
//  {
//    const auto& anims = model->animations;
//    auto dur = anims[currentAnimation].duration;
//    float timeInTicks = prevTimeElapsed * (anims[currentAnimation].ticksPerSecond != 0 ? anims[currentAnimation].ticksPerSecond : 25.f);
//    float animTime = toggleLoop ? fmod(timeInTicks, dur) : timeInTicks > dur ? dur : timeInTicks;
//
//    auto tV = Lerp(CalculatePosition(animTime, pChannel), CalculatePosition(0.f, channel), factor);
//    auto rQ = zg::Quaternion::Slerp(CalculateRotation(animTime, pChannel), CalculateRotation(0.f, channel), factor);
//    auto sV = Lerp(CalculateScale(animTime, pChannel), CalculateScale(0.f, channel), factor);
//
//    auto tM = BuildTranslate4x4(tV);
//    auto rM = rQ.GetMatrix44();
//    auto sM = BuildScale4x4(sV);
//
//    nTrans = tM * rM * sM;
//  }
//
//  auto gTrans = pTrans * nTrans;
//
//  if (model->OldboneOffsetMap.find(node->name) != model->OldboneOffsetMap.end())
//  {
//    auto id = model->OldboneOffsetMap[node->name];
//    boneTransforms[id] = gTrans * model->boneOffsets[id];
//    if (boneObjects[node->name])
//    {
//      auto out        = owner->transform.GetMatrix() * model->scalingMat * gTrans;
//      auto outWOScale = owner->transform.GetMatrix() * gTrans;
//
//      boneObjects[node->name]->transform.worldMatrix  = out;
//      boneObjects[node->name]->transform.position		= GetTranslation(out);
//      boneObjects[node->name]->transform.rotation		= GetRotation(out);
//      boneObjects[node->name]->transform.localScale		= GetScale(outWOScale);
//      boneObjects[node->name]->transform.scale = zg::Vector3{ 1.f, 1.f, 1.f };
//    }
//  }
//
//  for (const auto& elem : node->childNodes)
//    BlendAnimation(factor, &elem, gTrans);
//}
//
//void MeshRenderer::CopyAnimationInfo(const MeshRenderer* rhs)
//{
//  animating = false;
//  SetModelRuntime(rhs->model);
//  SetIShader(rhs->material);
//  textureMaps[0] = rhs->textureMaps[0];
//  textureMaps[1] = rhs->textureMaps[1];
//  textureMaps[2] = rhs->textureMaps[2];
//  textureMaps[3] = rhs->textureMaps[3];
//  
//  boneTransforms  = rhs->boneTransforms;
//  timeElapsed     = rhs->timeElapsed;
//  prevTimeElapsed = rhs->prevTimeElapsed;
//  animationSpeed       = rhs->animationSpeed;
//  finishedAnim    = rhs->finishedAnim;
//  blendTime       = rhs->blendTime;
//  blendTimer      = rhs->blendTimer;
//  toggleLoop        = rhs->toggleLoop;
//  revert          = rhs->revert;
//  prevAnim        = rhs->prevAnim;
//  prevAnimation   = rhs->prevAnimation;
//}

//void MeshRenderer::ResetBoneObjects()
//{
//	if(model->hasAnims)
//		UpdateBones(0.f, &model->iNode, zg::Matrix44{});
//}

#ifdef TINYXML2_EXPORT
//serial
void MeshRenderer::Serial(FXMLDocument* doc, FXMLNode* root) {
	FXMLElement* pElem = doc->NewElement(IS_MESHRENDERER);

	Component::Serial(doc, pElem);
	//mesh
	FXMLElement* pSubElem = doc->NewElement("mesh");
	pSubElem->SetAttribute("name", model->filename.c_str());
	pElem->InsertEndChild(pSubElem);
	//material
	pSubElem = doc->NewElement("material");
	pSubElem->SetAttribute("name", material->filename.c_str());
	pSubElem->SetAttribute("diffuse", textureMaps[0]->filename.c_str());
	pSubElem->SetAttribute("specular", textureMaps[1]->filename.c_str());
	pSubElem->SetAttribute("normal", textureMaps[2]->filename.c_str());
	pSubElem->SetAttribute("emissive", textureMaps[3]->filename.c_str());
	pElem->InsertEndChild(pSubElem);
	//color
	pSubElem = doc->NewElement("color");
	pSubElem->SetAttribute("r", color.r);
	pSubElem->SetAttribute("g", color.g);
	pSubElem->SetAttribute("b", color.b);
	pSubElem->SetAttribute("a", color.a);
	//pSubElem->SetAttribute("intensity", intensity);
	pElem->InsertEndChild(pSubElem);
	//attribute
	pSubElem = doc->NewElement("attribute");
	pSubElem->SetAttribute("isVisible", isVisible);
	pElem->InsertEndChild(pSubElem);
	//attribute
	pSubElem = doc->NewElement("misc");
	//pSubElem->SetAttribute("castsShadows", castShadows);
	pElem->InsertEndChild(pSubElem);
	//attribute
	pSubElem = doc->NewElement("anim");
	//pSubElem->SetAttribute("currentAnim", currentAnimation);
	pSubElem->SetAttribute("animationSpeed", animationSpeed);
	pSubElem->SetAttribute("toggleLoop", toggleLoop);
	//pSubElem->SetAttribute("blendTime", blendTime);
	//pSubElem->SetAttribute("animating", animating);
	//pSubElem->SetAttribute("receiveShadows", receiveShadows);
	//pSubElem->SetAttribute("cullFront", cullFront);
	//pSubElem->SetAttribute("postProcess", postProcess);
	//pSubElem->SetAttribute("justDraw", justDraw);
	//pSubElem->SetAttribute("bloom", bloom);
	pElem->InsertEndChild(pSubElem);
	//uvOffset
	pSubElem = doc->NewElement("uvOffset");
	//pSubElem->SetAttribute("x", uvOffset.x);
	//pSubElem->SetAttribute("y", uvOffset.x);
	pElem->InsertEndChild(pSubElem);

	pSubElem = doc->NewElement("uniforms");
	//for (auto& elem : uniformData)
	//{
	//  auto name = GetStringFromHash(elem.first).c_str();
	//  pSubElem->SetAttribute(name, elem.second.c_str());
	//}
	pElem->InsertEndChild(pSubElem);

	//pSubElem = doc->NewElement("callbacks");
	//auto callbackSize = callbacks.size();
	//for (auto i = 0u; i < callbackSize; ++i)
	//{
	//  auto animName = GetStringFromHash(model->animations[i].name);
	//  auto pAnim = doc->NewElement(animName.c_str());
	//  for (auto& frameCallback : callbacks[i])
	//  {
	//    auto pFrame = doc->NewElement("Frame");
	//    pFrame->SetAttribute("frame", frameCallback.first);
	//    for (auto& callback : frameCallback.second)
	//    {
	//      auto pCallBack = doc->NewElement("Callback");
	//      pCallBack->SetAttribute("GameObject", callback.goID);
	//      pCallBack->SetAttribute("Script", callback.sname.c_str());
	//      pCallBack->SetAttribute("Function", callback.fname.c_str());
	//      pCallBack->SetAttribute("TriggerOnce", callback.triggerOnce);
	//
	//      pFrame->InsertEndChild(pCallBack);
	//    }
	//
	//    pAnim->InsertEndChild(pFrame);
	//  }
	//
	//  pSubElem->InsertEndChild(pAnim);
	//}
	// pElem->InsertEndChild(pSubElem);

	root->InsertEndChild(pElem);
}
void MeshRenderer::Unserial(FXMLElement* pElem) {
	//CommonSprite
	Component::Unserial(pElem);
	//mesh
	FXMLElement* pSubElem = pElem->FirstChildElement("mesh");
	model = FindModel(pSubElem->Attribute("name"));
	InitBone();
	//material
	pSubElem = pSubElem->NextSiblingElement();
	SetIShader(FindShader(pSubElem->Attribute("name")));
	textureMaps[0] = FindTexture(pSubElem->Attribute("diffuse"));
	textureMaps[1] = FindTexture(pSubElem->Attribute("specular"));
	auto nMap = pSubElem->Attribute("normal");

	if (!strcmp(nMap, "null.dds"))
		nMap = "n_null.dds";

	textureMaps[2] = FindTexture(nMap);
	if (pSubElem->Attribute("emissive"))
	{
		auto eMap = pSubElem->Attribute("emissive");
		if (!strcmp(eMap, "null.dds"))
			eMap = "black.dds";

		textureMaps[3] = FindTexture(eMap);
	}
	//color
	pSubElem = pSubElem->NextSiblingElement();
	color.r = pSubElem->FloatAttribute("r");
	color.g = pSubElem->FloatAttribute("g");
	color.b = pSubElem->FloatAttribute("b");
	color.a = pSubElem->FloatAttribute("a");
	//intensity  = pSubElem->FloatAttribute("intensity", 1.f);
	//visibility
	pSubElem = pSubElem->NextSiblingElement();
	isVisible = pSubElem->BoolAttribute("isVisible");
	//casts shadow
	pSubElem = pSubElem->NextSiblingElement();
	//castShadows = pSubElem->BoolAttribute("castsShadows");
	//animation properties
	pSubElem = pSubElem->NextSiblingElement();
	//currentAnimation = pSubElem->IntAttribute("currentAnim");
	animationSpeed = pSubElem->FloatAttribute("animationSpeed");
	toggleLoop = pSubElem->BoolAttribute("toggleLoop");
	//blendTime = pSubElem->FloatAttribute("blendTime");
	//animating = pSubElem->BoolAttribute("animating");
	//receiveShadows = pSubElem->BoolAttribute("receiveShadows", true);
	//cullFront = pSubElem->BoolAttribute("cullFront");
	//postProcess = pSubElem->BoolAttribute("postProcess");
	//justDraw = pSubElem->BoolAttribute("justDraw");
	//bloom = pSubElem->BoolAttribute("bloom", true);

	//uvOffset
	pSubElem = pSubElem->NextSiblingElement();
	if (pSubElem)
	{
		//uvOffset.x = pSubElem->FloatAttribute("x");
		//uvOffset.y = pSubElem->FloatAttribute("y");
	}
	else
		return;
	pSubElem = pSubElem->NextSiblingElement();
	if (pSubElem)
	{
		//for (auto& elem : uniformData)
		//{
		//  auto name = GetStringFromHash(elem.first).c_str();
		//  auto val = pSubElem->Attribute(name);
		//
		//  if(val)
		//   elem.second = val;
		//}
	}
	else
		return;

	//callbacks.clear();
	//callbacks.reserve(100);
	//auto size = model->animations.size();
	//for (unsigned i = 0; i < size; ++i)
	//  callbacks.push_back(std::map<unsigned, std::vector<ScriptCallback>>());
	//
	//pSubElem = pSubElem->NextSiblingElement();
	//if (pSubElem)
	//{
	//  auto pAnim = pSubElem->FirstChildElement();
	//
	//  for (unsigned i = 0; i < size && pAnim; ++i)
	//  {
	//    auto pFrame = pAnim->FirstChildElement();
	//
	//    while (pFrame)
	//    {
	//      auto frameID = pFrame->IntAttribute("frame");
	//
	//      auto pCallBack = pFrame->FirstChildElement();
	//      while (pCallBack)
	//      {
	//        callbacks[i][frameID].emplace_back();
	//        auto& callback       = callbacks[i][frameID].back();
	//        callback.goID        = pCallBack->IntAttribute("GameObject");
	//        callback.sname       = pCallBack->Attribute("Script");
	//        callback.fname       = pCallBack->Attribute("Function");
	//        callback.triggerOnce = pCallBack->BoolAttribute("TriggerOnce");
	//
	//        pCallBack = pCallBack->NextSiblingElement();
	//      }
	//
	//      pFrame = pFrame->NextSiblingElement();
	//    }
	//
	//    pAnim = pAnim->NextSiblingElement();
	//  }
	//}
	//else
	//  return;
}
#endif

#ifdef USING_SOL2
void MeshRenderer::BindLua(sol::state & lua)
{
	lua.new_usertype<MeshRenderer>("MeshRenderer",
		sol::constructors<MeshRenderer(), MeshRenderer(const MeshRenderer&)>(),
		//Component
		"IsEnable", &MeshRenderer::IsEnable,
		"SetEnable", &MeshRenderer::SetEnable,
		"Owner", &MeshRenderer::Owner,

		"Transformation", &MeshRenderer::Transformation,
		"SetAnimation", &MeshRenderer::SetAnimation,
		"SetModel", &MeshRenderer::SetModelSR,
		"SetIShader", &MeshRenderer::SetIShaderS,
		"SetAnimationSpeed", &MeshRenderer::SetAnimationSpeed,
		"SetTexture", &MeshRenderer::SetDiffuse,
		"PlayAnimationOnce", &MeshRenderer::PlayAnimationOnce,
		"IsCurrentAnimation", &MeshRenderer::IsCurrentAnim,
		"animationSpeed", &MeshRenderer::animationSpeed,
		"color", &MeshRenderer::color,
		"uvOffset", &MeshRenderer::uvOffset,
		"isVisible", &MeshRenderer::isVisible,
		"castShadows", &MeshRenderer::castShadows,
		"toggleLoop", &MeshRenderer::toggleLoop,
		"finishedAnimating", &MeshRenderer::finishedAnim,
		"blendTime", &MeshRenderer::blendTime,
		"animating", &MeshRenderer::animating,
		"intensity", &MeshRenderer::intensity,

		"SetUniformData", sol::overload(
			static_cast<void (MeshRenderer::*)(const std::string&, bool)>(&MeshRenderer::SetUniformData),
			static_cast<void (MeshRenderer::*)(const std::string&, float)>(&MeshRenderer::SetUniformData),
			static_cast<void (MeshRenderer::*)(const std::string&, zg::Vector2)>(&MeshRenderer::SetUniformData),
			static_cast<void (MeshRenderer::*)(const std::string&, zg::Vector3)>(&MeshRenderer::SetUniformData),
			static_cast<void (MeshRenderer::*)(const std::string&, zg::Vector4)>(&MeshRenderer::SetUniformData)
		),
		"GetUniformBool", &MeshRenderer::GetUniformBool,
		"GetUniformFloat", &MeshRenderer::GetUniformFloat,
		"GetUniformVec2", &MeshRenderer::GetUniformVec2,
		"GetUniformVec3", &MeshRenderer::GetUniformVec3,
		"GetUniformVec4", &MeshRenderer::GetUniformVec4,
		"CopyAnimationInfo", &MeshRenderer::CopyAnimationInfo,
		"RestartAnimation", &MeshRenderer::RestartAnimation,

		"Red", &MeshRenderer::Red,
		"Black", &MeshRenderer::Black,
		"White", &MeshRenderer::White
		);
}

//void MeshRenderer::InitCallbacks()
//{
//  for (auto& anim : callbacks)
//  {
//    for (auto& frame : anim)
//    {
//      for (auto& callback : frame.second)
//      {
//        if (callback.goID == -2)
//          callback.go = owner;
//        else
//          callback.go = FindGameObjectByID(callback.goID);
//  
//        if (callback.go && callback.sname != "")
//          callback.script = &callback.go->GetScript(callback.sname);
//        else
//        {
//          callback.sname = "";
//          callback.fname = "";
//        }
//      }
//    }
//  }
//}
//void MeshRenderer::SetFrame(int frame)
//{
//  UNREFERENCED_PARAMETER(frame);
//  //currFrame = frame;
//  //prevFrame = frame;
//  //
//  //const auto& anims = model->animations;
//  //float timeInTicks = timeElapsed * (anims[currentAnimation].ticksPerSecond != 0 ? anims[currentAnimation].ticksPerSecond : 25.f);
//  //float animTime = toggleLoop ? fmod(timeInTicks, dur) : timeInTicks > dur ? dur : timeInTicks;
//  //
//  //
//  //timeElapsed = 
//}

//void ScriptCallback::Trigger()
//{
//  if (!triggered && go && script && !fname.empty())
//  {
//    (*script)[fname]();
//    triggered = true;
//  }
//}
//
//void ScriptCallback::Reset()
//{
//  if(!triggerOnce)
//    triggered = false;
//}

#endif

