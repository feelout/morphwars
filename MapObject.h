#ifndef MAPOBJECT_H
#define MAPOBJECT_H

#include "Tile.h"
#include "Drawer.h"

namespace Core
{
    class MapObject
    {
        protected:
            Tile *tile;
        public:
            MapObject(Tile *tile);
            virtual ~MapObject();

            virtual void draw(Drawer *target);
    };
}

#endif // MAPOBJECT_H
