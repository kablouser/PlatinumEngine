#include <string_view>
#include <vector>
#include <map>
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
	// POD type support
	//-----------------------------------------------------------------------

	template<typename T>
	TypeInfo& WithField(std::string fieldName, size_t offset)
	{
		fields.push_back(FieldInfo{ std::type_index(typeid(T)), fieldName, offset });
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
		//std::ostream&(std::ostream& outputStream, void* typeInstance)>
		BeginTypeInfo<T>().streamOut =
				[](std::ostream& outputStream, void* typeInstance) -> std::ostream&
				{
					outputStream << *(T*)typeInstance;
					return outputStream;
				};
	}

	std::pair<bool, const TypeInfo*> TryGetTypeInfo(const std::string& typeName)
	{
		auto iterator = typeNames.find(typeName);
		if (iterator == typeNames.end())
			return { false, nullptr };
		else
			return { true, &typeInfos.at(iterator->second) };
	}

	std::pair<bool, const TypeInfo*> TryGetTypeInfo(std::type_index typeIndex)
	{
		auto iterator = typeIndices.find(typeIndex);
		if (iterator == typeIndices.end())
			return { false, nullptr };
		else
			return { true, &typeInfos.at(iterator->second) };
	}

	template<typename T>
	std::pair<bool, const TypeInfo*> TryGetTypeInfo()
	{
		return TryGetTypeInfo(std::type_index(typeid(T)));
	}

	const char* GetTypeIndexName(std::type_index typeIndex)
	{
		auto[success, typeInfo] = TryGetTypeInfo(typeIndex);
		if (success)
			return typeInfo->typeName.c_str();
		else
			return typeIndex.name();
	}

	std::ostream& StreamOutTypeInfo(std::ostream& ostream, const TypeInfo& typeInfo)
	{
		ostream << typeInfo.typeName << std::endl
				<< '{' << std::endl;

		for (auto& i: typeInfo.fields)
		{
			ostream << '\t' <<
					GetTypeIndexName(i.typeIndex) << ' ' <<
					i.fieldName << ' ' <<
					i.offset << ';' << std::endl;
		}

		ostream << '}' << std::endl;

		return ostream;
	}

	template<typename T>
	std::ostream& StreamOutTypeInstance(std::ostream& ostream, T* typeInstance)
	{
		StreamOutTypeInstance(ostream, typeInstance, std::type_index(typeid(*typeInstance)), 0);
		ostream << ';' << std::endl;
		return ostream;
	}

private:

	std::vector<TypeInfo> typeInfos;
	std::map<std::string, size_t> typeNames;
	std::map<std::type_index, size_t> typeIndices;

	std::ostream& StreamOutTypeInstance(
			std::ostream& ostream,
			void* typeInstance,
			std::type_index typeIndex,
			unsigned int indents)
	{
		auto[success, typeInfo] = TryGetTypeInfo(typeIndex);
		if (success)
		{
			if (typeInfo->streamOut)
			{
				typeInfo->streamOut(ostream, typeInstance);
			}
			else
			{
				if (0 < indents)
					ostream << std::endl;
				std::string tabs(indents, '\t');

				ostream << tabs << '{' << std::endl;

				for (auto& field: typeInfo->fields)
				{
					ostream << tabs << '\t' << GetTypeIndexName(field.typeIndex) << ' ' << field.fieldName << " = ";
					StreamOutTypeInstance(ostream, field.AccessField(typeInstance), field.typeIndex, indents + 1);
					ostream << ';' << std::endl;
				}

				if (typeInfo->createDynamicFieldIterator)
				{
					for (auto iterator = typeInfo->createDynamicFieldIterator(typeInstance);
						 iterator->IsIterating(); iterator->MoveNext())
					{
						auto dynamicField = iterator->GetCurrent();
						ostream << tabs << '\t';
						StreamOutTypeInstance(ostream, dynamicField.typeInstance, dynamicField.typeIndex, indents + 1);
						ostream << ',' << std::endl;
					}
				}

				ostream << tabs << '}';
			}
		}
		else
		{
			ostream << "?";
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

	db.StreamOutTypeInfo(std::cout, *db.TryGetTypeInfo<MyStruct>().second);
	db.StreamOutTypeInfo(std::cout, *db.TryGetTypeInfo<std::vector<int>>().second);

	MyStruct test{
			69.0f, 420.0f, {1,1,2,3,5,8,13}
	};
	db.StreamOutTypeInstance(std::cout, &test);

	std::vector<int> myList{ 0, 1, 2, 3 };
	db.StreamOutTypeInstance(std::cout, &myList);

	return 0;
}
