#pragma once

#include <vector>
#include <map>

#include <string_view> // TypeName()

#include <utility> // std::pair
#include <optional> // std::optional
#include <typeindex> // std::type_index
#include <iostream> // std::ostream, std::istream
#include <functional> // std::function

#include <cassert> // assert()
#include <memory> // std::unique_ptr
#include <tuple> // std::get magic

#include <Helpers/VectorHelpers.h>
#include <TypeDatabase/VectorCollection.h>
#include <IDSystem/IDSystem.h>

namespace PlatinumEngine
{
	//-----------------------------------------------------------------------
	// Utility
	//-----------------------------------------------------------------------

	// Microsoft's implementation for `offsetof`
#define PLATINUM_OFFSETOF(typeName, fieldName)\
        (size_t)&reinterpret_cast<const volatile char&>((((typeName*)0)->fieldName))

	template<typename T>
	constexpr auto TypeName()
	{
		std::string_view name, prefix, suffix;
#ifdef __clang__
		name = __PRETTY_FUNCTION__;
		prefix = "auto PlatinumEngine::TypeName() [T = ";
		suffix = "]";
#elif defined(__GNUC__)
		name = __PRETTY_FUNCTION__;
		prefix = "constexpr auto PlatinumEngine::TypeName() [with T = ";
		suffix = "]";
#elif defined(_MSC_VER)
		name = __FUNCSIG__;
		prefix = "auto __cdecl PlatinumEngine::TypeName<";
		suffix = ">(void)";
#endif
		auto x = name.data();
		name.remove_prefix(prefix.size());
		name.remove_suffix(suffix.size());
		return name;
	}

	template<typename Test, template<typename...> class Ref>
	struct is_specialization : std::false_type
	{
	};

	template<template<typename...> class Ref, typename... Args>
	struct is_specialization<Ref<Args...>, Ref> : std::true_type
	{
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

		void* AccessField(void* typeInstance) const;
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

		TypeInfo(std::type_index newTypeIndex, bool isCollection);

		TypeInfo(TypeInfo&) = delete;

		TypeInfo& operator=(TypeInfo&) = delete;

		TypeInfo(TypeInfo&&) = default;

		TypeInfo& operator=(TypeInfo&&) = default;
	};

	// forward declare for TypeInfoFactory to use as reference
	class TypeDatabase;

	class TypeInfoFactory
	{
	public:
		// warning: you shouldn't need to access this index
		size_t typeInfoIndex;
		TypeDatabase& typeDatabase;

		TypeInfoFactory(size_t inTypeInfoIndex, TypeDatabase& inTypeDatabase);

		TypeInfo& GetTypeInfo();

		//-----------------------------------------------------------------------
		// Factory pattern thingy
		//-----------------------------------------------------------------------

		template<typename T>
		TypeInfoFactory& WithInherit()
		{
			return *WithInheritInternal(std::type_index(typeid(T)));
		}

		// magical template for user defined types
		template<typename T>
		TypeInfoFactory& WithField(std::string fieldName, size_t offset);

		TypeInfoFactory& WithCollection(std::unique_ptr<Collection>&& withCollection);

	private:

		TypeInfoFactory* WithInheritInternal(std::type_index&& typeIndex);

		TypeInfoFactory* WithFieldInternal(
				std::string& fieldName,
				size_t offset,
				std::type_index&& typeIndex);
	};

	class TypeDatabase
	{
	public:
		// let TypeInfoFactory access private data
		friend class TypeInfoFactory;

		// singleton instance
		static TypeDatabase* Instance;

		static void StreamOutString(std::ostream& outputStream, void* typeInstance);

		static void StreamInString(std::istream& inputStream, void* typeInstance);

		//-----------------------------------------------------------------------
		// Constructor
		//-----------------------------------------------------------------------

		TypeDatabase();

		~TypeDatabase();

		// note: defined in TypeDatabaseInit.cpp
		// because it will be very big
		void Init();

		//-----------------------------------------------------------------------
		// Creating Type Info
		//-----------------------------------------------------------------------

		template<typename T>
		TypeInfoFactory BeginTypeInfoWithoutAllocator(bool isCollection = false)
		{
			return BeginAbstractTypeInfoInternal(
					isCollection,
					std::string(TypeName<T>()),
					std::type_index(typeid(T)));
		}

		template<typename T>
		TypeInfoFactory BeginTypeInfo(bool isCollection = false)
		{
			static_assert(!std::is_abstract<T>(), "T should be NOT abstract.");
			TypeInfoFactory factory = BeginTypeInfoWithoutAllocator<T>(isCollection);
			// you cannot make_shared with abstract type
			factory.GetTypeInfo().allocate = []() -> std::shared_ptr<void>
			{ return std::make_shared<T>(); };
			return factory;
		}

		// magical template for arithmetic types
		template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0>
		bool CreateIfAutomatic(int)
		{
			// if type info NOT in database
			if (!GetTypeInfo<T>().first)
				CreateTypeInfoWithAutoStreams<T>();
			return true;
		}

		// magical template for std::vector<T> types
		template<typename T, std::enable_if_t<is_specialization<T, std::vector>::value, int> = 0>
		bool CreateIfAutomatic(int)
		{
			// if type info NOT in database
			if (!GetTypeInfo<T>().first)
			{
				// try to create the element type info. e.g: std::vector<double> would try to create double typeInfo
				CreateIfAutomatic < typename T::value_type > (0);
				BeginTypeInfo<std::vector<typename T::value_type>>(true)
						.WithCollection(std::make_unique<PlatinumEngine::VectorCollection<typename T::value_type>>());
			}
			return true;
		}

		// magical template for std::string types
		template<typename T, std::enable_if_t<std::is_same_v<T, std::string>, int> = 0>
		bool CreateIfAutomatic(int)
		{
			// if type info NOT in database
			if (!GetTypeInfo<T>().first)
			{
				TypeInfoFactory factory = BeginTypeInfo<T>();
				TypeInfo& typeInfo = factory.GetTypeInfo();
				typeInfo.streamOut = StreamOutString;
				typeInfo.streamIn = StreamInString;
				// full name std::__cxx11::basic_string<char>
				// too long, so let's give it an alias
				typeInfo.typeName = "std::string";
				_typeNames.insert({ "std::string", factory.typeInfoIndex });
			}
			return true;
		}

		// magical template for PlatinumEngine::SavedReference<T> types
		template<typename T, std::enable_if_t<is_specialization<T, PlatinumEngine::SavedReference>::value, int> = 0>
		bool CreateIfAutomatic(int)
		{
			// if type info NOT in database
			if (!GetTypeInfo<T>().first)
			{
				TypeInfoFactory factory = BeginTypeInfo<T>();
				TypeInfo& typeInfo = factory.GetTypeInfo();
				typeInfo.streamOut =
						[](std::ostream& outputStream, void* typeInstance) -> void
						{
							outputStream << ((T*)typeInstance)->id;
						};
				typeInfo.streamIn =
						[](std::istream& inputStream, void* typeInstance) -> void
						{
							inputStream >> ((T*)typeInstance)->id;
						};
			}
			return true;
		}

		// default magical template (for user defined types)
		// NOTE: MUST call with an int like this: CreateIfAutomatic(0);
		template<typename T>
		bool CreateIfAutomatic(...)
		{
			// default, user defined types cannot be automatically created
			return false;
		}

		template<typename T>
		void CreateTypeInfoWithAutoStreams()
		{
			TypeInfoFactory factory = BeginTypeInfo<T>();
			TypeInfo& typeInfo = factory.GetTypeInfo();
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

		bool FinalCheck();

		//-----------------------------------------------------------------------
		// Get Type Info
		//-----------------------------------------------------------------------

		std::pair<bool, const TypeInfo*> GetTypeInfo(const std::string& typeName);

		std::pair<bool, const TypeInfo*> GetTypeInfo(const std::type_index& typeIndex);

		template<typename T>
		std::pair<bool, const TypeInfo*> GetTypeInfo()
		{
			return GetTypeInfo(std::type_index(typeid(T)));
		}

		//-----------------------------------------------------------------------
		// Get Stored Type Name
		//-----------------------------------------------------------------------

		std::string GetStoredTypeName(const std::type_index& typeIndex);

		//-----------------------------------------------------------------------
		// Output Type Info
		//-----------------------------------------------------------------------

		void OutputTypeInfo(std::ostream& ostream, const std::type_index& typeIndex);

		template<typename T>
		void OutputTypeInfo(std::ostream& ostream)
		{
			OutputTypeInfo(ostream, std::type_index(typeid(T)));
		}

		//-----------------------------------------------------------------------
		// Serialization - nice wrappers
		//-----------------------------------------------------------------------

		template<typename T>
		void Serialize(std::ostream& ostream, T* typeInstance)
		{
			CreateIfAutomatic<T>(0);
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
			CreateIfAutomatic<T>(0);
			return DeserializeInternal(
					istream,
					typeInstance,
					std::type_index(typeid(*typeInstance)),
					SerializeSection::root);
		}

		//-----------------------------------------------------------------------
		// Serialization - disgusting details that you could use
		//-----------------------------------------------------------------------

		void OutputTypeInfo(
				std::ostream& ostream,
				const std::type_index& typeIndex,
				bool isInherited);

		enum class SerializeSection
		{
			root, inherited, field, collectionElement
		};

		void SerializeInternal(
				std::ostream& ostream,
				void* typeInstance,
				const std::type_index& typeIndex,
				unsigned int indents,
				SerializeSection section);

		DeserializeReturnCode DeserializeInternal(
				std::istream& istream,
				void* typeInstance,
				const std::type_index& typeIndex,
				SerializeSection section);

		std::pair<bool, const FieldInfo*> FindFieldName(
				const TypeInfo& typeInfo,
				const std::string& fieldName);

		static bool SkipCurlyBrackets(std::istream& istream, unsigned int bracketsLevel);

	private:

		std::vector<TypeInfo> _typeInfos;
		std::map<std::string, size_t> _typeNames;
		std::map<std::type_index, size_t> _typeIndices;

		TypeInfoFactory BeginAbstractTypeInfoInternal(
				bool isCollection,
				std::string typeName,
				std::type_index typeIndex);
	};

	template<typename T>
	TypeInfoFactory& TypeInfoFactory::WithField(std::string fieldName, size_t offset)
	{
		typeDatabase.CreateIfAutomatic<T>(0);
		return *WithFieldInternal(fieldName, offset, std::type_index(typeid(T)));
	}
}