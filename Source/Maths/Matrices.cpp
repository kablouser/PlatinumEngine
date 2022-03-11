//
// Created by Shihua on 08/03/2022.
//

#include <Maths/Matrices.h>
#include <glm/gtc/type_ptr.hpp>
#include <iterator>
using namespace PlatinumEngine::Maths;

void Mat4::ConvertFromArray(float* arrayMatrix)
{

	memcpy(this->matrix, arrayMatrix, 16*sizeof(float));

}

Mat4 Mat4::operator *(Mat4 anotherMat4)
{

	glm::mat4x4 leftMatrix  = glm::make_mat4(this->matrix);
	glm::mat4x4 rightMatrix = glm::make_mat4(anotherMat4.matrix);
	glm::mat4x4 result = leftMatrix * rightMatrix;




	// create a new Mat4 object

	Mat4 resultMat4;

	// get the glm::mat4x4 array pointer

	float* resultPtr =  glm::value_ptr(result);

	// convert the array into Mat4

	resultMat4.ConvertFromArray(resultPtr);

	// return the final result with the type Mat4

	return resultMat4;
}

Vec4 Mat4::operator *(Vec4 homogeneousVector)
{
	float test[] = {homogeneousVector.x, homogeneousVector.y, homogeneousVector.z,homogeneousVector.w};

	glm::mat4x4 leftMatrix  = glm::make_mat4(this->matrix);
	glm::vec4 rightVector   = glm::make_vec4(test);
	glm::vec4 result        = leftMatrix * rightVector;

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
							   0,0,0,1 };

	this->ConvertFromArray(identityMatrix);

}

void Mat4::SetTranslationMatrix(Vec3 translationDirection)
{

	float translationMatrix[] = { 1, 0, 0, translationDirection.x,
							   0, 1, 0, translationDirection.y,
							   0, 0, 1,translationDirection.z,
							   0,0,0,1};

	this->ConvertFromArray(translationMatrix);
}

void Mat4::SetRotationMatrix(Vec3 eulerAngle)
{

	// get rotation matrix (quaternion)
	glm::quat quaternionX(glm::vec3( eulerAngle.x, 0 , 0));

	glm::quat quaternionY(glm::vec3(0, eulerAngle.y, 0));

	glm::quat quaternionZ(glm::vec3( 0, 0, eulerAngle.z));

	glm::mat4x4 rotationMat4 = glm::mat4_cast(quaternionY) * glm::mat4_cast(quaternionX) * glm::mat4_cast(quaternionZ) ;


	// convert glm::mat4 into Mat4
	float* rotationMatrix = glm::value_ptr(rotationMat4);

	this->ConvertFromArray(rotationMatrix);

}

void Mat4::SetScaleMatrix(float scale)
{

	float scaleMatrix[] = { scale, 0, 0, 0,
								  0, scale, 0, 0,
								  0, 0, scale, 0,
								  0,0,0,1 };

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
