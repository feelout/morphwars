#ifndef PYTHONGLUE_H
#define PYTHONGLUE_H

#include <boost/python.hpp>
#include <string>

namespace Utility
{
	boost::python::object* GetMainPythonNamespace();
	void InitPythonGlueCode();
	void RunPythonScript(std::string path);
}

#endif //PYTHONGLUE_H
