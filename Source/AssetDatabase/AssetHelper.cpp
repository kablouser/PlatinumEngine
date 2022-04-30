#include "AssetDatabase/AssetHelper.h"
#include <Logger/Logger.h>

namespace PlatinumEngine
{
	AssetHelper::AssetHelper(AssetDatabase* assetDatabase, IDSystem& idSystem):
			_assetDatabase(assetDatabase),
			_idSystem(idSystem)
	{
	}
}
