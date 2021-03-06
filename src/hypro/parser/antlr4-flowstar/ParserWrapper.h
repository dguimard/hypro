#pragma once

#include "ErrorListener.h"
#include "HybridAutomatonLexer.h"
#include "HybridAutomatonParser.h"
#include "HyproHAVisitor.h"
#include <boost/tuple/tuple.hpp>

//#include <unistd.h>		//getcwd()

namespace hypro {

	template<typename Number>
	//boost::tuple<HybridAutomaton<Number,State_t<Number,Number>>, ReachabilitySettings> parseFlowstarFile(const std::string& filename);
	std::pair<HybridAutomaton<Number>, ReachabilitySettings> parseFlowstarFile(const std::string& filename);

} // namespace
