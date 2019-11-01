
#ifndef _ISHADER_H
#define _ISHADER_H

#include <string>
#include <map>
#include "ZGMath.h"

extern const char* TEXTURE_TYPE_NAME[12];
enum TEXTURE_TYPE_ENUM {
	TEXTURE_0 = 0,
	TEXTURE_1,
	TEXTURE_2,
	TEXTURE_3,
	TEXTURE_4,
	TEXTURE_5
};

enum ShaderTypeInfo
{
  UNIFORM_FLOAT,
  UNIFORM_FLOAT_VEC2,
  UNIFORM_FLOAT_VEC3,
  UNIFORM_FLOAT_VEC4,
  UNIFORM_INT,
  UNIFORM_BOOL
};

class ZG_API IShader
{
public:
	IShader		() noexcept;
	IShader		(const std::string& filename_);
	~IShader	();

	bool LoadShaders(const std::string& pathname_filename_);
	//std::map<unsigned, ShaderTypeInfo> uniforms;

	void Use();
	void End();
	void SendMat4	(const char* name_, float* value_)			const;
	void SendMat4	(const char* name_, zg::Matrix44 value_)		const;
	void SendMat4	(const unsigned int loc_, zg::Matrix44 value_)	const;
	void SendBool	(const char* name_, const bool& value_)		const;
	void SendFloat	(const char* name_, const float& value_)	const;
	void SendInt	(const char* name_, const int& value_)		const;
	void SendVec2	(const char* name_, const zg::Vector2& vec_)	const;
	void SendVec3	(const char* name_, const zg::Vector3& vec_)	const;
	void SendVec4	(const char* name_, const zg::Vector4& vec_)	const;
	void SendVec4	(const char* name_, const zg::Color& vec4_)		const;

	std::string filename;
	unsigned programID;		//main shader program Id
private:
	//unsigned vertID;
	//unsigned fragID;
};

//function
ZG_API void DestroyAllShaders();						//internal use only
ZG_API void LoadShadersFromDIR(const char* pathname_);	//internal use only

ZG_API IShader*& FindShader		(std::string&& filename_extension_);
ZG_API IShader*& FindShader		(const std::string& filename_extension_);
ZG_API IShader*& FindShader		(size_t hashedName);

ZG_API const std::map<size_t, IShader*>&	GetAllIShader();
ZG_API const std::vector<std::string>		GetAllIShaderName();

#endif //_SHADER_H
