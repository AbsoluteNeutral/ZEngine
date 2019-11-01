

#ifndef _MESHRENDERER_H
#define _MESHRENDERER_H

#include "Component.h"
#include "Model.h"
//#include <tuple>

#ifdef USING_SOL2
//struct ZG_API ScriptCallback
//{
//  ScriptCallback() 
//    : go{ nullptr },
//      timer{ 0.f },
//      goID{ -1 },
//      script{ nullptr },
//      sname{ "" },
//      fname{ "" },
//      triggerOnce{ false },
//      triggered{ false }
//  {
//  }
//  GameObject*       go;
//  float             timer;
//  int               goID;
//  sol::environment* script;
//  std::string       sname;
//  std::string       fname;
//  bool              triggerOnce;
//  bool              triggered;
//
//  void Trigger();
//  void Reset();
//};

#endif

class ZG_API MeshRenderer : public Component
{
public:
	MeshRenderer			(Model* mesh_ = nullptr, IShader* material_ = nullptr, const zg::Color& color_ = CC_WHITE) noexcept;
	MeshRenderer			(const MeshRenderer& rhs);
	~MeshRenderer			();
	MeshRenderer& operator=	(const MeshRenderer& rhs);
	
	// ____________________________________________________________ init
	void Attach	(GameObject* obj_, size_t arrayIndex_,
				const char* modelname_	= "null.models", 
				const char* diffuse_	= "null.dds",
				const char* material_	= "animation.glsl", 
				bool generateBones_		= true, 
				const zg::Color& color_ = CC_WHITE);
	void Init	(const char* modelname_, const char* diffuseName_, const char* glsl_,  bool generateBones_, const zg::Color& color_ = CC_WHITE);
	//MeshRenderer* Clone	();
private:	
	void InitBone			();
	void InitBoneToShader	();
	void CreateBoneGameObject(const INode* node, GameObject*& parent);

	// ____________________________________________________________ etc function
public:
	//void ShadowDraw		(const IShader * m);
	//void Draw				(const Camera* cam, LightSource* ls = nullptr);
	void Draw				(const Camera* cam);
	void DrawWireFrame		(const Camera* cam, const zg::Color& color_ = CC_GRAY_40);
	//void DrawPost			(const Camera* cam);
	void RestartAnimation	();
	void RenderSkeleton		();
	//void Update			(bool fireCallback = false);
	void Update				();
	void PostUpdate			();

	void Pause();
	void Play();
	void Stop();
private:
	//void UpdateBones			(float time, const INode* node, const zg::Matrix44& ptrans, bool fireCallback);
	//void UpdateBones			(float time, const INode* node, const zg::Matrix44& ptrans, zg::Matrix44& scaleFactor_);
	void UpdateBonesAnimation	(float time, const INode* node, const zg::Matrix44& ptrans, zg::Vector3& scaleFactor_);
	void UpdateBonesIK			(const INode* node, const zg::Matrix44& ptrans, zg::Vector3& scaleFactor_);

	// ____________________________________________________________ getters
public:
	std::string GetAnimationTitle	() const;
	Model*		GetModel			();
	IShader*	GetIShader			();
	int			GetAnimationCycles	() const;
	int			GetAnimationCycles	(const char* animationTitle_) const;
	bool		IsCurrentAnim		(const char* animationName_) const;
	bool		IsCurrentAnim		(const std::string& animationName_) const;
	
	// ____________________________________________________________ setters
	void SetAnimation		(const char* animationTitle_);
	void SetAnimation		(const std::string& animationTitle_);

	void SetAnimationSpeed	(float animationSpeed_);

	void SetModel			(Model* model_, bool generateBones_);
	void SetModel			(const char* modelname_models, bool generateBones_);
	void SetModel			(const std::string& modelname_models, bool generateBones_);
	//void SetModelRuntime	(Model* model_);
	//void SetModelSR		(const std::string& model);

	void SetIShader			(IShader* shader_ptr_);
	void SetIShader			(const char* shader_glsl_);
	void SetIShader			(const std::string& shader_glsl_);
	
	//void SetAnimationByID	(unsigned int id);
	void SetDiffuse			(const char* textureName_dds_);
	void SetDiffuse			(const std::string& textureName_dds_);
	
	Model*		 model;
	IShader*	 material;
	float		 timeElapsed;
	float		 animationSpeed;
	//unsigned int currentAnimation;
	unsigned int boneLocations[MAX_BONES];
	
	zg::Color     color;
	bool          isVisible;
	//bool          castShadows;
	//bool          revert;
	bool          toggleLoop;
	bool          prevLoop;
	bool          finishedAnim;
	//bool          blending;
	//bool          animating;
	//bool          receiveShadows;
	//bool          cullFront;
	//bool          postProcess;
	//bool          justDraw;
	//bool          bloom;
	//unsigned int  prevAnim;
	unsigned int  currFrame;
	unsigned int  prevFrame;
	//zg::Vector2   uvOffset;
	//float         blendTime;
	//float         blendTimer;
	float         prevTimeElapsed;
	//float         intensity;
	//unsigned int  prevAnimation;
	Texture2D*    textureMaps[4];
	
	std::vector<zg::Matrix44> boneTransforms;
	std::map<size_t, GameObject*> boneObjects;
	//std::map<unsigned, std::string> uniformData;
	//std::vector<std::map<unsigned, std::vector<ScriptCallback>>> callbacks;

	/*
	void SetUniformData(const std::string& name, bool val);
	void SetUniformData(const std::string& name, float val);
	void SetUniformData(const std::string& name, zg::Vector2 val);
	void SetUniformData(const std::string& name, zg::Vector3 val);
	void SetUniformData(const std::string& name, zg::Vector4 val);
	bool GetUniformBool(const std::string& name);
	float GetUniformFloat(const std::string& name);
	zg::Vector2 GetUniformVec2(const std::string& name);
	zg::Vector3 GetUniformVec3(const std::string& name);
	zg::Vector4 GetUniformVec4(const std::string& name);
	void PlayAnimationOnce(const std::string& anim);
	*/

	//void CopyAnimationInfo(const MeshRenderer* rhs);
	//void AddBoneObjects(GameObject* child);
	//void ResetBoneObjects();
	
	void SetFrame(int frame);

	// ____________________________________________________________ serial
#ifdef TINYXML2_EXPORT
	void Serial(FXMLDocument* doc, FXMLNode* root);
	void Unserial(FXMLElement* pElem);
#endif

#ifdef USING_SOL2
	//void InitCallbacks();
	static void BindLua(sol::state& lua);
#endif

private:
	Animation* modelAnimataion;
	//void BlendAnimation(float factor, const INode* node, const zg::Matrix44 & pTrans);
};

#endif
