#include "stdafx.h"
#include "EditorMB.h"

std::map<size_t, bool>		listOfselectionBool{};
std::vector<Selection>	listOfGameObjectSelection{};

bool toggle_inspector_collapse_all = false;
Selection selectedObj;

void SelectionUpdate_UponGameObjectAddAndDeleted(PickingCircle* picker_) {
	listOfselectionBool.clear();
	for (auto& elem : RM::R_GameObject) {
		listOfselectionBool[elem->GetID()] = false;
	}
	SelectObj(picker_->owner);
}

void SelectObj(GameObject* obj_) {
	for (auto& elem : listOfselectionBool)
		elem.second = false;

	listOfselectionBool[obj_->GetID()] = true;
	selectedObj.Set(obj_);
}

void CheckDragAndDrop(GameObject* gameobject_) 
{
	static GameObject* selected_GameObJeCt = nullptr;
	if (ImGui::BeginDragDropSource())
	{
		ImGui::SetDragDropPayload("GameObject*", gameobject_, sizeof(GameObject*));        // Set payload to carry the index of our item (could be anything)

		selected_GameObJeCt = gameobject_;
		ImGui::EndDragDropSource();
	}
	if (ImGui::BeginDragDropTarget())
	{
		if (ImGui::AcceptDragDropPayload("GameObject*"))
		{
			//IM_ASSERT(payload->DataSize == sizeof(GameObject*));
			//GameObject* payload_n = (GameObject*)payload->Data;
			if (selected_GameObJeCt) 
			{
				//if different parent
				if ((selected_GameObJeCt->GetParent() != gameobject_)) 
				{
					//if the targetObject is not my child
					if (!gameobject_->IsChildrenOf(selected_GameObJeCt))
					{
						//selected_GameObJeCt->Detach();				//detach regardless 
						selected_GameObJeCt->Attach(gameobject_);	//attach to new parent
					}
					//if the targetObject is my child
					else 
					{
						//attach all children and sub-children to grandparent, 
						//then attach to children to be new parent
						//Decpitacted
						//GameObject* getGrandParent = selected_GameObJeCt->GetParent();
						////gameobject_->Detach();						//detach regardless and have no parent 
						//std::vector<GameObject*>& listofChildren = selected_GameObJeCt->GetAllChildren();
						//if (getGrandParent)								//if there is grandparent, attach it
						//{
						//	for (auto& elem : listofChildren) {
						//		elem->Attach(getGrandParent);
						//	}
						//}
						////selected_GameObJeCt->Detach();				//detach regardless 
						//selected_GameObJeCt->Attach(gameobject_);	//attach to the new parent			
					}
					selected_GameObJeCt = nullptr;
				}
				//same parent as inital, do nothing
				else 
				{
				}
			}
		}
		ImGui::EndDragDropTarget();
	}

	//else {
	//	if (selected_GameObJeCt && KeyRelease(MOUSE_LEFT)) 
	//	{
	//		if (!ImGui::IsAnyItemHovered()) {
	//			selected_GameObJeCt->Detach();
	//			selected_GameObJeCt = nullptr;
	//		}
	//	}
	//}
}

void Show_GameObject_Child(GameObject* obj_) 
{
	std::string nameConcat;
	nameConcat += obj_->name + "##" + std::to_string(obj_->GetID());

	if (obj_->HasChild()) {
		ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow 
			| (listOfselectionBool[obj_->GetID()] ? ImGuiTreeNodeFlags_Selected : 0) 
			| (toggle_inspector_collapse_all ? ImGuiTreeNodeFlags_DefaultOpen : 0)
			;
		bool istreeOpen = ImGui::TreeNodeEx(nameConcat.c_str(), node_flags);
		if (ImGui::IsItemClicked())
		{
			SelectObj(obj_);
		}

		CheckDragAndDrop(obj_);
		if(istreeOpen)
		{
			auto& getListOfChildren = obj_->GetAllChildren();
			for (auto& elem : getListOfChildren)
			{
				Show_GameObject_Child(elem);
			}
			ImGui::TreePop();
		}
		
		
	}
	else {
		if (ImGui::Selectable(nameConcat.c_str(), &listOfselectionBool[obj_->GetID()])) 
		{
			SelectObj(obj_);
		}
		CheckDragAndDrop(obj_);
	}
}


void MB_Window_Inspector() {

	ImGui::SetWindowPos(ImVec2{ IMGU.w * 0.8f, editor_menubar_offsety }, ImGuiCond_Once);
	ImGui::SetWindowSize(ImVec2{ 200.0f, IMGU.h - editor_menubar_offsety }, ImGuiCond_Once);

	ImGui::Begin(toggle_name[WB_INSPECTOR], &toggle_WB[WB_INSPECTOR], ImVec2{}, -1.0f, ImGuiWindowFlags_AlwaysUseWindowPadding);

	ImGui::Checkbox("Collapse Al", &toggle_inspector_collapse_all);
	

	if (selectedObj.gameObject && KeyPressed(KEY_G))
	{
		selectedObj.gameObject->Detach();
	}

	//selection

	for (auto& elem : RM::R_GameObject) {
		ImGui::PushID(static_cast<int>(elem->GetID()));
		if (!elem->GetParent()) {
			Show_GameObject_Child(elem);
		}

		//if (KeyPressed(MOUSE_LEFT) && KeyDown(KEY_CTRL))
		//{
		//
		//}
		//else if (KeyPressed(MOUSE_LEFT)) {
		//	
		//}
		ImGui::PopID();
	}

	//selection
	

	ImGui::End();
}

