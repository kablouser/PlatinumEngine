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
	 	template<typename T>
		void Overwrite(ID id, T&& moveFrom);

		//--------------------------------------------------------------------------------------------------------------
		// Disgusting internal controls
		//--------------------------------------------------------------------------------------------------------------

		std::pair<bool, std::shared_ptr<void>*> GetSavedReferenceInternal(ID id, std::type_index typeIndex);

		SavedReference<void> GetSavedReferenceInternal(void* rawPointer, std::type_index typeIndex);

		// Add with desired id
		bool AddInternal(ID id, std::shared_ptr<void>& pointer, std::type_index typeIndex);

		// Add with new random id
		std::pair<ID,std::shared_ptr<void>&> AddInternal(std::shared_ptr<void>& pointer, std::type_index typeIndex);

		bool RemoveInternal(ID id, std::type_index typeIndex);

		/**
		 * @return an unique ID that doesn't exist in idMap
		 */
		ID GenerateID(IDMap& idMap);

	private:
		// owns these shared pointers
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
		// weak_ptr has no ownership
		std::weak_ptr<T> pointer;
		std::type_index typeIndex;

		SavedReference() : id(0), typeIndex(typeid(T))
		{
		}

		// this cannot be used when T is void
		SavedReference(IDSystem::ID inID, std::weak_ptr<T> inPointer) :
				id(inID), pointer(inPointer),
				// vvv if this causes compile errors, T is NOT void
				// it cannot work otherwise
				typeIndex(typeid(*inPointer.lock().get()))
		{
		}

		SavedReference(IDSystem::ID inID, std::weak_ptr<T> inPointer, std::type_index inTypeIndex) :
				id(inID), pointer(inPointer), typeIndex(inTypeIndex)
		{
		}

		SavedReference(const SavedReference<T>& copyFrom) :
			id(copyFrom.id), pointer(copyFrom.pointer), typeIndex(copyFrom.typeIndex)
		{
		}

		SavedReference(SavedReference<T>&& moveFrom) noexcept :
			id(moveFrom.id), pointer(std::move(moveFrom.pointer)), typeIndex(moveFrom.typeIndex)
		{
			moveFrom.id = 0;
		}

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
			return id != 0 && !pointer.expired();
		}

		// is the other the same?
		bool operator==(const SavedReference<T>& other)
		{
			return id == other.id && typeIndex == other.typeIndex;
		}

		// Dynamic casting to reference of another type
		template<typename U>
		explicit operator SavedReference<U>() const
		{
			std::shared_ptr<U> castedPointer = std::dynamic_pointer_cast<U>(pointer.lock());
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
			return { id, std::static_pointer_cast<U>(pointer.lock()), typeIndex };
		}

		std::shared_ptr<T> DeRef() const
		{
			return pointer.lock();
		}

		// Update weak pointer to the current pointer represented by the id
		void OnIDSystemUpdate(IDSystem& idSystem)
		{
			auto [success, sharedPointer] = idSystem.GetSavedReferenceInternal(id, typeIndex);
			if (success)
				pointer = std::static_pointer_cast<T>(*sharedPointer);
			else
				pointer = {};
		}
	};

	//------------------------------------------------------------------------------------------------------------------
	// Definitions of templates
	//------------------------------------------------------------------------------------------------------------------

	template<typename T>
	SavedReference<T> IDSystem::GetSavedReference(ID id)
	{
		std::weak_ptr<T> weakPointer;
		auto [success, sharedPointer] = GetSavedReferenceInternal(id, typeid(T));
		if (success)
			weakPointer = std::static_pointer_cast<T>(*sharedPointer);
		else
			weakPointer = {};
		return { id, std::move(weakPointer), typeid(T) };
	}

	template<typename T>
	SavedReference<T> IDSystem::GetSavedReference(T* rawPointer)
	{
		// somewhat safe
		return GetSavedReferenceInternal(rawPointer, typeid(*rawPointer)).template UnsafeCast<T>();
	}

	template<typename T>
	SavedReference<T> IDSystem::Add()
	{
		std::shared_ptr<void> voidPointer = std::make_shared<T>();
		auto[id,outVoidPointer] = AddInternal(voidPointer, typeid(T));
		// you must bind the weak_ptr to the shared_ptr in the
		return { id, std::static_pointer_cast<T>(outVoidPointer), typeid(T) };
	}

	template<typename T>
	bool IDSystem::Remove(SavedReference<T>& savedReference)
	{
		return RemoveInternal(savedReference.id, savedReference.typeIndex);
	}

	template<typename T>
	void IDSystem::Overwrite(ID id, T&& moveFrom)
	{
		SavedReference<T> savedReference = GetSavedReference<T>(id);
		if (savedReference)
		{
			*savedReference.DeRef().get() = std::move(moveFrom); // copy operator
		}
		else
		{
			std::shared_ptr<T> sharedPointer = std::make_shared<T>();
			*sharedPointer.get() = std::move(moveFrom); // copy operator
			std::shared_ptr<void> voidPointer = std::static_pointer_cast<void>(std::move(sharedPointer));
			AddInternal(id, voidPointer, typeid(moveFrom));
		}
	}
}