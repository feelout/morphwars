#include "StateStack.h"

using namespace Core;

void StateStack::push(EngineState *state)
{
	if(top())
		top()->pause();

	states.push(state);
	state->resume();
	//TODO: Start state, pause other. Maybe pause should set draw only in needing subclasses...
}

EngineState* StateStack::top() const
{
	if(states.empty())
		return NULL;
	return states.top();
}

EngineState* StateStack::pop()
{
	EngineState* result = top();
	if(result)
	{
		states.pop();
		if(top())
			top()->resume();
	}

	return result;
}
