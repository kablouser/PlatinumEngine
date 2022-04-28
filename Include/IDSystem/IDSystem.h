#pragma once

#include <vector>
#include <map>
#include <random>
#include <typeindex>
#include <memory> // std::shared_ptr

namespace PlatinumEngine
{
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
			auto[_, sharedPointer] = GetSavedReferenceInternal(id, std::type_index(typeid(T)));
			// static_pointer_cast because no need to check type
			return { id, std::static_pointer_cast<T>(sharedPointer) };
		}

		template<typename T>
		SavedReference<T> GetSavedReference(T* rawPointer)
		{
			auto[id, sharedPointer] = GetSavedReferenceInternal(
					rawPointer,
					// very important to have the pointer in-front of rawPointer, gets type of pointing to
					std::type_index(typeid(*rawPointer))
			);
			// static_pointer_cast because no need to check type
			return { id, std::static_pointer_cast<T>(sharedPointer) };
		}

		template<typename T>
		SavedReference<T> Add(std::shared_ptr<T> pointer)
		{
			ID id = AddInternal(std::type_index(typeid(T)), pointer);
			return { id, pointer };
		}

		template<typename T>
		SavedReference<T> Add()
		{
			return Add(std::make_shared<T>());
		}

		// forward declare
		template<typename T>
		bool Remove(SavedReference<T>& savedReference);

		//--------------------------------------------------------------------------------------------------------------
		// For the Asset Database to load stuff
		//--------------------------------------------------------------------------------------------------------------

		/**
		 * If type and id doesn't exist in system, then adds it.
		 * If type and id already exists, replaces them with new pointer.
		 */
		void Overwrite(const std::type_index& typeIndex, ID id, std::shared_ptr<void>& pointer);

		//--------------------------------------------------------------------------------------------------------------
		// Disgusting internal controls
		//--------------------------------------------------------------------------------------------------------------

		std::pair<ID, std::shared_ptr<void>> GetSavedReferenceInternal(
				std::size_t id,
				std::type_index&& typeIndex);

		std::pair<ID, std::shared_ptr<void>> GetSavedReferenceInternal(
				void* rawPointer,
				std::type_index&& typeIndex);

		// Add with desired id
		bool AddInternal(
				std::type_index&& typeIndex,
				ID id,
				const std::shared_ptr<void>& pointer);

		// Add with new random id
		ID AddInternal(
				std::type_index&& typeIndex,
				const std::shared_ptr<void>& pointer);

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

		SavedReference(IDSystem::ID inID, std::shared_ptr<T> inPointer) :
				id(inID), pointer(inPointer)
		{
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

		// Dynamic casting to reference of another type
		template<typename U>
		explicit operator SavedReference<U>() const
		{
			std::shared_ptr<U> castedPointer = std::dynamic_pointer_cast<U>(pointer);
			if (castedPointer)
				return { id, castedPointer };
			else
				return {}; // nullptr
		}

		// NO TYPE CHECKING, VERY DANGEROUS
		// Dynamic casting to reference of another type
		template<typename U>
		SavedReference<U> UnsafeCast() const
		{
			return { id, std::static_pointer_cast<U>(pointer) };
		}
	};

	// definition of forward declare
	template<typename T>
	bool IDSystem::Remove(SavedReference<T>& savedReference)
	{
		return RemoveInternal(
				savedReference.id,
				// performs dynamic cast if T is polymorphic
				// otherwise its just type_index of T
				std::type_index(typeid(savedReference.pointer.get())));
	}
}