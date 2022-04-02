#pragma once

// for size_t
#include <cstddef>
// for exception
#include <stdexcept>

namespace PlatinumEngine
{
	/**
	 * Jörmungandr, Midgard Wyrm
	 */
	template<typename T, size_t MAX_SIZE>
	class CircularBuffer
	{
	public:

		CircularBuffer() :
				_next(0),
				_isMaxed(false),
				// set elements in buffer to 0
				_buffer()
		{
		}

		CircularBuffer(CircularBuffer&) = delete;

		CircularBuffer(CircularBuffer&&) = delete;

		CircularBuffer<T, MAX_SIZE>& operator=(CircularBuffer<T, MAX_SIZE>&) = delete;

		CircularBuffer<T, MAX_SIZE>& operator=(CircularBuffer<T, MAX_SIZE>&&) = delete;

		size_t Size()
		{
			if (_isMaxed)
				return MAX_SIZE;
			else
				return _next;
		}

		void Add(T element)
		{
			_buffer[_next] = element;
			++_next;
			if (MAX_SIZE <= _next)
			{
				_next -= MAX_SIZE;
				_isMaxed = true;
			}
		}

		T& GetFromStart(size_t offsetFromStart)
		{
			if (Size() <= offsetFromStart)
				throw std::out_of_range("offsetFromHead is too large");

			if (_isMaxed)
			{
				size_t returnIndex = _next + offsetFromStart;
				if (returnIndex < MAX_SIZE)
					return _buffer[returnIndex];
				else
					return _buffer[returnIndex - MAX_SIZE];
			}
			else
			{
				return _buffer[offsetFromStart];
			}
		}

		T& GetFromEnd(size_t offsetFromEnd)
		{
			if (Size() <= offsetFromEnd)
				throw std::out_of_range("offsetFromTail is too large");

			size_t returnIndex = _next - offsetFromEnd - 1;
			if (returnIndex < _next)
				return _buffer[returnIndex];
			else
				return _buffer[returnIndex + MAX_SIZE];
		}

	private:

		/**
		 * next index to insert element
		 */
		size_t _next;
		bool _isMaxed;
		T _buffer[MAX_SIZE];
	};
}