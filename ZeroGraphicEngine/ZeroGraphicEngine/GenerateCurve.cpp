#include "stdafx.h"
#include "GenerateCurve.h"
#include "zgconfig.h"

namespace zg {

	CurvePath::CurvePath() noexcept
		: ListOfPoint{}
		, u{}
		, ListOfGizmoPoints{}
		, dimension(3)
		, numberOfPointsOnCurve(1)
	{}
	void CurvePath::ClearGizmoTable() {
		ListOfGizmoPoints.clear();
	}
	void CurvePath::GeneratePathPointOnCurve(const std::vector<zg::Vector3>& listOfPointsOnCurve_, float cuspsDelta_, int dimension_)
	{
		GeneratePathUsingControlPoint(listOfPointsOnCurve_, dimension_);

		size_t sz = listOfPointsOnCurve_.size() - 1;
		auto b = ListOfPoint.begin() + 1;
		b = ListOfPoint.insert(b, *(b - 1)) + 1;

		for (size_t i = 1; i < sz; ++i)
		{
			zg::Vector3 pi = *b;

			//zg::Vector3 ai = pi + (*(b + 1) - *(b - 1)) / cuspsDelta_;
			//zg::Vector3 bi = pi - (*(b + 1) - *(b - 1)) / cuspsDelta_;

			zg::Vector3 p = pi + (pi - *(b - 1));
			zg::Vector3 ai = (p + *(b + 1)) / cuspsDelta_;
			zg::Vector3 bi = pi + (pi - ai);

			b = ListOfPoint.insert(b, bi);
			b = ListOfPoint.insert(b + 2, ai);
			++b;
		}
		ListOfPoint.insert(ListOfPoint.end() - 1, *(ListOfPoint.end() - 1));
	}
	void CurvePath::GeneratePathUsingControlPoint(const std::vector<zg::Vector3>& listOfControlPoints_, int dimension_)
	{
		ListOfPoint = listOfControlPoints_;
#ifdef _64BITMACHINE
		numberOfPointsOnCurve = static_cast<int>(listOfControlPoints_.size());
#else
		numberOfPointsOnCurve = listOfControlPoints_.size();
#endif
		
		float sz = static_cast<float>(numberOfPointsOnCurve);
		
		dimension = dimension_;
		//int getSizeOfU = MIN2(dimension_, listOfControlPoints_.size() % dimension_);
		for (float i = 0.0f; i < sz; i += 1.0f)
		{
			u.push_back(i);
		}
	}
	void CurvePath::DrawCurve()
	{
		size_t sz = ListOfPoint.size() - 1;

		Gizmo::Sphere(ListOfPoint[0], 0.25f, CC_ORANGE);
		Gizmo::Sphere(ListOfPoint[1], 0.25f, CC_GREEN);
		Gizmo::Line(ListOfPoint[0], ListOfPoint[1], CC_GREEN);
		
		for (size_t i = 3; i < sz; i += 3)
		{
			Gizmo::Sphere(ListOfPoint[i], 0.25f, CC_ORANGE);
			Gizmo::Sphere(ListOfPoint[i - 1], 0.15f, CC_GREEN); //ai
			Gizmo::Sphere(ListOfPoint[i + 1], 0.15f, CC_GREEN); //bi

			Gizmo::Line(ListOfPoint[i], ListOfPoint[i - 1], CC_GREEN); //ai to pi
			Gizmo::Line(ListOfPoint[i], ListOfPoint[i + 1], CC_GREEN); //bi to pi

			//if (i > 2)
			//	Gizmo::Line(ListOfPoint[i], ListOfPoint[i - 3], CC_BLUE);
		}
		Gizmo::Line(ListOfPoint[sz], ListOfPoint[sz - 2], CC_BLUE);
		Gizmo::Sphere(ListOfPoint[sz], 0.25f, CC_ORANGE);

		//Draw curve
		sz = ListOfGizmoPoints.size();
		if (sz > 1)
		{
			for (size_t i = 1; i < sz; ++i) {
				Gizmo::Line(ListOfGizmoPoints[i - 1], ListOfGizmoPoints[i], CC_BLUE);
			}
		}
	}

	//______________________________________________________________________ Bernstein polynormial
#pragma region Bernstein polynormial
	Bezier_BernsteinPolynormial::Bezier_BernsteinPolynormial() noexcept
		: CurvePath{}
	{}
	Vector3 Bezier_BernsteinPolynormial::GetPointOnCurve(float time_)
	{
		int whichSeg = (int)std::floor(time_) * dimension;
		time_ -= std::floor(time_);

		size_t szOfPoint = ListOfPoint.size();
#ifdef _64BITMACHINE
		binomialCoeff = GetBinomialPower(static_cast<int>(MIN2(szOfPoint, dimension + 1)));
#else
		binomialCoeff = GetBinomialPower(MIN2(szOfPoint, dimension + 1));
#endif
		size_t szOfBinomial = binomialCoeff.size();

		float lamda = GetLamda(time_, 0.0f, 1.0f);
		float oneMinuslamda = 1.0f - lamda;
		std::vector<float> lamdaPow;
		std::vector<float> oneMinuslamdaPow;
		lamdaPow.push_back(1);
		oneMinuslamdaPow.push_back(1);
		for (size_t i = 1; i < szOfBinomial; ++i)
		{
			lamdaPow.push_back(lamdaPow[i - 1] * lamda);
			oneMinuslamdaPow.push_back(oneMinuslamdaPow[i - 1] * oneMinuslamda);
		}
		std::reverse(oneMinuslamdaPow.begin(), oneMinuslamdaPow.end());
		
		Vector3 pointOnCurve;
		for (size_t i = 0; i < szOfBinomial; ++i)
		{
			pointOnCurve += oneMinuslamdaPow[i] * lamdaPow[i] * binomialCoeff[i] * ListOfPoint[i + whichSeg];
		}
		return pointOnCurve;
	}
	void Bezier_BernsteinPolynormial::DrawCurve()
	{
		CurvePath::DrawCurve();
		//for (auto elem : ListOfPoint) {
		//	Gizmo::Sphere(elem, 0.25f, CC_ORANGE);
		//}
		//size_t sz = ListOfGizmoPoints.size();
		//if (sz > 1)
		//{
		//	for (size_t i = 1; i < sz; ++i) {
		//		Gizmo::Line(ListOfGizmoPoints[i - 1], ListOfGizmoPoints[i], CC_BLUE);
		//	}
		//	//Gizmo::Line(ListOfGizmoPoints[0], ListOfGizmoPoints[sz - 1], CC_BLUE);
		//}
	}

	void Bezier_BernsteinPolynormial::GenerateGizmoTable(int numberOfPoints_)
	{
		ListOfGizmoPoints.clear();

		size_t szOfPoint = ListOfPoint.size();
#ifdef _64BITMACHINE
		binomialCoeff = GetBinomialPower(static_cast<int>(MIN2(szOfPoint, dimension + 1)));
#else
		binomialCoeff = GetBinomialPower(MIN2(szOfPoint, dimension + 1));
#endif
		size_t szOfBinomial = binomialCoeff.size();
		--szOfPoint;

		for (size_t k = 0; k < szOfPoint; k += dimension)
		{
			float tmin = 0.0f;
			float tmax = 1.0f;
			float a = tmin;
			float b = (tmax - tmin) / numberOfPoints_;

			for (int j = 0; j < numberOfPoints_; ++j)
			{
				//pre compute lamda 
				float lamda = GetLamda(a, 0.0f, 1.0f);
				float oneMinuslamda = 1.0f - lamda;
				std::vector<float> lamdaPow;
				std::vector<float> oneMinuslamdaPow;
				lamdaPow.push_back(1);
				oneMinuslamdaPow.push_back(1);
				for (size_t i = 1; i < szOfBinomial; ++i)
				{
					lamdaPow.push_back(lamdaPow[i - 1] * lamda);
					oneMinuslamdaPow.push_back(oneMinuslamdaPow[i - 1] * oneMinuslamda);
				}
				std::reverse(oneMinuslamdaPow.begin(), oneMinuslamdaPow.end());

				//compute point on curve
				Vector3 apoint;
				for (size_t i = 0; i < szOfBinomial; ++i)
				{
					apoint += oneMinuslamdaPow[i] * lamdaPow[i] * binomialCoeff[i] * ListOfPoint[i + k];
				}
				ListOfGizmoPoints.push_back(apoint);
				a += b;
			}
		}
	}
#pragma endregion

	//______________________________________________________________________ Bezier_deCasteljau
#pragma region Bezier_deCasteljau
	Bezier_deCasteljau::Bezier_deCasteljau() noexcept
		: CurvePath{}
	{}
	void Bezier_deCasteljau::DrawCurve(float time_, bool toggleShell_)
	{
		for (auto elem : ListOfPoint) {
			Gizmo::Sphere(elem, 0.25f, CC_ORANGE);
		}
		size_t sz = ListOfGizmoPoints.size();
		if (sz > 1)
		{
			for (size_t i = 1; i < sz; ++i) {
				Gizmo::Line(ListOfGizmoPoints[i - 1], ListOfGizmoPoints[i], CC_BLUE);
			}
			//Gizmo::Line(ListOfGizmoPoints[0], ListOfGizmoPoints[sz - 1], CC_BLUE);
		}

		if (toggleShell_) 
		{
			int whichSeg = (int)std::floor(time_) * dimension;
			time_ -= std::floor(time_);

			std::vector<zg::Vector3> tmpVectors;
			std::vector<std::vector<zg::Vector3>> stages;
			int numberofStage = 0;
			std::vector<std::vector<float>> tstages;
			tstages.push_back(std::vector<float>{});

			sz = dimension + 1;
			for (size_t i = 0; i < sz; ++i)
			{
				tmpVectors.push_back(ListOfPoint[i + whichSeg]);
				tstages[numberofStage].push_back(0.0f);
			}
			stages.push_back(tmpVectors);

			sz = tmpVectors.size();
			while (tmpVectors.size() > 1)
			{
				std::vector<zg::Vector3> currVectors;
				tstages.push_back(std::vector<float>{});
				for (size_t i = 1; i < sz; ++i)
				{
					currVectors.push_back(Lerp(tmpVectors[i - 1], tmpVectors[i - 0], time_, 0.0f, 1.0f));
					tstages[numberofStage + 1].push_back(GetLamda(time_, 0.0f, 1.0f));
				}
				tmpVectors = currVectors;
				stages.push_back(tmpVectors);
				++numberofStage;
				sz = tmpVectors.size();
			}

			sz = stages.size();
			for (size_t i = 0; i < sz; ++i)
			{
				size_t sz1 = stages[i].size();
				for (size_t j = 1; j < sz1; ++j)
				{
					Gizmo::Line(stages[i][j - 1], stages[i][j], CC_RED);
					Gizmo::Sphere(stages[i][j], 0.1f, CC_BLACK);
					Gizmo::Sphere(stages[i][j - 1], 0.1f, CC_BLACK);
				}
			}
		}
	}

	Vector3 Bezier_deCasteljau::GetPointOnCurve(float time_)
	{
		int whichSeg = (int)std::floor(time_) * dimension;
		time_ -= std::floor(time_);

		std::vector<zg::Vector3> tmpVectors;
		std::vector<std::vector<zg::Vector3>> stages;
		int numberofStage = 0;
		std::vector<std::vector<float>> tstages;
		tstages.push_back(std::vector<float>{});

		size_t sz = dimension + 1;
		for (size_t i = 0; i < sz; ++i)
		{
			tmpVectors.push_back(ListOfPoint[i + whichSeg]);
			tstages[numberofStage].push_back(0.0f);
		}
		stages.push_back(tmpVectors);

		while (sz > 1)
		{
			std::vector<zg::Vector3> currVectors;
			tstages.push_back(std::vector<float>{});
			for (size_t i = 1; i < sz; ++i)
			{
				currVectors.push_back(Lerp(tmpVectors[i - 1], tmpVectors[i - 0], time_, 0.0f, 1.0f));
				tstages[numberofStage + 1].push_back(GetLamda(time_, 0.0f, 1.0f));
			}
			tmpVectors = currVectors;
			stages.push_back(tmpVectors);
			++numberofStage;
			sz = tmpVectors.size();
		}
		return stages[stages.size() - 1][stages[stages.size() - 1].size() - 1];
	}

	void Bezier_deCasteljau::GenerateGizmoTable(int numberOfPoints_)
	{
		ListOfGizmoPoints.clear();

		size_t szOfPoint = ListOfPoint.size() - 1;
		for (size_t k = 0; k < szOfPoint; k += dimension)
		{
			float tmin = 0.0f;
			float tmax = 1.0f;
			float a = tmin;
			float b = (tmax - tmin) / numberOfPoints_;

			for (int j = 0; j < numberOfPoints_; ++j)
			{
				std::vector<zg::Vector3> tmpVectors{};
				std::vector<std::vector<zg::Vector3>> stages{};

				int numberofStage = 0;

				size_t sz = dimension + 1;
				for (size_t i = 0; i < sz; ++i)
				{
					tmpVectors.push_back(ListOfPoint[i + k]);
				}
				stages.push_back(tmpVectors);

				while (sz > 1)
				{
					std::vector<zg::Vector3> currVectors{};
					for (size_t i = 1; i < sz; ++i)
					{
						currVectors.push_back(Lerp(tmpVectors[i - 1], tmpVectors[i - 0], a, 0.0f, 1.0f));
					}
					tmpVectors = currVectors;
					stages.push_back(tmpVectors);
					++numberofStage;
					sz = tmpVectors.size();
				}
				ListOfGizmoPoints.push_back(stages[stages.size() - 1][stages[stages.size() - 1].size() - 1]);
				a += b;
			}
		}
	}
#pragma endregion

	//______________________________________________________________________ BezierCubic
	BezierCubic::BezierCubic() noexcept
		: CurvePath{}
	{}
	Vector3 BezierCubic::GetPointOnCurve(float time_)
	{
		return GetPointOnCurve(time_, nullptr);
	}
	Vector3 BezierCubic::GetPointOnCurve(float time_, Vector3* getvelocityvector_)
	{
		//get point on curve using table
		//int index = time_ * ListOfGizmoPoints.size() / (numberOfPointsOnCurve - 1);

		//return ListOfGizmoPoints[index];
		int whichSeg = (int)std::floor(time_) * 3;
		time_ -= MAX2(0.0f, std::floor(time_));
		
		Vector3 p0 = ListOfPoint[0 + whichSeg];
		Vector3 p1 = ListOfPoint[1 + whichSeg];
		Vector3 p2 = ListOfPoint[2 + whichSeg];
		Vector3 p3 = ListOfPoint[3 + whichSeg];
		Vector3 a = -p0 + 3.0f * p1 - 3.0f * p2 + p3;
		Vector3 b = 3.0 * p0 - 6.0f * p1 + 3.0f * p2;
		Vector3 c = -3.0 * p0 + 3.0f * p1;
		Vector3 d = p0;
		
		float u2 = time_ * time_;
		float u3 = time_ * time_ * time_;

		if (getvelocityvector_)
			*getvelocityvector_ = (3.0f * a * u2 + 2.0f * b * time_ + c);

		return a * u3 + b * u2 + c * time_ + d;
	}

	void BezierCubic::GenerateGizmoTable(int numberOfPoints_)
	{
		ListOfGizmoPoints.clear();
		size_t sz = ListOfPoint.size();

		float tmin = 0.0f;
		float tmax = 1.0f;
		float inc = (tmax - tmin) / numberOfPoints_;
		for (size_t k = 0; k < sz - 1; k += 3)
		{
			float time_ = tmin;
			for (int j = 0; j < numberOfPoints_; ++j)
			{
				Vector3 p0 = ListOfPoint[0 + k];
				Vector3 p1 = ListOfPoint[1 + k];
				Vector3 p2 = ListOfPoint[2 + k];
				Vector3 p3 = ListOfPoint[3 + k];

				Vector3 a = -p0 + 3.0f * p1 - 3.0f * p2 + p3;
				Vector3 b = 3.0 * p0 - 6.0f * p1 + 3.0f * p2;
				Vector3 c = -3.0 * p0 + 3.0f * p1;
				Vector3 d = p0;

				float u2 = time_ * time_;
				float u3 = time_ * time_ * time_;
				ListOfGizmoPoints.push_back(a * u3 + b * u2 + c * time_ + d);
				time_ += inc;
			}
		}
	}

	void BezierCubic::GenerateFixedArcLengthTable(int fixframeRate_)
	{
		ArcLengthTable.clear();
		int numOfSegment = numberOfPointsOnCurve - 1;
		float tmin = 0.0f;
		float tmax = 1.0f;
		float inc = (tmax - tmin) / fixframeRate_;

		//for each line segments
		for (int k = 0; k < numOfSegment; ++k)
		{
			float time_ = tmin;
			ArcLengthTable[k] = std::vector<float>{};
			ArcLengthTable[k].reserve(fixframeRate_);

			float fullarclengthOfSegment = 1.0f / Integrate(k, 0.0f, 1.0f);
			//Compute ArcLength at each time_ increment
			for (int j = 0; j < fixframeRate_; ++j)
			{
				float arcLength = Integrate(k, 0.0f, time_);

				//normalize the arc length
				ArcLengthTable[k].push_back(arcLength * fullarclengthOfSegment);
				time_ += inc;
			}
		}
	}

	void BezierCubic::GenerateArcLengthTable()
	{
		ArcLengthSegU.clear();
		ArcLengthSegvector.clear();
		int numOfSegment = numberOfPointsOnCurve - 1;
		const float epi = 0.0001f;
		
		float sA = 0.0f;
		for (int k = 0; k < numOfSegment; ++k)
		{
			float ua = 0.0f;
			float ub = 1.0f;
			Vector3 pA = GetPointOnCurve(ua + k);
			
			if (k == numOfSegment - 1)
				ub = 0.99f;
			Vector3 pB = GetPointOnCurve(ub + k);
			GenerateArcLengthTableRe(k, ua, ub, pA, pB, sA, epi);
			
			sA += Integrate(k, 0.0f, 1.0f);
		}
	}
	void BezierCubic::GenerateArcLengthTableRe(size_t segment_, float timeL_, float timeR_, Vector3& pA, Vector3& pB, float sA, float epi)
	{
		//loop
		float um = (timeL_ + timeR_) * 0.5f;
		Vector3 pM = GetPointOnCurve(um + segment_);

		float A = (pA - pM).Length();
		float B = (pM - pB).Length();
		float C = (pA - pB).Length();
		float d = A + B - C;

		if (d < epi && C < epi)
		{
			//ArcLengthSeg[um + segment_] = sA + A;
			ArcLengthSegU.push_back(um + segment_);
			ArcLengthSegvector.push_back(sA + A);
		}
		else
		{
			GenerateArcLengthTableRe(segment_, timeL_, um, pA, pM, sA, epi);
			GenerateArcLengthTableRe(segment_, um, timeR_, pM, pB, sA + A, epi);
		}
	}


	float BezierCubic::GetArcLength(size_t lineSegment_, float time_)
	{
		size_t k = lineSegment_ * 3;
		Vector3 p0 = ListOfPoint[0 + k];
		Vector3 p1 = ListOfPoint[1 + k];
		Vector3 p2 = ListOfPoint[2 + k];
		Vector3 p3 = ListOfPoint[3 + k];
		Vector3 a = -p0 + 3.0f * p1 - 3.0f * p2 + p3;
		Vector3 b = 3.0f * p0 - 6.0f * p1 + 3.0f * p2;
		Vector3 c = -3.0f * p0 + 3.0f * p1;
		Vector3 d = p0;
		float A = 9.0f * a.Dot(a);
		float B = 12.0f * a.Dot(b);
		float C = 6.0f * a.Dot(c) + 4.0f * b.Dot(b);
		float D = 4.0f * b.Dot(c);
		float E = c.Dot(c);
		float u2 = time_ * time_;
		float u3 = time_ * time_ * time_;
		float u4 = u2 * u2;
		float partarclength = A * u4 + B * u3 + C * u2 + D * time_ + E;
		return std::sqrtf(partarclength);
	}

	float BezierCubic::Integrate(size_t lineSegment_, float a_, float b_)
	{
		float plus = (b_ + a_) * 0.5f;
		float minu = (b_ - a_) * 0.5f;
		float totallength =
			0.555556f * GetArcLength(lineSegment_, minu * -0.774597f + plus) +
			//0.888889f * GetArcLength(lineSegment_, minu * 0.0f + plus) +
			0.888889f * GetArcLength(lineSegment_, plus) +
			0.555556f * GetArcLength(lineSegment_, minu * 0.774597f + plus);

		return minu * totallength;
	}

	float BezierCubic::GetTimeU(float arclength_normalize_)
	{
		float getRealTime = arclength_normalize_ * (numberOfPointsOnCurve - 1);
		int whichLineSegment = (int)getRealTime;
		getRealTime -= whichLineSegment;

		float s = Integrate(whichLineSegment, 0.0f, getRealTime);

		const float EPISSS = 0.001f;
		float ua = 0.0f;
		float ub = 1.0f;
		float um = 1.0f;
		float sm = Integrate(whichLineSegment, 0.0f, um);

		while (std::abs(s - sm) > EPISSS)
		{
			um = (ua + ub) * 0.5f; 
			sm = Integrate(whichLineSegment, 0.0f, um);
			(sm < s) ? ua = um : ub = um;
		} 
		return um + whichLineSegment;
	}

	float BezierCubic::GetArcLengthFromTable(size_t key_, float timeu_)
	{
		int index = static_cast<int>(timeu_ * 60);
		if (index == 60)
			return ArcLengthTable[key_][index - 1];
		return ArcLengthTable[key_][index];
	}
	float BezierCubic::GetTimeUFromFixedTable(float arclength_normalize_)
	{
		//determine which line segment and which index from table
		size_t szOfPointsEachSegment = ArcLengthTable[0].size();
		size_t szOfTable = szOfPointsEachSegment * (numberOfPointsOnCurve - 1);
		size_t index = static_cast<size_t>(arclength_normalize_ * szOfTable);
		size_t whichLineSegment = index / szOfPointsEachSegment;
		index %= szOfPointsEachSegment;
		
		float s = ArcLengthTable[whichLineSegment][index];
		
		const float EPISSS = 0.01f;
		float ua = 0.0f;
		float ub = 1.0f;
		float um = 1.0f;
		float sm = GetArcLengthFromTable(whichLineSegment, um);
		
		//Compute for time U
		while (std::abs(s - sm) > EPISSS)
		{
			um = (ua + ub) * 0.5f;
			sm = GetArcLengthFromTable(whichLineSegment, um);
			(sm < s) ? ua = um : ub = um;
		}
		return um + whichLineSegment;
	}

	float BezierCubic::GetTimeUFromArcLengthTable(float arclength_normalize_)
	{
		float currentArcLength = arclength_normalize_ * GetTotalArcLength();
		const size_t maxpt = ArcLengthSegvector.size() - 1;
		size_t current = 0;
		size_t left    = 0;
		size_t righ    = maxpt;
		while (left < righ)
		{
			current = left + ((righ - left) / 2);
			zg::Clamp(&current, size_t(0), maxpt);

			if (ArcLengthSegvector[current] == currentArcLength)
				break;
			if (ArcLengthSegvector[current] < currentArcLength)
				left = current + 1;
			else
			{
				if (current == 0)
					break;
				else
					righ = current - 1;
			}
		}
		return ArcLengthSegU[current];
	}

	float BezierCubic::GetTotalArcLength() 
	{
		float TotalArcLength = 0.0f;
		int numOfSegment = numberOfPointsOnCurve - 1;
		for (int i = 0; i < numOfSegment; ++i)
		{
			float arclengSegment = Integrate(i, 0.0f, 1.0f);
			//ArcLengthSeg.push_back(arclengSegment);
			TotalArcLength += arclengSegment;
		}
		return TotalArcLength;
	}

	//______________________________________________________________________ internal functions
	float GetLamda(float t_, float r_, float s_)
	{
		if (r_ == s_) return s_;
		return (t_ - r_) / (s_ - r_);
	}
	Vector3 Lerp(const Vector3& start_, const Vector3& end_, float t_, float r_, float s_)
	{
		float lamda = GetLamda(t_, r_, s_);
		return (1.0f - lamda) * start_ + lamda * end_;
	}
	std::vector<float> GetBinomialPower(int power_)
	{
		std::vector<float> binomial;
		binomial.push_back(1);

		for (int j = 1; j < power_; ++j)
		{
			std::vector<float> aatmp;
			for (size_t i = 1; i < binomial.size(); ++i)
			{
				aatmp.push_back(binomial[i - 1] + binomial[i]);
			}
			aatmp.insert(aatmp.begin(), 1.0f);
			aatmp.push_back(1);
			binomial = aatmp;
		}
		return binomial;
	}

	float	GetSineInterpolation(float time_, float t1_, float t2_)
	{
		float vc = 2.0f / (1 + t2_ - t1_);

		//s1
		if (time_ < t1_)
		{
			return vc / (2.0f * t1_) * time_ * time_;
		}
		//s3
		else if (time_ > t2_)
		{
			return (vc * (time_ - t2_)) / (2.0f * (1.0f - t2_)) * (2.0f - time_ - t2_) + vc * (t2_ - t1_ * 0.5f);
		}
		//s3
		return vc * (time_ - t1_) + vc * 0.5f * t1_;
		
		//return (std::sin(PI * time_ - PI2) + 1) * 0.5f;
	}


}//namespace zg
