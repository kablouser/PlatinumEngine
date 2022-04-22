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

#include <Helpers/VectorHelpers.h>
#include <Serialization/VectorCollection.h>

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

		TypeInfo& WithCollection(std::unique_ptr<Collection>&& withCollection);
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
			return *BeginAbstractTypeInfoInternal(
					isCollection,
					std::string(TypeName<T>()),
					std::type_index(typeid(T)));
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
			// TODO refactor nicely
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

		std::pair<bool, const TypeInfo*> GetTypeInfo(const std::string& typeName);

		std::pair<bool, const TypeInfo*> GetTypeInfo(const std::type_index& typeIndex);

		template<typename T>
		std::pair<bool, const TypeInfo*> GetTypeInfo()
		{
			return GetTypeInfo(std::type_index(typeid(T)));
		}

		//-----------------------------------------------------------------------
		// Get Type Name
		//-----------------------------------------------------------------------

		const char* GetTypeName(const std::type_index& typeIndex);

		template<typename T>
		const char* GetTypeName()
		{
			return GetTypeName(std::type_index(typeid(T)));
		}

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

		TypeInfo* BeginAbstractTypeInfoInternal(
				bool isCollection,
				std::string typeName,
				std::type_index typeIndex);

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
	};

	class TypeInfoFactory
	{
	public:
		TypeInfo& typeInfo;
		TypeDatabase& typeDatabase;

		TypeInfoFactory(TypeInfo& inTypeInfo, TypeDatabase& inTypeDatabase);

		//-----------------------------------------------------------------------
		// Factory pattern thingy
		//-----------------------------------------------------------------------

		template<typename T>
		TypeInfoFactory& WithInherit()
		{
			return *WithInheritInternal(std::type_index(typeid(T)));
		}

		template<typename T>
		TypeInfoFactory& WithField(std::string fieldName, size_t offset)
		{
			return *WithFieldInternal(fieldName, offset, std::type_index(typeid(T)));
		}

		TypeInfoFactory& WithCollection(std::unique_ptr<Collection>&& withCollection);

	private:

		TypeInfoFactory* WithInheritInternal(std::type_index&& typeIndex);

		TypeInfoFactory* WithFieldInternal(
				std::string& fieldName,
				size_t offset,
				std::type_index&& typeIndex);
	};
}