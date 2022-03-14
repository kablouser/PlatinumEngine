//
// Created by Matt on 14/03/2022.
//

#include <catch2/catch.hpp>
#include <Maths/Vectors.h>
#include <sstream>
#include <string>

// ====== Vec4 Unit Tests ====== //
TEST_CASE("Vectors4Constructors", "[maths][vectors][vec4]")
{
	GIVEN("An empty Vec4 constructor")
	{
		PlatinumEngine::Maths::Vec4 vec4;
		THEN("The values should be 0")
		{
			CHECK(vec4.x == 0.0f);
			CHECK(vec4.y == 0.0f);
			CHECK(vec4.z == 0.0f);
			CHECK(vec4.w == 0.0f);
		}
	}

	GIVEN("3 non-zero values in Vec4 constructor")
	{
		PlatinumEngine::Maths::Vec4 vec4(1.0f, 2.0f, 3.0f);
		THEN("Only w is zero")
		{
			CHECK(vec4.x == 1.0f);
			CHECK(vec4.y == 2.0f);
			CHECK(vec4.z == 3.0f);
			CHECK(vec4.w == 0.0f);
		}
	}

	GIVEN("4 non-zero values in Vec4 constructor")
	{
		PlatinumEngine::Maths::Vec4 vec4(1.0f, 2.0f, 3.0f, 4.0f);
		THEN("All values non-zero")
		{
			CHECK(vec4.x == 1.0f);
			CHECK(vec4.y == 2.0f);
			CHECK(vec4.z == 3.0f);
			CHECK(vec4.w == 4.0f);
		}
	}
}

TEST_CASE("Vectors4Adding", "[maths][vectors][vec4]")
{
	GIVEN("Two Vector4's")
	{
		PlatinumEngine::Maths::Vec4 vec1(1.0f, 2.0f, 3.0f, 4.0f);
		PlatinumEngine::Maths::Vec4 vec2(1.0f, 2.0f, 3.0f, 4.0f);
		WHEN("The vectors are added together")
		{
			PlatinumEngine::Maths::Vec4 added = vec1 + vec2;
			THEN("They create a new Vec3 with correct values")
			{
				CHECK(added.x == 2.0f);
				CHECK(added.y == 4.0f);
				CHECK(added.z == 6.0f);
				CHECK(added.w == 8.0f);
			}
		}
	}
	GIVEN("One Vector4")
	{
		PlatinumEngine::Maths::Vec4 vec1(1.0f, 2.0f, 3.0f, 4.0f);
		WHEN("The vector is added to itself")
		{
			vec1 += vec1;
			THEN("Vector is changed to new values")
			{
				CHECK(vec1.x == 2.0f);
				CHECK(vec1.y == 4.0f);
				CHECK(vec1.z == 6.0f);
				CHECK(vec1.w == 8.0f);
			}
		}
	}
}

TEST_CASE("Vectors4Subtracting", "[maths][vectors][vec4]")
{
	GIVEN("Two Vector4's")
	{
		PlatinumEngine::Maths::Vec4 vec1(1.0f, 2.0f, 3.0f, 5.0f);
		PlatinumEngine::Maths::Vec4 vec2(6.0f, 5.0f, 4.0f, 8.0f);
		WHEN("One vector is subtracted from the other")
		{
			PlatinumEngine::Maths::Vec4 subtracted = vec2 - vec1;
			THEN("They create a new Vec3 with correct values")
			{
				CHECK(subtracted.x == 5.0f);
				CHECK(subtracted.y == 3.0f);
				CHECK(subtracted.z == 1.0f);
				CHECK(subtracted.w == 3.0f);
			}
		}
	}
	GIVEN("One Vector4")
	{
		PlatinumEngine::Maths::Vec4 vec1(1.0f, 2.0f, 3.0f, 4.0f);
		WHEN("The vector is subtracted from itself")
		{
			vec1 -= vec1;
			THEN("Vector is changed to new values")
			{
				CHECK(vec1.x == 0.0f);
				CHECK(vec1.y == 0.0f);
				CHECK(vec1.z == 0.0f);
				CHECK(vec1.w == 0.0f);
			}
		}
	}
}

TEST_CASE("Vectors4Multiplying", "[maths][vectors][vec4]")
{
	GIVEN("Two Vector4's")
	{
		PlatinumEngine::Maths::Vec4 vec1(1.0f, 2.0f, 3.0f, 4.0f);
		PlatinumEngine::Maths::Vec4 vec2(6.0f, 5.0f, 4.0f, 7.0f);
		WHEN("They are multiplied together")
		{
			PlatinumEngine::Maths::Vec4 multiplied = vec2*vec1;
			THEN("They create a new Vec4 with correct values")
			{
				CHECK(multiplied.x == 6.0f);
				CHECK(multiplied.y == 10.0f);
				CHECK(multiplied.z == 12.0f);
				CHECK(multiplied.w == 28.0f);
			}
		}
	}

	GIVEN("One Vector4")
	{
		PlatinumEngine::Maths::Vec4 vec1(1.0f, 2.0f, 3.0f, 4.0f);
		WHEN("Left multiplied with a scalar")
		{
			PlatinumEngine::Maths::Vec4 multiplied = 2.0f*vec1;
			THEN("They create a new Vec4 with correct values")
			{
				CHECK(multiplied.x == 2.0f);
				CHECK(multiplied.y == 4.0f);
				CHECK(multiplied.z == 6.0f);
				CHECK(multiplied.w == 8.0f);
			}
		}

		WHEN("Right multiplied with a scalar")
		{
			PlatinumEngine::Maths::Vec4 multiplied = vec1*2.0f;
			THEN("They create a new Vec4 with correct values")
			{
				CHECK(multiplied.x == 2.0f);
				CHECK(multiplied.y == 4.0f);
				CHECK(multiplied.z == 6.0f);
				CHECK(multiplied.w == 8.0f);
			}
		}

		WHEN("Multiplied by a scalar to itself")
		{
			vec1 *= 2.0f;
			THEN("The vectors values are changed correctly")
			{
				CHECK(vec1.x == 2.0f);
				CHECK(vec1.y == 4.0f);
				CHECK(vec1.z == 6.0f);
				CHECK(vec1.w == 8.0f);
			}
		}
	}
}

TEST_CASE("Vectors4Division", "[maths][vectors][vec4]")
{
	GIVEN("A Vector4")
	{
		PlatinumEngine::Maths::Vec4 vec4(4.0f, 6.0f, 8.0f, 10.0f);
		WHEN("Divide by a scalar")
		{
			PlatinumEngine::Maths::Vec4 divided = vec4 / 2.0f;
			THEN("Creates a new Vec3 with correct values")
			{
				CHECK(divided.x == 2.0f);
				CHECK(divided.y == 3.0f);
				CHECK(divided.z == 4.0f);
				CHECK(divided.w == 5.0f);
			}
		}

		WHEN("Divide by a scalar to itself")
		{
			vec4 /= 2.0f;
			THEN("The Vector values are changed correctly")
			{
				CHECK(vec4.x == 2.0f);
				CHECK(vec4.y == 3.0f);
				CHECK(vec4.z == 4.0f);
				CHECK(vec4.w == 5.0f);
			}
		}
	}
}

TEST_CASE("Vectors4OutStream", "[maths][vectors][vec4]")
{
	std::stringstream ss;
	GIVEN("One Vector4")
	{
		PlatinumEngine::Maths::Vec4 vec4(1.0f, 2.0f, 3.0f, 4.0f);
		WHEN("Using an output stream for the vector")
		{
			ss << vec4;
			THEN("Vector is formatted correctly")
			{
				CHECK(ss.str() == std::string{"1 2 3 4"});
			}
		}
	}
}

// ====== END Vec4 Unit Tests ====== //

// ====== Vec3 Unit Tests ====== //

TEST_CASE("Vectors3Constructors", "[maths][vectors][vec3]")
{
	GIVEN("An empty Vec3 constructor")
	{
		PlatinumEngine::Maths::Vec3 vec3;
		THEN("The values should be 0")
		{
			CHECK(vec3.x == 0.0f);
			CHECK(vec3.y == 0.0f);
			CHECK(vec3.z == 0.0f);
		}
	}

	GIVEN("3 non-zero values in Vec3 constructor")
	{
		PlatinumEngine::Maths::Vec3 vec3(1.0f, 2.0f, 3.0f);
		THEN("All values are non-zero")
		{
			CHECK(vec3.x == 1.0f);
			CHECK(vec3.y == 2.0f);
			CHECK(vec3.z == 3.0f);
		}
	}
}

TEST_CASE("Vectors3Adding", "[maths][vectors][vec3]")
{
	GIVEN("Two Vector3's")
	{
		PlatinumEngine::Maths::Vec3 vec1(1.0f, 2.0f, 3.0f);
		PlatinumEngine::Maths::Vec3 vec2(1.0f, 2.0f, 3.0f);
		WHEN("The vectors are added together")
		{
			PlatinumEngine::Maths::Vec3 added = vec1 + vec2;
			THEN("They create a new Vec3 with correct values")
			{
				CHECK(added.x == 2.0f);
				CHECK(added.y == 4.0f);
				CHECK(added.z == 6.0f);
			}
		}
	}
	GIVEN("One Vector3")
	{
		PlatinumEngine::Maths::Vec3 vec1(1.0f, 2.0f, 3.0f);
		WHEN("The vector is added to itself")
		{
			vec1 += vec1;
			THEN("Vector is changed to new values")
			{
				CHECK(vec1.x == 2.0f);
				CHECK(vec1.y == 4.0f);
				CHECK(vec1.z == 6.0f);
			}
		}
	}
}

TEST_CASE("Vectors3Subtracting", "[maths][vectors][vec3]")
{
	GIVEN("Two Vector3's")
	{
		PlatinumEngine::Maths::Vec3 vec1(1.0f, 2.0f, 3.0f);
		PlatinumEngine::Maths::Vec3 vec2(6.0f, 5.0f, 4.0f);
		WHEN("One vector is subtracted from the other")
		{
			PlatinumEngine::Maths::Vec3 subtracted = vec2 - vec1;
			THEN("They create a new Vec3 with correct values")
			{
				CHECK(subtracted.x == 5.0f);
				CHECK(subtracted.y == 3.0f);
				CHECK(subtracted.z == 1.0f);
			}
		}
	}
	GIVEN("One Vector3")
	{
		PlatinumEngine::Maths::Vec3 vec1(1.0f, 2.0f, 3.0f);
		WHEN("The vector is subtracted from itself")
		{
			vec1 -= vec1;
			THEN("Vector is changed to new values")
			{
				CHECK(vec1.x == 0.0f);
				CHECK(vec1.y == 0.0f);
				CHECK(vec1.z == 0.0f);
			}
		}
	}
}

TEST_CASE("Vectors3Multiplying", "[maths][vectors][vec3]")
{
	GIVEN("Two Vector3's")
	{
		PlatinumEngine::Maths::Vec3 vec1(1.0f, 2.0f, 3.0f);
		PlatinumEngine::Maths::Vec3 vec2(6.0f, 5.0f, 4.0f);
		WHEN("They are multiplied together")
		{
			PlatinumEngine::Maths::Vec3 multiplied = vec2*vec1;
			THEN("They create a new Vec3 with correct values")
			{
				CHECK(multiplied.x == 6.0f);
				CHECK(multiplied.y == 10.0f);
				CHECK(multiplied.z == 12.0f);
			}
		}
	}

	GIVEN("One Vector3")
	{
		PlatinumEngine::Maths::Vec3 vec1(1.0f, 2.0f, 3.0f);
		WHEN("Left multiplied with a scalar")
		{
			PlatinumEngine::Maths::Vec3 multiplied = 2.0f*vec1;
			THEN("They create a new Vec3 with correct values")
			{
				CHECK(multiplied.x == 2.0f);
				CHECK(multiplied.y == 4.0f);
				CHECK(multiplied.z == 6.0f);
			}
		}

		WHEN("Right multiplied with a scalar")
		{
			PlatinumEngine::Maths::Vec3 multiplied = vec1*2.0f;
			THEN("They create a new Vec3 with correct values")
			{
				CHECK(multiplied.x == 2.0f);
				CHECK(multiplied.y == 4.0f);
				CHECK(multiplied.z == 6.0f);
			}
		}

		WHEN("Multiplied by a scalar to itself")
		{
			vec1 *= 2.0f;
			THEN("The vectors values are changed correctly")
			{
				CHECK(vec1.x == 2.0f);
				CHECK(vec1.y == 4.0f);
				CHECK(vec1.z == 6.0f);
			}
		}
	}
}

TEST_CASE("Vectors3Division", "[maths][vectors][vec3]")
{
	GIVEN("A Vector3")
	{
		PlatinumEngine::Maths::Vec3 vec3(4.0f, 6.0f, 8.0f);
		WHEN("Divide by a scalar")
		{
			PlatinumEngine::Maths::Vec3 divided = vec3 / 2.0f;
			THEN("Creates a new Vec3 with correct values")
			{
				CHECK(divided.x == 2.0f);
				CHECK(divided.y == 3.0f);
				CHECK(divided.z == 4.0f);
			}
		}

		WHEN("Divide by a scalar to itself")
		{
			vec3 /= 2.0f;
			THEN("The Vector values are changed correctly")
			{
				CHECK(vec3.x == 2.0f);
				CHECK(vec3.y == 3.0f);
				CHECK(vec3.z == 4.0f);
			}
		}
	}
}

TEST_CASE("Vectors3OutStream", "[maths][vectors][vec3]")
{
	std::stringstream ss;
	GIVEN("One Vector3")
	{
		PlatinumEngine::Maths::Vec3 vec3(1.0f, 2.0f, 3.0f);
		WHEN("Using an output stream for the vector")
		{
			ss << vec3;
			THEN("Vector is formatted correctly")
			{
				CHECK(ss.str() == std::string{"1 2 3"});
			}
		}
	}
}

TEST_CASE("Vectors3Cross", "[maths][vectors][vec3]")
{
	GIVEN("Two Vector3's")
	{
		PlatinumEngine::Maths::Vec3 vec1(1.0f, 2.0f, 3.0f);
		PlatinumEngine::Maths::Vec3 vec2(6.0f, 5.0f, 4.0f);
		WHEN("Cross product is performed between them")
		{
			PlatinumEngine::Maths::Vec3 crossed = PlatinumEngine::Maths::Cross(vec1, vec2);
			THEN("Calculation performed and return a new vec")
			{
				CHECK(crossed.x == -7.0f);
				CHECK(crossed.y == 14.0f);
				CHECK(crossed.z == -7.0f);
			}
		}
	}
}

// ====== END Vec3 Unit Tests ====== //

// ====== Vec2 Unit Tests ====== //

TEST_CASE("Vectors2Constructors", "[maths][vectors][vec2]")
{
	GIVEN("An empty Vec2 constructor")
	{
		PlatinumEngine::Maths::Vec2 vec2;
		THEN("The values should be 0")
		{
			CHECK(vec2.x == 0.0f);
			CHECK(vec2.y == 0.0f);
		}
	}

	GIVEN("2 non-zero values in Vec2 constructor")
	{
		PlatinumEngine::Maths::Vec2 vec2(1.0f, 2.0f);
		THEN("All values are non-zero")
		{
			CHECK(vec2.x == 1.0f);
			CHECK(vec2.y == 2.0f);
		}
	}
}

TEST_CASE("Vectors2Adding", "[maths][vectors][vec2]")
{
	GIVEN("Two Vector2's")
	{
		PlatinumEngine::Maths::Vec2 vec1(1.0f, 2.0f);
		PlatinumEngine::Maths::Vec2 vec2(1.0f, 2.0f);
		WHEN("The vectors are added together")
		{
			PlatinumEngine::Maths::Vec2 added = vec1 + vec2;
			THEN("They create a new Vec3 with correct values")
			{
				CHECK(added.x == 2.0f);
				CHECK(added.y == 4.0f);
			}
		}
	}
	GIVEN("One Vector2")
	{
		PlatinumEngine::Maths::Vec2 vec1(1.0f, 2.0f);
		WHEN("The vector is added to itself")
		{
			vec1 += vec1;
			THEN("Vector is changed to new values")
			{
				CHECK(vec1.x == 2.0f);
				CHECK(vec1.y == 4.0f);
			}
		}
	}
}

TEST_CASE("Vectors2Subtracting", "[maths][vectors][vec2]")
{
	GIVEN("Two Vector2's")
	{
		PlatinumEngine::Maths::Vec2 vec1(1.0f, 2.0f);
		PlatinumEngine::Maths::Vec2 vec2(6.0f, 5.0f);
		WHEN("One vector is subtracted from the other")
		{
			PlatinumEngine::Maths::Vec2 subtracted = vec2 - vec1;
			THEN("They create a new Vec2 with correct values")
			{
				CHECK(subtracted.x == 5.0f);
				CHECK(subtracted.y == 3.0f);
			}
		}
	}
	GIVEN("One Vector2")
	{
		PlatinumEngine::Maths::Vec2 vec1(1.0f, 2.0f);
		WHEN("The vector is subtracted from itself")
		{
			vec1 -= vec1;
			THEN("Vector is changed to new values")
			{
				CHECK(vec1.x == 0.0f);
				CHECK(vec1.y == 0.0f);
			}
		}
	}
}

TEST_CASE("Vectors2Multiplying", "[maths][vectors][vec2]")
{
	GIVEN("Two Vector2's")
	{
		PlatinumEngine::Maths::Vec2 vec1(1.0f, 2.0f);
		PlatinumEngine::Maths::Vec2 vec2(6.0f, 5.0f);
		WHEN("They are multiplied together")
		{
			PlatinumEngine::Maths::Vec2 multiplied = vec2*vec1;
			THEN("They create a new Vec3 with correct values")
			{
				CHECK(multiplied.x == 6.0f);
				CHECK(multiplied.y == 10.0f);
			}
		}
	}

	GIVEN("One Vector2")
	{
		PlatinumEngine::Maths::Vec2 vec1(1.0f, 2.0f);
		WHEN("Left multiplied with a scalar")
		{
			PlatinumEngine::Maths::Vec2 multiplied = 2.0f*vec1;
			THEN("They create a new Vec2 with correct values")
			{
				CHECK(multiplied.x == 2.0f);
				CHECK(multiplied.y == 4.0f);
			}
		}

		WHEN("Right multiplied with a scalar")
		{
			PlatinumEngine::Maths::Vec2 multiplied = vec1*2.0f;
			THEN("They create a new Vec2 with correct values")
			{
				CHECK(multiplied.x == 2.0f);
				CHECK(multiplied.y == 4.0f);
			}
		}

		WHEN("Multiplied by a scalar to itself")
		{
			vec1 *= 2.0f;
			THEN("The vectors values are changed correctly")
			{
				CHECK(vec1.x == 2.0f);
				CHECK(vec1.y == 4.0f);
			}
		}
	}
}

TEST_CASE("Vectors2Division", "[maths][vectors][vec2]")
{
	GIVEN("A Vector2")
	{
		PlatinumEngine::Maths::Vec2 vec2(4.0f, 6.0f);
		WHEN("Divide by a scalar")
		{
			PlatinumEngine::Maths::Vec2 divided = vec2 / 2.0f;
			THEN("Creates a new Vec2 with correct values")
			{
				CHECK(divided.x == 2.0f);
				CHECK(divided.y == 3.0f);
			}
		}

		WHEN("Divide by a scalar to itself")
		{
			vec2 /= 2.0f;
			THEN("The Vector values are changed correctly")
			{
				CHECK(vec2.x == 2.0f);
				CHECK(vec2.y == 3.0f);
			}
		}
	}
}

TEST_CASE("Vectors2OutStream", "[maths][vectors][vec2]")
{
	std::stringstream ss;
	GIVEN("One Vector2")
	{
		PlatinumEngine::Maths::Vec2 vec2(1.0f, 2.0f);
		WHEN("Using an output stream for the vector")
		{
			ss << vec2;
			THEN("Vector is formatted correctly")
			{
				CHECK(ss.str() == std::string{"1 2"});
			}
		}
	}
}

// ====== END Vec2 Unit Tests ====== //

// ====== General Vector Operations ====== //

TEST_CASE("VectorsDotProduct", "[maths][vectors][dotproduct]")
{
	GIVEN("Two Vector4's")
	{
		PlatinumEngine::Maths::Vec4 vec1(1.0f, 2.0f, 3.0f, 4.0f);
		PlatinumEngine::Maths::Vec4 vec2(5.0f, 6.0f, 7.0f, 8.0f);
		WHEN("They are dot producted together")
		{
			float dotProduct = PlatinumEngine::Maths::Dot(vec1, vec2);
			THEN("The result should be")
			{
				CHECK(dotProduct == 70.0f);
			}
		}
	}

	GIVEN("Two Vector3's")
	{
		PlatinumEngine::Maths::Vec3 vec1(1.0f, 2.0f, 3.0f);
		PlatinumEngine::Maths::Vec3 vec2(5.0f, 6.0f, 7.0f);
		WHEN("They are dot producted together")
		{
			float dotProduct = PlatinumEngine::Maths::Dot(vec1, vec2);
			THEN("The result should be")
			{
				CHECK(dotProduct == 38.0f);
			}
		}
	}

	GIVEN("Two Vector2's")
	{
		PlatinumEngine::Maths::Vec2 vec1(1.0f, 2.0f);
		PlatinumEngine::Maths::Vec2 vec2(5.0f, 6.0f);
		WHEN("They are dot producted together")
		{
			float dotProduct = PlatinumEngine::Maths::Dot(vec1, vec2);
			THEN("The result should be")
			{
				CHECK(dotProduct == 17.0f);
			}
		}
	}
}

TEST_CASE("VectorsLength", "[maths][vectors][length]")
{
	GIVEN("A Vector4")
	{
		PlatinumEngine::Maths::Vec4 vec1(1.0f, 2.0f, 3.0f, 4.0f);
		float length = PlatinumEngine::Maths::Length(vec1);
		THEN("The length should be")
		{
			Approx target = Approx(5.47723f).epsilon(0.01);
			CHECK(length == target);
		}
	}

	GIVEN("A Vector3")
	{
		PlatinumEngine::Maths::Vec3 vec1(1.0f, 2.0f, 3.0f);
		float length = PlatinumEngine::Maths::Length(vec1);
		THEN("The length should be")
		{
			Approx target = Approx(3.74166f).epsilon(0.01);
			CHECK(length == target);
		}
	}

	GIVEN("A Vector2")
	{
		PlatinumEngine::Maths::Vec2 vec1(1.0f, 2.0f);
		float length = PlatinumEngine::Maths::Length(vec1);
		THEN("The length should be")
		{
			Approx target = Approx(2.23607f).epsilon(0.01);
			CHECK(length == target);
		}
	}
}

TEST_CASE("VectorsNormalise", "[maths][vectors][normalise]")
{
	GIVEN("A Vector4")
	{
		PlatinumEngine::Maths::Vec4 vec1(1.0f, 2.0f, 3.0f, 4.0f);
		PlatinumEngine::Maths::Vec4 normalised = PlatinumEngine::Maths::Normalise(vec1);
		THEN("The normalised vector should be")
		{
			Approx targetX = Approx(0.18257f).epsilon(0.01);
			Approx targetY = Approx(0.36515f).epsilon(0.01);
			Approx targetZ = Approx(0.54772f).epsilon(0.01);
			Approx targetW = Approx(0.7303f).epsilon(0.01);
			CHECK(normalised.x == targetX);
			CHECK(normalised.y == targetY);
			CHECK(normalised.z == targetZ);
			CHECK(normalised.w == targetW);
		}
	}

	GIVEN("A Vector3")
	{
		PlatinumEngine::Maths::Vec3 vec1(1.0f, 2.0f, 3.0f);
		PlatinumEngine::Maths::Vec3 normalised = PlatinumEngine::Maths::Normalise(vec1);
		THEN("The normalised vector should be")
		{
			Approx targetX = Approx(0.26726f).epsilon(0.01);
			Approx targetY = Approx(0.53452f).epsilon(0.01);
			Approx targetZ = Approx(0.80178f).epsilon(0.01);
			CHECK(normalised.x == targetX);
			CHECK(normalised.y == targetY);
			CHECK(normalised.z == targetZ);
		}
	}

	GIVEN("A Vector2")
	{
		PlatinumEngine::Maths::Vec2 vec1(1.0f, 2.0f);
		PlatinumEngine::Maths::Vec2 normalised = PlatinumEngine::Maths::Normalise(vec1);
		THEN("The normalised vector should be")
		{
			Approx targetX = Approx(0.44721f).epsilon(0.01);
			Approx targetY = Approx(0.89443f).epsilon(0.01);
			CHECK(normalised.x == targetX);
			CHECK(normalised.y == targetY);
		}
	}
}

TEST_CASE("VectorsDistance", "[maths][vectors][distance]")
{
	GIVEN("Two Vector4's")
	{
		PlatinumEngine::Maths::Vec4 vec1(1.0f, 2.0f, 3.0f, 4.0f);
		PlatinumEngine::Maths::Vec4 vec2(5.0f, 6.0f, 7.0f, 8.0f);
		float distance = PlatinumEngine::Maths::Distance(vec1, vec2);
		THEN("Then the distance between them is")
		{
			Approx target = Approx(8.0f).epsilon(0.01);
			CHECK(distance == target);
		}
	}

	GIVEN("Two Vector3's")
	{
		PlatinumEngine::Maths::Vec3 vec1(1.0f, 2.0f, 3.0f);
		PlatinumEngine::Maths::Vec3 vec2(5.0f, 6.0f, 7.0f);
		float distance = PlatinumEngine::Maths::Distance(vec1, vec2);
		THEN("Then the distance between them is")
		{
			Approx target = Approx(6.9282f).epsilon(0.01);
			CHECK(distance == target);
		}
	}

	GIVEN("Two Vector2's")
	{
		PlatinumEngine::Maths::Vec2 vec1(1.0f, 2.0f);
		PlatinumEngine::Maths::Vec2 vec2(5.0f, 6.0f);
		float distance = PlatinumEngine::Maths::Distance(vec1, vec2);
		THEN("Then the distance between them is")
		{
			Approx target = Approx(5.65685f).epsilon(0.01);
			CHECK(distance == target);
		}
	}
}

TEST_CASE("VectorsLengthSquared", "[maths][vectors][lengthsquared]")
{
	GIVEN("A Vector4")
	{
		PlatinumEngine::Maths::Vec4 vec1(1.0f, 2.0f, 3.0f, 4.0f);
		float lengthsquared = PlatinumEngine::Maths::LengthSquared(vec1);
		THEN("The length squared should be")
		{
			Approx target = Approx(30.0f).epsilon(0.01);
			CHECK(lengthsquared == target);
		}
	}

	GIVEN("A Vector3")
	{
		PlatinumEngine::Maths::Vec3 vec1(1.0f, 2.0f, 3.0f);
		float lengthsquared = PlatinumEngine::Maths::LengthSquared(vec1);
		THEN("The length should be")
		{
			Approx target = Approx(14.0f).epsilon(0.01);
			CHECK(lengthsquared == target);
		}
	}

	GIVEN("A Vector2")
	{
		PlatinumEngine::Maths::Vec2 vec1(1.0f, 2.0f);
		float lengthsquared = PlatinumEngine::Maths::LengthSquared(vec1);
		THEN("The length should be")
		{
			Approx target = Approx(5.0f).epsilon(0.01);
			CHECK(lengthsquared == target);
		}
	}
}

// ====== END General Vector Operations ====== //
