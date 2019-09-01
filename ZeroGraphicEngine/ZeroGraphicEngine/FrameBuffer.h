
#ifndef _FRAMEBUFFER_H
#define _FRAMEBUFFER_H
#include "IShader.h"

class ZG_API FrameBuffer
{
public:
	FrameBuffer() noexcept;
	~FrameBuffer();

	void Clear		();
	void Init		(unsigned int width, unsigned int height, const char* shader_glsl_name_ = "null.glsl");
	void Init		(unsigned int width, unsigned int height, const std::string& shader_glsl_name_ = "null.glsl");
	void Init		(unsigned int width, unsigned int height, size_t hased_shader_glsl_name_);
	void Init		(unsigned int width, unsigned int height, IShader* ishader_);
	void Resize		(unsigned int width, unsigned int height);

	void Begin		();
	void End		();
	void Activate	(GLenum activeTexture) const;
	void Deactivate	(GLenum activeTexture) const;
	void Bind		() const;

	void SetMaterial(size_t hased_shader_glsl_name_);
	void SetMaterial(const char* shader_glsl_name_);
	void SetMaterial(const std::string& shader_glsl_name_);
	void SetMaterial(IShader* ishader_);

	IShader* material;
	unsigned width;
	unsigned height;
	unsigned fbo;
	unsigned rbo;
	unsigned textureID;
};

#endif //
