/*****************************************************************************/
/*!
\file			IShader.cpp
\brief			OpenGLSL create shader Wrap
*/
/*****************************************************************************/
#include "stdafx.h"
#include "IShader.h"
#include "./GL/glew.h"

static std::map<size_t, IShader*> SHADER_POOL;	//internal use only

const char* TEXTURE_TYPE_NAME[12] = {
	"diffuseMap",
	"specularMap",
	"normalMap",
	"emissiveMap",
	"texture_4",
	"texture_5",
	"texture_6",
	"texture_7",
	"texture_8",
	"texture_9",
	"texture_10",
	"texture_11",
};

std::string readShader(const char* filename) {
	std::ifstream inp(filename);
	if (!inp.good()) {
		ErrorMsg("Failed to read %s", filename);
		return std::string{};
	}
	return std::string{ std::istreambuf_iterator<char>(inp),
						std::istreambuf_iterator<char>() };
}

IShader::IShader() noexcept
	:filename{}
	, programID(0)
	//, vertID(0)
	//, fragID(0)
{}

IShader::IShader(const std::string& filename_)
	: filename{ filename_ }
	, programID(0)
	//, vertID(0)
	//, fragID(0)
{}

IShader::~IShader() {
	glDeleteProgram(programID);
}

bool IShader::LoadShaders(const std::string& pathname_filename_)
{
	const GLsizei sizeOfLog = 1024;
	int checkError = 0;
	std::string&& source ="#version 400\n#define VS\n" + readShader(pathname_filename_.c_str());
	const char* ad1 = source.c_str();
	unsigned vertID = 0;
	unsigned fragID = 0;

	//Create Vertex
	vertID = glCreateShader(GL_VERTEX_SHADER);		//creates vertex shader
	glShaderSource(vertID, 1, &ad1, NULL);
	glCompileShader(vertID);
	glGetShaderiv(vertID, GL_COMPILE_STATUS, &checkError);	//verifies compilation
	if (!checkError) {
		GLchar infoLog[sizeOfLog];
		ZeroMemory(infoLog, sizeOfLog);
		glGetShaderInfoLog(vertID, sizeOfLog, NULL, infoLog);
		printf("%s \n", pathname_filename_.c_str());
		printf("%s \n", infoLog);
		getchar();
		return false;
	}

	//Create Fragment
	fragID = glCreateShader(GL_FRAGMENT_SHADER);		//creates vertex shader

	source[21] = 'F';
	const char* ad2 = source.c_str();
	glShaderSource(fragID, 1, &ad2, NULL);
	glCompileShader(fragID);
	glGetShaderiv(fragID, GL_COMPILE_STATUS, &checkError);	//verifies compilation
	if (!checkError) {
		GLchar infoLog[sizeOfLog];
		ZeroMemory(infoLog, sizeOfLog);
		glGetShaderInfoLog(fragID, sizeOfLog, NULL, infoLog);
		printf("%s \n", pathname_filename_.c_str());
		printf("%s \n", infoLog);
		getchar();
		return false;
	}

	//Create Program
	programID = glCreateProgram();				//create Shader Program
	if (glGetError() != GL_NO_ERROR) {				//check for error
		printf("Failed glCreateProgram()");			//print error
    getchar();
		return false;
	}

	glAttachShader(programID, vertID);		//attach vertex shader
	glAttachShader(programID, fragID);			//attach fragment shader
	glLinkProgram(programID);						//link program

	GLint result = GL_FALSE;
	int infoLogLength;
	glGetProgramiv(programID, GL_LINK_STATUS, &result);
	glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (infoLogLength > 0) {
		std::vector<char> ProgramErrorMessage(infoLogLength + 1);
		glGetProgramInfoLog(programID, infoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
    getchar();
	}

	glDetachShader(programID, vertID);
	glDetachShader(programID, fragID);
	glDeleteShader(vertID);
	glDeleteShader(fragID);

  //std::fstream file;
  //file.open(pathname_filename_, std::ios::in);

  //static std::map<unsigned, ShaderTypeInfo> types;
  //if (types.empty())
  //{
  //  types[GetHashFromString("float")] = UNIFORM_FLOAT;
  //  types[GetHashFromString("vec2")] = UNIFORM_FLOAT_VEC2;
  //  types[GetHashFromString("vec3")] = UNIFORM_FLOAT_VEC3;
  //  types[GetHashFromString("vec4")] = UNIFORM_FLOAT_VEC4;
  //  types[GetHashFromString("int")] = UNIFORM_INT;
  //  types[GetHashFromString("bool")] = UNIFORM_BOOL;
  //}

  //if (file.is_open())
  //{
  //  const int bufSz = 256;
  //  char buf[bufSz], uType[bufSz], uName[bufSz], uCheck[bufSz];
  //
  //  ZeroMemory(buf, bufSz);
  //  ZeroMemory(uType, bufSz);
  //  ZeroMemory(uName, bufSz);
  //  ZeroMemory(uCheck, bufSz);
  //
  //  while (!file.eof())
  //  {
  //    file.getline(buf, bufSz);
  //
  //    if (buf[0] == 'u')
  //    {
  //      sscanf_s(buf, "uniform %s %s %s", uType, bufSz, uName, bufSz, uCheck, bufSz);
  //
  //      if (uCheck[0] == '\0')
  //      {
  //        uName[strlen(uName) - 1] = '\0';
  //
  //        uniforms[GetHashFromString(uName)] = types[GetHashFromString(uType)];
  //      }
  //      else
  //        ZeroMemory(uCheck, bufSz);
  //    }
  //  }
  //}

  //if (!uniforms.empty())
  //{
  //  printf("Uniforms found in %s:\n", pathname_filename_.c_str());

  //  for (auto& elem : uniforms)
  //  {
  //    switch (elem.second)
  //    {
  //    case UNIFORM_FLOAT:
  //      printf("float ");
  //      break;
  //    case UNIFORM_FLOAT_VEC2:
  //      printf("vec2 ");
  //      break;
  //    case UNIFORM_FLOAT_VEC3:
  //      printf("vec3 ");
  //      break;
  //    case UNIFORM_FLOAT_VEC4:
  //      printf("vec4 ");
  //      break;
  //    case UNIFORM_INT:
  //      printf("int ");
  //      break;
  //    }

  //    printf("%s\n", GetStringFromHash(elem.first).c_str());
  //  }
  //}

	return true;
}

void IShader::Use() { glUseProgram(programID); }
void IShader::End() { glUseProgram(0); }
void IShader::SendMat4(const char* name_, float* value_) const {
	glUniformMatrix4fv(glGetUniformLocation(programID, name_), 1, GL_FALSE, value_);
}
void IShader::SendMat4(const char* name_, zg::Matrix44 value_) const {
	glUniformMatrix4fv(glGetUniformLocation(programID, name_), 1, GL_FALSE, value_);
}
void IShader::SendMat4(const unsigned int loc_, zg::Matrix44 value_) const{
	glUniformMatrix4fv(loc_, 1, GL_FALSE, value_);
}
void IShader::IShader::SendBool(const char* name_, const bool& value_) const {
	glUniform1i(glGetUniformLocation(programID, name_), value_);
}
void IShader::SendFloat(const char* name_, const float& value_) const {
	glUniform1f(glGetUniformLocation(programID, name_), value_);
}
void IShader::SendInt(const char* name_, const int& value_) const {
	glUniform1i(glGetUniformLocation(programID, name_), value_);
}
void IShader::SendVec2(const char* name_, const zg::Vector2& vec_) const {
	glUniform2fv(glGetUniformLocation(programID, name_), 1, &vec_.x);
}
void IShader::SendVec3(const char* name_, const zg::Vector3& vec_) const {
	glUniform3fv(glGetUniformLocation(programID, name_), 1, &vec_.x);
}
void IShader::SendVec4(const char* name_, const zg::Vector4& vec_) const {
	glUniform4fv(glGetUniformLocation(programID, name_), 1, &vec_.x);
}
void IShader::SendVec4(const char* name_, const zg::Color& vec4_) const {
	glUniform4fv(glGetUniformLocation(programID, name_), 1, &vec4_.r);
}

// ____________________________________________________________ function
//#define USE_RAWNEW 1

void DestroyAllShaders() 
{
#ifdef USE_RAWNEW
	for (auto& elem : SHADER_POOL) DeallocateOne(elem.second);
#else
	for (auto& elem : SHADER_POOL) 
		DeallocateMalloc(elem.second);
#endif	
}

void LoadShadersFromDIR(const char* pathname_) {
	DIR_itr&& directory = GetDirectory(pathname_);
	for (auto& p : directory) 
	{
		std::string filename;
		bool found = GetFileNameWithExtensionAndCheckExtension(p.path().string(), filename, "glsl");
		if (found) 
		{
			size_t hashnum = GenerateHash(filename);
#ifdef USE_RAWNEW
			IShader* get = Allocate<IShader>(1);
			get->filename = filename;
#else
			IShader* get = AllocateMalloc<IShader>(1);
			new (get) IShader{ filename };
#endif
			get->LoadShaders(p.path().string());
			SHADER_POOL[hashnum] = get;
		}
	}
}

IShader*& FindShader(std::string&& filename_extension_) {
	return FindShader(GetHashFromString(filename_extension_));
}
IShader*& FindShader(const std::string& filename_extension_) {
	return FindShader(GetHashFromString(filename_extension_));
}
IShader *& FindShader(size_t hashnum) {
	auto find = SHADER_POOL.find(hashnum);
	bool found = find == SHADER_POOL.end();

#if defined(_DEBUG) || defined(_EDITOR_ON)
	if (found)
		PopUp_Warn(std::string{ "Shader file Not Found: " + GetStringFromHash(hashnum) }.c_str());
	return found ? SHADER_POOL[GetHashFromString("null.glsl")] : find->second;
#else
	return SHADER_POOL[hashnum];
#endif
}

const std::map<size_t, IShader*>& GetAllIShader() {
	return SHADER_POOL;
}

const std::vector<std::string> GetAllIShaderName() {
	std::vector<std::string> allname;
	for (auto& elem : SHADER_POOL) {
		allname.push_back(GetStringFromHash(elem.first));
	}
	return allname;
}
