#include "stdafx.h"
#include "EditorMB.h"
#include "Engine.h"
#include "Simulation.h"
#include "GenerateCurve.h"


bool playIK = false;
bool keepUpdatingplayIK = false;

float timeElapsed = 0.0f;
static float maxTime = 1.0f;

zg::Vector3 movingposition{ 6.0f, 0.0f, 7.0f };
zg::Vector3 finalPosition{ 6.0f, 0.0f, 7.0f };

void IKUpdateCF();
void IKUpdateJacobian();
void IKUpdateCCD();

static GameObject* modelHeavy = nullptr;
static std::vector<PickingCircle*> listofnode;
std::vector<zg::Vector3> listOfClampRotMax;
std::vector<zg::Vector3> listOfClampRotMin;

std::vector<zg::Quaternion> initRot;
std::vector<zg::Quaternion> currentRot;
std::vector<zg::Quaternion> finalRot;


enum _TYPEOFALGO {
	_CF = 0,
	_JACO,
	_CCD
};
_TYPEOFALGO enum_typeofalgo = _CCD;

//std::vector<GameObject*> IKNodesCF;
//std::vector<GameObject*> IKNodesJaco;
//std::vector<GameObject*> IKNodesCCD;

std::map<_TYPEOFALGO, std::vector<GameObject*>> IKNodes;
std::map<_TYPEOFALGO, void(*)()> IKUpdate;

//curves staff
static std::vector<zg::Vector3> ListOfPoint;
static zg::BezierCubic BezierCubic{};

static float curve_timeElasped = 0.0f;
static float curvingSpeed = 0.5f;
static float animationPace = 0.493f;
GameObject* heavyyy = nullptr;

void ClampRot(zg::Quaternion& source_, const zg::Vector3& min_, const zg::Vector3& max_)
{
	zg::Vector3 eular = source_.ToEularDegree();
	zg::Clamp(&eular.x, min_.x, max_.x);
	zg::Clamp(&eular.y, min_.y, max_.y);
	zg::Clamp(&eular.z, min_.z, max_.z);
	source_.SetFromEulerAngles(eular);
}

void Init_Simulation2()
{
	//simulation2
	MainCam->owner->transform.SetPosition(0.0f, 20.38f, 27.0f);
	MainCam->owner->transform.SetRotation(-40.0f, 0.0f, 0.0f);

	listofnode.clear();
	IKNodes[_CF]		= std::vector<GameObject*>{};
	IKNodes[_JACO]		= std::vector<GameObject*>{};
	IKNodes[_CCD]		= std::vector<GameObject*>{};

	IKUpdate[_CF]		= IKUpdateCF;
	IKUpdate[_JACO]		= IKUpdateJacobian;
	IKUpdate[_CCD]		= IKUpdateCCD;

	float zoffset = 1.0f;

	for (int i = 0; i < 8; ++i) 
	{
		PickingCircle* picker1 = nullptr;
		picker1 = CreateGameObject(2, "Pyramid" + std::to_string(i));
		//picker1->owner->transform.SetPosition(0.0, 9.0f - (yoffset * i), 0.0f);
		picker1->owner->transform.SetPosition(0.0, 0.0f, (zoffset * i));

		listofnode.push_back(picker1);

		listOfClampRotMax.push_back(zg::Vector3{});
		listOfClampRotMin.push_back(zg::Vector3{});
		initRot.push_back(zg::Quaternion{});

		listOfClampRotMax[i] = { 40, 20, 25 };
		listOfClampRotMin[i] = { -40, -20, -25 };
	}

	SelectionUpdate_UponGameObjectAddAndDeleted(listofnode[3]);

	listofnode[1]->owner->Attach(listofnode[0]->owner);
	listofnode[2]->owner->Attach(listofnode[1]->owner);
	listofnode[3]->owner->Attach(listofnode[2]->owner);
	listofnode[4]->owner->Attach(listofnode[3]->owner);
	listofnode[5]->owner->Attach(listofnode[4]->owner);
	listofnode[6]->owner->Attach(listofnode[5]->owner);
	listofnode[7]->owner->Attach(listofnode[6]->owner);

	//Close Form
	IKNodes[_CF].push_back(listofnode[5]->owner);
	IKNodes[_CF].push_back(listofnode[6]->owner);
	IKNodes[_CF].push_back(listofnode[7]->owner);

	IKNodes[_JACO].push_back(listofnode[1]->owner);
	IKNodes[_JACO].push_back(listofnode[2]->owner);
	IKNodes[_JACO].push_back(listofnode[3]->owner);
	IKNodes[_JACO].push_back(listofnode[4]->owner);
	IKNodes[_JACO].push_back(listofnode[5]->owner);
	IKNodes[_JACO].push_back(listofnode[6]->owner);
	IKNodes[_JACO].push_back(listofnode[7]->owner);


	//CCD
	IKNodes[_CCD].push_back(listofnode[7]->owner);
	IKNodes[_CCD].push_back(listofnode[6]->owner);
	IKNodes[_CCD].push_back(listofnode[5]->owner);
	IKNodes[_CCD].push_back(listofnode[4]->owner);
	IKNodes[_CCD].push_back(listofnode[3]->owner);
	IKNodes[_CCD].push_back(listofnode[2]->owner);
	IKNodes[_CCD].push_back(listofnode[1]->owner);

	
	PickingCircle* picker1 = nullptr;
	picker1 = CreateGameObject(1, "Cube");
	picker1->owner->transform.SetPosition(0.0f, -1.0f, 0.0f);
	picker1->owner->transform.SetScale(20.0f, 0.2f, 20.0f);

	PickingCircle* picker = nullptr;
	picker = CreateGameObject(3, "Heayy" + std::to_string(RM::R_GameObject.size()));
	SelectionUpdate_UponGameObjectAddAndDeleted(picker);
	heavyyy = picker->owner;

	listofnode[0]->owner->transform.Rotate(-89.0f);
}

void Update_Simulation2()
{
	const float speed = 0.07f;
	const float speedR = 1.0f;
	{
		if (KeyDown(KEY_W))	movingposition.z -= speed;
		if (KeyDown(KEY_S))	movingposition.z += speed;
		if (KeyDown(KEY_A))	movingposition.x -= speed;
		if (KeyDown(KEY_D))	movingposition.x += speed;
		if (KeyDown(KEY_E))	movingposition.y -= speed;
		if (KeyDown(KEY_Q))	movingposition.y += speed;

		if (KeyDown(KEY_UP))	listofnode[0]->owner->transform.Rotate(-speedR);
		if (KeyDown(KEY_DOWN))	listofnode[0]->owner->transform.Rotate(speedR);
		if (KeyDown(KEY_LEFT))	listofnode[0]->owner->transform.Rotate(0.0f, -speedR);
		if (KeyDown(KEY_RIGHT)) listofnode[0]->owner->transform.Rotate(0.0f, speedR);

		
		//Update - pre compute IK Handler
		if (KeyRelease(KEY_SPACE))
		{
			finalPosition = movingposition;
			playIK = true;

			//generate Curve
			ListOfPoint.clear();
			ListOfPoint.push_back(heavyyy->transform.position);
			ListOfPoint.push_back(movingposition);
			BezierCubic.GeneratePathPointOnCurve(ListOfPoint, 2.0f);
			BezierCubic.GenerateArcLengthTable();
			//BezierCubic.GenerateGizmoTable();
			curve_timeElasped = 0.0f;

			//Draw and Update IK Handler
			size_t sz = IKNodes[enum_typeofalgo].size();
			if (sz > 1)
				IKUpdate[enum_typeofalgo]();
		}
	}

	//Draw IK Handler
	Gizmo::Sphere(movingposition, 0.1f, CC_ALICEBLUE);
	Gizmo::Sphere(finalPosition, 0.2f, CC_RED);

	switch (enum_typeofalgo)
	{
		case _CF:
		{
			size_t sz = IKNodes[_CF].size();
			if (sz > 1)
			{
				Gizmo::Line(IKNodes[_CF][0]->transform.position, IKNodes[_CF][sz - 1]->transform.position);
			}
		}
		break;
		case _JACO:
		{
			size_t sz = IKNodes[_JACO].size();
			if (sz > 1)
			{
				Gizmo::Line(IKNodes[_JACO][0]->transform.position, IKNodes[_JACO][sz - 1]->transform.position);
			}
		}
		break;
		case _CCD:
		{
			size_t sz = IKNodes[_CCD].size();
			if (sz)
			{
				Gizmo::Ray(IKNodes[_CCD][0]->transform.position, IKNodes[_CCD][0]->transform.Forward(), 2.0f, 0.1f, CC_BLUE);
				for (size_t i = 1; i < sz; ++i)
				{
					zg::Vector3 dir = (IKNodes[_CCD][i - 1]->transform.position - IKNodes[_CCD][i]->transform.position).Normalized();
					Gizmo::Ray(IKNodes[_CCD][i]->transform.position, dir, 2.0f, 0.1f);
					Gizmo::Ray(IKNodes[_CCD][i]->transform.position, IKNodes[_CCD][i]->transform.Forward(), 2.0f, 0.1f, CC_BLUE);
				}
				Gizmo::Line(IKNodes[_CCD][0]->transform.position, finalPosition);
			}
		}
		break;
	}

	if (keepUpdatingplayIK)
	{
		finalPosition = movingposition;
		//listofnode[0]->owner->transform.SetPosition(heavyyy->transform.position);
		size_t sz = IKNodes[enum_typeofalgo].size();
		if (sz > 1)
			IKUpdate[enum_typeofalgo]();
	}

	//play Slerp Animation
	if (playIK) 
	{
		//generate Curves
		{
			curve_timeElasped += FrameRateController::invFrame * curvingSpeed;
			if (curve_timeElasped > maxTime)
				curve_timeElasped = maxTime - 0.01f;

			//get normalized time / normalized value
			float getS = zg::GetSineInterpolation(curve_timeElasped / maxTime, 0.2f, 0.8f);
			if (getS >= 1.0f)
				getS = 0.99f;

			//get the real timeElasped from arc length table
			float getU = 0.0f;
			getU = BezierCubic.GetTimeUFromArcLengthTable(getS);
			if (getU >= maxTime)
				getU = 0.99f;

			zg::Vector3 pointOnCurve;
			zg::Vector3 velocOnCurve;

			//get the position on curve based on time
			pointOnCurve = BezierCubic.GetPointOnCurve(getU, &velocOnCurve);
			//Set the model's position
			heavyyy->transform.SetPosition(pointOnCurve);

			//animation Speed
			if (selectedObj.meshRenderer)
			{
				//Play walk animation
				if (!selectedObj.meshRenderer->IsCurrentAnim("walk"))
					selectedObj.meshRenderer->SetAnimation("walk");

				//Set animation Speed
				selectedObj.meshRenderer->SetAnimationSpeed(std::abs(velocOnCurve.Length() / animationPace * curvingSpeed));

				//rotation
				if (velocOnCurve.IsZero())
					velocOnCurve = ListOfPoint[1] - ListOfPoint[0];

				//Toggle between center of interest
				heavyyy->transform.LookAt(pointOnCurve + velocOnCurve);

				Gizmo::Ray(pointOnCurve, velocOnCurve.Normalized(), 2.0f, 0.1f);
			}
			BezierCubic.DrawCurve();
		}

		//slerp all rotation
		timeElapsed += FrameRateController::dttimescale * curvingSpeed;
		if (timeElapsed > maxTime) // || (listofnode[7]->owner->transform.position - finalPosition).Length() < 1.0f)
		{
			playIK = false;
			timeElapsed = 0.0f;
		}
		else
		{
			size_t sz = IKNodes[enum_typeofalgo].size();
			switch (enum_typeofalgo)
			{
				case _CF:
				{
					
					for (size_t i = 1; i < sz; ++i)
					{
						zg::Quaternion curr = zg::Slerp(currentRot[i - 1], finalRot[i - 1], timeElapsed);
						IKNodes[_CF][i]->transform.SetLocalRotation(curr);
					}
				}
				break;
				case _JACO:
				{
					if (sz > 1)
						IKUpdateJacobian();
				}
				break;
				case _CCD:
				{
					for (size_t i = 1; i < sz; ++i)
					{
						zg::Quaternion curr = zg::Slerp(currentRot[i - 1], finalRot[i - 1], timeElapsed);
						IKNodes[_CCD][i]->transform.SetLocalRotation(curr);
					}
				}
				break;
			}
			
		}
	}
}

void UpdateImgui_Simulation2()
{
	ImGui::Separator();
	ImGui::Text("W A S D E Q Key - to Move End point");
	ImGui::Text("Up Arrow, Down Arrow Key - Rotate Snake ");
	ImGui::Text("Left Arrow, Right Arrow Key - Rotate Snake ");
	ImGui::Text("SpaceBar - to Set End point");
	

	ImGui::DragFloat3("WHITE_movingposition", &movingposition.x, 0.01f);
	ImGui::DragFloat3("RED_finalPosition", &finalPosition.x, 0.01f);

	//ImGui::Checkbox("playIK", &playIK);
	ImGui::Checkbox("keepUpdatingIK", &keepUpdatingplayIK);

	// typeof curve
	//if (ImGui::RadioButton("Close form (broken)", enum_typeofalgo == _CF))
	//{
	//	enum_typeofalgo = _CF;
	//}
	//if (ImGui::RadioButton("Jacobian (broken)", enum_typeofalgo == _JACO)) 
	//{
	//	enum_typeofalgo = _JACO;
	//}
	if (ImGui::RadioButton("Cyclic Coordinate Descent", enum_typeofalgo == _CCD)) 
	{
		enum_typeofalgo = _CCD;
	}

	//CF
	//{
	//	if (ImGui::Button("CF_Clear"))
	//	{
	//		IKNodes[_CF].clear();
	//		if (selectedObj.gameObject)
	//		{
	//			if (selectedObj.meshRenderer)
	//				selectedObj.meshRenderer->Stop();
	//		}
	//	}
	//	if (selectedObj.gameObject)
	//	{
	//		if (IKNodes[_CF].size() < 4) 
	//		{
	//			if (ImGui::Button("Add Node, Using CF"))
	//			{
	//				if (!IKNodes[_CF].size())
	//					finalPosition = selectedObj.gameObject->transform.position;
	//
	//				IKNodes[_CF].push_back(selectedObj.gameObject);
	//			}
	//		}
	//		for (auto& elem : IKNodes[_CF])
	//		{
	//			ImGui::Text(elem->name.c_str());
	//			ImGui::DragFloat3("position", &elem->transform.position.x);
	//			ImGui::DragFloat3("localPosition", &elem->transform.localPosition.x);
	//		}
	//	}
	//}

	//Jacobian
	//{
	//	if (ImGui::Button("Jacobian_Clear"))
	//	{
	//		IKNodes[_CCD].clear();
	//		if (selectedObj.gameObject)
	//		{
	//			if (selectedObj.meshRenderer)
	//				selectedObj.meshRenderer->Stop();
	//		}
	//	}
	//	if (selectedObj.gameObject)
	//	{
	//		if (ImGui::Button("Add Node, Using Jacobian"))
	//		{
	//			if (!IKNodes[_CCD].size())
	//				finalPosition = selectedObj.gameObject->transform.position;
	//
	//			IKNodes[_CCD].push_back(selectedObj.gameObject);
	//		}
	//		for (auto& elem : IKNodes[_CCD])
	//		{
	//			ImGui::Text(elem->name.c_str());
	//			ImGui::DragFloat3("position", &elem->transform.position.x);
	//			ImGui::DragFloat3("localPosition", &elem->transform.localPosition.x);
	//		}
	//	}
	//}

	//CCD
	{

		if (ImGui::Button("Reset Node"))
		{
			keepUpdatingplayIK = false;
			size_t sz = listofnode.size();
			listofnode[0]->owner->transform.SetPosition(0.0f, 0.0f, 0.0f);
			listofnode[0]->owner->transform.SetRotation(zg::Quaternion{});
			listofnode[0]->owner->transform.Rotate(-89.0f);
			for (size_t i = 1; i < sz; ++i)
			{
				listofnode[i]->owner->transform.SetLocalPosition(0.0f, 0.0f, 1.0f);
				listofnode[i]->owner->transform.SetLocalRotation(zg::Quaternion{});
			}
		}
		//if (selectedObj.gameObject)
		//{
		//	for (auto& elem : IKNodes[_CCD])
		//	{
		//		ImGui::Text(elem->name.c_str());
		//		ImGui::DragFloat3("position", &elem->transform.position.x);
		//		ImGui::DragFloat3("localPosition", &elem->transform.localPosition.x);
		//	}
		//}
	}
}


void IKUpdateCF() 
{
	size_t sz = IKNodes[_CF].size();
	if (sz > 2) 
	{
		//Get current local rotation To Slerp later
		currentRot.clear();
		finalRot.clear();
		for (size_t i = 1; i < sz; ++i)
			currentRot.push_back(IKNodes[_CF][i]->transform.localRotation);

		for (size_t i = 0; i < sz; ++i)
		{
			if (IKNodes[_CF][i]->transform.localRotation.IsNan())
				IKNodes[_CF][i]->transform.localRotation.Reset();
		
			if (IKNodes[_CF][i]->transform.rotation.IsNan())
				IKNodes[_CF][i]->transform.rotation.Reset();
		
			if (IKNodes[_CF][i]->transform.position.IsNan())
				IKNodes[_CF][i]->transform.position.Reset();
		
			if (IKNodes[_CF][i]->transform.localPosition.IsNan())
				IKNodes[_CF][i]->transform.localPosition.Reset();
		
		}

		float L1 = (IKNodes[_CF][1]->transform.position - IKNodes[_CF][0]->transform.position).Length();
		float L2 = (IKNodes[_CF][2]->transform.position - IKNodes[_CF][1]->transform.position).Length();

		float x = finalPosition.x;
		float y = finalPosition.y;
		//float x = IKNodes[_CF][0]->transform.position.x;
		//float y = IKNodes[_CF][0]->transform.position.y;

		float alpha2 = std::acos((x * x + y * y - L1 * L1 - L2 * L2) / (2.0f * L1 * L2));
		float alpha1 = std::asin((y * (L1 + L2 * cos(alpha2)) - x * L2 * sin(alpha2)) / (x * x + y * y));

		zg::Quaternion tmp1{ zg::Vector3::Vector3Y, TO_DEG(alpha1) };
		zg::Quaternion tmp2{ zg::Vector3::Vector3Y, TO_DEG(alpha2) };

		//IKNodes[_CF][1]->transform.SetRotation(tmp2);

		//Rotate to the object 
		IKNodes[_CF][1]->transform.Rotate(tmp1);
		//Update local coordinates
		IKNodes[_CF][1]->transform.ResetFlag();
		//Clamp using constrasant
		ClampRot(IKNodes[_CF][1]->transform.localRotation, listOfClampRotMin[1], listOfClampRotMax[1]);
		//Update local coordinates once more
		IKNodes[_CF][1]->transform.SetLocalRotation(IKNodes[_CF][1]->transform.localRotation);
		//Update End Effector world's position
		IKNodes[_CF][0]->transform.ForceUpdate();

		//GameObject* tmp = IKNodes[_CF][2]->GetParent();
		//IKNodes[_CF][2]->Detach();
		//IKNodes[_CF][2]->transform.SetRotation(tmp1);
		//IKNodes[_CF][2]->Attach(tmp);

		//Rotate to the object 
		IKNodes[_CF][2]->transform.Rotate(tmp2);
		//Update local coordinates
		IKNodes[_CF][2]->transform.ResetFlag();
		//Clamp using constrasant
		ClampRot(IKNodes[_CF][1]->transform.localRotation, listOfClampRotMin[2], listOfClampRotMax[2]);
		//Update local coordinates once more
		IKNodes[_CF][2]->transform.SetLocalRotation(IKNodes[_CF][2]->transform.localRotation);
		//Update End Effector world's position
		IKNodes[_CF][0]->transform.ForceUpdate();

		for (size_t i = 0; i < sz; ++i)
		{
			if (IKNodes[_CF][i]->transform.localRotation.IsNan())
				IKNodes[_CF][i]->transform.localRotation.Reset();
		
			if (IKNodes[_CF][i]->transform.rotation.IsNan())
				IKNodes[_CF][i]->transform.rotation.Reset();
		
			if (IKNodes[_CF][i]->transform.position.IsNan())
				IKNodes[_CF][i]->transform.position.Reset();
		
			if (IKNodes[_CF][i]->transform.localPosition.IsNan())
				IKNodes[_CF][i]->transform.localPosition.Reset();
		}

		//Get final local rotation TO Slerp later
		for (size_t i = 1; i < sz; ++i)
			finalRot.push_back(IKNodes[_CF][i]->transform.localRotation);
	}
}

void IKUpdateJacobian() 
{
	int convergenceframe = 1;

	size_t sz = IKNodes[_JACO].size();
	zg::Vector3 curPosition = IKNodes[_JACO][sz - 1]->transform.position;
	zg::Vector3 deltaVector = (finalPosition - curPosition) / (float)convergenceframe;
	zg::MatrixMN deltaMat{ zg::Vector2{ deltaVector.x, deltaVector.z}, false};		//2x1 matrix

	std::vector<float> listOfalpha;

	float sumOfAlpha = 0.0f;
	for (size_t j = 0; j < sz - 1; ++j)
	{
		sumOfAlpha += IKNodes[_JACO][j]->transform.rotation.ToEularDegree().y;
		listOfalpha.push_back(IKNodes[_JACO][j]->transform.rotation.ToEularDegree().y);
	}
	zg::MatrixMN alpha{ listOfalpha, false };

	while (!(curPosition == finalPosition) && convergenceframe > 0)
	{
		//recompute jacobian
		std::vector<std::vector<float>> matrix;
		//3 rows x y z
		for (unsigned i = 0; i < 2; ++i)
		{
			matrix.push_back(std::vector<float>{});
			//numberOfLinks
			for (size_t j = 0; j < sz - 1; ++j)
			{
				matrix[i].push_back(0.0f);
			}
		}

		std::vector<float> L;
		//float L2 = (IKNodes[_JACO][2]->transform.position - IKNodes[_JACO][1]->transform.position).Length();
		//float L1 = (IKNodes[_JACO][1]->transform.position - IKNodes[_JACO][0]->transform.position).Length();
		for (int i = 1; i < sz; ++i)
		{
			L.push_back((IKNodes[_JACO][i]->transform.position - IKNodes[_JACO][i - 1]->transform.position).Length());
		}

		//for (unsigned i = 0; i < 2; ++i)
		{
			float getsumOfAlpha = sumOfAlpha;
			for (int j = (int)sz - 2; j > -1; --j)
			{
				if (j == sz - 2)
					matrix[0][j] = -L[j] * sin(getsumOfAlpha);
				else
					matrix[0][j] = -L[j] * sin(getsumOfAlpha) - matrix[0][j + 1];
				
				getsumOfAlpha -= listOfalpha[j];
			}
			getsumOfAlpha = sumOfAlpha;
			for (int j = (int)sz - 2; j > -1; --j)
			{
				if (j == sz - 2)
					matrix[1][j] = L[j] * cos(getsumOfAlpha);
				else
					matrix[1][j] = L[j] * cos(getsumOfAlpha) + matrix[1][j + 1];
				
				getsumOfAlpha -= listOfalpha[j];
			}
		}

		//matrix[0].push_back(-L[0] * sin(listOfalpha[0][1]) - L[1] * sin(listOfalpha[0][1] + listOfalpha[0][0]));
		//matrix[0].push_back(-L[1] * sin(listOfalpha[0][1] + listOfalpha[0][0]));
		//matrix[1].push_back( L[0] * cos(listOfalpha[0][1]) - L[1] * cos(listOfalpha[0][1] + listOfalpha[0][0]));
		//matrix[1].push_back( L[1] * cos(listOfalpha[0][1] + listOfalpha[0][0]));

		zg::MatrixMN jacobianM{ matrix }; //2 x k matrix
		//(k x 2 * ( 2 x k * k x 2)) * 2 x 1 = k x 1
		zg::MatrixMN deltaAlpha = (jacobianM.Transposed() * zg::GaussJordanInverse((jacobianM * jacobianM.Transposed()))) * deltaMat;

		for (size_t i = 0; i < sz - 1; ++i)
		{
			alpha.matrix[i][0] += deltaAlpha.matrix[i][0] / FrameRateController::dt;
		}

		for (size_t i = 0; i < sz - 1; ++i)
		{
			zg::Quaternion toQ{ zg::Vector3::Vector3Y, alpha.matrix[i][0] };

			//Rotate to the object 
			IKNodes[_JACO][i]->transform.Rotate(toQ);
			//Update local coordinates
			IKNodes[_JACO][i]->transform.ResetFlag();
			//Clamp using constrasant
			ClampRot(IKNodes[_JACO][i]->transform.localRotation, listOfClampRotMin[i], listOfClampRotMax[i]);
			//Update local coordinates once more
			IKNodes[_JACO][i]->transform.SetLocalRotation(IKNodes[_JACO][i]->transform.localRotation);
			//Update End Effector world's position
			
		}
		//IKNodes[_JACO][0]->transform.ForceUpdate();
		curPosition = IKNodes[_JACO][0]->transform.position;
		--convergenceframe; 
	}
}

void IKUpdateCCD() 
{
	zg::Vector3 curPosition = IKNodes[_CCD][0]->transform.position;
	zg::Vector3 pevPosition = curPosition;
	const float finalToCurrentEpi = 0.001f;
	float prevToCurrentEpi = 0.001f;

	//Get current local rotation To Slerp later
	size_t sz = IKNodes[_CCD].size();
	currentRot.clear();
	finalRot.clear();
	for (size_t i = 1; i < sz; ++i)
		currentRot.push_back(IKNodes[_CCD][i]->transform.localRotation);

	//Loop
	do
	{
		//get prev position of end factor
		pevPosition = curPosition;
		for (size_t i = 1; i < sz; ++i) 
		{
			//Compute Angle in world coordinates
			zg::Vector3 elemPos = IKNodes[_CCD][i]->transform.position;
			zg::Vector3 Vck		= curPosition  - elemPos;
			zg::Vector3 Vdk		= finalPosition - elemPos;
	
			Vck.Normalize();
			Vdk.Normalize();

			//Get axis and angle and convert to quaternion
			float alphaK	 = TO_DEG(std::acos(Vck.Dot(Vdk)));
			alphaK			 = ISNAN(alphaK) ? 0.0f : alphaK;
			zg::Vector3 axis = Vck.Cross(Vdk).Normalized();
			zg::Quaternion toQ{ axis, alphaK };

			//Rotate to the object 
			IKNodes[_CCD][i]->transform.Rotate(toQ);
			//Update local coordinates
			IKNodes[_CCD][i]->transform.ResetFlag();
			//Clamp using constrasant
			ClampRot(IKNodes[_CCD][i]->transform.localRotation, listOfClampRotMin[i], listOfClampRotMax[i]);
			//Update local coordinates once more
			IKNodes[_CCD][i]->transform.SetLocalRotation(IKNodes[_CCD][i]->transform.localRotation);
			//Update End Effector world's position
			IKNodes[_CCD][0]->transform.ForceUpdate();
	
			//Check for early exits
			curPosition = IKNodes[_CCD][0]->transform.position;
			if (std::abs((finalPosition - curPosition).Length()) < finalToCurrentEpi)
				goto a;
		}
		prevToCurrentEpi += 0.01f;
		//std::cout << (pevPosition - curPosition).Length() << std::endl;
	} while ((pevPosition - curPosition).Length() > prevToCurrentEpi);

a:
	//prevToCurrentEpi = (pevPosition - curPosition).Length();

	//Get final local rotation T0 Slerp later
	for (size_t i = 1; i < sz; ++i)
		finalRot.push_back(IKNodes[_CCD][i]->transform.localRotation);
}
