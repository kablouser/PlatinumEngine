#pragma once

#include <vector>
#include <map>
#include <set>
#include <random>
#include <typeindex>
#include <memory> // std::shared_ptr

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
		typedef std::map<ID, std::shared_ptr<void>> IDMap;

		// serialization of the managed memory

		// Only streamable types are streamed.
		static void StreamOut(std::ostream& outputStream, void* typeInstance);

		static void StreamIn(std::istream& inputStream, void* typeInstance);

		// False for unknown types and asset types. True otherwise.
		static bool IsStreamable(std::type_index typeIndex, std::string* outTypeName = nullptr);

		IDSystem();

		// Only streamable types are cleared.
		void Clear();

		//--------------------------------------------------------------------------------------------------------------
		// Nice wrapper controls
		//--------------------------------------------------------------------------------------------------------------

		template<typename T>
		SavedReference<T> GetSavedReference(ID id);

		template<typename T>
		SavedReference<T> GetSavedReference(T* rawPointer);

		template<typename T>
		SavedReference<T> Add(std::shared_ptr<T> pointer);

		template<typename T>
		SavedReference<T> Add();

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
		void Overwrite(ID id, std::shared_ptr<void>& pointer, std::type_index typeIndex);

		//--------------------------------------------------------------------------------------------------------------
		// For updating references
		//--------------------------------------------------------------------------------------------------------------

		// Updates all saved references to the current pointers using their ids
		// Should be called after Remove/RemoveInternal from this IDSystem. Or changing existing entries with Overwrite.
		void UpdateSavedReferences();

		//--------------------------------------------------------------------------------------------------------------
		// Disgusting internal controls
		//--------------------------------------------------------------------------------------------------------------

		std::shared_ptr<void> GetSavedReferenceInternal(ID id, std::type_index typeIndex);

		SavedReference<void> GetSavedReferenceInternal(void* rawPointer, std::type_index typeIndex);

		// Add with desired id
		bool AddInternal(ID id,	const std::shared_ptr<void>& pointer, std::type_index typeIndex);

		// Add with new random id
		ID AddInternal(const std::shared_ptr<void>& pointer, std::type_index typeIndex);

		bool RemoveInternal(ID id, std::type_index typeIndex);

		/**
		 * @return an unique ID that doesn't exist in idMap
		 */
		ID GenerateID(IDMap& idMap);

	private:
		std::map<std::type_index, IDMap> _typeIndexToIDMap;

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

		// This is the T parameter, useful for template magic
		typedef T Parameter;

		// 0 indicates nullptr
		IDSystem::ID id;
		std::shared_ptr<T> pointer;
		std::type_index typeIndex;

		~SavedReference()
		{
			AllSavedReferences.erase(this);
		}

		SavedReference() : id(0), typeIndex(typeid(T))
		{
			AllSavedReferences.insert(this);
		}

		// this cannot be used when T is void
		SavedReference(IDSystem::ID inID, std::shared_ptr<T> inPointer) :
				id(inID), pointer(inPointer), typeIndex(typeid(*inPointer.get()))
		{
			AllSavedReferences.insert(this);
		}

		SavedReference(IDSystem::ID inID, std::shared_ptr<T> inPointer, std::type_index inTypeIndex) :
				id(inID), pointer(inPointer), typeIndex(inTypeIndex)
		{
			AllSavedReferences.insert(this);
		}

		SavedReference(const SavedReference<T>& copyFrom) :
			id(copyFrom.id), pointer(copyFrom.pointer), typeIndex(copyFrom.typeIndex)
		{
			AllSavedReferences.insert(this);
		}

		SavedReference(SavedReference<T>&& moveFrom) noexcept :
			id(moveFrom.id), pointer(std::move(moveFrom.pointer)), typeIndex(moveFrom.typeIndex)
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
			typeIndex = copyFrom.typeIndex;
			return *this;
		}

		SavedReference<T>& operator=(SavedReference<T>&& moveFrom) noexcept
  		{
			if (this == &moveFrom)
				return *this;
			id = moveFrom.id;
			moveFrom.id = 0;
			pointer = std::move(moveFrom.pointer);
			typeIndex = moveFrom.typeIndex;
			return *this;
		}

		// is this a nullptr?
		explicit operator bool() const
		{
			return id != 0 && pointer;
		}

		// is the other the same?
		bool operator==(const SavedReference<T>& other)
		{
			return id == other.id && typeIndex == other.typeIndex;
		}

		// Update pointer to the current pointer represented by the id
		void UpdatePointer(IDSystem& idSystem)
		{
			pointer = std::move(idSystem.GetSavedReferenceInternal(id, typeIndex));
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
			return { id, std::static_pointer_cast<U>(pointer), typeIndex };
		}
	};

	//------------------------------------------------------------------------------------------------------------------
	// Definitions of templates
	//------------------------------------------------------------------------------------------------------------------

	template<typename T>
	SavedReference<T> IDSystem::GetSavedReference(ID id)
	{
		std::shared_ptr<void> sharedPointer = GetSavedReferenceInternal(id, typeid(T));
		// static_pointer_cast, somewhat safe
		return { id, std::static_pointer_cast<T>(sharedPointer), typeid(T) };
	}

	template<typename T>
	SavedReference<T> IDSystem::GetSavedReference(T* rawPointer)
	{
		SavedReference<void> savedReference = GetSavedReferenceInternal(rawPointer, typeid(T));
		// static_pointer_cast, somewhat safe
		return { savedReference.id, std::static_pointer_cast<T>(savedReference.pointer), typeid(T) };
	}

	template<typename T>
	SavedReference<T> IDSystem::Add(std::shared_ptr<T> pointer)
	{
		ID id = AddInternal(pointer, typeid(T));
		return { id, pointer, typeid(T) };
	}

	template<typename T>
	SavedReference<T> IDSystem::Add()
	{
		return Add(std::make_shared<T>());
	}

	template<typename T>
	bool IDSystem::Remove(SavedReference<T>& savedReference)
	{
		return RemoveInternal(savedReference.id, savedReference.typeIndex);
	}
}