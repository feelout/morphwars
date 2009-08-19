#ifndef SURFACE_H
#define SURFACE_H

#include <SDL/SDL.h>
#include <string>
#include <vector>
#include "assist.h"

namespace Graphics
{
	/**
	 * Represents an image
	 */
	class Surface
	{
		private:
			SDL_Surface *surface;

			SDL_Surface* createEmptySurface(int width, int height);
		public:
			/**
			 * Creates a surface from given SDL_Surface
			 * @param surface Source surface
			 */
			Surface(SDL_Surface *surface);
			/**
			 * Creates a surface with given dimensions
			 * @param width Width
			 * @param height Height
			 */
			Surface(int width, int hieght);
			/**
			 * Loads a surface from file
			 * @param filename File path
			 */
			Surface(std::string filename);

			Surface(const Surface& other);
			/**
			 * Frees associated SDL_Surface
			 */
			~Surface();

			int getWidth() const;
			int getHeight() const;

			/**
			 * Returns internal SDL_Surface*
			 */
			SDL_Surface *getSurface() const;

			/**
			 * Blits whole surface to target
			 * @param target Surface to blit to
			 * @param x x coord of blit point
			 * @param y y coord of blit point
			 */
			void blit(Surface *target, int x, int y) const;
			/**
			 * Blits part of surface to target
			 * @param target Surface to blit to
			 * @param clip Whic part of source to blit
			 * @param x x coord of blit point
			 * @param y y coord of blit point
			 */
			void blit(Surface *target, Rect *clip, int x, int y) const;

			Surface& operator=(const Surface& other); //Copy surface, not pointer

			/**
			 * Splits surface into frames
			 * @param strip Source surface
			 * @param frameWidth width of each frame
			 * @param frameHeight hieght of each frame
			 * @return Collection of frames
			 */
			static std::vector<Surface*> splitSpriteStrip(Surface *strip, int frameWidth, int frameHeight);
			/**
			 * Creates darkened version of the surface
			 * @param src Surface to darken
			 * @param factor Darkening factor
			 * @return Darkened copy of src
			 */
			// ACHTUNG : Do not darken color key!!!
			static Surface *createShadowedSurface(Surface *src, float factor);
	};
}

#endif //SURFACE_H
