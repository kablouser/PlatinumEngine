#include <TypeDatabase/TypeDatabase.h>
#include <Logger/Logger.h>
#include <set> // std::set

namespace PlatinumEngine
{
	//-----------------------------------------------------------------------
	// FieldInfo
	//-----------------------------------------------------------------------

	void* FieldInfo::AccessField(void* typeInstance) const
	{
		return (char*)typeInstance + offset;
	}

	//-----------------------------------------------------------------------
	// TypeInfo
	//-----------------------------------------------------------------------

	TypeInfo::TypeInfo(
			std::type_index newTypeIndex,
			bool isCollection) :
			typeIndex(newTypeIndex),
			isCollection(isCollection)
	{
	}

	//--------------------------------------------------------------------------------------
	// Type Info Factory
	//--------------------------------------------------------------------------------------

	TypeInfoFactory::TypeInfoFactory(size_t inTypeInfoIndex, TypeDatabase& inTypeDatabase) :
			typeInfoIndex(inTypeInfoIndex),
			typeDatabase(inTypeDatabase)
	{
	}

	TypeInfoFactory& TypeInfoFactory::WithCollection(std::unique_ptr<Collection>&& withCollection)
	{
		auto& typeInfo = typeDatabase._typeInfos.at(typeInfoIndex);
		// isCollection must be enabled
		assert(typeInfo.isCollection);
		typeInfo.collection = std::move(withCollection);
		return *this;
	}

	TypeInfoFactory* TypeInfoFactory::WithInheritInternal(std::type_index&& typeIndex)
	{
		auto& typeInfo = typeDatabase._typeInfos.at(typeInfoIndex);
		// only 1 inherited type allowed
		assert(!typeInfo.inheritedType.has_value());
		// collections cannot inherit
		assert(!typeInfo.isCollection);
		typeInfo.inheritedType = typeIndex;
		return this;
	}

	TypeInfoFactory* TypeInfoFactory::WithFieldInternal(
			std::string& fieldName,
			size_t offset,
			std::type_index&& typeIndex)
	{
		auto& typeInfo = typeDatabase._typeInfos.at(typeInfoIndex);
		// fields cannot be in collections
		assert(!typeInfo.isCollection);
		FieldInfo newFieldInfo{ typeIndex, std::move(fieldName), offset };
		typeInfo.fields.push_back(newFieldInfo);
		return this;
	}

	//-----------------------------------------------------------------------
	// TypeDatabase
	//-----------------------------------------------------------------------

	TypeDatabase* TypeDatabase::Instance = nullptr;

	TypeDatabase::TypeDatabase()
	{
		// should only be 1 in the program
		assert(Instance == nullptr);
		Instance = this;
		Init();
	}

	TypeDatabase::~TypeDatabase()
	{
		// should be this in the program
		assert(Instance == this);
		Instance = nullptr;
	}

	//-----------------------------------------------------------------------
	// Get Type Info
	//-----------------------------------------------------------------------

	std::pair<bool, const TypeInfo*> TypeDatabase::GetTypeInfo(const std::string& typeName)
	{
		auto iterator = _typeNames.find(typeName);
		if (iterator == _typeNames.end())
			return { false, nullptr };
		else
			return { true, &_typeInfos.at(iterator->second) };
	}

	std::pair<bool, const TypeInfo*> TypeDatabase::GetTypeInfo(const std::type_index& typeIndex)
	{
		auto iterator = _typeIndices.find(typeIndex);
		if (iterator == _typeIndices.end())
			return { false, nullptr };
		else
			return { true, &_typeInfos.at(iterator->second) };
	}

	//-----------------------------------------------------------------------
	// Final Check
	//-----------------------------------------------------------------------

	bool TypeDatabase::FinalCheck()
	{
		bool result = true;
		for (auto& typeInfo : _typeInfos)
		{
			if (typeInfo.inheritedType.has_value())
			{
				auto[success, _] = GetTypeInfo(typeInfo.inheritedType.value());
				if (!success)
				{
					result = false;
					PLATINUM_WARNING_STREAM << "Unknown inherited type for " << typeInfo.typeName;
				}
			}

			if (typeInfo.isCollection)
			{
				if (typeInfo.collection)
				{
					auto[success, _] = GetTypeInfo(typeInfo.collection.get()->elementTypeIndex);
					if (!success)
					{
						result = false;
						PLATINUM_WARNING_STREAM << "Unknown collection element type for " <<
												typeInfo.typeName;
					}
				}
				else
				{
					result = false;
					PLATINUM_WARNING("isCollection = true, but collection object is null");
				}
			}
			else
			{
				for (auto& field: typeInfo.fields)
				{
					auto[success, _] = GetTypeInfo(field.typeIndex);
					if (!success)
					{
						result = false;
						PLATINUM_WARNING_STREAM << "Unknown field type for " <<
							typeInfo.typeName << "::" << field.fieldName;
					}
				}
			}
		}
		return result;
	}


	//-----------------------------------------------------------------------
	// Get Type Name
	//-----------------------------------------------------------------------

	const char* TypeDatabase::GetTypeName(const std::type_index& typeIndex)
	{
		static char unknownName[] = "?";
		auto[success, typeInfo] = GetTypeInfo(typeIndex);
		if (success)
			return typeInfo->typeName.c_str();
		else
			return unknownName;
	}

	//-----------------------------------------------------------------------
	// Output Type Info
	//-----------------------------------------------------------------------

	void TypeDatabase::OutputTypeInfo(std::ostream& ostream, const std::type_index& typeIndex)
	{
		OutputTypeInfo(ostream, typeIndex, false);
		ostream << ';' << std::endl;
	}

	//-----------------------------------------------------------------------
	// Serialization - disgusting details that you could use
	//-----------------------------------------------------------------------

	void TypeDatabase::OutputTypeInfo(
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

	void TypeDatabase::SerializeInternal(
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

	TypeDatabase::DeserializeReturnCode TypeDatabase::DeserializeInternal(
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

		if (!(istream >> stringToken && stringToken == "{"))
			return DeserializeReturnCode::badFormat;

		if (typeInfo->isCollection)
		{
			// is a collection

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

			while (true)
			{
				// look ahead to check for end of collection
				auto startPosition = istream.tellg();
				// next string begins with }
				if (istream >> stringToken && stringToken.rfind('}', 0) == 0)
					break;
				// reset position
				istream.seekg(startPosition);

				// recurse
				auto returnCode = DeserializeInternal(
						istream,
						collectionElement.get(),
						elementTypeInfo->typeIndex,
						SerializeSection::collectionElement);

				if (returnCode == DeserializeReturnCode::success)
				{
					typeInfo->collection->Add(typeInstance, collectionElement.get());
				}
				if (returnCode != DeserializeReturnCode::badFormat)
				{
					// probably comma next
					std::getline(istream, stringToken); // skip line
				}
			}

			return DeserializeReturnCode::success;
		}
		else
		{
			// not a collection
			while (true)
			{
				if (!(istream >> stringToken))
					return DeserializeReturnCode::badFormat;

				if (stringToken.rfind("//", 0) == 0 // stringToken begins with //, comment
					||
					stringToken == "?")// unknown type
				{
					std::getline(istream, stringToken); // skip line
					continue;
				}
				else if (stringToken.rfind('}', 0) == 0)
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
				DeserializeInternal(
						istream,
						fieldInfo->AccessField(typeInstance),
						fieldInfo->typeIndex,
						SerializeSection::field);
				// probably semicolon next
				std::getline(istream, stringToken); // skip line
			}
		}
	}

	std::pair<bool, const FieldInfo*>
	TypeDatabase::FindFieldName(const TypeInfo& typeInfo, const std::string& fieldName)
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

	bool TypeDatabase::SkipCurlyBrackets(std::istream& istream, unsigned int bracketsLevel)
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

	TypeInfoFactory TypeDatabase::BeginAbstractTypeInfoInternal(
			bool isCollection,
			std::string typeName,
			std::type_index typeIndex)
	{
		size_t countTypeNames = _typeNames.count(typeName);
		size_t countTypeIndices = _typeIndices.count(typeIndex);
		if (0 == countTypeNames && 0 == countTypeIndices)
		{
			_typeNames[typeName] = _typeIndices[typeIndex] = _typeInfos.size();
			_typeInfos.emplace_back(typeIndex, isCollection);
			_typeInfos.at(_typeInfos.size() - 1).typeName = std::move(typeName);
			return TypeInfoFactory(_typeInfos.size() - 1,*this);
		}
		else
		{
			PLATINUM_WARNING("Type already exists in TypeDatabase.");
			// typeName already exists in database, return the existing entry
			if (countTypeNames == 0)
				return TypeInfoFactory(_typeNames[typeName],*this);
			else
				return TypeInfoFactory(_typeIndices[typeIndex],*this);
		}
	}
}