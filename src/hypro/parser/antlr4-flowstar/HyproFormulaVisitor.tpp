#include "HyproFormulaVisitor.h"

namespace hypro {

	////////////// Constructor and Destructor

	template<typename Number>
	HyproFormulaVisitor<Number>::HyproFormulaVisitor(std::vector<std::string> varVec) :
		vars(varVec)
	{ }

	template<typename Number>
	HyproFormulaVisitor<Number>::~HyproFormulaVisitor() { }

	///////////// Helping functions

	template<typename Number>
	Number HyproFormulaVisitor<Number>::stringToNumber(std::string string){
		double numInFloat = std::stod(string);
		Number numInNumber = Number(numInFloat);
		return numInNumber;
	}

	template<typename Number>
	Number HyproFormulaVisitor<Number>::multTogether(HybridAutomatonParser::TermContext* ctx){

		//Turn 2*3*4*5*x ... into 120*x
		Number multed = 1;
		if(ctx->NUMBER().size() > 1){
			for(auto ctxNum : ctx->NUMBER()){
				std::string num = ctxNum->getText();
				Number numInNumber = stringToNumber(num);
				multed = multed * numInNumber;
			}
		} else if(ctx->NUMBER().size() == 1){
			std::string num = ctx->NUMBER()[0]->getText();
			multed = stringToNumber(num);
		}
		std::cout << "Multiplied constants to " << multed << std::endl;
		return multed;
	}

	//For a given polynom context, return the vector of coefficients in the order of variables pushed into vars
	//The last coefficient is a constant, so the one without a respective variable
	template<typename Number>
	vector_t<Number> HyproFormulaVisitor<Number>::getPolynomCoeff(HybridAutomatonParser::PolynomContext* ctx){
		std::cout << "-- Bin bei getPolynomCoeff!" << std::endl;		
		vector_t<Number> coeffVec = vector_t<Number>::Zero(vars.size()+1);
		//std::cout << "---- coeffVec inital is:\n" << coeffVec << std::endl;
		for(auto mTerm : ctx->term()){
			Number multed = multTogether(mTerm);
			std::cout << "---- Amount of variables in this term: " << mTerm->VARIABLE().size() << std::endl;
			if(mTerm->VARIABLE().size() == 0){
				//put into last place of coeffVec
				coeffVec(coeffVec.rows()-1) = multed;
				std::cout << "---- No variables, just numbers. coeffVec is then:\n" << coeffVec << std::endl;
			} else if(mTerm->VARIABLE().size() == 1) {
				//put into place according to place of variable in vars	
				unsigned dest = 0;
				auto tmpVar = mTerm->VARIABLE()[0]->getText();
				for(unsigned i=0; i < vars.size(); i++){
					if(vars[i] == tmpVar){
						dest = i;
						break;
					}
				}
				coeffVec(dest) = multed;
				std::cout << "---- Variable was: " << tmpVar << ". coeffVec is now:\n" << coeffVec << std::endl;
			} else {
				std::cout << "ERROR: multiplication of several variables not allowed!" << std::endl;
			}
		}
		return coeffVec;
	}

	///////////// Functions inherited by FormulaBaseVisitor

	template<typename Number>
	antlrcpp::Any HyproFormulaVisitor<Number>::visitPolynom(HybridAutomatonParser::PolynomContext *ctx){
		std::cout << "-- Bin bei visitPolynom!" << std::endl;		

		//0.Syntax Check - Only legal variables in polynom?
		bool allVarsFound = true;
		std::string undefinedVars;
		for(auto currTerm : ctx->term()){
			for(auto maybeVar : currTerm->VARIABLE()){
				bool found = false;
				for(auto var : vars){
					if(maybeVar->getText() == var){
						found = true;
					}
				}
				if(!found){
					undefinedVars = maybeVar->getText() + ", " + undefinedVars;
					allVarsFound = false;
				}
			}
		}
		if(!allVarsFound){
			// TODO: FIND OUT HOW TO GET COMPLETE CONTEXT
			std::cerr << "Following variables were undefined in a term " << ": " << undefinedVars << std::endl;
		}

		//1.Call getPolynomCoeff to fill vector of length vars.size()+1
		vector_t<Number> tmpVec = getPolynomCoeff(ctx);

		//2.Return Vector
		return tmpVec;
	}
	
	template<typename Number>
	antlrcpp::Any HyproFormulaVisitor<Number>::visitEquation(HybridAutomatonParser::EquationContext *ctx){
		std::cout << "-- Bin bei visitEquation!" << std::endl;		

		//0.Syntax Check - Left side of equation legal?
		bool found = false;
		for(auto& variable : vars){
			if((variable + "'") == ctx->VARIABLE()->getText()){
				found = true;
			}
		}
		if(!found){
			std::cerr << "An equation has a not defined variable." << std::endl;
		}

		//1.Call visit(ctx->polynom()) to get vector and return it
		return visit(ctx->polynom());
	}
	
	template<typename Number>
	antlrcpp::Any HyproFormulaVisitor<Number>::visitConstraint(HybridAutomatonParser::ConstraintContext *ctx){
		std::cout << "-- Bin bei visitConstraint!" << std::endl;

		//0.Syntax Check - < or > not allowed
		if(ctx->BOOLRELATION() != NULL){
			if(ctx->BOOLRELATION()->getText() == "<" || ctx->BOOLRELATION()->getText() == ">"){
				std::cerr << "ERROR: Strict relations are not allowed in current build!";
			}	
		}

		//1.Call visit(ctx->polynom()) for both sides to get 2 vectors
		vector_t<Number> poly1 = visit(ctx->polynom()[0]);
		std::cout << "---- visited poly1" << std::endl;
		vector_t<Number> poly2 = visit(ctx->polynom()[1]);
		std::cout << "---- visited poly2" << std::endl;

		//2.Shorten both vectors by one, save last coeff and save everything in pair
		std::cout << "---- poly1 is:\n" << poly1 << std::endl;
		Number poly1Back = poly1(poly1.size()-1);
		poly1.conservativeResize(poly1.size()-1, Eigen::NoChange_t::NoChange);
		std::cout << "---- After shortening poly1 is:\n" << poly1 << " and poly1Back is: " << poly1Back << std::endl;

		std::cout << "---- poly2 is:\n" << poly2 << std::endl;
		Number poly2Back = poly2(poly2.size()-1);
		poly2.conservativeResize(poly2.size()-1, Eigen::NoChange_t::NoChange);
		std::cout << "---- After shortening poly2 is:\n" << poly2 << " and poly2Back is: " << poly2Back << std::endl;

		//2.Use them to calculate constraint vector
		std::vector<std::pair<vector_t<Number>,Number>> constraintVec;
		if(ctx->BOOLRELATION() != NULL){
			if(ctx->BOOLRELATION()->getText() == "<="){

				auto res = std::make_pair(poly1 - poly2, poly2Back - poly1Back);
				//std::cout << "---- Found a <=, res is:\n" << res.first << "\t" << res.second << std::endl;
				constraintVec.push_back(res);

			} else if(ctx->BOOLRELATION()->getText() == ">="){

				auto res = std::make_pair(poly2 - poly1, poly1Back - poly2Back);
				//std::cout << "---- Found a >=, res is:\n" << res.first << "\t" << res.second << std::endl;
				constraintVec.push_back(res);			

			}	
		}
		if(ctx->EQUALS() != NULL){
			auto res1 = std::make_pair(poly1 - poly2, poly2Back - poly1Back);
			//std::cout << "---- Found a =, res1 is:\n" << res1.first << "\t" << res1.second << std::endl;
			auto res2 = std::make_pair(poly2 - poly1, poly1Back - poly2Back);
			//std::cout << "---- Found a =, res2 is:\n" << res2.first << "\t" << res2.second << std::endl;
			constraintVec.push_back(res1);
			constraintVec.push_back(res2);
		}

		//3.Return a vector of pairs of constraint vectors and constant Numbers
		return constraintVec;
	}
	
	template<typename Number>
	antlrcpp::Any HyproFormulaVisitor<Number>::visitIntervalexpr(HybridAutomatonParser::IntervalexprContext *ctx){
		std::cout << "-- Bin bei visitIntervalexpr!" << std::endl;

		//0.Syntax Check - Check if interval is legal
		if(ctx->interval()->NUMBER()[0] > ctx->interval()->NUMBER()[1]){
			std::cerr << "ERROR: Interval left side is bigger than right side!" << std::endl;
		}	
		bool found = false;
		for(auto var : vars){
			if(ctx->VARIABLE()->getText() == var){
				found = true;
			}
		}
		if(!found){
			std::cerr << "ERROR: Variable in interval expression has not been defined!" << std::endl;
		}

		//1.Make constraint vectors
		std::vector<std::pair<vector_t<Number>,Number>> constraintVec;
		vector_t<Number> firstConstraint = vector_t<Number>::Zero(vars.size());
		vector_t<Number> secondConstraint = vector_t<Number>::Zero(vars.size());
		unsigned dest = 0;
		for(unsigned i=0; i < vars.size(); i++){
			if(vars[i] == ctx->VARIABLE()->getText()){
				dest = i;
				break;
			}
		}
		firstConstraint(dest) = Number(-1);
		secondConstraint(dest) = Number(1);
		Number firstConstant = Number(-1) * stringToNumber(ctx->interval()->NUMBER()[0]->getText());
		Number secondConstant = stringToNumber(ctx->interval()->NUMBER()[1]->getText());
		auto firstPair = std::make_pair(firstConstraint, firstConstant);
		auto secondPair = std::make_pair(secondConstraint, secondConstant);
		constraintVec.push_back(firstPair);
		constraintVec.push_back(secondPair);

		//2.Return vector of pairs of constraint vectors and constant Numbers!
		return constraintVec;
	}

}