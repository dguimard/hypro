#pragma once
#include "../../../../datastructures/HybridAutomaton/State.h"
#include "../../../../datastructures/HybridAutomaton/flow/rectangular.h"
#include "../../../../datastructures/HybridAutomaton/flow/rectangularOperations.h"
#include "../IHandler.h"

#include <sstream>
#include <string>

namespace hypro {
template <typename State>
class rectangularTimeEvolutionHandler : public ITimeEvolutionHandler {
	using Number = typename State::NumberType;

  protected:
	State* mState;  // A state containing the first segment for each subspace
	rectangularFlow<Number> mFlow;
	bool mComputed = false;
	size_t mIndex;
	bool mMarkedForDelete = false;

  public:
	rectangularTimeEvolutionHandler() = delete;
	rectangularTimeEvolutionHandler( State* state, size_t index, const rectangularFlow<Number>& flow )
		: mState( state )
		, mFlow( flow )
		, mIndex( index ) {}

	void handle();
	const char* handlerName() { return "rectangularTimeEvolutionHandler"; }
	void setMarkedForDelete( bool del ) { mMarkedForDelete = del; }
	bool getMarkedForDelete() { return mMarkedForDelete; }
};
}  // namespace hypro

#include "rectangularTimeEvolutionHandler.tpp"
