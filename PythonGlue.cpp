#include "PythonGlue.h"
#include "Unit.h"
#include "MapObject.h"
#include "Tile.h"
#include "Player.h"
#include "Map.h"
#include "Logger.h"

using namespace boost::python;
using namespace Core;
using Utility::Logger;

object main_namespace;

boost::python::object* Utility::GetMainPythonNamespace()
{
	return &main_namespace;
}


BOOST_PYTHON_MODULE(enums) // XXX Find out what to do with it
{
	/*main_namespace["direction"] = */enum_<Direction>("Direction")
		.value("CENTER", CENTER)
		  .value("NORTHWEST", NORTHWEST)
		  .value("NORTH", NORTH)
		  .value("NORTHEAST", NORTHEAST)
		  .value("EAST", EAST)
		  .value("SOUTHEAST", SOUTHEAST)
		  .value("SOUTH", SOUTH)
		  .value("SOUTHWEST", SOUTHWEST)
		.value("west", WEST);
}

void Utility::InitPythonGlueCode()
{
	WriteToLog("Initializing python bindings\n");
	Py_Initialize();

	object main_module((handle<>(borrowed(PyImport_AddModule("__main__")))));
	main_namespace = main_module.attr("__dict__");	

	WriteToLog("Binding Direction\n");

	//FIXME : Something wrong with this declaration


	initenums();

	Direction (Tile::*getDirFromTile)(Tile*) const = &Tile::getDirection;

	WriteToLog("Binding Tile\n");

	main_namespace["Tile"] = class_<Tile>("Tile", no_init)
						.def("getDirection", getDirFromTile)
						.def("getDistance", &Tile::getDistance)
						.def("canBeAdded", &Tile::canBeAdded)
						.def("addObject", &Tile::addObject)
						.def("removeObject", &Tile::removeObject)
						.def("getTopObject", &Tile::getTopObject,
								return_value_policy<reference_existing_object>())
						.def("isEnemy", &Tile::isEnemy)
						.def("getX", &Tile::getX)
						.def("getY", &Tile::getY)
						.def("getHeight", &Tile::getHeight)
						.def("isCliff", &Tile::isCliff)
						.def("getCliffDirection", &Tile::getCliffDirection);

	WriteToLog("Binding MapObjectType\n");
	/* MapObjectType is abstract - no idea what will it become */
	main_namespace["MapObjectType"] = class_<MapObjectType, boost::noncopyable>("MapObjectType", no_init)
						.def("getName", &MapObjectType::getName);
	//main_namespace["MapObject"] = class_<MapObject, boost::noncopyable>("MapObject", init<MapObjectType*, Tile*, Player*>())
	
	WriteToLog("Binding MapObject\n");
	main_namespace["MapObject"] = class_<MapObject, boost::noncopyable>("MapObject", no_init)
						.def("getTile", &MapObject::getType,
								return_value_policy<reference_existing_object>())
						.def("getOwner", &MapObject::getOwner,
								return_value_policy<reference_existing_object>())
						.def("getType", &MapObject::getType,
								return_value_policy<reference_existing_object>())
						.def("isEnemy", &MapObject::isEnemy)
						.def("damage", &MapObject::damage)
						.def("isDead", &MapObject::isDead);
}
