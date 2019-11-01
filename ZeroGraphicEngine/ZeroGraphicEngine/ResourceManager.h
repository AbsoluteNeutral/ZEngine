
#ifndef _RESOURCEMANAGER_H
#define _RESOURCEMANAGER_H

#include <string>
#include <vector>

#include "MemoryManager.h"
#include "HashString.h"

#include "PickingCircle.h"

#include "GameObject.h"
#include "AudioListener.h"
#include "Camera.h"
#include "MeshRenderer.h"
//#include "Sprite2D.h"
//#include "AABB.h"
//#include "BC.h"
//#include "OBB.h"
//#include "CapsuleCollider.h"
//#include "RigidBody.h"
//#include "Sound.h"
//#include "Script.h"
//#include "ParticleEmitter.h"
//#include "TextRenderer.h"
//#include "Light.h"
//#include "UISprite.h"

namespace ResourceArchtype {
	extern ZG_API std::map<size_t, unsigned> ARCH_GameObject;
	extern ZG_API std::map<size_t, unsigned> ARCH_Camera;
	extern ZG_API std::map<size_t, unsigned> ARCH_MeshRenderer;
	extern ZG_API std::map<size_t, unsigned> ARCH_Sprite2D;
	extern ZG_API std::map<size_t, unsigned> ARCH_ParticleEmitter;
	extern ZG_API std::map<size_t, unsigned> ARCH_AABB;
	extern ZG_API std::map<size_t, unsigned> ARCH_BC;
	extern ZG_API std::map<size_t, unsigned> ARCH_CapsuleCollider;
	extern ZG_API std::map<size_t, unsigned> ARCH_OBB;
	extern ZG_API std::map<size_t, unsigned> ARCH_RigidBody;
	extern ZG_API std::map<size_t, unsigned> ARCH_Script;
	extern ZG_API std::map<size_t, unsigned> ARCH_Sound;
	extern ZG_API std::map<size_t, unsigned> ARCH_TextRenderer;
	extern ZG_API std::map<size_t, unsigned> ARCH_Light;
	extern ZG_API std::map<size_t, unsigned> ARCH_UISprite;
	extern ZG_API std::map<size_t, unsigned> ARCH_VideoCompo;

	ZG_API void LoadArch(const std::string& pathname_filename_);
	ZG_API void ClearResourceArchtype();

	template<typename T>
	static size_t IsArchExist(size_t archtype_hash_number) {
		//auto f = ARCH_GameObject.find(archtype_hash_number);
		//if (typeid(T) == typeid(GameObject))
		//	if (f == ARCH_GameObject.end()) {
		//		NonErrorMsg(std::string{ "Arch type object not exist: " + GetStringFromHash(archtype_hash_number) }.c_str());
		//		return false;
		//}
		if (typeid(T) == typeid(GameObject)) {
			return ARCH_GameObject[archtype_hash_number];
		}
		else if (typeid(T) == typeid(Camera)) {
			return ARCH_Camera[archtype_hash_number];
		}
		else if (typeid(T) == typeid(MeshRenderer)) {
			return ARCH_MeshRenderer[archtype_hash_number];
		}
		//else if (typeid(T) == typeid(Sprite2D)) {
		//	return ARCH_Sprite2D[archtype_hash_number];
		//}
		//else if (typeid(T) == typeid(ParticleEmitter)) {
		//	return ARCH_ParticleEmitter[archtype_hash_number];
		//}
		//else if (typeid(T) == typeid(AABB)) {
		//	return ARCH_AABB[archtype_hash_number];
		//}
		//else if (typeid(T) == typeid(BC)) {
		//	return ARCH_BC[archtype_hash_number];
		//}
		//else if (typeid(T) == typeid(CapsuleCollider)) {
		//	return ARCH_CapsuleCollider[archtype_hash_number];
		//}
		//else if (typeid(T) == typeid(OBB)) {
		//	return ARCH_OBB[archtype_hash_number];
		//}
		//else if (typeid(T) == typeid(RigidBody)) {
		//	return ARCH_RigidBody[archtype_hash_number];
		//}
		//else if (typeid(T) == typeid(Sound)) {
		//	return ARCH_Sound[archtype_hash_number];
		//}
		//else if (typeid(T) == typeid(Script)) {
		//	return ARCH_Script[archtype_hash_number];
		//}
		//else if (typeid(T) == typeid(TextRenderer)) {
		//	return ARCH_TextRenderer[archtype_hash_number];
		//}
		//else if (typeid(T) == typeid(Light)) {
		//	return ARCH_Light[archtype_hash_number];
		//}
		//else if (typeid(T) == typeid(UISprite)) {
		//	return ARCH_UISprite[archtype_hash_number];
		//}
		//else if (typeid(T) == typeid(VideoCompo)) {
		//	return ARCH_VideoCompo[archtype_hash_number];
		//}
		return 0;
	}
}

namespace RM {
	// Archetpe init anim timeline
	//ZG_API void InitArchetypeCallbacks(std::vector<GameObject*>& tmpArchObjs);
	//ZG_API void UpdateArchetypeCallbacksID(std::vector<GameObject*>& tmpArchObjs);
	//ZG_API void InitArchetypeBones(GameObject* rootObj);
	//
	ZG_API void ClearResourceVaribles();
	
	extern ZG_API AudioListener* audioListener;
	//extern ZG_API LightSource lightsource[NUM_OF_LIGHTS];

#if defined(_EDITOR_ON) || !defined(_EDITOR_ON)
	extern ZG_API std::vector<PickingCircle*>	R_PickingCircle;

	extern ZG_API std::vector<GameObject*>		R_GameObject;
	extern ZG_API std::vector<Camera*>			R_Camera;
	extern ZG_API std::vector<MeshRenderer*>		R_MeshRenderer;
	//extern ZG_API std::vector<Sprite2D*>			R_Sprite2D;
	//extern ZG_API std::vector<ParticleEmitter*>	R_ParticleEmitter;
	//extern ZG_API std::vector<AABB*>				R_AABB;
	//extern ZG_API std::vector<BC*>				R_BC;
	//extern ZG_API std::vector<OBB*>				R_OBB;
	//extern ZG_API std::vector<CapsuleCollider*>	R_CapsuleCollider;
	//extern ZG_API std::vector<RigidBody*>			R_RigidBody;
	//extern ZG_API std::vector<Script*>			R_Script;
	//extern ZG_API std::vector<Sound*>				R_Sound;
	//extern ZG_API std::vector<TextRenderer*>		R_TextRenderer;
	//extern ZG_API std::vector<Light*>				R_Light;
	//extern ZG_API std::vector<UISprite*>			R_UISprite;
	//extern ZG_API std::vector<VideoCompo*>		R_VideoCompo;

	//Object Creation
	ZG_API PickingCircle*	CreateGameObjectWithPickingCircle(GameObject*& gameObjectGetter_, const char* gameobject_name_ = "GameObject");
	ZG_API GameObject*		CreateGameObject				 (const char* gameobject_name_ = "GameObject");
	ZG_API void				CreateMeshRenderer				 (GameObject* gameobject_, const char* modelname_ = "null.models", const char* diffuse_ = "null.dds");

	//Object Deletion
	ZG_API void RemoveGameObject	(GameObject* gameobject_, PickingCircle* picker_ = nullptr);
	template<typename T>
	ZG_API void RemoveComponent		(GameObject* gameobject_);

	//etc
	ZG_API void				DrawAllPickingCircle();
	ZG_API void				UpdateGameObjectID	();
	ZG_API PickingCircle*	Picking				(Camera* cam);
	ZG_API bool				IsHitMyPicking		(PickingCircle* pick, Camera* cam);
#else
	extern ZG_API GameObject*		R_GameObject;
	extern ZG_API Camera*			R_Camera;
	extern ZG_API MeshRenderer*		R_MeshRenderer;
	extern ZG_API Sprite2D*			R_Sprite2D;
	extern ZG_API ParticleEmitter*	R_ParticleEmitter;
	extern ZG_API AABB*				R_AABB;
	extern ZG_API BC*				R_BC;
	extern ZG_API OBB*				R_OBB;
	extern ZG_API CapsuleCollider*	R_CapsuleCollider;
	extern ZG_API RigidBody*			R_RigidBody;
	extern ZG_API Script*			R_Script;
	extern ZG_API Sound*				R_Sound;
	extern ZG_API TextRenderer*		R_TextRenderer;
	extern ZG_API Light*				R_Light;
	extern ZG_API UISprite*			R_UISprite;
	extern ZG_API VideoCompo*		R_VideoCompo;

	extern ZG_API unsigned			C_GameObject;
	extern ZG_API unsigned			C_Camera;
	extern ZG_API unsigned			C_MeshRenderer;
	extern ZG_API unsigned			C_Sprite2D;
	extern ZG_API unsigned			C_ParticleEmitter;
	extern ZG_API unsigned			C_AABB;
	extern ZG_API unsigned			C_BC;
	extern ZG_API unsigned			C_OBB;
	extern ZG_API unsigned			C_CapsuleCollider;
	extern ZG_API unsigned			C_RigidBody;
	extern ZG_API unsigned			C_Script;
	extern ZG_API unsigned			C_Sound;
	extern ZG_API unsigned			C_TextRenderer;
	extern ZG_API unsigned			C_Light;
	extern ZG_API unsigned			C_UISprite;
	extern ZG_API unsigned			C_VideoCompo;

	template<typename T>
	static void CTOR_EMPTY(T* ptr, size_t num) {
		T* b = ptr;
		T* e = ptr + num;
		while (b != e) {
			new(b++) T{};
		}
	}

	template<typename T>
	static T* CreateArray(unsigned size_) {
		return Allocate<T>(size_);
	}

#ifdef USE_PLACEMENTNEW
	template<typename T>
	static void AllocateResource(int num_, bool construct_empty_) {
		if (num_ <= 0) return;
		if (typeid(T) == typeid(GameObject)) {
			R_GameObject = CreateArray<GameObject>(num_);
			C_GameObject = num_;
			if (construct_empty_) CTOR_EMPTY(R_GameObject, num_);
		}
		else if (typeid(T) == typeid(Camera)) {
			R_Camera = CreateArray<Camera>(num_);
			C_Camera = num_;
			if (construct_empty_) CTOR_EMPTY(R_Camera, num_);
		}
		else if (typeid(T) == typeid(MeshRenderer)) {
			R_MeshRenderer = CreateArray<MeshRenderer>(num_);
			C_MeshRenderer = num_;
			if (construct_empty_) CTOR_EMPTY(R_MeshRenderer, num_);
		}
		else if (typeid(T) == typeid(Sprite2D)) {
			R_Sprite2D = CreateArray<Sprite2D>(num_);
			C_Sprite2D = num_;
			if (construct_empty_) CTOR_EMPTY(R_Sprite2D, num_);
		}
		else if (typeid(T) == typeid(AABB)) {
			R_AABB = CreateArray<AABB>(num_);
			C_AABB = num_;
			if (construct_empty_) CTOR_EMPTY(R_AABB, num_);
		}
		else if (typeid(T) == typeid(BC)) {
			R_BC = CreateArray<BC>(num_);
			C_BC = num_;
			if (construct_empty_) CTOR_EMPTY(R_BC, num_);
		}
		else if (typeid(T) == typeid(OBB)) {
			R_OBB = CreateArray<OBB>(num_);
			C_OBB = num_;
			if (construct_empty_) CTOR_EMPTY(R_OBB, num_);
		}
		else if (typeid(T) == typeid(CapsuleCollider)) {
			R_CapsuleCollider = CreateArray<OBB>(num_);
			C_CapsuleCollider = num_;
			if (construct_empty_) CTOR_EMPTY(R_CapsuleCollider, num_);
		}
		else if (typeid(T) == typeid(RigidBody)) {
			R_RigidBody = CreateArray<RigidBody>(num_);
			C_RigidBody = num_;
			if (construct_empty_) CTOR_EMPTY(R_RigidBody, num_);
		}
		else if (typeid(T) == typeid(Sound)) {
			R_Sound = CreateArray<Sound>(num_);
			C_Sound = num_;
			if (construct_empty_) CTOR_EMPTY(R_Sound, num_);
		}
		else if (typeid(T) == typeid(Script)) {
			R_Script = CreateArray<Script>(num_);
			C_Script = num_;
			if (construct_empty_) CTOR_EMPTY(R_Script, num_);
		}
		else if (typeid(T) == typeid(ParticleEmitter)) {
			R_ParticleEmitter = CreateArray<ParticleEmitter>(num_);
			C_ParticleEmitter = num_;
			if (construct_empty_) CTOR_EMPTY(R_ParticleEmitter, num_);
		}
		else if (typeid(T) == typeid(TextRenderer)) {
			R_TextRenderer = CreateArray<TextRenderer>(num_);
			C_TextRenderer = num_;
			if (construct_empty_) CTOR_EMPTY(R_TextRenderer, num_);
		}
		else if (typeid(T) == typeid(Light)) {
			R_Light = CreateArray<Light>(num_);
			C_Light = num_;
			if (construct_empty_) CTOR_EMPTY(R_Light, num_);
		}
		else if (typeid(T) == typeid(VideoCompo)) {
			R_VideoCompo = CreateArray<VideoCompo>(num_);
			C_VideoCompo = num_;
			if (construct_empty_) CTOR_EMPTY(R_VideoCompo, num_);
		}
		else ErrorMsg("Type doesn't exist");
	}
#else
	template<typename T>
	static void AllocateResource(int num_) {
		if (num_ <= 0) return;
		if (typeid(T) == typeid(GameObject)) {
			R_GameObject = CreateArray<GameObject>(num_);
			C_GameObject = num_;
		}
		else if (typeid(T) == typeid(Camera)) {
			R_Camera = CreateArray<Camera>(num_);
			C_Camera = num_;
		}
		else if (typeid(T) == typeid(MeshRenderer)) {
			R_MeshRenderer = CreateArray<MeshRenderer>(num_);
			C_MeshRenderer = num_;
		}
		else if (typeid(T) == typeid(Sprite2D)) {
			R_Sprite2D = CreateArray<Sprite2D>(num_);
			C_Sprite2D = num_;
		}
		else if (typeid(T) == typeid(AABB)) {
			R_AABB = CreateArray<AABB>(num_);
			C_AABB = num_;
		}
		else if (typeid(T) == typeid(BC)) {
			R_BC = CreateArray<BC>(num_);
			C_BC = num_;
		}
		else if (typeid(T) == typeid(CapsuleCollider)) {
			R_CapsuleCollider = CreateArray<CapsuleCollider>(num_);
			C_CapsuleCollider = num_;
		}
		else if (typeid(T) == typeid(OBB)) {
			R_OBB = CreateArray<OBB>(num_);
			C_OBB = num_;
		}
		else if (typeid(T) == typeid(RigidBody)) {
			R_RigidBody = CreateArray<RigidBody>(num_);
			C_RigidBody = num_;
		}
		else if (typeid(T) == typeid(Sound)) {
			R_Sound = CreateArray<Sound>(num_);
			C_Sound = num_;
		}
		else if (typeid(T) == typeid(Script)) {
			R_Script = CreateArray<Script>(num_);
			C_Script = num_;
		}
		else if (typeid(T) == typeid(ParticleEmitter)) {
			R_ParticleEmitter = CreateArray<ParticleEmitter>(num_);
			C_ParticleEmitter = num_;
		}
		else if (typeid(T) == typeid(TextRenderer)) {
			R_TextRenderer = CreateArray<TextRenderer>(num_);
			C_TextRenderer = num_;
		}
		else if (typeid(T) == typeid(UISprite)) {
			R_UISprite = CreateArray<UISprite>(num_);
			C_UISprite = num_;
		}
		else if (typeid(T) == typeid(VideoCompo)) {
			R_VideoCompo = CreateArray<VideoCompo>(num_);
			C_VideoCompo = num_;
		}
		else if (typeid(T) == typeid(Light)) {
			R_Light = CreateArray<Light>(num_);
			C_Light = num_;
			Light* b = R_Light;
			Light* e = R_Light + C_Light;
			LightSource* aa = lightsource;
			LightSource* bb = lightsource + NUM_OF_LIGHTS;

			while (b != e) {
				b->source = aa;
				if (aa != bb-1) {
					++aa;
				}
				++b;
			}
		}

		else ErrorMsg("Type doesn't exist");
	}
#endif

	//ZG_API void LoadScene(const std::string& pathname_filename_exten_, bool reload = false);
#endif //editor on?
	
}

#endif
