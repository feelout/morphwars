#include "UnitTypeManager.h"
#include "Logger.h"

using namespace Core;

UnitTypeManager* UnitTypeManager::instance=NULL;

UnitTypeManager::UnitTypeManager()
{
}

UnitTypeManager* UnitTypeManager::getInstance()
{
	if(!instance)
	{
		instance = new UnitTypeManager();
	}
	return instance;
}

UnitType* UnitTypeManager::getType(std::string typeName)
{
	Utility::Logger::getInstance()->log("UnitTypeManager::getType(%s)\n", typeName.c_str());

	if(!unitTypes[typeName])
	{
		Utility::Logger::getInstance()->log("\tNot found in collection\n");
		unitTypes[typeName] = new UnitType(typeName);
	}
	
	return unitTypes[typeName];
}
