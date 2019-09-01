#include "stdafx.h"
#include "Model.h"
#include "MeshRenderer.h"

static std::map<size_t, Model*>	MODEL_POOL;
static std::map<size_t, std::string> HASHSTRING_MODEL_NAMES;

Model::Model() noexcept
	: VAO(0), VBO(0), EBO(0)
	, meshes{}
	, vertices{}
	, indices{}
	, filename{}
	, iNode{}
	, boneCount{0}
	, hasAnims(false)
	, animations{}
	, boneOffsetMap{}
	, scalingFactor(0.01f)
	//, extra_calefare_scalingmatrix{}
{
	//extra_calefare_scalingmatrix = zg::BuildScale4x4(0.01f);
}
Model::Model(const char* filename_) noexcept
	: VAO(0), VBO(0), EBO(0)
	, meshes{}
	, vertices{}
	, indices{}
	, filename{ filename_ }
	, iNode{}
	, boneCount{ 0 }
	, hasAnims(false)
	, animations{}
	, boneOffsetMap{}
	, scalingFactor(0.01f)
	//, extra_calefare_scalingmatrix{}
{
	//extra_calefare_scalingmatrix = zg::BuildScale4x4(0.01f);
}
Model::Model(const std::string& filename_) noexcept
	: VAO(0), VBO(0), EBO(0)
	, meshes{}
	, vertices{}
	, indices{}
	, filename{ filename_ }
	, iNode{}
	, boneCount{ 0 }
	, hasAnims(false)
	, animations{}
	, boneOffsetMap{}
	, scalingFactor(0.01f)
	//, extra_calefare_scalingmatrix{}
{
	//extra_calefare_scalingmatrix = zg::BuildScale4x4(0.01f);
}
Model::~Model()
{
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO);
}

void Model::LoadCustomModel(const std::string & pathname_) // Custom Model Format, requirement of GAM300
{
	FILE* file;
	fopen_s(&file, pathname_.c_str(), "r");

	if (file != NULL)
	{
		const int bufSz = 1024;
		char buf[bufSz], str[bufSz];
		int n;
		bool loadTan = false;

		ZeroMemory(buf, bufSz);
		ZeroMemory(str, bufSz);
		fgets(buf, bufSz, file);

		if (buf[0] == 'v')
		{
			loadTan = true;
		}

		fgets(buf, bufSz, file);

		int meshCount = 0;
		float getSize = scalingFactor;
		//float getSize = extra_calefare_scalingmatrix.m0;
		sscanf(buf, "%f %d", &getSize, &meshCount);
		//extra_calefare_scalingmatrix = zg::BuildScale4x4(getSize);
		scalingFactor = getSize;

		//printf("%s, %d\n", path.c_str(), size);
		fgets(buf, bufSz, file);

		for (int i = 0; i < meshCount; ++i)
		{
			meshes.emplace_back();
			sscanf(buf, "%d %d %d", &meshes.back().IndiceCount, &meshes.back().BaseVertex, &meshes.back().BaseIndex);
			fgets(buf, bufSz, file);
		}

		IVertex v;
		while (buf[0] != 'I')
		{
			if (loadTan)
			{
				sscanf(buf, "\tV %f %f %f N %f %f %f U %f %f W %f %f %f %f J %f %f %f %f T %f %f %f"
					, &v.position.x, &v.position.y, &v.position.z
					, &v.normal.x, &v.normal.y, &v.normal.z
					, &v.uv.x, &v.uv.y
					, &v.weights.x, &v.weights.y, &v.weights.z, &v.weights.w
					, &v.boneIndex.x, &v.boneIndex.y, &v.boneIndex.z, &v.boneIndex.w
					, &v.tangent.x, &v.tangent.y, &v.tangent.z
				);
			}
			else {
				sscanf(buf, "\tV %f %f %f N %f %f %f U %f %f W %f %f %f %f J %f %f %f %f"
					, &v.position.x, &v.position.y, &v.position.z
					, &v.normal.x, &v.normal.y, &v.normal.z
					, &v.uv.x, &v.uv.y
					, &v.weights.x, &v.weights.y, &v.weights.z, &v.weights.w
					, &v.boneIndex.x, &v.boneIndex.y, &v.boneIndex.z, &v.boneIndex.w
				);
			}

			vertices.push_back(v);
			fgets(buf, bufSz, file); //Maybe Vertices
		} //Exits when read I #

		int indexSize = 0;
		sscanf(buf, "I %d", &indexSize);
		indices.reserve(indexSize);

		fgets(buf, bufSz, file); //Maybe Indices

		int n1 = 0, n2 = 0, n3 = 0;
		while (buf[0] != 'N')
		{
			sscanf(buf, "%d %d %d", &n1, &n2, &n3);
			indices.push_back(n1);
			indices.push_back(n2);
			indices.push_back(n3);
			fgets(buf, bufSz, file); //Maybe Indices
		} //Exits when read N START

		ReadNode(file, &iNode, buf, bufSz);

		int id = 0;

		while (buf[0] == 'A')
		{
			float getDuration = 0.0f;
			float getticksPerSecond = 0.0f;

			sscanf_s(buf, "A START %f %f %s", &getDuration, &getticksPerSecond, str, bufSz);

			size_t getanimationTitle = GetHashFromModelName(str);
			animations[getanimationTitle] = Animation{ getanimationTitle, getDuration, getticksPerSecond };
			Animation& currAnimation = animations[getanimationTitle];

			fgets(buf, bufSz, file);

			while (buf[0] != 'A' && buf[2] != 'E')
			{
				sscanf_s(buf, "%s", str, bufSz);
				size_t hashBoneName = GetHashFromModelName(str);
				currAnimation.listOfBones[hashBoneName] = BoneAnimationChannel{};
				currAnimation.listOfBones[hashBoneName].boneName = hashBoneName;
				auto& currC = currAnimation.listOfBones[hashBoneName];

				fgets(buf, bufSz, file);
				sscanf(buf, "PK%d", &currC.numOfpositionKey);

				for (auto i = 0u; i < currC.numOfpositionKey; ++i)
				{
					currC.positionKeys.emplace_back();

					fgets(buf, bufSz, file);
					sscanf(buf, "\t%f %f %f %f", &currC.positionKeys[i].time, &currC.positionKeys[i].value.x, &currC.positionKeys[i].value.y, &currC.positionKeys[i].value.z);
				}

				fgets(buf, bufSz, file);
				sscanf_s(buf, "SK%d", &currC.numOfscaleKey);

				for (auto i = 0u; i < currC.numOfscaleKey; ++i)
				{
					currC.scaleKeys.emplace_back();

					fgets(buf, bufSz, file);
					sscanf(buf, "\t%f %f %f %f", &currC.scaleKeys[i].time, &currC.scaleKeys[i].value.x, &currC.scaleKeys[i].value.y, &currC.scaleKeys[i].value.z);

				}

				fgets(buf, bufSz, file);
				sscanf_s(buf, "RK%d", &currC.numOfrotationKey);

				for (auto i = 0u; i < currC.numOfrotationKey; ++i)
				{
					currC.rotationKeys.emplace_back();

					fgets(buf, bufSz, file);
					sscanf(buf, "\t%f %f %f %f %f", &currC.rotationKeys[i].time, &currC.rotationKeys[i].value.x, &currC.rotationKeys[i].value.y, &currC.rotationKeys[i].value.z, &currC.rotationKeys[i].value.w);
				}

				fgets(buf, bufSz, file);

				//channelMap[id][currC.boneName] = &currC;
			}
			++id;

			fgets(buf, bufSz, file);
			//printf("Channels: %d\n", currAnimation.channels.size());
		}

		sscanf(buf, "B %d", &boneCount);

		for (unsigned i = 0; i < boneCount; ++i)
		{
			fgets(buf, bufSz, file);
			sscanf_s(buf, "%d %s ", &n, str, bufSz);
			size_t getHash = GetHashFromModelName(str);
#if defined(_DEBUG)
			boneOffsetMap[getHash] = IBone{ getHash };
#else
			boneOffsetMap[getHash] = IBone{};
#endif
			boneOffsetMap[getHash].index = n;
			zg::Matrix44& mat = boneOffsetMap[getHash].offset;
			fgets(buf, bufSz, file);
			sscanf(buf, "%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f"
				, &mat.m[0], &mat.m[1], &mat.m[2], &mat.m[3]
				, &mat.m[4], &mat.m[5], &mat.m[6], &mat.m[7]
				, &mat.m[8], &mat.m[9], &mat.m[10], &mat.m[11]
				, &mat.m[12], &mat.m[13], &mat.m[14], &mat.m[15]
			);
		}

		fclose(file);
	}

	//printf("Anims: %d, ChannelMap: %d\n", animations.size(), channelMap.size());

	if (animations.size())
		hasAnims = true;
}

void Model::ReadNode(FILE* file, INode* n, char* b, unsigned int sz)
{
	//N START
	if (b[2] == 'S')
	{
		const int bufSz = 1024;
		char str[bufSz];
		ZeroMemory(str, bufSz);
		zg::Matrix44 mat;

		//Name
		fgets(b, sz, file);
		std::string fullname{ b };
		fullname.erase(fullname.end() - 1);
		//sscanf_s(b, "\t%s", str, bufSz);

		//Matrix 4x4
		fgets(b, sz, file);
		sscanf(b, "%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f"
			, &mat.m[0], &mat.m[1], &mat.m[2], &mat.m[3]
			, &mat.m[4], &mat.m[5], &mat.m[6], &mat.m[7]
			, &mat.m[8], &mat.m[9], &mat.m[10], &mat.m[11]
			, &mat.m[12], &mat.m[13], &mat.m[14], &mat.m[15]
		);

		//Empty Line
		//fgets(b, sz, file);
#if defined(_DEBUG)
		n->myname = fullname;
#endif
		n->name = GetHashFromModelName(fullname);
		n->matrix = mat;

		fgets(b, sz, file);

		//N START
		if (b[2] == 'S')
		{
			n->childNodes.push_back(INode{});
			n->childNodes.back().parent = n;

			ReadNode(file, &n->childNodes.back(), b, sz);
		}
	}

	//Add each child
	while (b[2] == 'S')
	{
		n->childNodes.push_back(INode{});
		n->childNodes.back().parent = n;

		ReadNode(file, &n->childNodes.back(), b, sz);
	}

	fgets(b, sz, file);
}

void Model::Init()
{
	// create buffers/arrays
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	// load data into vertex buffers
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(IVertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	// set the vertex attribute pointers
	// vertex Positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(IVertex), (void*)0);
	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(IVertex), (void*)offsetof(IVertex, normal));
	// vertex texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(IVertex), (void*)offsetof(IVertex, uv));
	// weights
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(IVertex), (void*)offsetof(IVertex, weights));
	// joint IDs
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(IVertex), (void*)offsetof(IVertex, boneIndex));
	// tangents
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(IVertex), (void*)offsetof(IVertex, tangent));

	glBindVertexArray(0);

	//Print();
}

void Model::Print() {
	iNode.Print();
}
std::vector<IVertex> Model::GetListOfVertices() const {
	return vertices;
}

bool Model::IsAnimationExist(const char* animationTitle_) const {
	return IsAnimationExist(GetHashFromModelName(animationTitle_));
}
bool Model::IsAnimationExist(const std::string& animationTitle_) const {
	return IsAnimationExist(GetHashFromModelName(animationTitle_.c_str()));
}
bool Model::IsAnimationExist(size_t hashAnimationTitle_) const {
	return animations.find(hashAnimationTitle_) != animations.end();
}

void Model::Draw()
{
	glBindVertexArray(VAO);
	size_t getsz = meshes.size();
	for (unsigned i = 0; i < getsz; ++i)
		glDrawElementsBaseVertex(GL_TRIANGLES, meshes[i].IndiceCount, GL_UNSIGNED_INT, (void*)(sizeof(unsigned int) * meshes[i].BaseIndex), meshes[i].BaseVertex);
	glBindVertexArray(0);
}



//void Model::FindParentBones(INode* node)
//{
//	auto name = GetModelNameFromHash(node->name);
//	auto idx = name.find("_$");
//	if (idx != std::string::npos)
//	{
//		name[idx] = '\0';
//		parentBones[node->name] = GetHashFromModelName(name);
//	}
//}

void Model::ProcessMesh(unsigned meshID, aiMesh* mesh)
{
	//Process Each vertices
	for (auto i = 0u; i < mesh->mNumVertices; ++i)
	{
		IVertex v;
		//local coordinates of VERTEX of current mesh[meshID] in scene
		v.position.x = mesh->mVertices[i].x;
		v.position.y = mesh->mVertices[i].y;
		v.position.z = mesh->mVertices[i].z;

		//local NORMAL of current mesh[meshID] in scene
		v.normal.x = mesh->mNormals[i].x;
		v.normal.y = mesh->mNormals[i].y;
		v.normal.z = mesh->mNormals[i].z;

		//local TANGENTS of current mesh[meshID] in scene
		v.tangent.x = mesh->mTangents[i].x;
		v.tangent.y = mesh->mTangents[i].y;
		v.tangent.z = mesh->mTangents[i].z;

		//UV of current mesh[meshID] in scene
		if (mesh->HasTextureCoords(0))
		{
			v.uv.x = mesh->mTextureCoords[0][i].x;
			v.uv.y = mesh->mTextureCoords[0][i].y;
		}
		else
			v.uv = zg::Vector2{};

		v.weights = zg::Vector4{};
		v.boneIndex = zg::Vector4{};

		vertices.push_back(v);
	}

	//BONES of current mesh[meshID] in scene
	ProcessBones(meshID, mesh);
	//boneTransforms.resize(boneOffsets.size());

	//Process Each set of Indicies
	for (unsigned i = 0; i < mesh->mNumFaces; ++i)
	{
		const aiFace& face = mesh->mFaces[i];
		indices.push_back(face.mIndices[0]);
		indices.push_back(face.mIndices[1]);
		indices.push_back(face.mIndices[2]);
	}
}

void Model::ProcessBones(unsigned meshID, aiMesh* mesh)
{
	for (unsigned int i = 0; i < mesh->mNumBones; ++i)
	{
		unsigned int boneIndex = 0;
		//Get bone name
		std::string boneName{ mesh->mBones[i]->mName.data };
		size_t getHash = GetHashFromModelName(boneName);

		//printf("mesh[%d]: %s \n", meshID, boneName.c_str());
//NEW 
		if (boneOffsetMap.find(getHash) == boneOffsetMap.end())
		{
			boneIndex = boneCount;
			++boneCount;
#if defined(_DEBUG)
			boneOffsetMap[getHash] = IBone{ getHash };
#else
			boneOffsetMap[getHash] = IBone{};
#endif
			IBone& getiBone = boneOffsetMap[getHash];
			getiBone.index = boneIndex;
			getiBone.offset = zg::Matrix44{
				mesh->mBones[i]->mOffsetMatrix.a1, mesh->mBones[i]->mOffsetMatrix.a2, mesh->mBones[i]->mOffsetMatrix.a3, mesh->mBones[i]->mOffsetMatrix.a4,
				mesh->mBones[i]->mOffsetMatrix.b1, mesh->mBones[i]->mOffsetMatrix.b2, mesh->mBones[i]->mOffsetMatrix.b3, mesh->mBones[i]->mOffsetMatrix.b4,
				mesh->mBones[i]->mOffsetMatrix.c1, mesh->mBones[i]->mOffsetMatrix.c2, mesh->mBones[i]->mOffsetMatrix.c3, mesh->mBones[i]->mOffsetMatrix.c4,
				mesh->mBones[i]->mOffsetMatrix.d1, mesh->mBones[i]->mOffsetMatrix.d2, mesh->mBones[i]->mOffsetMatrix.d3, mesh->mBones[i]->mOffsetMatrix.d4
			};
		}
		else
			boneIndex = boneOffsetMap[getHash].index;
		

		for (unsigned j = 0; j < mesh->mBones[i]->mNumWeights; ++j)
		{
			//Get the Real vertex Index of this model
			unsigned vertexID = meshes[meshID].BaseVertex + mesh->mBones[i]->mWeights[j].mVertexId;

			//Get the Vertex of this Model that have been init just now
			IVertex& vert = vertices[vertexID];
			float weight = mesh->mBones[i]->mWeights[j].mWeight;

			//Current Vertex is Affected by 4 boneIndex only 
			for (unsigned k = 0; k < 4; ++k)
			{
				if (weight > vert.weights[k] || vert.weights[k] == 0.0f)
				{
					vert.boneIndex[k]  = static_cast<float>(boneIndex);	//storing boneIndex
					vert.weights[k] = weight;							//storing the weight 
					break;
				}
			}
		}
	}
}

void Model::ProcessNode(aiNode* ainode_, const aiScene* scene, INode* iNode_)
{
	iNode_->name = GetHashFromModelName(ainode_->mName.C_Str());
	//printf("Node: %s \n", ainode_->mName.C_Str());
	aiMatrix4x4 tmpp = ainode_->mTransformation;
	iNode_->matrix = zg::Matrix44 {
		tmpp.a1, tmpp.a2, tmpp.a3, tmpp.a4,
		tmpp.b1, tmpp.b2, tmpp.b3, tmpp.b4,
		tmpp.c1, tmpp.c2, tmpp.c3, tmpp.c4,
		tmpp.d1, tmpp.d2, tmpp.d3, tmpp.d4
	};

	for (unsigned i = 0; i < ainode_->mNumChildren; ++i)
	{
		iNode_->childNodes.push_back(INode{});
		iNode_->childNodes.back().parent = iNode_;
		ProcessNode(ainode_->mChildren[i], scene, &iNode_->childNodes[i] );
	}
}

bool Model::LoadFBXModel(const std::string& pathname_)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(pathname_, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices | aiProcess_CalcTangentSpace);
	
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ASSIMP Error: " << importer.GetErrorString() << std::endl;
		return false;
	}
	//GlobalInverseTransform = scene->mRootNode->mTransformation.Inverse();

	meshes.resize(scene->mNumMeshes);						//Get total Number Of Meshes In the scene
	//printf("Number Of Meshes: %d\n", scene->mNumMeshes);
	
	unsigned numVertices = 0;	//Get total Number Of Vertex
	unsigned numIndices = 0;	//Get total Number Of Indices
	
	for (unsigned i = 0; i < scene->mNumMeshes; ++i)
	{
		meshes[i].IndiceCount = scene->mMeshes[i]->mNumFaces * 3;
		meshes[i].BaseVertex = numVertices;
		meshes[i].BaseIndex = numIndices;
	
		numVertices += scene->mMeshes[i]->mNumVertices;
		numIndices += meshes[i].IndiceCount;
	}
	
	vertices.reserve(numVertices);		//Total Number Of Vertices in the model
	indices.reserve(numIndices);		//Total Number Of Indices in the model
	
	for (unsigned i = 0; i < meshes.size(); ++i)
	{
		aiMesh* mesh = scene->mMeshes[i];
		//std::string aaaa{ mesh->mName.data };
		//printf("Name Of Mesh[%d]: %s \n", i, aaaa.c_str());
		ProcessMesh(i, mesh);
	}
	
	//Process Root INode
	ProcessNode(scene->mRootNode, scene, &iNode);
	
	//Port number of animation
	for (unsigned int i = 0; i < scene->mNumAnimations; ++i)
	{
		//Get Name of the Animation
		size_t getanimationTitle = GetHashFromModelName(scene->mAnimations[i]->mName.C_Str());	

		//Init Animation struct
		animations[getanimationTitle] = Animation { 
			getanimationTitle
			, (float)scene->mAnimations[i]->mDuration
			, (float)scene->mAnimations[i]->mTicksPerSecond 
		};

		//Get current animation 
		Animation& currAnimation = animations[getanimationTitle];

		double maxDuration = 0;

		//Number of Channels/Bone affected
		for (unsigned j = 0; j < scene->mAnimations[i]->mNumChannels; ++j)
		{
			//Node Name Affected
			//Get Bone name
			size_t getBoneHashName = GetHashFromModelName(scene->mAnimations[i]->mChannels[j]->mNodeName.C_Str());
			//Init BoneChannel
			currAnimation.listOfBones[getBoneHashName] = BoneAnimationChannel{ getBoneHashName };
			//currAnimation.listOfBones[getBoneHashName].boneName = getBoneHashName;

			//Get Current BoneChannel 
			BoneAnimationChannel& currentBoneChannel = currAnimation.listOfBones[getBoneHashName];
			
			//Slot in all Current BoneAnimationChannel Position Key
			currentBoneChannel.numOfpositionKey = scene->mAnimations[i]->mChannels[j]->mNumPositionKeys;
			for (unsigned k = 0; k < currentBoneChannel.numOfpositionKey; ++k)
			{
				const aiVectorKey& key = scene->mAnimations[i]->mChannels[j]->mPositionKeys[k];
				currentBoneChannel.positionKeys.emplace_back(Vector3Key{ (float)key.mTime, zg::Vector3{ key.mValue.x,key.mValue.y, key.mValue.z} });
				//if (key.mTime > maxDuration) maxDuration = key.mTime;
			}

			//Slot in all Current BoneAnimationChannel Scale Key
			currentBoneChannel.numOfscaleKey = scene->mAnimations[i]->mChannels[j]->mNumScalingKeys;
			for (unsigned k = 0; k < currentBoneChannel.numOfscaleKey; ++k)
			{
				const aiVectorKey& key = scene->mAnimations[i]->mChannels[j]->mScalingKeys[k];
				currentBoneChannel.scaleKeys.emplace_back(Vector3Key{ (float)key.mTime, zg::Vector3{ key.mValue.x,key.mValue.y, key.mValue.z } });
				//if (key.mTime > maxDuration) maxDuration = key.mTime;
			}
	
			//Slot in all Current BoneAnimationChannel Rotation Key
			currentBoneChannel.numOfrotationKey = scene->mAnimations[i]->mChannels[j]->mNumRotationKeys;
			for (unsigned k = 0; k < currentBoneChannel.numOfrotationKey; ++k)
			{
				const aiQuatKey& key = scene->mAnimations[i]->mChannels[j]->mRotationKeys[k];
				currentBoneChannel.rotationKeys.emplace_back(QuaternionKey{ (float)key.mTime, zg::Quaternion{ key.mValue.x,key.mValue.y, key.mValue.z, key.mValue.w } });
				//if (key.mTime > maxDuration) maxDuration = key.mTime;
			}
			maxDuration = MAX3(currentBoneChannel.numOfpositionKey, currentBoneChannel.numOfscaleKey, currentBoneChannel.numOfrotationKey);
			//printf("Pos:%d SCA:%d Rot%d \n", currAnimation.channels[j].posKeyCount, currAnimation.channels[j].scaleKeyCount, currAnimation.channels[j].rotKeyCount);
		}

		//Get the Max Total Duration/Frame of each Animation
		currAnimation.duration = static_cast<float>(maxDuration);
	}

	if (animations.size())
		hasAnims = true;

	return true;
}

void Model::ExportFile(const std::string& fileOut)
{
	std::fstream outFile;
	outFile.open(fileOut + filename, std::ios::out);

	outFile << "v2" << std::endl;
	//outFile << extra_calefare_scalingmatrix.m0 << " " << meshes.size() << std::endl;
	outFile << scalingFactor << " " << meshes.size() << std::endl;

	for (const auto& mesh : meshes)
		outFile << mesh.IndiceCount << " " << mesh.BaseVertex << " " << mesh.BaseIndex << " " << std::endl;

	for (const auto& vert : vertices)
	{
		outFile << "\tV " << vert.position.x << " " << vert.position.y << " " << vert.position.z
			    << " N " << vert.normal.x << " " << vert.normal.y << " " << vert.normal.z
			    << " U " << vert.uv.x << " " << vert.uv.y
			    << " W " << vert.weights.x << " " << vert.weights.y << " " << vert.weights.z << " " << vert.weights.w
			    << " J " << vert.boneIndex.x << " " << vert.boneIndex.y << " " << vert.boneIndex.z << " " << vert.boneIndex.w
			    << " T " << vert.tangent.x << " " << vert.tangent.y << " " << vert.tangent.z << std::endl;
	}

	size_t tmpsz = indices.size();
	outFile << "I " << tmpsz << std::endl;
	for (size_t i = 2; i < tmpsz; i += 3) {

		outFile << indices[i - 2] << " " << indices[i - 1] << " " << indices[i] << std::endl;
	}

	ExportNodes(outFile, &iNode);

	for (const auto& anim : animations)
	{
		outFile << "A START " << anim.second.duration << " " << anim.second.ticksPerSecond << " " << GetModelNameFromHash(anim.first) << std::endl;

		//outFile << "\t" << anim.name << std::endl;
		//outFile << "\t" << anim.duration << std::endl;
		//outFile << "\t" << anim.ticksPerSecond << std::endl;

		for (const auto& chn : anim.second.listOfBones)
		{
			outFile << GetModelNameFromHash(chn.first) << std::endl;

			outFile << "PK " << chn.second.positionKeys.size() << std::endl;
			for (const auto& key : chn.second.positionKeys)
			{
				outFile << "\t" << key.time << " " << key.value.x << " " << key.value.y << " " << key.value.z << std::endl;
			}

			outFile << "SK " << chn.second.scaleKeys.size() << std::endl;
			for (const auto& key : chn.second.scaleKeys)
			{
				outFile << "\t" << key.time << " " << key.value.x << " " << key.value.y << " " << key.value.z << std::endl;
			}

			outFile << "RK " << chn.second.rotationKeys.size() << std::endl;
			for (const auto& key : chn.second.rotationKeys)
			{
				outFile << "\t" << key.time << " " << key.value.x << " " << key.value.y << " " << key.value.z << " " << key.value.w << std::endl;
			}
		}

		outFile << "A END" << std::endl;
	}

	outFile << "B " << boneCount << std::endl;
	for (const auto& elem : boneOffsetMap)
	{
		const zg::Matrix44& bo = elem.second.offset;
		outFile << elem.second.index << " " << GetModelNameFromHash(elem.first) << std::endl;
		outFile << bo.m[0] << " " << bo.m[1] << " " << bo.m[2] << " " << bo.m[3] << " "
				<< bo.m[4] << " " << bo.m[5] << " " << bo.m[6] << " " << bo.m[7]  << " "
				<< bo.m[8] << " " << bo.m[9] << " " << bo.m[10]<< " " << bo.m[11] << " "
				<< bo.m[12]<< " " << bo.m[13]<< " " << bo.m[14]<< " " << bo.m[15] << std::endl;
	}

	//for (const auto& bo : boneOffsets)
	//{
	//	outFile << "BO START" << std::endl;
	//
	//	outFile << bo.m[0] << " " << bo.m[1] << " " << bo.m[2] << " " << bo.m[3]  << " "
	//		    << bo.m[4] << " " << bo.m[5] << " " << bo.m[6] << " " << bo.m[7]  << " "
	//		    << bo.m[8] << " " << bo.m[9] << " " << bo.m[10]<< " " << bo.m[11] << " "
	//		    << bo.m[12]<< " " << bo.m[13]<< " " << bo.m[14]<< " " << bo.m[15] << std::endl;
	//	//outFile << "\t" << bo.m[0] << " " << bo.m[4] << " " << bo.m[8] << " " << bo.m[12]
	//	//		<< " " << bo.m[1] << " " << bo.m[5] << " " << bo.m[9] << " " << bo.m[13]
	//	//		<< " " << bo.m[2] << " " << bo.m[6] << " " << bo.m[10] << " " << bo.m[14]
	//	//		<< " " << bo.m[3] << " " << bo.m[7] << " " << bo.m[11] << " " << bo.m[15] << std::endl;
	//	//outFile << "BO END" << std::endl;
	//}
}

void Model::ExportNodes(std::fstream & outFile, const INode* node)
{
	outFile << "N START" << std::endl;
	outFile << GetModelNameFromHash(node->name) << std::endl;
	outFile << node->matrix.m[0] << " " << node->matrix.m[1] << " " << node->matrix.m[2] << " " << node->matrix.m[3]  << " "
			<< node->matrix.m[4] << " " << node->matrix.m[5] << " " << node->matrix.m[6] << " " << node->matrix.m[7]  << " "
			<< node->matrix.m[8] << " " << node->matrix.m[9] << " " << node->matrix.m[10]<< " " << node->matrix.m[11] << " "
			<< node->matrix.m[12]<< " " << node->matrix.m[13]<< " " << node->matrix.m[14]<< " " << node->matrix.m[15] << std::endl;

	for (const auto& child : node->childNodes)
		ExportNodes(outFile, &child);

	outFile << "N END" << std::endl;
}

// ____________________________________________________________ function
//#define USE_RAWNEW 1

void DestroyAllModels() {
#ifdef USE_RAWNEW
	for (auto& elem : MODEL_POOL) DeallocateOne(elem.second);
#else
	for (auto& elem : MODEL_POOL)
		DeallocateMalloc(elem.second);
#endif
}

void LoadModelFromDIR(const char* pathname_)
{
	DIR_itr&& directory = GetDirectory(pathname_);
	for (auto & p : directory) {
		std::string filename;
		bool found = GetFileNameWithExtensionAndCheckExtension(p.path().string(), filename, "models");

		if (found) 
		{
			size_t hashnum = GenerateHash_ModelName(filename);
#ifdef USE_RAWNEW
			Model* model = Allocate<Model>(1);
			model->filename = filename;
#else
			Model* model = AllocateMalloc<Model>(1);
			new (model) Model{ filename };
#endif
			model->LoadCustomModel(p.path().string());
			MODEL_POOL[hashnum] = model;
		}
	}

	for (auto& mesh : MODEL_POOL)
		mesh.second->Init();
}

void LoadFBXModelAndConvertFromDIR(const char* fbx_pathname_, const char* models_pathname_) 
{
	DIR_itr&& directory = GetDirectory(models_pathname_);
	for (auto & p : directory) {
		std::string filename;
		bool found = GetFileNameWithExtensionAndCheckExtension(p.path().string(), filename, "models");

		if (found) {
			size_t hashnum = GenerateHash_ModelName(filename);

#ifdef USE_RAWNEW
			Model* model = Allocate<Model>(1);
			model->filename = filename;
#else
			Model* model = AllocateMalloc<Model>(1);
			new (model) Model{ filename };
#endif
			model->LoadCustomModel(p.path().string());
			MODEL_POOL[hashnum] = model;
		}
	}

	DIR_itr&& directory2 = GetDirectory(fbx_pathname_);
	for (auto& p : directory2) {
		std::string filename;

		bool found = GetFileNameWithExtensionAndCheckExtension(p.path().string(), filename, "fbx");
		if (found) {
			GetFileNameNoExtension(p.path().string(), filename);
			filename += ".models";
			size_t hashnum = GenerateHash_ModelName(filename);
			bool findifModelExist = MODEL_POOL.find(hashnum) == MODEL_POOL.end();
			if (findifModelExist) 
			{
#ifdef USE_RAWNEW
				Model* model = Allocate<Model>(1);
				model->filename = filename;
				bool loaded = model->LoadFBXModel(p.path().string());
				if (loaded)
				{
					model->ExportFile(models_pathname_);
					MODEL_POOL[hashnum] = model;
				}
				else 
				{
					DeallocateOne(model);
				}

#else
				Model* model = AllocateMalloc<Model>(1);
				new (model) Model{ filename };
				bool loaded = model->LoadFBXModel(p.path().string());
				if (loaded)
				{
					model->ExportFile(models_pathname_);
					MODEL_POOL[hashnum] = model;
				}
				else
				{
					DeallocateMalloc(model);
				}
#endif
			}
		}
	}

//	DIR_itr&& directory = GetDirectory(models_pathname_);
//	for (auto & p : directory) {
//		std::string filename;
//		bool found = GetFileNameWithExtensionAndCheckExtension(p.path().string(), filename, "models");
//
//		if (found) {
//			size_t hashnum = GenerateHash_ModelName(filename);
//			Model* model = Allocate<Model>(1);
//
//#ifdef USE_RAWNEW
//			model->filename = filename;
//			model->LoadCustomModel(p.path().string());
//			MODEL_POOL[hashnum] = model;
//#else
//			new (get) Model{ filename };
//			get->LoadCustomModel(p.path().string());
//			MODEL_POOL[hashnum] = get;
//#endif
//		}
//	}

	for (auto& mesh : MODEL_POOL)
		mesh.second->Init();
}

Model*& FindModel(std::string&& filename_extension_) {
	return FindModel(GetHashFromModelName(filename_extension_));
}
Model*& FindModel(const std::string& filename_extension_) {
	return FindModel(GetHashFromModelName(filename_extension_));
}
Model *& FindModel(size_t hashnum) {
	auto find = MODEL_POOL.find(hashnum);
	bool found = find == MODEL_POOL.end();

#if defined(_DEBUG) || defined(_EDITOR_ON)
	if (found)
		PopUp_Warn(std::string{ "Models file Not Found: " + GetModelNameFromHash(hashnum) }.c_str());
	return found ? MODEL_POOL[GetHashFromModelName("null.models")] : find->second;
#else
	return SHADER_POOL[hashnum];
#endif
}

const std::map<size_t, Model*>& GetAllModel() {
	return MODEL_POOL;
}

std::vector<std::string> GetAllModelName() {
	std::vector<std::string> allname;
	for (auto& elem : MODEL_POOL) {
		allname.push_back(GetModelNameFromHash(elem.first));
	}
	return allname;
}

//Hashing
size_t GenerateHash_ModelName(const char* model_names_bones_) {
	static std::hash<std::string> hasher;
	size_t hashedString = hasher(model_names_bones_);						// Compute the hash for the string
	HASHSTRING_MODEL_NAMES.emplace(hashedString, model_names_bones_);
	return hashedString;
}
size_t GenerateHash_ModelName(const std::string& model_names_bones_) {
	return GenerateHash_ModelName(model_names_bones_.c_str());
}

size_t GetHashFromModelName(const char* model_names_bones_) {
	static std::hash<std::string> hasher;
	size_t hashedString = hasher(model_names_bones_);	// Compute the hash for the string

														// Search lookup table for the hashed string
	auto it = HASHSTRING_MODEL_NAMES.find(hashedString);

	// If the hashedString is not registered
	if (it == HASHSTRING_MODEL_NAMES.end())
		HASHSTRING_MODEL_NAMES.emplace(hashedString, model_names_bones_);

	return hashedString;
}
size_t GetHashFromModelName(const std::string& model_names_bones_) {
	return GetHashFromModelName(model_names_bones_.c_str());
}

std::string GetModelNameFromHash(size_t hash_) {
#if defined(_DEBUG) || defined(_EDITOR_ON)
	auto it = HASHSTRING_MODEL_NAMES.find(hash_);
	// If it is not found, return the empty string
	if (it == HASHSTRING_MODEL_NAMES.end())
		ErrorMsg("Hash number %d not found", hash_);
	return it->second;
#else
	return HASHSTRING_MODEL_NAMES[hash_];
#endif
}