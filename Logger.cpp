#include "Logger.h"
#include <stdarg.h>
#include <SDL/SDL.h>

using namespace Utility;

Logger* Logger::instance = NULL;

const std::string DEFAULT_LOG_NAME = "log.txt";

Logger* Logger::getInstance()
{
	if(!instance)
	{
		instance = new Logger(DEFAULT_LOG_NAME);
	}

	return instance;
}

Logger::Logger(std::string filename)
	: filename(filename)
{
	// TODO: Mb changed later to w+, not
	// to produce 2gB logs
	file = fopen(filename.c_str(),"w+");

	if(!file)
	{
		fprintf(stderr, "Couldn`t open logfile %s!\n",
				DEFAULT_LOG_NAME.c_str());
		file = stdout; // Redirection to stdout
	}
	log("Log started on %i\n", SDL_GetTicks());
}

void Logger::log(const char* fmt, ...)
{
	va_list args;
	va_start(args,fmt);
	vfprintf(file, fmt, args);
	va_end(args);

	fflush(file);
}

void Logger::close()
{
	log("Log closed on %i\n\n", SDL_GetTicks());
	fclose(file);
	delete instance;
	//maybe add instance = NULL
}
