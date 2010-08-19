#ifndef PYTHONGLUE_H
#define PYTHONGLUE_H

#include <boost/python.hpp>

namespace Utility
{
	boost::python::object* GetMainPythonNamespace();
	void InitPythonGlueCode();
}

#endif //PYTHONGLUE_H
