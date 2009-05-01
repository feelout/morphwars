/*
   Tileset.h - propably not needed. Do not commit to git
 */
#ifndef TILESET_H
#define TILESET_H

#include "Tile.h"
#include <vector>
#include <string>

/**
 * TileSet servs as tile types palette
 */
class TileSet
{
	private:
		/**
		 * All types in tileset
		 */
		std::vector<TileType*> types;

		/**
		 * Load tileset from a folder
		 * @param name name of tileset
		 */
		bool loadTileSet(std::string name);
	public:
		/**
		 * Creates tileset with given name. Loader looks into folder
		 * "Tilesets/$name", reads definition file and fills palette
		 * @param name name of tileset
		 */
		TileSet(std::string name);
		/**
		 * Destroys all held types
		 */
		~TileSet();

		TileType *getType(int n);
};

#endif //TILESET_H
