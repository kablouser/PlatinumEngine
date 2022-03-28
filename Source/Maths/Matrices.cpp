//
// Created by Shihua on 08/03/2022.
//

#include <Maths/Matrices.h>
#include <glm/gtc/type_ptr.hpp>
#include <iterator>
namespace PlatinumEngine
{
	namespace Maths
	{
		Mat4 Mat4::operator*(float scale)
		{

			Mat4 result;

			for(int i=0;i<4;i++)
			{
				for(int j=0;j<4;j++)
				{

					result[i][j] = (*this)[i][j] * scale;

				}


			}

			return result;
		}

		Mat4 Mat4::operator*(Mat4 anotherMat4)
		{

			// Create glm matrices for calculate multiplication
			glm::mat4x4 leftMatrix = glm::make_mat4(this->matrix);
			glm::mat4x4 rightMatrix = glm::make_mat4(anotherMat4.matrix);
			glm::mat4x4 result = leftMatrix * rightMatrix;


			// create a new Mat4 object

			Mat4 resultMat4;

			// get the glm::mat4x4 array pointer

			float* resultPtr = glm::value_ptr(result);

			// convert the array into Mat4

			resultMat4.ConvertFromArray(resultPtr, 16);

			// return the final result with the type Mat4

			return resultMat4;
		}

		Vec4 Mat4::operator*(Vec4 homogeneousVector)
		{
			float vectorArray[] = { homogeneousVector.x, homogeneousVector.y, homogeneousVector.z, homogeneousVector.w };

			glm::mat4x4 leftMatrix = glm::make_mat4(this->matrix);
			glm::vec4 rightVector = glm::make_vec4(vectorArray);
			glm::vec4 result = leftMatrix * rightVector;

			Vec4 resultVec4;

			resultVec4.x = result.x;
			resultVec4.y = result.y;
			resultVec4.z = result.z;
			resultVec4.w = result.w;

			return resultVec4;

		}

		Mat4 Mat4::operator+(Mat4 otherMatrix)
		{

			Mat4 result;

			for (int y = 0; y < 4; y++)
			{
				for (int x = 0; x < 4; x++)
				{

					result[y][x] = (*this)[y][x] + otherMatrix[y][x];

				}
			}

			return result;

		}

		Mat4 Mat4::operator-(Mat4 otherMatrix)
		{

			Mat4 result;

			for (int y = 0; y < 4; y++)
			{
				for (int x = 0; x < 4; x++)
				{

					result[y][x] = (*this)[y][x] - otherMatrix[y][x];

				}
			}

			return result;

		}

		Mat4& Mat4::operator=(const Mat4& otherMatrix)
		{

			if(this == & otherMatrix)
				return *this;

			std::memcpy(this->matrix, otherMatrix.matrix, 16 * sizeof(float));

			return (*this);

		}

		void Mat4::SetIdentityMatrix()
		{

			float identityMatrix[] = { 1, 0, 0, 0,
									   0, 1, 0, 0,
									   0, 0, 1, 0,
									   0, 0, 0, 1 };

			this->ConvertFromArray(identityMatrix, 16);

		}

		void Mat4::SetTranslationMatrix(Vec3 translationDirection)
		{

			float translationMatrix[] = { 1, 0, 0, 0,
										  0, 1, 0, 0,
										  0, 0, 1, 0,
										  translationDirection.x, translationDirection.y, translationDirection.z, 1 };

			this->ConvertFromArray(translationMatrix, 16);
		}

		void Mat4::SetRotationMatrix(Vec3 eulerAngle)
		{

			// get rotation matrix (quaternion)
			glm::quat quaternionX(glm::vec3(eulerAngle.x, 0, 0));

			glm::quat quaternionY(glm::vec3(0, eulerAngle.y, 0));

			glm::quat quaternionZ(glm::vec3(0, 0, eulerAngle.z));

			glm::mat4x4 rotationMat4 =
					glm::mat4_cast(quaternionY) * glm::mat4_cast(quaternionX) * glm::mat4_cast(quaternionZ);


			// convert glm::mat4 into Mat4
			float* rotationMatrix = glm::value_ptr(rotationMat4);

			this->ConvertFromArray(rotationMatrix, 16);

		}

		void Mat4::SetScaleMatrix(PlatinumEngine::Maths::Vec3 scale)
		{

			float scaleMatrix[] = { scale.x, 0, 0, 0,
									0, scale.y, 0, 0,
									0, 0, scale.z, 0,
									0, 0, 0, 1 };

			this->ConvertFromArray(scaleMatrix, 16);
		}

		void Mat4::SetOrthogonalMatrix(float left, float right, float bottom, float top, float zNear, float zFar)
		{

			glm::mat4x4 OrthogonalGLM = glm::ortho(left, right, bottom, top, zNear, zFar);

			float* orthogonalMatrix = glm::value_ptr(OrthogonalGLM);

			this->ConvertFromArray(orthogonalMatrix, 16);

		}

		void Mat4::SetFrustumMatrix(float left, float right, float bottom, float top, float near, float far)
		{

			// convert glm::mat4 into Mat4
			glm::mat4x4 frustumGLM = glm::frustum(left, right, bottom, top, near, far);

			float* frustumMatrix = glm::value_ptr(frustumGLM);

			this->ConvertFromArray(frustumMatrix, 16);

		}

		void Mat4::SetPerspectiveMatrix(float fovy, float aspect, float near, float far)
		{
			// convert glm::mat4 into Mat4
			glm::mat4x4 frustumGLM = glm::perspective(fovy, aspect, near, far);

			float* frustumMatrix = glm::value_ptr(frustumGLM);

			this->ConvertFromArray(frustumMatrix, 16);

		}

		Mat3 Mat3::operator*(float scale)
		{

			Mat3 result;

			for(int i=0;i<3;i++)
			{
				for(int j=0;j<3;j++)
				{

					result[i][j] = (*this)[i][j] * scale;

				}

			}

			return result;

		}
    
		Mat3 Mat3::operator*(Mat3 anotherMat3)
		{

			// Create glm matrices for calculate multiplication
			glm::mat3x3 leftMatrix = glm::make_mat3(this->matrix);
			glm::mat3x3 rightMatrix = glm::make_mat3(anotherMat3.matrix);
			glm::mat3x3 result = leftMatrix * rightMatrix;


			// create a new Mat4 object

			Mat3 resultMat3;

			// get the glm::mat4x4 array pointer

			float* resultPtr = glm::value_ptr(result);

			// convert the array into Mat4

			resultMat3.ConvertFromArray(resultPtr, 9);

			// return the final result with the type Mat4

			return resultMat3;

		}

		Vec3 Mat3::operator*(Vec3 vector)
		{
			float vectorArray[] = { vector.x, vector.y, vector.z};

			glm::mat3x3 leftMatrix = glm::make_mat3(this->matrix);
			glm::vec3 rightVector = glm::make_vec3(vectorArray);
			glm::vec3 result = leftMatrix * rightVector;

			Vec3 resultVec3;

			resultVec3.x = result.x;
			resultVec3.y = result.y;
			resultVec3.z = result.z;

			return resultVec3;


		}

		Mat3 Mat3::operator+(Mat3 otherMatrix)
		{

			Mat3 result;

			for (int y = 0; y < 3; y++)
			{
				for (int x = 0; x < 3; x++)
				{

					result[y][x] = (*this)[y][x] + otherMatrix[y][x];

				}
			}

			return result;

		}

		Mat3 Mat3::operator-(Mat3 otherMatrix)
		{
			Mat3 result;

			for (int y = 0; y < 3; y++)
			{
				for (int x = 0; x < 3; x++)
				{

					result[y][x] = (*this)[y][x] - otherMatrix[y][x];

				}
			}

			return result;

		}

		Mat3& Mat3::operator=(const Mat3& otherMatrix)
		{

			if(this == & otherMatrix)
				return *this;

			std::memcpy(this->matrix, otherMatrix.matrix, 9 * sizeof(float));

			return (*this);

		}

		void Mat3::SetIdentityMatrix()
		{
			float identityMatrix[] = { 1, 0, 0,
									   0, 1, 0,
									   0, 0, 1};

			this->ConvertFromArray(identityMatrix, 9);

		}

		void Mat3::SetRotationMatrix(Vec3 eulerAngle)
		{
			// get rotation matrix (quaternion)
			glm::quat quaternionX(glm::vec3(eulerAngle.x, 0, 0));

			glm::quat quaternionY(glm::vec3(0, eulerAngle.y, 0));

			glm::quat quaternionZ(glm::vec3(0, 0, eulerAngle.z));

			glm::mat3x3 rotationMat3 =
					glm::mat3_cast(quaternionY) * glm::mat3_cast(quaternionX) * glm::mat3_cast(quaternionZ);


			// convert glm::mat4 into Mat4
			float* rotationMatrix = glm::value_ptr(rotationMat3);

			this->ConvertFromArray(rotationMatrix, 9);
		}

		void Mat3::SetScaleMatrix(PlatinumEngine::Maths::Vec3 scale)
		{
			float scaleMatrix[] = { scale.x, 0, 0,
									0, scale.y, 0,
									0, 0, scale.z };

			this->ConvertFromArray(scaleMatrix, 9);
      
		}

	}
}