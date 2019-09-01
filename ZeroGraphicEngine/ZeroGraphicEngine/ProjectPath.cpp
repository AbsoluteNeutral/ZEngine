/*****************************************************************************/
/*!
\file			ProjectPath.cpp
\brief			ResourceManager, Global Pointers/Vector
*/
/*****************************************************************************/

#include "stdafx.h"
#include "ProjectPath.h"


//////////////////////////////////////////////////////////////////////////////////////////////////// DirectoryTools
char SAVE_FILE_PATH[256];
std::string project_path{};

std::string getPath(int PROJECT_PATH_enum_) {
	switch (PROJECT_PATH_enum_)
	{
	case _PATH_DEFAULT:			return project_path + "..\\all_resources\\";
	case _PATH_ARCH:			return project_path + "..\\all_resources\\archetypes\\";
	case _PATH_ETC:				return project_path + "..\\all_resources\\etc\\";
	case _PATH_DIALOG:			return project_path + "..\\all_resources\\dialog\\";
	case _PATH_FONT:			return project_path + "..\\all_resources\\font\\";
	case _PATH_FBX	:			return project_path + "..\\all_resources\\fbx\\";
	case _PATH_MATERIALS:		return project_path + "..\\all_resources\\materials\\";
	case _PATH_MODELS:			return project_path + "..\\all_resources\\models\\";
	case _PATH_SCENE:			return project_path + "..\\all_resources\\scene\\";
	case _PATH_SCRIPTS:			return project_path + "..\\all_resources\\scripts\\";
	case _PATH_SHADERS:			return project_path + "..\\all_resources\\shaders\\";
	case _PATH_SOUNDS:			return project_path + "..\\all_resources\\sound\\";
	case _PATH_TEXTURES:		return project_path + "..\\all_resources\\textures\\";
	case _XML_DEFAULT_PATHNAME:	return project_path + "..\\all_resources\\data\\";
	case _PATH_METADATA:		return project_path + "..\\all_resources\\metadata\\";
	case _PATH_CACHE:			return project_path + "..\\all_resources\\cache\\";
	case _PATH_CUTSCENE:		return project_path + "..\\all_resources\\cutscenes\\";
	case _PATH_GLOBALS:			return project_path + "..\\all_resources\\globalVariables\\";
	case _PATH_VIDEO:			return project_path + "..\\all_resources\\video\\";
	default:
		ErrorMsg("Path NOt found");
		return std::string{};
		break;
	}
}

void LoadSaveFilePath(const std::string& foldername_) {
	ZeroMemory(SAVE_FILE_PATH, 256);

	size_t strlen = 0;
	char* UserName = nullptr;
	_dupenv_s(&UserName, &strlen, "USERPROFILE");

	char* tmp = SAVE_FILE_PATH;
	strcpy(tmp, UserName);
	tmp += strlen - 1;

	strcpy(tmp, "\\Documents\\");
	tmp += 11;

	strcpy(tmp, foldername_.c_str());
	CreateDirectoryA(SAVE_FILE_PATH, NULL);
	delete UserName;

	char buffer[MAX_PATH];
	GetModuleFileName(NULL, buffer, MAX_PATH);
	std::string::size_type pos = std::string(buffer).find_last_of("\\/");
	project_path = std::string(buffer).substr(0, pos - 3);
}

void LoadSaveFilePath(std::string&& foldername_) {
	ZeroMemory(SAVE_FILE_PATH, 256);
	size_t strlen = 0;
	char* UserName = nullptr;
	_dupenv_s(&UserName, &strlen, "USERPROFILE");

	char* tmp = SAVE_FILE_PATH;
	strcpy(tmp, UserName);
	tmp += strlen - 1;

	strcpy(tmp, "\\Documents\\");
	tmp += 11;

	strcpy(tmp, foldername_.c_str());
	CreateDirectoryA(SAVE_FILE_PATH, NULL);
	delete UserName;

	char buffer[MAX_PATH];
	GetModuleFileName(NULL, buffer, MAX_PATH);
	std::string::size_type pos = std::string(buffer).find_last_of("\\/");
	project_path = std::string(buffer).substr(0, pos - 3);
}

DIR_itr GetDirectory(const std::string& filepath) {
	return DIR_itr(filepath);
}

void CopyFileToDirectory(const std::string& SRC, const std::string& DEST)
{
	fs::path dir = SRC;
	std::string temp{ DEST + dir.filename().string() };

	if (SRC != temp)
	{
		std::ifstream src(SRC, std::ios::binary);
		std::ofstream dest(temp, std::ios::binary);
		dest << src.rdbuf();

		if (!(src && dest))
		{
			PopUp_Warn("Failed to copy!");
		}
		else
			::MessageBoxA(nullptr, std::string{ dir.filename().string() + "\nsuccessfully copied to\n" + DEST }.c_str(), "Copy", MB_OK);
	}
}

bool GetFileNameWithExtensionAndCheckExtension(const std::string& pathname_, std::string& filename_, const char* extension_name_) {
	std::size_t found = pathname_.find_last_of(".");
	bool correct = pathname_.substr(found + 1) == extension_name_;

	if (correct) {
		found = pathname_.find_last_of("/\\");
		filename_ = pathname_.substr(found + 1);
	}
	return correct;
}

void GetFileNameWithExtension(const std::string& pathname_, std::string& filename_) {
	std::size_t found = pathname_.find_last_of("/\\");
	filename_ = pathname_.substr(found + 1);
}

void GetFileNameNoExtension(const std::string& pathname_, std::string& filename_) {
	std::size_t found = pathname_.find_last_of("/\\");
	std::size_t found2 = pathname_.find_last_of(".");
	filename_ = pathname_.substr(found + 1, found2 - found-1);
}

std::string GetFileExtension(const std::string& pathname_filename) {
	std::size_t found = pathname_filename.find_last_of(".");
	return pathname_filename.substr(found + 1);
}

int GetFileSize(const std::string& src)
{
	std::streampos begin, end;
	std::ifstream myfile(src.c_str(), std::ios::binary);
	begin = myfile.tellg();
	myfile.seekg(0, std::ios::end);
	end = myfile.tellg();
	myfile.close();
	return static_cast<int>(end - begin);
}
