#include "stdafx.h"
#include "FrameBuffer.h"
#include "AssetsManager.h"

FrameBuffer::FrameBuffer() noexcept
	:material(nullptr)
	, width(1280)
	, height(720)
	, fbo(0)
	, rbo(0)
	, textureID(0)
{}

FrameBuffer::~FrameBuffer()
{}

void FrameBuffer::Clear()
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glBindTexture(GL_TEXTURE_2D, 0);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 0, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, 0);

	glDeleteTextures(1, &textureID);
	textureID = 0;

	glDeleteRenderbuffers(1, &rbo);
	rbo = 0;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDeleteFramebuffers(1, &fbo);
	fbo = 0;
}

void FrameBuffer::Init(unsigned int width_, unsigned int height_, const char* shader_glsl_name_)
{
	Init(width_, height_, FindShader(shader_glsl_name_));
}
void FrameBuffer::Init(unsigned int width_, unsigned int height_, const std::string& shader_glsl_name_)
{
	Init(width_, height_, FindShader(shader_glsl_name_));
}
void FrameBuffer::Init(unsigned int width_, unsigned int height_, size_t hased_shader_glsl_name_)
{
	Init(width_, height_, FindShader(hased_shader_glsl_name_));
}
void FrameBuffer::Init(unsigned int width_, unsigned int height_, IShader* ishader_)
{
	width = width_;
	height = height_;

	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width_, height_);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);

	//Generate Texture
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width_, height_, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//Bind Texture to framebuffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureID, 0);

	auto result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (result != GL_FRAMEBUFFER_COMPLETE)
	{
		Warn("FrameBuffer failed: 0x%x\n", result);
		return;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	material = ishader_;
}
void FrameBuffer::Resize(unsigned int width_, unsigned int height_)
{
	Clear();
	Init(width_, height_, material);
}


void FrameBuffer::Begin(){
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}
void FrameBuffer::End(){
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void FrameBuffer::Activate(GLenum activeTexture) const{
	glActiveTexture(activeTexture);
	glBindTexture(GL_TEXTURE_2D, textureID);
}
void FrameBuffer::Deactivate(GLenum activeTexture) const{
	glActiveTexture(activeTexture);
	glBindTexture(GL_TEXTURE_2D, 0);
}
void FrameBuffer::Bind() const{
	glBindTexture(GL_TEXTURE_2D, textureID);
}

void FrameBuffer::SetMaterial(size_t hased_shader_glsl_name_)
{
	SetMaterial(FindShader(hased_shader_glsl_name_));
}
void FrameBuffer::SetMaterial(const char* shader_glsl_name_)
{
	SetMaterial(FindShader(shader_glsl_name_));
}
void FrameBuffer::SetMaterial(const std::string& shader_glsl_name_)
{
	SetMaterial(FindShader(shader_glsl_name_));
}
void FrameBuffer::SetMaterial(IShader* ishader_){
	material = ishader_;
}
