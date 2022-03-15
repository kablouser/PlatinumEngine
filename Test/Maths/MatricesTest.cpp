//
// Created by Shihua on 15/03/2022.
//

#include <catch2/catch.hpp>
#include <Maths/Matrices.h>


TEST_CASE("MatrixConstructors", "[maths][matrices][vec4]")
{
	GIVEN("Four empty Matrix constructors")
	{
		PlatinumEngine::Maths::Matrix<2,2,int> mat1;
		PlatinumEngine::Maths::Matrix<2,3,int> mat2;
		PlatinumEngine::Maths::Matrix<3,3,int> mat3;
		PlatinumEngine::Maths::Matrix<4,4,int> mat4;

		THEN("The values should all be 0")
		{

			for(int &i : mat1.matrix)
			{
				

			}


		}
	}

}
