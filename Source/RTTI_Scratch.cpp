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

//-----------------------------------------------------------------------
// Utility
//-----------------------------------------------------------------------

template<typename T>
constexpr auto type_name()
{
	std::string_view name, prefix, suffix;
#ifdef __clang__
	name = __PRETTY_FUNCTION__;
  prefix = "auto type_name() [T = ";
  suffix = "]";
#elif defined(__GNUC__)
	name = __PRETTY_FUNCTION__;
	prefix = "constexpr auto type_name() [with T = ";
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

class DynamicFieldIterator
{
public:

	class DynamicFieldInfo
	{
	public:
		std::type_index typeIndex;
		void* typeInstance;
	};

	// allow inherited classes to be destroyed
	virtual ~DynamicFieldIterator()
	{
	}

	// interface
	virtual bool IsIterating() = 0;

	virtual void MoveNext() = 0;

	virtual DynamicFieldInfo GetCurrent() = 0;
};

class TypeInfo
{
public:

	// should be unique?
	// with its template arguments
	std::string typeName;
	std::type_index typeIndex;
	std::optional<std::type_index> inheritedType;

	std::vector<FieldInfo> fields;

	// can be elements in an array type
	// but not in the fields!
	std::function<
			std::unique_ptr<DynamicFieldIterator>(void* typeInstance)>
			createDynamicFieldIterator;

	std::function<
			std::ostream&(std::ostream& outputStream, void* typeInstance)>
			streamOut;

	TypeInfo(std::type_index newTypeIndex) : typeIndex(newTypeIndex)
	{
	}

	TypeInfo(TypeInfo&) = default;

	TypeInfo(TypeInfo&&) = default;

	TypeInfo& operator=(TypeInfo&) = default;

	TypeInfo& operator=(TypeInfo&&) = default;

	//-----------------------------------------------------------------------
	// Factory pattern thingy
	//-----------------------------------------------------------------------

	template<typename T>
	TypeInfo& WithInherit()
	{
		// only 1 inherited type allowed
		assert(!inheritedType.has_value());
		inheritedType = std::type_index(typeid(T));
		return *this;
	}

	template<typename T>
	TypeInfo& WithField(std::string fieldName, size_t offset)
	{
		fields.push_back(FieldInfo{ std::type_index(typeid(T)), std::move(fieldName), offset });
		return *this;
	}
};

//-----------------------------------------------------------------------
// Vector support
//-----------------------------------------------------------------------

template<typename T>
class VectorDynamicFieldIterator : public DynamicFieldIterator
{
public:

	VectorDynamicFieldIterator(const std::vector<T>* toIterateOver) :
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

	DynamicFieldInfo GetCurrent() override
	{
		return { std::type_index(typeid(T)), (void*)&_toIterateOver->at(_currentIndex) };
	}

private:

	const std::vector<T>* _toIterateOver;
	size_t _currentIndex;
};

class TypeDatabase
{
public:

	template<typename T>
	TypeInfo& BeginTypeInfo()
	{
		std::string typeName = std::string(type_name<T>());
		std::type_index typeIndex = std::type_index(typeid(T));
		size_t countTypeNames = typeNames.count(typeName);
		size_t countTypeIndices = typeIndices.count(typeIndex);
		if (0 == countTypeNames && 0 == countTypeIndices)
		{
			typeNames[typeName] = typeIndices[typeIndex] = typeInfos.size();
			typeInfos.emplace_back(typeIndex);
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
	void CreateVectorTypeInfo()
	{
		BeginTypeInfo<std::vector<T>>().createDynamicFieldIterator =
				[](void* typeInstance) -> std::unique_ptr<DynamicFieldIterator>
				{
					return std::make_unique<VectorDynamicFieldIterator<T>>
							((const std::vector<T>*)typeInstance);
				};
	}

	template<typename T>
	void CreatePrimitiveTypeInfo()
	{
		static_assert(std::is_fundamental<T>::value);
		BeginTypeInfo<T>().streamOut =
				[](std::ostream& outputStream, void* typeInstance) -> std::ostream&
				{
					outputStream << *(T*)typeInstance;
					return outputStream;
				};
	}

	template<typename T>
	std::ostream& StreamOutTypeInstance(std::ostream& ostream, T* typeInstance)
	{
		ostream << '{' << std::endl;
		StreamOutTypeInstanceInternal(
				ostream,
				typeInstance,
				std::type_index(typeid(*typeInstance)),
				1);
		ostream << '}' << std::endl;
		return ostream;
	}

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

	template<typename T>
	const char* GetTypeIndexName()
	{
		return GetTypeIndexName(std::type_index(typeid(T)));
	}

	const char* GetTypeIndexName(const std::type_index& typeIndex)
	{
		auto[success, typeInfo] = GetTypeInfo(typeIndex);
		if (success)
			return typeInfo->typeName.c_str();
		else
			return typeIndex.name();
	}

	template<typename T>
	std::ostream& StreamOutTypeInfo(std::ostream& ostream)
	{
		return StreamOutTypeInfo(ostream, std::type_index(typeid(T)));
	}

	std::ostream& StreamOutTypeInfo(std::ostream& ostream, const std::type_index& typeIndex)
	{
		ostream << '{' << std::endl;
		StreamOutTypeInfoInternal(ostream, typeIndex, 1);
		ostream << '}' << std::endl;
		return ostream;
	}

private:

	std::vector<TypeInfo> typeInfos;
	std::map<std::string, size_t> typeNames;
	std::map<std::type_index, size_t> typeIndices;

	std::ostream& StreamOutTypeInfoInternal(
			std::ostream& ostream,
			const std::type_index& typeIndex,
			unsigned int indents)
	{
		std::string tabs(indents, '\t');
		auto[success, typeInfo] = GetTypeInfo(typeIndex);
		if (success)
		{
			if (typeInfo->inheritedType.has_value())
			{
				StreamOutTypeInfoInternal(ostream, *typeInfo->inheritedType, indents);
			}

			for (auto& field: typeInfo->fields)
			{
				ostream << tabs <<
						GetTypeIndexName(field.typeIndex) << ' ' <<
						field.fieldName << ' ' <<
						field.offset << ';' << std::endl;
			}
		}
		else
		{
			ostream << tabs << '?' << std::endl;
		}
		return ostream;
	}

	std::ostream& StreamOutTypeInstanceInternal(
			std::ostream& ostream,
			void* typeInstance,
			const std::type_index& typeIndex,
			unsigned int indents)
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

				if (typeInfo->inheritedType.has_value())
				{
					ostream << "what?" << std::endl;
					StreamOutTypeInstanceInternal(
							ostream,
							typeInstance,
							*typeInfo->inheritedType,
							indents);
				}

				for (auto& field: typeInfo->fields)
				{
					ostream << tabs << '\t' << GetTypeIndexName(field.typeIndex) << ' ' << field.fieldName << " = ";
					StreamOutTypeInstanceInternal(
							ostream,
							field.AccessField(typeInstance),
							field.typeIndex,
							0);
					ostream << ';' << std::endl;
				}

				if (typeInfo->createDynamicFieldIterator)
				{
					for (auto iterator = typeInfo->createDynamicFieldIterator(typeInstance);
						 iterator->IsIterating(); iterator->MoveNext())
					{
						auto dynamicField = iterator->GetCurrent();
						StreamOutTypeInstanceInternal(
								ostream,
								dynamicField.typeInstance,
								dynamicField.typeIndex,
								indents + 1);
						ostream << ',' << std::endl;
					}
				}
			}
		}
		else
		{
			ostream << '?';
		}
		return ostream;
	}
};


int main()
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

	db.StreamOutTypeInfo<MyStruct>(std::cout);
	db.StreamOutTypeInfo<std::vector<int>>(std::cout);

	MyStruct test{
			69.0f, 420.0f, { 1, 1, 2, 3, 5, 8, 13 }
	};
	db.StreamOutTypeInstance(std::cout, &test);
	std::vector<int> myList{ 0, 1, 2, 3 };
	db.StreamOutTypeInstance(std::cout, &myList);

	std::cout << "=======================================" << std::endl;

	class AbstractClass
	{
	public:
		float time;
		MyStruct data;

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

	db.BeginTypeInfo<AbstractClass>()
			.WithField<float>("time", PLATINUM_OFFSETOF(AbstractClass, time))
			.WithField<MyStruct>("data", PLATINUM_OFFSETOF(AbstractClass, data));
	db.BeginTypeInfo<ChildClass>()
			.WithInherit<AbstractClass>()
			.WithField<int>("counter", PLATINUM_OFFSETOF(ChildClass, counter));
	db.BeginTypeInfo<FurtherChildClass>()
			.WithInherit<ChildClass>()
			.WithField<int>("reset", PLATINUM_OFFSETOF(FurtherChildClass, reset));

	db.StreamOutTypeInfo<AbstractClass>(std::cout);
	db.StreamOutTypeInfo<ChildClass>(std::cout);
	db.StreamOutTypeInfo<FurtherChildClass>(std::cout);
	struct Anon{ };
	db.StreamOutTypeInfo<Anon>(std::cout);

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

//	db.StreamOutTypeInstance(std::cout, &what);
//	db.StreamOutTypeInstance(std::cout, &what2);
//
//	db.StreamOutTypeInstance(std::cout, (ChildClass*)&what2);

	// TODO implement properly and see what needs extending

	return 0;
}
