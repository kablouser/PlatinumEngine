#pragma once
#include <Serialization/Collection.h>
#include <vector>
#include <Helpers/VectorHelpers.h>

namespace PlatinumEngine
{
//-----------------------------------------------------------------------
// Vector support
//-----------------------------------------------------------------------

	template<typename T>
	class VectorIterator : public Iterator
	{
	public:

		explicit VectorIterator(std::vector<T>* toIterateOver) :
				_toIterateOver(toIterateOver),
				_currentIndex(0)
		{
		}

		bool IsIterating() override
		{
			return _currentIndex < _toIterateOver->size();
		}

		void MoveNext() override
		{
			++_currentIndex;
		}

		void* GetCurrent() override
		{
			return (void*)&_toIterateOver->at(_currentIndex);
		}

		void RemoveCurrent() override
		{
			if (_currentIndex < _toIterateOver->size())
				_toIterateOver->erase(_toIterateOver->begin()+_currentIndex);
		}

	private:

		std::vector<T>* _toIterateOver;
		size_t _currentIndex;
	};

	template<typename ElementType>
	class VectorCollection : public Collection
	{
	public:
		VectorCollection() : Collection(std::type_index(typeid(ElementType)))
		{
		}

		bool Add(void* collectionInstance, void* elementInstance) override
		{
			((std::vector<ElementType>*)collectionInstance)->push_back(*(ElementType*)elementInstance);
			return true;
		}

		std::unique_ptr<Iterator> GetIterator(void* collectionInstance) override
		{
			return std::make_unique<VectorIterator<ElementType>>((std::vector<ElementType>*)collectionInstance);
		}
	};
}