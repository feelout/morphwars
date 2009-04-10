#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <string>

/**
 * Singleton logging class.
 * All log message are written to file or stdout
 */
class Logger
{
	private:
		/**
		 * The only instance
		 */
		static Logger *instance;

		/**
		 * Opened log file name
		 */
		std::string filename;
		/**
		 * Log file
		 */
		FILE *file;

		/**
		 * Private constructor - opens log file
		 * @param filename Log file name
		 */
		Logger(std::string filename);
	public:
		/**
		 * Returns or creates singleton instance
		 */
		static Logger* getInstance();

		/**
		 * Logs message in printf style
		 * @param fmt printf-style format
		 */
		void log(const char* fmt,...);
		/**
		 * Closes log file and deletes singleton
		 */
		void close();
};

#endif //LOGGER_H
