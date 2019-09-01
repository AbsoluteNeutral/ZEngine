/*****************************************************************************/
/*!
\file			Texture2D.h
\brief			OpenGL Wrap Generate Texture mapping
*/
/*****************************************************************************/
#ifndef _TEXTURE2D_H
#define _TEXTURE2D_H

#include <string>
#include "HashString.h"

//#ifndef MAKEFOURCC
//	#define MAKEFOURCC(ch0, ch1, ch2, ch3) \
//					   (GLuint)( \
//					   (((GLuint)(GLubyte)(ch3) << 24) & 0xFF000000) | \
//					   (((GLuint)(GLubyte)(ch2) << 16) & 0x00FF0000) | \
//					   (((GLuint)(GLubyte)(ch1) <<  8) & 0x0000FF00) | \
//					    ((GLuint)(GLubyte)(ch0)        & 0x000000FF) )
//#endif // !MAKEFOURCC

#define FOURCC_DXT1 MAKEFOURCC('D', 'X', 'T', '1')
#define FOURCC_DXT3 MAKEFOURCC('D', 'X', 'T', '3')
#define FOURCC_DXT5 MAKEFOURCC('D', 'X', 'T', '5')

enum ZG_API TextureMode {
	TEXTURE_CLAMP = 0,
	TEXTURE_REPEAT
};

class ZG_API Texture2D {

public:
	Texture2D() noexcept;
	Texture2D(const std::string& filename);
	~Texture2D();

	int w, h, chnl;
	unsigned int id;
	unsigned int textureMode;
	std::string filename;
	bool Load(const std::string& path_filename, unsigned int textureMode = TEXTURE_REPEAT);
	bool Load(unsigned char * data, unsigned width, unsigned height, unsigned int textureMode = TEXTURE_REPEAT);
private:
	unsigned int texure_Mode;
};


// Functions
ZG_API void DestroyAllTextures();						//internal use only
ZG_API void LoadTexturesFromDIR(const char* pathname_);	//internal use only


//ZG_API Texture2D*&							LoadTexture			(const std::string& pathname_filename_extension_);

ZG_API Texture2D*&							FindTexture			(std::string&& filename_extension_);
ZG_API Texture2D*&							FindTexture			(const std::string& filename_extension_);
ZG_API Texture2D*&							FindTexture			(size_t hashedName);
ZG_API const std::map<size_t, Texture2D*>&	GetAllTexture2D		();
ZG_API std::vector<std::string>				GetAllTexture2DName	();

#endif //_TEXTURE2D_H
