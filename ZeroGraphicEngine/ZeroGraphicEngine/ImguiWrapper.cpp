/*****************************************************************************/
/*!
\file			ImguiWrapper.cpp
\brief			Imgui Wrapper
*/
/*****************************************************************************/
#include "stdafx.h"
#include "ImguiWrapper.h"
#include "EditorMain.h"
#include "Engine.h"

#include <experimental/filesystem>

#include <tchar.h>
#include <psapi.h>
#include <strsafe.h>

#define BUFSIZE 512

namespace fs = std::experimental::filesystem;

/******************************************************************/
/*                        LOCAL VARIABLES                         */
/******************************************************************/
ImguiWrapper		IMGU{};
GridManager2D		GRIDMANAGER_2D{};
//GridManager3D		GRIDMANAGER_3D{};
unsigned			GAMECamera_NUM = 0;
unsigned			GAMEAudioListener_CameraNUM = 0;
bool NSIGHTCOMPAT = false;

INT64 ticks_per_second = 0;
INT64 last_time = 0;
// === END OF LOCAL VARIABLES  === //

BOOL GetFileNameFromHandle(HANDLE hFile);

ImguiWrapper::ImguiWrapper() noexcept
	:mainHWND(0), highDpiScale(1.0f)
{}

ImguiWrapper::~ImguiWrapper(){}

void ImImpl_RenderDrawLists(ImDrawData* draw_data)
{
	// We are using the OpenGL fixed pipeline to make the example code simpler to read!
	// A probable faster way to render would be to collate all vertices from all cmd_lists into a single vertex buffer.
	// Setup render state: alpha-blending enabled, no face culling, no depth testing, scissor enabled, vertex/texcoord/color pointers.
	glPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT | GL_TRANSFORM_BIT);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_SCISSOR_TEST);
	if (!NSIGHTCOMPAT)
	{
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);
	}
	glEnable(GL_TEXTURE_2D);
	glUseProgram(0); // You may want this if using this code in an OpenGL 3+ context
	
	// Setup orthographic projection matrix
	const float width = static_cast<float>(ImGui::GetIO().DisplaySize.x / 1.0f);
	const float height = static_cast<float>(ImGui::GetIO().DisplaySize.y / 1.0f);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0.0f, width, height, 0.0f, -1.0f, +1.0f);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	
	// Render command lists
#define OFFSETOF(TYPE, ELEMENT) ((size_t)&(((TYPE *)0)->ELEMENT))
	for (int n = 0; n < draw_data->CmdListsCount; n++){
		const ImDrawList* cmd_list = draw_data->CmdLists[n];
		const unsigned char* vtx_buffer = (const unsigned char*)&cmd_list->VtxBuffer.front();
		const ImDrawIdx* idx_buffer = &cmd_list->IdxBuffer.front();

    if (!NSIGHTCOMPAT)
    {
      glVertexPointer(2, GL_FLOAT, sizeof(ImDrawVert), (void*)(vtx_buffer + OFFSETOF(ImDrawVert, pos)));
      glTexCoordPointer(2, GL_FLOAT, sizeof(ImDrawVert), (void*)(vtx_buffer + OFFSETOF(ImDrawVert, uv)));
      glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(ImDrawVert), (void*)(vtx_buffer + OFFSETOF(ImDrawVert, col)));
    }

		for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.size(); cmd_i++)
		{
			const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
			if (pcmd->UserCallback)
			{
				pcmd->UserCallback(cmd_list, pcmd);
			}
			else
			{
				glBindTexture(GL_TEXTURE_2D, (GLuint)(intptr_t)pcmd->TextureId);
				glScissor((GLint)(pcmd->ClipRect.x * 1.0f),
					(GLint)((height - pcmd->ClipRect.w) * 1.0f),
					(GLint)((pcmd->ClipRect.z - pcmd->ClipRect.x) * 1.0f),
					(GLint)((pcmd->ClipRect.w - pcmd->ClipRect.y) * 1.0f));

				glDrawElements(GL_TRIANGLES, (GLsizei)pcmd->ElemCount, GL_UNSIGNED_SHORT, idx_buffer);
			}
			idx_buffer += pcmd->ElemCount;
		}
	}
#undef OFFSETOF
	// Restore modified state
  if (!NSIGHTCOMPAT)
  {
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
  }
	glBindTexture(GL_TEXTURE_2D, 0);
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glPopAttrib();
}
void ImguiWrapper::LoadFontTexture() {
	//ImFont* my_font2 = io.Fonts->AddFontFromFileTTF("extra_fonts/Karla-Regular.ttf", 15.0f);
	//ImFont* my_font3 = io.Fonts->AddFontFromFileTTF("extra_fonts/ProggyClean.ttf", 13.0f); my_font3->DisplayOffset.y += 1;
	//ImFont* my_font4 = io.Fonts->AddFontFromFileTTF("extra_fonts/ProggyTiny.ttf", 10.0f); my_font4->DisplayOffset.y += 1;
	//ImFont* my_font5 = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 20.0f, io.Fonts->GetGlyphRangesJapanese());
	//io.Fonts->AddFontFromFileTTF("..\\Dep\\imgui-master\\extra_fonts\\Ruda-Bold.ttf", 14.0f);

	unsigned char* pixels = nullptr;
	int width = 0, height = 0;
	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->GetTexDataAsAlpha8(&pixels, &width, &height);

	GLuint tex_id;
	glGenTextures(1, &tex_id);
	glBindTexture(GL_TEXTURE_2D, tex_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, width, height, 0, GL_ALPHA, GL_UNSIGNED_BYTE, pixels);

	// Store our identifier
	io.Fonts->TexID = (void *)(intptr_t)tex_id;
}

//void ImguiWrapper::LoadAllArchetypes()
//{
//	for (const auto& elem : GetArchetypeName())
//		objectPoolCount[GetHashFromString(elem)] = 0;
//}

bool ImguiWrapper::Init() {
	mainHWND = Engine::sys_WinDisplay.GetHWND();

	// Keyboard mapping. ImGui will use those arrayOfIndices to peek into the io.KeyDown[] array that we will update during the application lifetime.
	//RECT rect;
	//GetClientRect(mainHWND, &rect);
	//int display_w = (int)(rect.right - rect.left);
	//int display_h = (int)(rect.bottom - rect.top);
	//ImGuiIO& io						= ImGui::GetIO();
	//io.DisplaySize					= ImVec2((float)display_w, (float)display_h);   // Display size, in pixels. For clamping windows positions.
	ImGuiIO& io = ImGui::GetIO();
	io.DeltaTime					= 1.0f / 60.0f;
	io.KeyMap[ImGuiKey_Tab]			= VK_TAB;                              
	io.KeyMap[ImGuiKey_LeftArrow]	= VK_LEFT;
	io.KeyMap[ImGuiKey_RightArrow]	= VK_RIGHT;
	io.KeyMap[ImGuiKey_UpArrow]		= VK_UP;
	io.KeyMap[ImGuiKey_DownArrow]	= VK_DOWN;
	io.KeyMap[ImGuiKey_Home]		= VK_HOME;
	io.KeyMap[ImGuiKey_End]			= VK_END;
	io.KeyMap[ImGuiKey_Delete]		= VK_DELETE;
	io.KeyMap[ImGuiKey_Backspace]	= VK_BACK;
	io.KeyMap[ImGuiKey_Enter]		= VK_RETURN;
	io.KeyMap[ImGuiKey_Escape]		= VK_ESCAPE;
	io.KeyMap[ImGuiKey_A]			= 'A';
	io.KeyMap[ImGuiKey_C]			= 'C';
	io.KeyMap[ImGuiKey_V]			= 'V';
	io.KeyMap[ImGuiKey_X]			= 'X';
	io.KeyMap[ImGuiKey_Y]			= 'Y';
	io.KeyMap[ImGuiKey_Z]			= 'Z';
	io.RenderDrawListsFn			= ImImpl_RenderDrawLists;
	LoadFontTexture();
	//LoadScriptBank();
	//LoadAllArchetypes();

	CreateOneCameraOnScene();

	//simulation2
	MainCam->owner->transform.SetPosition(0.0f, 20.38f, 27.0f);
	MainCam->owner->transform.SetRotation(-40.0f, 0.0f, 0.0f);

	//MainCam->owner->transform.SetPosition(-8.136f, 4.39f, -1.684f);
	//MainCam->owner->transform.SetRotation(-122.07f, -47.495f, 97.5168f);

	MainCam->SetSize(1);
	MainCam->SetFarPlane(100000.f);
	MainCam->Update();

	GRIDMANAGER_2D.Create(50);
	//GRIDMANAGER_3D.Create(10);
	//CmdMgr.CreateCache();
	EngineSetFrameRate(true);
	return true;
}

void ImguiWrapper::CreateOneCameraOnScene() {
	//GameObject* gobj = CreateGameObject(_EMPTY, "Camera", false, false);
	//Camera* cam = Allocate<Camera>(1);
	//cam->Attach(gobj, RM::R_Camera.size());
	//RM::R_Camera.push_back(cam);
	//RM::audioListener->Attach(gobj);
	//cam->transform->SetPosition(0.0f, 0.0f, 20.0f);
	//cam->SetSize(1);
	//cam->Update();
	//GAMECamera_NUM = 0;
	//GAMEAudioListener_CameraNUM = 0;
}

void ImguiWrapper::UpdateImGui(){
	// Setup time step
	INT64 current_time;
	QueryPerformanceCounter((LARGE_INTEGER *)&current_time);
	ImGuiIO& io = ImGui::GetIO();
	io.DeltaTime	= (float)(current_time - last_time) / ticks_per_second;
	last_time		= current_time;
	// Setup inputs
    // (we already got mouse position, buttons, wheel from the window message callback)
    BYTE keystate[256];
    GetKeyboardState(keystate);
    for (int i = 0; i < 256; i++)
        io.KeysDown[i] = (keystate[i] & 0x80) != 0;
    io.KeyCtrl = (keystate[VK_CONTROL] & 0x80) != 0;
    io.KeyShift = (keystate[VK_SHIFT] & 0x80) != 0;
    //io.MousePos : filled by WM_MOUSEMOVE event
    //io.MouseDown : filled by WM_*BUTTON* events
    //io.MouseWheel : filled by WM_MOUSEWHEEL events
}

void ImguiWrapper::SystemUpdate(const HANDLE* dwChangeHandles, DWORD& dwWaitStatus) 
{
	UNREFERENCED_PARAMETER(dwChangeHandles);
	UNREFERENCED_PARAMETER(dwWaitStatus);

	//RM::R_GameObject.push_back(Allocate<GameObject>(1));
	//RM::R_MeshRenderer.push_back(Allocate<MeshRenderer>(1));
	//RM::R_MeshRenderer[0]->Attach(RM::R_GameObject[0], 0);
	//RM::R_MeshRenderer[0]->Init(FindModel("Asmodeus.models"), FindShader("animation.glsl"));
	//RM::R_GameObject[0]->name = "AAAAA";

	EditorInit();

	while (EngineIsRunning()) {
		UpdateImGui();
		//if (!toggle_shortcut_play)
		{
			EditorUpdate();

			EngineLateUpdate();

			if (IsEngineForcedClose()) {
				EngineUnForcedClose();
				EngineQuit();
				//imgui_click_menubar_exit = true;

				//if (KeyRelease(KEY_0))
				//	NSIGHTCOMPAT = !NSIGHTCOMPAT;
			}
			if (KeyRelease(KEY_F11))
				FullScreen(!IsFullScreen());
			if (KeyRelease(KEY_ESCAPE))
				EngineQuit();
		}
		//else {
			//GSM.GameLoop();
		//}

		//dwWaitStatus = WaitForSingleObject(dwChangeHandles[0], 1000);
		//
		//switch (dwWaitStatus)
		//{
		//case WAIT_OBJECT_0:
		//{
		//	// A file was created, renamed, or deleted in the directory.
		//	// Refresh this directory and restart the notification.
		//	PFILE_NAME_INFO nameInfo;
		//	ULONG nameSize;
		//	BOOL result;
		//
		//	//
		//	// Allocate an information structure that is hopefully large enough to
		//	// retrieve name information.
		//	//
		//
		//	nameSize = sizeof(FILE_NAME_INFO) + (sizeof(WCHAR) * MAX_PATH);
		//	nameInfo = (PFILE_NAME_INFO)LocalAlloc(LMEM_ZEROINIT, nameSize);
		//	if (GetFileInformationByHandleEx(dwChangeHandles[0], FileNameInfo, nameInfo, sizeof(FILE_NAME_INFO) + (_MAX_PATH * sizeof(TCHAR))))
		//	{
		//		std::wcout << nameInfo->FileName << std::endl;
		//	}
		//
		//	if (FindNextChangeNotification(dwChangeHandles[0]) == FALSE)
		//	{
		//		printf("\n ERROR: FindNextChangeNotification function failed.\n");
		//		//ExitProcess(GetLastError());
		//	}
		//	break;
		//}
		//case WAIT_TIMEOUT:
		//
		//	// A timeout occurred, this would happen if some value other 
		//	// than INFINITE is used in the Wait call and no changes occur.
		//	// In a single-threaded environment you might not want an
		//	// INFINITE wait.
		//
		//	printf("\nNo changes in the timeout period.\n");
		//	break;
		//
		//default:
		//	printf("\n ERROR: Unhandled dwWaitStatus.\n");
		//	//ExitProcess(GetLastError());
		//	break;
		//}
	}
}

bool ImguiWrapper::SystemExit(){
	//RE::ClearResourceEditor();
	//remove(std::string{ PATH_METADATA + "tmpPlayscene.meta" }.c_str());
	return true;
}

void ToolTip(const char* text) {
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::Text(text);
		ImGui::EndTooltip();
	}
}

//BOOL GetFileNameFromHandle(HANDLE hFile)
//{
//	BOOL bSuccess = FALSE;
//	TCHAR pszFilename[MAX_PATH + 1];
//	HANDLE hFileMap;
//
//	// Get the file size.
//	DWORD dwFileSizeHi = 0;
//	DWORD dwFileSizeLo = GetFileSize(hFile, &dwFileSizeHi);
//
//	if (dwFileSizeLo == 0 && dwFileSizeHi == 0)
//	{
//		_tprintf(TEXT("Cannot map a file with a length of zero.\n"));
//		return FALSE;
//	}
//
//	// Create a file mapping object.
//	hFileMap = CreateFileMapping(hFile,
//		NULL,
//		PAGE_READONLY,
//		0,
//		1,
//		NULL);
//
//	//Get the error message, if any.
//	DWORD errorMessageID = ::GetLastError();
//
//	LPSTR messageBuffer = nullptr;
//	size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
//		NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);
//
//	std::string message(messageBuffer, size);
//
//	//Free the buffer.
//	LocalFree(messageBuffer);
//
//	std::cout << message << std::endl;
//
//	if (hFileMap)
//	{
//		// Create a file mapping to get the file name.
//		void* pMem = MapViewOfFile(hFileMap, FILE_MAP_READ, 0, 0, 1);
//
//		if (pMem)
//		{
//			if (GetMappedFileName(GetCurrentProcess(),
//				pMem,
//				pszFilename,
//				MAX_PATH))
//			{
//
//				// Translate path with device name to drive letters.
//				TCHAR szTemp[BUFSIZE];
//				szTemp[0] = '\0';
//
//				if (GetLogicalDriveStrings(BUFSIZE - 1, szTemp))
//				{
//					TCHAR szName[MAX_PATH];
//					TCHAR szDrive[3] = TEXT(" :");
//					BOOL bFound = FALSE;
//					TCHAR* p = szTemp;
//
//					do
//					{
//						// Copy the drive letter to the template string
//						*szDrive = *p;
//
//						// Look up each device name
//						if (QueryDosDevice(szDrive, szName, MAX_PATH))
//						{
//							size_t uNameLen = _tcslen(szName);
//
//							if (uNameLen < MAX_PATH)
//							{
//								bFound = _tcsnicmp(pszFilename, szName, uNameLen) == 0
//									&& *(pszFilename + uNameLen) == _T('\\');
//
//								if (bFound)
//								{
//									// Reconstruct pszFilename using szTempFile
//									// Replace device path with DOS path
//									TCHAR szTempFile[MAX_PATH];
//									StringCchPrintf(szTempFile,
//										MAX_PATH,
//										TEXT("%s%s"),
//										szDrive,
//										pszFilename + uNameLen);
//									StringCchCopyN(pszFilename, MAX_PATH + 1, szTempFile, _tcslen(szTempFile));
//								}
//							}
//						}
//
//						// Go to the next NULL character.
//						while (*p++);
//					} while (!bFound && *p); // end of string
//				}
//			}
//			bSuccess = TRUE;
//			UnmapViewOfFile(pMem);
//		}
//
//		CloseHandle(hFileMap);
//	}
//	_tprintf(TEXT("File name is %s\n"), pszFilename);
//	return(bSuccess);
//}