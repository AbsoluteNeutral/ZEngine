/*****************************************************************************/
/*!
\file			ResourceArchtype.cpp
\brief			Handles ResourceArchtype allocation
*/
/*****************************************************************************/
#include "stdafx.h"
#include "ResourceManager.h"

namespace ResourceArchtype {
	ZG_API std::map<size_t, unsigned> /* GameObject*>	*/	ARCH_GameObject;
	ZG_API std::map<size_t, unsigned> /* Camera*>		*/	ARCH_Camera;
	ZG_API std::map<size_t, unsigned> /* MeshRenderer*>	*/	ARCH_MeshRenderer;
	ZG_API std::map<size_t, unsigned> /* Sprite2D*>		*/	ARCH_Sprite2D;
	ZG_API std::map<size_t, unsigned> /* ParticleEmitter**/ ARCH_ParticleEmitter;
	ZG_API std::map<size_t, unsigned> /* AABB*>			*/	ARCH_AABB;
	ZG_API std::map<size_t, unsigned> /* BC*>			*/	ARCH_BC;
	ZG_API std::map<size_t, unsigned> /* CapsuleCollider**/ ARCH_CapsuleCollider;
	ZG_API std::map<size_t, unsigned> /* OBB*>			*/	ARCH_OBB;
	ZG_API std::map<size_t, unsigned> /* RigidBody*>	*/	ARCH_RigidBody;
	ZG_API std::map<size_t, unsigned> /* unsigned>		*/	ARCH_Script;
	ZG_API std::map<size_t, unsigned> /* Sound*>		*/	ARCH_Sound;
	ZG_API std::map<size_t, unsigned> /* TextRenderer*>	*/	ARCH_TextRenderer;
	ZG_API std::map<size_t, unsigned> /* Light*>		*/	ARCH_Light;
	ZG_API std::map<size_t, unsigned> /* UISprite*>		*/	ARCH_UISprite;
	ZG_API std::map<size_t, unsigned> /* VideoCompo*>	*/	ARCH_VideoCompo;

	void ClearResourceArchtype() {
		//for (auto elem : ARCH_GameObject)		DeallocateOne(elem.second);
		//for (auto elem : ARCH_Camera)			DeallocateOne(elem.second);
		//for (auto elem : ARCH_MeshRenderer)	DeallocateOne(elem.second);
		//for (auto elem : ARCH_Sprite2D)		DeallocateOne(elem.second);
		//for (auto elem : ARCH_ParticleEmitter)DeallocateOne(elem.second);
		//for (auto elem : ARCH_AABB)			DeallocateOne(elem.second);
		//for (auto elem : ARCH_BC)				DeallocateOne(elem.second);
		//for (auto elem : ARCH_CapsuleCollider)DeallocateOne(elem.second);
		//for (auto elem : ARCH_OBB)			DeallocateOne(elem.second);
		//for (auto elem : ARCH_RigidBody)		DeallocateOne(elem.second);
		//for (auto elem : ARCH_Script)			DeallocateOne(elem.second);
		//for (auto elem : ARCH_Sound)			DeallocateOne(elem.second);
		//for (auto elem : ARCH_TextRenderer)	DeallocateOne(elem.second);
		//for (auto elem : ARCH_Light)			DeallocateOne(elem.second);
	}

	void LoadArch(const std::string& pathname_filename_exten_) {
		std::string filenameNoextension;
		GetFileNameNoExtension(pathname_filename_exten_, filenameNoextension);
		size_t hashnum = GetHashFromString(filenameNoextension);

		std::vector<GameObject*> tmpArchObjs;
		FXMLDocument doc;
		if (XMLLoadPath(doc, pathname_filename_exten_))
		{
			FXMLNode* pRoot = doc.FirstChild();
			ErrorIf(pRoot == nullptr, " nullptr, Nodes/Element not found");

			FXMLElement* pElem = pRoot->FirstChildElement();
			//ARCH_GameObject[hashnum] = 0;
			//ARCH_Camera[hashnum] = 0;
			//ARCH_MeshRenderer[hashnum] = 0;
			//ARCH_Sprite2D[hashnum] = 0;
			//ARCH_ParticleEmitter[hashnum] = 0;
			//ARCH_AABB[hashnum] = 0;
			//ARCH_BC[hashnum] = 0;
			//ARCH_CapsuleCollider[hashnum] = 0;
			//ARCH_OBB[hashnum] = 0;
			//ARCH_RigidBody[hashnum] = 0;
			//ARCH_Script[hashnum] = 0;
			//ARCH_Sound[hashnum] = 0;
			//ARCH_TextRenderer[hashnum] = 0;
			//ARCH_Light[hashnum] = 0;
			//ARCH_UISprite[hashnum] = 0;
			//ARCH_VideoCompo[hashnum] = 0;
			while (pElem)
			{
				if (strcmp(pElem->Name(), IS_GAMEOBJECT) != 0) break;

				++ARCH_GameObject[hashnum];

				FXMLElement* pSubElem = pElem->FirstChildElement();
				while (pSubElem)
				{
					const char * getName = pSubElem->Name();
					if (strcmp(getName, IS_TRANSFORM) == 0) {

					}
					else if (strcmp(getName, IS_CAMERA) == 0) {
						//Camera* component = Allocate<Camera>(1);
						//component->Attach(obj, RM::R_Camera.size());
						//component->Unserial(pSubElem);
						//RM::R_Camera.push_back(component);
						++ARCH_Camera[hashnum];
					}
					else if (strcmp(getName, IS_MESHRENDERER) == 0) {
						/*MeshRenderer* component = Allocate<MeshRenderer>(1);
						component->Attach(obj, RM::R_MeshRenderer.size());
						component->Unserial(pSubElem);
						RM::R_MeshRenderer.push_back(component);*/
						++ARCH_MeshRenderer[hashnum];
					}
					else if (strcmp(getName, IS_SPRITE2D) == 0) {
						/*Sprite2D* component = Allocate<Sprite2D>(1);
						component->Attach(obj, RM::R_Sprite2D.size());
						component->Unserial(pSubElem);
						RM::R_Sprite2D.push_back(component);*/
						++ARCH_Sprite2D[hashnum];
					}
					else if (strcmp(getName, IS_PARTICLEEMITTER) == 0) {
						/*ParticleEmitter* component = Allocate<ParticleEmitter>(1);
						component->Attach(obj, RM::R_ParticleEmitter.size());
						component->Unserial(pSubElem);
						RM::R_ParticleEmitter.push_back(component);*/
						++ARCH_ParticleEmitter[hashnum];
					}
					else if (strcmp(getName, IS_AABB) == 0) {
						/*AABB* component = Allocate<AABB>(1);
						component->Attach(obj, RM::R_AABB.size());
						component->Unserial(pSubElem);
						RM::R_AABB.push_back(component);*/
						++ARCH_AABB[hashnum];
					}
					else if (strcmp(getName, IS_BC) == 0) {
						/*BC* component = Allocate<BC>(1);
						component->Attach(obj, RM::R_BC.size());
						component->Unserial(pSubElem);
						RM::R_BC.push_back(component);*/
						++ARCH_BC[hashnum];
					}
					else if (strcmp(getName, IS_CAPSULECOLLIDER) == 0) {
						/*CapsuleCollider* component = Allocate<CapsuleCollider>(1);
						component->Attach(obj, RM::R_CapsuleCollider.size());
						component->Unserial(pSubElem);
						RM::R_CapsuleCollider.push_back(component);*/
						++ARCH_CapsuleCollider[hashnum];
					}
					else if (strcmp(getName, IS_OBB) == 0) {
						/*OBB* component = Allocate<OBB>(1);
						component->Attach(obj, RM::R_OBB.size());
						component->Unserial(pSubElem);
						RM::R_OBB.push_back(component);*/
						++ARCH_OBB[hashnum];
					}
					else if (strcmp(getName, IS_RIGIDBODY) == 0) {
						/*RigidBody* component = Allocate<RigidBody>(1);
						component->Attach(obj, RM::R_RigidBody.size());
						component->Unserial(pSubElem);
						RM::R_RigidBody.push_back(component);*/
						++ARCH_RigidBody[hashnum];
					}
					else if (strcmp(getName, IS_SCRIPT) == 0) {
						FXMLElement* pSubSubElem = pSubElem->FirstChildElement("attribute");
						while (pSubSubElem) {
							/*Script* component = Allocate<Script>(1);
							component->Unserial(pSubSubElem, obj, RM::R_Script.size());
							component->Attach(obj, RM::R_Script.size(), component->filename, pathname_filename_exten_);
							component->Init(false);
							RM::R_Script.push_back(component);*/
							++ARCH_Script[hashnum];
							pSubSubElem = pSubSubElem->NextSiblingElement();
						}
					}
					else if (strcmp(getName, IS_SOUND) == 0) {
						/*Sound* component = Allocate<Sound>(1);
						component->Attach(obj, RM::R_Sound.size());
						component->Unserial(pSubElem);
						RM::R_Sound.push_back(component);*/
						++ARCH_Sound[hashnum];
					}
					else if (strcmp(getName, IS_TEXTRENDERER) == 0) {
						/*TextRenderer* component = Allocate<TextRenderer>(1);
						component->Attach(obj, RM::R_TextRenderer.size());
						component->Unserial(pSubElem);
						RM::R_TextRenderer.push_back(component);*/
						++ARCH_TextRenderer[hashnum];
					}
					else if (strcmp(getName, IS_LIGHT) == 0) {
						/*Light* component = Allocate<Light>(1);
						component->Attach(obj, RM::R_Light.size());
						component->Unserial(pSubElem);
						RM::R_Light.push_back(component);*/
						++ARCH_Light[hashnum];
					}
					else if (strcmp(getName, IS_UISPRITE) == 0) {
						/*Light* component = Allocate<Light>(1);
						component->Attach(obj, RM::R_Light.size());
						component->Unserial(pSubElem);
						RM::R_Light.push_back(component);*/
						++ARCH_UISprite[hashnum];
					}
					else if (strcmp(getName, IS_VIDEOCOMPO) == 0) {
						/*Light* component = Allocate<Light>(1);
						component->Attach(obj, RM::R_Light.size());
						component->Unserial(pSubElem);
						RM::R_Light.push_back(component);*/
						++ARCH_VideoCompo[hashnum];
					}
					else {
#if	defined(_DEBUG)
						Warn("not a Component, \">>END\" NOT FOUND");
#endif
					}
					pSubElem = pSubElem->NextSiblingElement();
				}//end component loop

				pElem = pElem->NextSiblingElement();
			}//end loop
		}
	}

}//namespace ResourceArchtype
