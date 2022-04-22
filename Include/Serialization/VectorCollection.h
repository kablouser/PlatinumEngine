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

		explicit VectorIterator(const std::vector<T>* toIterateOver) :
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

	private:

		const std::vector<T>* _toIterateOver;
		size_t _currentIndex;
	};

	template<typename T>
	class VectorCollection : public Collection
	{
	public:
		VectorCollection() : Collection(std::type_index(typeid(T)))
		{
		}

		bool Add(void* collectionInstance, void* elementInstance) override
		{
			((std::vector<T>*)collectionInstance)->push_back(*(T*)elementInstance);
			return true;
		}

		bool RemoveFirst(void* collectionInstance, void* elementInstance) override
		{
			return PlatinumEngine::VectorHelpers::RemoveFirst(
					*(std::vector<T>*)collectionInstance,
					*(T*)elementInstance);
		}

		std::unique_ptr<Iterator> GetIterator(const void* collectionInstance) override
		{
			return std::make_unique<VectorIterator<T>>((const std::vector<T>*)collectionInstance);
		}
	};
}