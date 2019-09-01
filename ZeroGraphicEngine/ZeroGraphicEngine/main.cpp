#include "stdafx.h"
#include "ImguiWrapper.h"
#include "UnitTest.h"

int WINAPI WinMain(HINSTANCE hInstanceH, HINSTANCE prevInstanceH, LPSTR command_line, int showCmd)
{
	UNREFERENCED_PARAMETER(prevInstanceH);
	UNREFERENCED_PARAMETER(command_line);
	UNREFERENCED_PARAMETER(showCmd);

#if !defined(_EDITOR_ON)
	EngineInitialize(hInstanceH);
	RM::R_GameObject.push_back(Allocate<GameObject>(1));
	RM::R_MeshRenderer.push_back(Allocate<MeshRenderer>(1));
	RM::R_MeshRenderer[0]->Init(FindModel("Heavy.models"), FindShader("animation.glsl"));
	RM::R_MeshRenderer[0]->Attach(RM::R_GameObject[0], 0);
	MainCam->owner->transform.SetPosition(0.0f, 0.0f, 10.0f);
	while (EngineIsRunning() && !IsEngineForcedClose()) {

		EngineUpdate();

		//HANDLE console = GetStdHandle(STD_ERROR_HANDLE);
		//SetConsoleTextAttribute(console, FOREGROUND_RED | FOREGROUND_INTENSITY);
		if (KeyRelease(KEY_F11))
			FullScreen(!IsFullScreen());
		if (KeyRelease(KEY_ESCAPE))
			EngineQuit();

		Gizmo::Circle(Vector3{ 0.0f, 0.0f, 0.0f }, 0.1f);

		//if (GetMouseDelta().x != 0 || GetMouseDelta().y != 0) {
		//	std::cout << GetMouseDelta().x << ",";
		//	std::cout << GetMouseDelta().y << std::endl;
		//}

		EngineLateUpdate();
		//std::cout << GetFrameRate() << std::endl;
	}

	EngineExit();
#else
	HANDLE dwChangeHandles[2];
	DWORD dwWaitStatus;

	ImGui::CreateContext();
	//zg::SetResolution(R_800x600);

	EngineInitialize(hInstanceH, WindowProc, "ZG_EDITOR");

	//zg::UnitTestSite.RunAll();

	//std::vector<std::vector<float>> matrix;
	//for (unsigned i = 0; i < 4; ++i)
	//{
	//	matrix.push_back(std::vector<float>{});
	//	for (unsigned j = 0; j < 4; ++j)
	//	{
	//		matrix[i].push_back(zg::randomRange(0, 10));
	//	}
	//}
	//
	//zg::MatrixMN tmp{ matrix };
	//std::cout << tmp;
	//std::cout << tmp.Transposed();
	//std::cout << tmp * tmp.Transposed();
	//std::cout << GaussJordanInverse(tmp);

	//std::cout << zg::Vector2(0.1f, 0.0f).Normalized();

	IMGU.Init();
	
	IMGU.SystemUpdate(dwChangeHandles, dwWaitStatus);
	IMGU.SystemExit();

	EngineExit();
	ImGui::DestroyContext();

#endif

#if defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
}
