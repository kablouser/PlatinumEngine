#include <string_view>

template <typename T>
constexpr auto type_name() {
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

#include <vector>
#include <iostream>
#include <functional>

int main()
{
	const volatile int x = 0;
	std::cout << type_name<decltype(x)>() << std::endl;
	std::cout << sizeof(std::function<void(void)>) << std::endl;
}


#include <string>
#include <vector>
#include <typeindex>

class TypeInfo;

class FieldInfo
{
public:
	TypeInfo* typeInfo;
	std::string fieldName;

	void* AccessGeneric(void* instance);
	int& AccessInt(void* instance);
	double& AccessDouble(void* instance);
};

class TypeInfo
{
public:
	std::string name; // not unique, but "cross-platform"
	std::type_index index; // unique for each type

	std::vector<FieldInfo> fields;

	void Serialize();
	void Deserialize();
};
