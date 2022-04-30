#pragma once

#include <vector>
#include <map>
#include <set>
#include <random>
#include <typeindex>
#include <memory> // std::shared_ptr

// todo REmove
#include <Logger/Logger.h>

namespace PlatinumEngine
{

	template<typename T>
	class SavedReference;

	// 1 global variable visible to everyone
	extern std::set<void*> AllSavedReferences;

	// Track IDs for all types
	class IDSystem
	{
	public:

		// 0 is reserved for nullptr
		typedef std::size_t ID;

		// serialization of the managed memory

		// Ignores unknown types and Asset types. They are not streamed.
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
		// For updating references
		//--------------------------------------------------------------------------------------------------------------

		// Updates all saved references to the current pointers using their ids
		// Should be called after Remove/RemoveInternal from this IDSystem. Or changing existing entries with Overwrite.
		void UpdateSavedReferences();

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
				const std::type_index&& typeIndex,
				ID id,
				const std::shared_ptr<void>& pointer);

		// Add with new random id
		ID AddInternal(
				const std::type_index&& typeIndex,
				const std::shared_ptr<void>& pointer);

		bool RemoveInternal(
				ID id,
				std::type_index&& typeIndex);

		/**
		 * @param idMap target map to add the ID to
		 * @return an unique ID that doesn't exist in idMap
		 */
		ID GenerateID(std::map<ID, std::shared_ptr<void>>& idMap);

		friend class Scene; // todo remove
	private:
		typedef std::map<ID, std::shared_ptr<void>> IDMap;
		std::map<std::type_index, IDMap> _managedMemory;

		// random number generator
		std::mt19937 _generator;
		std::uniform_int_distribution<ID> _anyNumber;
	};

	std::string Interop(std::type_index ti);// todo remove

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
		const std::type_index typeIndex;

		~SavedReference()
		{
			AllSavedReferences.erase(this);
		}

		SavedReference() : id(0), typeIndex(typeid(T))
		{
			AllSavedReferences.insert(this);
		}

		SavedReference(IDSystem::ID inID, std::shared_ptr<T> inPointer) :
				id(inID), pointer(inPointer), typeIndex(typeid(T))
		{
			AllSavedReferences.insert(this);
		}

		SavedReference(const SavedReference<T>& copyFrom) :
			id(copyFrom.id), pointer(copyFrom.pointer), typeIndex(typeid(T))
		{
			AllSavedReferences.insert(this);
		}

		SavedReference(SavedReference<T>&& moveFrom) noexcept :
			id(moveFrom.id), pointer(std::move(moveFrom.pointer)), typeIndex(typeid(T))
		{
			moveFrom.id = 0;
			AllSavedReferences.insert(this);
		};

		SavedReference<T>& operator=(const SavedReference<T>& copyFrom)
		{
			if (this == &copyFrom)
				return *this;
			id = copyFrom.id;
			pointer = copyFrom.pointer;
			return *this;
		}

		SavedReference<T>& operator=(SavedReference<T>&& moveFrom) noexcept
  		{
			if (this == &moveFrom)
				return *this;
			id = moveFrom.id;
			moveFrom.id = 0;
			pointer = std::move(moveFrom.pointer);
			return *this;
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

		// Update pointer to the current pointer represented by the id
		void UpdatePointer(IDSystem& idSystem)
		{
			void* before = pointer.get();

			auto[outID, currentPointer] = idSystem.GetSavedReferenceInternal(id, std::type_index(typeIndex));
			if (outID == 0)
				// typeIndex is incorrect, this is fine. The SavedReferences inside idSystem all have incorrect type indices.
				return;

			pointer = std::move(currentPointer);

			void* after = pointer.get();
			if (before != after)
			{
				if (!before && after)
				{
					PLATINUM_INFO_STREAM << "Created " << Interop(typeIndex) << " " << after;
				}
				else if (before && !after)
				{
					PLATINUM_INFO_STREAM << "Deleted " << Interop(typeIndex) << " " << before;
				}
				else
				{
					PLATINUM_INFO_STREAM << "Edited " << Interop(typeIndex) << " from " << before << " to " << after;
				}
			}
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
		// Static casting to reference of another type
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
				// NOTE: remember to reference get(). we want type of the pointing to data
				std::type_index(typeid(*savedReference.pointer.get())));
	}
}