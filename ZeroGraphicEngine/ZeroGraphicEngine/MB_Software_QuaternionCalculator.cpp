#include "stdafx.h"
#include "EditorMB.h"

//bool toggle_window = true;
static int item_current = 0;
const char* format = "%.7f";
const char* items[] = { "zg::Quaternion", "Matrix" };

enum USING_WHAT_DEGREE {
	__DEGREE = 0,
	__RADIAN
};
USING_WHAT_DEGREE usingWhatDegree = __DEGREE;
zg::Quaternion input_q1{};
zg::Vector3 AxisAngle_vec{};
float AxisAngle_angle = 0.0f;

zg::Vector3 input_eularAngle{};

//output
zg::Quaternion out_q1{};
zg::Vector3 out_eularAngle{};
zg::Matrix44 out_matrix44{};

void BuildQuaternionCalculator()
{
	ImGui::Begin(toggle_name[WB_QUATERNION], &toggle_WB[WB_QUATERNION], ImVec2{ 500.0f, 500.0f }, -1.0f, ImGuiWindowFlags_AlwaysUseWindowPadding);
	ImGui::Combo("combo", &item_current, items, IM_ARRAYSIZE(items));

	switch (item_current) {
	case 0: 
	{
		if (ImGui::RadioButton("Degree", usingWhatDegree == __DEGREE)) {
			usingWhatDegree = __DEGREE;
		}
		ImGui::SameLine();
		if (ImGui::RadioButton("Radian", usingWhatDegree == __RADIAN)) {
			usingWhatDegree = __RADIAN;
		}
		
		if (ImGui::DragFloat4("zg::Quaternion", &input_q1.x, 0.01f, -1.0f, 1.0f, format)) {
			out_eularAngle = zg::ToEularDegree(input_q1);
			out_q1 = zg::ToQuaternion(out_eularAngle);// out_q1 = zg::Quaternion::FromEulerAngles(out_eularAngle);
		}

		if (ImGui::DragFloat3("Euler Angle", &input_eularAngle.x, 0.1f, -1800.0f, 1800.0f, format)) {
			//std::cout << "IN" << input_eularAngle;
			switch (usingWhatDegree) {
			case __RADIAN:
				out_q1.SetFromEulerAngles(input_eularAngle.x, input_eularAngle.y, input_eularAngle.z);
				break;
			default:
				out_q1.SetFromEulerAngles(input_eularAngle);
				break;
			}
			out_eularAngle = zg::ToEularDegree(out_q1);
			out_matrix44 = out_q1.GetMatrix44();
		}
		//std::cout << out_q1 * out_q1.GetConjugated();
		//std::cout << out_q1 * out_q1.GetInverted() ;
		//std::cout <<  out_q1.GetConjugated()  * out_q1 ;
		//std::cout <<  out_q1.GetInverted() * out_q1 ;

		//zg::Quaternion qqq( -2, 3, 2, 11 / 11);
		//zg::Quaternion xxx = zg::Quaternion(-2, 0, -2, 11);
		//std::cout <<  qqq * xxx ;
		//std::cout <<  (qqq * xxx).Length();
		//std::cout <<  (qqq * xxx).Normalized();
		//std::cout <<  (qqq * xxx).GetInverted();
		//std::cout << out_eularAngle;
		
		ImGui::Text("Output:"); 

		ImGui::DragFloat4("out_Quaternion", &out_q1.x, 0.01f, -1.0f, 1.0f, format);
		ImGui::DragFloat3("out_Euler Angle", &out_eularAngle.x, 0.001f, -1800.0f, 1800.0f, format);
		
		ImGui::InputFloat4("out_Matrix1", &out_matrix44.m[0], 8);
		ImGui::InputFloat4("out_Matrix2", &out_matrix44.m[4], 8);
		ImGui::InputFloat4("out_Matrix3", &out_matrix44.m[8], 8);
		ImGui::InputFloat4("out_Matrix4", &out_matrix44.m[12], 8);


	}
		break;
	case 1:
	{

	}
		break;
	}

	ImGui::End();

}//End DrawEditor
