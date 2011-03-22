#include <dirent.h>
#include "assist.h"
#include "Logger.h"

const RGBColor RGBColor::RED = RGBColor(255, 0, 0);
const RGBColor RGBColor::YELLOW = RGBColor(255, 255, 0);
const RGBColor RGBColor::BLUE = RGBColor(0, 0, 255);
const RGBColor RGBColor::GREEN = RGBColor(0, 255, 0);
const RGBColor RGBColor::WHITE = RGBColor(255, 255, 255);
const RGBColor RGBColor::BLACK = RGBColor(0, 0, 0);

SDL_Surface *createSurface(int w, int h)
{
	Uint32 rmask, gmask, bmask, amask;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	rmask = 0xff000000;
	gmask = 0x00ff0000;
	bmask = 0x0000ff00;
	amask = 0x000000ff;
#else
	rmask = 0x000000ff;
	gmask = 0x0000ff00;
	bmask = 0x00ff0000;
	amask = 0xff000000;
#endif
	return SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_SRCCOLORKEY, w , h, rmask, 32, gmask, bmask, amask);
}

std::vector<std::string> ListFilesInFolder(std::string dirname)
{
	std::vector<std::string> result;

	DIR *dp;
	dirent *dirp;
	
	if( (dp = opendir(dirname.c_str())) == NULL )
	{
		Utility::Logger::getInstance()->log("Failed to open folder %s\n", dirname.c_str());
		return result;
	}

	while( (dirp = readdir(dp)) != NULL)
	{
		// Remove '.' and '..'
		if(dirp->d_name[0] == '.') continue;

		result.push_back(std::string(dirp->d_name));
	}

	closedir(dp);
	return result;
}

//version made by evilskater @ CC Grad @ UFRN (University of Rio Grande do Norte) - BRAZIL
//based in -> http://www.freebookzone.com/others/itoa.h <- this version works... but have memory leaks.
//let's try a new one!
std::string br_itoa(int n)
{
	char * s = new char[17];
	std::string u;

	if (n < 0) { //turns n positive
		n = (-1 * n); 
		u = "-"; //adds '-' on result string
	}

	int i=0; //s counter

	do {
		s[i++]= n%10 + '0'; //conversion of each digit of n to char
		n -= n%10; //update n value
	}

	while ((n /= 10) > 0);

	for (int j = i-1; j >= 0; j--) { 
		u += s[j]; //building our string number
	}

	delete[] s; //free-up the memory!
	return u;
}
