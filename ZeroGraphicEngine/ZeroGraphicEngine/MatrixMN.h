
#ifndef _MATRIXMN_H
#define _MATRIXMN_H

#include "Matrix.h"
#include <vector>

namespace zg{
	class ZG_API MatrixMN
	{
	public:
		//float** matrix;
		std::vector<std::vector<float>> matrix;
		size_t numOfRow;
		size_t numOfCol;

		MatrixMN()														noexcept;
		MatrixMN(size_t row_, size_t col_)								noexcept;
		MatrixMN(const Vector2& vector_, bool row_)						noexcept;
		MatrixMN(const Vector3& vector_, bool row_)						noexcept;
		MatrixMN(const std::vector<float>& arrayOfFloat_, bool row_)	noexcept;
		MatrixMN(const std::vector<std::vector<float>>& arrayOfFloat_)	noexcept;
	
		//static
		static const MatrixMN	Zero;

		void		SetIdentity		();
		void		SetZero			();
		void		Transpose		();
	
		//getters
		bool					IsSquare		() const;
		std::vector<float>		GetCol			(size_t index_) const;
		std::vector<float>		GetRow			(size_t index_) const;
		MatrixMN				Transposed		() const;

		ZG_API friend MatrixMN operator*(const MatrixMN& matrix_, const MatrixMN& rhs);
		ZG_API friend std::ostream& operator<<(std::ostream& os, const MatrixMN& m);
	};

	ZG_API MatrixMN	GaussJordanInverse	(const MatrixMN& mat);

} //namespace zg
#endif //_MATRIX33_H
