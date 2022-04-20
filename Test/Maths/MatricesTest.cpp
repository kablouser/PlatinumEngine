//
// Created by Shihua on 15/03/2022.
//

#include <catch2/catch.hpp>
#include <Maths/Matrices.h>
#include <glm/gtc/type_ptr.hpp>


//________Test Constructor_________

TEST_CASE("MatrixConstructors", "[maths][matrices][vector]")
{
	GIVEN("Four empty Matrix constructors")
	{
		PlatinumEngine::Maths::Matrix<2,2,int> mat1;
		PlatinumEngine::Maths::Matrix<2,3,int> mat2;
		PlatinumEngine::Maths::Matrix<3,3,int> mat3;
		PlatinumEngine::Maths::Matrix<4,4,int> mat4;

		PlatinumEngine::Maths::Mat3 mat5;
		PlatinumEngine::Maths::Mat4 mat6;

		THEN("The values should all be 0")
		{

			REQUIRE(sizeof(mat1.matrix) / sizeof(mat1.matrix[0]) == 4);

			for(int y = 0 ; y < 2; y++)
			{
				for(int x = 0 ; x < 2; x++)
				{
					REQUIRE(mat1[y][x] == 0);
				}


			}

			REQUIRE(sizeof(mat2.matrix) / sizeof(mat2.matrix[0]) == 6);

			for(int y = 0 ; y < 2; y++)
			{
				for(int x = 0 ; x < 3; x++)
				{
					REQUIRE(mat2[y][x] == 0);
				}


			}

			REQUIRE(sizeof(mat3.matrix) / sizeof(mat3.matrix[0]) == 9);

			for(int y = 0 ; y < 3; y++)
			{
				for(int x = 0 ; x < 3; x++)
				{
					REQUIRE(mat3[y][x] == 0);
				}


			}

			REQUIRE(sizeof(mat4.matrix) / sizeof(mat4.matrix[0]) == 16);

			for(int y = 0 ; y < 4; y++)
			{
				for(int x = 0 ; x < 4; x++)
				{
					REQUIRE(mat4[y][x] == 0);
				}


			}


			REQUIRE(sizeof(mat5.matrix) / sizeof(mat5.matrix[0]) == 9);

			for(int y = 0 ; y < 3; y++)
			{
				for(int x = 0 ; x < 3; x++)
				{
					REQUIRE(mat5[y][x] == 0);
				}


			}


			REQUIRE(sizeof(mat6.matrix) / sizeof(mat6.matrix[0]) == 16);

			for(int y = 0 ; y < 4; y++)
			{
				for(int x = 0 ; x < 4; x++)
				{
					REQUIRE(mat6[y][x] == 0);
				}
			}
		}

		THEN("Create matrix with array")
		{

			int temp[] ={1,1,1,1,
						   1,3,1,1,
						   1,2,1,1,
						   1,1,9,1};
			mat4 = PlatinumEngine::Maths::Matrix<4,4,int>(temp);

			REQUIRE(mat4[2][3] == 9);

		}
	}


	GIVEN("Four diagonal Matrix constructors")
	{
		PlatinumEngine::Maths::Matrix<2,2,int> mat1(1);
		PlatinumEngine::Maths::Matrix<2,3,int> mat2(2);

		PlatinumEngine::Maths::Mat3 mat3(1.0f);
		PlatinumEngine::Maths::Mat4 mat4(2.0f);

		THEN("Print the Matrices")
		{
			std::cout<<mat1;
			std::cout<<mat2;
			std::cout<<mat3;
			std::cout<<mat4;
		}

		THEN("The non-zero value should be on diagonal")
		{

			for(int y = 0 ; y < 2; y++)
			{
				for(int x = 0 ; x < 2; x++)
				{
					if(mat1[y][x] == 1)
					{

						REQUIRE(x == y);

					}

				}
			}


			for(int y = 0 ; y < 2; y++)
			{
				for(int x = 0 ; x < 3; x++)
				{
					if(mat2[y][x] == 2)
					{

						REQUIRE(x == y);

					}

				}
			}

			for(int y = 0 ; y < 3; y++)
			{
				for(int x = 0 ; x < 3; x++)
				{
					if(mat3[y][x] == 1.0f)
					{

						REQUIRE(x == y);

					}

				}
			}

			for(int y = 0 ; y < 4; y++)
			{
				for(int x = 0 ; x < 4; x++)
				{
					if(mat4[y][x] == 2.0f)
					{

						REQUIRE(x == y);

					}

				}
			}


		}
	}

}


//________Operations_________

TEST_CASE("MatrixOperations", "[maths][matrices][vector]")
{

	GIVEN("Three different Matrix<2,3> constructors")
	{
		PlatinumEngine::Maths::Matrix<2,3,int> mat1;
		PlatinumEngine::Maths::Matrix<2,3,int> mat2(1);
		PlatinumEngine::Maths::Matrix<2,3,int> mat3(2);

		THEN("Do addition")
		{
			mat1 = mat2 + mat3;
			REQUIRE(mat1[0][0] == 3);

			mat2 = mat2 + mat2;
			REQUIRE(mat2[0][0] == 2);

			mat3 = mat1 + mat3;
			REQUIRE(mat3[1][1] == 5);

		}

		THEN("Do Subtraction")
		{
			mat1 = mat2 - mat3;
			REQUIRE(mat1[0][0] == -1);

			mat2 = mat2 - mat2;
			REQUIRE(mat2[0][0] == 0);

			mat3 = mat1 - mat3;
			REQUIRE(mat3[1][1] == -3);

		}
	}

	GIVEN("Three different Mat3 constructors")
	{
		PlatinumEngine::Maths::Mat3 mat1;
		PlatinumEngine::Maths::Mat3 mat2(1.0f);
		PlatinumEngine::Maths::Mat3 mat3(2.0f);

		THEN("Do addition")
		{
			mat1 = mat2 + mat3;
			REQUIRE(mat1[0][0] == 3);

			mat2 = mat2 + mat2;
			REQUIRE(mat2[0][0] == 2);

			mat3 = mat1 + mat3;
			REQUIRE(mat3[2][2] == 5);

		}

		THEN("Do Subtraction")
		{
			mat1 = mat2 - mat3;
			REQUIRE(mat1[0][0] == -1);

			mat2 = mat2 - mat2;
			REQUIRE(mat2[0][0] == 0);

			mat3 = mat1 - mat3;
			REQUIRE(mat3[2][2] == -3);
		}

		THEN("Do Multiplication")
		{
			mat1 = mat2 * mat3;
			REQUIRE(mat1[0][0] == 2.f);

			mat2 = mat2 * 2.f;
			REQUIRE(mat2[0][0] == 2.f);

			PlatinumEngine::Maths::Vec3 vec3new = mat2 * PlatinumEngine::Maths::Vec3(1.f,2.f,3.f);
			REQUIRE(vec3new[0] == 2.f);
			REQUIRE(vec3new[1] == 4.f);
			REQUIRE(vec3new[2] == 6.f);
		}
	}


	GIVEN("Three different Mat4 constructors")
	{
		PlatinumEngine::Maths::Mat4 mat1;
		PlatinumEngine::Maths::Mat4 mat2(1.0f);
		PlatinumEngine::Maths::Mat4 mat3(2.0f);

		THEN("Do addition")
		{
			mat1 = mat2 + mat3;
			REQUIRE(mat1[0][0] == 3);

			mat2 = mat2 + mat2;
			REQUIRE(mat2[0][0] == 2);

			mat3 = mat1 + mat3;
			REQUIRE(mat3[3][3] == 5);

		}

		THEN("Do Subtraction")
		{
			mat1 = mat2 - mat3;
			REQUIRE(mat1[0][0] == -1);

			mat2 = mat2 - mat2;
			REQUIRE(mat2[0][0] == 0);

			mat3 = mat1 - mat3;
			REQUIRE(mat3[3][3] == -3);

		}

		THEN("Do Multiplication")
		{
			mat1 = mat2 * mat3;
			REQUIRE(mat1[0][0] == 2.f);

			mat2 = mat2 * 2.f;
			REQUIRE(mat2[0][0] == 2.f);

			PlatinumEngine::Maths::Vec4 vec3new = mat2 * PlatinumEngine::Maths::Vec4(1.f,2.f,3.f,4.f);
			REQUIRE(vec3new[0] == 2.f);
			REQUIRE(vec3new[1] == 4.f);
			REQUIRE(vec3new[2] == 6.f);
			REQUIRE(vec3new[3] == 8.f);
		}


	}

}


//________Preset Matrices_________

TEST_CASE("MatrixPreset", "[maths][matrices][vector]")
{
	GIVEN("Three Mat3 matrices and set it to be different type of matrix.")
	{
		PlatinumEngine::Maths::Mat3 mat1, mat2, mat3;

		mat1.SetIdentityMatrix();

		mat2.SetRotationMatrix(PlatinumEngine::Maths::Vec3(3.14f/3.f,0.f,0.f));

		mat3.SetScaleMatrix(PlatinumEngine::Maths::Vec3(2.f,3.f,4.f));



		THEN("Test if the identical matrix is the same as the one in glm.")
		{
			glm::mat3x3 temp(1.0f);

			float* tempArray = glm::value_ptr(temp);

			for (int i = 0; i < 9; i++)
			{

				REQUIRE(mat1.matrix[i] == tempArray[i]);

			}

		}

		THEN("Test if the rotation matrix is the same as the one in glm.")
		{
			glm::mat3x3 temp = glm::mat3_cast(glm::quat(glm::vec3(3.14f/3.f,0.f,0.f)));

			float* tempArray = glm::value_ptr(temp);

			for (int i = 0; i < 9; i++)
			{

				REQUIRE(mat2.matrix[i] == tempArray[i]);

			}

		}

		THEN("Test if the scale matrix is the same as the one in glm.")
		{
			glm::mat4x4 temp(1.f);
			temp = glm::scale(temp, glm::vec3(2.f,3.f,4.f));

			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					REQUIRE(mat3[i][j] == temp[j][i]);
				}
			}

		}

	}

	GIVEN("Seven Mat4 matrices and set it to be different type of matrix.")
	{
		PlatinumEngine::Maths::Mat4 mat1, mat2, mat3, mat4, mat5, mat6, mat7;

		mat1.SetIdentityMatrix();

		mat2.SetRotationMatrix(PlatinumEngine::Maths::Vec3(3.14f/3.f,0.f,0.f));

		mat3.SetScaleMatrix(PlatinumEngine::Maths::Vec3(2.f,3.f,4.f));



		THEN("Test if the identical matrix is the same as the one in glm.")
		{
			glm::mat4x4 temp(1.0f);

			float* tempArray = glm::value_ptr(temp);

			for (int i = 0; i < 16; i++)
			{

				REQUIRE(mat1.matrix[i] == tempArray[i]);

			}

		}

		THEN("Test if the rotation matrix is the same as the one in glm.")
		{
			glm::mat4x4 temp = glm::mat4_cast(glm::quat(glm::vec3(3.14f/3.f,0.f,0.f)));

			float* tempArray = glm::value_ptr(temp);

			for (int i = 0; i < 16; i++)
			{

				REQUIRE(mat2.matrix[i] == tempArray[i]);

			}

		}

		THEN("Test if the scale matrix is the same as the one in glm.")
		{
			glm::mat4x4 temp(1.f);
			temp = glm::scale(temp, glm::vec3(2.f,3.f,4.f));

			float* tempArray = glm::value_ptr(temp);

			for (int i = 0; i < 16; i++)
			{
				REQUIRE(mat3.matrix[i] == tempArray[i]);

			}

		}

		mat4.SetTranslationMatrix(PlatinumEngine::Maths::Vec3(1.f,2.f,3.f));

		mat5.SetOrthogonalMatrix(-1,1,-1,1,-1,1);

		mat6.SetFrustumMatrix(-100,100,-100,100,1,80);

		mat7.SetPerspectiveMatrix(60,1,1,100);



		THEN("Test if the translation matrix is the same as the one in glm.")
		{
			glm::mat4x4 temp(1.f);
			temp = glm::translate(temp,glm::vec3(1.f,2.f,3.f));

			float* tempArray = glm::value_ptr(temp);

			for (int i = 0; i < 16; i++)
			{
				REQUIRE(mat4.matrix[i] == tempArray[i]);

			}

		}

		THEN("Test if the Ortho matrix is the same as the one in glm.")
		{
			glm::mat4x4 temp(1.f);
			temp = glm::ortho(-1.f,1.f,-1.f,1.f,-1.f,1.f);

			float* tempArray = glm::value_ptr(temp);

			for (int i = 0; i < 16; i++)
			{
				REQUIRE(mat5.matrix[i] == tempArray[i]);

			}

		}

		THEN("Test if the Frustum matrix is the same as the one in glm.")
		{
			glm::mat4x4 temp(1.f);
			temp = glm::frustum(-100.f,100.f,-100.f,100.f,1.f,80.f);

			float* tempArray = glm::value_ptr(temp);

			for (int i = 0; i < 16; i++)
			{
				REQUIRE(mat6.matrix[i] == tempArray[i]);

			}

		}


		THEN("Test if the Frustum matrix is the same as the one in glm.")
		{
			glm::mat4x4 temp(1.f);
			temp = glm::perspective(60.f,1.f,1.f,100.f);

			float* tempArray = glm::value_ptr(temp);

			for (int i = 0; i < 16; i++)
			{
				REQUIRE(mat7.matrix[i] == tempArray[i]);

			}

		}


	}


}


TEST_CASE("Test inverse and transpose functions")
{
	GIVEN("Two Mat3 and Mat4 variables.")
	{
		PlatinumEngine::Maths::Mat3 mat31;
		mat31.SetRotationMatrix(PlatinumEngine::Maths::Vec3(3.14/6.f, 0.f, 0.f));
		PlatinumEngine::Maths::Mat3 mat32;
		mat32.SetRotationMatrix(PlatinumEngine::Maths::Vec3(3.14/6.f, 0.f, 0.f));
		PlatinumEngine::Maths::Mat4 mat41;
		mat41.SetRotationMatrix(PlatinumEngine::Maths::Vec3(3.14/6.f, 0.f, 0.f));
		PlatinumEngine::Maths::Mat4 mat42;
		mat42.SetRotationMatrix(PlatinumEngine::Maths::Vec3(3.14/6.f, 0.f, 0.f));

		THEN("Apply inverse functions")
		{

			PlatinumEngine::Maths::Mat3 inverseMatrix3 = PlatinumEngine::Maths::Inverse(mat31);
			mat32.Inverse();

			std::cout<<mat31<<std::endl;
			std::cout<<inverseMatrix3<<std::endl;
			std::cout<<mat32<<std::endl;



			PlatinumEngine::Maths::Mat4 inverseMatrix4 = PlatinumEngine::Maths::Inverse(mat41);
			mat42.Inverse();

			std::cout<<mat41<<std::endl;
			std::cout<<inverseMatrix4<<std::endl;
			std::cout<<mat42<<std::endl;

		}

		THEN("Apply transpose functions")
		{

			PlatinumEngine::Maths::Mat3 inverseMatrix3 = PlatinumEngine::Maths::Transpose(mat31);
			mat32.Transpose();

			std::cout<<mat31<<std::endl;
			std::cout<<inverseMatrix3<<std::endl;
			std::cout<<mat32<<std::endl;



			PlatinumEngine::Maths::Mat4 inverseMatrix4 = PlatinumEngine::Maths::Transpose(mat41);
			mat42.Transpose();

			std::cout<<mat41<<std::endl;
			std::cout<<inverseMatrix4<<std::endl;
			std::cout<<mat42<<std::endl;

		}

	}

}



//_______END_________