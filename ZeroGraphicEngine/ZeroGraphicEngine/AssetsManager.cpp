
#include "stdafx.h"
#include "AssetsManager.h"
//#include "GlobalGameObjectFunction.h"
//#include "GlobalFileSystem.h"
//#include "ResourceManager.h"
#include <thread>


//static std::map<size_t, Texture2D*> TEXTURE_POOL;
//static std::map<size_t, Material*>	MATERIAL_POOL;
//static std::map<size_t, SoundTrack*>SOUND_POOL;
static std::map<size_t, std::string>SCRIPT_POOL;
//static std::map<size_t, Font*>		  FONT_POOL;
//static std::map<size_t, Skybox*>		SKYBOX_POOL;

static std::chrono::high_resolution_clock::time_point start;

static void BeginTimer()
{
  start = std::chrono::high_resolution_clock::now();
}

static float EndTimer()
{
  std::chrono::duration<float> time = std::chrono::high_resolution_clock::now() - start;
  return time.count();
}

//void LoadMeshes()
//{
//  float loadTimer = 0.f;
//  BeginTimer();
//  {	////////// === Load Mesh === //////////
//    DIR_itr&& directory = GetDirectory(PATH_MODELS);
//    for (auto & p : directory) {
//      std::string filename;
//      GetFileNameWithExtension(p.path().string(), filename);
//
//      std::size_t found = filename.find(".models");
//      if (found != std::string::npos) {
//        size_t hashnum = GenerateHash2(filename);
//        Model* model = Allocate<Model>(1);
//
//#ifdef USE_RAWNEW
//        model->filename = filename;
//        //mesh->LoadModelsFile(p.path().string().c_str());
//        model->LoadCustomModel(p.path().string());
//        MODEL_POOL[hashnum] = model;
//#else
//        new (get) IShader{ filename };
//        get->LoadShaders(p.path().string());
//        SHADER_POOL[hashnum] = get;
//#endif
//      }
//    }
//  }
//  loadTimer += EndTimer();
//  printf("Loaded meshes in %fs.\n", loadTimer);
//}

void LoadSounds()
{
  float loadTimer = 0.f;
//  BeginTimer();
//  {////////// === Load SoundTrack === //////////
//    DIR_itr&& directory = GetDirectory(PATH_SOUNDS);
//    for (auto & p : directory) {
//      std::string filename;
//      GetFileNameWithExtension(p.path().string(), filename);
//
//      std::size_t found = filename.find(".ogg");
//      if (found == std::string::npos)
//        found = filename.find(".wav");
//
//      if (found != std::string::npos) {
//        size_t hashnum = GenerateHash3(filename);
//        SoundTrack* get = Allocate<SoundTrack>(1);
//#ifdef USE_RAWNEW
//        get->filename = filename;
//        get->Load(p.path().string());
//        SOUND_POOL[hashnum] = get;
//#else
//        new (get) SoundTrack{ filename };
//        get->Load(p.path().string());
//        SOUND_POOL[hashnum] = get;
//#endif
//      }
//    }
//  }

  loadTimer += EndTimer();
  printf("Loaded sounds in %fs.\n", loadTimer);
}

void LoadAllAssets() 
{
  //std::thread t1(LoadMeshes);
  //std::thread t2(LoadSounds);
  
	//float loadTimer = 0.f;
	//BeginTimer();
	LoadShadersFromDIR(PATH_SHADERS.c_str());


//	{////////// === Load IShader === //////////
//		DIR_itr&& directory = GetDirectory(PATH_SHADERS);
//		for (auto & p : directory) {
//			std::string filename;
//			GetFileNameWithExtension(p.path().string(), filename);
//
//			std::size_t found = filename.find(".glsl");
//			if (found != std::string::npos) {
//				size_t hashnum	= GenerateHash(filename);
//				IShader* get = Allocate<IShader>(1);
//#ifdef USE_RAWNEW
//				get->filename = filename;
//				get->LoadShaders(p.path().string());
//				SHADER_POOL[hashnum] = get;
//#else
//				new (get) IShader{ filename };
//				get->LoadShaders(p.path().string());
//				SHADER_POOL[hashnum] = get;
//#endif
//			}
//		}
//	}
  //loadTimer += EndTimer();
  //printf("Loaded shaders in %fs.\n", loadTimer);
  //
  //loadTimer = 0.f;
 // BeginTimer();
//	{////////// === Load Texture2D === //////////
//		DIR_itr&& directory = GetDirectory(PATH_TEXTURES);
//		for (auto & p : directory) {
//			std::string filename;
//			GetFileNameWithExtension(p.path().string(), filename);
//
//			size_t hashnum	= GenerateHash(filename);
//			auto find		= TEXTURE_POOL.find(hashnum);
//			Texture2D* get  = Allocate<Texture2D>(1);
//#ifdef USE_RAWNEW
//			get->filename = filename;
//			if (get->Load(p.path().string())) {
//				TEXTURE_POOL[hashnum] = get;
//			}
//			else DeallocateOne(get);
//#else
//			new (get) Texture2D{ filename };
//			get->filename = filename;
//			if (get->Load(p.path().string())) {
//				TEXTURE_POOL[hashnum] = get;
//			}
//			else DeallocateOne(get);
//#endif
//		}
//	}
	LoadTexturesFromDIR(PATH_TEXTURES.c_str());
	LoadFBXModelAndConvertFromDIR(PATH_FBX.c_str(), PATH_MODELS.c_str());
	//LoadModelFromDIR(PATH_MODELS.c_str());
 // loadTimer += EndTimer();
  //printf("Loaded textures in %fs.\n", loadTimer);
 // loadTimer = 0.f;
 // BeginTimer();
//	{////////// === Load Material === //////////
//		DIR_itr&& directory = GetDirectory(PATH_MATERIALS);
//		for (auto & p : directory) {
//			std::string filename;
//			GetFileNameWithExtension(p.path().string(), filename);
//
//			std::size_t found = filename.find(".material");
//			if (found != std::string::npos) {
//				size_t hashnum = GenerateHash(filename);
//				Material* get = Allocate<Material>(1);
//#ifdef USE_RAWNEW
//				get->filename = filename;
//				get->LoadMaterial(p.path().string());
//				MATERIAL_POOL[hashnum] = get;
//#else
//				new (get) Material{ filename };
//				get->LoadMaterial(p.path().string());
//				MATERIAL_POOL[hashnum] = get;
//#endif
//			}
//		}
//	}
  //loadTimer += EndTimer();
 // printf("Loaded materials in %fs.\n", loadTimer);


 // loadTimer = 0.f;
 // BeginTimer();
////////////////////////////////////////////////////////////////////// NO ALLOCATION
//	{////////// === Load etc === //////////
//		FileMgr file;
//		if (file.Open(PATH_ETC + "gametag.data", FILE_READ)) {
//			std::string ptrstring;
//			while (!file.IsEof()) {
//				file.ReadLine(ptrstring);
//
//				////////// === Load GameTag === //////////
//				if (ptrstring == "t:") {
//					file.ReadLine(ptrstring);
//					while (ptrstring != "c:") {
//						AddGameTag(ptrstring);
//						file.ReadLine(ptrstring);
//					}
//				}
//				////////// === Load Collision group === //////////
//				if (ptrstring == "c:") {
//					size_t hashedString = 0;
//					char a[2];
//					char b[128];
//					ZeroMemory(a, 2);
//					ZeroMemory(b, 128);
//					file.ReadLine(ptrstring);
//					while (ptrstring != "raycast:") {
//						sscanf_s(ptrstring.c_str(), "%s %s", a, 2, b, 128);
//						if (a[0] == 'P') {
//							static std::hash<std::string> hasher;
//
//							hashedString = hasher(b);
//						}
//						else if (a[0] == 'C') {
//							static std::hash<std::string> hasher;
//							if (b[0] == '\0')
//								ErrorMsg("Error file format in collision group!");
//							size_t tag = hasher(b);
//							AddCollisionTag(hashedString, tag);
//						}
//						file.ReadLine(ptrstring);
//					}
//				}
//
//				////////// === Load RayCast group === //////////
//				if (ptrstring == "raycast:") {
//					size_t hashedString = 0;
//					char a[2];
//					char b[128];
//					ZeroMemory(a, 2);
//					ZeroMemory(b, 128);
//					file.ReadLine(ptrstring);
//					while (!file.IsEof()) {
//						sscanf_s(ptrstring.c_str(), "%s %s", a, 2, b, 128);
//						if (a[0] == 'P') {
//							static std::hash<std::string> hasher;
//							hashedString = hasher(b);
//						}
//						else if (a[0] == 'C') {
//							static std::hash<std::string> hasher;
//							if (b[0] == '\0')
//								ErrorMsg("Error file format in raycast group!");
//							size_t tag = hasher(b);
//							AddRayCastTag(hashedString, tag);
//						}
//						file.ReadLine(ptrstring);
//					}
//					break;
//				}
//			}
//			file.Close();
//		}
//	}
  //loadTimer += EndTimer();
  //printf("Loaded other data in %fs.\n", loadTimer);
  //loadTimer = 0.f;
  //BeginTimer();
//	{////////// === Load InputAlias === //////////
//		FileMgr file;
//		if (file.Open(PATH_ETC + "inputalias.data", FILE_READ)) {
//			std::string ptrstring;
//			file.ReadLine(ptrstring);
//			while (!file.IsEof()) {
//				
//				if (ptrstring == "qwert:") {
//					std::string one;
//					file.ReadLine(one);
//					AddInputKeyAlias(one);
//
//					char a[2];
//					char b[4];
//					ZeroMemory(a, 2);
//					ZeroMemory(b, 4);
//					file.ReadLine(ptrstring);
//					while (!file.IsEof()) {
//						sscanf_s(ptrstring.c_str(), "%s %s", a, 2, b, 4);
//						if (a[0] == 'k') {
//							AddInputKeyCode(one, atoi(b));
//						}
//						else if (a[0] == 'c') {
//							AddInputControllerCode(one, atoi(b));
//						}
//						file.ReadLine(ptrstring);
//
//						if (ptrstring == "qwert:")
//							break;
//					}
//
//				}
//			}
//			file.Close();
//		}
//	}
  //loadTimer += EndTimer();
  //printf("Loaded InputAlias in %fs.\n", loadTimer);
  //loadTimer = 0.f;
  BeginTimer();
	////////////////////////////////////////////////////////////////////// ALLOCATION
	//{////////// === Load Archetype === //////////
	//	DIR_itr&& directory = GetDirectory(PATH_ARCH);
	//	for (auto & p : directory) {
	//		std::string filename;
	//		GetFileNameWithExtension(p.path().string(), filename);
	//
	//		std::size_t found = filename.find(".xml");
	//		if (found != std::string::npos) {
	//			//size_t hashnum = GenerateHash(filename);
	//			ResourceArchtype::LoadArch(p.path().string());
	//		}
	//	}
	//}
 // loadTimer += EndTimer();
 // printf("Loaded Archetypes in %fs.\n", loadTimer);
 // loadTimer = 0.f;
 // BeginTimer();
//	{////////// === Load Scripts === //////////
//		DIR_itr&& directory = GetDirectory(PATH_SCRIPTS);
//		for (auto & p : directory) {
//			std::string filename;
//			GetFileNameWithExtension(p.path().string(), filename);
//
//			std::size_t found = filename.find(".lua");
//			if (found != std::string::npos) {
//				static std::hash<std::string> hasher;
//				size_t hashedString = hasher(filename);	// Compute the hash for the string
//				SCRIPT_POOL[hashedString] = filename;
//			}
//		}
//	}
  //loadTimer += EndTimer();
  //printf("Loaded scripts in %fs.\n", loadTimer);
  //loadTimer = 0.f;
  //BeginTimer();
//  {////////// === Load Fonts === //////////
//    DIR_itr&& directory = GetDirectory(PATH_FONT);
//    for (auto & p : directory)
//    {
//      std::string filename;
//      GetFileNameWithExtension(p.path().string(), filename);
//
//      std::size_t found = filename.find(".fontData");
//      if (found != std::string::npos)
//      {
//        size_t hashnum = GenerateHash(filename);
//        Font* get = Allocate<Font>(1);
//#ifdef USE_RAWNEW
//        get->Load(p.path().string());
//        FONT_POOL[hashnum] = get;
//#else
//        new (get) Material{ filename };
//        get->LoadMaterial(p.path().string());
//        MATERIAL_POOL[hashnum] = get;
//#endif
//      }
//    }
//  }

 // loadTimer += EndTimer();
//  printf("Loaded fonts in %fs.\n", loadTimer);

  //t1.join();
  //t2.join();
  //
  //MergeHashTables();
  //
  //for (auto& mesh : MODEL_POOL)
  //  mesh.second->Init();

  //loadTimer = 0.f;
  //BeginTimer();
  //{////////// === Load Skyboxes === //////////
  //  Skybox* def = Allocate<Skybox>(1);
  //  SKYBOX_POOL[GetHashFromString("None")] = def;
  //
  //  auto names = GetAllTextureNames();
  //  for (auto& elem : names)
  //  {
  //    if (elem[0] == 'S' && elem[1] == 'B')
  //    {
  //      auto name = elem.substr(0, elem.find('_'));
  //
  //      if (SKYBOX_POOL.find(GetHashFromString(name)) == SKYBOX_POOL.end())
  //      {
  //        Skybox* sb = Allocate<Skybox>(1);
  //
  //        sb->SetTextures(name);
  //        SKYBOX_POOL[GetHashFromString(name)] = sb;
  //      }
  //    }
  //  }
  //}
 // loadTimer += EndTimer();
 // printf("Loaded skyboxes in %fs.\n", loadTimer);
}
void DestroyAllAssets() {

	DestroyAllModels();
	DestroyAllShaders();
	DestroyAllTextures();

	//for (auto& elem : SHADER_POOL)	DeallocateOne(elem.second); SHADER_POOL.clear();
	//for (auto& elem : MODEL_POOL)	DeallocateOne(elem.second);	MODEL_POOL.clear();
	//for (auto& elem : TEXTURE_POOL)	DeallocateOne(elem.second);	TEXTURE_POOL.clear();
	//for (auto& elem : MATERIAL_POOL)DeallocateOne(elem.second);	MATERIAL_POOL.clear();
	//for (auto& elem : SOUND_POOL)	DeallocateOne(elem.second);	SOUND_POOL.clear();
	//for (auto& elem : FONT_POOL)	DeallocateOne(elem.second);	FONT_POOL.clear();
	//for (auto& elem : SKYBOX_POOL)	DeallocateOne(elem.second);	SKYBOX_POOL.clear();
	//ResourceArchtype::ClearResourceArchtype();
}

////////// === Load IShader === //////////
//IShader*& FindShader(std::string&& filename_extension_) {
//	size_t hashnum = GenerateHash(filename_extension_);
//#if defined(DEBUG_MODE_ON) || defined(_DEBUG) || defined(_DEBUG) || defined(_EDITOR_ON)
//	auto find = SHADER_POOL.find(hashnum);
//	if (find == SHADER_POOL.end())
//		PopUp_Warn(std::string{ "Shader Not Found: " + filename_extension_ }.c_str());
//	return find == SHADER_POOL.end() ? SHADER_POOL[GetHashFromString("null.glsl")] : find->second;
//#else
//	return SHADER_POOL[hashnum];
//#endif
//}
//IShader*& FindShader(const std::string& filename_extension_) {
//	size_t hashnum = GenerateHash(filename_extension_);
//#if defined(_DEBUG) || defined(_EDITOR_ON)
//	auto find = SHADER_POOL.find(hashnum);
//	if (find == SHADER_POOL.end())
//		PopUp_Warn(std::string{ "Shader Not Found: " + filename_extension_ }.c_str());
//	return find == SHADER_POOL.end() ? SHADER_POOL[GetHashFromString("null.glsl")] : find->second;
//#else
//	return SHADER_POOL[hashnum];
//#endif
//}
//IShader *& FindShader(unsigned int hashnum)
//{
//#if defined(_DEBUG) || defined(_EDITOR_ON)
//	auto find = SHADER_POOL.find(hashnum);
//	if (find == SHADER_POOL.end())
//		PopUp_Warn(std::string{ "Texture Not Found: " + GetStringFromHash(hashnum) }.c_str());
//	return find == SHADER_POOL.end() ? SHADER_POOL[GetHashFromString("null.glsl")] : find->second;
//#else
//	return SHADER_POOL[hashnum];
//#endif
//}
//
//const std::map<size_t, IShader*>& GetAllShaders() {
//	return SHADER_POOL;
//}
//std::vector<std::string> GetAllShadersName() {
//	std::vector<std::string> allname;
//	for (auto& elem : SHADER_POOL) {
//		allname.push_back(GetStringFromHash(elem.first));
//	}
//	return allname;
//}

////////// === Load Mesh === //////////
//Model*& FindModel(std::string&& filename_extension_) {
//	size_t hashnum = GenerateHash(filename_extension_);
//	auto find = MODEL_POOL.find(hashnum);
//	bool found = find == MODEL_POOL.end();
//
//#if defined(_DEBUG) || defined(_EDITOR_ON)
//	if (found)
//		PopUp_Warn(std::string{ "Models file Not Found: " + filename_extension_ }.c_str());
//	return find == MODEL_POOL.end() ? MODEL_POOL[GetHashFromString("null.models")] : find->second;
//#else
//	return MODEL_POOL[hashnum];
//#endif
//}
//
//Model*& FindModel(const std::string& filename_extension_) {
//	size_t hashnum = GenerateHash(filename_extension_);
//	auto find = MODEL_POOL.find(hashnum);
//	bool found = find == MODEL_POOL.end();
//
//#if defined(_DEBUG) || defined(_EDITOR_ON)
//	if (found)
//		PopUp_Warn(std::string{ "Models file Not Found: " + filename_extension_ }.c_str());
//	return find == MODEL_POOL.end() ? MODEL_POOL[GetHashFromString("null.models")] : find->second;
//#else
//	return MODEL_POOL[hashnum];
//#endif
//}
//
//Model *& FindModel(unsigned int hashnum) {
//	auto find = MODEL_POOL.find(hashnum);
//	bool found = find == MODEL_POOL.end();
//
//#if defined(_DEBUG) || defined(_EDITOR_ON)
//	if (found)
//		PopUp_Warn(std::string{ "Models file Not Found: " + GetStringFromHash(hashnum) }.c_str());
//	return found ? MODEL_POOL[GetHashFromString("null.models")] : find->second;
//#else
//	return SHADER_POOL[hashnum];
//#endif
//}

//const std::map<size_t, Model*>& GetAllModel() {
//	return MODEL_POOL;
//}
//
//std::vector<std::string> GetAllModelName() {
//	std::vector<std::string> allname;
//	for (auto& elem : MODEL_POOL) {
//		allname.push_back(GetStringFromHash(elem.first));
//	}
//	return allname;
//}

////////// === Load Texture2D === //////////
//Texture2D*& LoadTexture(const std::string& pathname_filename_extension) {
//	std::string filename;
//	GetFileNameWithExtension(pathname_filename_extension, filename);
//	size_t hashnum	= GenerateHash(filename);
//	auto find		= TEXTURE_POOL.find(hashnum);
//
//	if (find == TEXTURE_POOL.end()) {
//		Texture2D* get = new Texture2D{};
//		if (get->Load(pathname_filename_extension)) {
//			TEXTURE_POOL[hashnum] = get;
//			return TEXTURE_POOL[hashnum];
//		}
//		else {
//			delete get;
//			return TEXTURE_POOL[GetHashFromString("null.dds")];
//		}
//	}
//	else return find->second;
//}
//Texture2D*& FindTexture(std::string&& filename_extension_) 
//{
//  return FindTexture(GetHashFromString(filename_extension_));
//}
//Texture2D*& FindTexture(const std::string& filename_extension_) 
//{
//	return FindTexture(GetHashFromString(filename_extension_));
//}
//Texture2D *& FindTexture(size_t hashnum)
//{
//#if defined(_DEBUG) || defined(_EDITOR_ON)
//  auto find = TEXTURE_POOL.find(hashnum);
//  if (find == TEXTURE_POOL.end())
//    PopUp_Warn(std::string{ "Texture Not Found: " + GetStringFromHash(hashnum) }.c_str());
//  return find == TEXTURE_POOL.end() ? TEXTURE_POOL[GetHashFromString("null.dds")] : find->second;
//#else
//  return TEXTURE_POOL[hashnum];
//#endif
//}

//const std::map<size_t, Texture2D*>& GetAllTextures() {
//	return TEXTURE_POOL;
//}
//
//std::vector<std::string> GetAllTextureNames() {
//	std::vector<std::string> allname;
//	for (auto& elem : TEXTURE_POOL) {
//		allname.push_back(GetStringFromHash(elem.first));
//	}
//	return allname;
//}

////////// === Load Material === //////////
//Material*& LoadMaterial(const std::string& pathname_filename_extension) {
//	std::string filename;
//	GetFileNameWithExtension(pathname_filename_extension, filename);
//	std::size_t found = filename.find(".material");
//	if (found != std::string::npos) {
//		size_t hashnum	= GenerateHash(filename);
//		auto find		= MATERIAL_POOL.find(hashnum);
//		if (find == MATERIAL_POOL.end()) {
//			Material* get			= new Material{};
//			get->filename			= filename;
//			get->LoadMaterial(pathname_filename_extension);
//			MATERIAL_POOL[hashnum]	= get;
//			return MATERIAL_POOL[hashnum];
//		}
//		else return find->second;
//	}
//	else {
//		PopUp_Warn("Not a .material File");
//		return MATERIAL_POOL[GenerateHash("null.material")];
//	}
//}
//Material*& FindMaterial(std::string&& filename_extension_) 
//{
//	return FindMaterial(GenerateHash(filename_extension_));
//}
//Material*& FindMaterial(const std::string& filename_extension_) 
//{
//  return FindMaterial(GenerateHash(filename_extension_));
//}
//
//Material *& FindMaterial(unsigned int hashedName)
//{
//#if defined(DEBUG_MODE_ON) || defined(_DEBUG) || defined(_EDITOR_ON)
//  auto find = MATERIAL_POOL.find(hashedName);
//  if (find == MATERIAL_POOL.end())
//    PopUp_Warn(std::string{ "Material Not Found: " + GetStringFromHash(hashedName) }.c_str());
//  return find == MATERIAL_POOL.end() ? MATERIAL_POOL[GetHashFromString("null.material")] : find->second;
//#else
//  return MATERIAL_POOL[hashnum];
//#endif
//}

//const std::map<size_t, Material*>& GetAllMaterials() {
//	return MATERIAL_POOL;
//}
//std::vector<std::string> GetAllMaterialName() {
//	std::vector<std::string> allname;
//	for (auto& elem : MATERIAL_POOL) {
//		allname.push_back(GetStringFromHash(elem.first));
//	}
//	return allname;
//}

////////// === Load SoundTrack === //////////
//SoundTrack*& LoadSoundTracks(const std::string& pathname_filename_extension_) {
//	std::string filename;
//	GetFileNameWithExtension(pathname_filename_extension_, filename);
//
//	size_t hashnum	= GenerateHash(filename);
//	auto find		= SOUND_POOL.find(hashnum);
//
//	if (find == SOUND_POOL.end()) {
//		SoundTrack* get = new SoundTrack{};
//		get->filename = filename;
//		get->Load(pathname_filename_extension_);
//		SOUND_POOL[hashnum] = get;
//		return SOUND_POOL[hashnum];
//	}
//	else return find->second;
//}
//SoundTrack*& FindSoundTrack(std::string&& filename_extension_) {
//	size_t hashnum = GetHashFromString(filename_extension_);
//	auto find = SOUND_POOL.find(hashnum);
//#if defined(DEBUG_MODE_ON) || defined(_DEBUG) || defined(_EDITOR_ON)
//	if (find == SOUND_POOL.end())
//		PopUp_Warn(std::string{ "SoundTrack Not Found: " + filename_extension_ }.c_str());
//	return find->second;
//#else
//	return SOUND_POOL[hashnum];
//#endif
//}
//SoundTrack*& FindSoundTrack(const std::string& filename_extension_) {
//	size_t hashnum = GetHashFromString(filename_extension_);
//#if defined(DEBUG_MODE_ON) || defined(_DEBUG) || defined(_EDITOR_ON)
//	auto find = SOUND_POOL.find(hashnum);
//	if (find == SOUND_POOL.end())
//		PopUp_Warn(std::string{ "SoundTrack Not Found: " + filename_extension_ }.c_str());
//	return find->second;
//#else
//	return SOUND_POOL[hashnum];
//#endif
//}
//const std::map<size_t, SoundTrack*>& GetAllSoundTracks() {
//	return SOUND_POOL;
//}
//std::vector<std::string> GetAllSoundTrackName() {
//	std::vector<std::string> allname;
//	for (auto& elem : SOUND_POOL) {
//		allname.push_back(GetStringFromHash(elem.first));
//	}
//	return allname;
//}
//
//////////// === Load Archetype Name === //////////
//std::vector<std::string> GetArchetypeName() {
//	std::vector<std::string> allname;
//	for (auto& elem : ResourceArchtype::ARCH_GameObject) {
//		allname.push_back(GetStringFromHash(elem.first));
//	}
//	return allname;
//}

////////// === Load Script === //////////
//size_t AddScriptHash(const std::string& filename_extension_) {
//	static std::hash<std::string> hasher;
//	size_t hashedString = hasher(filename_extension_);	// Compute the hash for the string
//	SCRIPT_POOL[hashedString] = filename_extension_;
//	return hashedString;
//}
//std::string FindScriptStringFromHash(size_t hash_lua_) {
//	return SCRIPT_POOL[hash_lua_];
//}
//size_t FindScriptHashFromString(const std::string& filename_extension_) {
//	static std::hash<std::string> hasher;
//	size_t hashedString = hasher(filename_extension_);	// Compute the hash for the string
//	return hashedString;
//}
//std::vector<std::string> GetAllScriptName() {
//	std::vector<std::string> allname;
//	for (auto& elem : SCRIPT_POOL) {
//		allname.push_back(elem.second);
//	}
//	return allname;
//}

////////// === Load Font === //////////
//Font*& LoadFont(const std::string& pathname_filename_extension_) {
//  std::string filename;
//  GetFileNameWithExtension(pathname_filename_extension_, filename);
//
//  size_t hashnum = GenerateHash(filename);
//  auto find = FONT_POOL.find(hashnum);
//
//  if (find == FONT_POOL.end()) {
//    Font* get = new Font{};
//    get->Load(pathname_filename_extension_);
//    FONT_POOL[hashnum] = get;
//    return FONT_POOL[hashnum];
//  }
//  else return find->second;
//}
//
//Font *& FindFont(std::string && filename_extension_)
//{
//  return FindFont(GetHashFromString(filename_extension_));
//}
//
//Font *& FindFont(const std::string & filename_extension_)
//{
//  return FindFont(GetHashFromString(filename_extension_));
//}
//
//Font *& FindFont(unsigned int hashedName)
//{
//#if defined(DEBUG_MODE_ON) || defined(_DEBUG) || defined(_EDITOR_ON)
//  auto find = FONT_POOL.find(hashedName);
//  if (find == FONT_POOL.end())
//    PopUp_Warn(std::string{ "Texture Not Found: " + GetStringFromHash(hashedName) }.c_str());
//  return find == FONT_POOL.end() ? FONT_POOL[GetHashFromString("null.dds")] : find->second;
//#else
//  return FONT_POOL[hashedName];
//#endif
//}
//
//const std::map<size_t, Font*>& GetAllFonts()
//{
//  return FONT_POOL;
//}
//
//std::vector<std::string> GetAllFontName()
//{
//  std::vector<std::string> allname;
//  for (auto& elem : FONT_POOL) {
//    allname.push_back(GetStringFromHash(elem.first));
//  }
//  return allname;
//}
//
//Skybox *& FindSkybox(const std::string & skyboxName)
//{
//  return FindSkybox(GetHashFromString(skyboxName));
//}
//
//Skybox *& FindSkybox(unsigned int hashedName)
//{
//#if defined(DEBUG_MODE_ON) || defined(_DEBUG) || defined(_EDITOR_ON)
//  auto find = SKYBOX_POOL.find(hashedName);
//  if (find == SKYBOX_POOL.end())
//    PopUp_Warn(std::string{ "Skybox Not Found: " + GetStringFromHash(hashedName) }.c_str());
//  return find == SKYBOX_POOL.end() ? SKYBOX_POOL.begin()->second : find->second;
//#else
//  return FONT_POOL[hashedName];
//#endif
//}
//
//std::vector<std::string> GetAllSkyboxNames()
//{
//  std::vector<std::string> allname;
//  for (auto& elem : SKYBOX_POOL) {
//    allname.push_back(GetStringFromHash(elem.first));
//  }
//  return allname;
//}
