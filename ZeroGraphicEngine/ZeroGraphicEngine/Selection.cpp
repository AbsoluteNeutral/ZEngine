#include "stdafx.h"
#include "Selection.h"
#include "EditorMain.h"

std::vector<Selection>	ListOfSelection{};

Selection::Selection() noexcept
	:
	//worldMatrix{}
	//, localMatrix{}
	eularAngle			{}
	, localEularAngle	{}
	, meshgeneratebonesss(true)
	, gameObject		{ nullptr }
	, camera			{ nullptr }
	, meshRenderer		{ nullptr }
	//, sprite2D			{ nullptr }
	//, particleEmitter	{ nullptr }
	//, aabb				{ nullptr }
	//, bc				{ nullptr }
	//, obb				{ nullptr }
	//, capsuleCollider	{ nullptr }
	//, rigidBody			{ nullptr }
	//, sound				{ nullptr }
	//, textRenderer		{ nullptr }
	//, light				{ nullptr }
	//, uisprite			{ nullptr }
	//, videocompo		{ nullptr }
	//, script			{}
{}

Selection::~Selection() {
}

void Selection::Set(PickingCircle* objToSelect) {
	pickingCircle = objToSelect;
	pickingCircle->Selected(true);
	Set(objToSelect->owner);
}
void Selection::Set(GameObject* rhs){
	gameObject = rhs;

	//worldMatrix = gameObject->transform.GetWorldMatrix();
	//localMatrix = gameObject->transform.GetLocalMatrix();
	eularAngle		= rhs->transform.rotation.ToEularDegree();
	localEularAngle = rhs->transform.localRotation.ToEularDegree();
	camera			= gameObject->GetCamera();
	meshRenderer	= gameObject->GetMeshRenderer();

	//unsigned checkID = 0;
	//
	//checkID			= gameObject->GetComponentID<Camera>();
	//camera			= checkID == COMPO_NOT_EXIST ? nullptr : RM::R_Camera[checkID];
	//
	//checkID			= gameObject->GetComponentID<MeshRenderer>();
	//meshRenderer	= checkID == COMPO_NOT_EXIST ? nullptr : RM::R_MeshRenderer[checkID];
	//
	//checkID			= gameObject->GetComponentID<Sprite2D>();
	//sprite2D		= checkID == COMPO_NOT_EXIST ? nullptr : RM::R_Sprite2D[checkID];
	//
	//checkID			= gameObject->GetComponentID<ParticleEmitter>();
	//particleEmitter = checkID == COMPO_NOT_EXIST ? nullptr : RM::R_ParticleEmitter[checkID];
	//
	//checkID			= gameObject->GetComponentID<AABB>();
	//aabb			= checkID == COMPO_NOT_EXIST ? nullptr : RM::R_AABB[checkID];
	//
	//checkID			= gameObject->GetComponentID<BC>();
	//bc				= checkID == COMPO_NOT_EXIST ? nullptr : RM::R_BC[checkID];
	//
	//checkID			= gameObject->GetComponentID<CapsuleCollider>();
	//capsuleCollider = checkID == COMPO_NOT_EXIST ? nullptr : RM::R_CapsuleCollider[checkID];
	//
	//checkID			= gameObject->GetComponentID<OBB>();
	//obb				= checkID == COMPO_NOT_EXIST ? nullptr : RM::R_OBB[checkID];
	//
	//checkID			= gameObject->GetComponentID<RigidBody>();
	//rigidBody		= checkID == COMPO_NOT_EXIST ? nullptr : RM::R_RigidBody[checkID];
	//
	////script
	//script.clear();
	//std::map<size_t, unsigned>& getmap = gameObject->GetListScript();
	//for (auto& elem : getmap) {
	//	script.push_back(RM::R_Script[elem.second]);
	//}
	//
	//checkID			= gameObject->GetComponentID<Sound>();
	//sound			= checkID == COMPO_NOT_EXIST ? nullptr : RM::R_Sound[checkID];
	//
	//checkID			= gameObject->GetComponentID<TextRenderer>();
	//textRenderer	= checkID == COMPO_NOT_EXIST ? nullptr : RM::R_TextRenderer[checkID];
	//
	//checkID			= gameObject->GetComponentID<Light>();
	//light			= checkID == COMPO_NOT_EXIST ? nullptr : RM::R_Light[checkID];
	//
	//checkID			= gameObject->GetComponentID<UISprite>();
	//uisprite		= checkID == COMPO_NOT_EXIST ? nullptr : RM::R_UISprite[checkID];
	//
	//checkID = gameObject->GetComponentID<VideoCompo>();
	//videocompo = checkID == COMPO_NOT_EXIST ? nullptr : RM::R_VideoCompo[checkID];
	
}

void Selection::SetEmpty(){
	if(pickingCircle)
		pickingCircle->Selected(false);

	pickingCircle	= nullptr;

	gameObject		= nullptr;
	camera			= nullptr;
	meshRenderer	= nullptr;
	//sprite2D		= nullptr;
	//particleEmitter	= nullptr;
	//aabb			= nullptr;
	//bc				= nullptr;
	//capsuleCollider	= nullptr;
	//obb				= nullptr;
	//rigidBody		= nullptr;
	//script.clear();
	//sound			= nullptr;
	//textRenderer	= nullptr;
	//light			= nullptr;
	//textRenderer	= nullptr;
	//uisprite		= nullptr;
	//videocompo		= nullptr;
}

void Selection::ForceUpdate() 
{
	eularAngle		= gameObject->transform.rotation.ToEularDegree();
	localEularAngle = gameObject->transform.localRotation.ToEularDegree();
	//if (aabb)	aabb->ForceUpdate();
	//if (bc)		bc->ForceUpdate();
	//if (capsuleCollider)	capsuleCollider->ForceUpdate();
	//if (obb)	obb->ForceUpdate();
}

bool Selection::isEmpty(){
	return gameObject == nullptr;
}

bool Selection::operator==(const Selection& rhs) const
{
	if (pickingCircle != rhs.pickingCircle)		return false;
	if (gameObject != rhs.gameObject)			return false;
	if (camera != rhs.camera)					return false;
	if (meshRenderer != rhs.meshRenderer)		return false;
	//if (sprite2D != rhs.sprite2D)				return false;
	//if (particleEmitter != rhs.particleEmitter) return false;
	//if (aabb != rhs.aabb)						return false;
	//if (bc != rhs.bc)							return false;
	//if (obb != rhs.obb)							return false;
	//if (capsuleCollider != rhs.capsuleCollider)	return false;
	//if (rigidBody != rhs.rigidBody)				return false;
	//if (script != rhs.script)					return false;
	//if (sound != rhs.sound)						return false;
	//if (textRenderer != rhs.textRenderer)		return false;
	//if (light != rhs.light)						return false;
	//if (textRenderer != rhs.textRenderer)		return false;
	//if (uisprite != rhs.uisprite)				return false;
	//if (videocompo != rhs.videocompo)			return false;

	return true;
}


SELECTION_TYPE CheckSelectionType()
{
	//if (Multiselection.IsEmpty())
	//{
	//	return SEL_NONE;
	//}
	//
	//if (Multiselection.IsSingle())
	//{
	//	return SEL_SINGLE;
	//}

	return SEL_MULTIPLE;
}

void SelectInverse()
{
	//auto oldSel = GetAllSelected();
	//Multiselection.Clear();
	//
	//for (auto& pick : RE::R_PICKING)
	//{
	//	bool canSelect = true;
	//
	//	for (auto& sel : oldSel)
	//	{		
	//		if (pick == sel.pickingCircle)
	//		{
	//			canSelect = false;
	//			break;
	//		}
	//	}
	//
	//	if(canSelect)
	//		Multiselection.Insert(pick);
	//}
}
