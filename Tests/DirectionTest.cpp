#include <utility>

enum Direction
	{ 
		CENTER = 0,
		NORTHWEST,
		NORTH,
		NORTHEAST,
		EAST,
		SOUTHEAST,
		SOUTH,
		SOUTHWEST,
		WEST,
	};

Direction getDirection(std::pair<int, int> src, std::pair<int, int> dst)
{
	int dx = dst.first - src.first;
	int dy = dst.second - src.second;

	bool src_shifted = (src.second % 2) == 1;
	bool dst_shifted = (dst.second % 2) == 1;

	if(dy == 0)
	{
		if(dx > 0) return EAST;
		else if(dx < 0) return WEST;
		else return CENTER;
	}

	if(src_shifted == dst_shifted)
	{
		if(dy > 0)
		{
			if(dx > 0) return SOUTHEAST;
			else if(dx < 0) return SOUTHWEST;
			else return SOUTH;
		}
		else
		{
			if(dx > 0) return NORTHEAST;
			else if(dx < 0) return NORTHWEST;
			else return NORTH;
		}
	}
	else
	{
		if(dy > 0)
		{
			if(!src_shifted)
			{
				if(dx < 0) return SOUTHWEST;
				else return SOUTHEAST;
			}
			else
			{
				if(dx > 0) return SOUTHEAST;
				else return SOUTHWEST;
			}
		}
		else
		{
			if(!src_shifted)
			{
				if(dx < 0) return NORTHWEST;
				else return NORTHEAST;
			}
			else
			{
				if(dx > 0) return NORTHEAST;
				else return NORTHWEST;
			}
		}
	}
}

int main(int argc, char *arvg[])
{
}
