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

		CircularBuffer(const CircularBuffer<T, MAX_SIZE>& copyFrom) :
				_next(copyFrom._next),
				_isMaxed(copyFrom._isMaxed)
		{
			for (size_t i = 0; i < MAX_SIZE; ++i)
				_buffer[i] = copyFrom._buffer[i];
		}

		CircularBuffer(CircularBuffer<T, MAX_SIZE>&& moveFrom) noexcept:
				_next(moveFrom._next),
				_isMaxed(moveFrom._isMaxed)
		{
			moveFrom._next = 0;
			moveFrom._isMaxed = false;
			for (size_t i = 0; i < MAX_SIZE; ++i)
			{
				_buffer[i] = moveFrom._buffer[i];
				moveFrom._buffer[i] = {};
			}
		}

		CircularBuffer<T, MAX_SIZE>& operator=(const CircularBuffer<T, MAX_SIZE>& copyFrom)
		{
			if (&copyFrom == this)
				return *this;

			_next = copyFrom._next;
			_isMaxed = copyFrom._isMaxed;
			for (size_t i = 0; i < MAX_SIZE; ++i)
				_buffer[i] = copyFrom._buffer[i];

			return *this;
		}

		CircularBuffer<T, MAX_SIZE>& operator=(CircularBuffer<T, MAX_SIZE>&& moveFrom)
		{
			if (&moveFrom == this)
				return *this;

			_next = moveFrom._next;
			_isMaxed = moveFrom._isMaxed;
			moveFrom._next = 0;
			moveFrom._isMaxed = false;
			for (size_t i = 0; i < MAX_SIZE; ++i)
			{
				_buffer[i] = moveFrom._buffer[i];
				moveFrom._buffer[i] = {};
			}

			return *this;
		}

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