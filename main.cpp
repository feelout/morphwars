#include "Engine.h"
#include <boost/python/errors.hpp>

int main(int argc, char *argv[])
{
	try
	{
		Engine::init(800, 640, false);
		Engine::getInstance()->runGameCycle();
	}
	catch(...)
	{
		boost::python::handle_exception();
	}

	return 0;
}
