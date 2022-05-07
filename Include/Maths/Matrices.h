//
// Created by Shihua on 08/03/2022.
//
/// Warning:
/// 1. Matrix class are using column-major array like glm.
/// 2. Use Matrix[y/row][x/column] to access every elements.


#pragma once

#include <cassert>
#include <glm/glm.hpp>
#include <cstring>
#include <array>
#include "Vectors.h"

//------------------------
//  Template Class
//------------------------
namespace PlatinumEngine
{
	namespace Maths
	{
		// forward declare the Quaternion class
		class Quaternion;

		//-----------------------------------------------
		//  Matrix template class and its helper class
		//-----------------------------------------------

		// ___class MatrixHelper___
		// Declare Matrix helper class (help with rewriting operator [] to make "matrix[y][x]" works)

		template<unsigned int numberOfRow, unsigned int numberOfColumn, typename T>
		class MatrixHelper;


		// ___class Matrix___
		// A template class for matrix classes. Column/x major
		template<unsigned int numberOfRow, unsigned int numberOfColumn, typename T>
		class Matrix
		{

		public:

			//___FUNCTION___

			/***
			 * Overloading operator[] function. For accessing very element in the matrix.
			 * @param column : This is the id of column (x)
			 * @return    : Return object of MatrixHelper class, which contains another overloaded operator [] function
			 */
			MatrixHelper<numberOfRow, numberOfColumn, T> operator[](const int column);


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
			 * Overloading operator- function. For scaling the matrix.
			 * @param otherMatrix
			 */
			Matrix<numberOfRow, numberOfColumn, T>&
			operator=(const Matrix<numberOfRow, numberOfColumn, T>& otherMatrix);

			/**
			 * Convert array into matrix
			 * @param arrayMatrix : array
			 * @param arraySize : size of array
			 */
			void ConvertFromArray(const T* array, unsigned int arraySize);

			//___CONSTRUCTOR___
			Matrix(); // create empty matrix
			explicit Matrix(T valueForPacking); // fill the matrix (the std::vector) with valueForPacking
			explicit Matrix(const std::array<T, numberOfRow* numberOfColumn>& matrixArray); // fill the matrix with array
			explicit Matrix(const float matrixArray[numberOfRow * numberOfColumn]);

			//___VARIABLE___
			T matrix[numberOfRow * numberOfColumn];

		};


		// ___class MatrixHelper___
		// Implementation of MatrixHelper class
		template<unsigned int numberOfRow, unsigned int numberOfColumn, typename T>
		class MatrixHelper
		{

		public:
			/***
			 * Return the reference of a specific value Matrix[x/column][y/row] in matrix array.
			 * @param row y
			 * @return
			 */
			T& operator[](unsigned int row);


		private:

			//___DECLARE FRIEND CLASS___
			friend class Matrix<numberOfRow, numberOfColumn, T>;


			//___PARAMETERS___
			Matrix<numberOfRow, numberOfColumn, T>& _matrix;
			unsigned int _currentColumn;

			//___CONSTRUCTOR___
			MatrixHelper(Matrix<numberOfRow, numberOfColumn, T>& matrix, unsigned int column);
		};

	}
}


namespace PlatinumEngine
{
	namespace Maths
	{
		// Column major. Mat4[x][y]
		class Mat4 : public Matrix<4, 4, float>
		{
		public:

			//___FUNCTION___
			Mat4 operator*(float scale);

			Mat4 operator*(Mat4 anotherMat4);

			Vec4 operator*(Vec4 homogeneousVector);

			Vec3 MultiplyVec3(Vec3 homogeneousVector, float w);

			Mat4 operator+(Mat4 otherMatrix);

			Mat4 operator-(Mat4 otherMatrix);

			Mat4& operator=(const Mat4& otherMatrix);

			void Inverse();

			void Transpose();

			void SetIdentityMatrix();

			void SetTranslationMatrix(Vec3 translationDirection);

			void SetRotationMatrix(Vec3 eulerAngle);

			void SetRotationMatrix(Quaternion quaternion);

			void SetScaleMatrix(Vec3 scale);

			void SetOrthogonalMatrix(float left, float right, float bottom, float top, float zNear, float zFar);

			void SetFrustumMatrix(float left, float right, float bottom, float top, float near, float far);

			// fovy in degrees
			void SetPerspectiveMatrix(float fovy, float aspect, float near, float far);

			void SetTRSMatrix(Vec3 translate, Quaternion rotate, float scale);

			//___CONSTRUCTOR___
			using Matrix<4, 4, float>::Matrix;

			//___CASTING___
			operator glm::mat4() const;

			void ConvertFromGLM(glm::mat4 mat4);

			void Decompose(
					Maths::Vec3* outTranslation,
					Maths::Quaternion* outQuaternion,
					float* scale) const;

		};

		// Column major. Mat3[x][y]
		class Mat3 : public Matrix<3, 3, float>
		{
		public:

			//___FUNCTION___
			Mat3 operator*(float scale);

			Mat3 operator*(Mat3 anotherMat4);

			Vec3 operator*(Vec3 homogeneousVector);

			Mat3 operator+(Mat3 otherMatrix);

			Mat3 operator-(Mat3 otherMatrix);

			Mat3& operator=(const Mat3& otherMatrix);

			void Inverse();

			void Transpose();

			void SetIdentityMatrix();

			void SetRotationMatrix(Vec3 eulerAngle);

			void SetRotationMatrix(Quaternion eulerAngle);

			void SetScaleMatrix(PlatinumEngine::Maths::Vec3 scale);

			//___CONSTRUCTOR___
			using Matrix<3, 3, float>::Matrix;

			//___CASTING___
			operator glm::mat3() const;

			void ConvertFromGLM(glm::mat3 mat3);

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

			if (numberOfRow < numberOfColumn)
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

		template<unsigned int numberOfRow, unsigned int numberOfColumn, typename T>
		Matrix<numberOfRow, numberOfColumn, T>::Matrix(const std::array<T, numberOfRow* numberOfColumn>& matrixArray)
		{
			std::memcpy(this->matrix, matrixArray.data(), numberOfRow * numberOfColumn *sizeof(T));
		}

		template<unsigned int numberOfRow, unsigned int numberOfColumn, typename T>
		Matrix<numberOfRow, numberOfColumn, T>::Matrix(const float matrixArray[numberOfRow * numberOfColumn])
		{
			std::memcpy(this->matrix, matrixArray, numberOfRow * numberOfColumn *sizeof(T));
		}


		// ___OVERLOADING FUNCTIONS___
		template<unsigned int numberOfRow, unsigned int numberOfColumn, typename T>
		MatrixHelper<numberOfRow, numberOfColumn, T> Matrix<numberOfRow, numberOfColumn, T>
		::operator[](const int column)
		{
			return MatrixHelper<numberOfRow, numberOfColumn, T>(*this, column);
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

					result[x][y] = (*this)[x][y] + otherMatrix[x][y];

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

					result[x][y] = (*this)[x][y] - otherMatrix[x][y];

				}
			}

			return result;

		}


		template<unsigned int numberOfRow, unsigned int numberOfColumn, typename T>
		Matrix<numberOfRow, numberOfColumn, T>& Matrix<numberOfRow, numberOfColumn, T>
		::operator=(const Matrix<numberOfRow, numberOfColumn, T>& otherMatrix)
		{
			if (this == &otherMatrix)
				return *this;

			std::memcpy(this->matrix, otherMatrix.matrix, numberOfRow * numberOfColumn * sizeof(T));

			return (*this);
		}


		template<unsigned int numberOfRow, unsigned int numberOfColumn, typename T>
		void Matrix<numberOfRow, numberOfColumn, T>
		::ConvertFromArray(const T* array, unsigned int arraySize)
		{
			if (arraySize == numberOfRow * numberOfColumn)
				memcpy(this->matrix, array, numberOfRow * numberOfColumn * sizeof(T));
		}

		//-------------------------------------------
		// template function for matrix helper class
		//-------------------------------------------

		// ___CONSTRUCTOR___
		template<unsigned int numberOfRow, unsigned int numberOfColumn, typename T>
		MatrixHelper<numberOfRow, numberOfColumn, T>
		::MatrixHelper(Matrix<numberOfRow, numberOfColumn, T>& matrix, unsigned int currentColumn)
				:_matrix(matrix), _currentColumn(currentColumn)
		{
		}


		// ___OVERLOADING FUNCTION___
		template<unsigned int numberOfRow, unsigned int numberOfColumn, typename T>
		T& MatrixHelper<numberOfRow, numberOfColumn, T>
		::operator[](unsigned int row)
		{

			assert(numberOfRow > row || numberOfColumn > _currentColumn);

			return _matrix.matrix[_currentColumn * numberOfRow + row];

		}


		//-------------------------------------------
		// overloading << operator
		//-------------------------------------------
		template<unsigned int numberOfRow, unsigned int numberOfColumn, typename T>
		inline std::ostream& operator<<(std::ostream& out, Matrix<numberOfRow, numberOfColumn, T>& m)
		{
			for (int j = 0; j < numberOfColumn; j++)
			{
				for (int i = 0; i < numberOfRow; i++)
				{
					out << m[i][j] << ", ";
				}
				out << std::endl;
			}

			return out;
		}
	}
}

//--------------------------------------
// Functions to process matrix3 and 4
// External syntax sugar
//--------------------------------------
namespace PlatinumEngine
{
	namespace Maths
	{
		Mat3 Inverse(Mat3 mat3);

		Mat3 Transpose(Mat3 mat3);

		Mat4 Inverse(Mat4 mat4);

		Mat4 Transpose(Mat4 mat4);
	}
}