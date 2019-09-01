/*****************************************************************************/
/*!
\file			GameObject.h
\brief			Game Object
*/
/*****************************************************************************/
#ifndef _GAMEOBJECT_H
#define _GAMEOBJECT_H

#include "XMLWrap.h"
#include <vector>
#include <map>
#include <typeindex>
#include <typeinfo>

#pragma warning(disable : 4616 4251) //dllexport

#ifdef _64BITMACHINE
	#define COMPO_NOT_EXIST 0xFFFFFFFFFFFFFFFF
#else
	#define COMPO_NOT_EXIST 0xFFFFFFFF
#endif

#define	NUM_OF_COMPONENT_SIZE 10
#define	GRID_NOT_EXIST -842150451

class Camera;
class MeshRenderer;
//class Sprite2D;
//class AABB;
//class BC;
//class CapsuleCollider;
//class OBB;
//class RigidBody;
//class Sound;
//class Script;
//class ParticleEmitter;
//class Light;
//class TextRenderer;


class ZG_API GameObject
{
public:

	GameObject(const char* name = "GameObject") noexcept;
	GameObject(const GameObject& rhs);
	GameObject& operator=(const GameObject& rhs);
	~GameObject();

	GameObject* Clone();

	// ____________________________________________________________ getters
	const std::vector<GameObject*>& GetAllChildren				() const;						//get all children of this gameobject
	std::vector<GameObject*>&		GetAllChildren				();								//get all children of this gameobject
	std::vector<GameObject*>		GetAllChildrenByName		(const char* name_);			//get all children of this gameobject by name
	std::vector<GameObject*>		GetAllChildrenByName		(const std::string& name_);		//get all children of this gameobject by name
	std::vector<GameObject*>		GetAllChildrenByArchName	(const char* archname_);		//get all children of this gameobject by archname
	std::vector<GameObject*>		GetAllChildrenByArchName	(const std::string& archname_);	//get all children of this gameobject by archname
	std::vector<GameObject*>		GetAllChildrenByTag			(const char* tag_name_);		//get all children of this gameobject by tag name
	std::vector<GameObject*>		GetAllChildrenByTag			(const std::string& tag_name_);	//get all children of this gameobject by tag name
	std::vector<GameObject*>		GetAllChildrenByTag			(size_t tag_);					//get all children of this gameobject by tag

	GameObject*						GetChildByName				(const char* name_);
	GameObject*						GetChildByName				(const std::string& name_);
	GameObject*						GetChildByTag				(const char* tag_name_);
	GameObject*						GetChildByTag				(const std::string& tag_name_);
	GameObject*						GetChildByTag				(size_t tag_name_);

	// ______________________________ const
	const GameObject*				GetChildByName				(const char* name_) const;
	const GameObject*				GetChildByName				(const std::string& name_) const;
	const GameObject*				GetChildByTag				(const char* tag_name_) const;
	const GameObject*				GetChildByTag				(const std::string& tag_name_) const;
	const GameObject*				GetChildByTag				(size_t tag_name_) const;

	size_t							GetID						() const;
	int								GetLayerNumber				() const;
	size_t							GetParentID					() const;
	GameObject*						GetParent					();
	const GameObject*				GetParent					() const;
	GameObject*						GetRootParent				();
	size_t							GetTag						() const;
	size_t*							GetTagPtr					();
	std::string						GetTagString				() const;

	bool HasChild					() const;
	bool IsChild					() const;
	bool IsChildOf					(const char* name_) const;
	bool IsChildOf					(const std::string& name_) const;
	bool IsChildrenOf				(const GameObject* root_parent_) const;	//check if is recursivly under the root parent
	bool IsChildOf					(const GameObject* parent_) const;		//check if is under the single parent
	
	bool IsArchetype				() const;
	bool IsActive					() const;
	bool IsStatic					() const;

	// ____________________________________________________________ setters
	void SetActive		(bool toggleActive_);
	void SetStatic		(bool toggleStatic_);
	void SetID			(size_t id_);
	void SetLayerNum	(int num_);
	void SetTag			(const char* tag_);
	void SetTag			(const std::string& tag_);
	
	// ____________________________________________________________ etc function
	void Attach			(GameObject*& parent);
	void Detach			();
	void RemoveChild	(GameObject* child_);
	void RearrangeChild	(GameObject* childToMove, GameObject* newChildPos);
	void RenderParentAndChild();		//internal use only
	void UpdateParentID();				//internal use only


	Camera*				GetCamera();
	MeshRenderer*		GetMeshRenderer();
	//Sprite2D*			GetSprite2D();
	//ParticleEmitter*	GetParticleEmitter();
	//AABB*				GetAABB();
	//BC*				GetBC();
	//CapsuleCollider*	GetCapsuleCollider();
	//OBB*				GetOBB();
	//RigidBody*		GetRigidBody();
	//Sound*			GetSound();
	//TextRenderer*		GetTextRenderer();
	//Light*			GetLight();
	//UISprite*			GetUISprite();

	template<typename T>
	void AddComponent(size_t array_index) {
		mComponentId[typeid(T)] = array_index;
	}

	template<typename T>
	size_t GetComponentID() const {
		auto find = mComponentId.find(typeid(T));
		if (find == mComponentId.end())
			return COMPO_NOT_EXIST;
		return find->second;
	}

	template<typename T>
	void RemoveComponent() {
		mComponentId.erase(typeid(T));
	}

#ifdef TINYXML2_EXPORT
	//serial
	void Serial		(FXMLElement* pElem);
	void Unserial	(FXMLElement* pElem);
#endif

#if defined(_USE_LUA_PUSH)
	static void BindLua(sol::state& lua);
#endif

#if defined(USING_SOL2)
	sol::object GetScript(std::string filename, std::string key);
	bool HasScript(const std::string& filename);
	sol::environment&	GetScript(const std::string& filename);
	Script*				GetScriptPtr(const std::string& filename);

	//std::map<size_t, unsigned>& GetListScript()
	//{
	//	return mScript;
	//}
	//void AddScript(const std::string& filename_lua, unsigned array_index){
	//	size_t hashnum   = FindScriptHashFromString(filename_lua);
	//	auto find = mScript.find(hashnum);
	//	if (find == mScript.end())
	//		mScript[hashnum] = array_index;
	//}
	//unsigned GetScriptID(const std::string& filename_lua) const {
	//	size_t hashnum = FindScriptHashFromString(filename_lua);
	//	auto find = mScript.find(hashnum);
	//	if (find == mScript.end())
	//		return COMPO_NOT_EXIST;
	//	return find->second;
	//}
	//void RemoveScript(const std::string& lua_script_name) {
	//	size_t hashnum = FindScriptHashFromString(lua_script_name);
	//	mScript.erase(hashnum);
	//}
#endif


//members variables
	std::string name;		//read-only / internal use only
	std::string archName;	//read-only / internal use only
	Transform transform;	

	bool toggleActive;		//read-only / internal use only
	bool toggleStatic;		//read-only / internal use only

private:
	size_t tag;
	GameObject* parent;

	int layerNumber;
	size_t id;
	size_t parentID;
	
	std::vector<GameObject*> child;
	std::map<std::type_index, size_t> mComponentId; //std::map<type,arrayID>
	//std::map<size_t, unsigned> mScript;				  //std::map<type,arrayID>
};

namespace zg {
	ZG_API extern size_t next_UniqueGameObjectID;

	//gameobject Tags
	ZG_API size_t AddGameObjectTag		(const char* tagname_);			//interal use only
	ZG_API size_t AddGameObjectTag		(const std::string& tagname_);	//interal use only
	ZG_API void RemoveGameObjectTag		(const std::string& tagname_);	//interal use only

	ZG_API size_t GetTagFromString		(const char* tagname_);
	ZG_API size_t GetTagFromString		(const std::string& tagname_);
	ZG_API std::string GetStringFromTag	(size_t hash_);
	ZG_API std::vector<std::string> GetListOfGameTagString();

	//Get First GameObject Found
	ZG_API GameObject* FindGameObjectByID	(int id);
	ZG_API GameObject* FindGameObjectByName	(const char* gameobjectname_);
	ZG_API GameObject* FindGameObjectByName	(const std::string& gameobjectname_);

	//Get List of GameObject Found
	ZG_API std::vector<GameObject*>		FindListOfGameObjectByName		(const char* gameobjectname_);
	ZG_API std::vector<GameObject*>		FindListOfGameObjectByName		(const std::string& gameobjectname_);
	ZG_API std::vector<GameObject*>		FindListOfGameObjectByArchName	(const char* archname_);
	ZG_API std::vector<GameObject*>		FindListOfGameObjectByArchName	(const std::string& archname_);
	ZG_API std::vector<GameObject*>		FindListOfGameObjectByTagName	(const char* tagname_);
	ZG_API std::vector<GameObject*>		FindListOfGameObjectByTagName	(const std::string& tagname_);
	ZG_API std::vector<GameObject*>		FindListOfGameObjectByTag		(size_t hash_);
}


#endif //_GAMEOBJECT_H
