#include "stdafx.h"
#include "EditorMB.h"
#include "Selection.h"

void EditorMainMenuBar_Windows() {
	if (ImGui::BeginMenu("Windows")) {


		ImGui::MenuItem(toggle_name[WB_INSPECTOR], NULL, &toggle_WB[WB_INSPECTOR]);
		//if (ImGui::MenuItem(toggle_name[WB_INSPECTOR], NULL, &toggle_WB[WB_INSPECTOR]))
		//{
		//
		//}
		
		ImGui::MenuItem(toggle_name[WB_PROPERTIES], NULL, &toggle_WB[WB_PROPERTIES]);
		//if (ImGui::MenuItem(toggle_name[WB_PROPERTIES], NULL, &toggle_WB[WB_PROPERTIES]))
		//
		//{
		//	
		//}
		ImGui::EndMenu();
	}
}
