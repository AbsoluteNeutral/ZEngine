
#ifndef _CAMERA_H
#define _CAMERA_H

#include "Component.h"
#include "ZGMath.h"
#include "FrameBuffer.h"

enum ZG_API  CameraType {
	CAM_ORTHO = 0,
	CAM_PERSPECTIVE
};

#define MIN_CAM_NEARPLANE 0.00001f
#define MAX_CAM_GIMBALLOCK 89.0f
#define MAX_CAM_FOV_DEG 179.0f
#define MIN_CAM_FOV_DEG 1.0f
#define MAX_CAM_FOV_RAD 3.1398473f	
#define MIN_CAM_FOV_RAD 0.0174533f

class ZG_API Camera : public Component
{
public:
	Camera			 (CameraType type_ = CAM_PERSPECTIVE, ASPECT_RATIO type_aspectratio_ = CAM_16_9) noexcept;
	Camera			 (const Camera& rhs) noexcept;
	Camera& operator=(const Camera& rhs);
	virtual ~Camera	 ();

	void Attach			(GameObject* obj_, int num);
	Camera* Clone		();

	//setters
	void SetAspectRatio	(ASPECT_RATIO type_aspectratio_);
	void SetFOV			(float degree_);
	void SetFarPlane	(float farPlane_);
	void SetNearPlane	(float nearPlane_);
	void SetSize		(int size_);
	void SetType		(int type_);
	
	void LookAt(const Transform& target_);
	void RotateClamp(float x_, float y_, float z_);

	//void SetMaterial(const std::string& name);
	//void SetMaterial(size_t hashedName);

	//getters
	float			GetFarPlane()	const;
	float			GetNearPlane() const;
	zg::Vector3		GetForward() const;
	zg::Vector3		GetUp() const;
	zg::Vector3		GetRight() const;
	float			GetFOVdeg() const;
	float			GetFOVrad() const;
	float			GetAspectRatio() const;
	int				GetSize() const;
	int				GetType() const;
	float			GetViewportWidth() const;
	float			GetViewportHeight() const;
	//Color		GetPixelColor(int x, int y) const;
 
	zg::Matrix44	OrthongonalMatrix() const;
	zg::Matrix44	ProjectionMatrix() const;
	zg::Matrix44	ProjViewMatrix() const;
	zg::Matrix44	ViewMatrix() const;

	//void ResizeBuffer(int width, int height);

	//etc
#if defined(_EDITOR_ON) || defined(_DEBUG)
	void DebugDraw() const;
#endif
	void Update();
	void Serial(FXMLDocument* doc, FXMLNode* root);
	virtual void Unserial(FXMLElement* pElem) override;

#if defined(USING_SOL2)
	static void BindLua(sol::state& lua);
#endif
	//void SendUniforms(Material* material) const;
	//void SetUniformData(const std::string& name, bool val);
	//void SetUniformData(const std::string& name, float val);
	//void SetUniformData(const std::string& name, Vector2 val);
	//void SetUniformData(const std::string& name, zg::Vector3 val);
	//void SetUniformData(const std::string& name, Vector4 val);
	//bool GetUniformBool(const std::string& name);
	//float GetUniformFloat(const std::string& name);
	//Vector2 GetUniformVec2(const std::string& name);
	//zg::Vector3 GetUniformVec3(const std::string& name);
	//Vector4 GetUniformVec4(const std::string& name);

	//FrameBuffer* fbo;
	//size_t fboP;
	//unsigned id;
	//bool renderToTexture;
	//bool postprocessing;
	//unsigned int bWidth;
	//unsigned int bHeight;
	//std::map<unsigned, std::string> uniformData;
private:
	CameraType type;
	ASPECT_RATIO type_aspectratio;

	int size;
	float width;
	float height;
	float aspectRatio;
	int aspectWH;

	zg::Matrix44 view;
	zg::Matrix44 projection;

	float fovRAD;
	float np;			//near plane
	float fp;			//far plane

private:
	//functions
	void UpdateWidthHeight();
	void OrthRH();
	void PerspectiveRH();	
};
#endif _CAMERA_H
