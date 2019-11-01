
#include "stdafx.h"
#include "GridManager.h"

GridManager3D::GridManager3D() noexcept
	:Grid{}
{}
GridManager3D::~GridManager3D(){
}

void GridManager3D::Create(int numOfGrid, float gridsize) {
	size = gridsize;
	numberofgrid = numOfGrid;
	float positive = numOfGrid * 0.5f * gridsize;
	float negative = -positive;
	points.resize(8);

	zg::Vector3 p0{ negative, negative, positive };
	zg::Vector3 p1{ positive, negative, positive };
	zg::Vector3 p2{ positive, positive, positive };
	zg::Vector3 p3{ negative, positive, positive };
	zg::Vector3 p4{ negative, negative, negative };
	zg::Vector3 p5{ positive, negative, negative };
	zg::Vector3 p6{ positive, positive, negative };
	zg::Vector3 p7{ negative, positive, negative };

	points[0] = p4;  points[1] = p7;
	points[2] = p5;  points[3] = p6;
	points[4] = p0;  points[5] = p3;
	points[6] = p1;  points[7] = p2;

	float y = positive;
	for (int j = 0; j <= numOfGrid; ++j) {
		float x = negative;
		float z = positive;
		for (int i = 0; i <= numOfGrid; ++i) {
			points.push_back(zg::Vector3{ x, y, -z });
			points.push_back(zg::Vector3{ x, y, z });
			x += gridsize;
		}
		x = positive;
		z = negative;
		for (int i = 0; i <= numOfGrid; ++i) {
			points.push_back(zg::Vector3{ -x, y, z });
			points.push_back(zg::Vector3{ x, y, z });
			z += gridsize;
		}
		y -= gridsize;
	}
}
void GridManager3D::SetSize(float size_) {
	points.clear();
	Create(numberofgrid, size_);
}

void GridManager3D::SetColor(const zg::Color& c) {
	color = c;
}
void GridManager3D::Draw() {
	Gizmo::Mesh(points, color);
}

GridManager2D::GridManager2D() noexcept
	:Grid{}
{}
GridManager2D::~GridManager2D() {}
void GridManager2D::Create(int numOfGrid, float gridsize, bool xy_true_xz_false_) {
	xyplane = xy_true_xz_false_;
	size = gridsize;
	numberofgrid = numOfGrid;
	float positive = numOfGrid * 0.5f * gridsize;
	float negative = -positive;
	points.resize(8);

	if (xyplane) {
		zg::Vector3 p0{ negative, negative, 0.0f };
		zg::Vector3 p1{ positive, negative, 0.0f };
		zg::Vector3 p2{ positive, positive, 0.0f };
		zg::Vector3 p3{ negative, positive, 0.0f };

		points[0] = p0;	points[1] = p1;
		points[2] = p1;	points[3] = p2;
		points[4] = p2;	points[5] = p3;
		points[6] = p3;	points[7] = p0;

		float x = negative + gridsize;
		float y = positive;
		for (int i = 1; i < numOfGrid; ++i) {
			points.push_back(zg::Vector3{ x, -y, 0.0f });
			points.push_back(zg::Vector3{ x,  y, 0.0f });
			x += gridsize;
		}
		x = positive;
		y = negative + gridsize;
		for (int i = 1; i < numOfGrid; ++i) {
			points.push_back(zg::Vector3{ -x, y, 0.0f });
			points.push_back(zg::Vector3{ x, y, 0.0f });
			y += gridsize;
		}
	}
	else {
		zg::Vector3 p0 { negative, 0.0f, negative };
		zg::Vector3 p1 { positive, 0.0f, negative };
		zg::Vector3 p2 { positive, 0.0f, positive };
		zg::Vector3 p3 { negative, 0.0f, positive };

		points.push_back(p0);	points.push_back(p1);
		points.push_back(p1);	points.push_back(p2);
		points.push_back(p2);	points.push_back(p3);
		points.push_back(p3);	points.push_back(p0);

		float x = negative + gridsize;
		float z = positive;
		for (int i = 1; i < numOfGrid; ++i) {
			points.push_back(zg::Vector3{ x, 0.0f, -z });
			points.push_back(zg::Vector3{ x, 0.0f, z });
			x += gridsize;
		}
		x = positive;
		z = negative + gridsize;
		for (int i = 1; i < numOfGrid; ++i) {
			points.push_back(zg::Vector3{ -x, 0.0f, z });
			points.push_back(zg::Vector3{ x, 0.0f, z });
			z += gridsize;
		}
	}
}

void GridManager2D::SetSize(float size_) {
	points.clear();
	Create(numberofgrid, size_);
}
void GridManager2D::SetColor(const zg::Color& c) {
	color = c;
}
void GridManager2D::Draw() {
	Gizmo::Mesh(points, color);
}

