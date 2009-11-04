/**
 * LoggerTest.cpp - testing logging system
 */
#include <stdio.h>
#include <string.h>
#include "../Logger.h"

using namespace Utility;

int main()
{
	Logger::getInstance()->log("String = %s\n", "String");
	Logger::getInstance()->log("54 = %i\n", 54);
	Logger::getInstance()->log("26.5 = %f\n", 26.5);

	Logger::getInstance()->close();

	FILE *f_test = fopen("log.txt", "r");

	char strtest[8];
	int inttest;
	float floattest;

	fscanf(f_test, "String = %s\n", &strtest);
	fscanf(f_test, "54 = %i\n", &inttest);
	fscanf(f_test, "26.5 = %f\n", &floattest);

	fclose(f_test);

	if(strcmp("String", strtest) != 0)
	{
		printf("String test failed!\n");
		return -1;
	}

	if(54 != inttest)
	{
		printf("Integer test failed!\n");
		return -1;
	}

	if(26.5 != floattest)
	{
		printf("Float test failed\n");
		return -1;
	}

	printf("All tests succeded\n");
	return 0;
}
