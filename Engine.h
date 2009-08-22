#ifndef ENGINE_H
#define ENGINE_H

#include <SDL/SDL.h>
#include "EventDispatcher.h"
#include "FPSEqualizer.h"
#include "Scenario.h"
#include "Button.h"
#include "Renderer.h"
#include "StateStack.h"
#include "Menu.h"

/**
 * Engine coordinates all subsystems and game cycle
 */
class Engine
{
	private:
		/**
		 * The only singleton instance
		 */
		static Engine *instance;

		/**
		 * Stops game cycle after iteration
		 */
		bool quit;

		Graphics::Renderer *renderer;
		EventDispatcher *dispatcher;
		Utility::FPSEqualizer *equalizer;
		Core::Scenario *currentScenario;
		Core::StateStack *stateStack;

		Core::MenuState *menu; //TODO : maybe move out of class definition

		/**
		 * Constructs window and initializes subsystems
		 * @param w Width of window
		 * @param h Height of window
		 * @param fs Fullscreen mode
		 */
		Engine(int w, int h, bool fs);
		/**
		 * Destructor - performs cleanup
		 */
		~Engine(); //cleanup
	public:
		/**
		 * Initialize Engine with given parametres
		 * @param width Width of window
		 * @param height Height of window
		 * @param fullscreen Fullscreen mode
		 */
		static void init(int width, int height, bool fullscreen);
		/**
		 * Returns singleton instance
		 */
		static Engine* getInstance();

		/**
		 * Starts game cycle
		 */
		void runGameCycle();
		/**
		 * Stops engine
		 */
		void stop();

		//DEBUG
		bool loadScenario(std::string path);
		void changeState(Core::EngineState *newState);
		Graphics::Renderer* getRenderer() const;
};

#endif // ENGINE_H
