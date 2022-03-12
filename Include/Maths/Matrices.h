//
// Created by Shihua on 08/03/2022.
//
/// Warning:
/// 1. Matrix class are using column-major array like glm.
/// 2. Use Matrix[y/row][x/column] to access every elements.


#pragma once

#include <assert.h>
#include <glm/glm.hpp>
#include <string.h>
#include <Maths/Vectors.h>

//------------------------
//  Template Class
//------------------------
namespace PlatinumEngine
{
	namespace Maths
	{

		//-----------------------------------------------
		//  Matrix template class and its helper class
		//-----------------------------------------------

		// ___class MatrixHelper___
		// Declare Matrix helper class (help with rewriting operator [] to make "matrix[y][x]" works)

		template<unsigned int numberOfRow, unsigned int numberOfColumn, typename T>
		class MatrixHelper;


		// ___class Matrix___
		// A template class for matrix classes
		template<unsigned int numberOfRow, unsigned int numberOfColumn, typename T>
		class Matrix
		{

		public:

			//___FUNCTION___

			/***
			 * Overloading operator[] function. For accessing very element in the matrix.
			 * @param row : This is the id of row (y)
			 * @return    : Return object of MatrixHelper class, which contains another overloaded operator [] function
			 */
			MatrixHelper<numberOfRow, numberOfColumn, T> operator[](const int row);


			/***
			 * Overloading operator+ function. For addition between two matrices.
			 * @param otherMatrix : another matrix object for doing the addition
			 * @return A copy of matrix object  (the final result).
			 */
			Matrix<numberOfRow, numberOfColumn, T> operator+(Matrix<numberOfRow, numberOfColumn, T> otherMatrix);


			/***
			 * Overloading operator- function. For subtraction between two matrices.
			 * @param otherMatrix : another matrix object for doing the subtraction
			 * @return A copy of matrix object  (the final result).
			 */
			Matrix<numberOfRow, numberOfColumn, T> operator-(Matrix<numberOfRow, numberOfColumn, T> otherMatrix);


			/***
			 * Overloading operator* function. For scaling the matrix.
			 * @param scale
			 * @return A copy of matrix object (the final result).
			 */
			Matrix<numberOfRow, numberOfColumn, T> operator*(float scale);


			/***
			 * Overloading operator- function. For scaling the matrix.
			 * @param otherMatrix
			 */
			void operator=(Matrix<numberOfRow, numberOfColumn, T> otherMatrix);


			//___CONSTRUCTOR___
			Matrix(); // create empty matrix
			Matrix(T valueForPacking); // fill the matrix (the std::vector) with valueForPacking



			//___VARIABLE___
			T matrix[numberOfRow * numberOfColumn];

		private:


		};


		// ___class MatrixHelper___
		// Implementation of MatrixHelper class
		template<unsigned int numberOfRow, unsigned int numberOfColumn, typename T>
		class MatrixHelper
		{

		public:
			/***
			 * Return the reference of a specific value Matrix[y/row][x/column] in matrix array.
			 * @param column
			 * @return
			 */
			T& operator[](unsigned int column);


		private:

			//___DECLAIRE FRIEND CLASS___
			friend class Matrix<numberOfRow, numberOfColumn, T>;


			//___PARAMETERS___
			Matrix<numberOfRow, numberOfColumn, T>& _matrix;
			unsigned int _currentRow;

			//___CONSTRUCTOR___
			MatrixHelper(Matrix<numberOfRow, numberOfColumn, T>& matrix, unsigned int currentRow);
		};

	}
}




//------------------------
//  Matrix Class
//------------------------

namespace PlatinumEngine
{
	namespace Maths
	{

		class Mat4 : public Matrix<4, 4, float>
		{
		public:

			//___FUNCTION___
			Mat4 operator*(Mat4 anotherMat4);

			Vec4 operator*(Vec4 homogeneousVector);

			void ConvertFromArray(float* arrayMat4);

			void SetIdentityMatrix();

			void SetTranslationMatrix(Vec3 translationDirection);

			void SetRotationMatrix(Vec3 eulerAngle);

			void SetScaleMatrix(float scale);

			void SetOrthogonalMatrix(float left, float right, float bottom, float top, float zNear, float zFar);

			void SetFrustumMatrix(float left, float right, float bottom, float top, float near, float far);

			void SetPerspectiveMatrix(float fovy, float aspect, float near, float far);


			//___CONSTRUCTOR___
			using Matrix<4, 4, float>::Matrix;


			//___VARIABLE___


		};


		class Mat3 : public Matrix<3, 3, float>
		{
		public:

			//___FUNCTION___
			Mat3 operator*(Mat3 anotherMat4);

			Vec3 operator*(Vec3 homogeneousVector);

			void ConvertFromArray(float* arrayMat3);

			void SetIdentityMatrix();

			void SetRotationMatrix(Vec3 eulerAngle);

			void SetScaleMatrix(float scale);

			//___CONSTRUCTOR___
			using Matrix<3, 3, float>::Matrix;


			//___VARIABLE___


		};


	}
}



//--------------------------------------
// Implementation of template classes
//--------------------------------------

namespace PlatinumEngine
{
	namespace Maths
	{


		//--------------------------------------
		// template function for matrix class
		//--------------------------------------

		// ___CONSTRUCTORS___
		template<unsigned int numberOfRow, unsigned int numberOfColumn, typename T>
		Matrix<numberOfRow, numberOfColumn, T>::Matrix()
		{

			memset(this->matrix, 0, sizeof(this->matrix));

		}

		template<unsigned int numberOfRow, unsigned int numberOfColumn, typename T>
		Matrix<numberOfRow, numberOfColumn, T>::Matrix(T valueForPacking)
		{
			// initiate the matrix with 0
			memset(this->matrix, 0, sizeof(this->matrix));

			// get the length of diagonal
			int diagonalLength = 0;

			if(numberOfRow < numberOfColumn)
			{
				diagonalLength = numberOfRow;
			}
			else
			{
				diagonalLength = numberOfColumn;
			}

			// fill the diagonal with the valueForPacking
			for (int i = 0; i < diagonalLength; i++)
				(*this)[i][i] = valueForPacking;
		}


		// ___OVERLOADING FUNCTIONS___
		template<unsigned int numberOfRow, unsigned int numberOfColumn, typename T>
		MatrixHelper<numberOfRow, numberOfColumn, T> Matrix<numberOfRow, numberOfColumn, T>
		::operator[](const int row)
		{
			return MatrixHelper<numberOfRow, numberOfColumn, T>(*this, row);
		}


		template<unsigned int numberOfRow, unsigned int numberOfColumn, typename T>
		Matrix<numberOfRow, numberOfColumn, T> Matrix<numberOfRow, numberOfColumn, T>
		::operator+(Matrix<numberOfRow, numberOfColumn, T> otherMatrix)
		{

			Matrix<numberOfRow, numberOfColumn, T> result;

			for (int y = 0; y < numberOfRow; y++)
			{
				for (int x = 0; x < numberOfColumn; x++)
				{

					result[y][x] = (*this)[y][x] + otherMatrix[y][x];

				}
			}

			return result;

		}


		template<unsigned int numberOfRow, unsigned int numberOfColumn, typename T>
		Matrix<numberOfRow, numberOfColumn, T> Matrix<numberOfRow, numberOfColumn, T>
		::operator-(Matrix<numberOfRow, numberOfColumn, T> otherMatrix)
		{

			Matrix<numberOfRow, numberOfColumn, T> result;

			for (int y = 0; y < numberOfRow; y++)
			{
				for (int x = 0; x < numberOfColumn; x++)
				{

					result[y][x] = (*this)[y][x] - otherMatrix[y][x];

				}
			}

			return result;

		}


		template<unsigned int numberOfRow, unsigned int numberOfColumn, typename T>
		Matrix<numberOfRow, numberOfColumn, T> Matrix<numberOfRow, numberOfColumn, T>
		::operator*(float scale)
		{

			Matrix<numberOfRow, numberOfColumn, T> result;

			for (int y = 0; y < numberOfRow; y++)
			{
				for (int x = 0; x < numberOfColumn; x++)
				{

					result[y][x] = (*this)[y][x] * scale;

				}
			}

			return result;

		}


		template<unsigned int numberOfRow, unsigned int numberOfColumn, typename T>
		void Matrix<numberOfRow, numberOfColumn, T>
		::operator=(Matrix<numberOfRow, numberOfColumn, T> otherMatrix)
		{

			for (int y = 0; y < numberOfRow; y++)
			{
				for (int x = 0; x < numberOfColumn; x++)
				{

					(*this)[y][x] = otherMatrix[y][x];

				}
			}


		}

		//-------------------------------------------
		// template function for matrix helper class
		//-------------------------------------------

		// ___CONSTRUCTOR___
		template<unsigned int numberOfRow, unsigned int numberOfColumn, typename T>
		MatrixHelper<numberOfRow, numberOfColumn, T>
		::MatrixHelper(Matrix<numberOfRow, numberOfColumn, T>& matrix, unsigned int currentRow)
				:_matrix(matrix), _currentRow(currentRow)
		{
		}


		// ___OVERLOADING FUNCTION___
		template<unsigned int numberOfRow, unsigned int numberOfColumn, typename T>
		T& MatrixHelper<numberOfRow, numberOfColumn, T>
		::operator[](unsigned int column)
		{

			assert(numberOfRow > _currentRow || numberOfColumn > column);

			return _matrix.matrix[column * numberOfRow + _currentRow];

		}



		//-------------------------------------------
		// overloading << operator
		//-------------------------------------------

		inline std::ostream& operator<<(std::ostream &out, Mat4 &m)
		{
			return out <<m[0][0]<<" "<<m[0][1]<<" "<<m[0][2]<<" "<<m[0][3]<<"\n"<<
					   m[1][0]<<" "<<m[1][1]<<" "<<m[1][2]<<" "<<m[1][3]<<"\n"<<
					   m[2][0]<<" "<<m[2][1]<<" "<<m[2][2]<<" "<<m[2][3]<<"\n"<<
					   m[3][0]<<" "<<m[3][1]<<" "<<m[3][2]<<" "<<m[3][3];
		}

		inline std::ostream& operator<<(std::ostream &out, Mat3 &m)
		{
			return out <<m[0][0]<<" "<<m[0][1]<<" "<<m[0][2]<<"\n"<<
					   m[1][0]<<" "<<m[1][1]<<" "<<m[1][2]<<"\n"<<
					   m[2][0]<<" "<<m[2][1]<<" "<<m[2][2]<<"\n";
		}

	}
}


