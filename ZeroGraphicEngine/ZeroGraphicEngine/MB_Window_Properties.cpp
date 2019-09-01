#include "stdafx.h"
#include "EditorMB.h"


static float drag_float_speed = 0.1f;
void Show_Transform();
void Show_MeshRenderer();

void MB_Window_Properties() {

	if (!selectedObj.isEmpty()) 
	{
		ImGui::SetWindowPos(ImVec2{ IMGU.w * 0.6f, editor_menubar_offsety }, ImGuiCond_Once);
		ImGui::SetWindowSize(ImVec2{ 200.0f, IMGU.h - editor_menubar_offsety }, ImGuiCond_Once);

		ImGui::Begin(toggle_name[WB_PROPERTIES], &toggle_WB[WB_PROPERTIES], ImVec2{}, -1.0f, ImGuiWindowFlags_AlwaysUseWindowPadding);

		selectedObj.ForceUpdate();
		Show_Transform();
		if (selectedObj.meshRenderer) Show_MeshRenderer();
		ImGui::End();
	}
}

void Show_Transform() 
{
	Transform& compo = selectedObj.gameObject->transform;
	//if (compo.IsBone()) {
	//	if (ImGui::TreeNodeEx("Transform", ImGuiTreeNodeFlags_DefaultOpen))
	//	{
	//		ImGui::Text("Bone GameObject, cannot be changed");
	//		ImGui::TreePop();
	//	}
	//	return;
	//}

	ImGui::DragFloat("Unversal DragFloat_Value", &drag_float_speed, 0.01f);

	GameObject* getParent = selectedObj.gameObject->GetParent();
	ImGui::Text("Name: %s, ID: %d", selectedObj.gameObject->name.c_str(), selectedObj.gameObject->GetID());

	if(getParent)
		ImGui::Text("Parent: %s, ID: %d", getParent->name.c_str(), selectedObj.gameObject->GetParentID());
	else
		ImGui::Text("Parent: %s, ID: %d", "", selectedObj.gameObject->GetParentID());

	if (ImGui::TreeNodeEx("Transform", ImGuiTreeNodeFlags_DefaultOpen)) 
	{
		if (ImGui::Button("Reset Transform")) 
		{
			selectedObj.eularAngle.SetZero();
			selectedObj.localEularAngle.SetZero();
			compo.Reset();
		}

		if (!getParent)
		{
			if (ImGui::DragFloat3("Position", &compo.position.x, drag_float_speed))
			{
				compo.SetPosition(compo.position);
			}
			//localEular = compo.rotation.ToEularDegree();
			if (ImGui::DragFloat3("Rotation", &selectedObj.eularAngle.x, drag_float_speed))
			{
				compo.SetRotation(selectedObj.eularAngle);
			}
			zg::Vector3 ee2 = compo.rotation.ToEularDegree();
			ImGui::DragFloat4("QRotation", &compo.rotation.x, drag_float_speed);
			ImGui::DragFloat3("QRotationE", &ee2.x, drag_float_speed);
			if (ImGui::DragFloat3("Scale", &compo.scale.x, drag_float_speed))
			{
				compo.SetScale(compo.scale);
			}
		}
		else 
		{
			if (ImGui::DragFloat3("LocalPosition", &compo.localPosition.x, drag_float_speed))
			{
				compo.SetLocalPosition(compo.localPosition);
			}
			//localEular = compo.rotation.ToEularDegree();
			if (ImGui::DragFloat3("LocalRotation", &selectedObj.localEularAngle.x, drag_float_speed))
			{
				compo.SetLocalRotation(selectedObj.localEularAngle);
			}
			zg::Vector3 ee = compo.localRotation.ToEularDegree();
			ImGui::DragFloat4("QlocalRotation", &compo.localRotation.x, drag_float_speed);
			ImGui::DragFloat3("QlocalRotationE", &ee.x, drag_float_speed);
			
			if (ImGui::DragFloat3("LocalScale", &compo.localScale.x, drag_float_speed))
			{
				compo.SetLocalScale(compo.localScale);
			}
		}
		ImGui::TreePop();
	}
	
}

void Show_MeshRenderer() 
{
	MeshRenderer* compo = selectedObj.gameObject->GetMeshRenderer();
	
	if (ImGui::TreeNodeEx("MeshRenderer", ImGuiTreeNodeFlags_DefaultOpen))
	{
		//Color
		ImGui::ColorEdit4("Color", &compo->color.r);

		//Gemerate Bones
		if (ImGui::Checkbox("Generate Bones", &selectedObj.meshgeneratebonesss))
		{
			compo->SetModel(compo->GetModel(), selectedObj.meshgeneratebonesss);
		}
		//Set Texture
		static std::vector<std::string> mmmm_textureList;
		ImGui::Text("Diffuse");
		ImGui::SameLine();
		if (ImGui::Button(compo->textureMaps[0]->filename.c_str()))
		{
			mmmm_textureList = GetAllTexture2DName();
			std::sort(mmmm_textureList.begin(), mmmm_textureList.end());
		}
		if (ImGui::BeginPopupContextItem("List_of_Textures", 0))
		{
			for (auto elem : mmmm_textureList)
			{
				if (ImGui::Selectable(elem.c_str()))
				{
					compo->SetDiffuse(elem);
				}
			}

			ImGui::EndPopup();
		}

		//Set Model
		static std::vector<std::string> mmmm_modelList;
		ImGui::Text("Model");
		ImGui::SameLine();
		if (ImGui::Button(compo->GetModel()->filename.c_str()))
		{
			mmmm_modelList = GetAllModelName();
			std::sort(mmmm_modelList.begin(), mmmm_modelList.end());
		}
		ToolTip("Left Click to Select Model");
		if (ImGui::BeginPopupContextItem("List_of_Model", 0))
		{
			for (auto elem : mmmm_modelList)
			{
				if (ImGui::Selectable(elem.c_str()))
				{
					if (compo->GetModel()->filename == elem)
						break;
					compo->SetModel(elem, selectedObj.meshgeneratebonesss);
				}
			}

			ImGui::EndPopup();
		}

		//Loop Animation
		ImGui::Checkbox("Loop Animation", &compo->toggleLoop);
		static bool playyyy = false;
		if(ImGui::Checkbox("Play Animation", &playyyy)) {
			if (playyyy)
				compo->Play();
			else
				compo->Stop();
		}
		ImGui::SliderFloat("Animation Speed", &compo->animationSpeed, 0.0f, 10.0f);

		//Set Animation
		const auto& anims = compo->GetModel()->animations;

		if (ImGui::BeginCombo("Current Animation", compo->GetAnimationTitle().c_str()))
		{
			for (const auto& elem : anims) {
				if (ImGui::Selectable(GetModelNameFromHash(elem.first).c_str()))
				{
					compo->SetAnimation(GetModelNameFromHash(elem.first));
				}
			}

			ImGui::EndCombo();
		}

		ImGui::TreePop();
	}
}