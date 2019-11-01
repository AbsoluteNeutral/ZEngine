
#ifndef _ASSETSMANAGER_H
#define _ASSETSMANAGER_H

#include "IShader.h"
//#include "Material.h"
#include "Model.h"
//#include "SoundTrack.h"
#include "Texture2D.h"
//#include "Font.h"
//#include "Skybox.h"

enum ASSETS_TYPE {
	ASSETS_ISHADER,
	ASSETS_MATERIAL,
	ASSETS_MESH,
	ASSETS_SOUNDTRACK,
	ASSETS_TEXTURE,
	ASSETS_FONT,
	ASSETS_SKYBOX
};

ZG_API void LoadAllAssets();
//ZG_API void ReLoadAllAssets(ASSETS_TYPE assets_type_);
ZG_API void DestroyAllAssets();

////////// === Load IShader === //////////
//ZG_API IShader*& FindShader				(std::string&& filename_extension_);
//ZG_API IShader*& FindShader				(const std::string& filename_extension_);
//ZG_API IShader*& FindShader				(unsigned int hashedName);
//ZG_API const std::map<size_t, IShader*>& GetAllShaders();
//ZG_API std::vector<std::string> GetAllShadersName();

////////// === Load Mesh === //////////
//ZG_API Model*& FindModel					(std::string&& filename_extension_);
//ZG_API Model*& FindModel					(const std::string& filename_extension_);
//ZG_API Model*& FindModel					(unsigned int hashedName);
//ZG_API const std::map<size_t, Model*>& GetAllModel();
//ZG_API std::vector<std::string> GetAllModelName();

////////// === Load Texture2D === //////////
//ZG_API Texture2D*& LoadTexture(const std::string& pathname_filename_extension_);
//ZG_API Texture2D*& FindTexture(std::string&& filename_extension_);
//ZG_API Texture2D*& FindTexture(const std::string& filename_extension_);
//ZG_API Texture2D*& FindTexture(size_t hashedName);
//ZG_API const std::map<size_t, Texture2D*>& GetAllTextures();
//ZG_API std::vector<std::string> GetAllTextureNames();

////////// === Load Material === //////////
//ZG_API Material*& LoadMaterial(const std::string& pathname_filename_extension);
//ZG_API Material*& FindMaterial(std::string&& filename_extension_);
//ZG_API Material*& FindMaterial(const std::string& filename_extension_);
//ZG_API Material*& FindMaterial(unsigned int hashedName);
//ZG_API const std::map<size_t, Material*>& GetAllMaterials();
//ZG_API std::vector<std::string> GetAllMaterialName();

////////// === Load SoundTrack === //////////
//ZG_API SoundTrack*& LoadSoundTracks(const std::string& pathname_filename_extension_);
//ZG_API SoundTrack*& FindSoundTrack(std::string&& filename_extension_);
//ZG_API SoundTrack*& FindSoundTrack(const std::string& filename_extension_);
//ZG_API const std::map<size_t, SoundTrack*>& GetAllSoundTracks();
//ZG_API std::vector<std::string> GetAllSoundTrackName();

////////// === Load Script === //////////
//ZG_API size_t AddScriptHash(const std::string& filename_extension_);
//ZG_API std::string FindScriptStringFromHash(size_t hash_lua_);
//ZG_API size_t FindScriptHashFromString(const std::string& filename_extension_);
//ZG_API std::vector<std::string> GetAllScriptName();
//
//////////// === Load Archetype Name === //////////
//ZG_API std::vector<std::string> GetArchetypeName();

////////// === Load Font === //////////
//ZG_API Font*& LoadFont(const std::string& pathname_filename_extension_);
//ZG_API Font*& FindFont(std::string&& filename_extension_);
//ZG_API Font*& FindFont(const std::string& filename_extension_);
//ZG_API Font*& FindFont(unsigned int hashedName);
//ZG_API const std::map<size_t, Font*>& GetAllFonts();
//ZG_API std::vector<std::string> GetAllFontName();
//
//////////// === Find Skyboxes === //////////
//ZG_API Skybox*& FindSkybox(const std::string& skyboxName);
//ZG_API Skybox*& FindSkybox(unsigned int hashedName);
//ZG_API std::vector<std::string> GetAllSkyboxNames();
#endif
