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

	TypeInfo& TypeInfoFactory::GetTypeInfo()
	{
		return typeDatabase._typeInfos.at(typeInfoIndex);
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

	void TypeDatabase::StreamOutString(std::ostream& outputStream, void* typeInstance)
	{
		// put the string inside quotes, and escape any quotes inside the string
		auto string = (std::string*)typeInstance;
		outputStream << '"';
		for (char character: *string)
		{
			if (character == '"')
				outputStream << (char)'\\'; // backslash isn't treated as a char by default?
			outputStream << character;
		}
		outputStream << '"';
	}

	void TypeDatabase::StreamInString(std::istream& inputStream, void* typeInstance)
	{
		auto string = (std::string*)typeInstance;
		string->clear();

		// remove leading white-space
		inputStream >> std::ws;

		char currentCharacter,nextCharacter;
		if (!inputStream.get(currentCharacter) || !inputStream.get(nextCharacter))
			return; // bad format, must have at least 2 quotes ""
		if (currentCharacter != '"')
		{
			PLATINUM_WARNING("StreamInString bad format.");
			return; // bad format, must start with a quote
		}

		// step 1 - past the first quote
		currentCharacter = nextCharacter;
		if (!inputStream.get(nextCharacter))
		{
			if (currentCharacter == '"')
				return; // okay format, but weird
			else
				return; // bad format
		}

		do
		{
			// detect escaped quote
			if (currentCharacter == '\\' && nextCharacter == '"')
			{
				// don't push_back the backslash
			}
			else
			{
				string->push_back(currentCharacter);
				if (nextCharacter == '"')
					break;
			}

			// step
			currentCharacter = nextCharacter;
			if (!inputStream.get(nextCharacter))
				return; // bad format
		}
		while(true);
	}

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
		for (auto& typeInfo: _typeInfos)
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
	// Get Stored Type Name
	//-----------------------------------------------------------------------

	std::string TypeDatabase::GetStoredTypeName(const std::type_index& typeIndex)
	{
		auto[success, typeInfo] = GetTypeInfo(typeIndex);
		if (success)
			return typeInfo->typeName;
		else
			return "?";
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
				ostream << GetStoredTypeName(typeIndex) << std::endl <<
						'{' << std::endl;

			if (typeInfo->inheritedType.has_value())
				OutputTypeInfo(ostream, *typeInfo->inheritedType, true);

			if (isInherited)
				ostream << '\t' << "// inherited from " << GetStoredTypeName(typeIndex) << std::endl;
			else if (typeInfo->inheritedType.has_value())
				ostream << '\t' << "// from " << GetStoredTypeName(typeIndex) << std::endl;

			for (auto& field: typeInfo->fields)
			{
				ostream << '\t' <<
						GetStoredTypeName(field.typeIndex) << ' ' <<
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
					ostream << tabs << GetStoredTypeName(typeIndex) << std::endl <<
							tabs << '{' << std::endl;
					break;
				case SerializeSection::field:
					ostream << std::endl << tabs << '{' << std::endl;
					break;
				case SerializeSection::collectionElement:
					//ostream << tabs << '{' << std::endl;
					ostream << '{' << std::endl;
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
					ostream << tabs << '\t' << "// inherited from " << GetStoredTypeName(typeIndex) << std::endl;
				else if (typeInfo->inheritedType.has_value())
					ostream << tabs << '\t' << "// from " << GetStoredTypeName(typeIndex) << std::endl;

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
						ostream << tabs << '\t' << GetStoredTypeName(field.typeIndex) << ' ' << field.fieldName
								<< " = ";
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
			PLATINUM_WARNING_STREAM << "Serializing a unknown type. Have you forgotten to create type info for something?";
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
				PLATINUM_WARNING_STREAM << "Unknown element type for collection: " << typeInfo->typeName;
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
				auto startPosition = istream.tellg();
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

				// after all these cases, the only case left is a field declaration
				// but type name could contain spaces, so find '=' and work backwards

				// reset to start
				istream.seekg(startPosition);
				// get entire line with type name, field name and '='
				// this getline means if there's a bad format, you don't need to consume the rest of the line
				if (!std::getline(istream, stringToken))
					continue; // getline failed

				// work forward
				// typeName begins at the first non-white-space character
				std::string::size_type
						typeNameBegin = std::string::npos,
						typeNameEnd = std::string::npos; // npos indicates no begin
				for (std::string::size_type i = 0; i < stringToken.size(); ++i)
					if (!std::isspace(stringToken[i])) // match with first non-space
					{
						typeNameBegin = i;
						break;
					}
				if (typeNameBegin == std::string::npos)
					continue; // entire line only contains spaces, bad format, continue parsing

				// work backward --- look at this example "type name fieldName = something;"
				std::string::size_type equalIndex = stringToken.find_last_of('=');
				if (equalIndex == std::string::npos)
					continue; // '=' not in line, bad format, continue parsing
				// move back 2 from '='
				if (equalIndex < 2)
					continue; // prevent underflow, index is unsigned number
				std::string::size_type fieldNameEnd = equalIndex - 2; // safe to subtract
				// find begin index of fieldName, backwards from fieldNameEnd
				std::string::size_type fieldNameBegin = std::string::npos; // npos indicates no begin
				// 1 <= i to prevent underflow
				for (std::string::size_type i = fieldNameEnd; 1 <= i; --i)
					if (std::isspace(stringToken[i])) // match with last space
					{
						fieldNameBegin = i + 1;
						typeNameEnd = i - 1; // subtraction can't underflow because 1 <= i
						break;
					}
				if (fieldNameBegin == std::string::npos)
					continue; // bad format

				if (typeNameEnd < typeNameBegin || fieldNameEnd < fieldNameBegin)
					continue; // protection against subtraction underflow
				std::string
						typeName = stringToken.substr(typeNameBegin, typeNameEnd - typeNameBegin + 1),
						fieldName = stringToken.substr(fieldNameBegin,
						fieldNameEnd - fieldNameBegin + 1); // +1 because lamppost

				auto[fieldSuccess, fieldTypeInfo]=GetTypeInfo(typeName);
				if (!fieldSuccess)
				{
					PLATINUM_WARNING_STREAM << "Unknown type: " << typeName;
					continue; // unknown type
				}

				// find fieldName in our type with inheritance
				auto[foundFieldName, fieldInfo] = FindFieldName(*typeInfo, fieldName);
				if (!foundFieldName)
				{
					PLATINUM_WARNING_STREAM << "Unknown field name: " << typeInfo->typeName << '.' << fieldName;
					continue; // mismatch, no fieldName
				}

				if (fieldTypeInfo->typeIndex != fieldInfo->typeIndex)
				{
					PLATINUM_WARNING_STREAM << "Field type doesn't match: " << typeInfo->typeName << '.' << fieldName;
					continue; // mismatch, field type is unexpected
				}

				istream.seekg(startPosition + std::streamoff(equalIndex + 1)); // after the equals

				// recurse
				DeserializeReturnCode code = DeserializeInternal(
						istream,
						fieldInfo->AccessField(typeInstance),
						fieldInfo->typeIndex,
						SerializeSection::field);
				if (code != DeserializeReturnCode::success)
					PLATINUM_WARNING_STREAM << "Deserialization recurse has return code " << (int)code;

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
			else
				PLATINUM_WARNING_STREAM << "Unknown inherited type for: " << typeInfo.typeName;
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
			return TypeInfoFactory(_typeInfos.size() - 1, *this);
		}
		else
		{
			PLATINUM_WARNING("Type already exists in TypeDatabase.");
			// typeName already exists in database, return the existing entry
			if (countTypeNames == 0)
				return TypeInfoFactory(_typeNames[typeName], *this);
			else
				return TypeInfoFactory(_typeIndices[typeIndex], *this);
		}
	}
}