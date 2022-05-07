#include <catch2/catch.hpp>
#include <IDSystem/IDSystem.h>
#include <TypeDatabase/TypeDatabase.h>
#include <sstream>

using namespace PlatinumEngine;

TEST_CASE("TestIDSystem", "[IDSystem]")
{
	struct CustomStructure
	{
		int x,y,z;
	};

	PlatinumEngine::TypeDatabase typeDatabase;
	typeDatabase.CreateIfAutomatic<int>();
	typeDatabase.CreateIfAutomatic<float>();
	typeDatabase.CreateIfAutomatic<double>();
	typeDatabase.BeginTypeInfo<CustomStructure>()
			.WithField<int>("x", PLATINUM_OFFSETOF(CustomStructure, x))
			.WithField<int>("y", PLATINUM_OFFSETOF(CustomStructure, y))
			.WithField<int>("z", PLATINUM_OFFSETOF(CustomStructure, z));

	PlatinumEngine::IDSystem idSystem;
	*idSystem.Add<int>().DeRef() = 69;
	*idSystem.Add<int>().DeRef() = 420;
	*idSystem.Add<float>().DeRef() = 3.14f;
	*idSystem.Add<double>().DeRef() = 3.1414f;
	*idSystem.Add<CustomStructure>().DeRef() = CustomStructure{ 1,1,2 };

	std::ostringstream outputString;
//	std::cout << "Original" << std::endl;
//	typeDatabase.Serialize(std::cout, &idSystem);
	typeDatabase.Serialize(outputString, &idSystem);

	PlatinumEngine::IDSystem idSystem2;
	std::istringstream inputString(outputString.str());
	typeDatabase.Deserialize(inputString, &idSystem2);
//	std::cout << "ReSerialized" << std::endl;
//	typeDatabase.Serialize(std::cout, &idSystem2);

	CHECK(inputString.str() == outputString.str());
}