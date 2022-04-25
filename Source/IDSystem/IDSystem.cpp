#include <IDSystem/IDSystem.h>
#include <TypeDatabase/TypeDatabase.h>
#include <iostream>
#include <cassert> // for assert()

// todo remove after debugging
#include <Logger/Logger.h>

namespace PlatinumEngine
{
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
		for (auto& typeEntry: idSystem->managedMemory)
		{
			namedIDMaps.emplace_back(
					typeEntry.first,
					&typeEntry.second,
					TypeDatabase::Instance->GetTypeName(typeEntry.first)
			);
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
				return;
			// next string begins with }
			if (stringToken.rfind('}', 0) == 0)
				return;

			ID id;
			if (!(stringToken == "{" &&
				  inputStream >> id &&
				  inputStream >> stringToken && stringToken == ","))
			{
				// bad format
				std::getline(inputStream, stringToken); // skip line
				continue;
			}

			auto startPosition = inputStream.tellg();
			if (!(inputStream >> stringToken))
			{
				// bad format
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
					TypeDatabase::Instance->DeserializeInternal(
							inputStream,
							temp.get(),
							typeInfo->typeIndex,
							TypeDatabase::SerializeSection::collectionElement);

					if (idSystem->AddInternal(typeInfo->typeIndex, id, temp))
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
				std::getline(inputStream, stringToken); // skip line
				continue;
			}
		}
	}

	IDSystem::IDSystem() :
			_generator(std::random_device()()),
			_anyNumber(std::numeric_limits<ID>::min(), std::numeric_limits<ID>::max())
	{
	}

	std::pair<IDSystem::ID, std::shared_ptr<void>> IDSystem::GetSavedReferenceInternal(
			std::size_t id,
			std::type_index&& typeIndex)
	{
		if (id == 0)
			// 0 is reserved for nullptr
			return {}; // nullptr

		auto managedMemoryIterator = managedMemory.find(typeIndex);
		if (managedMemoryIterator == managedMemory.end())
			// no entries for type
			return {}; // nullptr

		auto idMap = managedMemoryIterator->second;
		auto idMapIterator = idMap.find(id);
		if (idMapIterator == idMap.end())
			// id doesn't exist for type
			return {}; // nullptr

		// only return shared_ptr if type matches
		return {id, idMapIterator->second};
	}

	std::pair<IDSystem::ID, std::shared_ptr<void>> IDSystem::GetSavedReferenceInternal(
			void* rawPointer,
			std::type_index&& typeIndex);
	{
		if (rawPointer == nullptr)
			return {}; // nullptr

		auto managedMemoryIterator = managedMemory.find(typeIndex);
		if (managedMemoryIterator == managedMemory.end())
			// no entries for type
			return {}; // nullptr

		auto idMap = managedMemoryIterator->second;
		// very slow part
		for (auto& idEntry : idMap)
		{
			if (idEntry.second.get() == rawPointer)
				return idEntry.second;
		}
		// id doesn't exist for type
		return {}; // nullptr
	}

	bool IDSystem::AddInternal(
			const std::type_index& typeIndex,
			IDSystem::ID id,
			const std::shared_ptr<void>& pointer)
	{
		auto& idMap = managedMemory[typeIndex];
		auto[_, success] =idMap.insert({ id, pointer });
		return success;
	}

	IDSystem::ID IDSystem::AddInternal(
			const std::shared_ptr<void>& pointer,
			std::type_index&& typeIndex)
	{
		auto& idMap = managedMemory[typeIndex];
		ID newID = GenerateID(idMap);
		// checks the newID was added, and is unique in map
		assert(idMap.insert({ newID, pointer }).second);
		return newID;
	}

	bool IDSystem::RemoveInternal(
			IDSystem::ID id,
			std::type_index&& typeIndex)
	{
		auto managedMemoryIterator = managedMemory.find(typeIndex);
		if (managedMemoryIterator == managedMemory.end())
			// no entries for type
			return false;

		auto idMap = managedMemoryIterator->second;
		// erase returns number of elements removed
		return idMap.erase(id) == 1;
	}

	IDSystem::ID IDSystem::GenerateID(std::map<ID, std::shared_ptr<void>>& idMap)
	{
		ID result;
		do
		{
			result = _anyNumber(_generator);
		} while (idMap.find(result) != idMap.end());

		return result;
	}
}
