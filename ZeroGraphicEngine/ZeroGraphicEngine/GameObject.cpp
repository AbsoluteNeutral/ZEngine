/*****************************************************************************/
/*!
\file			GameObject.cpp
\brief			Game Object
*/
/*****************************************************************************/
#include "stdafx.h"
#include "GameObject.h"
#include "Logging.h"
#include "Gizmo.h"
#include "ResourceManager.h"

GameObject::GameObject(const char* obj_name) noexcept
	:name{ obj_name }
	, archName{ "" }
	, transform{}
	, toggleActive(true)
	, toggleStatic(false)
	, tag(0)
	, parent(nullptr)
	, layerNumber(0)
	, id(zg::next_UniqueGameObjectID++)
	, parentID(COMPO_NOT_EXIST)
	, child{}
	, mComponentId{}
{}

GameObject::GameObject(const GameObject& rhs)
	: name{ rhs.name }
	, archName{ rhs.archName }
	, transform{ rhs.transform }
	, toggleActive(rhs.toggleActive)
	, toggleStatic(rhs.toggleStatic)
	, tag(rhs.tag)
	, parent(nullptr)
	, layerNumber(rhs.layerNumber)
	, id(zg::next_UniqueGameObjectID++)
	, parentID(COMPO_NOT_EXIST)
	, child{}
	, mComponentId{}
{}

GameObject& GameObject::operator=(const GameObject& rhs){
	SetActive(rhs.toggleActive);
	SetStatic(rhs.toggleStatic);

	tag = rhs.tag;
	transform = rhs.transform;
	archName = rhs.archName;

	if (parent)
		Detach();

	parent = rhs.parent;

	if (rhs.parent)
	{
		//transform.Attach(&rhs.parent->transform);
		rhs.parent->child.push_back(this);
	}
	return *this;
}

GameObject::~GameObject()
{}

GameObject* GameObject::Clone() {
#ifdef USE_RAWNEW
	return new GameObject{ *this };
#else
	GameObject* tmp = Allocate<GameObject>(1);
	new(tmp) GameObject{ *this };
	return tmp;
#endif
}

// ____________________________________________________________ getters
const std::vector<GameObject*>& GameObject::GetAllChildren() const	{ return child;	}
std::vector<GameObject*>& GameObject::GetAllChildren()				{ return child;	}
std::vector<GameObject*> GameObject::GetAllChildrenByName(const char* gameObject_name_) {
	std::vector<GameObject*> tmp;
	for (const auto& elem : child) {
		if (elem->name == gameObject_name_)
			tmp.push_back(elem);
	}
	return tmp;
}
std::vector<GameObject*> GameObject::GetAllChildrenByName(const std::string& gameObject_name_) {
	return GetAllChildrenByName(gameObject_name_.c_str());
}

std::vector<GameObject*> GameObject::GetAllChildrenByArchName(const char* archname_) {
	std::vector<GameObject*> tmp;
	for (const auto& elem : child) {
		if (elem->archName == archname_)
			tmp.push_back(elem);
	}
	return tmp;
}
std::vector<GameObject*> GameObject::GetAllChildrenByArchName(const std::string& archname_) {
	return GetAllChildrenByArchName(archname_.c_str());
}

std::vector<GameObject*> GameObject::GetAllChildrenByTag(const char* tagname_) {
	size_t getHashedtag = zg::GetTagFromString(tagname_);
	return GetAllChildrenByTag(getHashedtag);
}
std::vector<GameObject*> GameObject::GetAllChildrenByTag(const std::string& tagname_) {
	size_t getHashedtag = zg::GetTagFromString(tagname_);
	return GetAllChildrenByTag(getHashedtag);
}
std::vector<GameObject*> GameObject::GetAllChildrenByTag(size_t tagname_) {
	std::vector<GameObject*> tmp;
	for (const auto& elem : child) {
		if (elem->tag == tagname_)
			tmp.push_back(elem);
	}
	return tmp;
}

GameObject* GameObject::GetChildByName(const char* name_) {
	for (const auto& elem : child) {
		if (elem->name == name_)
			return elem;
	}
#if defined(_DEBUG)
	TestLog(0, "Child not found in %s, Child Name: %s", name.c_str(), name_);
#endif
	return nullptr;
}
GameObject* GameObject::GetChildByName(const std::string& name_) {
	return GetChildByName(name_.c_str());
}

GameObject* GameObject::GetChildByTag(const char* tag_name_) {
	size_t getHashedtag = zg::GetTagFromString(tag_name_);
	return GetChildByTag(getHashedtag);
}
GameObject* GameObject::GetChildByTag(const std::string& tag_name_) {
	size_t getHashedtag = zg::GetTagFromString(tag_name_);
	return GetChildByTag(getHashedtag);
}
GameObject* GameObject::GetChildByTag(size_t tag_name_) {
	for (const auto& elem : child) {
		if (elem->tag == tag_name_)
			return elem;
	}
#if defined(_DEBUG)
	TestLog(0, "Tag not found in %s, Child Name: %s", name.c_str(), zg::GetStringFromTag(tag_name_).c_str());
#endif
	return nullptr;
}

// ______________________________ const
const GameObject* GameObject::GetChildByName(const char* name_) const {
	for (const auto& elem : child) {
		if (elem->name == name_)
			return elem;
	}
#if defined(_DEBUG)
	TestLog(0, "Child not found in %s, Child Name: %s", name.c_str(), name_);
#endif
	return nullptr;
}
const GameObject* GameObject::GetChildByName(const std::string& name_) const {
	return GetChildByName(name_.c_str());
}

const GameObject* GameObject::GetChildByTag(const char* tag_name_) const {
	size_t getHashedtag = zg::GetTagFromString(tag_name_);
	return GetChildByTag(getHashedtag);
}
const GameObject* GameObject::GetChildByTag(const std::string& tag_name_) const {
	size_t getHashedtag = zg::GetTagFromString(tag_name_);
	return GetChildByTag(getHashedtag);
}
const GameObject* GameObject::GetChildByTag(size_t tag_name_) const {
	for (const auto& elem : child) {
		if (elem->tag == tag_name_)
			return elem;
	}
#if defined(_DEBUG)
	TestLog(0, "Tag not found in %s, Child Name: %s", name.c_str(), zg::GetStringFromTag(tag_name_).c_str());
#endif
	return nullptr;
}

size_t				GameObject::GetID()			const	{ return id; }
size_t				GameObject::GetParentID()	const	{ return parentID; }
GameObject*			GameObject::GetParent()				{ return parent; }
const GameObject*	GameObject::GetParent()		const	{ return parent; }
GameObject* GameObject::GetRootParent()
{
	if (!parent)
		return nullptr;

	GameObject* getRoot = parent;
	while (getRoot->parent)
		getRoot = getRoot->parent;
	return getRoot;
}

int		GameObject::GetLayerNumber()	const { return layerNumber; }
size_t	GameObject::GetTag()			const { return tag; }
size_t*	GameObject::GetTagPtr()			{ return &tag; }
std::string GameObject::GetTagString()	const  
{ 
	return zg::GetStringFromTag(tag);
}

bool GameObject::HasChild()		const	{ return !child.empty(); }
bool GameObject::IsChild()		const	{ return !(parent == nullptr); }

bool GameObject::IsChildOf(const char* name_) const {
	for (const auto& elem : child)
		if (elem->name == name_)
			return true;
	return false;
}
bool GameObject::IsChildOf(const std::string& name_) const {
	return IsChildOf(name_.c_str());
}

bool GameObject::IsChildrenOf(const GameObject* root_parent_) const {

	auto getChilden = root_parent_->child;
	for (const auto& elem : getChilden)
	{
		if (this == elem)
			return true;
		if (IsChildrenOf(elem))
			return true;
	}
	return false;
}
//check if is under the single parent	
bool GameObject::IsChildOf(const GameObject* parent_) const {
	return parent == parent_;
}

bool	GameObject::IsArchetype()		const	{ return !archName.empty(); }
bool	GameObject::IsActive()			const	{ return toggleActive; }
bool	GameObject::IsStatic()			const	{ return toggleStatic; }


// ____________________________________________________________ setters
void GameObject::SetActive(bool toggleActive_) {
	toggleActive = toggleActive_;
	transform.SetStatic(!toggleActive || toggleStatic);
	for (auto& elem : child)
		elem->SetActive(toggleActive_);
}

void GameObject::SetStatic(bool toggleStatic_){
	toggleStatic = toggleStatic_;
	transform.SetStatic(!toggleActive || toggleStatic);
//	unsigned a = GetComponentID<RigidBody>();
//	if (a == COMPO_NOT_EXIST)
//		return;
//#if defined(_EDITOR_ON)
//	if(toggleStatic)
//		RM::R_RigidBody[a]->SetBodyType(STATIC);
//	else 
//		RM::R_RigidBody[a]->SetBodyType(DYNAMIC);
//#else
//	if (toggleStatic)
//		RM::R_RigidBody[a].SetBodyType(STATIC);
//	else				  
//		RM::R_RigidBody[a].SetBodyType(DYNAMIC);
//#endif
}

void GameObject::SetID(size_t id_) {
	id = id_;
}
void GameObject::SetLayerNum(int num_) {
	layerNumber = num_;
}

void GameObject::SetTag(const char* tag_) {
	tag = zg::GetTagFromString(tag_);
}
void GameObject::SetTag(const std::string& tag_) {
	SetTag(tag_.c_str());
}

//____________________________________________________________ etc function
void GameObject::Attach(GameObject*& parent_) {
	//if parent is nullptr
	//if parent is itself
	//if parent is same
	if (!parent_ || parent_ == this || parent == parent_)
		return;

	//if parent already exist
	if (parent)
	{
		//delete child from old parent
		auto b = parent->child.begin();
		auto e = parent->child.end();
		std::vector<GameObject*>::iterator itrToRemove = std::find(b, e, this);
		parent->child.erase(itrToRemove);
	}

	//assign to the new parent
	parent = parent_;
	transform.Attach(&parent->transform);
	parent->child.push_back(this);
	parentID = parent->id;
}

void GameObject::Detach()
{
	//if no parent
	if (!parent) return;

	//remove myself from parent
	auto b = parent->child.begin();
	auto e = parent->child.end();
	std::vector<GameObject*>::iterator itrToRemove = std::find(b, e, this);
	parent->child.erase(itrToRemove);

	//detach transform
	transform.Detach();
	parent = nullptr;
	parentID = COMPO_NOT_EXIST;
}

void GameObject::RemoveChild(GameObject* child_) {
	std::vector<GameObject*>::iterator b = child.begin();
	while (b != child.end()) {
		if (*b == child_) {
			child.erase(b);
			break;
		}
		++b;
	}
}

void GameObject::RearrangeChild(GameObject* childToMove, GameObject* newChildPos) {
	auto erasePos = std::find(child.begin(), child.end(), childToMove);

	if (erasePos == child.end())
		return;

	child.erase(erasePos);

	auto insertPos = std::find(child.begin(), child.end(), newChildPos);

	if (insertPos == child.end())
		return;

	child.insert(insertPos, childToMove);
}

void GameObject::RenderParentAndChild()
{
	Gizmo::Sphere(transform.position, 0.025f, CC_RED);
	for (const auto& elem : child)
	{
		Gizmo::Line(elem->parent->transform.position, elem->transform.position, CC_BLUE);
		elem->RenderParentAndChild();
	}
}

void GameObject::UpdateParentID() 
{
	parentID = parent ? parent->id : COMPO_NOT_EXIST;
}

//____________________________________________________________ serial
#ifdef TINYXML2_EXPORT
void GameObject::Serial(FXMLElement* pElem) {
	pElem->SetAttribute("name", name.c_str());
	pElem->SetAttribute("archName", archName.c_str());
	pElem->SetAttribute("toggleActive", toggleActive);
	pElem->SetAttribute("toggleStatic", toggleStatic);

	pElem->SetAttribute("tag", zg::GetStringFromTag(tag).c_str());
	pElem->SetAttribute("layerNumber", layerNumber);
	pElem->SetAttribute("id", static_cast<unsigned>(id));
	pElem->SetAttribute("parentID", static_cast<unsigned>(parentID));
}

void GameObject::Unserial(FXMLElement* pElem) {
	name		= pElem->Attribute("name");
	archName	= pElem->Attribute("archName");	
	SetActive(pElem->BoolAttribute("toggleActive"));
	SetStatic(pElem->BoolAttribute("toggleStatic"));

	tag			= zg::GetTagFromString(pElem->Attribute("tag"));
	layerNumber = pElem->IntAttribute("layerNumber");
	id			= pElem->IntAttribute("id");
	parentID	= pElem->IntAttribute("parentID");
	transform.SetStatic(!toggleActive || toggleStatic);
}

#endif

#if defined(USING_SOL2)
//#if defined(_EDITOR_ON) || defined(_INGAME)
//sol::object GameObject::GetScript(std::string filename, std::string key)
//{
//	unsigned id_ = mScript[FindScriptHashFromString(filename)];
//	Script* script = nullptr;
//	if (id_ != COMPO_NOT_EXIST)
//	{
//		script = RM::R_Script[id_];
//		return script->lua[key];
//	}
//
//	return NULL;
//}

bool GameObject::HasScript(const std::string& filename){
	//unsigned _id = mScript[FindScriptHashFromString(filename)];
	size_t getid = FindScriptHashFromString(filename);
	auto b = mScript.find(getid);
	if (b != mScript.end())
	{
		return true;
	}
	return false;
}
Script*	GameObject::GetScriptPtr(const std::string& filename) {
	size_t getid = FindScriptHashFromString(filename);
	auto b = mScript.find(getid);
	if (b != mScript.end())
	{
#if defined(_EDITOR_ON)
		return RM::R_Script[b->second];
#else
		return RM::R_Script + b->second;
#endif
	}
	return nullptr;
}
sol::environment& GameObject::GetScript(const std::string& filename)
{
	unsigned _id = mScript[FindScriptHashFromString(filename)];
	Script* script = nullptr;
	if (_id != COMPO_NOT_EXIST)
	{
#if defined(_EDITOR_ON)
		script = RM::R_Script[_id];
#else
		script = RM::R_Script + _id;
#endif
		return script->lua;
	}

	ErrorMsg("No Script Found!");
	return script->lua;
}
#endif

Camera* GameObject::GetCamera()
{
	size_t checkID = GetComponentID<Camera>();
#if defined(_EDITOR_ON) || !defined(_EDITOR_ON)
	return (checkID == COMPO_NOT_EXIST) ? nullptr : RM::R_Camera[checkID];
#else
#if defined(_DEBUG)
	return (checkID == COMPO_NOT_EXIST) ? nullptr : RM::R_Camera + checkID;
#else
	return RM::R_Camera[GetComponentID<Camera>()];
#endif
#endif
}

MeshRenderer* GameObject::GetMeshRenderer()
{
	size_t checkID = GetComponentID<MeshRenderer>();
#if defined(_EDITOR_ON) || !defined(_EDITOR_ON)
	return (checkID == COMPO_NOT_EXIST) ? nullptr : RM::R_MeshRenderer[checkID];
#else
#if defined(_DEBUG)
	return (checkID == COMPO_NOT_EXIST) ? nullptr : RM::R_MeshRenderer + checkID;
#else
	return RM::R_MeshRenderer[GetComponentID<MeshRenderer>()];
#endif
#endif
}

//Sprite2D* GameObject::GetSprite2D()
//{
//#if defined(_EDITOR_ON)
//	unsigned _id = GetComponentID<Sprite2D>();
//	if (_id != COMPO_NOT_EXIST)
//		return RM::R_Sprite2D[_id];
//	else {
//		//ErrorMsg(std::string{ "Sprite2D Component does not exist in" + name }.c_str());
//		//return RM::R_Sprite2D[_id];
//		return nullptr;
//	}
//#else
//#if defined(DEBUG_MODE_ON) || defined(_DEBUG)
//	unsigned _id = GetComponentID<Sprite2D>();
//	if (_id != COMPO_NOT_EXIST)
//		return RM::R_Sprite2D + _id;
//	else {
//		//ErrorMsg(std::string{ "Sprite2D Component does not exist in" + name }.c_str());
//		//return RM::R_Sprite2D + _id;
//		return nullptr;
//	}
//#else
//	return RM::R_Sprite2D[GetComponentID<Sprite2D>()];
//#endif
//#endif
//}
//
//ParticleEmitter* GameObject::GetParticleEmitter()
//{
//#if defined(_EDITOR_ON)
//	unsigned _id = GetComponentID<ParticleEmitter>();
//	if (_id != COMPO_NOT_EXIST)
//		return RM::R_ParticleEmitter[_id];
//	else {
//		//ErrorMsg(std::string{ "ParticleEmitter Component does not exist in" + name }.c_str());
//		//return RM::R_ParticleEmitter[_id];
//		return nullptr;
//	}
//#else
//#if defined(DEBUG_MODE_ON) || defined(_DEBUG)
//	unsigned _id = GetComponentID<ParticleEmitter>();
//	if (_id != COMPO_NOT_EXIST)
//		return RM::R_ParticleEmitter + _id;
//	else {
//		//ErrorMsg(std::string{ "ParticleEmitter Component does not exist in" + name }.c_str());
//		//return RM::R_ParticleEmitter + _id;
//		return nullptr;
//	}
//#else
//	return RM::R_ParticleEmitter[GetComponentID<ParticleEmitter>()];
//#endif
//#endif
//}
//
//AABB* GameObject::GetAABB()
//{
//#if defined(_EDITOR_ON)
//	unsigned _id = GetComponentID<AABB>();
//	if (_id != COMPO_NOT_EXIST)
//		return RM::R_AABB[_id];
//		
//	else {
//		//ErrorMsg(std::string{ "AABB Component does not exist in" + name }.c_str());
//		//return RM::R_AABB[_id];
//		return nullptr;
//	}
//#else
//#if defined(DEBUG_MODE_ON) || defined(_DEBUG)
//	unsigned _id = GetComponentID<AABB>();
//	if (_id != COMPO_NOT_EXIST)
//		return RM::R_AABB + _id;
//	else {
//		//ErrorMsg(std::string{ "AABB Component does not exist in" + name }.c_str());
//		//return RM::R_AABB + _id;
//		return nullptr;
//	}
//#else
//	return RM::R_AABB[GetComponentID<AABB>()];
//#endif
//#endif
//}
//
//BC* GameObject::GetBC()
//{
//#if defined(_EDITOR_ON)
//	unsigned _id = GetComponentID<BC>();
//	if (_id != COMPO_NOT_EXIST)
//		return RM::R_BC[_id];
//	else {
//		//ErrorMsg(std::string{ "BC Component does not exist in" + name }.c_str());
//		//return RM::R_BC[_id];
//		return nullptr;
//	}
//#else
//#if defined(DEBUG_MODE_ON) || defined(_DEBUG)
//	unsigned _id = GetComponentID<BC>();
//	if (_id != COMPO_NOT_EXIST)
//		return RM::R_BC + _id;
//	else {
//		//ErrorMsg(std::string{ "BC Component does not exist in" + name }.c_str());
//		//return RM::R_BC + _id;
//		return nullptr;
//	}
//#else
//	return RM::R_BC[GetComponentID<BC>()];
//#endif
//#endif
//}
//
//CapsuleCollider* GameObject::GetCapsuleCollider()
//{
//#if defined(_EDITOR_ON)
//	unsigned _id = GetComponentID<CapsuleCollider>();
//	if (_id != COMPO_NOT_EXIST)
//		return RM::R_CapsuleCollider[_id];
//	else {
//		//ErrorMsg(std::string{ "CapsuleCollider Component does not exist in" + name }.c_str());
//		//return RM::R_CapsuleCollider[_id];
//		return nullptr;
//	}
//#else
//#if defined(DEBUG_MODE_ON) || defined(_DEBUG)
//	unsigned _id = GetComponentID<CapsuleCollider>();
//	if (_id != COMPO_NOT_EXIST)
//		return RM::R_CapsuleCollider + _id;
//	else {
//		//ErrorMsg(std::string{ "CapsuleCollider Component does not exist in" + name }.c_str());
//		//return RM::R_CapsuleCollider + _id;
//		return nullptr;
//	}
//#else
//	return RM::R_CapsuleCollider[GetComponentID<CapsuleCollider>()];
//#endif
//#endif
//}
//
//OBB* GameObject::GetOBB()
//{
//#if defined(_EDITOR_ON)
//	unsigned _id = GetComponentID<OBB>();
//	if (_id != COMPO_NOT_EXIST)
//		return RM::R_OBB[_id];
//	else {
//		//ErrorMsg(std::string{ "OBB Component does not exist in" + name }.c_str());
//		//return RM::R_OBB[_id];
//		return nullptr;
//	}
//#else
//#if defined(DEBUG_MODE_ON) || defined(_DEBUG)
//	unsigned _id = GetComponentID<OBB>();
//	if (_id != COMPO_NOT_EXIST)
//		return RM::R_OBB + _id;
//	else {
//		//ErrorMsg(std::string{ "OBB Component does not exist in" + name }.c_str());
//		//return RM::R_OBB + _id;
//		return nullptr;
//	}
//#else
//	return RM::R_OBB[GetComponentID<OBB>()];
//#endif
//#endif
//}
//
//RigidBody* GameObject::GetRigidBody()
//{
//#if defined(_EDITOR_ON)
//	unsigned _id = GetComponentID<RigidBody>();
//	if (_id != COMPO_NOT_EXIST)
//		return RM::R_RigidBody[_id];
//	else {
//		ErrorMsg(std::string{ "RigidBody Component does not exist in GameObject: " + name }.c_str());
//		return RM::R_RigidBody[_id];
//	}
//#else
//#if defined(DEBUG_MODE_ON) || defined(_DEBUG)
//	unsigned _id = GetComponentID<RigidBody>();
//	if (_id != COMPO_NOT_EXIST)
//		return RM::R_RigidBody + _id;
//	else {
//		ErrorMsg(std::string{ "RigidBody Component does not exist in GameObject: " + name }.c_str());
//		return RM::R_RigidBody + _id;
//	}
//#else
//	return RM::R_RigidBody[GetComponentID<RigidBody>()];
//#endif
//#endif
//}
//Sound* GameObject::GetSound()
//{
//#if defined(_EDITOR_ON)
//	unsigned _id = GetComponentID<Sound>();
//	if (_id != COMPO_NOT_EXIST)
//		return RM::R_Sound[_id];
//	else {
//		//ErrorMsg(std::string{ "Sound Component does not exist in GameObject: " + name }.c_str());
//		return nullptr;
//	}
//#else
//#if defined(DEBUG_MODE_ON) || defined(_DEBUG)
//	unsigned _id = GetComponentID<Sound>();
//	if (_id != COMPO_NOT_EXIST)
//		return RM::R_Sound + _id;
//	else {
//		//ErrorMsg(std::string{ "Sound Component does not exist in GameObject: " + name }.c_str());
//		return nullptr;
//	}
//#else
//	return RM::R_Sound[GetComponentID<Sound>()];
//#endif
//#endif
//}
//
//TextRenderer * GameObject::GetTextRenderer()
//{
//#if defined(_EDITOR_ON)
//	unsigned _id = GetComponentID<TextRenderer>();
//	if (_id != COMPO_NOT_EXIST)
//		return RM::R_TextRenderer[_id];
//	else {
//		//ErrorMsg(std::string{ "TextRenderer Component does not exist in" + name }.c_str());
//		//return RM::R_TextRenderer[_id];
//		return nullptr;
//	}
//#else
//#if defined(DEBUG_MODE_ON) || defined(_DEBUG)
//	unsigned _id = GetComponentID<TextRenderer>();
//	if (_id != COMPO_NOT_EXIST)
//		return RM::R_TextRenderer + _id;
//	else {
//		//ErrorMsg(std::string{ "TextRenderer Component does not exist in" + name }.c_str());
//		//return RM::R_TextRenderer + _id;
//		return nullptr;
//	}
//#else
//	return RM::R_TextRenderer[GetComponentID<TextRenderer>()];
//#endif
//#endif
//}
//Light* GameObject::GetLight()
//{
//#if defined(_EDITOR_ON)
//	unsigned _id = GetComponentID<Light>();
//	if (_id != COMPO_NOT_EXIST)
//		return RM::R_Light[_id];
//	else {
//		ErrorMsg(std::string{ "Light Component does not exist in" + name }.c_str());
//		return RM::R_Light[_id];
//	}
//#else
//#if defined(DEBUG_MODE_ON) || defined(_DEBUG)
//	unsigned _id = GetComponentID<Light>();
//	if (_id != COMPO_NOT_EXIST)
//		return RM::R_Light + _id;
//	else {
//		ErrorMsg(std::string{ "Light Component does not exist in" + name }.c_str());
//		return RM::R_Light + _id;
//	}
//#else
//	return RM::R_Light[GetComponentID<Light>()];
//#endif
//#endif
//}
//
//UISprite* GameObject::GetUISprite() {
//#if defined(_EDITOR_ON)
//	unsigned _id = GetComponentID<UISprite>();
//	if (_id != COMPO_NOT_EXIST)
//		return RM::R_UISprite[_id];
//	else {
//		//ErrorMsg(std::string{ "UISprite Component does not exist in" + name }.c_str());
//		//return RM::R_UISprite[_id];
//		return nullptr;
//	}
//#else
//#if defined(DEBUG_MODE_ON) || defined(_DEBUG)
//	unsigned _id = GetComponentID<UISprite>();
//	if (_id != COMPO_NOT_EXIST)
//		return RM::R_UISprite + _id;
//	else {
//		//ErrorMsg(std::string{ "UISprite Component does not exist in" + name }.c_str());
//		//return RM::R_UISprite + _id;
//		return nullptr;
//	}
//#else
//	return RM::R_UISprite[GetComponentID<UISprite>()];
//#endif
//#endif
//}

#if defined(USING_SOL2)
void GameObject::BindLua(sol::state & lua)
{
	lua.new_usertype<GameObject>("GameObject",
		sol::constructors<GameObject(), GameObject(const char*), GameObject(const GameObject&)>(),

		"GetParent", &GameObject::GetParent,
		"GetRootParent", &GameObject::GetRootParent,

		"GetChildByName", sol::overload(
			static_cast<GameObject* (GameObject::*)(const std::string&)>(&GameObject::GetChildByName),
			static_cast<GameObject* (GameObject::*)(const char*)>(&GameObject::GetChildByName)
		),

		"IsChild", sol::overload(
			static_cast<bool (GameObject::*)(const std::string&) const>(&GameObject::IsChild),
			static_cast<bool (GameObject::*)(const char*) const>(&GameObject::IsChild)
		),

		"HasChild", &GameObject::HasChild,
		"GetAllChildren", &GameObject::GetAllChildren,
		"GetAllChildrenByName", &GameObject::GetAllChildrenByName,
		"GetAllChildrenByArchName", &GameObject::GetAllChildrenByArchName,
		"GetAllChildrenByTagName", &GameObject::GetAllChildrenByTagName,
		"IsArchetype", &GameObject::IsArchetype,
		"IsActive", &GameObject::IsActive,
		"IsStatic", &GameObject::IsStatic,
		"GetLayerNumber", &GameObject::GetLayerNumber,

		"GetTag", &GameObject::GetTag,
		"GetTagPtr", &GameObject::GetTagPtr,

		"GetTagString", &GameObject::GetTagString,
		"GetID", &GameObject::GetID,
		"GetParentID", &GameObject::GetParentID,

		"GetTransform", &GameObject::GetTransform,

		"SetActive", &GameObject::SetActive,
		"SetStatic", &GameObject::SetStatic,
		"SetLayerNum", &GameObject::SetLayerNum,
		"SetTag", sol::overload(
			static_cast<void (GameObject::*)(std::string&&)>(&GameObject::SetTag),
			static_cast<void (GameObject::*)(const std::string&)>(&GameObject::SetTag)
		),

		//"SetID", &GameObject::SetID,
		//"UpdateParentID", &GameObject::UpdateParentID,

		"Attach", &GameObject::Attach,
		"Detach", &GameObject::Detach,
		"RemoveChild", &GameObject::RemoveChild,

		"HasScript", &GameObject::HasScript,
		"GetScriptPtr", &GameObject::GetScriptPtr,
		"GetScript", &GameObject::GetScript,
		"GetCamera", &GameObject::GetCamera,
		"GetMeshRenderer", &GameObject::GetMeshRenderer,
		"GetSprite2D", &GameObject::GetSprite2D,
		"GetAABB", &GameObject::GetAABB,
		"GetBC", &GameObject::GetBC,
		"GetOBB", &GameObject::GetOBB,
		"GetCapsuleCollider", &GameObject::GetCapsuleCollider,
		"GetRigidBody", &GameObject::GetRigidBody,
		"GetSound", &GameObject::GetSound,
		"GetParticleEmitter", &GameObject::GetParticleEmitter,
		"GetTextRenderer", &GameObject::GetTextRenderer,
		"GetLight", &GameObject::GetLight,
		"GetUISprite", &GameObject::GetUISprite,

		"name", &GameObject::name,
		"archName", &GameObject::archName,
		"transform", &GameObject::transform
		);
}
#endif

namespace zg {
	ZG_API size_t next_UniqueGameObjectID = 0;

	std::map<size_t, std::string>	GLOBAL_GAMETAG_STRING_TABLE;

	size_t AddGameObjectTag(const char* tagname_) {
		static std::hash<std::string> hasher;
		size_t hashedString = hasher(tagname_);

		auto f = GLOBAL_GAMETAG_STRING_TABLE.find(hashedString);
		if (f == GLOBAL_GAMETAG_STRING_TABLE.end()) {
			GLOBAL_GAMETAG_STRING_TABLE[hashedString] = tagname_;
			//GLOBAL_COLLISION_GROUP_STRING_TABLE[hashedString] = std::vector<size_t>{};
			//GLOBAL_RAYCAST_GROUP_STRING_TABLE[hashedString] = std::vector<size_t>{};
		}
		return hashedString;
	}
	size_t AddGameObjectTag(const std::string& string_) { return AddGameObjectTag(string_.c_str()); }

	void RemoveGameObjectTag(const std::string& string_) {
		static std::hash<std::string> hasher;
		size_t hashedString = hasher(string_);
		auto find = GLOBAL_GAMETAG_STRING_TABLE.find(hashedString);
		if (find != GLOBAL_GAMETAG_STRING_TABLE.end()) {
			GLOBAL_GAMETAG_STRING_TABLE.erase(hashedString);
			//GLOBAL_COLLISION_GROUP_STRING_TABLE.erase(hashedString);
			//GLOBAL_RAYCAST_GROUP_STRING_TABLE.erase(hashedString);
		}
	}

	size_t GetTagFromString(const char* tagname_) {
		static std::hash<std::string> hasher;
		size_t hashedString = hasher(tagname_);
#if defined(_DEBUG) || defined(_EDITOR_ON)
		auto it = GLOBAL_GAMETAG_STRING_TABLE.find(hashedString);
		if (it == GLOBAL_GAMETAG_STRING_TABLE.end())
			ErrorMsg(std::string{ "Tag's string not found" }.c_str());
		return hashedString;
#else
		return hashedString;
#endif
	}
	size_t GetTagFromString(const std::string& tagname_) { return GetTagFromString(tagname_.c_str()); }

	std::string GetStringFromTag(size_t hash_) {
#if defined(_DEBUG) || defined(_EDITOR_ON)
		auto it = GLOBAL_GAMETAG_STRING_TABLE.find(hash_);
		if (it == GLOBAL_GAMETAG_STRING_TABLE.end()) {
			return std::string{ "default" };
		}
		return it->second;
#else
		return GLOBAL_GAMETAG_STRING_TABLE[hash_];
#endif
	}

	std::vector<std::string> GetListOfGameTagString() {
		std::vector<std::string> tmp;
		tmp.resize(GLOBAL_GAMETAG_STRING_TABLE.size());
		auto b = tmp.begin();
		for (auto& elem : GLOBAL_GAMETAG_STRING_TABLE) {
			*b++ = elem.second;
		}
		return tmp;
	}

	GameObject* FindGameObjectByID(int id_)
	{
#if defined(_EDITOR_ON) || ! defined(_EDITOR_ON) 
		for (auto& elem : RM::R_GameObject) {
			if (elem->GetID() == id_)
				return elem;
		}
		return nullptr;
#else
		GameObject* b = RM::R_GameObject;
		GameObject* e = RM::R_GameObject + RM::C_GameObject;
		while (b != e) {
			if (b->GetID() == id)
				return b;
			++b;
		}
		return nullptr;
#endif
	}


	GameObject* FindGameObjectByName(const char* gameobjectname_) {
		static std::hash<std::string> hasher;
		size_t hashedString = hasher(gameobjectname_);
#if defined(_EDITOR_ON) || ! defined(_EDITOR_ON) 
		for (auto& elem : RM::R_GameObject) {
			if (hasher(elem->name) == hashedString)
				return elem;
		}
		return nullptr;
#else
		GameObject* b = RM::R_GameObject;
		GameObject* e = RM::R_GameObject + RM::C_GameObject;
		while (b != e) {
			if (hasher(b->name) == hashedString)
				return b;
			++b;
		}
		return nullptr;
#endif
	}
	GameObject* FindGameObjectByName(const std::string& gameobjectname_) { return FindGameObjectByName(gameobjectname_.c_str()); }

	std::vector<GameObject*> FindListOfGameObjectByName(const char* gameobjectname_) {
		static std::hash<std::string> hasher;
		size_t hashedString = hasher(gameobjectname_);
		std::vector<GameObject*> list{};
#if defined(_EDITOR_ON) || !defined(_EDITOR_ON)
		for (auto& elem : RM::R_GameObject) {
			if (hasher(elem->name) == hashedString)
				list.push_back(elem);
		}
#else
		GameObject* b = RM::R_GameObject;
		GameObject* e = RM::R_GameObject + RM::C_GameObject;
		while (b != e) {
			if (hasher(b->name) == hashedString)
				list.push_back(b);
			++b;
		}
#endif
		return list;
	}
	std::vector<GameObject*> FindListOfGameObjectByName(const std::string& gameobjectname_) { return FindListOfGameObjectByName(gameobjectname_.c_str()); }

	std::vector<GameObject*> FindListOfGameObjectByArchName(const char* archname_) {
		static std::hash<std::string> hasher;
		size_t hashedString = hasher(archname_);
		std::vector<GameObject*> list{};
#if defined(_EDITOR_ON)
		for (auto& elem : RM::R_GameObject) {
			if (hasher(elem->archName) == hashedString)
				list.push_back(elem);
		}
#else
		GameObject* b = RM::R_GameObject;
		GameObject* e = RM::R_GameObject + RM::C_GameObject;
		while (b != e) {
			if (hasher(b->archName) == hashedString)
				list.push_back(b);
			++b;
		}
#endif
		return list;
	}
	std::vector<GameObject*> FindListOfGameObjectByArchName(const std::string& gameobjectname_) { return FindListOfGameObjectByArchName(gameobjectname_.c_str()); }

	std::vector<GameObject*>		FindListOfGameObjectByTag(size_t hash_) {
		std::vector<GameObject*> list{};
#if defined(_EDITOR_ON)
		for (auto& elem : RM::R_GameObject) {
			if (elem->GetTag() == hash_)
				list.push_back(elem);
		}
#else
		GameObject* b = RM::R_GameObject;
		GameObject* e = RM::R_GameObject + RM::C_GameObject;
		while (b != e) {
			if (b->GetTag() == hashedString)
				list.push_back(b);
			++b;
		}
#endif
		return list;
	}
	std::vector<GameObject*> FindListOfGameObjectByTagName(const char* tagname_) {
		static std::hash<std::string> hasher;
		return FindListOfGameObjectByTag(hasher(tagname_));
	}
	std::vector<GameObject*> FindListOfGameObjectByTagName(const std::string& tagname_) {
		static std::hash<std::string> hasher;
		return FindListOfGameObjectByTag(hasher(tagname_));
	}
} //namespace zg
