
#include "stdafx.h"

#include "EditorMain.h"
#include "EditorMB.h"
#include "Engine.h"

void EditorInputUpdate()
{
	//Camera movement
	const int hovered = ImGuiHoveredFlags_AnyWindow | ImGuiHoveredFlags_RootAndChildWindows;
	const float cameraSpeed = 0.5f;
	const float scalefactor = 0.0005f;

	if (!ImGui::IsAnyWindowHovered()) {
		switch (MainCam->GetType()) {
		case CAM_PERSPECTIVE:
			if (!ImGui::IsAnyItemHovered())
			{
				//&& !Editor_Gizmo.imgui_selected_and_move

				//if (KeyDown(MOUSE_RIGHT) && (!ImGuizmo::IsOver() || !ImGuizmo::IsUsing()) && !ImGui::IsWindowHovered(hovered))
				if (KeyDown(MOUSE_RIGHT) && !ImGui::IsWindowHovered(hovered))
				{
					MainCam->owner->transform.Rotate(GetMouseDelta().y * 0.25f, GetMouseDelta().x * 0.25f);

					//float cameraEularX = MainCam->owner->transform.rotation.ToEularDegree().x;
					//if (cameraEularX > 89.8f)
					//{
					//	//MainCam->owner->transform.Rotate(0.0f, GetMouseDelta().x * 0.25f);
					//	MainCam->owner->transform.SetRotation(89.0f);
					//}
					//else if (cameraEularX < -89.8f)
					//{
					//	//MainCam->owner->transform.Rotate(0.0f, GetMouseDelta().x * 0.25f);
					//	MainCam->owner->transform.SetRotation(-89.0f);
					//}
					
					//MainCam->owner->transform.Rotate((float)GetMouseDelta().y * 0.25f, (float)GetMouseDelta().x * 0.25f, 0.0f);
					//if (MainCam->GetTransform().rotation.z > 89.0f)
					//	MainCam->SetRotation(89.0f);
					//else if (MainCam->GetTransform().rotation.z < -89.0f)
					//	MainCam->SetRotation(-89.0f);
				}

				//if (KeyRelease(MOUSE_RIGHT))
				//{
				//	if (MainCam->owner->transform.rotation.x > 89.9f)
				//		MainCam->SetRotation(
				//			90.0f - (MainCam->owner->transform.rotation.x - 90.0f)
				//			, MainCam->owner->transform.rotation.y - 180.0f
				//		);
				//	else if (MainCam->owner->transform.rotation.x < -89.9f)
				//			MainCam->SetRotation(
				//				90.0f - (MainCam->owner->transform.rotation.x + 90.0f)
				//				, MainCam->owner->transform.rotation.y + 180.0f
				//			);
				//		
				//	//MainCam->RotateClamp((float)GetMouseDelta().y * 0.25f, (float)GetMouseDelta().x * 0.25f, 0.0f);
				//
				//	//if (MainCam->GetTransform().rotation.z > 89.0f)
				//	//	MainCam->SetRotation(89.0f);
				//	//else if (MainCam->GetTransform().rotation.z < -89.0f)
				//	//	MainCam->SetRotation(-89.0f);
				//}

				if (KeyDown(MOUSE_MIDDLE)) {
					if (GetMouseDelta().x > 0.0f) {
						MainCam->owner->transform.Translate(-MainCam->GetRight() * cameraSpeed);
					}
					else if (GetMouseDelta().x < 0.0f) {
						MainCam->owner->transform.Translate(MainCam->GetRight() * cameraSpeed);
					}
					if (GetMouseDelta().y > 0.0f) {
						MainCam->owner->transform.Translate(MainCam->GetUp() * cameraSpeed);
					}
					else if (GetMouseDelta().y < 0.0f) {
						MainCam->owner->transform.Translate(-MainCam->GetUp()  * cameraSpeed);
					}

				}
			}
			if (KeyDown(MOUSE_RIGHT) && KeyDown(KEY_W))
				MainCam->owner->transform.Translate(cameraSpeed * MainCam->GetForward());
			if (KeyDown(MOUSE_RIGHT) && KeyDown(KEY_S) && KeyUp(KEY_LCTRL) && KeyUp(KEY_RCTRL))
				MainCam->owner->transform.Translate(-cameraSpeed * MainCam->GetForward());
			if (KeyDown(MOUSE_RIGHT) && KeyDown(KEY_A) && KeyUp(KEY_LCTRL) && KeyUp(KEY_RCTRL))
				MainCam->owner->transform.Translate(-MainCam->GetRight() * cameraSpeed);
			if (KeyDown(MOUSE_RIGHT) && KeyDown(KEY_D) && KeyUp(KEY_LCTRL) && KeyUp(KEY_RCTRL))
				MainCam->owner->transform.Translate(MainCam->GetRight() * cameraSpeed);
			if (KeyDown(MOUSE_RIGHT) && KeyDown(KEY_Q))
				MainCam->owner->transform.Translate(-zg::Vector3::Vector3Y * cameraSpeed);
			if (KeyDown(MOUSE_RIGHT) && KeyDown(KEY_E))
				MainCam->owner->transform.Translate(zg::Vector3::Vector3Y * cameraSpeed);
			break;

		case CAM_ORTHO:
			if (KeyDown(VK_MBUTTON) && !ImGui::IsMouseHoveringAnyWindow()) {
				if (GetMouseDelta().x) {
					MainCam->owner->transform.Translate(-GetMouseDelta().x, GetMouseDelta().y, 0);
					//MainCam->transform.translation.x -= GetMouseDelta().x;
					//MainCam->transform.translation.y += GetMouseDelta().y;
					//MainCam->SetPosition(MainCam->transform.translation.x, MainCam->transform.translation.y, MainCam->transform.translation.z);
				}
			}
			break;
		}
	}

	//UnSelection
	if (KeyRelease(MOUSE_RIGHT) && !ImGui::IsAnyItemHovered() && !ImGui::IsAnyWindowHovered())
	{
		//selectedObj.SetEmpty();
	}

}//End EditorInputUpdate
