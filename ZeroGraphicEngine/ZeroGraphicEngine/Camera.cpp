
#include "stdafx.h"
#include "Camera.h"
#include "Gizmo.h"

Camera::Camera(CameraType type_, ASPECT_RATIO type_aspectratio_) noexcept
	:Component{}
	//, fbo (nullptr)
	//, fboP(0)
	//, id(0)
	//, renderToTexture(false)
	//, postprocessing(true)
	//, bWidth(1920)
	//, bHeight(1080)
	//, uniformData{}
	, type{ type_ }
	, type_aspectratio(type_aspectratio_)
	, size(1)
	, width(0)
	, height(0)
	, aspectRatio(0.0f)
	, aspectWH(0)
	, view{}
	, projection{}
	, fovRAD(TO_RAD(45.0f))
	, np{ MIN_CAM_NEARPLANE }
	, fp{ 10000.0f }
{
	//fboP = GenerateHash("PostProcessing.material");
	SetAspectRatio(type_aspectratio_);
}

Camera::Camera(const Camera& rhs) noexcept
	:Component{ rhs }
	//, fbo(nullptr)
	//, fboP(0)
	//, id(0)
	//, renderToTexture(rhs.renderToTexture)
	//, postprocessing(rhs.postprocessing)
	//, bWidth(rhs.bWidth)
	//, bHeight(rhs.bHeight)
	//, uniformData{rhs.uniformData}
	, type{ rhs.type }
	, type_aspectratio(rhs.type_aspectratio)
	, size(rhs.size)
	, width(rhs.width)
	, height(rhs.height)
	, aspectRatio(rhs.aspectRatio)
	, aspectWH(rhs.aspectWH)
	, view{}
	, projection{}
	, fovRAD(rhs.fovRAD)
	, np{ rhs.np }
	, fp{ rhs.fp }
{
	SetAspectRatio(type_aspectratio);
	SetType(type);
}
Camera& Camera::operator=(const Camera& rhs) {
	Component::operator=(rhs);
	//renderToTexture = rhs.renderToTexture;
	//postprocessing = rhs.postprocessing;
	//bWidth			= rhs.bWidth;
	//bHeight			= rhs.bHeight;
	//uniformData		= rhs.uniformData;
	type			 = rhs.type;
	type_aspectratio = rhs.type_aspectratio;
	size			 = rhs.size;
	width			 = rhs.width;
	height			 = rhs.height;
	aspectRatio		 = rhs.aspectRatio;
	aspectWH		 = rhs.aspectWH;
	fovRAD			 = rhs.fovRAD;
	np				 = rhs.np;
	fp				 = rhs.fp;
	SetAspectRatio(type_aspectratio);
	SetType(type);
	return *this;
}
void Camera::Attach(GameObject* obj_, int num) 
{
	owner = obj_;
	obj_->AddComponent<Camera>(num);
	owner->transform.SetFacingVector(zg::Vector3{ 0.0f, 0.0f, -1.0f });
	view = LookAtRH(owner->transform.position, owner->transform.position + owner->transform.Forward(), zg::Vector3::Vector3Y);

	//if (GetHashFromString(obj_->name) != GetHashFromString("DefaultCamera"))
	//  num++;
	//if (num < 8)
	//{
	//  //printf("Binding %d to %s...\n", num, obj_->name.c_str());
	//  fbo = &Engine::sys_OpenGLGraphic.FBOs[num];
	//  id = num;
	//
	//  if (GetHashFromString(obj_->name) != GetHashFromString("DefaultCamera"))
	//    SetMaterial("PostProcessing.material");
	//  else
	//    SetMaterial("default.material");
	//}
}

Camera* Camera::Clone() {
#ifdef USE_RAWNEW
	return new Camera{ *this };
#else
	Camera* tmp = Allocate<Camera>(1);
	new(tmp) Camera{ *this };
	return tmp;
#endif
}

Camera::~Camera(){}

//setters
void Camera::SetAspectRatio(ASPECT_RATIO type_aspectratio_) {
	aspectWH ^= aspectWH;
	type_aspectratio = type_aspectratio_;

	switch (type_aspectratio_) {
	case CAM_4_3:
		aspectRatio = 4.0f / 3.0f;
		aspectWH |= 4 << 8;
		aspectWH |= 3;
		break;
	case CAM_16_10:
		aspectRatio = 16.0f / 10.0f;
		aspectWH |= 16 << 8;
		aspectWH |= 10;
		break;
	default:
		aspectRatio = 16.0f / 9.0f;
		aspectWH |= 16 << 8;
		aspectWH |= 9;
		break;
	}
	SetType(type);
}

void Camera::SetFOV(float degree_) {
	float getRad = TO_RAD(degree_);
	fovRAD = getRad < MIN_CAM_FOV_RAD ? MIN_CAM_FOV_RAD : getRad > MAX_CAM_FOV_RAD ? MAX_CAM_FOV_RAD : getRad;
	PerspectiveRH();
	UpdateWidthHeight();
}
void Camera::SetFarPlane(float farPlane_) {
	fp = farPlane_ > np ? farPlane_ : np;
	SetType(type);
}
void Camera::SetNearPlane(float nearPlane_) {
	np = nearPlane_ < MIN_CAM_NEARPLANE ? MIN_CAM_NEARPLANE : nearPlane_;
	SetType(type);
}
void Camera::SetSize(int size_) {
	size   = size_ <= 0 ? 1 : size_;
	OrthRH();
	UpdateWidthHeight();
}
void Camera::SetType(int type_) {
	switch (type_) {
	case CAM_PERSPECTIVE:
		type = CAM_PERSPECTIVE;
		PerspectiveRH();
		break;
	case CAM_ORTHO:
	default:
		type = CAM_ORTHO;
		OrthRH();
	}
	UpdateWidthHeight();
}

void Camera::LookAt(const Transform& target_) {
	owner->transform.CameraLookAt(target_);
}

//void Camera::SetPosition(float x_, float y_, float z_) {
//	transform->SetPosition(x_, y_, z_);
//}
//void Camera::SetPosition(const zg::Vector3& position_) {
//	transform->SetPosition(position_);
//}
//void Camera::Translate(float x_, float y_, float z_) {
//	transform->Translate(x_, y_, z_);
//}
//void Camera::Translate(const zg::Vector3& position_) {
//	transform->Translate(position_);
//}
//void Camera::SetRotation(float x_) {
//	transform->SetRotation(x_);
//}
//void Camera::SetRotation(float x_, float y_) {
//	transform->SetRotation(x_, y_);
//}
//void Camera::SetRotation(float x_, float y_, float z_) {
//	transform->SetRotation(x_, y_, z_);
//}
//void Camera::SetRotation(const zg::Vector3& rotation_) {
//	transform->SetRotation(rotation_);
//}
//void Camera::Rotate(float x_, float y_, float z_) {
//	transform->Rotate(x_, y_, z_);
//}
void Camera::RotateClamp(float x_, float y_, float z_) {
	zg::Vector3 eular{ owner->transform.rotation.ToEularDegree() + zg::Vector3{ x_,y_,z_ } };
	//transform->rotation.x += x_;
	//transform->rotation.y += y_;
	//transform->rotation.z += z_;
	//ClampAngleDeg(&transform->rotation.x);
	//ClampAngleDeg(&transform->rotation.y);
	//ClampAngleDeg(&transform->rotation.z);

	eular.x = zg::Clamp(eular.x, -89.0f, 89.0f);
	owner->transform.SetRotation(eular);
}

//void Camera::SetMaterial(const std::string & name)
//{
//	SetMaterial(GetHashFromString(name));
//}
//void Camera::SetMaterial(size_t hashedName)
//{
//  fboP = hashedName;
//  //fbo->SetMaterial(hashedName);
//
//  //auto& uniforms = fbo->material->shader->uniforms;
//  //uniformData.clear();
//  //
//  //for (auto& uniform : uniforms)
//  //{
//  //  std::string data;
//  //  switch (uniform.second)
//  //  {
//  //  case UNIFORM_FLOAT:
//  //    data = "0.0";
//  //    break;
//  //  case UNIFORM_FLOAT_VEC2:
//  //    data = "0.0 0.0";
//  //    break;
//  //  case UNIFORM_FLOAT_VEC3:
//  //    data = "0.0 0.0 0.0";
//  //    break;
//  //  case UNIFORM_FLOAT_VEC4:
//  //    data = "0.0 0.0 0.0 0.0";
//  //    break;
//  //  case UNIFORM_INT:
//  //    data = "0";
//  //    break;
//  //  case UNIFORM_BOOL:
//  //    data = "0";
//  //    break;
//  //  }
//  //
//  //  uniformData[uniform.first] = data;
//  //}
//}

//getters
float			Camera::GetFarPlane() const			{ return fp; }
float			Camera::GetNearPlane() const		{ return np; }
zg::Vector3		Camera::GetForward() const			{ return owner->transform.Forward(); }
zg::Vector3		Camera::GetUp() const				{ return owner->transform.Up(); }
zg::Vector3		Camera::GetRight() const			{ return owner->transform.Right(); }
float			Camera::GetFOVdeg() const			{ return TO_DEG(fovRAD);}
float			Camera::GetFOVrad() const			{ return fovRAD;}
float			Camera::GetAspectRatio() const		{ return aspectRatio;}
int				Camera::GetSize() const				{ return size; }
int				Camera::GetType() const				{ return type; }
float			Camera::GetViewportWidth() const	{ return width; }
float			Camera::GetViewportHeight() const	{ return height; }
//Color Camera::GetPixelColor(int x, int y) const
//{
//	//glBindFramebuffer(GL_FRAMEBUFFER, fbo->fbo);
//	//static GLubyte rgb[3];
//	//static Color result;
//	//glReadPixels(x, y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, rgb);
//	//glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
//	//result.SetColors(rgb[0], rgb[1], rgb[2]);
//	//return result;
//}
zg::Matrix44  Camera::ViewMatrix() const		{ return view; }

//void Camera::ResizeBuffer(int width_, int height_)
//{
//	//bWidth = width_;
//	//bHeight = height_;
//	//fbo->Resize(width_, height_, 0.f);
//}

zg::Matrix44  Camera::OrthongonalMatrix() const	{ 
	zg::Matrix44 pp = zg::Matrix44::Identity;

	pp.md[0][0] = 2.0f / (aspectWH >> 8) * size + 0.0f;	//(right - left);
	pp.md[1][1] = 2.0f / (aspectWH & 0x000000FF) * size + 0.0f;	// (top - bottom);
	pp.md[3][0] = 0.0f;				//-(right + left) / (right - left);
	pp.md[3][1] = 0.0f;				//-(top + bottom) / (top - bottom);

	pp.md[2][2] = -2.0f / (fp - np);
	pp.md[3][2] = -(fp + np) / (fp - np);

	return pp * view;
}
zg::Matrix44  Camera::ProjectionMatrix() const	{ return projection; }
zg::Matrix44  Camera::ProjViewMatrix() const	{ return projection * view; }

#if defined(_EDITOR_ON) || defined(_DEBUG)
void Camera::DebugDraw() const {
	switch (type) {
		case CAM_PERSPECTIVE: 
		{
			const float far_height = fp * tanf(fovRAD * 0.5f);
			const float far_width = far_height * aspectRatio;

			zg::Matrix44 rotate{ owner->transform.rotation.GetMatrix44() };
			zg::Vector3 trans = owner->transform.position;
			zg::Vector3 points[8]{
				trans + rotate * zg::Vector3{ -width, +height, -np },
				trans + rotate * zg::Vector3{ +width, +height, -np },
				trans + rotate * zg::Vector3{ +width, -height, -np },
				trans + rotate * zg::Vector3{ -width, -height, -np },
				trans + rotate * zg::Vector3{ -far_width,  +far_height,  -fp },
				trans + rotate * zg::Vector3{ +far_width,  +far_height,  -fp },
				trans + rotate * zg::Vector3{ +far_width,  -far_height,  -fp },
				trans + rotate * zg::Vector3{ -far_width,  -far_height,  -fp }
			};
			Gizmo::Frustum(points);
		}
		break;
		default:
		{
			zg::Matrix44 rotate{ owner->transform.rotation.GetMatrix44() };
			zg::Vector3 trans = owner->transform.position;
			zg::Vector3 points[8]{
				trans + rotate * zg::Vector3{ -width, +height, -np },
				trans + rotate * zg::Vector3{ +width, +height, -np },
				trans + rotate * zg::Vector3{ +width, -height, -np },
				trans + rotate * zg::Vector3{ -width, -height, -np },
				trans + rotate * zg::Vector3{ -width,  +height,  -fp },
				trans + rotate * zg::Vector3{ +width,  +height,  -fp },
				trans + rotate * zg::Vector3{ +width,  -height,  -fp },
				trans + rotate * zg::Vector3{ -width,  -height,  -fp }
			};
			Gizmo::Frustum(points);
			break;
		}
	}
}
#endif

void Camera::Update() {
	view = LookAtRH(owner->transform.position, owner->transform.position + owner->transform.Forward(), zg::Vector3::Vector3Y);
	//(this->*GetUpdate)();
	//owner->transform.ForceUpdate();
}

void Camera::UpdateWidthHeight() {
	switch (type) {
	case CAM_PERSPECTIVE:
		height = np * tanf(fovRAD * 0.5f);
		width  = height * aspectRatio;
		break;
	case CAM_ORTHO:
	default:
		width  = (aspectWH >> 8) * size + 0.0f;
		height = (aspectWH & 0x000000FF) * size + 0.0f;
	}
}

void Camera::OrthRH() {
	//projection.md[0][0] = 2.0f / Engine::sys_WinDisplay.mWidth;	//(right - left);
	//projection.md[1][1] = 2.0f / Engine::sys_WinDisplay.mHeight;	// (top - bottom);

	projection.md[0][0] = 2.0f / (aspectWH >> 8) * size + 0.0f;	//(right - left);
	projection.md[1][1] = 2.0f / (aspectWH & 0x000000FF) * size + 0.0f;	// (top - bottom);
	projection.md[3][0] = 0.0f;				//-(right + left) / (right - left);
	projection.md[3][1] = 0.0f;				//-(top + bottom) / (top - bottom);

	projection.md[2][2] = -2.0f / (fp - np);
	projection.md[3][2] = -(fp + np) / (fp - np);
}

void Camera::PerspectiveRH() {
	projection = zg::PerspectiveRH(fovRAD, aspectRatio, np, fp);
	//projection = ::FrustumRH(0, width, 0, height, np, fp);
	//const float tanHalfFovy = tan(fovRAD);
	//const float fn = fp - np;
	////projection = ::PerpestiveRH(fieldOfView, aspectRatio, nearPlane, farPlane);
	//projection.md[0][0] = 1.0f / (aspectRatio * tanHalfFovy);
	//projection.md[1][1] = 1.0f / (tanHalfFovy);
	//projection.md[2][3] = -1.0f;
	//projection.md[2][2] = -(fp + np) / fn;
	//projection.md[3][2] = -(2.0f * fp * np) / fn;
	//glm::perspective(fovRAD, aspectRatio, np, fp);
}

//serial
void Camera::Serial(FXMLDocument* doc, FXMLNode* root) {
	FXMLElement* pElem = doc->NewElement(IS_CAMERA);
	//Component
	Component::Serial(doc, pElem);
	//type
	FXMLElement* pSubElem = doc->NewElement("type");
	pSubElem->SetAttribute("num", (int)type);
	pElem->InsertEndChild(pSubElem);
	//type_aspectratio
	pSubElem = doc->NewElement("type_aspectratio");
	pSubElem->SetAttribute("num", type_aspectratio);
	pElem->InsertEndChild(pSubElem);
	//size
	pSubElem = doc->NewElement("size");
	pSubElem->SetAttribute("num", size);
	pElem->InsertEndChild(pSubElem);
	//attributes
	pSubElem = doc->NewElement("attributes");
	pSubElem->SetAttribute("fovDEG", TO_DEG(fovRAD));
	pSubElem->SetAttribute("np", np);
	pSubElem->SetAttribute("fp", fp);
	pElem->InsertEndChild(pSubElem);

	pSubElem = doc->NewElement("renderSettings");
	//pSubElem->SetAttribute("renderToTexture", renderToTexture);
	//pSubElem->SetAttribute("postprocessing", postprocessing);
	//pSubElem->SetAttribute("bufferWidth", bWidth);
	//pSubElem->SetAttribute("bufferHeight", bHeight);
	//pSubElem->SetAttribute("FBOMaterial", GetStringFromHash(fboP).c_str());
	//for (auto& elem : uniformData)
	//{
	//  auto name = GetStringFromHash(elem.first).c_str();
	//  pSubElem->SetAttribute(name, elem.second.c_str());
	//}
	pElem->InsertEndChild(pSubElem);

	root->InsertEndChild(pElem);
}
void Camera::Unserial(FXMLElement* pElem) {
	//Component
	Component::Unserial(pElem);
	//type
	FXMLElement* pSubElem = pElem->FirstChildElement("type");
	int getint = pSubElem->IntAttribute("num");
	switch (getint) {
		case 0: SetType(CAM_ORTHO); break;
		case 1: SetType(CAM_PERSPECTIVE); break;
		default:
			PopUp_Warn("Camera: No such type, Set default to CAM_PERSPECTIVE");
			SetType(CAM_PERSPECTIVE);
			break;
	}
	//type_aspectratio
	pSubElem = pSubElem->NextSiblingElement();
	getint = pSubElem->IntAttribute("num");
	switch (getint) {
		case 0: SetAspectRatio(CAM_4_3); break;
		case 1: SetAspectRatio(CAM_16_10); break;
		case 2: SetAspectRatio(CAM_16_9); break;
		default: 
			PopUp_Warn("Camera: No such Camera_AspectRatio, Set default to CAM_16_9");
			SetAspectRatio(CAM_16_9); break;
	}
	//size
	pSubElem = pSubElem->NextSiblingElement();
	SetSize(pSubElem->IntAttribute("num"));
	//attributes
	pSubElem	= pSubElem->NextSiblingElement();
	fovRAD		= TO_RAD(pSubElem->FloatAttribute("fovDEG"));
	np			= pSubElem->FloatAttribute("np");
	fp			= pSubElem->FloatAttribute("fp");

	pSubElem = pSubElem->NextSiblingElement();
	if (pSubElem)
	{
		//postprocessing  = pSubElem->BoolAttribute("postprocessing", true);
		//renderToTexture = pSubElem->BoolAttribute("renderToTexture");
		//bWidth          = pSubElem->UnsignedAttribute("bufferWidth");
		//bHeight         = pSubElem->UnsignedAttribute("bufferHeight");
		////SetMaterial("PostProcessing.material");
		//ResizeBuffer();
		//
		//for (auto& elem : uniformData)
		//{
		//	auto name = GetStringFromHash(elem.first).c_str();
		//	auto val = pSubElem->Attribute(name);
		//	
		//	if (val)
		//		elem.second = val;
		//}
	}

	Update();
}

#if defined(_USE_LUA_PUSH)
void Camera::BindLua(sol::state & lua)
{
	lua.new_usertype<Camera>("Camera",
    sol::constructors<Camera(), Camera(CameraType, Camera_AspectRatio), Camera(const Camera&)>(),
    //Component
    "IsEnable", &Camera::IsEnable,
    "SetEnable", &Camera::SetEnable,
    "Owner", &Camera::Owner,
    //Camera
    "GetTransform", &Camera::GetTransform,
    "SetTransform", &Camera::SetTransform,
    "SetAspectRatio", &Camera::SetAspectRatio,
    "SetFOV", &Camera::SetFOV,
    "SetFarPlane", &Camera::SetFarPlane,
    "SetNearPlane", &Camera::SetNearPlane,
    "SetSize", &Camera::SetSize,
    "SetType", &Camera::SetType,
    "LookAt", &Camera::LookAt,
    "GetPixel", &Camera::GetPixelColor,
    //"SetPosition", sol::overload(
    //	static_cast<void (Camera::*)(float, float, float)>(&Camera::SetPosition),
    //	static_cast<void (Camera::*)(const zg::Vector3&)>(&Camera::SetPosition)
    //),
    //"SetRotation", sol::overload(
    //	static_cast<void (Camera::*)(float)>(&Camera::SetRotation),
    //	static_cast<void (Camera::*)(float, float)>(&Camera::SetRotation),
    //	static_cast<void (Camera::*)(float, float, float)>(&Camera::SetRotation),
    //	static_cast<void (Camera::*)(const zg::Vector3&)>(&Camera::SetRotation)
    //),
    //"Translate", sol::overload(
    //	static_cast<void (Camera::*)(float, float, float)>(&Camera::Translate),
    //	static_cast<void (Camera::*)(const zg::Vector3&)>(&Camera::Translate)
    //),
    //"Rotate", &Camera::Rotate,
    "GetFarPlane", &Camera::GetFarPlane,
    "GetNearPlane", &Camera::GetNearPlane,
    "GetForward", &Camera::GetBack,
    "GetFront", &Camera::GetForward,
    "GetFOVdeg", &Camera::GetFOVdeg,
    "GetFOVrad", &Camera::GetFOVrad,
    "GetAspectRatio", &Camera::GetAspectRatio,
    "GetSize", &Camera::GetSize,
    "GetType", &Camera::GetType,
    "GetViewportWidth", &Camera::GetViewportWidth,
    "GetViewportHeight", &Camera::GetViewportHeight,
    "ProjectionMatrix", &Camera::ProjectionMatrix,
    "ProjViewMatrix", &Camera::ProjViewMatrix,
    "ViewMatrix", &Camera::ViewMatrix,
    "bWidth", &Camera::bWidth,
    "bHeight", &Camera::bHeight,
    "postprocessing", &Camera::postprocessing,
	  "SetUniformData", sol::overload(
		  static_cast<void (Camera::*)(const std::string&, bool)>(&Camera::SetUniformData),
		  static_cast<void (Camera::*)(const std::string&, float)>(&Camera::SetUniformData),
		  static_cast<void (Camera::*)(const std::string&, Vector2)>(&Camera::SetUniformData),
		  static_cast<void (Camera::*)(const std::string&, zg::Vector3)>(&Camera::SetUniformData),
		  static_cast<void (Camera::*)(const std::string&, Vector4)>(&Camera::SetUniformData)
	  ),
	  "GetUniformBool", &Camera::GetUniformBool,
	  "GetUniformFloat", &Camera::GetUniformFloat,
	  "GetUniformVec2", &Camera::GetUniformVec2,
	  "GetUniformVec3", &Camera::GetUniformVec3,
	  "GetUniformVec4", &Camera::GetUniformVec4
		);
}
#endif
//void Camera::SendUniforms(Material * material) const
//{
//  auto& uniforms = material->shader->uniforms;
//
//  material->SendBool("PostProcess", postprocessing);
//  material->SendVec2("resolution", Vector2{ (float)bWidth, (float)bHeight });
//  for (auto& data : uniformData)
//  {
//    switch (uniforms[data.first])
//    {
//    case UNIFORM_FLOAT:
//    {
//      static float f;
//      sscanf_s(data.second.c_str(), "%f", &f);
//      material->SendFloat(GetStringFromHash(data.first).c_str(), f);
//      break;
//    }
//    case UNIFORM_FLOAT_VEC2:
//    {
//      static Vector2 v;
//      sscanf_s(data.second.c_str(), "%f %f", &v.x, &v.y);
//      material->SendVec2(GetStringFromHash(data.first).c_str(), v);
//      break;
//    }
//    case UNIFORM_FLOAT_VEC3:
//    {
//      static zg::Vector3 v;
//      sscanf_s(data.second.c_str(), "%f %f %f", &v.x, &v.y, &v.z);
//      material->SendVec3(GetStringFromHash(data.first).c_str(), v);
//      break;
//    }
//    case UNIFORM_FLOAT_VEC4:
//    {
//      static Vector4 v;
//      sscanf_s(data.second.c_str(), "%f %f %f %f", &v.x, &v.y, &v.z, &v.w);
//      material->SendVec4(GetStringFromHash(data.first).c_str(), v);
//      break;
//    }
//    case UNIFORM_INT:
//    {
//      static int i;
//      sscanf_s(data.second.c_str(), "%d", &i);
//      material->SendInt(GetStringFromHash(data.first).c_str(), i);
//      break;
//    }
//    case UNIFORM_BOOL:
//    {
//      static int i;
//      sscanf_s(data.second.c_str(), "%d", &i);
//      material->SendBool(GetStringFromHash(data.first).c_str(), (bool)i);
//      break;
//    }
//    }
//  }
//}
//
//void Camera::SetUniformData(const std::string & name, bool val)
//{
//	unsigned hashedName = GetHashFromString(name);
//
//	auto it = uniformData.find(hashedName);
//
//	if (it != uniformData.end())
//	{
//		auto& uniforms = fbo->material->shader->uniforms;
//
//		if (uniforms[hashedName] == UNIFORM_BOOL)
//		{
//			static char buf[BUFSIZ];
//			ZeroMemory(buf, BUFSIZ);
//
//			sprintf_s(buf, BUFSIZ, "%d", val);
//
//			uniformData[hashedName] = buf;
//		}
//		else
//		{
//			printf("Invalid type! %s is not a bool!\n", name.c_str());
//		}
//	}
//	else
//		printf("Invalid uniform name! %s is not a material property!\n", name.c_str());
//}
//
//void Camera::SetUniformData(const std::string & name, float val)
//{
//	unsigned hashedName = GetHashFromString(name);
//
//	auto it = uniformData.find(hashedName);
//
//	if (it != uniformData.end())
//	{
//		auto& uniforms = fbo->material->shader->uniforms;
//
//		if (uniforms[hashedName] == UNIFORM_FLOAT)
//		{
//			static char buf[BUFSIZ];
//			ZeroMemory(buf, BUFSIZ);
//
//			sprintf_s(buf, BUFSIZ, "%f", val);
//
//			uniformData[hashedName] = buf;
//		}
//		else
//		{
//			printf("Invalid type! %s is not a float!\n", name.c_str());
//		}
//	}
//	else
//		printf("Invalid uniform name! %s is not a material property!\n", name.c_str());
//}
//
//void Camera::SetUniformData(const std::string & name, Vector2 val)
//{
//	unsigned hashedName = GetHashFromString(name);
//
//	auto it = uniformData.find(hashedName);
//
//	if (it != uniformData.end())
//	{
//		auto& uniforms = fbo->material->shader->uniforms;
//
//		if (uniforms[hashedName] == UNIFORM_FLOAT_VEC2)
//		{
//			static char buf[BUFSIZ];
//			ZeroMemory(buf, BUFSIZ);
//
//			sprintf_s(buf, BUFSIZ, "%f %f", val.x, val.y);
//
//			uniformData[hashedName] = buf;
//		}
//		else
//		{
//			printf("Invalid type! %s is not a Vector2!\n", name.c_str());
//		}
//	}
//	else
//		printf("Invalid uniform name! %s is not a material property!\n", name.c_str());
//}
//
//void Camera::SetUniformData(const std::string & name, zg::Vector3 val)
//{
//	unsigned hashedName = GetHashFromString(name);
//
//	auto it = uniformData.find(hashedName);
//
//	if (it != uniformData.end())
//	{
//		auto& uniforms = fbo->material->shader->uniforms;
//
//		if (uniforms[hashedName] == UNIFORM_FLOAT_VEC3)
//		{
//			static char buf[BUFSIZ];
//			ZeroMemory(buf, BUFSIZ);
//
//			sprintf_s(buf, BUFSIZ, "%f %f %f", val.x, val.y, val.z);
//
//			uniformData[hashedName] = buf;
//		}
//		else
//		{
//			printf("Invalid type! %s is not a zg::Vector3!\n", name.c_str());
//		}
//	}
//	else
//		printf("Invalid uniform name! %s is not a material property!\n", name.c_str());
//}
//
//void Camera::SetUniformData(const std::string & name, Vector4 val)
//{
//	unsigned hashedName = GetHashFromString(name);
//
//	auto it = uniformData.find(hashedName);
//
//	if (it != uniformData.end())
//	{
//		auto& uniforms = fbo->material->shader->uniforms;
//
//		if (uniforms[hashedName] == UNIFORM_FLOAT_VEC4)
//		{
//			static char buf[BUFSIZ];
//			ZeroMemory(buf, BUFSIZ);
//
//			sprintf_s(buf, BUFSIZ, "%f %f %f %f", val.x, val.y, val.z, val.w);
//
//			uniformData[hashedName] = buf;
//		}
//		else
//		{
//			printf("Invalid type! %s is not a Vector4!\n", name.c_str());
//		}
//	}
//	else
//		printf("Invalid uniform name! %s is not a material property!\n", name.c_str());
//}
//
//bool Camera::GetUniformBool(const std::string & name)
//{
//	int val = 0;
//	unsigned hashedName = GetHashFromString(name);
//
//	auto it = uniformData.find(hashedName);
//
//	if (it != uniformData.end())
//	{
//		auto& uniforms = fbo->material->shader->uniforms;
//
//		if (uniforms[hashedName] == UNIFORM_BOOL)
//		{
//			static char buf[BUFSIZ];
//			ZeroMemory(buf, BUFSIZ);
//
//			sscanf_s(it->second.c_str(), "%d", &val);
//			return (bool)val;
//		}
//		else
//		{
//			printf("Invalid type! %s is not a bool!\n", name.c_str());
//		}
//	}
//	else
//		printf("Invalid uniform name! %s is not a material property!\n", name.c_str());
//
//	return false;
//}
//
//float Camera::GetUniformFloat(const std::string & name)
//{
//	float val = std::numeric_limits<float>().min();
//	unsigned hashedName = GetHashFromString(name);
//
//	auto it = uniformData.find(hashedName);
//
//	if (it != uniformData.end())
//	{
//		auto& uniforms = fbo->material->shader->uniforms;
//
//		if (uniforms[hashedName] == UNIFORM_FLOAT)
//		{
//			static char buf[BUFSIZ];
//			ZeroMemory(buf, BUFSIZ);
//
//			sscanf_s(it->second.c_str(), "%f", &val);
//			return val;
//		}
//		else
//		{
//			printf("Invalid type! %s is not a float!\n", name.c_str());
//		}
//	}
//	else
//		printf("Invalid uniform name! %s is not a material property!\n", name.c_str());
//
//	return std::numeric_limits<float>().min();
//}
//
//Vector2 Camera::GetUniformVec2(const std::string & name)
//{
//	Vector2 val{ std::numeric_limits<float>().min(), std::numeric_limits<float>().min() };
//	unsigned hashedName = GetHashFromString(name);
//
//	auto it = uniformData.find(hashedName);
//
//	if (it != uniformData.end())
//	{
//		auto& uniforms = fbo->material->shader->uniforms;
//
//		if (uniforms[hashedName] == UNIFORM_FLOAT_VEC2)
//		{
//
//			static char buf[BUFSIZ];
//			ZeroMemory(buf, BUFSIZ);
//
//			sscanf_s(it->second.c_str(), "%f %f", &val.x, &val.y);
//		}
//		else
//		{
//			printf("Invalid type! %s is not a Vector2!\n", name.c_str());
//		}
//	}
//	else
//		printf("Invalid uniform name! %s is not a material property!\n", name.c_str());
//
//	return val;
//}
//
//zg::Vector3 Camera::GetUniformVec3(const std::string & name)
//{
//	zg::Vector3 val{ std::numeric_limits<float>().min(),
//				 std::numeric_limits<float>().min(),
//				 std::numeric_limits<float>().min() };
//
//	unsigned hashedName = GetHashFromString(name);
//
//	auto it = uniformData.find(hashedName);
//
//	if (it != uniformData.end())
//	{
//		auto& uniforms = fbo->material->shader->uniforms;
//
//		if (uniforms[hashedName] == UNIFORM_FLOAT_VEC3)
//		{
//			static char buf[BUFSIZ];
//			ZeroMemory(buf, BUFSIZ);
//
//			sscanf_s(it->second.c_str(), "%f %f %f", &val.x, &val.y, &val.z);
//		}
//		else
//		{
//			printf("Invalid type! %s is not a zg::Vector3!\n", name.c_str());
//		}
//	}
//	else
//		printf("Invalid uniform name! %s is not a material property!\n", name.c_str());
//
//	return val;
//}
//
//Vector4 Camera::GetUniformVec4(const std::string & name)
//{
//	Vector4 val{ std::numeric_limits<float>().min(),
//				 std::numeric_limits<float>().min(),
//				 std::numeric_limits<float>().min(),
//				 std::numeric_limits<float>().min() };
//
//	unsigned hashedName = GetHashFromString(name);
//
//	auto it = uniformData.find(hashedName);
//
//	if (it != uniformData.end())
//	{
//		auto& uniforms = fbo->material->shader->uniforms;
//
//		if (uniforms[hashedName] == UNIFORM_FLOAT_VEC4)
//		{
//			static char buf[BUFSIZ];
//			ZeroMemory(buf, BUFSIZ);
//
//			sscanf_s(it->second.c_str(), "%f %f %f %f", &val.x, &val.y, &val.z, &val.w);
//		}
//		else
//		{
//			printf("Invalid type! %s is not a Vector4!\n", name.c_str());
//		}
//	}
//	else
//		printf("Invalid uniform name! %s is not a material property!\n", name.c_str());
//
//	return val;
//}
