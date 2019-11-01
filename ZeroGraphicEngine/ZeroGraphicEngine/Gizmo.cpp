
#include "stdafx.h"
#include "Gizmo.h"
#include "AssetsManager.h"
#include "zgconfig.h"

ZG_API Gizmo DebugGizmo{};
ZG_API zg::Color wireFrameColor = CC_BLACK;
std::vector<Gizmo::GizmoShape> Gizmo::list;

void HelpAddCircle(std::vector<zg::Vector3>& mSegments, const zg::Vector3& axis1, const zg::Vector3& axis2, const zg::Vector3& center = zg::Vector3{},
					float radius = 1.0f, float circle_revolve = 1.0f, float start_angle = 0.0f) {
	float angle = start_angle;
	const float incAngle = 360.0f / _GIZMO_NUMOFCIRCLEPTS * circle_revolve;
	for (int i = 0; i < _GIZMO_NUMOFCIRCLEPTS; ++i) {
		float radangle = TO_RAD(angle);
		float radangl1 = angle + incAngle;
		radangl1 = TO_RAD(radangl1);
		mSegments.emplace_back(zg::Vector3{ center + radius * (axis1 * std::cos(radangle) + axis2 * std::sin(radangle)) });
		mSegments.emplace_back(zg::Vector3{ center + radius * (axis1 * std::cos(radangl1) + axis2 * std::sin(radangl1)) });
		angle += incAngle;
	}
}
void HelpAddCircle(zg::Vector3* mSegments, const zg::Vector3& axis1, const zg::Vector3& axis2, const zg::Vector3& center = zg::Vector3{},
	float radius = 1.0f, float circle_revolve = 1.0f, float start_angle = 0.0f) {
	float angle = start_angle;
	const float incAngle = 360.0f / _GIZMO_NUMOFCIRCLEPTS * circle_revolve;

	zg::Vector3* tmp = mSegments;
	for (int i = 0; i < _GIZMO_NUMOFCIRCLEPTS; ++i) {
		float radangle = TO_RAD(angle);
		float radangl1 = angle + incAngle;
		radangl1 = TO_RAD(radangl1);
		*tmp = zg::Vector3{ center + radius * (axis1 * std::cos(radangle) + axis2 * std::sin(radangle)) };
		++tmp;
		*tmp = zg::Vector3{ center + radius * (axis1 * std::cos(radangl1) + axis2 * std::sin(radangl1)) };
		++tmp;
		angle += incAngle;
	}
}

Gizmo::Gizmo() noexcept
	:gizmo_shader(nullptr)
	, gizmo_no_trans_shader(nullptr)
	, VERTEX_ARRAY(0)
	, BUFF_OBJ(0)
	, cameraMatrix{}
{
}

Gizmo::~Gizmo(){
	glDeleteBuffers(1, &BUFF_OBJ);
	glDeleteVertexArrays(1, &VERTEX_ARRAY);
}

bool Gizmo::Init() {
	gizmo_shader			= FindShader("gizmo.glsl");
	gizmo_no_trans_shader	= FindShader("gizmo_no_trans.glsl");
	glGenVertexArrays(1, &VERTEX_ARRAY);
	glGenBuffers(1, &BUFF_OBJ);

	//Circle
	//std::vector<zg::Vector3> sphereSegment;
	//const zg::Vector3 xAxis{ 1.0f, 0.0f, 0.0f };
	//const zg::Vector3 yAxis{ 0.0f, 1.0f, 0.0f };
	//const zg::Vector3 zAxis{ 0.0f, 0.0f, 1.0f };
	//HelpAddCircle(sphereSegment, xAxis, yAxis);
	//glBindVertexArray(VERTEX_ARRAY[_GIZMO_CIRCLE]);
	//glBindBuffer(GL_ARRAY_BUFFER, BUFF_OBJ[_GIZMO_CIRCLE]);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(zg::Vector3) * sphereSegment.size(), &sphereSegment[0], GL_STATIC_DRAW);
	//
	////Sphere
	//glBindVertexArray(VERTEX_ARRAY[_GIZMO_SPHERE]);
	//glBindBuffer(GL_ARRAY_BUFFER, BUFF_OBJ[_GIZMO_SPHERE]);
	//HelpAddCircle(sphereSegment, yAxis, zAxis);
	//HelpAddCircle(sphereSegment, zAxis, xAxis);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(zg::Vector3) * sphereSegment.size(), &sphereSegment[0], GL_STATIC_DRAW);
	//
	////Cube
	//glBindVertexArray(VERTEX_ARRAY[_GIZMO_CUBE]);
	//glBindBuffer(GL_ARRAY_BUFFER, BUFF_OBJ[_GIZMO_CUBE]);
	//zg::Vector3 vCube[]{
	//	{ -0.5f, -0.5f,  0.5f }, //fleft btn
	//	{  0.5f, -0.5f,  0.5f }, //frigh btn
	//	{  0.5f,  0.5f,  0.5f }, //frigh top
	//	{ -0.5f,  0.5f,  0.5f }, //fleft top
	//	{ -0.5f,  0.5f, -0.5f }, //bleft top	
	//	{  0.5f,  0.5f, -0.5f }, //brigh top
	//	{  0.5f, -0.5f, -0.5f }, //brigh btn
	//	{ -0.5f, -0.5f, -0.5f }  //bleft btn
	//};
	//LineSegment aabb[]{
	//	{ vCube[0], vCube[1] },
	//	{ vCube[1], vCube[2] },
	//	{ vCube[2], vCube[3] },
	//	{ vCube[3], vCube[0] },
	//	{ vCube[4], vCube[5] },
	//	{ vCube[5], vCube[6] },
	//	{ vCube[6], vCube[7] },
	//	{ vCube[7], vCube[4] },
	//	{ vCube[0], vCube[7] },
	//	{ vCube[1], vCube[6] },
	//	{ vCube[2], vCube[5] },
	//	{ vCube[3], vCube[4] },
	//};
	//glBufferData(GL_ARRAY_BUFFER, sizeof(aabb), aabb, GL_STATIC_DRAW);
	
	//Ray
	//glBindVertexArray(VERTEX_ARRAY[_GIZMO_RAY]);
	//glBindBuffer(GL_ARRAY_BUFFER, BUFF_OBJ[_GIZMO_RAY]);
	//glBufferData(GL_ARRAY_BUFFER, 0, 0, GL_STATIC_DRAW);

	//Capsules
	//glBindVertexArray(VERTEX_ARRAY[_GIZMO_CAPSULE]);
	//glBindBuffer(GL_ARRAY_BUFFER, BUFF_OBJ[_GIZMO_CAPSULE]);
	//glBufferData(GL_ARRAY_BUFFER, 0, 0, GL_STATIC_DRAW);

	//unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	return true;
}

void Gizmo::Draw(Camera * cam) {
	glUseProgram(gizmo_no_trans_shader->programID);

	glUniformMatrix4fv(glGetUniformLocation(gizmo_no_trans_shader->programID, "projview"), 1, GL_FALSE, cam->ProjViewMatrix());
	glBindVertexArray(VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, BUFF_OBJ);

	glEnableVertexAttribArray(0);												//position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(zg::Vector3), 0);

	for (auto& elem : list) {
		glUniform4f(glGetUniformLocation(gizmo_no_trans_shader->programID, "color"), elem.color.r, elem.color.g, elem.color.b, elem.color.a);
		size_t sz = elem.sz;
		glBufferData(GL_ARRAY_BUFFER, sizeof(zg::Vector3) * sz, &elem.vertex[0], GL_STATIC_DRAW);
#ifdef _64BITMACHINE
		glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(sz));
#else
		glDrawArrays(GL_LINES, 0, sz);
#endif
		
	}

	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glUseProgram(0);

	list.clear();
}
//Line
void Gizmo::Line(const zg::Vector2& start_, const zg::Vector2& end_, const zg::Color& color_) {
	Line(zg::Vector3{ start_.x, start_.y, 0.0f }, zg::Vector3{ end_.x, end_.y, 0.0f }, color_);
}
void Gizmo::Line(const zg::Vector3& start_, const zg::Vector2& end_, const zg::Color& color_) {
	Line(start_, zg::Vector3{ end_.x, end_.y, 0.0f }, color_);
}
void Gizmo::Line(const zg::Vector3& start_, const zg::Vector3& end_, const zg::Color& color_) {
	zg::Vector3 line[2]{ start_, end_ };
	list.push_back(GizmoShape_Line{ line, color_ });
}
//Circle
void Gizmo::Circle(const zg::Vector3& center_, float radius_, const zg::Color& color_) {
	zg::Vector3 sphereSegment[_GIZMO_NUMOFCIRCLE_VERTEX];
	HelpAddCircle(sphereSegment, zg::Vector3::Vector3X, zg::Vector3::Vector3Y, center_, radius_);
	list.push_back(GizmoShape_Circle{ sphereSegment, color_ });
}
void Gizmo::Circle(const zg::Vector3& center_, float radius_, const zg::Vector3& axis_, const zg::Color& color_ ) {
	zg::Vector3 u;
	zg::Vector3 v;
	GenerateOrthonormalBasis(axis_, &u, &v);
	zg::Vector3 sphereSegment[_GIZMO_NUMOFCIRCLE_VERTEX];
	HelpAddCircle(sphereSegment, zg::Vector3::Vector3X, zg::Vector3::Vector3Y, center_, radius_);
	list.push_back(GizmoShape_Circle{ sphereSegment, color_ });
}
//Sphere
void Gizmo::Sphere(const zg::Vector3& center_, float radius_, const zg::Color& color_) {
	zg::Vector3 sphereSegment[_GIZMO_NUMOFSPHERE_VERTEX];
	HelpAddCircle(sphereSegment, zg::Vector3::Vector3X, zg::Vector3::Vector3Y, center_, radius_);
	HelpAddCircle(sphereSegment + _GIZMO_NUMOFCIRCLE_VERTEX, zg::Vector3::Vector3Y, zg::Vector3::Vector3Z, center_, radius_);
	HelpAddCircle(sphereSegment + _GIZMO_NUMOFCIRCLE_VERTEX + _GIZMO_NUMOFCIRCLE_VERTEX, zg::Vector3::Vector3Z, zg::Vector3::Vector3X, center_, radius_);
	list.push_back(GizmoShape_Sphere{ sphereSegment, color_ });
}
void Gizmo::Sphere(const std::vector<zg::Vector3>& points, float radius_, const zg::Color& color_) {
	for (auto elem : points) {
		Sphere(elem, radius_, color_);
	}
}
//aabb
void Gizmo::AABB(const zg::Vector3& min_, const zg::Vector3& max_, const zg::Color& color_){
	zg::Vector3 vCube[]{
		{ min_.x, min_.y,  max_.z }, //fleft btn	//3
		{ max_.x, min_.y,  max_.z }, //frigh btn	//2
		max_,						 //frigh top	//1
		{ min_.x, max_.y, max_.z },	 //fleft top	//0
		{ min_.x, max_.y, min_.z },	 //bleft top	//4
		{ max_.x, max_.y, min_.z },	 //brigh top	//5
		{ max_.x, min_.y, min_.z },	 //brigh btn	//6
		min_						 //bleft btn	//7
	};
	zg::Vector3 aabb[_GIZMO_NUMOF_AABB_PT]{
		{ vCube[0]} , { vCube[1] },
		{ vCube[1]} , { vCube[2] },
		{ vCube[2]} , { vCube[3] },
		{ vCube[3]} , { vCube[0] },
		{ vCube[4]} , { vCube[5] },
		{ vCube[5]} , { vCube[6] },
		{ vCube[6]} , { vCube[7] },
		{ vCube[7]} , { vCube[4] },
		{ vCube[0]} , { vCube[7] },
		{ vCube[1]} , { vCube[6] },
		{ vCube[2]} , { vCube[5] },
		{ vCube[3]} , { vCube[4] }
	};
	list.push_back(GizmoShape_AABB{ aabb, color_ });
}
//box
void Gizmo::Box(const Transform& transform_, const zg::Color& color_) {
	zg::Matrix44 matrix;
	matrix = BuildTRS4x4(transform_.position, transform_.rotation, transform_.scale);
	Box(matrix, color_);
}
void Gizmo::Box(zg::Matrix44& matrix_, const zg::Color& color_) {
	zg::Vector3 vCube[]{
		matrix_ * zg::Vector3{ -0.5f, -0.5f,  0.5f }, //fleft btn
		matrix_ * zg::Vector3{  0.5f, -0.5f,  0.5f }, //frigh btn
		matrix_ * zg::Vector3{  0.5f,  0.5f,  0.5f }, //frigh top
		matrix_ * zg::Vector3{ -0.5f,  0.5f,  0.5f }, //fleft top
		matrix_ * zg::Vector3{ -0.5f,  0.5f, -0.5f }, //bleft top	
		matrix_ * zg::Vector3{  0.5f,  0.5f, -0.5f }, //brigh top
		matrix_ * zg::Vector3{  0.5f, -0.5f, -0.5f }, //brigh btn
		matrix_ * zg::Vector3{ -0.5f, -0.5f, -0.5f }  //bleft btn
	};
	zg::Vector3 aabb[_GIZMO_NUMOF_AABB_PT]{
		{ vCube[0]} , { vCube[1] },
		{ vCube[1]} , { vCube[2] },
		{ vCube[2]} , { vCube[3] },
		{ vCube[3]} , { vCube[0] },
		{ vCube[4]} , { vCube[5] },
		{ vCube[5]} , { vCube[6] },
		{ vCube[6]} , { vCube[7] },
		{ vCube[7]} , { vCube[4] },
		{ vCube[0]} , { vCube[7] },
		{ vCube[1]} , { vCube[6] },
		{ vCube[2]} , { vCube[5] },
		{ vCube[3]} , { vCube[4] }
	};
	list.push_back(GizmoShape_AABB{ aabb, color_ });
}
void Gizmo::Box(const zg::Vector3& center_, float uniform_scale_, const zg::Color& color_) {
	zg::Vector3 scale{ uniform_scale_, uniform_scale_, uniform_scale_ };
	scale *= 0.5f;
	zg::Vector3 max = center_ + scale;	//top right
	zg::Vector3 min = center_ - scale;	//bottom left
	AABB(min, max, color_);
}
void Gizmo::Box(const zg::Vector3& center_, const zg::Vector3& scale_, const zg::Color& color_) {
	zg::Vector3 scale = scale_ * 0.5f;
	zg::Vector3 max = center_ + scale;	//top right
	zg::Vector3 min = center_ - scale;	//bottom left
	AABB(min, max, color_);
}

void Gizmo::Ray(const zg::Vector3& start_, const zg::Vector3& direction_, float t, float radius_ , const zg::Color& color_) {
	//line
	zg::Vector3 mSegments[2 + _GIZMO_NUMOFCIRCLE_VERTEX + _GIZMO_NUMOFCIRCLE_VERTEX]{};
	mSegments[0] = start_;
	mSegments[1] = start_ + direction_ * t;

	//Arrao Ring
	zg::Vector3 xAxis{};
	zg::Vector3 zAxis{};
	GenerateOrthonormalBasis(direction_, &xAxis, &zAxis);
	zg::Vector3 arrowcenter{ start_ + direction_ * t * 0.9f };
	HelpAddCircle(mSegments + 2, xAxis, zAxis, arrowcenter, radius_);

	//Arrao Head
	zg::Vector3* tmp = mSegments + 2 + _GIZMO_NUMOFCIRCLE_VERTEX;
	const float incAngle = 360.0f / _GIZMO_NUMOFCIRCLEPTS;
	float angle = 0.0f;
	for (int i = 0; i < _GIZMO_NUMOFCIRCLEPTS; ++i) {
		float radangle = TO_RAD(angle);
		float radangle1 = angle + incAngle;
		radangle1 = TO_RAD(radangle1);
		*tmp = zg::Vector3{ arrowcenter + radius_ * (xAxis * std::cos(radangle) + zAxis * std::sin(radangle)) };
		++tmp;
		*tmp = mSegments[1];
		++tmp;
		angle += incAngle;
	}
	list.push_back(GizmoShape_Ray{ mSegments, color_ });
}
//capsule
void Gizmo::Capsule(const zg::Vector3& center_, float halfheight, float radius, const zg::Color& color_) {
	//line
	std::vector<zg::Vector3> mSegments;
	zg::Vector3 p0{ center_.x, center_.y + halfheight, center_.z };
	zg::Vector3 p1{ center_.x, center_.y - halfheight, center_.z };
	
	zg::Vector3 p2{ center_.x + radius, center_.y + halfheight, center_.z };
	zg::Vector3 p3{ center_.x + radius, center_.y - halfheight, center_.z };
	zg::Vector3 p4{ center_.x - radius, center_.y + halfheight, center_.z };
	zg::Vector3 p5{ center_.x - radius, center_.y - halfheight, center_.z };
	zg::Vector3 p6{ center_.x, center_.y + halfheight, center_.z + radius };
	zg::Vector3 p7{ center_.x, center_.y - halfheight, center_.z + radius };
	zg::Vector3 p8{ center_.x, center_.y + halfheight, center_.z - radius };
	zg::Vector3 p9{ center_.x, center_.y - halfheight, center_.z - radius };
	
	//mSegments.emplace_back(p0); mSegments.emplace_back(p1);
	mSegments.emplace_back(p2); mSegments.emplace_back(p3);
	mSegments.emplace_back(p4); mSegments.emplace_back(p5);
	mSegments.emplace_back(p6); mSegments.emplace_back(p7);
	mSegments.emplace_back(p8); mSegments.emplace_back(p9);
	
	const zg::Vector3 xAxis{ 1.0f, 0.0f, 0.0f };
	const zg::Vector3 yAxis{ 0.0f, 1.0f, 0.0f };
	const zg::Vector3 zAxis{ 0.0f, 0.0f, 1.0f };
	//top half circle
	HelpAddCircle(mSegments, xAxis, zAxis, p0, radius);
	HelpAddCircle(mSegments, xAxis, yAxis, p0, radius, 0.5f);
	HelpAddCircle(mSegments, yAxis, zAxis, p0, radius, 0.5f,-90.0f);
	//btn half circle
	HelpAddCircle(mSegments, xAxis, zAxis, p1, radius);
	HelpAddCircle(mSegments, xAxis, yAxis, p1, radius, 0.5f, -180.0f);
	HelpAddCircle(mSegments, yAxis, zAxis, p1, radius, 0.5f, 90.0f);
	list.push_back(GizmoShape_Mesh{ mSegments, color_ });
}

void Gizmo::Frustum(const zg::Vector3 points[8], const zg::Color& color_) {
	const zg::Vector3 mSegments[_GIZMO_NUMOF_AABB_PT]{
		points[0], points[1],
		points[1], points[2],
		points[2], points[3],
		points[3], points[0],

		points[4], points[5],
		points[5], points[6],
		points[6], points[7],
		points[7], points[4],

		points[0], points[4],
		points[1], points[5],
		points[2], points[6],
		points[3], points[7]
	};
	list.push_back(GizmoShape_AABB{ mSegments, color_ });
}


void Gizmo::Mesh(const std::vector<zg::Vector3>& points, const zg::Color& color_) {
	list.push_back(GizmoShape_Mesh{ points, color_ });
}
