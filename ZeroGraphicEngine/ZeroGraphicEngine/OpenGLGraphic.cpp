
#include "stdafx.h"

#include "OpenGLGraphic.h"
#include "Engine.h"
//#include "ResourceManager.h"

#define SHADOW_WIDTH 4096
#define SHADOW_HEIGHT 4096

#if defined(_INGAME)
zg::Color OpenGLGraphic::Window_background_color{ CC_BLACK };		//window background/clear color
#else
zg::Color OpenGLGraphic::Window_background_color{ CC_GRAY_27 };		//window background/clear color
#endif

zg::Vector3 OpenGLGraphic::globalAmbient	= zg::Vector3{ 0.1f, 0.1f, 0.1f };
zg::Vector3 OpenGLGraphic::attenuation		= zg::Vector3{ 1.0f, 0.09f, 0.032f };
zg::Vector3 OpenGLGraphic::fogColor			= zg::Vector3{ 0.5f, 0.5f, 0.5f };
zg::Vector2 OpenGLGraphic::fogFactor		= zg::Vector2{ 1.f, 250.f };
zg::Color OpenGLGraphic::wireframe_color= CC_GRAY_40;
bool OpenGLGraphic::toggle_bone			= true;
bool OpenGLGraphic::toggle_wireframe	= false;
bool OpenGLGraphic::toggle_drawmesh		= true;
float OpenGLGraphic::bloomThreshold		= 1.f;
float OpenGLGraphic::bloomCoeff			= 1.f;
float OpenGLGraphic::gamma				= 1.f;
//Skybox* OpenGLGraphic::skybox = nullptr;
//std::vector<UISprite*> OpenGLGraphic::orderedUISprites;

inline void RenderQuad() // TO BE REPLACED
{
  static GLuint quadVAO = 0;
  static GLuint quadVBO;

  if (quadVAO == 0)
  {
    float quadVertices[] = {
      // positions        // texture Coords
      -1.f,  1.f, 0.f, 0.f, 1.f,
      -1.f, -1.f, 0.f, 0.f, 0.f,
       1.f,  1.f, 0.f, 1.f, 1.f,
       1.f, -1.f, 0.f, 1.f, 0.f,
    };
    // setup plane VAO
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
  }

  glBindVertexArray(quadVAO);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  glBindVertexArray(0);
}

OpenGLGraphic::OpenGLGraphic() noexcept
	: System()
    , mWidth{ 1280 }
    , mHeight{ 720 }
    //FBOs{},
    , TFBO{}
    //BloomFBO{},
    //PingPongFBO{},
    //showFog{ false },
    //floatingPoint{},
    //skyboxMdl{ nullptr },
    //skyboxMat{ nullptr },
    //skyboxTrans{},
    //transcluentObjects{},
    //postprocessingObjects{},
    //transcluentObjects3{},
    //uiText{},
	//maskedUI{}
	//, toggle_wireframe(false)
{}

OpenGLGraphic::~OpenGLGraphic()
{
  //for (auto& elem : FBOs)
  //  elem.Clear();
  //
  TFBO.Clear();
  //BloomFBO.Clear();
  //PingPongFBO.Clear();
}

void OpenGLGraphic::BindDrawComponent() {
//#if defined(_EDITOR_ON)
//	transcluentObjects.reserve(RM::R_MeshRenderer.size());
//  postprocessingObjects.reserve(RM::R_MeshRenderer.size());
//  postprocessingParticles.reserve(RM::R_ParticleEmitter.size());
//	transcluentObjects3.reserve(RM::R_TextRenderer.size());
//  uiText.reserve(RM::R_TextRenderer.size());
//  maskedUI.reserve(RM::R_UISprite.size());
//#else
//	transcluentObjects.reserve(RM::C_MeshRenderer);
//	postprocessingObjects.reserve(RM::C_MeshRenderer);
//	postprocessingParticles.reserve(RM::C_ParticleEmitter);
//	transcluentObjects3.reserve(RM::C_TextRenderer);
//	uiText.reserve(RM::C_TextRenderer);
//	maskedUI.reserve(RM::C_UISprite);
//#endif

//	listOfDrawComponents.clear();
//
//#if defined(_EDITOR_ON)
//	listOfDrawComponents.reserve(
//		RM::R_Sprite2D.size()
//		+ RM::R_MeshRenderer.size()
//		+ RM::R_TextRenderer.size()
//	);
//
//	for(auto& elem : RM::R_Sprite2D)		listOfDrawComponents.push_back(elem);
//	for(auto& elem : RM::R_MeshRenderer)	listOfDrawComponents.push_back(elem);
//	for(auto& elem : RM::R_TextRenderer)	listOfDrawComponents.push_back(elem);
//#else
//	listOfDrawComponents.reserve(
//		RM::C_Sprite2D
//		+ RM::C_MeshRenderer
//		+ RM::C_TextRenderer
//	);
//
//	{	Sprite2D* b = RM::R_Sprite2D;
//		Sprite2D* e = RM::R_Sprite2D + RM::C_Sprite2D;
//		while (b != e) listOfDrawComponents.push_back(b++);
//	}
//	{	MeshRenderer* b = RM::R_MeshRenderer;
//		MeshRenderer* e = RM::R_MeshRenderer + RM::C_MeshRenderer;
//		while (b != e) listOfDrawComponents.push_back(b++);
//	}
//	{	TextRenderer* b = RM::R_TextRenderer;
//		TextRenderer* e = RM::R_TextRenderer + RM::C_TextRenderer;
//		while (b != e) listOfDrawComponents.push_back(b++);
//	}
//#endif
}
///Create OpenGLSystem
bool OpenGLGraphic::InitGLEW() {
	///Initalize glewInit()
	if (glewInit()) {
		ErrorMsg("Failed glewInit()");
		return false;
	}

	Logg("OpenGLGraphic.cpp OPENGL Created");
	glEnable(GL_ALPHA_TEST);							///enable alpha
	glDepthMask(GL_TRUE);
	glAlphaFunc(GL_GREATER, 0.0f);						///enable alpha
	glEnable(GL_BLEND);									///enable blend
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glBlendFunc(GL_ZERO, GL_SRC_COLOR);
	//glBlendFunc(GL_ONE_MINUS_DST_ALPHA, GL_DST_ALPHA);
	//glBlendFunc(GL_ONE, GL_ONE);
	glEnable(GL_DEPTH_TEST);	///enable depth
	glEnable(GL_CULL_FACE);		///enable cullface
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(0.1f, 1.f);
	//glEnable(GL_PROGRAM_POINT_SIZE);
	//glEnable(GL_POINT_SPRITE); 
	//glTexEnvi(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE);

  glColorMask(TRUE, TRUE, TRUE, TRUE);
	glClearColor(
		Window_background_color.r, 
		Window_background_color.g,
		Window_background_color.b, 
		Window_background_color.a);
	return true;
}
bool OpenGLGraphic::InitSystem(int width_, int height_)
{
	//for(int i = 0; i < NUM_OF_LIGHTS; ++i)
	//  RM::lightsource[i].Init(SHADOW_WIDTH, SHADOW_HEIGHT, "shadow.material");
	//
	//for(int i = 0; i < NUM_OF_BUFFERS; ++i)
	//  FBOs[i].Init(1920, 1200, "PostProcessing.material", FLOATING, 0.f);
	//
	mWidth = width_;
	mHeight = height_;
	TFBO.Init(width_, height_, "nullpost.glsl");
	//BloomFBO.Init(1920, 1200, "bloom.material", FLOATING, 0.f);
	//PingPongFBO.Init(1920, 1200, "blur.material", FLOATING, 0.f);
	//
	//skybox    = FindSkybox("SBnebula");
	//skyboxMdl = FindModel("skybox.models");
	//skyboxMat = FindMaterial("skybox.material");
	//skyboxTrans.SetPosition(zg::Vector3{});
	//skyboxTrans.SetScale(zg::Vector3{ 1000.f, 1000.f, 1000.f });
	//skyboxTrans.ForceUpdate();

	ToggleWireFrame(toggle_wireframe);
	return true;
}
void OpenGLGraphic::SetResoultion(RESOLUTION_SIZE size_) {
	zg::SetResolution(size_);
	TFBO.Resize(zg::GOBAL_RESOLUTION_WIDTH, zg::GOBAL_RESOLUTION_HEIGHT);
}

void OpenGLGraphic::ToggleWireFrame(bool toggle_wireframe_) {
	toggle_wireframe = toggle_wireframe_;
}

void OpenGLGraphic::ReSizeGLScene(int width_, int height_){
	// Resize And Initialize The GL Window
	//static bool firstResize = true;

	mWidth  = width_ <= 0 ? 1 : width_;
	mHeight = height_ <= 0 ? 1 : height_;
	//if (EngineIsRunning()) 
	//	MainCam->ResizeBuffer(mWidth, mHeight);

	glViewport(0, 0, width_, height_); //Reset The Current Viewport

	//if (firstResize)
	//{
	//  firstResize = false;
	//  return;
	//}

  //for (auto& elem : FBOs)
  //  elem.Resize(mWidth, mHeight, 0.f);
//#if defined(_EDITOR_ON)
//  for (auto& elem : RM::R_Camera)
//    elem->ResizeBuffer(mWidth, mHeight);
//#else
//  Camera* b = RM::R_Camera;
//  Camera* e = RM::R_Camera + RM::C_Camera;
//  while (b != e) {
//	  (b++)->ResizeBuffer(mWidth, mHeight);
//  }
//#endif
	//1024 × 576
  
  //BloomFBO.Resize(mWidth, mHeight, 0.f);
  //PingPongFBO.Resize(mWidth, mHeight, 0.f);
}

void OpenGLGraphic::SystemUpdate()
{
#if defined(_EDITOR_ON) || !defined(_EDITOR_ON)
	
	for (auto& elem : RM::R_MeshRenderer)
		elem->Update();

	for (auto& elem : RM::R_GameObject)
		elem->transform.Update();

	//for (auto& elem : RM::R_MeshRenderer)
	//	elem->PostUpdate();
#endif

	//set viewport to screen's resolution buffer size
	glViewport(0, 0, TFBO.width, TFBO.height);

	// first pass
	TFBO.Begin();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // we're not using the stencil buffer now

#if defined(_EDITOR_ON) || !defined(_EDITOR_ON)
	DebugGizmo.Draw(MainCam);
#endif
	if (toggle_wireframe)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		for (auto& elem : RM::R_MeshRenderer)
			elem->DrawWireFrame(MainCam, wireframe_color);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	else if(toggle_drawmesh)
		for (auto& elem : RM::R_MeshRenderer)
			elem->Draw(MainCam);

	if(toggle_bone)
		for (auto& elem : RM::R_MeshRenderer)
			elem->RenderSkeleton();

	TFBO.End();

	//reset to window size
	glViewport(0, 0, mWidth, mHeight);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//second pass
	TFBO.material->Use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TFBO.textureID);
	RenderQuad();


#if defined(_EDITOR_ON)
	//for (auto& elem : RM::R_MeshRenderer)
	//{
	//  if (!elem->owner->isActive || !elem->enable || !elem->isVisible || elem->color.a <= 0.0f)
	//    continue;
	//
	//  elem->Update();
	//}
	//
	//for (auto& elem : RM::R_Light)
	//{
	//  if (!elem->owner->isActive || !elem->enable)
	//    continue;
	//
	//  elem->Update();
	//}
	//
	//for (auto& elem : RM::R_ParticleEmitter)
	//{
	//	if (!elem->owner->isActive || !elem->enable)
	//	  continue;
	//	
	//	elem->Update();
	//}
#else
	//{
	//  MeshRenderer* b = RM::R_MeshRenderer;
	//  MeshRenderer* e = RM::R_MeshRenderer + RM::C_MeshRenderer;
	//  while (b != e) {
	//	  if (!b->owner->isActive || !b->enable || !b->isVisible || b->color.a <= 0.0f) {
	//		  b++;
	//		  continue;
	//	  }
	//	  (b++)->Update();
	//  }
	//}
	//{
	//  Light* b = RM::R_Light;
	//  Light* e = RM::R_Light + RM::C_Light;
	//  while (b != e)
	//  {
	//    if (!b->owner->isActive || !b->enable)
	//    {
	//      b++;
	//      continue;
	//    }
	//
	//	  (b++)->Update();
	//  }
	//}
	//{
	//  ParticleEmitter* b = RM::R_ParticleEmitter;
	//  ParticleEmitter* e = RM::R_ParticleEmitter + RM::C_ParticleEmitter;
	//  while (b != e)
	//  {
	//    if (!b->owner->isActive || !b->enable)
	//    {
	//      b++;
	//      continue;
	//    }
	//
	//	  (b++)->Update();
	//  }
	//}
#endif
}

void OpenGLGraphic::SetBackgroundColor(const zg::Color& c_) {
	Window_background_color = c_;
	glClearColor(
		Window_background_color.r,
		Window_background_color.g,
		Window_background_color.b,
		Window_background_color.a);
}

//void OpenGLGraphic::ShadowPass()
//{
//  glCullFace(GL_FRONT);
//  glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
//  for (int i = 0; i < NUM_OF_LIGHTS; ++i)
//  {
//    RM::lightsource[i].BeginShadow();
//    glClear(GL_DEPTH_BUFFER_BIT);
//    RM::lightsource[i].RenderShadows();
//    RM::lightsource[i].EndShadow();
//  }
//  glEnable(GL_CULL_FACE);
//}

//void OpenGLGraphic::DrawSkybox(const Camera* cam, bool realSkybox)
//{
//  static auto nullMat = FindMaterial("null.material");
//  auto mat = realSkybox ? skyboxMat : nullMat;
//  
//  mat->Use();
//  
//  skybox->UseTextures(mat);
//  skyboxMdl->DrawSkybox(cam, mat, skyboxTrans.GetMatrixPtr());
//
//  nullMat->ActiveTexture();
//}

//void OpenGLGraphic::RenderPass(const Camera* cam)
//{
//  static auto font = FindMaterial("font.material");
//  static auto particle = FindMaterial("particle.material");
//  static auto sprite = FindMaterial("sprite.material");
//  static auto toon = FindMaterial("toon.material");
//  static auto pbr = FindMaterial("PBR.material");
//  static auto blend = FindMaterial("blend.material");
//  static auto blank = FindMaterial("blank.material");
//
//  UpdateShader(font, cam);
//  UpdateShader(particle, cam);
//  UpdateShader(sprite, cam);
//  UpdateShader(toon, cam);
//  UpdateShader(pbr, cam);
//  UpdateShader(blend, cam);
//  UpdateShader(blank, cam);
//
//  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//	glCullFace(GL_BACK);
//#if defined(_EDITOR_ON)
//  if (!IsThereVideoPlaying())
//  {
//    for (auto& elem : RM::R_MeshRenderer)
//    {
//      if (!elem->owner->isActive || !elem->enable || !elem->isVisible || elem->color.a <= 0.0f)
//        continue;
//  
//      if (elem->postProcess)
//        postprocessingObjects.push_back(elem);
//      else if (!elem->justDraw && elem->color.a < 1.0f)
//        transcluentObjects.push_back(elem);
//      else
//        elem->Draw(cam, RM::lightsource);
//    }
//  
//    glDisable(GL_CULL_FACE);
//    font->Use();
//    for (auto& elem : RM::R_TextRenderer)
//    {
//      if (!elem->owner->isActive || !elem->enable || !elem->isVisible || elem->color.a <= 0.0f)
//        continue;
//  
//      if (elem->isUI)
//        uiText.push_back(elem);
//      else if (elem->color.a < 1.f)
//        transcluentObjects3.push_back(elem);
//      else
//        elem->Draw(font, cam);
//    }
//    glEnable(GL_CULL_FACE);
//  
//    glBlendFunc(GL_ONE, GL_ONE);
//    glDepthMask(GL_FALSE);
//  
//    for (auto& elem : transcluentObjects)
//      elem->Draw(cam, RM::lightsource);
//    transcluentObjects.clear();
//    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//    glDepthMask(GL_TRUE);
//  
//    particle->Use();
//    particle->SendMat4("proj", cam->ProjectionMatrix());
//    particle->SendMat4("view", cam->ViewMatrix());
//    particle->SendVec3("camUp", MainCam->GetUp());
//    particle->SendVec3("camRight", MainCam->GetRight());
//    particle->SendVec3("camForward", MainCam->GetForward());
//    particle->SendBool("distort", false);
//
//    for (auto& elem : RM::R_ParticleEmitter)
//    {
//      if (elem->owner->isActive && elem->enable)
//      {
//        if (elem->distort)
//          postprocessingParticles.push_back(elem);
//        else
//          elem->Draw(particle);
//      }
//    }
//    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//  
//    glDisable(GL_CULL_FACE);
//    font->Use();
//    for (auto& elem : transcluentObjects3)
//      elem->Draw(font, cam);
//    transcluentObjects3.clear();
//  }
//#else
//  if (!IsThereVideoPlaying())
//  {
//    {
//      MeshRenderer* b = RM::R_MeshRenderer;
//      MeshRenderer* e = RM::R_MeshRenderer + RM::C_MeshRenderer;
//      while (b != e)
//      {
//        if (!b->owner->isActive || !b->enable || !b->isVisible || b->color.a <= 0.0f)
//        {
//          b++;
//          continue;
//        }
//
//        if (b->postProcess)
//          postprocessingObjects.push_back(b++);
//        else if (!b->justDraw && b->color.a < 1.0)
//          transcluentObjects.push_back(b++);
//        else
//          (b++)->Draw(cam, RM::lightsource);
//      }
//    }
//
//    {
//      glDisable(GL_CULL_FACE);
//      font->Use();
//
//      TextRenderer* b = RM::R_TextRenderer;
//      TextRenderer* e = RM::R_TextRenderer + RM::C_TextRenderer;
//
//      while (b != e)
//      {
//        if (!b->owner->isActive || !b->enable || !b->isVisible || b->color.a <= 0.0f)
//        {
//          b++;
//          continue;
//        }
//
//        if (b->isUI)
//          uiText.push_back(b++);
//        else if (b->color.a < 1.f)
//          transcluentObjects3.push_back(b++);
//        else
//          (b++)->Draw(font, cam);
//      }
//    }
//
//    glBlendFunc(GL_ONE, GL_ONE);
//    glDepthMask(GL_FALSE);
//    for (auto& elem : transcluentObjects)
//    {
//      elem->Draw(cam, RM::lightsource);
//    }
//    glDepthMask(GL_TRUE);
//    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//    transcluentObjects.clear();
//
//    particle->Use();
//    particle->SendMat4("proj", cam->ProjectionMatrix());
//    particle->SendMat4("view", cam->ViewMatrix());
//    particle->SendVec3("camUp", MainCam->GetUp());
//    particle->SendVec3("camRight", MainCam->GetRight());
//    particle->SendVec3("camForward", MainCam->GetForward());
//    {
//      ParticleEmitter* b = RM::R_ParticleEmitter;
//      ParticleEmitter* e = RM::R_ParticleEmitter + RM::C_ParticleEmitter;
//      while (b != e)
//      {
//        if (!b->owner->isActive || !b->enable)
//        {
//          b++;
//          continue;
//        }
//
//        if (b->distort)
//          postprocessingParticles.push_back(b++);
//        else
//          (b++)->Draw(particle);
//      }
//    }
//    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//    glDisable(GL_CULL_FACE);
//    font->Use();
//    for (auto& elem : transcluentObjects3)
//      elem->Draw(font, cam);
//    transcluentObjects3.clear();
//  }
//#endif
//  glUseProgram(0);
//}

//void OpenGLGraphic::RenderPost(const Camera * cam)
//{
//  static auto post = FindMaterial("distort.material");
//  static float time = 0.f;
//
//  if (cam == GetDefaultCamera())
//  {
//    postprocessingObjects.clear();
//    postprocessingParticles.clear();
//    return;
//  }
//
//  UpdateShader(post, cam);
//
//  time += Engine::sys_FrameRateController.dttimescale;
//
//  StartBuffer(cam->id, false);
//  glViewport(0, 0, mWidth, mHeight);
//  post->Use();
//  glActiveTexture(GL_TEXTURE0);
//  glUniform1i(glGetUniformLocation(post->shader->programID, "baseTexture"), 0);
//  glBindTexture(GL_TEXTURE_2D, TFBO.texture);
//  glActiveTexture(GL_TEXTURE1);
//  glUniform1i(glGetUniformLocation(post->shader->programID, "maskTexture"), 1);
//  glBindTexture(GL_TEXTURE_2D, FindTexture("Cloud_D.dds")->id);
//  post->SendFloat("time", time);
//
//  for (auto& elem : postprocessingObjects)
//    elem->DrawPost(cam);
//  postprocessingObjects.clear();
//
//  EndBuffer(cam->id);
//}

//void OpenGLGraphic::RenderUI(const Camera * cam)
//{
//  static auto font = FindMaterial("font.material");
//  static auto sprite = FindMaterial("sprite.material");
//
//  if (cam == GetDefaultCamera())
//  {
//    uiText.clear();
//    return;
//  }
//
//  TFBO.Begin(); 
//  GLuint clearColor[4] = { 0, 0, 0, 1 };
//  glClearTexImage(TFBO.texture2, 0, GL_RGBA, GL_FLOAT, &clearColor);
//  UpdateShader(sprite, cam);
//
//  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//  glCullFace(GL_BACK);
//
//  glClear(GL_DEPTH_BUFFER_BIT);
//  sprite->Use();
//  
//  glDepthMask(GL_FALSE);
//  for (auto& elem : orderedUISprites)
//  {
//    if (!elem->owner->isActive || !elem->enable || !elem->isVisible || elem->color.a <= 0.0f)
//      continue;
//
//	if (elem->ignoreMask)
//		elem->Update();
//	else
//		maskedUI.push_back(elem); 
//  }
//
//  glDepthMask(GL_TRUE);
//  for (auto& elem : maskedUI)
//  {
//    elem->Update();
//  }
//  maskedUI.clear();
//
//  glClear(GL_DEPTH_BUFFER_BIT);
//  if (!IsThereVideoPlaying())
//  {
//    font->Use();
//    for (auto& elem : uiText)
//      elem->Draw(font, cam);
//    uiText.clear();
//  }
//
//  TFBO.End();
//
//  bool first = true;
//  bool horizontal = true;
//
//  int count = 10;
//
//  PingPongFBO.material->Use();
//
//  for (int i = 0; i < count; ++i)
//  {
//    PingPongFBO.material->SendInt("horizontal", horizontal);
//
//    if (horizontal)
//    {
//      PingPongFBO.Begin();
//      if (first)
//      {
//        glActiveTexture(GL_TEXTURE0);
//        glUniform1i(glGetUniformLocation(TFBO.material->shader->programID, "texture_0"), 0);
//        glBindTexture(GL_TEXTURE_2D, TFBO.texture2);
//        first = false;
//      }
//      else
//        BloomFBO.Activate(GL_TEXTURE0);
//    }
//    else
//    {
//      BloomFBO.Begin();
//      PingPongFBO.Activate(GL_TEXTURE0);
//    }
//
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//    RenderQuad();
//
//    if (horizontal)
//      PingPongFBO.End();
//    else
//      BloomFBO.End();
//
//    horizontal = !horizontal;
//  }
//
//  glViewport(0, 0, mWidth, mHeight);
//  TFBO.material->Use();
//  glActiveTexture(GL_TEXTURE0);
//  glUniform1i(glGetUniformLocation(TFBO.material->shader->programID, "texture_0"), 0);
//  glBindTexture(GL_TEXTURE_2D, TFBO.texture);
//  glActiveTexture(GL_TEXTURE1);
//  glUniform1i(glGetUniformLocation(TFBO.material->shader->programID, "bloom"), 1);
//  glBindTexture(GL_TEXTURE_2D, BloomFBO.texture);
//  TFBO.material->SendFloat("gamma", gamma);
//  TFBO.material->SendBool("UIPass", true);
//
//  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//  RenderQuad();
//  glBindTexture(GL_TEXTURE_2D, 0);
//}

//void OpenGLGraphic::StartBuffer(unsigned id, bool clear)
//{
//  glViewport(0, 0, FBOs[id].width, FBOs[id].height);
//  FBOs[id].Begin();
//  
//  if(clear)
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//}

//void OpenGLGraphic::EndBuffer(unsigned id)
//{
//  FBOs[id].End();
//}

//void OpenGLGraphic::RenderBloom(const Camera* cam)
//{
//  bool first = true;
//  bool horizontal = true;
//
//  int count = 10;
//
//  PingPongFBO.material->Use();
//
//  for (int i = 0; i < count; ++i)
//  {
//    PingPongFBO.material->SendInt("horizontal", horizontal);
//
//    if (horizontal)
//    {
//      PingPongFBO.Begin();
//      if (first)
//      {
//        glActiveTexture(GL_TEXTURE0);
//        glUniform1i(glGetUniformLocation(cam->fbo->material->shader->programID, "texture_0"), 0);
//        glBindTexture(GL_TEXTURE_2D, cam->fbo->texture2);
//        first = false;
//      }
//      else
//        BloomFBO.Activate(GL_TEXTURE0);
//    }
//    else
//    {
//      BloomFBO.Begin();
//      PingPongFBO.Activate(GL_TEXTURE0);
//    }
//
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//    RenderQuad();
//
//    if (horizontal)
//      PingPongFBO.End();
//    else
//      BloomFBO.End();
//
//    horizontal = !horizontal;
//  }
//}

//void OpenGLGraphic::RenderBuffer(const Camera* cam, bool post)
//{
//  glViewport(0, 0, mWidth, mHeight);
//  cam->fbo->material->Use();
//  glActiveTexture(GL_TEXTURE0);
//  glUniform1i(glGetUniformLocation(cam->fbo->material->shader->programID, "texture_0"), 0);
//  glBindTexture(GL_TEXTURE_2D, cam->fbo->texture);
//  glActiveTexture(GL_TEXTURE1);
//  glUniform1i(glGetUniformLocation(cam->fbo->material->shader->programID, "bloom"), 1);
//  glBindTexture(GL_TEXTURE_2D, BloomFBO.texture);
//  cam->SendUniforms(cam->fbo->material);
//  cam->fbo->material->SendFloat("gamma", gamma);
//  cam->fbo->material->SendBool("UIPass", false);
//  cam->fbo->material->SendBool("post", post);
//  
//#if defined(_EDITOR_ON)
//  if (cam != GetDefaultCamera())
//#endif
//  {
//    glViewport(0, 0, TFBO.width, TFBO.height);
//    TFBO.Begin();
//  }
//
//  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//  RenderQuad();
//  glBindTexture(GL_TEXTURE_2D, 0);
//
//#if defined(_EDITOR_ON)
//  if (cam != GetDefaultCamera())
//#endif
//  {
//    TFBO.End();
//  }
//}

//void OpenGLGraphic::Serial(FXMLDocument * doc, FXMLNode * root)
//{
//  FXMLElement* pElem = doc->NewElement("GlobalVariables");
//
//  pElem->SetAttribute("Attx", attenuation.x);
//  pElem->SetAttribute("Atty", attenuation.y);
//  pElem->SetAttribute("Attz", attenuation.z);
//  pElem->SetAttribute("gAx", globalAmbient.x);
//  pElem->SetAttribute("gAy", globalAmbient.y);
//  pElem->SetAttribute("gAz", globalAmbient.z);
//  pElem->SetAttribute("fCx", fogColor.x);
//  pElem->SetAttribute("fCy", fogColor.y);
//  pElem->SetAttribute("fCz", fogColor.z);
//  pElem->SetAttribute("near", fogFactor.x);
//  pElem->SetAttribute("far", fogFactor.y);
//  pElem->SetAttribute("bloomThreshold", bloomThreshold);
//  pElem->SetAttribute("bloomCoeff", bloomCoeff);
//  pElem->SetAttribute("showFog", showFog);
//  pElem->SetAttribute("Skybox", skybox ? skybox->prefix.c_str() : "None");
//
//  root->InsertEndChild(pElem);
//}

//void OpenGLGraphic::Unserial(FXMLElement * pElem)
//{
//  auto pSubElem = pElem->FirstChildElement();
//  attenuation.x   = pSubElem->FloatAttribute("Attx");
//  attenuation.y   = pSubElem->FloatAttribute("Atty");
//  attenuation.z   = pSubElem->FloatAttribute("Attz");
//  globalAmbient.x = pSubElem->FloatAttribute("gAx");
//  globalAmbient.y = pSubElem->FloatAttribute("gAy");
//  globalAmbient.z = pSubElem->FloatAttribute("gAz");
//  fogColor.x      = pSubElem->FloatAttribute("fCx");
//  fogColor.y      = pSubElem->FloatAttribute("fCy");
//  fogColor.z      = pSubElem->FloatAttribute("fCz");
//  fogFactor.x     = pSubElem->FloatAttribute("near");
//  fogFactor.y     = pSubElem->FloatAttribute("far");
//  bloomThreshold  = pSubElem->FloatAttribute("bloomThreshold", 1.f);
//  bloomCoeff      = pSubElem->FloatAttribute("bloomCoeff", 1.f);
//  showFog         = pSubElem->BoolAttribute("showFog");
//  auto name       = pSubElem->Attribute("Skybox");
//
//  if (name)
//  {
//    if (strcmp(name, "None") != 0)
//      skybox = FindSkybox(name);
//  }
//  else
//    skybox = FindSkybox("SBnebula");
//}
//
//void OpenGLGraphic::UpdateShader(Material * mat, const Camera* cam)
//{
//  mat->Use();
//  for (int i = 0; i < NUM_OF_LIGHTS; ++i)
//  {
//    static std::string shadowMap = "shadowMap[i]";
//    shadowMap[10] = static_cast<char>('0' + i);
//    RM::lightsource[i].shadowFBO.Activate(GL_TEXTURE6 + i);
//    mat->SendInt(shadowMap.c_str(), 6 + i);
//    RM::lightsource[i].UpdateShader(mat);
//  }
//
//  mat->SendMat4("projview", cam->ProjViewMatrix());
//  mat->SendVec3("camPos", cam->GetTransform().translation);
//  mat->SendVec3("GlobalAmbient", globalAmbient);
//  mat->SendVec3("FogColor", fogColor);
//  mat->SendVec2("FogNearFar", fogFactor);
//  mat->SendFloat("BloomThreshold", bloomThreshold);
//  mat->SendFloat("BloomCoefficient", bloomCoeff);
//  mat->SendBool("ShowFog", showFog || !IsDefaultCamera());
//}
//
//void OpenGLGraphic::SetSkybox(const std::string & name)
//{
//  skybox = FindSkybox(name);
//}
//
//void OpenGLGraphic::SetGamma(float gamma_)
//{
//  gamma = gamma_;
//}
//
//void OpenGLGraphic::SetFogNear(float near_)
//{
//	fogFactor.x = near_;
//}
//
//void OpenGLGraphic::SetFogFar(float far_)
//{
//	fogFactor.y = far_;
//}
//
//void OpenGLGraphic::GetFogColor(Vector4 & color)
//{
//  fogColor = color;
//}
//
//Vector4 OpenGLGraphic::SetFogColor()
//{
//  return Vector4{ fogColor, 1.0 };
//}
//
//float OpenGLGraphic::GetGamma()
//{
//  return gamma;
//}
//
//float OpenGLGraphic::GetFogNear()
//{
//	return fogFactor.x;
//}
//
//float OpenGLGraphic::GetFogFar()
//{
//	return fogFactor.y;
//}
//
//void OpenGLGraphic::OrderUISprites()
//{
//  std::sort(orderedUISprites.begin(), orderedUISprites.end(),
//    [](UISprite* lhs, UISprite* rhs) {return lhs->owner->transform.translation.z < rhs->owner->transform.translation.z; });
//}

bool OpenGLGraphic::SystemExit()
{
	return true;
}

//void OpenGLGraphic::Draw()
//{
//#if not defined(_EDITOR_ON)
//  ShadowPass();
//
//  glCullFace(GL_FRONT);
//  StartBuffer(MainCam->id);
//  DrawSkybox(MainCam);
//  RenderPass(MainCam);
//
//#if !defined(NDEBUG)
//  DebugGizmo.Draw(MainCam);
//#endif
//  EndBuffer(MainCam->id);
//  RenderBuffer(MainCam);
//
//  RenderPost(MainCam);
//  RenderBloom(MainCam);
//  RenderBuffer(MainCam, true);
//  
//  RenderUI(MainCam);
//#endif
//}
