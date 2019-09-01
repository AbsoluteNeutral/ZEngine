#ifndef _GENERATECURVE_H
#define _GENERATECURVE_H

#include <map>
#include <vector>
#include "ZGMath.h"

namespace zg {

	class ZG_API CurvePath {
	public:
		CurvePath	() noexcept;

		void ClearGizmoTable				();
		void GeneratePathPointOnCurve		(const std::vector<zg::Vector3>& listOfPointsOnCurve_, float cuspsDelta_ = 2.0f, int dimension_ = 3);
		void GeneratePathUsingControlPoint	(const std::vector<zg::Vector3>& listOfControlPoints_, int dimension_ = 3);

		virtual void	DrawCurve			();

		virtual Vector3	GetPointOnCurve		(float time_) = 0;
		virtual void	GenerateGizmoTable	(int numberOfPoints_) = 0;

		std::vector<Vector3> ListOfPoint;	//list of points on curve / control points
		std::vector<float> u;				//list of time intervals
	protected:
		std::vector<Vector3> ListOfGizmoPoints;
		unsigned dimension;					//the dimensionlity
		int numberOfPointsOnCurve;
	};
	
	//______________________________________________________________________ Bernstein polynormial
	class ZG_API Bezier_BernsteinPolynormial : public CurvePath {
	public:
		Bezier_BernsteinPolynormial			() noexcept;

		virtual void	DrawCurve			();
		virtual Vector3	GetPointOnCurve		(float time_);
		virtual void	GenerateGizmoTable	(int numberOfPoints_ = 60);
	private:
		std::vector<float> binomialCoeff;
	};

	//______________________________________________________________________ de Casteljau
	class ZG_API Bezier_deCasteljau : public CurvePath {
	public:
		Bezier_deCasteljau() noexcept;

		void	DrawCurve						(float time_, bool toggleShell_ = true);
		virtual Vector3	GetPointOnCurve			(float time_);
		virtual void	GenerateGizmoTable		(int numberOfPoints_ = 60);
	};

	class ZG_API BezierCubic : public CurvePath {
	public:
		BezierCubic() noexcept;

		virtual Vector3	GetPointOnCurve			(float time_);
				Vector3	GetPointOnCurve			(float time_, Vector3* getvelocityvector_);
		virtual void	GenerateGizmoTable		(int numberOfPoints_ = 60);

		
		void			GenerateArcLengthTable		();
		void			GenerateFixedArcLengthTable	(int fixframeRate_ = 60);
		
		float			GetTimeU					(float arclength_normalize_);
		float			GetTimeUFromFixedTable		(float arclength_normalize_);
		float			GetTimeUFromArcLengthTable	(float arclength_normalize_);

		float			GetTotalArcLength();
	private:
		float			GetArcLengthFromTable(size_t key_, float timeu_);
		float			GetArcLength(size_t lineSegment_, float u_);
		float			Integrate(size_t lineSegment_, float a_, float b_);

		void			GenerateArcLengthTableRe(size_t segment_, float timeL_, float timeR_, Vector3&pA, Vector3& pB, float sA, float epi_);

		std::map<size_t, std::vector<float>> ArcLengthTable;	//line segment, arclength
		//std::vector<float> ArcLengthSeg;					//line segment, arclength
		//std::vector<std::map<float, float>> ArcLengthSeg;		//line segment, arclength
		//std::vector<std::vector<float>> ArcLengthSeg;			//line segment, arclength
		//std::map<float, float> ArcLengthSeg;					//line segment, arclength
		std::vector<float> ArcLengthSegU;					//line segment, arclength
		std::vector<float> ArcLengthSegvector;					//line segment, arclength
	};

	//______________________________________________________________________ internal functions
	ZG_API float				GetLamda			(float t_, float r_, float s_);
	ZG_API Vector3				Lerp				(const Vector3& start_, const Vector3& end_, float t_, float r_, float s_);
	ZG_API std::vector<float>	GetBinomialPower	(int power_);

	ZG_API float GetSineInterpolation(float time_, float left_, float right_);
}//namespace zg

#endif
