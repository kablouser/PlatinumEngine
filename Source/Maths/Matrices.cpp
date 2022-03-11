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

		void Mat4::ConvertFromArray(float* arrayMatrix)
		{

			memcpy(this->matrix, arrayMatrix, 16 * sizeof(float));

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

			resultMat4.ConvertFromArray(resultPtr);

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

		void Mat4::SetIdentityMatrix()
		{

			float identityMatrix[] = { 1, 0, 0, 0,
									   0, 1, 0, 0,
									   0, 0, 1, 0,
									   0, 0, 0, 1 };

			this->ConvertFromArray(identityMatrix);

		}

		void Mat4::SetTranslationMatrix(Vec3 translationDirection)
		{

			float translationMatrix[] = { 1, 0, 0, 0,
										  0, 1, 0, 0,
										  0, 0, 1, 0,
										  translationDirection.x, translationDirection.y, translationDirection.z, 1 };

			this->ConvertFromArray(translationMatrix);
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

			this->ConvertFromArray(rotationMatrix);

		}

		void Mat4::SetScaleMatrix(float scale)
		{

			float scaleMatrix[] = { scale, 0, 0, 0,
									0, scale, 0, 0,
									0, 0, scale, 0,
									0, 0, 0, 1 };

			this->ConvertFromArray(scaleMatrix);
		}


		void Mat4::SetOrthogonalMatrix(float left, float right, float bottom, float top, float zNear, float zFar)
		{

			glm::mat4x4 OrthogonalGLM = glm::ortho(left, right, bottom, top, zNear, zFar);

			float* orthogonalMatrix = glm::value_ptr(OrthogonalGLM);

			this->ConvertFromArray(orthogonalMatrix);


		}


		void Mat4::SetFrustumMatrix(float left, float right, float bottom, float top, float near, float far)
		{

			// convert glm::mat4 into Mat4
			glm::mat4x4 frustumGLM = glm::frustum(left, right, bottom, top, near, far);

			float* frustumMatrix = glm::value_ptr(frustumGLM);

			this->ConvertFromArray(frustumMatrix);

		}

		void Mat4::SetPerspectiveMatrix(float fovy, float aspect, float near, float far)
		{
			// convert glm::mat4 into Mat4
			glm::mat4x4 frustumGLM = glm::perspective(fovy, aspect, near, far);

			float* frustumMatrix = glm::value_ptr(frustumGLM);

			this->ConvertFromArray(frustumMatrix);

		}


		void Mat4::PrintMatrix()
		{
			printf("Matrix 4x4: \n %.1f, %.1f, %.1f, %.1f \n %.1f, %.1f, %.1f, %.1f \n %.1f, %.1f, %.1f, %.1f \n %.1f, %.1f, %.1f, %.1f \n\n",
					(*this)[0][0],(*this)[0][1], (*this)[0][2],(*this)[0][3],
					(*this)[1][0],(*this)[1][1], (*this)[1][2],(*this)[1][3],
					(*this)[2][0],(*this)[2][1], (*this)[2][2],(*this)[2][3],
					(*this)[3][0],(*this)[3][1], (*this)[3][2],(*this)[3][3]
			);

		}


		void Mat3::ConvertFromArray(float* arrayMatrix)
		{

			memcpy(this->matrix, arrayMatrix, 9 * sizeof(float));

		}

		void Mat3::SetIdentityMatrix()
		{
			float identityMatrix[] = { 1, 0, 0,
									   0, 1, 0,
									   0, 0, 1};

			this->ConvertFromArray(identityMatrix);

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

			this->ConvertFromArray(rotationMatrix);
		}


		void Mat3::SetScaleMatrix(float scale)
		{
			float scaleMatrix[] = { scale, 0, 0,
									0, scale, 0,
									0, 0, scale };

			this->ConvertFromArray(scaleMatrix);

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

			resultMat3.ConvertFromArray(resultPtr);

			// return the final result with the type Mat4

			return resultMat3;

		}


		Vec3 Mat3::operator*(Vec3 vector)
		{
			float vectorArray[] = { vector.x, vector.y, vector.z};

			glm::mat3x3 leftMatrix = glm::make_mat4(this->matrix);
			glm::vec3 rightVector = glm::make_vec4(vectorArray);
			glm::vec3 result = leftMatrix * rightVector;

			Vec3 resultVec3;

			resultVec3.x = result.x;
			resultVec3.y = result.y;
			resultVec3.z = result.z;

			return resultVec3;


		}


		void Mat3::PrintMatrix()
		{
			printf("Matrix 3x3: \n %.1f, %.1f, %.1f \n %.1f, %.1f, %.1f\n %.1f, %.1f, %.1f \n\n",
					(*this)[0][0],(*this)[0][1], (*this)[0][2],
					(*this)[1][0],(*this)[1][1], (*this)[1][2],
					(*this)[2][0],(*this)[2][1], (*this)[2][2]
			);

		}

	}
}