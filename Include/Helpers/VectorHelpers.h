#pragma once

#include <vector>
#include <algorithm>

namespace PlatinumEngine
{
	namespace VectorHelpers
	{
		template<typename T>
		bool RemoveFirst(std::vector<T>& vector, T elementToRemove)
		{
			auto iterator = std::find(vector.begin(), vector.end(), elementToRemove);
			if (iterator != vector.end())
			{
				vector.erase(iterator);
				return true;
			}
			return false;
		}
	};
}