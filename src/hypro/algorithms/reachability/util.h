#pragma once
#include "TrafoParameters.h"
#include "../../representations/GeometricObject.h"
#include "../../util/Plotter.h"
#include <carl/util/SFINAE.h>

namespace hypro {
	namespace reachability {

		template<typename Representation>
		void printFlowpipe( const std::vector<Representation>& _flowpipe ) {
			for ( const auto& segment : _flowpipe ) {
				std::cout << segment << ", " << std::endl;
			}
		}

		template<typename Representation>
		void printFlowpipeReduced( const std::vector<Representation>& _flowpipe ) {
			std::cout << *_flowpipe.begin() << ", " << std::endl;
			std::cout << "(...)" << std::endl;
			std::cout << *_flowpipe.back() << std::endl;
		}

	} // namespace reachability

/*
 * Functionality for the reachability computation
 */

template <typename Number>
Number hausdorffError( const Number& delta, const matrix_t<Number>& matrix, const Number& _supremum ) {
	Number result;
	// calculate matrix infinity norm
	Number norm = inftyNorm(matrix);

	// Number tmp = delta * t;
	Number tmp = delta * norm;

	double tmpExp = std::exp(carl::toDouble(tmp));
	result = carl::rationalize<Number>(tmpExp);

	//tmp.exp( result );
	result = result - 1 - tmp;
	result *= _supremum;
	return result;
}

template<typename Number, typename Representation>
std::vector<Box<Number>> errorBoxes( const Number& delta, const matrix_t<Number>& flow, const Representation& initialSet, const matrix_t<Number>& trafoMatrix, const Box<Number>& externalInput ) {
	std::vector<Box<Number>> res;
	unsigned dim = flow.cols();
	matrix_t<Number> matrixBlock = matrix_t<Number>::Zero(3*dim, 3*dim);
	matrixBlock.block(0,0,dim,dim) = abs(flow);
	matrixBlock.block(0,dim,dim,dim) = matrix_t<Number>::Identity(dim,dim);
	matrixBlock.block(dim,2*dim,dim,dim) = matrix_t<Number>::Identity(dim,dim);
	matrixBlock = delta*matrixBlock;
	matrix_t<double> convertedBlock = convert<Number,double>(matrixBlock);
	//std::cout << "MatrixBlock: " << std::endl << convertedBlock << std::endl;
	convertedBlock = convertedBlock.exp();
	//std::cout << "exp(MatrixBlock): " << std::endl << convertedBlock << std::endl;
	matrixBlock = convert<double,Number>(convertedBlock);

	// TODO: Introduce better variable naming!
	//Box<Number> b1 = Box<Number>(initialSet.matrix(), initialSet.vector());
	auto b1 = Converter<Number>::toBox(initialSet);
	matrix_t<Number> tmpMatrix = flow*(matrix_t<Number>::Identity(dim,dim) - trafoMatrix);
	// std::cout << "Flow: " << flow << std::endl << "trafoMatrix: " << trafoMatrix << std::endl;
	// std::cout << __func__ << " TmpMtrix: " << tmpMatrix << std::endl;
	b1 = b1.linearTransformation(matrix_t<Number>(tmpMatrix.block(0,0,dim-1,dim-1)),vector_t<Number>(tmpMatrix.block(0,dim-1,dim-1,1)));
	b1 = b1.makeSymmetric();
	//Plotter<Number>::getInstance().addObject(b1.vertices());
	assert(b1.isSymmetric());
	b1 = b1.linearTransformation(matrix_t<Number>(matrix_t<Number>(matrixBlock.block(0,dim,dim-1,dim-1))), vector_t<Number>(matrixBlock.block(0,2*dim-1, dim-1,1)));
	// std::cout << "B1: " << std::endl << b1 << std::endl;

	matrix_t<Number> tmpTrafo = (flow*flow*trafoMatrix).block(0,0,dim-1,dim-1);
	vector_t<Number> tmpTrans = (flow*flow*trafoMatrix).block(0,dim-1,dim-1,1);
	//Representation tmp = initialSet.linearTransformation( tmpTrafo, tmpTrans );
	Representation tmp = applyLinearTransformation(initialSet, TrafoParameters<Number>(tmpTrafo, tmpTrans));
	//Box<Number> b2 = Box<Number>(tmp.matrix(), tmp.vector());
	auto b2 = Converter<Number>::toBox(tmp);
	b2 = b2.makeSymmetric();
	//Plotter<Number>::getInstance().addObject(b2.vertices());
	assert(b2.isSymmetric());
	b2 = b2.linearTransformation(matrix_t<Number>(matrix_t<Number>(matrixBlock.block(0,2*dim,dim-1,dim-1))), vector_t<Number>(matrixBlock.block(0,3*dim-1,dim-1,1)));
	// std::cout << "B2: " << std::endl << b2 << std::endl;

	Box<Number> errorBoxX0 = b1.minkowskiSum(b2);

	// std::cout << "External Input: " << externalInput << std::endl;
	Box<Number> errorBoxExternalInput = Box<Number>( std::make_pair(Point<Number>(vector_t<Number>::Zero(dim-1)), Point<Number>(vector_t<Number>::Zero(dim-1))) );
	errorBoxExternalInput = externalInput.linearTransformation(flow.block(0,0,dim-1,dim-1), flow.block(0,dim-1,dim-1,1));
	errorBoxExternalInput.makeSymmetric();
	errorBoxExternalInput = errorBoxExternalInput.linearTransformation(matrix_t<Number>(matrix_t<Number>(matrixBlock.block(0,2*dim,dim-1,dim-1))), vector_t<Number>(matrixBlock.block(0,3*dim-1,dim-1,1)));

	Box<Number> differenceBox(errorBoxX0.minkowskiDecomposition(errorBoxExternalInput));
	assert(!externalInput.empty() || errorBoxX0 == differenceBox);

	res.emplace_back(errorBoxX0);
	res.emplace_back(errorBoxExternalInput);
	res.emplace_back(differenceBox);

	return res;
}

/**
 * based on the Hausdorff distance, constructs the box (also a polytope) that is used for bloating the initial
 * approximation
 */
template <typename Number, typename Representation>
Representation computePolytope( unsigned int _dim, Number _radius ) {
	matrix_t<Number> mat = hypro::matrix_t<Number>::Zero( 2 * _dim, _dim );
	vector_t<Number> vec( 2 * _dim, 1 );
	int i = 0;
	for ( unsigned z = 0; z < _dim; ++z ) {
		vec( i ) = _radius;
		vec( i + 1 ) = _radius;

		mat( i, z ) = 1;
		mat( i + 1, z ) = -1;

		i = i + 2;
	}
	Representation poly = Representation( mat, vec );
	return poly;
}

template<typename Number, typename Representation, carl::DisableIf< std::is_same<Representation, SupportFunction<Number> > > = carl::dummy>
Representation applyLinearTransformation( const Representation& _in, const TrafoParameters<Number>& parameters ) {
	return _in.linearTransformation(parameters.matrix(), parameters.vector());
}

template<typename Number, typename Representation, carl::EnableIf< std::is_same<Representation, SupportFunction<Number> > > = carl::dummy>
Representation applyLinearTransformation( const Representation& _in, const TrafoParameters<Number>& parameters) {
	return _in.linearTransformation(parameters.parameters());
}

template<typename Number, typename Representation, carl::DisableIf< std::is_same<Representation, SupportFunction<Number> > > = carl::dummy>
void applyReduction( Representation& ) {
}

template<typename Number, typename Representation, carl::EnableIf< std::is_same<Representation, SupportFunction<Number> > > = carl::dummy>
void applyReduction( Representation& _in) {
	_in.forceLinTransReduction();
}

template<typename Number, typename Representation, carl::DisableIf< std::is_same<Representation, SupportFunction<Number>> > = carl::dummy>
void aggregationReduction( Representation&, Transition<Number>* , Number , Number  ) {
}

template<typename Number, typename Representation, carl::EnableIf< std::is_same<Representation, SupportFunction<Number>> > = carl::dummy>
void aggregationReduction( Representation& _in, Transition<Number>* transition, Number timeBound, Number timeStep) {
	Number temp = timeBound/timeStep;
	unsigned long multPerEval = _in.multiplicationsPerEvaluation();
	unsigned long estimatedNumberOfEvaluations =  (transition->guard().mat.rows() + transition->target()->invariant().mat.rows()) * carl::toInt<carl::uint>(carl::ceil(temp));
	unsigned long estimatedCostWithoutReduction = estimatedNumberOfEvaluations * multPerEval;
	unsigned long hyperplanesForReduction = 2* _in.dimension() * (_in.dimension()-1)+ 2* _in.dimension();
	unsigned long estimatedCostWithReduction = hyperplanesForReduction* multPerEval+ estimatedNumberOfEvaluations * carl::pow(hyperplanesForReduction, 2);
	if (estimatedCostWithReduction < estimatedCostWithoutReduction) {
		auto tmpHPoly = Converter<Number>::toHPolytope(_in);
		SupportFunction<Number> newSet(tmpHPoly.matrix(), tmpHPoly.vector());
		_in = newSet;
	}
}



} // namespace hypro
