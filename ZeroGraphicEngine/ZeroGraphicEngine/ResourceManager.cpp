
#include "stdafx.h"
#include "ResourceManager.h"
#include "Engine.h"

namespace RM {

	ZG_API AudioListener* audioListener = nullptr;
	//ZG_API LightSource lightsource[NUM_OF_LIGHTS] {};

#if defined(_EDITOR_ON) || !defined(_EDITOR_ON)
	ZG_API std::vector<PickingCircle*>	R_PickingCircle{};
	ZG_API std::vector<GameObject*>		R_GameObject{};
	ZG_API std::vector<Camera*>			R_Camera{};
	ZG_API std::vector<MeshRenderer*>	R_MeshRenderer{};
	//ZG_API std::vector<Sprite2D*>		R_Sprite2D{};
	//ZG_API std::vector<ParticleEmitter*>	R_ParticleEmitter{};
	//ZG_API std::vector<AABB*>			R_AABB{};
	//ZG_API std::vector<BC*>				R_BC{};
	//ZG_API std::vector<OBB*>				R_OBB{};
	//ZG_API std::vector<CapsuleCollider*>	R_CapsuleCollider{};
	//ZG_API std::vector<RigidBody*>		R_RigidBody{};
	//ZG_API std::vector<Script*>			R_Script{};
	//ZG_API std::vector<Sound*>			R_Sound{};
	//ZG_API std::vector<TextRenderer*>	R_TextRenderer{};
	//ZG_API std::vector<Light*>			R_Light{};
	//ZG_API std::vector<UISprite*>		R_UISprite{};
	//ZG_API std::vector<VideoCompo*>		R_VideoCompo{};

#else
	ZG_API unsigned C_GameObject = 0;
	ZG_API unsigned C_Camera = 0;
	ZG_API unsigned C_MeshRenderer = 0;
	ZG_API unsigned C_Sprite2D = 0;
	ZG_API unsigned C_AABB = 0;
	ZG_API unsigned C_BC = 0;
	ZG_API unsigned C_CapsuleCollider = 0;
	ZG_API unsigned C_OBB = 0;
	ZG_API unsigned C_RigidBody = 0;
	ZG_API unsigned C_Sound = 0;
	ZG_API unsigned C_Script = 0;
	ZG_API unsigned C_ParticleEmitter = 0;
	ZG_API unsigned C_TextRenderer = 0;
	ZG_API unsigned C_Light = 0;
	ZG_API unsigned C_UISprite = 0;
	ZG_API unsigned C_VideoCompo = 0;

	ZG_API GameObject*			R_GameObject = nullptr;
	ZG_API Camera*				R_Camera = nullptr;
	ZG_API MeshRenderer*			R_MeshRenderer = nullptr;
	ZG_API Sprite2D*				R_Sprite2D = nullptr;
	ZG_API ParticleEmitter*		R_ParticleEmitter = nullptr;
	ZG_API AABB*					R_AABB = nullptr;
	ZG_API BC*					R_BC = nullptr;
	ZG_API CapsuleCollider*		R_CapsuleCollider = nullptr;
	ZG_API OBB*					R_OBB = nullptr;
	ZG_API RigidBody*			R_RigidBody = nullptr;
	ZG_API Sound*				R_Sound = nullptr;
	ZG_API Script*				R_Script = nullptr;
	ZG_API TextRenderer*			R_TextRenderer = nullptr;
	ZG_API Light*				R_Light = nullptr;
	ZG_API UISprite*				R_UISprite = nullptr;
	ZG_API VideoCompo*			R_VideoCompo = nullptr;

#endif
	/*
void InitArchetypeCallbacks(std::vector<GameObject*>& tmpArchObjs)
{
	 Set up mesh renderer
	for (auto& elem : tmpArchObjs)
	{
		if (elem->GetComponentID<MeshRenderer>() != COMPO_NOT_EXIST)
		{
			MeshRenderer* mesh = elem->GetMeshRenderer();

			for (auto& anim : mesh->callbacks)
			{
				for (auto& frame : anim)
				{
					for (auto& callback : frame.second)
					{
						if (callback.goID == -2)
							callback.go = elem;
						else
						{
							for (auto& tmpMeshObj : tmpArchObjs)
							{
								if (tmpMeshObj->GetID() == callback.goID)
									callback.go = tmpMeshObj;
							}
						}

						if (callback.go && callback.sname != "")
							callback.script = &callback.go->GetScript(callback.sname);
						else
						{
							callback.sname = "";
							callback.fname = "";
						}
					}
				}
			}
		}
	}
}

void UpdateArchetypeCallbacksID(std::vector<GameObject*>& tmpArchObjs)
{
	for (auto& elem : tmpArchObjs)
	{
		if (elem->GetComponentID<MeshRenderer>() != COMPO_NOT_EXIST)
		{
			MeshRenderer* mesh = elem->GetMeshRenderer();

			for (auto& anim : mesh->callbacks)
				for (auto& frame : anim)
					for (auto& callback : frame.second)
						if (callback.go)
							callback.goID = callback.go->GetID();
		}
	}
}

void InitArchetypeBones(GameObject* rootObj)
{
	if (rootObj->GetComponentID<MeshRenderer>() != COMPO_NOT_EXIST)
	{
		MeshRenderer* m = rootObj->GetMeshRenderer();

		auto& children = rootObj->GetAllChildren();
		for (auto& child : children)
			m->AddBoneObjects(child);
	}
}
*/

	void ClearResourceVaribles() {
		//Engine::sys_Logic.ClearGarbarge();
#if defined(_EDITOR_ON) || !defined(_EDITOR_ON)
		//for (auto elem : R_Sound)			elem->Stop();

		for (auto elem : R_PickingCircle)			DeallocateOne(elem);

		for (auto elem : R_GameObject)		DeallocateOne(elem);
		for (auto elem : R_Camera)			DeallocateOne(elem);
		for (auto elem : R_MeshRenderer)	DeallocateOne(elem);
		//for (auto elem : R_Sprite2D)		DeallocateOne(elem);
		//for (auto elem : R_ParticleEmitter)	DeallocateOne(elem);
		//for (auto elem : R_AABB)			DeallocateOne(elem);
		//for (auto elem : R_BC)				DeallocateOne(elem);
		//for (auto elem : R_CapsuleCollider)	DeallocateOne(elem);
		//for (auto elem : R_OBB)				DeallocateOne(elem);
		//for (auto elem : R_RigidBody)		DeallocateOne(elem);
		//for (auto elem : R_Script)			DeallocateOne(elem);
		//for (auto elem : R_Sound)			DeallocateOne(elem);
		//for (auto elem : R_TextRenderer)	DeallocateOne(elem);
		//for (auto elem : R_Light)			DeallocateOne(elem);
		//for (auto elem : R_UISprite)		DeallocateOne(elem);
		//for (auto elem : R_VideoCompo)		DeallocateOne(elem);

		R_GameObject.clear();
		R_Camera.clear();
		R_MeshRenderer.clear();
		//R_Sprite2D.clear();
		//R_ParticleEmitter.clear();
		//R_AABB.clear();
		//R_BC.clear();
		//R_CapsuleCollider.clear();
		//R_OBB.clear();
		//R_RigidBody.clear();
		//R_Script.clear();
		//R_Sound.clear();
		//R_TextRenderer.clear();
		//R_Light.clear();
		//R_UISprite.clear();
		//R_VideoCompo.clear();

		SetDefaultCamera();
#else
		{
			Sound* b = R_Sound;
			Sound* e = R_Sound + C_Sound;
			while (b != e) {
				b->Stop();
				++b;
			}
		}

		Deallocate(R_GameObject, C_GameObject);
		Deallocate(R_Camera, C_Camera);
		Deallocate(R_MeshRenderer, C_MeshRenderer);
		Deallocate(R_Sprite2D, C_Sprite2D);
		Deallocate(R_ParticleEmitter, C_ParticleEmitter);
		Deallocate(R_AABB, C_AABB);
		Deallocate(R_BC, C_BC);
		Deallocate(R_CapsuleCollider, C_CapsuleCollider);
		Deallocate(R_OBB, C_OBB);
		Deallocate(R_RigidBody, C_RigidBody);
		Deallocate(R_Sound, C_Sound);
		Deallocate(R_Script, C_Script);
		Deallocate(R_TextRenderer, C_TextRenderer);
		Deallocate(R_Light, C_Light);
		Deallocate(R_UISprite, C_UISprite);
		Deallocate(R_VideoCompo, C_VideoCompo);

		C_GameObject =
			C_Camera =
			C_MeshRenderer =
			C_Sprite2D =
			C_ParticleEmitter =
			C_AABB =
			C_BC =
			C_CapsuleCollider =
			C_OBB =
			C_RigidBody =
			C_Script =
			C_Sound =
			C_TextRenderer =
			C_Light =
			C_UISprite =
			C_VideoCompo =
			0;
#endif
		//ClearObjPool();
	}

#if defined(_EDITOR_ON) || !defined(_EDITOR_ON)
	//object creation
	PickingCircle* CreateGameObjectWithPickingCircle(GameObject*& gameObjectGetter_, const char* gameobject_name_) 
	{
		gameObjectGetter_		= Allocate<GameObject>(1);
		PickingCircle* picker	= Allocate<PickingCircle>(1);

		gameObjectGetter_->name = gameobject_name_;
		picker->Attach(gameObjectGetter_);
		R_GameObject.push_back(gameObjectGetter_);
		R_PickingCircle.push_back(picker);

		return picker;
	}

	GameObject*	CreateGameObject(const char* gameobject_name_) {
		GameObject* obj = Allocate<GameObject>(1);
		obj->name = gameobject_name_;
		R_GameObject.push_back(obj);
		return obj;
	}

	void	CreateMeshRenderer(GameObject* gameobject_, const char* modelname_, const char* diffuse_) {
		MeshRenderer* component = Allocate<MeshRenderer>(1);
		component->Attach(gameobject_
			, R_MeshRenderer.size()
			, modelname_
			, diffuse_
			, "animation.glsl"
			, true
		);
		R_MeshRenderer.push_back(component);
	}

	//object deletion
	//internal use
	template<typename T>
	void RemoveComponent_InternalUse(GameObject*& gameobject_, std::vector<T*>& resources_)
	{
		size_t id_ = gameobject_->GetComponentID<T>();
		auto b = resources_.begin() + id_;
		auto e = resources_.end();

		DeleteS(*b);
		gameobject_->RemoveComponent<T>();
		resources_.erase(b);

		b = resources_.begin() + id_;
		while (b != e)
		{
			GameObject* tmp = (*b)->owner;
			tmp->AddComponent<T>(tmp->GetComponentID<T>() - 1);
			++b;
		}
	}
	//internal use
	void RemoveAllComponents	(GameObject*& gameobject_, PickingCircle* picker_);

	static std::vector<GameObject* > getallllll;
	void RemoveGameObject_Internal(GameObject*& gameobject_, PickingCircle* picker_)
	{
		//remove all components first
		RemoveAllComponents(gameobject_, picker_);
	}

	void RemoveAllComponents(GameObject*& gameobject_, PickingCircle* picker_)
	{
		if (gameobject_->GetCamera())		RemoveComponent_InternalUse<Camera>(gameobject_, R_Camera);
		if (gameobject_->GetMeshRenderer()) RemoveComponent_InternalUse<MeshRenderer>(gameobject_, R_MeshRenderer);

		{
			if (gameobject_->HasChild())
			{
				for (auto& elem : gameobject_->GetAllChildren())
				{
					RemoveGameObject_Internal(elem, picker_);
				}
			}

			//gameobject_->Detach();
			getallllll.push_back(gameobject_);
			//auto findObj = std::find(RM::R_GameObject.begin(), RM::R_GameObject.end(), gameobject_);
			//DeleteS(*findObj);
			//RM::R_GameObject.erase(findObj);
		}
		{
			if (picker_)
			{
				auto findObj = std::find(RM::R_PickingCircle.begin(), RM::R_PickingCircle.end(), picker_);
				DeleteS(*findObj);
				RM::R_PickingCircle.erase(findObj);
			}
		}
	}

	void RemoveGameObject(GameObject* gameobject_, PickingCircle* picker_) 
	{
		//remove all components first
		RemoveGameObject_Internal(gameobject_, picker_);
		if (gameobject_->IsChild())
		{
			gameobject_->Detach();
		}
		for (auto& elem : getallllll) 
		{
			auto findObj = std::find(RM::R_GameObject.begin(), RM::R_GameObject.end(), elem);
			if (findObj != RM::R_GameObject.end())
			{
				DeleteS(*findObj);
				RM::R_GameObject.erase(findObj);
			}
		}
		getallllll.clear();
		UpdateGameObjectID();
	}

	//internal use
	template<typename T>
	void RemoveComponent(GameObject* gameobject_)
	{
		if (typeid(T) == typeid(Camera)) 
		{
			if (gameobject_->GetCamera())		
				RemoveComponent<Camera>(gameobject_, R_Camera);
		}
		else if (typeid(T) == typeid(MeshRenderer))
		{
			if (gameobject_->GetMeshRenderer()) 
				RemoveComponent<MeshRenderer>(gameobject_, R_MeshRenderer);
		}
	}

	//void RemoveMeshRenderer(GameObject* gameobject_) {
	//	unsigned id_	= gameobject_->GetComponentID<MeshRenderer>();
	//	auto b			= R_MeshRenderer.begin() + id_;
	//	auto e			= R_MeshRenderer.end();
	//
	//	DeleteS(*b);
	//	gameobject_->RemoveComponent<MeshRenderer>();
	//	R_MeshRenderer.erase(b);
	//
	//	b = R_MeshRenderer.begin() + id_;
	//	while (b != e) 
	//	{
	//		GameObject* tmp = (*b)->owner;
	//		tmp->AddComponent<MeshRenderer>(tmp->GetComponentID<MeshRenderer>() - 1);
	//		++b;
	//	}
	//}

	//etc
	void DrawAllPickingCircle() {
		for (auto& elem : R_PickingCircle) {
			elem->Draw();
		}
	}
	void UpdateGameObjectID() {
		size_t sz = R_GameObject.size();
		for (size_t i = 0; i < sz; ++i) 
		{
			R_GameObject[i]->SetID(i);
		}
		for (size_t i = 0; i < sz; ++i) 
		{
			R_GameObject[i]->UpdateParentID();
		}
		zg::next_UniqueGameObjectID = R_GameObject.size();
	}

	PickingCircle* Picking(Camera* cam) {
		//MOUSE coordinates, viewport to NDC
		int screenW = ScreenWidth();
		int screenH = ScreenHeight();
		float xn = (2.0f * GetMouseScreenPosX() + 1.0f - screenW) / screenW;
		float yn = (-(2.0f * GetMouseScreenPosY()) + screenH - 1.0f) / screenH;

		//NDC to viewframe
		//get height and width of view finder
		//convert to radian
		//compute view frame width and height
		float viewFrameH = cam->GetNearPlane() * tanf(cam->GetFOVrad() / 2.0f); //cam->GetViewportWidth();
		float viewFrameW = viewFrameH * cam->GetAspectRatio();					//cam->GetViewportHeight();

																				//set up mouse coord in Viewframe
																				//dir of the ray
		zg::Vector3 viewMouse{ viewFrameW * xn,
			viewFrameH * yn,
			-cam->GetNearPlane() };

		//picking
		float smallestT = 0x4F000000;			//set largest float value
		size_t sz = R_PickingCircle.size();		//number of obejct to check
		size_t hitPick = COMPO_NOT_EXIST;		//starting picked number

		for (size_t i = 0; i < sz; ++i) {
			//get SPHERE in view coordinates
			//gfxSphere bs{ scene[i].GetModelBVSphere().Transform(cam->GetLookAtMatrix() * scene[i].GetWorldMtx()) };
			//float rSq = bs.radius * bs.radius;			//viewFrame RADIUS Sq

			zg::Vector3 viewcenter = R_PickingCircle[i]->GetViewCoord(MainCam->ViewMatrix());
			float rSq = R_PickingCircle[i]->radius * R_PickingCircle[i]->radius;

			zg::Vector3 CP{ viewcenter - viewMouse };	//MOUSE to SPHERE.center vector
			float CPdistSq = LengthSq(CP);			//dist sq

													//trival acceptance
			if (CPdistSq <= rSq)
				return R_PickingCircle[i];				//return picked number

			float m = 0.0f;				//dir/length of ray from Camera center to SPHERE.center
			float n = 0.0f;				//shorest distance from sphere.center to ray
										//trival rejection
			if (CPdistSq > rSq) {
				m = CP.Dot(viewMouse);			//dot product viewMouse To SPHERE center with viewMouse

												//1st, trival rejection, ray moving away from sphere
				if (m < 0.0f) continue;			//if sphere is behind of viewMouse, p < 0
												//2nd, trival rejection, ray move towards sphere
				m /= viewMouse.Length();	//normalize Length
				n = CPdistSq - m * m;		//by pythagoras thm, shorest distance from sphere.center to ray

				if (n > rSq) continue;					//ray does not hit sphere
			}

			//trival rejection failed, ray hit sphere
			//compute intersection
			//Length of intersection, by pytahgoras thm
			float s = sqrtf(rSq - n);

			float t = m - s;					//Length of ray from CAM to sphere intersect
			if (t > 0.0f && smallestT > t) {		//if time of intersection more than 0.0f, get the smallest T
				hitPick = i;					//get picked number
				smallestT = t;					//get the smallest time of intersection
			}
		}
		if (hitPick == COMPO_NOT_EXIST)
			return nullptr;
		return R_PickingCircle[hitPick];						//return the object's id
	}

	bool IsHitMyPicking(PickingCircle* pick, Camera* cam) {
		//MOUSE coordinates, viewport to NDC
		int screenW = ScreenWidth();
		int screenH = ScreenHeight();
		float xn = (2.0f * GetMouseScreenPosX() + 1.0f - screenW) / screenW;
		float yn = (-(2.0f * GetMouseScreenPosY()) + screenH - 1.0f) / screenH;

		//NDC to viewframe
		//get height and width of view finder
		//convert to radian
		//compute view frame width and height
		float viewFrameH = cam->GetNearPlane() * tanf(cam->GetFOVrad() / 2.0f); //cam->GetViewportWidth();
		float viewFrameW = viewFrameH * cam->GetAspectRatio();					//cam->GetViewportHeight();

																				//set up mouse coord in Viewframe
																				//dir of the ray
		zg::Vector3 viewMouse{ viewFrameW * xn,
			viewFrameH * yn,
			-cam->GetNearPlane() };

		//picking
		//float smallestT = 0x4F000000;		//set largest float value

		zg::Vector3 viewcenter = pick->GetViewCoord(MainCam->ViewMatrix());
		float rSq = pick->radius * pick->radius;

		zg::Vector3 CP{ viewcenter - viewMouse };	//MOUSE to SPHERE.center vector
		float CPdistSq = LengthSq(CP);			//dist sq

												//trival acceptance
		if (CPdistSq <= rSq)
			return true;				//return picked number

		float m = 0.0f;				//dir/length of ray from Camera center to SPHERE.center
		float n = 0.0f;				//shorest distance from sphere.center to ray
									//trival rejection
		if (CPdistSq > rSq) {
			m = CP.Dot(viewMouse);			//dot product viewMouse To SPHERE center with viewMouse

											//1st, trival rejection, ray moving away from sphere
			if (m < 0.0f) return false;			//if sphere is behind of viewMouse, p < 0
												//2nd, trival rejection, ray move towards sphere
			m /= viewMouse.Length();	//normalize Length
			n = CPdistSq - m * m;		//by pythagoras thm, shorest distance from sphere.center to ray

			if (n > rSq) return false;					//ray does not hit sphere
		}

		//trival rejection failed, ray hit sphere
		//compute intersection
		//Length of intersection, by pytahgoras thm
		float s = sqrtf(rSq - n);

		float t = m - s;					//Length of ray from CAM to sphere intersect
		if (t > 0.0f) {		//if time of intersection more than 0.0f, get the smallest T
			return true;
		}

		return false;
	}

#endif

#if !defined(_EDITOR_ON)
	/*
	void LoadObjectPoolObject(const std::string& archName_, int numberOfObject
		, GameObject*		& B_GameObject
		, Camera*			& B_CAMERA
		, MeshRenderer*		& B_MESHRENDERER
		//, Sprite2D*			& B_SPRITE2D
		//, ParticleEmitter*	& B_PE
		//, AABB*				& B_AABB
		//, BC*				& B_BC
		//, CapsuleCollider*	& B_CapsuleCollider
		//, OBB*				& B_OBB
		//, RigidBody*		& B_RIGIDBODY
		//, Sound*			& B_SOUND
		//, Script*			& B_Script
		//, TextRenderer*		& B_TEXTRENDERER
		//, Light*			& B_LIGHT
		//, UISprite*			& B_UISprite
		//, VideoCompo*		& B_VideoCompo
	);
	*/
	enum COMPONEMENT_TYPEEE {
		CO_GameObject,
		CO_Camera,
		CO_MeshRenderer,
		CO_Sprite2D,
		CO_ParticleEmitter,
		CO_AABB,
		CO_BC,
		CO_CapsuleCollider,
		CO_OBB,
		CO_RigidBody,
		CO_Script,
		CO_Sound,
		CO_TextRenderer,
		CO_Light,
		CO_UISprite,
		CO_VideoCompo,
		CO_COUNT // Leave as last
	};
	/*
	void LoadScene(const std::string& pathname_filename_exten_, bool reload)
	{
	  Engine::sys_OpenGLGraphic.orderedUISprites.clear();
		GameObject::nextObjID = 0;

		FXMLDocument doc;
		std::map<std::string, int> getObjectPoolCount{};

	  for (auto& source : lightsource)
	  {
		source.ResetLightSource();
	  }

		if (XMLLoadPath(doc, pathname_filename_exten_))
		{
			FXMLNode* pRoot = doc.FirstChild();
			XMLErrorNullPtr(pRoot);
			FXMLElement* pRootElement = pRoot->ToElement();
			FXMLElement* pElem = pRoot->FirstChildElement();

			if (!reload) {
				int getInt[CO_COUNT]{};
				for (int i = 0; i < CO_COUNT; ++i) {
					getInt[i] = 0;
				}

				getInt[CO_GameObject]			= pRootElement->IntAttribute(NUM_GAMEOBJECT);
				getInt[CO_Camera]				= pRootElement->IntAttribute(NUM_CAMERA);
				getInt[CO_MeshRenderer]			= pRootElement->IntAttribute(NUM_MESHRENDERER);
				getInt[CO_Sprite2D]				= pRootElement->IntAttribute(NUM_SPRITE2D);
				getInt[CO_ParticleEmitter]		= pRootElement->IntAttribute(NUM_PARTICLEEMITTER);
				getInt[CO_AABB]					= pRootElement->IntAttribute(NUM_AABB);
				getInt[CO_BC]					= pRootElement->IntAttribute(NUM_BC);
				getInt[CO_CapsuleCollider]		= pRootElement->IntAttribute(NUM_CAPSULECOLLIDER);
				getInt[CO_OBB]					= pRootElement->IntAttribute(NUM_OBB);
				getInt[CO_RigidBody]			= pRootElement->IntAttribute(NUM_RIGIDBODY);
				getInt[CO_Script]				= pRootElement->IntAttribute(NUM_SCRIPT);
				getInt[CO_Sound]				= pRootElement->IntAttribute(NUM_SOUND);
				getInt[CO_TextRenderer]			= pRootElement->IntAttribute(NUM_TEXTRENDERER);
				getInt[CO_Light]				= pRootElement->IntAttribute(NUM_LIGHT);
				getInt[CO_UISprite]				= pRootElement->IntAttribute(NUM_UISPRITE);
				getInt[CO_VideoCompo]			= pRootElement->IntAttribute(NUM_VIDEOCOMPO);

				Engine::sys_OpenGLGraphic.orderedUISprites.reserve(CO_UISprite);

				if (strcmp(pElem->Name(), IS_OBJECTPOOL) == 0)
				{
					ListOfObjectPool.clear();
					FXMLElement* pSubElem = pElem->FirstChildElement();
					while (pSubElem)
					{
						const char* getName = pSubElem->Name();
						unsigned count = std::stoul(pSubElem->Attribute("num"));

						getObjectPoolCount[getName] = count;

						if (count > 0) {
							size_t hashnum = GetHashFromString(getName);

							getInt[CO_GameObject]		+= ResourceArchtype::IsArchExist<GameObject>		(hashnum) * count;
							getInt[CO_Camera]			+= ResourceArchtype::IsArchExist<Camera>			(hashnum) * count;
							getInt[CO_MeshRenderer]		+= ResourceArchtype::IsArchExist<MeshRenderer>		(hashnum) * count;
							getInt[CO_Sprite2D]			+= ResourceArchtype::IsArchExist<Sprite2D>			(hashnum) * count;
							getInt[CO_ParticleEmitter]	+= ResourceArchtype::IsArchExist<ParticleEmitter>	(hashnum) * count;
							getInt[CO_AABB]				+= ResourceArchtype::IsArchExist<AABB>				(hashnum) * count;
							getInt[CO_BC]				+= ResourceArchtype::IsArchExist<BC>				(hashnum) * count;
							getInt[CO_CapsuleCollider]	+= ResourceArchtype::IsArchExist<CapsuleCollider>	(hashnum) * count;
							getInt[CO_OBB]				+= ResourceArchtype::IsArchExist<OBB>				(hashnum) * count;
							getInt[CO_RigidBody]		+= ResourceArchtype::IsArchExist<RigidBody>			(hashnum) * count;
							getInt[CO_Script]			+= ResourceArchtype::IsArchExist<Script>			(hashnum) * count;
							getInt[CO_Sound]			+= ResourceArchtype::IsArchExist<Sound>				(hashnum) * count;
							getInt[CO_TextRenderer]		+= ResourceArchtype::IsArchExist<TextRenderer>		(hashnum) * count;
							getInt[CO_Light]			+= ResourceArchtype::IsArchExist<Light>				(hashnum) * count;
							getInt[CO_UISprite]			+= ResourceArchtype::IsArchExist<UISprite>			(hashnum) * count;
							getInt[CO_VideoCompo]		+= ResourceArchtype::IsArchExist<VideoCompo>		(hashnum) * count;
						}


						pSubElem = pSubElem->NextSiblingElement();
					}
				}

				AllocateResource<GameObject>		(getInt[CO_GameObject]		);
				AllocateResource<Camera>			(getInt[CO_Camera]			);
				AllocateResource<MeshRenderer>		(getInt[CO_MeshRenderer]	);
				AllocateResource<Sprite2D>			(getInt[CO_Sprite2D]		);
				AllocateResource<ParticleEmitter>	(getInt[CO_ParticleEmitter]	);
				AllocateResource<AABB>				(getInt[CO_AABB]			);
				AllocateResource<BC>				(getInt[CO_BC]				);
				AllocateResource<CapsuleCollider>	(getInt[CO_CapsuleCollider]	);
				AllocateResource<OBB>				(getInt[CO_OBB]				);
				AllocateResource<RigidBody>			(getInt[CO_RigidBody]		);
				AllocateResource<Script>			(getInt[CO_Script]			);
				AllocateResource<Sound>				(getInt[CO_Sound]			);
				AllocateResource<TextRenderer>		(getInt[CO_TextRenderer]	);
				AllocateResource<Light>				(getInt[CO_Light]			);
				AllocateResource<UISprite>			(getInt[CO_UISprite]		);
				AllocateResource<VideoCompo>		(getInt[CO_VideoCompo]		);
			}
			else {
				Deallocate(R_Script, C_Script);
				C_Script = 0.0f;
				ClearObjPool();

				{
					Sound* b = R_Sound;
					Sound* e = R_Sound + C_Sound;
					while (b != e) {
						b->Stop();
						++b;
					}
				}

				int getInt[CO_COUNT]{};
				for (int i = 0; i < CO_COUNT; ++i) {
					getInt[i] = 0;
				}

				getInt[CO_Script] = pRootElement->IntAttribute(NUM_SCRIPT);

				if (strcmp(pElem->Name(), IS_OBJECTPOOL) == 0)
				{
					ListOfObjectPool.clear();
					FXMLElement* pSubElem = pElem->FirstChildElement();
					while (pSubElem)
					{
						const char* getName = pSubElem->Name();
						unsigned count = std::stoul(pSubElem->Attribute("num"));

						getObjectPoolCount[getName] = count;

						if (count > 0) {
							size_t hashnum = GetHashFromString(getName);
							getInt[CO_Script] += ResourceArchtype::IsArchExist<Script>(hashnum) * count;
						}
						pSubElem = pSubElem->NextSiblingElement();
					}
				}

				AllocateResource<Script>(getInt[CO_Script]);
			}
			GameObject*			lastGameObject		= R_GameObject		;
			Script*				lastScript			= R_Script;

			GameObject*			B_GameObject		= R_GameObject		;
			Camera*				B_CAMERA			= R_Camera			;
			MeshRenderer*		B_MESHRENDERER		= R_MeshRenderer	;
			Sprite2D*			B_SPRITE2D			= R_Sprite2D		;
			ParticleEmitter*	B_PE				= R_ParticleEmitter	;
			AABB*				B_AABB				= R_AABB			;
			BC*					B_BC				= R_BC				;
			CapsuleCollider*	B_CapsuleCollider	= R_CapsuleCollider	;
			OBB*				B_OBB				= R_OBB				;
			RigidBody*			B_RIGIDBODY			= R_RigidBody		;
			Sound*				B_SOUND				= R_Sound			;
			Script*				B_Script			= R_Script			;
			TextRenderer*		B_TEXTRENDERER		= R_TextRenderer	;
			Light*				B_LIGHT				= R_Light			;
			UISprite*			B_UISprite			= R_UISprite		;
			VideoCompo*			B_VideoCompo		= R_VideoCompo;

			pElem = pElem->NextSiblingElement();
			while (pElem)
			{
				if (strcmp(pElem->Name(), IS_GAMEOBJECT) != 0) break;

				B_GameObject->Unserial(pElem);

				FXMLElement* pSubElem = pElem->FirstChildElement();
				while (pSubElem) {
					const char * getName = pSubElem->Name();
					if (strcmp(getName, IS_TRANSFORM) == 0) {
						B_GameObject->transform.Unserial(pSubElem);
					}
					else if (strcmp(getName, IS_CAMERA) == 0) {
						B_CAMERA->Attach(B_GameObject, B_CAMERA - R_Camera);
						B_CAMERA->Unserial(pSubElem);
						++B_CAMERA;
					}
					else if (strcmp(getName, IS_MESHRENDERER) == 0) {
						B_MESHRENDERER->Attach(B_GameObject, B_MESHRENDERER - R_MeshRenderer);
						B_MESHRENDERER->Unserial(pSubElem);
						++B_MESHRENDERER;
					}
					else if (strcmp(getName, IS_SPRITE2D) == 0) {
						B_SPRITE2D->Attach(B_GameObject, B_SPRITE2D - R_Sprite2D);
						B_SPRITE2D->Unserial(pSubElem);
						++B_SPRITE2D;
					}
					else if (strcmp(getName, IS_PARTICLEEMITTER) == 0) {
						B_PE->Attach(B_GameObject, B_PE - R_ParticleEmitter);
						B_PE->Unserial(pSubElem);
						++B_PE;
					}
					else if (strcmp(getName, IS_AABB) == 0) {
						B_AABB->Attach(B_GameObject, B_AABB - R_AABB);
						B_AABB->Unserial(pSubElem);
						++B_AABB;
					}
					else if (strcmp(getName, IS_BC) == 0) {
						B_BC->Attach(B_GameObject, B_BC - R_BC);
						B_BC->Unserial(pSubElem);
						++B_BC;
					}
					else if (strcmp(getName, IS_CAPSULECOLLIDER) == 0) {
						B_CapsuleCollider->Attach(B_GameObject, B_CapsuleCollider - R_CapsuleCollider);
						B_CapsuleCollider->Unserial(pSubElem);
						++B_CapsuleCollider;
					}
					else if (strcmp(getName, IS_OBB) == 0) {
						B_OBB->Attach(B_GameObject, B_OBB - R_OBB);
						B_OBB->Unserial(pSubElem);
						++B_OBB;
					}
					else if (strcmp(getName, IS_RIGIDBODY) == 0) {
						B_RIGIDBODY->Attach(B_GameObject, B_RIGIDBODY - R_RigidBody);
						B_RIGIDBODY->Unserial(pSubElem);
						++B_RIGIDBODY;
					}
					else if (strcmp(getName, IS_SCRIPT) == 0) {
						FXMLElement* pSubSubElem = pSubElem->FirstChildElement("attribute");
						while (pSubSubElem) {
							B_Script->Unserial(pSubSubElem, B_GameObject, B_Script - R_Script);
							B_Script->Attach(B_GameObject, B_Script - R_Script, B_Script->filename, pathname_filename_exten_);
							++B_Script;
							pSubSubElem = pSubSubElem->NextSiblingElement();
						}
					}
					else if (strcmp(getName, IS_SOUND) == 0) {
						B_SOUND->Attach(B_GameObject, B_SOUND - R_Sound);
						B_SOUND->Unserial(pSubElem);
						++B_SOUND;
					}
					else if (strcmp(getName, IS_TEXTRENDERER) == 0) {
						B_TEXTRENDERER->Attach(B_GameObject, B_TEXTRENDERER - R_TextRenderer);
						B_TEXTRENDERER->Unserial(pSubElem);
						++B_TEXTRENDERER;
					}
					else if (strcmp(getName, IS_LIGHT) == 0) {
						B_LIGHT->Attach(B_GameObject, B_LIGHT - R_Light);
						B_LIGHT->Unserial(pSubElem);
			  B_LIGHT->AttachSource();
						++B_LIGHT;
					}
					else if (strcmp(getName, IS_UISPRITE) == 0) {
						B_UISprite ->Attach(B_GameObject, B_UISprite - R_UISprite);
						B_UISprite ->Unserial(pSubElem);
			  Engine::sys_OpenGLGraphic.orderedUISprites.push_back(B_UISprite);
						++B_UISprite;
					}
					else if (strcmp(getName, IS_VIDEOCOMPO) == 0) {
						B_VideoCompo->Attach(B_GameObject, B_VideoCompo - R_VideoCompo);
						B_VideoCompo->Unserial(pSubElem);
						++B_VideoCompo;
					}
					else {
	#if	defined(_DEBUG)
						logLine("not a Component, \">>END\" NOT FOUND");
	#endif
					}
					pSubElem = pSubElem->NextSiblingElement();
				}//end component loop

				++B_GameObject;
				pElem = pElem->NextSiblingElement();
			}//end loop

			lastGameObject = B_GameObject;
			lastScript	   = B_Script;

			//load camera to attach / use audiolistener
			while (pElem) {
				if (strcmp(pElem->Name(), IS_GAMESETUP) == 0)
				{
					FXMLElement* pSubElem = pElem->FirstChildElement();
					int num1 = pSubElem->IntAttribute("num");
					SetCAMERA(R_Camera + num1);

					pSubElem = pSubElem->NextSiblingElement();
					int num2 = pSubElem->IntAttribute("num");
					audioListener->Attach(R_Camera[num2].owner);
				}
				pElem = pElem->NextSiblingElement();
			}

			B_GameObject = R_GameObject;
			GameObject*	E_GameObject = lastGameObject;

			int highestID = 0;

			while (B_GameObject != E_GameObject) {
				if (B_GameObject->GetID() > highestID)
					highestID = B_GameObject->GetID();

				// Attach to parents after all loaded
				if (B_GameObject->GetParentID() != -1) {
					GameObject* B1 = R_GameObject;
					while (B1 != E_GameObject)
					{
						if (B_GameObject->GetParentID() == B1->GetID()) {
							B_GameObject->Attach(B1);
							break;
						}
						++B1;
					}
				}
				++B_GameObject;
			}

			GameObject::nextObjID = highestID + 1;

			for (auto& elem : getObjectPoolCount) {
				if (elem.second == 0)  continue;

				LoadObjectPoolObject(elem.first, elem.second
					, B_GameObject
					, B_CAMERA
					, B_MESHRENDERER
					, B_SPRITE2D
					, B_PE
					, B_AABB
					, B_BC
					, B_CapsuleCollider
					, B_OBB
					, B_RIGIDBODY
					, B_SOUND
					, B_Script
					, B_TEXTRENDERER
					, B_LIGHT
					, B_UISprite
					, B_VideoCompo
				);
			}

			InitToObjectPool();

			BindColliderAndRigidBody();
			BindDrawComponent();

			MeshRenderer* B_MeshRenderer = R_MeshRenderer;
			MeshRenderer* E_MeshRenderer = R_MeshRenderer + C_MeshRenderer;
			while (B_MeshRenderer != E_MeshRenderer)
			{
			  auto children = B_MeshRenderer->owner->GetAllChildren();
			  for (auto& child : children)
			  {
				B_MeshRenderer->AddBoneObjects(child);
			  }

			  B_MeshRenderer->InitCallbacks();
			  ++B_MeshRenderer;
			}
			//static int couu = 0;

			B_UISprite = R_UISprite;
			UISprite* E_UISprite = R_UISprite + C_UISprite;
			while (B_UISprite != E_UISprite)
			{
				B_UISprite->cameratorender = &R_Camera[B_UISprite->cameraArrayIndex];
				++B_UISprite;
			}
			//B_VideoCompo = R_VideoCompo;
			//VideoCompo* E_UISprite = R_VideoCompo + C_VideoCompo;
			//while (B_VideoCompo != R_VideoCompo)
			//{
			//	B_VideoCompo->cameratorender = &R_Camera[B_VideoCompo->cameraArrayIndex];
			//	++B_VideoCompo;
			//}

			B_TEXTRENDERER = R_TextRenderer;
			TextRenderer* E_TextRenderer = R_TextRenderer + C_TextRenderer;
			while (B_TEXTRENDERER != E_TextRenderer)
			{
				if (B_TEXTRENDERER->cameraArrayIndex > -1) {
					B_TEXTRENDERER->cameratorender = &R_Camera[B_TEXTRENDERER->cameraArrayIndex];
				}
				++B_TEXTRENDERER;
			}

			B_Script = R_Script;
			Script*	E_Script = lastScript;
			while (B_Script != E_Script) {

				B_Script->Init(true);
				++B_Script;
				//++couu;
				//std::cout << couu << std::endl;
			}
			E_Script = R_Script + C_Script;

			while (B_Script != E_Script) {
				B_Script->Init(false);
				++B_Script;
				//++couu;
				//std::cout << couu << std::endl;
			}

			B_Script = R_Script;
			while (B_Script != E_Script) {
				B_Script->Start();
				++B_Script;
			}
		}
		else
			ErrorMsg(std::string{ pathname_filename_exten_ + "File Not File, wrong name " }.c_str());

	  std::string filename;
	  GetFileNameNoExtension(pathname_filename_exten_, filename);

	  Engine::sys_CutsceneManager.Clear();

	  if (XMLLoadPath(doc, PATH_CUTSCENE + filename + ".cutscenes"))
	  {
		FXMLNode* pRoot = doc.FirstChild();
		XMLErrorNullPtr(pRoot);
		FXMLElement* pRootElement = pRoot->ToElement();

		Engine::sys_CutsceneManager.Unserial(pRootElement);
	  }

	  if (XMLLoadPath(doc, PATH_GLOBALS + filename + ".globals"))
	  {
		FXMLNode* pRoot = doc.FirstChild();
		XMLErrorNullPtr(pRoot);
		FXMLElement* pRootElement = pRoot->ToElement();

		Engine::sys_OpenGLGraphic.Unserial(pRootElement);
	  }

	  std::sort(Engine::sys_OpenGLGraphic.orderedUISprites.begin(), Engine::sys_OpenGLGraphic.orderedUISprites.end(),
		[](UISprite* lhs, UISprite* rhs) { return lhs->owner->transform.translation.z < rhs->owner->transform.translation.z; });

	  Engine::sys_OpenGLGraphic.ReSizeGLScene(ScreenWidth(), ScreenHeight());
	}


	void LoadObjectPoolObject(const std::string& archName_, int numberOfObject
		, GameObject*		& B_GameObject
		, Camera*			& B_CAMERA
		, MeshRenderer*		& B_MESHRENDERER
		, Sprite2D*			& B_SPRITE2D
		, ParticleEmitter*	& B_PE
		, AABB*				& B_AABB
		, BC*				& B_BC
		, CapsuleCollider*	& B_CapsuleCollider
		, OBB*				& B_OBB
		, RigidBody*		& B_RIGIDBODY
		, Sound*			& B_SOUND
		, Script*			& B_Script
		, TextRenderer*		& B_TEXTRENDERER
		, Light*			& B_LIGHT
		, UISprite *		& B_UISprite
		, VideoCompo *		& B_VideoCompo
	) {

		std::string pathname_filename_exten_{ PATH_ARCH + archName_ + ".xml" };
		//size_t highGameobjectNum = B_GameObject - R_GameObject;

		int& highGameobjectNum = GameObject::nextObjID;

		std::vector<GameObject*> storeObjectpool;
		storeObjectpool.resize(numberOfObject);
		auto b = storeObjectpool.begin();

		FXMLDocument doc;
		if (XMLLoadPath(doc, pathname_filename_exten_))
		{
			for (int i = 0; i < numberOfObject; ++i) {
				FXMLNode* pRoot = doc.FirstChild();
				XMLErrorNullPtr(pRoot);

				FXMLElement* pElem = pRoot->FirstChildElement();
				std::vector<GameObject*> tmpArchObjs;

				while (pElem)
				{
					if (strcmp(pElem->Name(), IS_GAMEOBJECT) != 0) break;

					B_GameObject->Unserial(pElem);
			B_GameObject->SetActive(false);

					FXMLElement* pSubElem = pElem->FirstChildElement();
					while (pSubElem)
					{
						const char * getName = pSubElem->Name();
						if (strcmp(getName, IS_TRANSFORM) == 0) {
							B_GameObject->transform.Unserial(pSubElem);
						}
						else if (strcmp(getName, IS_CAMERA) == 0) {
							B_CAMERA->Attach(B_GameObject, B_CAMERA - R_Camera);
							B_CAMERA->Unserial(pSubElem);
							++B_CAMERA;
						}
						else if (strcmp(getName, IS_MESHRENDERER) == 0) {

							B_MESHRENDERER->Attach(B_GameObject, B_MESHRENDERER - R_MeshRenderer);
							B_MESHRENDERER->Unserial(pSubElem);
							++B_MESHRENDERER;
						}
						else if (strcmp(getName, IS_SPRITE2D) == 0) {
							B_SPRITE2D->Attach(B_GameObject, B_SPRITE2D - R_Sprite2D);
							B_SPRITE2D->Unserial(pSubElem);
							++B_SPRITE2D;
						}
						else if (strcmp(getName, IS_PARTICLEEMITTER) == 0) {
							B_PE->Attach(B_GameObject, B_PE - R_ParticleEmitter);
							B_PE->Unserial(pSubElem);
							++B_PE;
						}
						else if (strcmp(getName, IS_AABB) == 0) {
							B_AABB->Attach(B_GameObject, B_AABB - R_AABB);
							B_AABB->Unserial(pSubElem);
							++B_AABB;
						}
						else if (strcmp(getName, IS_BC) == 0) {
							B_BC->Attach(B_GameObject, B_BC - R_BC);
							B_BC->Unserial(pSubElem);
							++B_BC;
						}
						else if (strcmp(getName, IS_CAPSULECOLLIDER) == 0) {
							B_CapsuleCollider->Attach(B_GameObject, B_CapsuleCollider - R_CapsuleCollider);
							B_CapsuleCollider->Unserial(pSubElem);
							++B_CapsuleCollider;
						}
						else if (strcmp(getName, IS_OBB) == 0) {
							B_OBB->Attach(B_GameObject, B_OBB - R_OBB);
							B_OBB->Unserial(pSubElem);
							++B_OBB;
						}
						else if (strcmp(getName, IS_RIGIDBODY) == 0) {
							B_RIGIDBODY->Attach(B_GameObject, B_RIGIDBODY - R_RigidBody);
							B_RIGIDBODY->Unserial(pSubElem);
							++B_RIGIDBODY;
						}
						else if (strcmp(getName, IS_SCRIPT) == 0) {
							FXMLElement* pSubSubElem = pSubElem->FirstChildElement("attribute");
							while (pSubSubElem) {
								B_Script->Unserial(pSubSubElem, B_GameObject, B_Script - R_Script);
								B_Script->Attach(B_GameObject, B_Script - R_Script, B_Script->filename, pathname_filename_exten_);
								++B_Script;
								pSubSubElem = pSubSubElem->NextSiblingElement();
							}
						}
						else if (strcmp(getName, IS_SOUND) == 0) {
							B_SOUND->Attach(B_GameObject, B_SOUND - R_Sound);
							B_SOUND->Unserial(pSubElem);
							++B_SOUND;
						}
						else if (strcmp(getName, IS_TEXTRENDERER) == 0) {
							B_TEXTRENDERER->Attach(B_GameObject, B_TEXTRENDERER - R_TextRenderer);
							B_TEXTRENDERER->Unserial(pSubElem);
							++B_TEXTRENDERER;
						}
						else if (strcmp(getName, IS_LIGHT) == 0) {
							B_LIGHT->Attach(B_GameObject, B_LIGHT - R_Light);
							B_LIGHT->Unserial(pSubElem);
							++B_LIGHT;
						}
						else if (strcmp(getName, IS_UISPRITE) == 0) {
							B_UISprite->Attach(B_GameObject, B_UISprite - R_UISprite);
							B_UISprite->Unserial(pSubElem);
				Engine::sys_OpenGLGraphic.orderedUISprites.push_back(B_UISprite);
							++B_UISprite;
						}
						else if (strcmp(getName, IS_VIDEOCOMPO) == 0) {
							B_VideoCompo->Attach(B_GameObject, B_VideoCompo - R_VideoCompo);
							B_VideoCompo->Unserial(pSubElem);
							++B_VideoCompo;
						}
						else {
	#if	defined(_DEBUG)
							logLine("not a Component, \">>END\" NOT FOUND");
	#endif
						}
						pSubElem = pSubElem->NextSiblingElement();
					}//end component loop

					tmpArchObjs.push_back(B_GameObject);
					++B_GameObject;
					pElem = pElem->NextSiblingElement();
				}//end loop

				*b = tmpArchObjs[0];
				++b;

				GameObject* rootObj = *tmpArchObjs.begin();

				// Attach to parents after all loaded
				for (auto& elem : tmpArchObjs)
				{
					if (elem->GetParentID() != -1)
					{
						for (auto& parent : tmpArchObjs)
						{
							if (elem->GetParentID() == parent->GetID())
								elem->Attach(parent);
						}
					}
				}

				// Set up mesh renderer
				InitArchetypeCallbacks(tmpArchObjs);

				// Update objs' ID (The one stored in file is just to keep track of parents)
				for (auto& elem : tmpArchObjs)
					elem->SetID(++highGameobjectNum);
				for (auto& elem : tmpArchObjs) {
					elem->UpdateParentID();
				}

				// Update anim timeline's REAL ID (The one stored in file is just to keep track of parents)
				UpdateArchetypeCallbacksID(tmpArchObjs);
				InitArchetypeBones(rootObj);

			}//end for numberOfObjects

		}//end if
		ListOfObjectPool[GetHashFromString(archName_)] = storeObjectpool;
	}
	*/
#endif

}// namespace RM
