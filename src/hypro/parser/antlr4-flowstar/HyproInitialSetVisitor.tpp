#include "HyproInitialSetVisitor.h"

namespace hypro {

	//Constructor & Destructor
	template<typename Number>
	HyproInitialSetVisitor<Number>::HyproInitialSetVisitor(std::vector<std::string>& varVec, std::set<Location<Number>*>& lSet) : 
		vars(varVec),
		locSet(lSet)
	{ }

	template<typename Number>
	HyproInitialSetVisitor<Number>::~HyproInitialSetVisitor() { }

	//Inherited
	template<typename Number>
	antlrcpp::Any HyproInitialSetVisitor<Number>::visitInit(HybridAutomatonParser::InitContext *ctx){

		std::cout << "-- Bin bei visitInitial!" << std::endl;

		//0.Syntax Check - given variable name really a location name?
		//If it is, save the location pointer for the localStateMap
		bool found = false;
		Location<Number>* initialLoc;
		for(const auto& loc : locSet){
			if(ctx->initstate()->VARIABLE()->getText() == loc->getName()){
				found = true;
				initialLoc = loc;
			}
		}
		if(!found){
			std::cerr << "ERROR: Given location name in init does not exist." << std::endl;
		}

		//1.Get ConstraintSet, build State and return localStateMap
		ConstraintSet<Number> conSet = visit(ctx->initstate());
		std::cout << "---- Hab ein ConstraintSet mitgebracht!" << std::endl;
		std::cout << "---- matrix:\n" << conSet.matrix() << "and vector:\n" << conSet.vector() << std::endl;
		State<Number,ConstraintSet<Number>> state;
		std::cout << "---- state deklariert" << std::endl;
		state.setLocation(initialLoc);
		std::cout << "---- location gesettet" << std::endl;
		//std::vector<boost::variant<ConstraintSet<Number>>> mSet;
		std::cout << "---- Made a vecter of sets" << std::endl;
		//mSet.push_back(conSet);
		std::cout << "---- pushed conSet inside" << std::endl;
		state.setSet(conSet,0);
		assert(state.getNumberSets() == 1);
		std::cout << "---- State gebaut!" << std::endl;
		std::cout << state << std::endl;
		locationStateMap initialState;
		initialState.insert(std::make_pair(initialLoc, state));
		return initialState;
	}

	template<typename Number>
  	antlrcpp::Any HyproInitialSetVisitor<Number>::visitInitstate(HybridAutomatonParser::InitstateContext *ctx){

  		std::cout << "-- Bin bei visitInitstate!" << std::endl;

  		//0.Check if there is a constraint for every stated variable
  		//We get the the assigned text via the start and stop indices given by ctx.
  		//In the text we count the occurrences of every variable name and check if every variable occurs
  		//at least one time.
  		std::vector<unsigned> varCount;
  		for(unsigned i=0; i < vars.size(); i++){
  			varCount.push_back(0);
  		}
  		std::size_t startIndex = ctx->start->getStartIndex();
  		std::size_t endIndex = ctx->stop->getStopIndex();
  		misc::Interval inter(startIndex, endIndex);
  		CharStream* input = ctx->start->getInputStream();
  		std::string text = input->getText(inter);
  		for(unsigned i=0; i < vars.size(); i++){
  			unsigned pos = text.find(vars[i]);
  			if(pos != std::string::npos){
  				varCount[i]++;
  				text = text.erase(pos, vars[i].size());
  			}
  		}
  		for(unsigned i=0; i < varCount.size(); i++){
  			if(varCount[i] == 0){
  				std::cerr << "ERROR: For variable " << vars[i] << " no initial constraint has been set." << std::endl;
  			}
  		}

  		//1.Call HyproFormulaVisitor and get pair of matrix and vector
		HyproFormulaVisitor<Number> visitor(vars);
		std::pair<matrix_t<Number>,vector_t<Number>> result = visitor.visit(ctx->constrset());

		//2.Build ConstraintSet and return it
		return ConstraintSet<Number>(result.first, result.second);
  	}
}