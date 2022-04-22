#include <string_view>
#include <utility>
#include <vector>
#include <map>
#include <optional>
#include <typeindex>
#include <iostream>
#include <functional>
#include <cassert>
#include <memory>
#include <sstream>
#include <Helpers/VectorHelpers.h>

//-----------------------------------------------------------------------
// Utility
//-----------------------------------------------------------------------

template<typename T>
constexpr auto TypeName()
{
	std::string_view name, prefix, suffix;
#ifdef __clang__
	name = __PRETTY_FUNCTION__;
  prefix = "auto type_name() [T = ";
  suffix = "]";
#elif defined(__GNUC__)
	name = __PRETTY_FUNCTION__;
	prefix = "constexpr auto TypeName() [with T = ";
	suffix = "]";
#elif defined(_MSC_VER)
	name = __FUNCSIG__;
  prefix = "auto __cdecl type_name<";
  suffix = ">(void)";
#endif
	name.remove_prefix(prefix.size());
	name.remove_suffix(suffix.size());
	return name;
}

// Microsoft implementation
#define PLATINUM_OFFSETOF(typeName, fieldName)\
    (size_t)&reinterpret_cast<const volatile char&>((((typeName*)0)->fieldName))

//-----------------------------------------------------------------------
// Collection interface
//-----------------------------------------------------------------------

class Iterator
{
public:

	// allow inherited classes to be destroyed
	virtual ~Iterator() = default;

	// interface
	virtual bool IsIterating() = 0;

	virtual void MoveNext() = 0;

	virtual void* GetCurrent() = 0;
};

class Collection
{
public:
	// this is the type of the elements inside this collection
	// not the type of the collection
	std::type_index elementTypeIndex;

	explicit Collection(const std::type_index& newElementTypeIndex) : elementTypeIndex(newElementTypeIndex)
	{
	}

	virtual bool Add(void* collectionInstance, void* elementInstance) = 0;

	virtual bool RemoveFirst(void* collectionInstance, void* elementInstance) = 0;

	virtual std::unique_ptr<Iterator> GetIterator(const void* collectionInstance) = 0;
};

//-----------------------------------------------------------------------
// Vector support
//-----------------------------------------------------------------------

template<typename T>
class VectorIterator : public Iterator
{
public:

	explicit VectorIterator(const std::vector<T>* toIterateOver) :
			_toIterateOver(toIterateOver),
			_currentIndex(0)
	{
	}

	bool IsIterating() override
	{
		return _currentIndex < _toIterateOver->size();
	}

	void MoveNext() override
	{
		++_currentIndex;
	}

	void* GetCurrent() override
	{
		return (void*)&_toIterateOver->at(_currentIndex);
	}

private:

	const std::vector<T>* _toIterateOver;
	size_t _currentIndex;
};

template<typename T>
class VectorCollection : public Collection
{
public:
	VectorCollection() : Collection(std::type_index(typeid(T)))
	{
	}

	bool Add(void* collectionInstance, void* elementInstance) override
	{
		((std::vector<T>*)collectionInstance)->push_back(*(T*)elementInstance);
		return true;
	}

	bool RemoveFirst(void* collectionInstance, void* elementInstance) override
	{
		return PlatinumEngine::VectorHelpers::RemoveFirst(
				*(std::vector<T>*)collectionInstance,
				*(T*)elementInstance);
	}

	std::unique_ptr<Iterator> GetIterator(const void* collectionInstance) override
	{
		return std::make_unique<VectorIterator<T>>((const std::vector<T>*)collectionInstance);
	}
};

//-----------------------------------------------------------------------
// Core classes
//-----------------------------------------------------------------------

class FieldInfo
{
public:
	std::type_index typeIndex;

	std::string fieldName;
	size_t offset;

	void* AccessField(void* typeInstance) const
	{
		return (char*)typeInstance + offset;
	}
};

class TypeInfo
{
public:

	// should be unique. includes its template arguments
	std::string typeName;
	std::type_index typeIndex;
	// only 1 inherited parent allowed
	std::optional<std::type_index> inheritedType;

	// shared_ptr<void> is legal, but unique_ptr<void> is illegal
	std::function<std::shared_ptr<void>(void)> allocate;

	std::function<void(std::ostream& outputStream, void* typeInstance)>
			streamOut;
	std::function<void(std::istream& inputStream, void* typeInstance)>
			streamIn;

	// can either have fields or collection, not both
	bool isCollection;
	std::vector<FieldInfo> fields;
	std::unique_ptr<Collection> collection;

	TypeInfo(std::type_index newTypeIndex, bool isCollection) : typeIndex(newTypeIndex), isCollection(isCollection)
	{
	}

	TypeInfo(TypeInfo&) = delete;

	TypeInfo(TypeInfo&&) = default;

	TypeInfo& operator=(TypeInfo&) = delete;

	TypeInfo& operator=(TypeInfo&&) = default;

	//-----------------------------------------------------------------------
	// Factory pattern thingy
	//-----------------------------------------------------------------------

	template<typename T>
	TypeInfo& WithInherit()
	{
		// only 1 inherited type allowed
		assert(!inheritedType.has_value());
		// collections cannot inherit
		assert(!isCollection);
		inheritedType = std::type_index(typeid(T));
		return *this;
	}

	template<typename T>
	TypeInfo& WithField(std::string fieldName, size_t offset)
	{
		// fields cannot be in collections
		assert(!isCollection);
		fields.push_back(FieldInfo{ std::type_index(typeid(T)), std::move(fieldName), offset });
		return *this;
	}

	TypeInfo& WithCollection(std::unique_ptr<Collection>&& withCollection)
	{
		// isCollection must be enabled
		assert(isCollection);
		collection = std::move(withCollection);
		return *this;
	}
};

class TypeDatabase
{
public:

	//-----------------------------------------------------------------------
	// Creating Type Info
	//-----------------------------------------------------------------------


	template<typename T>
	TypeInfo& BeginAbstractTypeInfo(bool isCollection = false)
	{
		std::string typeName = std::string(TypeName<T>());
		auto typeIndex = std::type_index(typeid(T));
		size_t countTypeNames = typeNames.count(typeName);
		size_t countTypeIndices = typeIndices.count(typeIndex);
		if (0 == countTypeNames && 0 == countTypeIndices)
		{
			typeNames[typeName] = typeIndices[typeIndex] = typeInfos.size();
			typeInfos.emplace_back(typeIndex, isCollection);
			TypeInfo& typeInfo = typeInfos[typeInfos.size() - 1];
			typeInfo.typeName = std::move(typeName);
			return typeInfo;
		}
		else
		{
			// TODO replace with PLATINUM_WARNING
			std::cout << "TypeDatabase WARNING: type already exists in database " << typeName << std::endl;
			// typeName already exists in database, return the existing entry
			if (countTypeNames == 0)
				return typeInfos.at(typeNames[typeName]);
			else
				return typeInfos.at(typeIndices[typeIndex]);
		}
	}

	template<typename T>
	TypeInfo& BeginTypeInfo(bool isCollection = false)
	{
		static_assert(!std::is_abstract<T>(), "T should be NOT abstract.");
		TypeInfo& typeInfo = BeginAbstractTypeInfo<T>(isCollection);
		typeInfo.allocate = []() -> std::shared_ptr<void>
		{ return std::make_shared<T>(); };
		return typeInfo;
	}

	template<typename T>
	void CreateVectorTypeInfo()
	{
		BeginTypeInfo<std::vector<T>>(true)
				.WithCollection(std::make_unique<VectorCollection<T>>());
	}

	template<typename T>
	void CreatePrimitiveTypeInfo()
	{
		static_assert(std::is_fundamental<T>::value, "T should be primitive aka fundamental.");
		TypeInfo& typeInfo = BeginTypeInfo<T>();
		typeInfo.streamOut =
				[](std::ostream& outputStream, void* typeInstance) -> void
				{
					outputStream << *(T*)typeInstance;
				};
		typeInfo.streamIn =
				[](std::istream& inputStream, void* typeInstance) -> void
				{
					inputStream >> *(T*)typeInstance;
				};
	}

	//-----------------------------------------------------------------------
	// Get Type Info
	//-----------------------------------------------------------------------

	std::pair<bool, const TypeInfo*> GetTypeInfo(const std::string& typeName)
	{
		auto iterator = typeNames.find(typeName);
		if (iterator == typeNames.end())
			return { false, nullptr };
		else
			return { true, &typeInfos.at(iterator->second) };
	}

	template<typename T>
	std::pair<bool, const TypeInfo*> GetTypeInfo()
	{
		return GetTypeInfo(std::type_index(typeid(T)));
	}

	std::pair<bool, const TypeInfo*> GetTypeInfo(const std::type_index& typeIndex)
	{
		auto iterator = typeIndices.find(typeIndex);
		if (iterator == typeIndices.end())
			return { false, nullptr };
		else
			return { true, &typeInfos.at(iterator->second) };
	}

	//-----------------------------------------------------------------------
	// Get Type Name
	//-----------------------------------------------------------------------

	const char* GetTypeName(const std::type_index& typeIndex)
	{
		static char unknownName[] = "?";
		auto[success, typeInfo] = GetTypeInfo(typeIndex);
		if (success)
			return typeInfo->typeName.c_str();
		else
			return unknownName;
	}

	template<typename T>
	const char* GetTypeName()
	{
		return GetTypeName(std::type_index(typeid(T)));
	}

	//-----------------------------------------------------------------------
	// Output Type Info
	//-----------------------------------------------------------------------

	void OutputTypeInfo(std::ostream& ostream, const std::type_index& typeIndex)
	{
		OutputTypeInfo(ostream, typeIndex, false);
		ostream << ';' << std::endl;
	}

	template<typename T>
	void OutputTypeInfo(std::ostream& ostream)
	{
		OutputTypeInfo(ostream, std::type_index(typeid(T)));
	}

	//-----------------------------------------------------------------------
	// Serialization
	//-----------------------------------------------------------------------

	template<typename T>
	void Serialize(std::ostream& ostream, T* typeInstance)
	{
		SerializeInternal(
				ostream,
				typeInstance,
				std::type_index(typeid(*typeInstance)),
				0,
				SerializeSection::root);
		ostream << ';' << std::endl;
	}

	enum class DeserializeReturnCode
	{
		success, badFormat, unknownType, missingAllocators, missingCollection
	};

	template<typename T>
	DeserializeReturnCode Deserialize(std::istream& istream, T* typeInstance)
	{
		return DeserializeInternal(
				istream,
				typeInstance,
				std::type_index(typeid(*typeInstance)),
				SerializeSection::root);
	}

private:

	std::vector<TypeInfo> typeInfos;
	std::map<std::string, size_t> typeNames;
	std::map<std::type_index, size_t> typeIndices;

	void OutputTypeInfo(
			std::ostream& ostream,
			const std::type_index& typeIndex,
			bool isInherited)
	{
		auto[success, typeInfo] = GetTypeInfo(typeIndex);
		if (success)
		{
			if (!isInherited)
				ostream << GetTypeName(typeIndex) << std::endl <<
						'{' << std::endl;

			if (typeInfo->inheritedType.has_value())
				OutputTypeInfo(ostream, *typeInfo->inheritedType, true);

			if (isInherited)
				ostream << '\t' << "// inherited from " << GetTypeName(typeIndex) << std::endl;
			else if (typeInfo->inheritedType.has_value())
				ostream << '\t' << "// from " << GetTypeName(typeIndex) << std::endl;

			for (auto& field: typeInfo->fields)
			{
				ostream << '\t' <<
						GetTypeName(field.typeIndex) << ' ' <<
						field.fieldName << ' ' <<
						field.offset << ';' << std::endl;
			}

			if (!isInherited)
				ostream << '}';
		}
		else
		{
			if (isInherited)
				ostream << '\t';
			ostream << '?';
		}
	}

	enum class SerializeSection
	{
		root, inherited, field, collectionElement
	};

	void SerializeInternal(
			std::ostream& ostream,
			void* typeInstance,
			const std::type_index& typeIndex,
			unsigned int indents,
			SerializeSection section)
	{
		auto[success, typeInfo] = GetTypeInfo(typeIndex);
		if (success)
		{
			if (typeInfo->streamOut)
			{
				typeInfo->streamOut(ostream, typeInstance);
			}
			else
			{
				std::string tabs(indents, '\t');

				// { root, inherited, field, dynamicField };
				switch (section)
				{
				case SerializeSection::root:
					ostream << tabs << GetTypeName(typeIndex) << std::endl <<
							tabs << '{' << std::endl;
					break;
				case SerializeSection::field:
					ostream << std::endl << tabs << '{' << std::endl;
					break;
				case SerializeSection::collectionElement:
					ostream << tabs << '{' << std::endl;
					break;
				default:
					break;
				}

				if (typeInfo->inheritedType.has_value())
				{
					SerializeInternal(
							ostream,
							typeInstance,
							*typeInfo->inheritedType,
							indents,
							SerializeSection::inherited);
				}

				if (section == SerializeSection::inherited)
					ostream << tabs << '\t' << "// inherited from " << GetTypeName(typeIndex) << std::endl;
				else if (typeInfo->inheritedType.has_value())
					ostream << tabs << '\t' << "// from " << GetTypeName(typeIndex) << std::endl;

				if (typeInfo->isCollection)
				{
					if (typeInfo->collection)
					{
						for (auto iterator = typeInfo->collection->GetIterator(typeInstance);
							 iterator->IsIterating(); iterator->MoveNext())
						{
							auto currentElement = iterator->GetCurrent();
							ostream << tabs << '\t';
							SerializeInternal(
									ostream,
									currentElement,
									typeInfo->collection->elementTypeIndex,
									indents + 1,
									SerializeSection::collectionElement);
							ostream << ',' << std::endl;
						}
					}
				}
				else
				{
					for (auto& field: typeInfo->fields)
					{
						ostream << tabs << '\t' << GetTypeName(field.typeIndex) << ' ' << field.fieldName << " = ";
						SerializeInternal(
								ostream,
								field.AccessField(typeInstance),
								field.typeIndex,
								indents + 1,
								SerializeSection::field);
						ostream << ';' << std::endl;
					}
				}

				if (section != SerializeSection::inherited)
					ostream << tabs << '}';
			}
		}
		else
		{
			ostream << '?';
		}
	}

	DeserializeReturnCode DeserializeInternal(
			std::istream& istream,
			void* typeInstance,
			const std::type_index& typeIndex,
			SerializeSection section)
	{
		auto[success, typeInfo] = GetTypeInfo(typeIndex);
		if (!success)
			return DeserializeReturnCode::unknownType;

		if (typeInfo->streamIn)
		{
			typeInfo->streamIn(istream, typeInstance);
			return istream ? DeserializeReturnCode::success : DeserializeReturnCode::badFormat;
		}

		std::string stringToken;
		if (section == SerializeSection::root &&
			!(istream >> stringToken && stringToken == typeInfo->typeName))
			return DeserializeReturnCode::badFormat;

		if (section != SerializeSection::inherited &&
			!(istream >> stringToken && stringToken == "{"))
			return DeserializeReturnCode::badFormat;

		while (true)
		{
			// save position from start of line
			auto beginPosition = istream.tellg();
			if (!(istream >> stringToken))
				return DeserializeReturnCode::badFormat;

			if (stringToken.rfind("//", 0) == 0 // stringToken begins with //, comment
				||
				stringToken == "?")// unknown type
			{
				std::getline(istream, stringToken); // skip line
				continue;
			}
			else if (stringToken.rfind("}", 0) == 0)
			{
				// stringToken begins with }, end of block
				return DeserializeReturnCode::success;
			}
			else if (stringToken == "{")
			{
				// structure to unknown type
				// skip brackets
				if (SkipCurlyBrackets(istream, 1))
					continue;
				else
					return DeserializeReturnCode::badFormat;
			}

			if (typeInfo->isCollection)
			{
				istream.seekg(beginPosition);

				if (!typeInfo->collection)
				{
					// no collection info
					if (SkipCurlyBrackets(istream, 1))// skip brackets
						return DeserializeReturnCode::missingCollection;
					else
						return DeserializeReturnCode::badFormat;
				}

				auto[foundElement, elementTypeInfo]= GetTypeInfo(typeInfo->collection->elementTypeIndex);
				if (!foundElement)
				{
					// no collection element info
					if (SkipCurlyBrackets(istream, 1))// skip brackets
						return DeserializeReturnCode::missingCollection;
					else
						return DeserializeReturnCode::badFormat;
				}

				if (!(elementTypeInfo->allocate))
				{
					// no element allocators
					if (SkipCurlyBrackets(istream, 1))// skip brackets
						return DeserializeReturnCode::missingAllocators;
					else
						return DeserializeReturnCode::badFormat;
				}

				std::shared_ptr<void> collectionElement = elementTypeInfo->allocate();

				// recurse
				auto returnCode = DeserializeInternal(
						istream,
						collectionElement.get(),
						elementTypeInfo->typeIndex,
						SerializeSection::collectionElement);

				if (returnCode == DeserializeReturnCode::success)
					typeInfo->collection->Add(typeInstance, collectionElement.get());

				// probably comma next
				std::getline(istream, stringToken); // skip line
			}
			else
			{
				auto[fieldSuccess, fieldTypeInfo]=GetTypeInfo(stringToken);
				if (!fieldSuccess)
				{
					// unknown type
					std::getline(istream, stringToken); // skip line
					continue;
				}

				if (!(istream >> stringToken))
				{
					// bad format
					std::getline(istream, stringToken); // skip line
					continue;
				}

				// find fieldName in our type with inheritance
				auto[foundFieldName, fieldInfo] = FindFieldName(*typeInfo, stringToken);
				if (!foundFieldName)
				{
					// mismatch, no fieldName
					std::getline(istream, stringToken); // skip line
					continue;
				}

				if (fieldTypeInfo->typeIndex != fieldInfo->typeIndex)
				{
					// mismatch, field type is unexpected
					std::getline(istream, stringToken); // skip line
					continue;
				}

				if (!(istream >> stringToken && stringToken == "="))
				{
					// expected '='
					std::getline(istream, stringToken); // skip line
					continue;
				}

				// recurse
				auto returnCode = DeserializeInternal(
						istream,
						fieldInfo->AccessField(typeInstance),
						fieldInfo->typeIndex,
						SerializeSection::field);
				// probably semicolon next
				std::getline(istream, stringToken); // skip line
			}
		}
	}

	static bool SkipCurlyBrackets(std::istream& istream, unsigned int bracketsLevel)
	{
		while (0 < bracketsLevel && istream)
		{
			auto character = istream.get();
			if (character == '{')
				++bracketsLevel;
			else if (character == '}')
				--bracketsLevel;
		}

		return (bool)istream;
	}

	std::pair<bool, const FieldInfo*> FindFieldName(const TypeInfo& typeInfo, const std::string& fieldName)
	{
		for (auto& field: typeInfo.fields)
		{
			if (field.fieldName == fieldName)
				return { true, &field };
		}

		if (typeInfo.inheritedType.has_value())
		{
			auto[success, inheritedTypeInfo]= GetTypeInfo(*typeInfo.inheritedType);
			if (success)
				// recurse
				return FindFieldName(*inheritedTypeInfo, fieldName);
		}

		return { false, nullptr };
	}
};

int main()
{
	while (true)
	{
		struct MyStruct
		{
			float field0;
			float field1;
			std::vector<double> someValues;
		};

		TypeDatabase db;

		db.BeginTypeInfo<MyStruct>()
				.WithField<float>("field0", PLATINUM_OFFSETOF(MyStruct, field0))
				.WithField<float>("field1", PLATINUM_OFFSETOF(MyStruct, field1))
				.WithField<std::vector<double>>("someValues", PLATINUM_OFFSETOF(MyStruct, someValues));

		db.CreateVectorTypeInfo<int>();
		db.CreateVectorTypeInfo<double>();

		db.CreatePrimitiveTypeInfo<double>();
		db.CreatePrimitiveTypeInfo<float>();
		db.CreatePrimitiveTypeInfo<int>();

//		db.OutputTypeInfo<MyStruct>(std::cout);
//		db.OutputTypeInfo<std::vector<int>>(std::cout);

		MyStruct test{
				69.0f, 420.0f, { 1, 1, 2, 3, 5, 8, 13 }
		};
//		db.Serialize(std::cout, &test);
		std::vector<int> myList{ 0, 1, 2, 3 };
//		db.Serialize(std::cout, &myList);

//		std::cout << "=======================================" << std::endl;

		struct UnknownType
		{
		};

		class AbstractClass
		{
		public:
			float time;
			MyStruct data;
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

		db.BeginAbstractTypeInfo<AbstractClass>()
				.WithField<float>("time", PLATINUM_OFFSETOF(AbstractClass, time))
				.WithField<MyStruct>("data", PLATINUM_OFFSETOF(AbstractClass, data))
				.WithField<UnknownType>("unknownData", PLATINUM_OFFSETOF(AbstractClass, unknownData));
		db.BeginTypeInfo<ChildClass>()
				.WithInherit<AbstractClass>()
				.WithField<int>("counter", PLATINUM_OFFSETOF(ChildClass, counter));
		db.BeginTypeInfo<FurtherChildClass>()
				.WithInherit<ChildClass>()
				.WithField<int>("reset", PLATINUM_OFFSETOF(FurtherChildClass, reset));

//		db.OutputTypeInfo<AbstractClass>(std::cout);
//		db.OutputTypeInfo<ChildClass>(std::cout);
//		db.OutputTypeInfo<FurtherChildClass>(std::cout);
//		db.OutputTypeInfo<UnknownType>(std::cout);

		ChildClass what = {};
		what.data.field0 = 256;
		what.data.field1 = 1024;
		what.time = 69.88f;
		what.counter = 420;

		FurtherChildClass what2 = {};
		what2.data.field0 = 256;
		what2.data.field1 = 1024;
		what2.time = 69.88f;
		what2.counter = 420;
		what2.reset = 920;

//		db.Serialize(std::cout, &what);
//		db.Serialize(std::cout, &what2);

//		db.Serialize(std::cout, (ChildClass*)&what2);


//		std::cout << "DESERIALIZE >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>" << std::endl;
		{
			// TODO serialize stuff that is actually useful
			// TODO stream in
			std::ostringstream outputString;
			db.Serialize(outputString, &test);

			std::istringstream inputString(outputString.str());

			MyStruct readMyStruct;

			db.Deserialize(inputString, &readMyStruct);
//			std::cout << "original >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>" << std::endl;
//			db.Serialize(std::cout, &test);
//			std::cout << "scuffed >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>" << std::endl;
//			db.Serialize(std::cout, &readMyStruct);
		}

		// inherited deserialization
		{
			// TODO serialize stuff that is actually useful
			// TODO stream in
			std::ostringstream outputString;
			db.Serialize(outputString, &what2);

			std::istringstream inputString(outputString.str());

			FurtherChildClass readMyFurther;

			db.Deserialize(inputString, &readMyFurther);
//			std::cout << "original >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>" << std::endl;
//			db.Serialize(std::cout, &what2);
//			std::cout << "scuffed >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>" << std::endl;
//			db.Serialize(std::cout, &readMyFurther);
		}
	}

	return 0;
}
