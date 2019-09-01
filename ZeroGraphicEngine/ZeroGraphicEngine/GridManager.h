/*****************************************************************************/
/*!
\file			GridManager3D.h
\brief			Grid Wrapper
*/
/*****************************************************************************/
#ifndef _GRID_MANAGER_H
#define _GRID_MANAGER_H

class Grid
{
public:
	Grid()
		:size{1.0f}
		, numberofgrid{ 20 }
		, color{ CC_BLACK }
	{}
	float size;
	int numberofgrid;
	zg::Color color;
	std::vector<zg::Vector3> points;
};

class GridManager3D : public Grid
{
public:
	GridManager3D() noexcept;
	~GridManager3D();
	//init
	void Create(int numberOfGrid_in_3D_space_, float girdsize = 1.0f);
	void SetSize(float size_);
	//setters
	void SetColor(const zg::Color& c);
	void Draw();
};

class GridManager2D : public Grid
{
public:
	GridManager2D() noexcept;
	~GridManager2D();
	//init
	void Create(int numberOfGrid_in_2D_space_, float girdsize = 1.0f, bool xy_true_xz_false_ = false);
	void SetSize(float size_);
	//setters
	void SetColor(const zg::Color& c);
	void Draw();
private:
	bool xyplane;
};

#endif //_GRID_MANAGER_H
