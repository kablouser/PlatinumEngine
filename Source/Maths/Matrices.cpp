//
// Created by Shihua on 08/03/2022.
//

#include <Maths/Matrices.h>
#include <Maths/Quaternion.h>
#include <Maths/Common.h>
#include <glm/gtc/type_ptr.hpp>
#include <iterator>
#include <glm/gtx/quaternion.hpp>

namespace PlatinumEngine
{
	namespace Maths
	{
		Mat4 Mat4::operator*(float scale)
		{

			Mat4 result;

			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
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
			resultMat4.ConvertFromGLM(result);
			return resultMat4;
		}

		Vec4 Mat4::operator*(Vec4 homogeneousVector)
		{
			float vectorArray[] = { homogeneousVector.x, homogeneousVector.y, homogeneousVector.z,
									homogeneousVector.w };

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

			if (this == &otherMatrix)
				return *this;

			std::memcpy(this->matrix, otherMatrix.matrix, 16 * sizeof(float));

			return (*this);

		}

		void Mat4::Inverse()
		{
			ConvertFromGLM(glm::inverse((glm::mat4)*this));
		}

		void Mat4::Transpose()
		{
			ConvertFromGLM(glm::transpose((glm::mat4)*this));
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
					glm::mat4_cast(quaternionX) * glm::mat4_cast(quaternionY) * glm::mat4_cast(quaternionZ);


			// convert glm::mat4 into Mat4
			float* rotationMatrix = glm::value_ptr(rotationMat4);

		}

		void Mat4::SetRotationMatrix(Quaternion quaternion)
		{
			ConvertFromGLM(glm::toMat4(glm::quat(
					quaternion.w, quaternion.x, quaternion.y, quaternion.z)));
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
			ConvertFromGLM(glm::ortho(left, right, bottom, top, zNear, zFar));
		}

		void Mat4::SetFrustumMatrix(float left, float right, float bottom, float top, float near, float far)
		{
			ConvertFromGLM(glm::frustum(left, right, bottom, top, near, far));
		}

		void Mat4::SetPerspectiveMatrix(float fovy, float aspect, float near, float far)
		{
			ConvertFromGLM(glm::perspective(fovy * Common::DEG2RAD, aspect, near, far));
		}

		void Mat4::SetTRSMatrix(
				PlatinumEngine::Maths::Vec3 translate,
				PlatinumEngine::Maths::Quaternion rotate,
				float scale)
		{
			glm::mat4x4 trs(1.0f);
			trs = glm::translate(trs, { translate.x, translate.y, translate.z });
			trs *= glm::toMat4(glm::quat(rotate.w, rotate.x, rotate.y, rotate.z));
			trs = glm::scale(trs, { scale, scale, scale });
			ConvertFromGLM(trs);
		}

		Mat4::operator glm::mat4() const
		{
			return glm::make_mat4(matrix);
		}

		void Mat4::ConvertFromGLM(glm::mat4 mat4)
		{
			ConvertFromArray(glm::value_ptr(mat4), 16);
		}

		Mat3 Mat3::operator*(float scale)
		{

			Mat3 result;

			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
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
			resultMat3.ConvertFromGLM(result);
			return resultMat3;
		}

		Vec3 Mat3::operator*(Vec3 vector)
		{
			float vectorArray[] = { vector.x, vector.y, vector.z };

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

			if (this == &otherMatrix)
				return *this;

			std::memcpy(this->matrix, otherMatrix.matrix, 9 * sizeof(float));

			return (*this);

		}

		void Mat3::Inverse()
		{
			ConvertFromGLM(glm::inverse((glm::mat3)*this));
		}

		void Mat3::Transpose()
		{
			ConvertFromGLM(glm::transpose((glm::mat3)*this));
		}

		void Mat3::SetIdentityMatrix()
		{
			float identityMatrix[] = { 1, 0, 0,
									   0, 1, 0,
									   0, 0, 1 };

			this->ConvertFromArray(identityMatrix, 9);

		}

		void Mat3::SetRotationMatrix(Vec3 eulerAngle)
		{
			// get rotation matrix (quaternion)
			glm::quat quaternionX(glm::vec3(eulerAngle.x, 0, 0));

			glm::quat quaternionY(glm::vec3(0, eulerAngle.y, 0));

			glm::quat quaternionZ(glm::vec3(0, 0, eulerAngle.z));

			ConvertFromGLM(glm::mat3_cast(quaternionY * quaternionX * quaternionZ));
		}

		void Mat3::SetScaleMatrix(PlatinumEngine::Maths::Vec3 scale)
		{
			float scaleMatrix[] = { scale.x, 0, 0,
									0, scale.y, 0,
									0, 0, scale.z };

			this->ConvertFromArray(scaleMatrix, 9);

		}

		Mat3::operator glm::mat3() const
		{
			return glm::make_mat3(matrix);
		}

		void Mat3::ConvertFromGLM(glm::mat3 mat3)
		{
			this->ConvertFromArray(glm::value_ptr(mat3), 9);
		}

	}
}


namespace PlatinumEngine
{
	namespace Maths
	{
		Mat3 Inverse(Mat3 mat3)
		{
			Mat3 result;
			result.ConvertFromGLM(glm::inverse((glm::mat3)mat3));
			return result;
		}

		Mat3 Transpose(Mat3 mat3)
		{
			Mat3 result;
			result.ConvertFromGLM(glm::transpose((glm::mat3)mat3));
			return result;

		}

		Mat4 Inverse(Mat4 mat4)
		{
			Mat4 result;
			result.ConvertFromGLM(glm::inverse((glm::mat4)mat4));
			return result;
		}

		Mat4 Transpose(Mat4 mat4)
		{
			Mat4 result;
			result.ConvertFromGLM(glm::transpose((glm::mat4)mat4));
			return result;
		}

	}
}


namespace PlatinumEngine
{
	namespace Maths
	{
		Mat3 Inverse(Mat3 matrix3x3)
		{
			glm::mat3x3 glmMatrix3 = glm::make_mat3(matrix3x3.matrix);

			glmMatrix3 = glm::inverse(glmMatrix3);

			float* inverseMatrix3Array = glm::value_ptr(glmMatrix3);

			matrix3x3.ConvertFromArray(inverseMatrix3Array, 9);

			return matrix3x3;
		}

		Mat3 Transpose(Mat3 matrix3x3)
		{
			glm::mat3x3 glmMatrix3 = glm::make_mat3(matrix3x3.matrix);

			glmMatrix3 = glm::transpose(glmMatrix3);

			float* transposedMatrix3Array = glm::value_ptr(glmMatrix3);

			matrix3x3.ConvertFromArray(transposedMatrix3Array, 9);

			return matrix3x3;

		}

		Mat4 Inverse(Mat4 matrix4x4)
		{
			glm::mat4x4 glmMatrix4 = glm::make_mat4(matrix4x4.matrix);

			glmMatrix4 = glm::inverse(glmMatrix4);

			float* inverseMatrix4Array = glm::value_ptr(glmMatrix4);

			matrix4x4.ConvertFromArray(inverseMatrix4Array, 16);

			return matrix4x4;
		}
		Mat4 Transpose(Mat4 matrix4x4)
		{
			glm::mat4x4 glmMatrix4 = glm::make_mat4(matrix4x4.matrix);

			glmMatrix4 = glm::transpose(glmMatrix4);

			float* transposeMatrix4Array = glm::value_ptr(glmMatrix4);

			matrix4x4.ConvertFromArray(transposeMatrix4Array, 16);

			return matrix4x4;
		}

	}
}