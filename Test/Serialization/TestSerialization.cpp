#include <catch2/catch.hpp>
#include <Serialization/Serialization.h>
#include <sstream>

using namespace PlatinumEngine;

TEST_CASE("Serialization", "[serialization]")
{
	TypeDatabase database;

	// Plain Old Data plainData
//	std::cout << "== Plain Old Data ==" << std::endl;

	struct PlainStructure
	{
		float field0;
		float field1;
		std::vector<double> someValues;
	};

	database.BeginTypeInfo<PlainStructure>()
			.WithField<float>("field0", PLATINUM_OFFSETOF(PlainStructure, field0))
			.WithField<float>("field1", PLATINUM_OFFSETOF(PlainStructure, field1))
			.WithField<std::vector<double>>("someValues", PLATINUM_OFFSETOF(PlainStructure, someValues));

	database.CreateVectorTypeInfo<int>();
	database.CreateVectorTypeInfo<double>();

	database.CreatePrimitiveTypeInfo<double>();
	database.CreatePrimitiveTypeInfo<float>();
	database.CreatePrimitiveTypeInfo<int>();

//	database.OutputTypeInfo<PlainStructure>(std::cout);
//	database.OutputTypeInfo<std::vector<int>>(std::cout);

	PlainStructure plainData{
			69.0f, 420.0f, { 1, 1, 2, 3, 5, 8, 13 }
	};
//	database.Serialize(std::cout, &plainData);
	std::vector<int> listData{ 0, 1, 2, 3 };
//	database.Serialize(std::cout, &listData);

//	std::cout << "== Inherited Data ==" << std::endl;

	struct UnknownType
	{
	};

	class AbstractClass
	{
	public:
		float time;
		PlainStructure data;
		UnknownType unknownData;

		virtual void Get() = 0;
	};

	class ChildClass : public AbstractClass
	{
	public:
		int counter;

		void Get()
		{
		};
	};

	class FurtherChildClass : public ChildClass
	{
	public:
		int reset;

		void Get()
		{
		}
	};

	database.BeginAbstractTypeInfo<AbstractClass>()
			.WithField<float>("time", PLATINUM_OFFSETOF(AbstractClass, time))
			.WithField<PlainStructure>("data", PLATINUM_OFFSETOF(AbstractClass, data))
			.WithField<UnknownType>("unknownData", PLATINUM_OFFSETOF(AbstractClass, unknownData));
	database.BeginTypeInfo<ChildClass>()
			.WithInherit<AbstractClass>()
			.WithField<int>("counter", PLATINUM_OFFSETOF(ChildClass, counter));
	database.BeginTypeInfo<FurtherChildClass>()
			.WithInherit<ChildClass>()
			.WithField<int>("reset", PLATINUM_OFFSETOF(FurtherChildClass, reset));

//	database.OutputTypeInfo<AbstractClass>(std::cout);
//	database.OutputTypeInfo<ChildClass>(std::cout);
//	database.OutputTypeInfo<FurtherChildClass>(std::cout);
//	database.OutputTypeInfo<UnknownType>(std::cout);

	ChildClass childObject = {};
	childObject.data.field0 = 256;
	childObject.data.field1 = 1024;
	childObject.time = 69.88f;
	childObject.counter = 420;

	FurtherChildClass furtherChildObject = {};
	furtherChildObject.data.field0 = 256;
	furtherChildObject.data.field1 = 1024;
	furtherChildObject.time = 69.88f;
	furtherChildObject.counter = 420;
	furtherChildObject.reset = 920;

//	database.Serialize(std::cout, &childObject);
//	database.Serialize(std::cout, &furtherChildObject);
	// polymorphism test
//	database.Serialize(std::cout, (ChildClass*)&furtherChildObject);

//	std::cout << "== deserialize ==" << std::endl;
	{
		std::ostringstream serializedData;
		database.Serialize(serializedData, &plainData);
//		std::cout << "== Original plain data ==" << std::endl;
//		database.Serialize(std::cout, &plainData);

		PlainStructure deserializeData;
		std::istringstream inputSerializedData(serializedData.str());
		database.Deserialize(inputSerializedData, &deserializeData);

		std::ostringstream reSerializedData;
		database.Serialize(reSerializedData, &plainData);
//		std::cout << "== ReSerialized plain data ==" << std::endl;
//		database.Serialize(std::cout, &reSerializedData);

		CHECK(serializedData.str() == reSerializedData.str());
	}

	//	std::cout << "== inherited deserialization ==" << std::endl;
	{
		std::ostringstream serializedData;
		database.Serialize(serializedData, &plainData);
//		std::cout << "== Original plain data ==" << std::endl;
//		database.Serialize(std::cout, &plainData);

		FurtherChildClass deserializeData;
		std::istringstream inputSerializedData(serializedData.str());
		database.Deserialize(inputSerializedData, &deserializeData);

		std::ostringstream reSerializedData;
		database.Serialize(reSerializedData, &plainData);
//		std::cout << "== ReSerialized plain data ==" << std::endl;
//		database.Serialize(std::cout, &reSerializedData);

		CHECK(serializedData.str() == reSerializedData.str());
	}
}