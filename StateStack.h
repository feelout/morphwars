#ifndef STATE_STACK_H
#define STATE_STACK_H

#include <stack>
#include "EngineState.h"

namespace Core
{
	class StateStack
	{
		private:
			std::stack<EngineState*> states;
		public:
			void push(EngineState *state);
			EngineState* top() const;
			EngineState* pop();
	};
}

#endif //STATE_STACK_H
