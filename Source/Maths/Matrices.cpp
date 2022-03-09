//
// Created by Shihua on 08/03/2022.
//

#include <Maths/Matrices.h>
#include <glm/gtc/type_ptr.hpp>

using namespace PlatinumEngine::Maths;

void Mat4::ConvertFromGLM(glm::mat4x4 glmMat4)
{
	const float* matPtr =  glm::value_ptr(glmMat4);

	for(int i =0 ;i < 16;i++)
	{
		this->matrix[i] = matPtr[i];
	}
}


Mat4 Mat4::operator *(Mat4 anotherMat4)
{
	glm::mat4x4 leftMatrix  = glm::make_mat4(this->matrix);
	glm::mat4x4 rightMatrix = glm::make_mat4(anotherMat4.matrix);
	glm::mat4x4 result = leftMatrix * rightMatrix;

	Mat4 resultMat4;
	resultMat4.ConvertFromGLM(result);

	return resultMat4;
}

Mat4 Mat4::operator *(Vec4 homogeneousVector)
{



}