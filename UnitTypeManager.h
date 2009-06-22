#ifndef UNITTYPEMANAGER_H
#define UNITTYPEMANAGER_H

#include <map>
#include <string>
#include "Unit.h"

namespace Core
{
	class UnitTypeManager
	{
		private:
			UnitTypeManager();

			static UnitTypeManager* instance;

			std::map<std::string, Unit*> unitTypes;

			bool loadUnitType(std::string typeName);
		public:
			static UnitTypeManager* getInstance();

			/**
			 * Returns type with a given typename
			 * @param typename Type name
			 * @return Prototype for a unit
			 * NOTE: Unlike AnimationManager, we
			 * do not need to create copy of type.
			 * Returned Unit type is used in constructor
			 */
			Unit* getType(std::string typeName);
	};
}

#endif //UNITTYPEMANAGER_H
