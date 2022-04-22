#include <Serialization/Serialization.h>

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

	TypeInfo& TypeInfo::WithCollection(std::unique_ptr<Collection>&& withCollection)
	{
		// isCollection must be enabled
		assert(isCollection);
		collection = std::move(withCollection);
		return *this;
	}

	//-----------------------------------------------------------------------
	// Get Type Info
	//-----------------------------------------------------------------------

	std::pair<bool, const TypeInfo*> TypeDatabase::GetTypeInfo(const std::string& typeName)
	{
		auto iterator = typeNames.find(typeName);
		if (iterator == typeNames.end())
			return { false, nullptr };
		else
			return { true, &typeInfos.at(iterator->second) };
	}

	std::pair<bool, const TypeInfo*> TypeDatabase::GetTypeInfo(const std::type_index& typeIndex)
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
	// Serialization
	//-----------------------------------------------------------------------

	TypeInfo* TypeDatabase::BeginAbstractTypeInfoInternal(
			bool isCollection,
			std::string typeName,
			std::type_index typeIndex)
	{
		size_t countTypeNames = typeNames.count(typeName);
		size_t countTypeIndices = typeIndices.count(typeIndex);
		if (0 == countTypeNames && 0 == countTypeIndices)
		{
			typeNames[typeName] = typeIndices[typeIndex] = typeInfos.size();
			typeInfos.emplace_back(typeIndex, isCollection);
			TypeInfo& typeInfo = typeInfos[typeInfos.size() - 1];
			typeInfo.typeName = std::move(typeName);
			return &typeInfo;
		}
		else
		{
			// TODO replace with PLATINUM_WARNING
			std::cout << "TypeDatabase WARNING: type already exists in database " << typeName << std::endl;
			// typeName already exists in database, return the existing entry
			if (countTypeNames == 0)
				return &typeInfos.at(typeNames[typeName]);
			else
				return &typeInfos.at(typeIndices[typeIndex]);
		}
	}

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

	std::pair<bool, const FieldInfo*> TypeDatabase::FindFieldName(const TypeInfo& typeInfo, const std::string& fieldName)
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

	//--------------------------------------------------------------------------------------
	// Type Info Factory
	//--------------------------------------------------------------------------------------

	TypeInfoFactory::TypeInfoFactory(TypeInfo& inTypeInfo, TypeDatabase& inTypeDatabase) :
		typeInfo(inTypeInfo),
		typeDatabase(inTypeDatabase)
	{
	}

	TypeInfoFactory& TypeInfoFactory::WithCollection(std::unique_ptr<Collection>&& withCollection)
	{
		// isCollection must be enabled
		assert(typeInfo.isCollection);
		typeInfo.collection = std::move(withCollection);
		return *this;
	}

	TypeInfoFactory* TypeInfoFactory::WithInheritInternal(std::type_index&& typeIndex)
	{
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
		return this;
	}
}