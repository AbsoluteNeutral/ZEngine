#include "stdafx.h"
#include "MatrixMN.h"

namespace zg {

	MatrixMN::MatrixMN() noexcept
		: matrix{}
		, numOfRow(0)
		, numOfCol(0)
	{}
	MatrixMN::MatrixMN(size_t row_, size_t col_) noexcept
		: matrix{}
		, numOfRow(row_)
		, numOfCol(col_)

	{
		//matrix = new float*;
		//matrix = new float*[row_];
		//float** getrow = matrix;
		//while (row_--) 
		//{
		//	unsigned tmpcol = col_;
		//	while (tmpcol--) 
		//	{
		//		*getrow = new float[col_];
		//	}
		//}

		for (unsigned i = 0; i < numOfRow; ++i)
		{
			matrix.push_back(std::vector<float>{});
			for (unsigned j = 0; j < numOfCol; ++j)
			{
				matrix[i].push_back(0.0f);
			}
		}
	}
	MatrixMN::MatrixMN(const Vector2& vector_, bool row_) noexcept 
		: matrix{}
		, numOfRow(1)
		, numOfCol(2)
	{
		if (row_)
		{
			matrix.push_back(std::vector<float>{});
			matrix[0].push_back(vector_.x);
			matrix[0].push_back(vector_.y);
		}
		else
		{
			matrix.push_back(std::vector<float>{});
			matrix.push_back(std::vector<float>{});
			matrix[0].push_back(vector_.x);
			matrix[1].push_back(vector_.y);
			numOfRow = 2;
			numOfCol = 1;
		}
	}
	MatrixMN::MatrixMN(const Vector3& vector_, bool row_) noexcept 
		: matrix{}
		, numOfRow(1)
		, numOfCol(3)
	{
		if (row_)
		{
			matrix.push_back(std::vector<float>{});
			matrix[0].push_back(vector_.x);
			matrix[0].push_back(vector_.y);
			matrix[0].push_back(vector_.z);
		}
		else
		{
			matrix.push_back(std::vector<float>{});
			matrix.push_back(std::vector<float>{});
			matrix.push_back(std::vector<float>{});
			matrix[0].push_back(vector_.x);
			matrix[1].push_back(vector_.y);
			matrix[2].push_back(vector_.z);
			numOfRow = 3;
			numOfCol = 1;
		}
	}
	MatrixMN::MatrixMN(const std::vector<float>& arrayOfFloat_, bool row_) noexcept
		: matrix{}
		, numOfRow(1)
		, numOfCol(arrayOfFloat_.size())
	{
		if (row_)
		{
			matrix.push_back(std::vector<float>{});
			for (const auto& elem : arrayOfFloat_)
				matrix[0].push_back(elem);

		}
		else
		{
			numOfRow = arrayOfFloat_.size();
			numOfCol = 1;
			for (size_t i = 0; i < numOfRow; ++i)
			{
				matrix.push_back(std::vector<float>{});
				matrix[i].push_back(arrayOfFloat_[i]);
			}
		}
	}
	MatrixMN::MatrixMN(const std::vector<std::vector<float>>& arrayOfFloat_) noexcept
		: matrix{arrayOfFloat_}
		, numOfRow(arrayOfFloat_.size())
		, numOfCol(arrayOfFloat_[0].size())
	{
	}

	//setters
	void MatrixMN::SetIdentity() 
	{
		if (IsSquare())
		{
			for (size_t i = 0; i < numOfCol; ++i)
			{
				for (size_t j = 0; j < numOfRow; ++j)
				{
					if (i == j)
						matrix[i][j] = 1.0f;
					else
						matrix[i][j] = 0.0f;
				}
			}
		}
	}
	void MatrixMN::SetZero()
	{
		if (IsSquare())
		{
			for (size_t i = 0; i < numOfCol; ++i)
			{
				for (size_t j = 0; j < numOfRow; ++j)
				{
					matrix[i][j] = 0.0f;
				}
			}
		}
	}

	void MatrixMN::Transpose() 
	{
		std::vector<std::vector<float>> copy{ matrix };

		matrix.clear();
		for (size_t i = 0; i < numOfCol; ++i)
		{
			matrix.push_back(std::vector<float>{});
			for (size_t j = 0; j < numOfRow; ++j)
			{
				matrix[i].push_back(copy[j][i]);
			}
		}

		//swap values
		numOfRow = numOfRow ^ numOfCol;
		numOfCol = numOfRow ^ numOfCol;
		numOfRow = numOfRow ^ numOfCol;
	}

	bool MatrixMN::IsSquare() const {
		return numOfCol == numOfRow;
	}
	std::vector<float>	MatrixMN::GetCol(size_t index_) const
	{
		std::vector<float> tmp;
		for (unsigned j = 0; j < numOfCol; ++j)
		{
			tmp.push_back(matrix[index_][j]);
		}
		return tmp;
	}
	std::vector<float>	MatrixMN::GetRow(size_t index_) const
	{
		return matrix[index_];
	}
	MatrixMN MatrixMN::Transposed() const 
	{
		MatrixMN tmp{ *this };
		tmp.Transpose();
		return tmp;
	}

	MatrixMN operator*(const MatrixMN& lhs, const MatrixMN& rhs) 
	{
		if (lhs.numOfCol == rhs.numOfRow)
		{
			std::vector<std::vector<float>> tmp;

			for (unsigned m = 0; m < lhs.numOfRow; ++m)
			{
				tmp.push_back(std::vector<float>{});
				for (unsigned n = 0; n < rhs.numOfCol; ++n)
				{
					float gettotal = 0.0f;
					for (unsigned i = 0; i < lhs.numOfCol; ++i)
					{
							gettotal += lhs.matrix[m][i] * rhs.matrix[i][n];
					}

					tmp[m].push_back(gettotal);
				}
			}

			return MatrixMN{ tmp };
		}
		else
			throw "Explode";
	}

	std::ostream& operator<<(std::ostream& os, const MatrixMN& m) 
	{
		for (size_t i = 0; i < m.numOfRow; ++i)
		{
			for (size_t j = 0; j < m.numOfCol; ++j)
			{
				os << m.matrix[i][j] << " ";
			}
			os << "\n";
		}
		os << std::endl;
		return os;
	}

	MatrixMN GaussJordanInverse(const MatrixMN& matrix_)
	{
		if (matrix_.IsSquare())
		{
			size_t dim = matrix_.numOfRow;

			MatrixMN idn{ dim , dim };
			idn.SetIdentity();

			std::vector<std::vector<float>> row;
			for (size_t i = 0; i < matrix_.numOfRow; ++i)
			{
				row.push_back(matrix_.GetRow(i));
			}

			for (size_t i = 0; i < dim; ++i)
			{
				float divider = 1.0f / row[i][i];

				for (size_t j = 0; j < dim; ++j)
				{
					row[i][j]		 *= divider;
					idn.matrix[i][j] *= divider;
				}
				
				for (size_t j = 0; j < dim; ++j) 
				{
					if (i == j) continue;

					if (row[j][i]) 
					{
						std::vector<float> tmp = row[i];
						std::vector<float> tmd = idn.matrix[i];

						for (size_t k = 0; k < dim; ++k)
						{
							tmp[k] *= row[j][i];
							tmd[k] *= row[j][i];
						}

						for (size_t k = 0; k < dim; ++k)
						{
							row[j][k]		 -= tmp[k];
							idn.matrix[j][k] -= tmd[k];
						}
					}
				}
			}
			return idn;
		}
		else
			return MatrixMN{};
	}

}//namespace zg
