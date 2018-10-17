#pragma once

#include "RootGrowNode.h"
#include "SupportFunctionContentNew.h"

namespace hypro {

template<typename Number>
class SupportFunctionContentNew;	

//Specialized subclass for transformations as example of a unary operator
template<typename Number>
class TrafoOp : public RootGrowNode<Number> {
private:
	////// General Interface

	SFNEW_TYPE type = SFNEW_TYPE::TRAFO;
	unsigned originCount = 1;
	std::vector<RootGrowNode<Number>*> mChildren = std::vector<RootGrowNode<Number>*>(1,nullptr);

	////// Members for this class

	// 2^power defines the max. number of successive lin.trans before reducing the SF
	unsigned currentExponent;
	std::size_t successiveTransformations;
	std::shared_ptr<const LinTrafoParameters<Number>> parameters;	

public:
	
	////// Constructors & Destructors

	//Thou shalt not make an unconnected transformation without any parameters
	TrafoOp() = delete;

	//Set new trafoOp object as parent of origin, 
	TrafoOp(SupportFunctionContentNew<Number>* origin, const matrix_t<Number>& A, const vector_t<Number>& b) : currentExponent(1) {
		
		// Determine, if we need to create new parameters or if this matrix and vector pair has already been used (recursive).
		parameters = std::make_shared<const LinTrafoParameters<Number>>(A,b);
		// in case this transformation has already been performed, parameters will be updated.
		origin->addUnaryOp(this);
		assert(origin->getRoot() == this);
		assert(this->getChildren().size() == 1);
		origin->hasTrafo(parameters, A, b);
		
		//if(Setting::USE_LIN_TRANS_REDUCTION){
			// best points for reduction are powers of 2 thus we only use these points for possible reduction points
			bool reduced;
			do {
				reduced = false;
				if (origin->getRoot()->getChildren().at(0)->getType() == SFNEW_TYPE::TRAFO && *(dynamic_cast<TrafoOp<Number>*>(origin->getRoot()->getChildren().at(0))->getParameters()) == *parameters && dynamic_cast<TrafoOp<Number>*>(origin->getRoot()->getChildren().at(0))->getCurrentExponent() == currentExponent){
					successiveTransformations = dynamic_cast<TrafoOp<Number>*>(origin->getRoot()->getChildren().at(0))->getSuccessiveTransformations()+1;
				} else {
					successiveTransformations = 0;
				}
				if (successiveTransformations == unsigned(carl::pow(2,parameters->power)-1)) {
					reduced = true;
					currentExponent = currentExponent*(carl::pow(2,parameters->power));
					for(std::size_t i = 0; i < unsigned(carl::pow(2,parameters->power)-1); i++ ){
						origin->getRoot()->getChildren().at(0) = origin->getRoot()->getChildren().at(0)->getChildren().at(0);
					}
					// Note: The following assertion does not hold in combination with the current reduction techniques.
					//assert(mChildren.at(0)->type() != SF_TYPE::LINTRAFO || (mChildren.at(0)->linearTrafoParameters()->parameters == this->parameters && mChildren.at(0)->linearTrafoParameters()->currentExponent >= currentExponent) );
				} 
			} while (reduced == true);
			//assert(mChildren.at(0)->checkTreeValidity());
		//}
	}

	~TrafoOp(){}

	////// Getters & Setters

	SFNEW_TYPE getType() const { return type; }
	unsigned getOriginCount() const { return originCount; }
	unsigned getCurrentExponent() const { return currentExponent; }
	std::size_t getSuccessiveTransformations() const { return successiveTransformations; }
	std::shared_ptr<const LinTrafoParameters<Number>> getParameters() const { return parameters; }

	//transforms parameters
	matrix_t<Number> transform(const matrix_t<Number>& param) const {
		return parameters->getTransformedDirections(param, currentExponent);
	}

	//should not be reachable
	std::vector<EvaluationResult<Number>> compute(const matrix_t<Number>& ) const { 
		std::cout << "USED COMPUTE FROM TRAFOOP SUBCLASS.\n"; 
		assert(false); 
		return std::vector<EvaluationResult<Number>>();
	}

	//Given the results, return vector of evaluation results (here only first place needed, since unary op), here, we also modify
	std::vector<EvaluationResult<Number>> aggregate(std::vector<std::vector<EvaluationResult<Number>>>& resultStackBack, const matrix_t<Number>& currentParam) const {
		assert(resultStackBack.size() == 1); 
		const std::pair<matrix_t<Number>, vector_t<Number>>& parameterPair = parameters->getParameterSet(currentExponent);
		if(resultStackBack.front().begin()->errorCode != SOLUTION::INFEAS){
			unsigned directionCnt = 0;
			for(auto& entry : resultStackBack.front()){
				vector_t<Number> currentDir(currentParam.row(directionCnt));
				if(entry.errorCode == SOLUTION::INFTY) {
					entry.supportValue = 1;
				} else {
					//TRACE("hypro.representations.supportFunction", ": Entry val before trafo: " << entry.optimumValue);
					entry.optimumValue = parameterPair.first * entry.optimumValue + parameterPair.second;
					// As we know, that the optimal vertex lies on the supporting Halfspace, we can obtain the distance by dot product.
					entry.supportValue = entry.optimumValue.dot(currentDir);
				}
				//auto t = convert<Number,double>(currentParam.row(directionCnt));
				//TRACE("hypro.representations.supportFunction", "Direction: " << t << ", Entry value: " << entry.supportValue);
				++directionCnt;
			}
		}
		for(auto& e : resultStackBack.front()){
			std::cout << e << std::endl;
		}
		return resultStackBack.front();
	}

	bool hasTrafo(std::shared_ptr<const LinTrafoParameters<Number>>& ltParam, const matrix_t<Number>& A, const vector_t<Number>& b){
		if(parameters->matrix() == A && parameters->vector() == b){
			ltParam = parameters;
		} 
		return true;
	}

};

} //namespace hypro