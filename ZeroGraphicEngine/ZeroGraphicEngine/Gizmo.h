/*****************************************************************************/
/*!
\file			Gizmo.h
\brief			Debug Draw of different shapes
*/
/*****************************************************************************/
#ifndef _GIZMO_H
#define _GIZMO_H

#include "IShader.h"
#include "ZGMath.h"

#define _GIZMO_NUMOFCIRCLEPTS 12
static const unsigned _gizmo_numOfcircle_vertex = _GIZMO_NUMOFCIRCLEPTS * 2;
#define _GIZMO_NUMOFCIRCLE_VERTEX _gizmo_numOfcircle_vertex
static const unsigned _gizmo_numOfSpherept = _GIZMO_NUMOFCIRCLEPTS * 3;
static const unsigned _gizmo_numOfSpherept_vertex = _gizmo_numOfSpherept * 2;
#define _GIZMO_NUMOFSPHERE _gizmo_numOfSpherept
#define _GIZMO_NUMOFSPHERE_VERTEX _gizmo_numOfSpherept_vertex
#define _GIZMO_NUMOF_AABB_PT 24

class ZG_API Gizmo{
	class GizmoShape {
	public:
		GizmoShape(const zg::Vector3* vertices, const zg::Color& c, size_t size)
			: color(c), sz(size), vertex{}
		{
			vertex.resize(sz);
			zg::CopyVector3Array(vertex, vertices, sz);
		} 
		GizmoShape(const std::vector<zg::Vector3>& vertices, const zg::Color& c, size_t size)
			: color(c), sz(size), vertex{ vertices }
		{}
		zg::Vector3* GetVertices() { 
			return &vertex[0]; 
		};
		zg::Color color;
		size_t sz;
		std::vector<zg::Vector3> vertex;
	};// ========== ========== ========== ========== ========== ========== BASE class
	class GizmoShape_Line : public GizmoShape {
	public:
		GizmoShape_Line(const zg::Vector3* vertices, const zg::Color& color_)
			:GizmoShape(vertices, color_, 2)
		{}
	};// ========== ========== ========== ========== ========== ========== LINE
	class GizmoShape_Circle : public GizmoShape {
	public:
		GizmoShape_Circle(const zg::Vector3* vertices, const zg::Color& color_)
			:GizmoShape(vertices, color_, _GIZMO_NUMOFCIRCLE_VERTEX)
		{}
	};// ========== ========== ========== ========== ========== ========== CIRCLE
	class GizmoShape_Sphere : public GizmoShape {
	public:
		GizmoShape_Sphere(const zg::Vector3* vertices, const zg::Color& color_)
			:GizmoShape(vertices, color_, _GIZMO_NUMOFSPHERE_VERTEX)
		{}
	};// ========== ========== ========== ========== ========== ========== SPHERE
	class GizmoShape_AABB : public GizmoShape {
	public:
		GizmoShape_AABB(const zg::Vector3* vertices, const zg::Color& color_)
			:GizmoShape(vertices, color_, _GIZMO_NUMOF_AABB_PT)
		{}
	};// ========== ========== ========== ========== ========== ========== AABB
	class GizmoShape_Ray: public GizmoShape {
	public:
		GizmoShape_Ray(const zg::Vector3* vertices, const zg::Color& color_)
			:GizmoShape(vertices, color_, 2 + _GIZMO_NUMOFCIRCLE_VERTEX + _GIZMO_NUMOFCIRCLE_VERTEX)
		{}
	};// ========== ========== ========== ========== ========== ========== Ray
	class GizmoShape_Mesh : public GizmoShape {
	public:
		GizmoShape_Mesh(const std::vector<zg::Vector3>& vertices, const zg::Color& color_)
			:GizmoShape(vertices, color_, vertices.size())
		{}
	};// ========== ========== ========== ========== ========== ========== Ray
	Gizmo& operator=(const Gizmo&);
	Gizmo(const Gizmo&);
public:
	Gizmo() noexcept;
	~Gizmo();

	bool Init();
	void Draw(Camera * cam);

	//line
	static void Line	(const zg::Vector2& start_, const zg::Vector2& end_, const zg::Color& color_ = CC_GREEN_3);
	static void Line	(const zg::Vector3& start_, const zg::Vector2& end_, const zg::Color& color_ = CC_GREEN_3);
	static void Line	(const zg::Vector3& start_, const zg::Vector3& end_, const zg::Color& color_ = CC_GREEN_3);
	//circle
	static void Circle	(const zg::Vector3& center_, float radius_, const zg::Color& color_ = CC_GREEN_3);
	static void Circle	(const zg::Vector3& center_, float radius_, const zg::Vector3& axis_, const zg::Color& color_ = CC_GREEN_3);
	//sphere
	static void Sphere	(const zg::Vector3& center_, float radius_, const zg::Color& color_ = CC_GREEN_3);
	static void Sphere	(const std::vector<zg::Vector3>& points, float radius_, const zg::Color& color_ = CC_GREEN_3);
	//aabb
	static void AABB	(const zg::Vector3& min_, const zg::Vector3& max_, const zg::Color& color_ = CC_GREEN_3);
	//box
	static void Box		(const Transform& transform_,	const zg::Color& color_ = CC_GREEN_3);
	static void Box		(zg::Matrix44& marix_,	const zg::Color& color_ = CC_GREEN_3);
	static void Box		(const zg::Vector3& center_,	const zg::Vector3& scale_, const zg::Color& color_ = CC_GREEN_3);
	static void Box		(const zg::Vector3& center_,	float uniform_scale_,	const zg::Color& color_ = CC_GREEN_3);
	//ray
	static void Ray		(const zg::Vector3& start_,	const zg::Vector3& direction_, float t_ = 2.0f, float radius_ = 0.1f, const zg::Color& color_ = CC_GREEN_3);
	//capsule
	static void Capsule	(const zg::Vector3& center_, float halfheight, float radius, const zg::Color& color_ = CC_GREEN_3);
	//Frustum
	static void Frustum (const zg::Vector3 points[8], const zg::Color& color_ = CC_GREEN_3);
	//Mesh
	static void Mesh	(const std::vector<zg::Vector3>& points, const zg::Color& color_ = CC_BLACK);
private:
	IShader* gizmo_shader;
	IShader* gizmo_no_trans_shader;
	unsigned VERTEX_ARRAY;
	unsigned BUFF_OBJ;
	zg::Matrix44 cameraMatrix;
	
	static std::vector<GizmoShape> list;
};

ZG_API extern Gizmo DebugGizmo;
ZG_API extern zg::Color wireFrameColor;

#endif //_GIZMO_H
