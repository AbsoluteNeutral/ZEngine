
#ifndef _MODEL_H
#define _MODEL_H

#define MAX_BONES 100
#include "ZGMath.h"
#include "ModelSkeleton.h"

#include <string>
#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

//______________________________________________________________________ Model, concatenation of ModelSkeleton
class ZG_API Model
{
public:
	Model() noexcept;
	Model(const char* filename_) noexcept;
	Model(const std::string& filename_) noexcept;
	~Model();

	void Init();
	void Draw				();
	void LoadCustomModel	(const std::string& pathname_);
	bool LoadFBXModel		(const std::string& pathname_);
	void ExportFile			(const std::string& fileOut);
	void Print				();

	std::vector<IVertex> GetListOfVertices() const;
	bool IsAnimationExist	(const char* animationTitle_) const;
	bool IsAnimationExist	(const std::string& animationTitle_) const;
	bool IsAnimationExist	(size_t hashAnimationTitle_) const;

private:
	void ProcessMesh		(unsigned meshID, aiMesh* mesh);
	void ProcessBones		(unsigned meshID, aiMesh* mesh);
	void ProcessNode		(aiNode* node, const aiScene * scene, INode* n);
	void ExportNodes		(std::fstream& outFile, const INode* node);
	void ReadNode			(FILE* file, INode* n, char* b, unsigned int sz);
	//void FindParentBones	(INode*  node);

	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;
	std::vector<IMesh> meshes;			//Contains All Meshes in the Model/Scene
	std::vector<IVertex> vertices;		//Contains All Vertices in the Model/Scene
	std::vector<unsigned int> indices;	//Contains All Indices of each triangle in the Model/Scene
	
public:
	std::string filename;	//model filename
	INode iNode;			
	unsigned int boneCount;
	bool hasAnims;
	
	std::unordered_map<size_t, Animation> animations;
	std::map<size_t, IBone> boneOffsetMap;	//bone offset matrix
	float    scalingFactor;
	//zg::Matrix44 extra_calefare_scalingmatrix;
	//std::map<unsigned, std::vector<zg::Vector3>> listOfFaceVertex;
};

//function

ZG_API void DestroyAllModels				();							//internal use only
ZG_API void LoadModelFromDIR				(const char* pathname_);	//internal use only
ZG_API void LoadFBXModelAndConvertFromDIR	(const char* fbx_pathname_, const char* models_pathname_);	//internal use only

ZG_API Model*& FindModel					(std::string&& filename_extension_);
ZG_API Model*& FindModel					(const std::string& filename_extension_);
ZG_API Model*& FindModel					(size_t hashedName);

ZG_API const std::map<size_t, Model*>&	GetAllModel		();
ZG_API std::vector<std::string>			GetAllModelName	();

//Hashing
ZG_API size_t GenerateHash_ModelName	(const char* model_names_bones_);
ZG_API size_t GenerateHash_ModelName	(const std::string& model_names_bones_);
ZG_API size_t GetHashFromModelName		(const char* model_names_bones_);
ZG_API size_t GetHashFromModelName		(const std::string& string_);
ZG_API std::string GetModelNameFromHash	(size_t hash_);

#endif
