#include "stdafx.h"
#include "Engine.h"
#include "EditorMB.h"
#include "GenerateCurve.h"
#include "Simulation.h"

bool play_curve = true;
bool toggle_drawpath = true;
bool follow_model = false;

int use_cubicBezierTable = 2;

float easein = 0.1f;
float easeout = 0.6f;

static float curvingSpeed = 0.1f;
static float animationPace = 0.493f;

static float maxTime = 0.0f;
static float curve_timeElasped = 0.0f;
static std::vector<zg::Vector3> ListOfPoint;
zg::Vector3 centerofInterest;

bool toggle_centerOfInterest = false;

zg::Bezier_BernsteinPolynormial Bezier_BernsteinPolynormial{};
bool toggle_BPC_slotitascontrolpoits = false;

zg::Bezier_deCasteljau Bezier_deCasteljau{};
bool toggle_DCC_slotitascontrolpoits = false;

static zg::BezierCubic BezierCubic{};
float cusps = 2.0f;


enum _TYPEOFCURVE {
	_BernsteinPolynormialCurve = 0,
	_deCasteljauCurve,
	_BezierCubic,
};

_TYPEOFCURVE enum_typeofcurve = _BezierCubic;

void Init_Simulation1()
{
	MainCam->owner->transform.SetPosition(-8.136f, 4.39f, -1.684f);
	MainCam->owner->transform.SetRotation(-122.07f, -47.495f, 97.5168f);

	PickingCircle* picker = nullptr;
	picker = CreateGameObject(3, "Heayy" + std::to_string(RM::R_GameObject.size()));
	SelectionUpdate_UponGameObjectAddAndDeleted(picker);
	ListOfPoint.clear();
	ListOfPoint.push_back(zg::Vector3{ 0.0f, 0.0f, 0.0f });
	ListOfPoint.push_back(zg::Vector3{ 8.0f, 0.0f, 8.0f });
	ListOfPoint.push_back(zg::Vector3{ 15.0f, 0.0f, 15.0f });
	ListOfPoint.push_back(zg::Vector3{ 25.0f, 0.0f, 20.0f });
	ListOfPoint.push_back(zg::Vector3{ 25.0f, 0.0f, 10.0f });
	ListOfPoint.push_back(zg::Vector3{ 15.0f, 0.0f, 5.0f });
	ListOfPoint.push_back(zg::Vector3{ 10.0f, 0.0f, 0.0f });
	ListOfPoint.push_back(zg::Vector3{ 8.0f, 0.0f, -10.0f });


	for (auto& elem : ListOfPoint)
		centerofInterest += elem;

	centerofInterest /= static_cast<float>(ListOfPoint.size());

	maxTime = MAX2(0.0f, (float)ListOfPoint.size() - 1.0f);

	Bezier_BernsteinPolynormial.GeneratePathPointOnCurve(ListOfPoint, cusps);
	Bezier_deCasteljau.GeneratePathPointOnCurve(ListOfPoint, cusps);
	BezierCubic.GeneratePathPointOnCurve(ListOfPoint, cusps);
	BezierCubic.GenerateArcLengthTable();
	BezierCubic.GenerateFixedArcLengthTable();

	switch (enum_typeofcurve) {
	case _BernsteinPolynormialCurve:
		Bezier_BernsteinPolynormial.GenerateGizmoTable();
		break;
	case _deCasteljauCurve:
		Bezier_deCasteljau.GenerateGizmoTable();
		break;
	case _BezierCubic:
		BezierCubic.GenerateGizmoTable();
		break;
	}
}

void Update_Simulation1() 
{
	if (play_curve && selectedObj.gameObject)
	{
		curve_timeElasped += FrameRateController::invFrame * curvingSpeed;
		if (curve_timeElasped > maxTime)
			curve_timeElasped = 0.0f;

		switch (enum_typeofcurve) {
		case _BernsteinPolynormialCurve:
			selectedObj.gameObject->transform.SetPosition(Bezier_BernsteinPolynormial.GetPointOnCurve(curve_timeElasped));
			break;
		case _deCasteljauCurve:
			selectedObj.gameObject->transform.SetPosition(Bezier_deCasteljau.GetPointOnCurve(curve_timeElasped));
			break;
		case _BezierCubic:
		{
			//get normalized time / normalized value
			float getS = zg::GetSineInterpolation(curve_timeElasped / maxTime, easein, easeout);
			if (getS >= 1.0f)
				getS = 0.0f;

			//get the real timeElasped from arc length table
			float getU = 0.0f;
			switch (use_cubicBezierTable)
			{
			case 0:
				getU = BezierCubic.GetTimeU(getS);
				break;
			case 1:
				getU = BezierCubic.GetTimeUFromFixedTable(getS);
				break;
			case 2:
				getU = BezierCubic.GetTimeUFromArcLengthTable(getS);
				break;
			}
			if (getU >= maxTime)
				getU = 0.0f;

			zg::Vector3 pointOnCurve;
			zg::Vector3 velocOnCurve;

			//get the position on curve based on time
			pointOnCurve = BezierCubic.GetPointOnCurve(getU, &velocOnCurve);
			//Set the model's position
			selectedObj.gameObject->transform.SetPosition(pointOnCurve);

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
				if (toggle_centerOfInterest)
					selectedObj.gameObject->transform.LookAt(centerofInterest);
				else
					selectedObj.gameObject->transform.LookAt(pointOnCurve + velocOnCurve);

				Gizmo::Ray(pointOnCurve, velocOnCurve.Normalized());

				//Toggle Camera Following the model
				if (follow_model)
				{
					MainCam->owner->transform.CameraLookAt(pointOnCurve);
					MainCam->owner->transform.SetPosition(pointOnCurve);
					MainCam->owner->transform.Translate(selectedObj.gameObject->transform.Right() * 3.0f);
					MainCam->owner->transform.Translate(0.0f, 1.0f);
				}
			}
		}
		break;
		}
	}

	if (toggle_drawpath)
	{
		switch (enum_typeofcurve) {
		case _BernsteinPolynormialCurve:
			Bezier_BernsteinPolynormial.DrawCurve();
			break;
		case _deCasteljauCurve:
			Bezier_deCasteljau.DrawCurve(curve_timeElasped, true);
			break;
		case _BezierCubic:
			BezierCubic.DrawCurve();
			break;
		}
	}
}

void UpdateImgui_Simulation1() 
{
	ImGui::Separator();
	ImGui::Text("Type Of Curves:");

	ImGui::Checkbox("Toggle Path", &toggle_drawpath);
	ImGui::Checkbox("PlayCurve", &play_curve);
	ImGui::Checkbox("Follow_Model", &follow_model);

	ImGui::SliderFloat("curve_TimeElasped", &curve_timeElasped, 0.0f, 6.9f);


	ImGui::SliderFloat("Curve Speed", &curvingSpeed, 0.0f, 1.0f);

	ImGui::DragFloat("Pace", &animationPace, 0.001f, 0.001f, 10.0f, "%.6f");

	// typeof curve
	if (ImGui::RadioButton("Bezier_BernsteinPolynormial", enum_typeofcurve == _BernsteinPolynormialCurve))
	{
		enum_typeofcurve = _BernsteinPolynormialCurve;

		if (toggle_BPC_slotitascontrolpoits)
			Bezier_BernsteinPolynormial.GeneratePathUsingControlPoint(ListOfPoint);
		else
			Bezier_BernsteinPolynormial.GeneratePathPointOnCurve(ListOfPoint, cusps);
		Bezier_BernsteinPolynormial.GenerateGizmoTable();

		Bezier_deCasteljau.ClearGizmoTable();
		BezierCubic.ClearGizmoTable();
	}
	if (ImGui::RadioButton("Bezier_deCasteljau", enum_typeofcurve == _deCasteljauCurve)) {
		enum_typeofcurve = _deCasteljauCurve;

		if (toggle_DCC_slotitascontrolpoits)
			Bezier_deCasteljau.GeneratePathUsingControlPoint(ListOfPoint);
		else
			Bezier_deCasteljau.GeneratePathPointOnCurve(ListOfPoint, cusps);
		Bezier_deCasteljau.GenerateGizmoTable();

		Bezier_BernsteinPolynormial.ClearGizmoTable();
		BezierCubic.ClearGizmoTable();
	}
	if (ImGui::RadioButton("BezierCubic", enum_typeofcurve == _BezierCubic)) {
		enum_typeofcurve = _BezierCubic;

		BezierCubic.GeneratePathPointOnCurve(ListOfPoint, cusps);
		BezierCubic.GenerateGizmoTable();

		Bezier_BernsteinPolynormial.ClearGizmoTable();
		Bezier_deCasteljau.ClearGizmoTable();
	}

	//Cusps
	if (ImGui::SliderFloat("Cusps", &cusps, 0.1f, 10.0f, "%.1f", 1.0f))
	{
		switch (enum_typeofcurve) {
		case _BernsteinPolynormialCurve:
		{
			if (toggle_BPC_slotitascontrolpoits)
				Bezier_BernsteinPolynormial.GeneratePathUsingControlPoint(ListOfPoint);
			else
				Bezier_BernsteinPolynormial.GeneratePathPointOnCurve(ListOfPoint, cusps);
			Bezier_BernsteinPolynormial.GenerateGizmoTable();
		}
		break;
		case _deCasteljauCurve:
		{
			if (toggle_DCC_slotitascontrolpoits)
				Bezier_deCasteljau.GeneratePathUsingControlPoint(ListOfPoint);
			else
				Bezier_deCasteljau.GeneratePathPointOnCurve(ListOfPoint, cusps);
			Bezier_deCasteljau.GenerateGizmoTable();
		}
		break;
		case _BezierCubic:
			BezierCubic.GeneratePathPointOnCurve(ListOfPoint, cusps);
			BezierCubic.GenerateGizmoTable();
			break;
		}
	}
	ImGui::SliderFloat("Ease In", &easein, 0.1f, 0.5f, "%.1f");
	ImGui::SliderFloat("Ease Out", &easeout, 0.5f, 1.0f, "%.1f");

	//aditional table
	switch (enum_typeofcurve) {
	case _BernsteinPolynormialCurve:
		if (ImGui::Checkbox("Slot As Controls Points", &toggle_BPC_slotitascontrolpoits))
		{
			if (toggle_BPC_slotitascontrolpoits)
				Bezier_BernsteinPolynormial.GeneratePathUsingControlPoint(ListOfPoint);
			else
				Bezier_BernsteinPolynormial.GeneratePathPointOnCurve(ListOfPoint);
			Bezier_BernsteinPolynormial.GenerateGizmoTable();
		}
		break;
	case _deCasteljauCurve:
		if (ImGui::Checkbox("Slot As Controls Points", &toggle_DCC_slotitascontrolpoits))
		{
			if (toggle_DCC_slotitascontrolpoits)
				Bezier_deCasteljau.GeneratePathUsingControlPoint(ListOfPoint);
			else
				Bezier_deCasteljau.GeneratePathPointOnCurve(ListOfPoint);

			Bezier_deCasteljau.GenerateGizmoTable();
		}
		break;
	case _BezierCubic:
		if (ImGui::RadioButton("No_Table", use_cubicBezierTable == 0)) {
			use_cubicBezierTable = 0;
			animationPace = 0.493f;
		}
		if (ImGui::RadioButton("Use_FixedTable", use_cubicBezierTable == 1)) {
			use_cubicBezierTable = 1;
		}
		if (ImGui::RadioButton("Use_ArcLengthTable", use_cubicBezierTable == 2)) {
			use_cubicBezierTable = 2;
			animationPace = 0.493f;
		}
		ImGui::Checkbox("Toggle Ceneter Of Interest", &toggle_centerOfInterest);

		break;
	}
}