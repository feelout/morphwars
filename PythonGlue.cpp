#include <stdio.h>
#include "PythonGlue.h"
#include "Unit.h"
#include "MapObject.h"
#include "Tile.h"
#include "Player.h"
#include "Map.h"
#include "Logger.h"
#include "Order.h"
#include "Widget.h"

using namespace boost::python;
using namespace Core;
using Utility::Logger;

object main_namespace;

boost::python::object* Utility::GetMainPythonNamespace()
{
	return &main_namespace;
}

class OrderWrapper : public Core::Order, public wrapper<Order>
{
	public:
		OrderWrapper(const Order& order)
			: Order(order)
		{
		}

		OrderWrapper(MapObject *unit, Map * map)
			: Order(unit, map)
		{
		}

		virtual void execute(Tile *target)
		{
			this->get_override("execute")(target);
		}

		virtual void process()
		{
			this->get_override("process")();
		}

};

class IOrderCreatorWrapper : public IOrderCreator, public wrapper<IOrderCreator>
{
	public:
		IOrderCreatorWrapper()
		{
		}

		IOrderCreatorWrapper(const IOrderCreator& other)
		{
		}

		virtual boost::shared_ptr<Order> createOrder(MapObject *object, Map *map)
		{
			return this->get_override("createOrder")(object, map);
		}
};

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

	WriteToLog("Binding Map\n");
	main_namespace["Map"] = class_<Map>("Map", init<Rect, int, int, std::string, Gui::Widget*>())
						.def("getWidth", &Map::getWidth)
						.def("getHeight", &Map::getHeight)
						.def("getTile", &Map::getTile,
								return_value_policy<reference_existing_object>());

	WriteToLog("Binding MapObjectType\n");
	/* MapObjectType is abstract - no idea what will it become */
	main_namespace["MapObjectType"] = class_<MapObjectType, boost::noncopyable>("MapObjectType", no_init)
						.def("getName", &MapObjectType::getName);
	//main_namespace["MapObject"] = class_<MapObject, boost::noncopyable>("MapObject", init<MapObjectType*, Tile*, Player*>())
	
	WriteToLog("Binding MapObject\n");
	main_namespace["MapObject"] = class_<MapObject >("MapObject", no_init)
						.def("getTile", &MapObject::getTile,
								return_value_policy<reference_existing_object>())
						.def("getOwner", &MapObject::getOwner,
								return_value_policy<reference_existing_object>())
						.def("getType", &MapObject::getType,
								return_value_policy<reference_existing_object>())
						.def("isEnemy", &MapObject::isEnemy)
						.def("damage", &MapObject::damage)
						.def("isDead", &MapObject::isDead);
	WriteToLog("Binding Unit\n");
	main_namespace["Unit"] = class_<Unit, bases<MapObject> >("Unit", no_init)
						.def("getHP", &Unit::getHP)
						.def("getSP", &Unit::getSP)
						.def("getMP", &Unit::getMP)
						.def("getHitsLeft", &Unit::getHitsLeft)
						.def("setHP", &Unit::setHP)
						.def("setSP", &Unit::setSP)
						.def("setMP", &Unit::setMP)
						.def("changePosition", &Unit::changePosition);	
	WriteToLog("Binding Order\n");
	main_namespace["Order"] = class_<OrderWrapper>("Order", init<MapObject*, Map*>())
						.def("execute", &OrderWrapper::execute)
						.def("process", &OrderWrapper::process)
						.def("stop", &Order::stop)
						.def("isDone", &Order::isDone);

	WriteToLog("Binding IOrderCreator\n");
	main_namespace["IOrderCreator"] = class_<IOrderCreatorWrapper>("IOrderCreator")
						.def("createOrder", &IOrderCreatorWrapper::createOrder);//,
								//return_value_policy<reference_existing_object>());

	WriteToLog("Binding OrderFactory\n");
	main_namespace["OrderFactory"] = class_<OrderFactory>("OrderFactory")
						.def("getInstance", &OrderFactory::getInstance,
								return_value_policy<reference_existing_object>())
						.staticmethod("getInstance")
						.def("createOrder", &OrderFactory::createOrder)
								//return_value_policy<reference_existing_object>())
						.def("registerCreator", &OrderFactory::registerCreator);
}

void Utility::RunPythonScript(std::string path)
{
	FILE* script = fopen(path.c_str(),"r");
	if(!script)
		Utility::Logger::getInstance()->log("Unable to open script file %s\n", path.c_str());
	if(PyRun_SimpleFile(script, path.c_str()) == -1)
		Utility::Logger::getInstance()->log("Couldn`t run script from file %s\n", path.c_str());
	fclose(script);
}
