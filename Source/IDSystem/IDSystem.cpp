#include <IDSystem/IDSystem.h>
#include <TypeDatabase/TypeDatabase.h>
#include <Logger/Logger.h>
#include <Loaders/LoaderCommon.h>
#include <iostream>
#include <cassert> // for assert()

namespace PlatinumEngine
{
	// Instance of the global variable
	std::set<void*> AllSavedReferences;

	void IDSystem::StreamOut(std::ostream& outputStream, void* typeInstance)
	{
		assert(TypeDatabase::Instance && "Stream function only legal after type database is constructed");
		auto* idSystem = (IDSystem*)typeInstance;
		outputStream << "IDSystem" << std::endl << '{' << std::endl;

		// NOTE:
		// output order must NOT depend on type_index
		// consistent ordering across platforms is nice for version control
		// type_index is platform dependant

		// get names for all ID maps
		typedef std::tuple<std::type_index, IDMap*, std::string> NamedIDMap;
		std::vector<NamedIDMap> namedIDMaps;
		for (auto& typeEntry: idSystem->_typeIndexToIDMap)
		{
			std::string typeName;
			if (!IsStreamable(typeEntry.first, &typeName))
				continue;

			namedIDMaps.emplace_back(
					typeEntry.first,
					&typeEntry.second,
					typeName);
		}

		// sort with name
		std::sort(namedIDMaps.begin(), namedIDMaps.end(),
				[](const NamedIDMap& lhs, const NamedIDMap& rhs)
				{
					return std::get<2>(lhs) < std::get<2>(rhs);
				});

		// now loop through sorted list
		for (auto& namedIDMap: namedIDMaps)
		{
			for (auto& idEntry: *std::get<1>(namedIDMap)) // NamedIDMap
			{
				if (!idEntry.second) // if null
					continue;

				outputStream << "\t{" << std::endl;
				outputStream << "\t\t" << idEntry.first << ',' << std::endl;
				outputStream << "\t\t" << std::get<2>(namedIDMap) << ' '; // name of type
				TypeDatabase::Instance->SerializeInternal(
						outputStream,
						idEntry.second.get(),
						std::get<0>(namedIDMap), // type_index
						2,
						TypeDatabase::SerializeSection::field);
				outputStream << std::endl << "\t}," << std::endl;
			}
		}

		outputStream << '}';
	}

	void IDSystem::StreamIn(std::istream& inputStream, void* typeInstance)
	{
		assert(TypeDatabase::Instance && "Stream function only legal after type database is constructed");
		auto* idSystem = (IDSystem*)typeInstance;

		std::string stringToken;
		if (!(inputStream >> stringToken && stringToken == "IDSystem" &&
			  inputStream >> stringToken && stringToken == "{"))
			return;

		while (inputStream.good())
		{
			if (!(inputStream >> stringToken))
				break;// todo return
			// next string begins with }
			if (stringToken.rfind('}', 0) == 0)
				break;// todo return

			ID id;
			if (!(stringToken == "{" &&
				  inputStream >> id &&
				  inputStream >> stringToken && stringToken == ","))
			{
				// bad format
				PLATINUM_WARNING("Parsing bad format in ID System");
				std::getline(inputStream, stringToken); // skip line
				continue;
			}

			auto startPosition = inputStream.tellg();
			if (!(inputStream >> stringToken) || stringToken == "?")
			{
				// bad format or unknown type
				std::getline(inputStream, stringToken); // skip line
				continue;
			}

			auto[success, typeInfo]= TypeDatabase::Instance->GetTypeInfo(stringToken);
			if (!success)
			{
				// no type info
				// type name might be entire line with spaces
				// get whole line from startPosition
				inputStream.seekg(startPosition);
				inputStream >> std::ws; // remove leading white space
				std::getline(inputStream, stringToken);
				// remove trailing white space
				stringToken.erase(
						std::find_if(
								stringToken.rbegin(),
								stringToken.rend(),
								[](unsigned char ch)
								{
									return !std::isspace(ch);
								}).base(),
						stringToken.end());

				// attempt type info lookup again
				auto[success2, typeInfo2] = TypeDatabase::Instance->GetTypeInfo(stringToken);
				success = success2;
				typeInfo = typeInfo2;
			}

			if (success)
			{
				if (typeInfo->allocate)
				{
					auto temp = typeInfo->allocate();
					TypeDatabase::DeserializeReturnCode code = TypeDatabase::Instance->DeserializeInternal(
							inputStream,
							temp.get(),
							typeInfo->typeIndex,
							TypeDatabase::SerializeSection::collectionElement);
					if (code != TypeDatabase::DeserializeReturnCode::success)
						PLATINUM_WARNING_STREAM << "IDSystem stream in has deserialization code " << (int)code;

					if (idSystem->AddInternal(id, temp, typeInfo->typeIndex))
					{
						// good
					}
					else
					{
						// bad, ID wasn't unique
						PLATINUM_WARNING_STREAM << "ID is used already by type:" << typeInfo->typeName
												<< ", id =" << id;
					}
				}
				else
				{
					PLATINUM_INFO_STREAM << "type missing allocator: " << typeInfo->typeName;
				}
			}
			else
			{
				PLATINUM_INFO_STREAM << "unknown type: \"" << stringToken << "\"";

				// look ahead for open {, and remove if found
				startPosition = inputStream.tellg();
				if (!(inputStream >> stringToken))
				{
					// bad format
					PLATINUM_WARNING("Parsing bad format in ID System");
					std::getline(inputStream, stringToken); // skip line
					continue;
				}

				if (stringToken == "{") // found
					TypeDatabase::SkipCurlyBrackets(inputStream, 1);
				else // not found
					inputStream.seekg(startPosition);
			}

			// expect next },
			if (!(inputStream >> stringToken && stringToken == "},"))
			{
				// bad format
				PLATINUM_WARNING("Parsing bad format in ID System");
				std::getline(inputStream, stringToken); // skip line
				continue;
			}
		}

		PLATINUM_INFO_STREAM << idSystem->_typeIndexToIDMap.size();
		for (auto& kvp : idSystem->_typeIndexToIDMap)
			PLATINUM_INFO_STREAM << TypeDatabase::Instance->GetStoredTypeName(kvp.first) << kvp.second.size();
	}

	bool IDSystem::IsStreamable(std::type_index typeIndex, std::string* outTypeName)
	{
		// asset types are not streamable. Reason being you shouldn't change assets from the engine
		if (Loaders::IsAsset(typeIndex))
			return false;

		// unknown types should not be streamed, no point in streaming it
		if (outTypeName == nullptr)
			return TypeDatabase::Instance->GetStoredTypeName(typeIndex) != "?";
		else
		{
			*outTypeName = TypeDatabase::Instance->GetStoredTypeName(typeIndex);
			return *outTypeName != "?";
		}
	}

	IDSystem::IDSystem() :
			_generator(std::random_device()()),
			_anyNumber(std::numeric_limits<ID>::min(), std::numeric_limits<ID>::max())
	{
	}

	void IDSystem::Clear()
	{
		for (auto& keyValuePair : _typeIndexToIDMap)
		{
			if (!IsStreamable(keyValuePair.first))
				continue;

			keyValuePair.second.clear();
		}
	}

	void IDSystem::Overwrite(ID id, std::shared_ptr<void>& pointer, std::type_index typeIndex)
	{
		_typeIndexToIDMap[typeIndex][id] = pointer;
	}

	void IDSystem::UpdateSavedReferences()
	{
		// while UpdatePointer, SavedReferences could be added/removed
		std::set<void*> allSavedReferencesCopy = AllSavedReferences;
		int i = 0;
		int tots = allSavedReferencesCopy.size();
		for (void* savedReference: allSavedReferencesCopy)
		{
			reinterpret_cast<SavedReference<void>*>(savedReference)->UpdatePointer(*this);
			++i;
		}
	}

	std::shared_ptr<void> IDSystem::GetSavedReferenceInternal(ID id, std::type_index typeIndex)
	{
		if (id == 0)
			// 0 is reserved for nullptr
			return {}; // nullptr

		auto typeIndexToIDMapIterator = _typeIndexToIDMap.find(typeIndex);
		if (typeIndexToIDMapIterator == _typeIndexToIDMap.end())
			// not found
			return {}; // nullptr

		IDMap& idMap = typeIndexToIDMapIterator->second;
		auto idMapIterator = idMap.find(id);
		if (idMapIterator == idMap.end())
			// not found
			return {}; // nullptr

		// found
		return idMapIterator->second; // return a copy
	}

	SavedReference<void> IDSystem::GetSavedReferenceInternal(void* rawPointer, std::type_index typeIndex)
	{
		if (rawPointer == nullptr)
			return {}; // nullptr

		auto typeIndexToIDMapIterator = _typeIndexToIDMap.find(typeIndex);
		if (typeIndexToIDMapIterator == _typeIndexToIDMap.end())
			// not found
			return {}; // nullptr

		IDMap& idMap = typeIndexToIDMapIterator->second;
		// very slow part
		for (auto& idEntry: idMap)
		{
			if (idEntry.second.get() == rawPointer)
				return { idEntry.first, idEntry.second, typeIndex }; // return a copy
		}

		// not found
		return {}; // nullptr
	}

	bool IDSystem::AddInternal(IDSystem::ID id, const std::shared_ptr<void>& pointer, std::type_index typeIndex)
	{
		auto[_, success] =_typeIndexToIDMap[typeIndex].insert({ id, pointer });
		return success;
	}

	IDSystem::ID IDSystem::AddInternal(const std::shared_ptr<void>& pointer, std::type_index typeIndex)
	{
		IDMap& idMap = _typeIndexToIDMap[typeIndex];
		ID newID = GenerateID(idMap);
		assert(idMap.insert({ newID, pointer }).second); // checks the newID was added
		return newID;
	}

	bool IDSystem::RemoveInternal(IDSystem::ID id, std::type_index typeIndex)
	{
		auto typeIndexToIDMapIterator = _typeIndexToIDMap.find(typeIndex);
		if (typeIndexToIDMapIterator == _typeIndexToIDMap.end())
			// not found
			return false; // nullptr

		IDMap& idMap = typeIndexToIDMapIterator->second;
		// erase returns number of elements removed
		return idMap.erase(id) == 1; // should be 1 when removed
	}

	IDSystem::ID IDSystem::GenerateID(IDMap& idMap)
	{
		ID result;
		do
		{
			result = _anyNumber(_generator);
		} while (idMap.find(result) != idMap.end());

		return result;
	}
}
