
#ifndef _OPENGLGRAPHIC_H
#define _OPENGLGRAPHIC_H

#include "System.h"
#include "FrameBuffer.h"
#include "ZGMath.h"

#define NUMOFLIGHTS 16
#define NUM_OF_BUFFERS 4

class ZG_API OpenGLGraphic : public System
{
public:
	OpenGLGraphic() noexcept;
	virtual ~OpenGLGraphic();

	void BindDrawComponent();

	bool InitGLEW();
	bool InitSystem(int width_, int height_);
	virtual void  SystemUpdate();
	virtual bool  SystemExit();

	//setters
	void ReSizeGLScene(int width, int height);

	//void Draw();
	
	void SetBackgroundColor		(const zg::Color& c_);
	void SetResoultion			(RESOLUTION_SIZE size_);
	void ToggleWireFrame		(bool toggle_wireframe_);
	//void ShadowPass();
	//void DrawSkybox(const Camera* cam, bool realSkybox = true);
	//void RenderPass(const Camera* cam);
	//void RenderPost(const Camera* cam);
	//void RenderUI(const Camera* cam);
	//void StartBuffer(unsigned id, bool clear = true);
	//void EndBuffer(unsigned id);
	//void RenderBloom(const Camera* cam);
	//void RenderBuffer(const Camera* cam, bool post = false);
	//void Serial(FXMLDocument* doc, FXMLNode* root);
	//void Unserial(FXMLElement* pElem);
	//void UpdateShader(Material* mat, const Camera* cam);
	//static void SetSkybox(const std::string& name);
	//static void SetGamma(float gamma_);
	//static void SetFogNear(float near_);
	//static void SetFogFar(float far_);
	//static void GetFogColor(Vector4& color);
	//static Vector4 SetFogColor();

	//static float GetGamma();
	//static float GetFogNear();
	//static float GetFogFar();
	//static void OrderUISprites();

	int mWidth;
	int mHeight;

	//Framebuffer FBOs[NUM_OF_BUFFERS];
	FrameBuffer TFBO;
	//Framebuffer BloomFBO;
	//Framebuffer PingPongFBO;

	static zg::Vector3 globalAmbient;
	static zg::Vector3 attenuation;
	static zg::Vector3 fogColor;
	static zg::Vector2 fogFactor;
	static zg::Color wireframe_color;
	static bool toggle_bone;
	static bool toggle_wireframe;
	static bool toggle_drawmesh;
	static float   bloomThreshold;
	static float   bloomCoeff;
	static float   gamma;

	bool    showFog;

	static zg::Color Window_background_color;		//window background/clear color
	//static std::vector<UISprite*> orderedUISprites;
	//static Skybox* skybox;
private:
	
	//Framebuffer floatingPoint;
	//Model*      skyboxMdl;
	//Material*   skyboxMat;
	//Transform   skyboxTrans;

	//std::vector<MeshRenderer*>	transcluentObjects;
	//std::vector<MeshRenderer*>	postprocessingObjects;
	//std::vector<ParticleEmitter*>	postprocessingParticles;
	//std::vector<TextRenderer*>	transcluentObjects3;
	//std::vector<TextRenderer*>	uiText;
	//std::vector<UISprite*>		maskedUI;
};

#endif // !_OPENGLGRAPHIC_H
