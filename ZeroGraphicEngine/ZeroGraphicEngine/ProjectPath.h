
#ifndef _PROJECTPATH_H
#define _PROJECTPATH_H

#include "Logging.h"
#include <string>
#include <filesystem>

//For Serializing Component
#define IS_GAMEOBJECT "gameobject"
#define IS_TRANSFORM "transform"
#define IS_CAMERA "camera"
#define IS_SPRITE2D "sprite"
#define IS_MESHRENDERER "meshrenderer"
#define IS_PARTICLEEMITTER "particleemitter"
#define IS_AABB "aabb"
#define IS_BC "bc"
#define IS_OBB "obb"
#define IS_CAPSULECOLLIDER "capsulecollider"
#define IS_RIGIDBODY "rigidbody"
#define IS_SCRIPT "scripts"
#define IS_SOUND "sound"
#define IS_LIGHT "light"
#define IS_TEXTRENDERER "textrenderer"
#define IS_UISPRITE "uisprite"
#define IS_VIDEOCOMPO "videoCompo"
//etc
#define IS_OBJECTPOOL "objectpool"
#define IS_GAMESETUP "gamesetup"
#define IS_GAMECAMERA "gamecamera"
#define IS_AUDIOLISTENER "audiolistener"

//For Saving Number Of Objects
#define NUM_GAMEOBJECT "numGameObject"
#define NUM_CAMERA "numCamera"
#define NUM_MESHRENDERER "numMeshRenderer"
#define NUM_SPRITE2D "numSprite2D"
#define NUM_PARTICLEEMITTER "numParticleEmitter"
#define NUM_AABB "numAABB"
#define NUM_BC "numBC"
#define NUM_OBB "numOBB"
#define NUM_CAPSULECOLLIDER "numCapsuleCollider"
#define NUM_RIGIDBODY "numRigidBody"
#define NUM_SCRIPT "numScript"
#define NUM_SOUND "numSound"
#define NUM_TEXTRENDERER "numTextRenderer"
#define NUM_LIGHT "numLight"
#define NUM_UISPRITE "numUISprite"
#define NUM_VIDEOCOMPO "numVideoCompo"

enum PROJECT_PATH
{
	_PATH_DEFAULT,
	_PATH_ARCH,
	_PATH_ETC,
	_PATH_DIALOG,
	_PATH_FONT,
	_PATH_FBX,
	_PATH_MATERIALS,
	_PATH_MODELS,
	_PATH_SCENE,
	_PATH_SCRIPTS,
	_PATH_SHADERS,
	_PATH_SOUNDS,
	_PATH_TEXTURES,
	_XML_DEFAULT_PATHNAME,
	_PATH_METADATA,
	_PATH_CACHE,
	_PATH_CUTSCENE,
	_PATH_GLOBALS,
	_PATH_VIDEO
};

extern char SAVE_FILE_PATH[256];
ZG_API std::string getPath(int PROJECT_PATH_enum_);

#define PATH_DEFAULT			getPath(_PATH_DEFAULT)
#define PATH_ARCH				getPath(_PATH_ARCH)
#define PATH_ETC				getPath(_PATH_ETC)
#define PATH_DIALOG				getPath(_PATH_DIALOG)
#define PATH_FONT				getPath(_PATH_FONT)
#define PATH_FBX				getPath(_PATH_FBX)
#define PATH_MATERIALS			getPath(_PATH_MATERIALS)
#define PATH_MODELS				getPath(_PATH_MODELS)
#define PATH_SCENE				getPath(_PATH_SCENE)
#define PATH_SCRIPTS			getPath(_PATH_SCRIPTS)
#define PATH_SHADERS			getPath(_PATH_SHADERS)
#define PATH_SOUNDS				getPath(_PATH_SOUNDS)
#define PATH_TEXTURES			getPath(_PATH_TEXTURES)
#define XML_DEFAULT_PATHNAME	getPath(_XML_DEFAULT_PATHNAME)
#define PATH_METADATA			getPath(_PATH_METADATA)
#define PATH_CACHE				getPath(_PATH_CACHE)
#define PATH_CUTSCENE			getPath(_PATH_CUTSCENE)
#define PATH_GLOBALS			getPath(_PATH_GLOBALS)
#define PATH_VIDEO				getPath(_PATH_VIDEO)

ZG_API void LoadSaveFilePath	(const std::string& foldername_);
ZG_API void LoadSaveFilePath	(std::string&& foldername_);

namespace fs = std::experimental::filesystem;
typedef fs::directory_iterator DIR_itr;

ZG_API DIR_itr GetDirectory			(const std::string& filepath);
ZG_API void CopyFileToDirectory		(const std::string& SRC, const std::string& DEST);
ZG_API int GetFileSize				(const std::string& src);

//string staff
ZG_API bool GetFileNameWithExtensionAndCheckExtension(const std::string& pathname_, std::string& filename_, const char* extension_name_);
ZG_API void GetFileNameWithExtension				 (const std::string& pathname_, std::string& filename_);
ZG_API void GetFileNameNoExtension					 (const std::string& pathname_, std::string& filename_);
ZG_API std::string GetFileExtension					 (const std::string& pathname_filename);
#endif //_PROJECTPATH_H
