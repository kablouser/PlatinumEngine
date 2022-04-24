#pragma once
#include <typeindex>
#include <memory>

namespace PlatinumEngine
{
	//-----------------------------------------------------------------------
	// Collection interface
	//-----------------------------------------------------------------------

	class Iterator
	{
	public:

		// allow inherited classes to be destroyed
		virtual ~Iterator() = default;

		// interface
		virtual bool IsIterating() = 0;

		virtual void MoveNext() = 0;

		virtual void* GetCurrent() = 0;

		virtual void RemoveCurrent() = 0;
	};

	class Collection
	{
	public:
		// this is the type of the elements inside this collection
		// not the type of the collection
		std::type_index elementTypeIndex;

		explicit Collection(const std::type_index& newElementTypeIndex) : elementTypeIndex(newElementTypeIndex)
		{
		}

		virtual bool Add(void* collectionInstance, void* elementInstance) = 0;

		virtual std::unique_ptr<Iterator> GetIterator(void* collectionInstance) = 0;
	};
}