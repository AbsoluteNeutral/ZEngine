/*****************************************************************************/
/*!
\file			Texture2D.cpp
\brief			OpenGL Wrap Generate Texture mapping
*/
/*****************************************************************************/
#include "stdafx.h"
#include "Texture2D.h"
#include "Logging.h"
#include <map>

#include "./GL/glew.h"

#define STB_IMAGE_IMPLEMENTATION
#include "./stb/stb_image.h"

static std::map<size_t, Texture2D*> TEXTURE_POOL;

Texture2D::Texture2D() noexcept
	:w(1), h(1), chnl(4), id(0), textureMode(0), texure_Mode(0)
{}

Texture2D::Texture2D(const std::string& filename)
	: w(1), h(1), chnl(4), id(0), textureMode(0), filename{ filename }, texure_Mode(0)
{}

bool Texture2D::Load(const std::string& path_filename, unsigned int textureMode_) 
{
	std::string&& extensionName{ GetFileExtension(path_filename) };
	//Load DDS
	if (strcmp(extensionName.c_str(), "dds") == 0){
		unsigned char header[124];
		ZeroMemory(header, 124);
		FILE *fp;
		/* try to open the file */
		fp = fopen(path_filename.c_str(), "rb");
		if (fp == NULL) {
			ErrorMsg(std::string{ "Unable to Open" + path_filename }.c_str());
			return false;
		}

		/* verify the type of file */
		char filecode[4]{ 0, 0, 0, 0 };
		fread(filecode, 1, 4, fp);
		if (strncmp(filecode, "DDS ", 4) != 0) {
			fclose(fp);
			ErrorMsg(std::string{ "is .dds extension but NOT DDS format" + path_filename }.c_str());
			return false;
		}

		/* get the surface desc */
		fread(&header, 124, 1, fp);

		unsigned int height		 = *(unsigned int*)&(header[8]);
		unsigned int width		 = *(unsigned int*)&(header[12]);
		unsigned int linearSize	 = *(unsigned int*)&(header[16]);
		unsigned int mipMapCount = *(unsigned int*)&(header[24]);
		unsigned int fourCC		 = *(unsigned int*)&(header[80]);

		unsigned char *buffer = nullptr;
		unsigned int bufsize;
		/* how big is it going to be including all mipmaps? */
		bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize;
		buffer = (unsigned char*)malloc(bufsize * sizeof(unsigned char));
		fread(buffer, 1, bufsize, fp);
		/* close the file pointer */
		fclose(fp);

		//unsigned int components = (fourCC == FOURCC_DXT1) ? 3 : 4;
		unsigned int format;
		switch (fourCC)
		{
		case FOURCC_DXT1:
			format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
			break;
		case FOURCC_DXT3:
			format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
			break;
		case FOURCC_DXT5:
			format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
			break;
		default:
			free(buffer);
			return false;
		}

		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);

    static std::string fname;
    GetFileNameWithExtension(path_filename.c_str(), fname);
    
    if (fname[0] == 'S' && fname[1] == 'B')
    {
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }
    else
    {
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
		unsigned int offset = 0;

		/* load the mipmaps */
		for (unsigned int level = 0; level < mipMapCount && (width || height); ++level){
			unsigned int size = ((width + 3) / 4)*((height + 3) / 4)*blockSize;
			glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height, 0, size, buffer + offset);

			offset += size;
			width /= 2;
			height /= 2;
		}
		free(buffer);
		return true;
	}
	//Load jpg, png, tga
	else 
	{
		unsigned char* data = nullptr;
		data = stbi_load(path_filename.c_str(), &w, &h, &chnl, 4);
		stbi_set_flip_vertically_on_load(false);
		if (!data) 
			return false;
		//ErrorIf(!data, std::string{ path_filename + " Texture File Not Found" }.c_str());

		textureMode = textureMode_;
		
		switch (textureMode)
		{
		case TEXTURE_CLAMP:
			texure_Mode = GL_CLAMP_TO_EDGE;
		case TEXTURE_REPEAT:
			texure_Mode = GL_REPEAT;
		}

		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, texure_Mode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, texure_Mode);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h,
								    0, GL_RGBA, GL_UNSIGNED_BYTE,
								    data);
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
		stbi_image_free(data);
		return true;
	}
	return false;
}

bool Texture2D::Load(unsigned char * data, unsigned width, unsigned height,  unsigned int textureMode_)
{
	stbi_set_flip_vertically_on_load(false);
	if (!data) return false;
	textureMode = textureMode_;
	
	switch (textureMode)
	{
	case TEXTURE_CLAMP:
	  texure_Mode = GL_CLAMP;
	case TEXTURE_REPEAT:
	  texure_Mode = GL_REPEAT;
	}
	
	w = width;
	h = height;
	
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, texure_Mode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, texure_Mode);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h,
	  0, GL_BGR_EXT, GL_UNSIGNED_BYTE,
	  data);
	//glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	return true;
}

Texture2D::~Texture2D(){
	glDeleteTextures(1, &id);
}

// ____________________________________________________________ Extern Functions
//#define USE_RAWNEW 1

void DestroyAllTextures() {
#ifdef USE_RAWNEW
	for (auto& elem : TEXTURE_POOL) DeallocateOne(elem.second);
#else
	for (auto& elem : TEXTURE_POOL)
		DeallocateMalloc(elem.second);
#endif
}

void LoadTexturesFromDIR(const char* pathname_) {
	DIR_itr&& directory = GetDirectory(pathname_);
	for (auto & p : directory) {
		std::string filename;
		GetFileNameWithExtension(p.path().string(), filename);

		size_t hashnum = GenerateHash(filename);
		auto find = TEXTURE_POOL.find(hashnum);
#ifdef USE_RAWNEW
		Texture2D* get = Allocate<Texture2D>(1);
		get->filename = filename;
		if (get->Load(p.path().string())) {
			TEXTURE_POOL[hashnum] = get;
		}
		else DeallocateOne(get);
#else
		Texture2D* get = AllocateMalloc<Texture2D>(1);
		new (get) Texture2D{ filename };
		get->filename = filename;
		if (get->Load(p.path().string())) {
			TEXTURE_POOL[hashnum] = get;
		}
		else DeallocateMalloc(get);
#endif
	}
}

Texture2D*& FindTexture(const std::string& filename_extension_) {
	return FindTexture(GetHashFromString(filename_extension_));
}
Texture2D*& FindTexture(std::string&& filename_extension_) {
	return FindTexture(GetHashFromString(filename_extension_));
}
Texture2D*& FindTexture(size_t hashnum) {
	auto find = TEXTURE_POOL.find(hashnum);
	bool found = find == TEXTURE_POOL.end();

#if defined(_DEBUG) || defined(_EDITOR_ON)
	if (found)
		PopUp_Warn(std::string{ "Texture file Not Found: " + GetStringFromHash(hashnum) }.c_str());
	return found ? TEXTURE_POOL[GetHashFromString("null.dds")] : find->second;
#else
	return TEXTURE_POOL[hashnum];
#endif
}

const std::map<size_t, Texture2D*>& GetAllTexture2D() {
	return TEXTURE_POOL;
}

std::vector<std::string> GetAllTexture2DName() {
	std::vector<std::string> allname;
	for (auto& elem : TEXTURE_POOL) {
		allname.push_back(GetStringFromHash(elem.first));
	}
	return allname;
}
