#pragma once

#include <vector>
#include <map>
#include <random>
#include <typeindex>
#include <memory> // std::shared_ptr

namespace PlatinumEngine
{
	// forward declare for IDSystem
	template<typename T>
	class SavedReference;

	// Track IDs for all types
	class IDSystem
	{
	public:

		// 0 is reserved for nullptr
		typedef std::size_t ID;

		// serialization of the managed memory

		static void StreamOut(std::ostream& outputStream, void* typeInstance);

		static void StreamIn(std::istream& inputStream, void* typeInstance);

		IDSystem();

		//--------------------------------------------------------------------------------------------------------------
		// Nice wrapper controls
		//--------------------------------------------------------------------------------------------------------------

		template<typename T>
		SavedReference<T> GetSavedReference(ID id)
		{
			auto [_, sharedPointer] = GetSavedReferenceInternal(id, std::type_index(typeid(T)));
			return {id, std::static_pointer_cast<T>(sharedPointer)};
		}

		template<typename T>
		SavedReference<T> GetSavedReference(T* rawPointer)
		{
			auto [id, sharedPointer] = GetSavedReferenceInternal(rawPointer, std::type_index(typeid(T)));
			return {id, std::static_pointer_cast<T>(sharedPointer)};
		}

		template<typename T>
		ID Add(std::shared_ptr<T> pointer)
		{
			return AddInternal(pointer, std::type_index(typeid(T)));
		}

		template<typename T>
		bool Remove(ID id)
		{
			return RemoveInternal(id, std::type_index(typeid(T)));
		};

		//--------------------------------------------------------------------------------------------------------------
		// Disgusting internal controls
		//--------------------------------------------------------------------------------------------------------------

		std::pair<ID, std::shared_ptr<void>> GetSavedReferenceInternal(
				std::size_t id,
				std::type_index&& typeIndex);

		std::pair<ID, std::shared_ptr<void>> GetSavedReferenceInternal(
				void* rawPointer,
				std::type_index&& typeIndex);

		bool AddInternal(
				const std::type_index& typeIndex,
				ID id,
				const std::shared_ptr<void>& pointer);

		ID AddInternal(
				const std::shared_ptr<void>& pointer,
				std::type_index&& typeIndex);

		bool RemoveInternal(
				ID id,
				std::type_index&& typeIndex);

		/**
		 * @param idMap target map to add the ID to
		 * @return an unique ID that doesn't exist in idMap
		 */
		ID GenerateID(std::map<ID, std::shared_ptr<void>>& idMap);

	private:
		typedef std::map<ID, std::shared_ptr<void>> IDMap;

		std::map<std::type_index, IDMap> managedMemory;
		// random number generator
		std::mt19937 _generator;

		std::uniform_int_distribution<ID> _anyNumber;
	};

	/**
	 * A reference that can be saved automatically.
	 * Serializable.
	 * @tparam T type of thing to save a reference to
	 */
	template<typename T>
	class SavedReference
	{
	public:

		// 0 indicates nullptr
		IDSystem::ID id;
		std::shared_ptr<T> pointer;

		SavedReference() : id(0)
		{
		}

		SavedReference(T* rawPointer, IDSystem& idSystem) :
				pointer(rawPointer)
		{
			id = idSystem.Add(pointer);
		}

		void OnIDSystemUpdate(const IDSystem& idSystem)
		{
			pointer = idSystem.GetSavedReference<T>(id);
		}

		// is this a nullptr?
		explicit operator bool() const
		{
			return id != 0 && pointer;
		}

		// is the other the same?
		bool operator==(const SavedReference& other)
		{
			return id == other.id;
		}
	};
}