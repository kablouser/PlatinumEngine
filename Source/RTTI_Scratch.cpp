#include <string_view>
#include <vector>
#include <iostream>
#include <functional>
#include <cassert>
#include <memory>

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

template<class... T>
std::vector<const char*> getTypeNames()
{
	return {typeid(T).name()...};
}

class DynamicFieldIterator
{
public:

	// not needed, but C++ is really inconsistent
	virtual ~DynamicFieldIterator()	{}

	// interface
  	virtual bool MoveNext() = 0;
	virtual void* GetCurrent() = 0;
	virtual size_t GetSize() = 0;
};

class VectorFieldIterator : public DynamicFieldIterator
{
public:

	VectorFieldIterator(const std::vector<float>& toIterateOver) :
			_toIterateOver(toIterateOver),
			_currentIndex(0)
	{
	}

	bool MoveNext() override
	{
		std::cout << "MoveNext" << std::endl;
		if (_currentIndex < _toIterateOver.size())
		{
			++_currentIndex;
			return true;
		}
		return false;
	}

	void* GetCurrent() override
	{
		return (void*)&_toIterateOver.at(_currentIndex);
	}

	size_t GetSize() override
	{
		return _toIterateOver.size();
	}

private:

	const std::vector<float>& _toIterateOver;
	size_t _currentIndex;
};

class TypeInfo;

class FieldInfo
{
public:
	std::string typeName;
	std::string fieldName;
	size_t offset;

	void* AccessField(void* typeInstance)
	{
		return (char*)typeInstance + offset;
	}
};

class TypeInfo
{
public:
	std::string name; // should be unique?

	bool isTemplate;
	std::string templateArgument0;

	std::vector<FieldInfo> fields;
	// can be elements in an array type
	// but not in the fields!
	std::function<std::shared_ptr<DynamicFieldIterator>(void* typeInstance)> getDynamicFieldIterator;

	void Serialize();

	void Deserialize();

	void* AccessGeneric(void* typeInstance);

	int& AccessInt(void* typeInstance);

	double& AccessDouble(void* typeInstance);
};

int main()
{
	const volatile static int x = 0;
	std::cout << "type name = " << type_name<decltype(x)>() << std::endl;

	float myFloat;
	TypeInfo floatTypeInfo{
			"float",
			false,
			{},
			{}
	};

	struct MyStruct
	{
		float field0;
		float field1;
	};
	TypeInfo myStructTypeInfo{
		"MyStruct",
		false,
		{},
		{
				{"float", "field0", offsetof(MyStruct, field0)},
				{"float", "field1", offsetof(MyStruct, field1)}
		}
	};

	{
		MyStruct myStructInstance;
		auto field0TrueOff = (char*)&myStructInstance.field0 - (char*)&myStructInstance;
		auto field1TrueOff = (char*)&myStructInstance.field1 - (char*)&myStructInstance;
		assert(field0TrueOff == myStructTypeInfo.fields[0].offset && "field0 offset test");
		assert(field1TrueOff == myStructTypeInfo.fields[1].offset && "field1 offset test");
	}

	class BaseClass
	{
	public:
		float baseField0;

		virtual void Method()
		{

		};
	};

	class ChildClass : public BaseClass
	{
	public:
		float childField0;

		void Method() override
		{

		}
	};

	ChildClass *dangerousPointer = 0;
	TypeInfo myChildClassTypeInfo{
			"ChildClass",
			false,
			{},
			{
					{"float", "baseField0", (size_t)&dangerousPointer->baseField0 },
					{"float", "childField0", (size_t)&dangerousPointer->childField0 }
			}
	};

	{
		ChildClass myChildClassInstance;
		auto field0TrueOff = (char*)&myChildClassInstance.baseField0 - (char*)&myChildClassInstance;
		auto field1TrueOff = (char*)&myChildClassInstance.childField0 - (char*)&myChildClassInstance;
		assert(field0TrueOff == myChildClassTypeInfo.fields[0].offset && "base field0 offset test");
		assert(field1TrueOff == myChildClassTypeInfo.fields[1].offset && "child field0 offset test");

		std::cout << "field0 true off " << field0TrueOff << std::endl;
		std::cout << "field1 true off " << field1TrueOff << std::endl;
	}

	std::vector<float> myFloats {0,1,2};
	std::shared_ptr<DynamicFieldIterator> iterator = std::make_shared<VectorFieldIterator>(myFloats);



	decltype(iterator);

	return 0;
}
