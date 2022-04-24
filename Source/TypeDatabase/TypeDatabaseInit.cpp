#include <TypeDatabase/TypeDatabase.h>
#include <IDSystem/IDSystem.h>

void PlatinumEngine::TypeDatabase::Init()
{
	auto idSystem = BeginTypeInfo<IDSystem>();
	auto& idSystemTypeInfo = _typeInfos.at(idSystem.typeInfoIndex);
	idSystemTypeInfo.streamIn = IDSystem::StreamIn;
	idSystemTypeInfo.streamOut = IDSystem::StreamOut;
}