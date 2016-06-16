/**
 * Implementation of principal component analysis for a set of sample points.
 * @file  pca.tpp
 * @author Simon Froitzheim
 *
 * @since       2015-02-15
 * @version	2016-03-17
 */

#include "pca.h"

namespace hypro {

template<typename Number>
std::vector<Halfspace<Number>> computeOrientedBox(const std::vector<Point<Number>>& samples){
	std::vector<Halfspace<Number>> res;

	if(!samples.empty()){
		//gets dimension of sample points
	    unsigned dim = samples[0].rawCoordinates().rows();
	    //gets number of sample points
	    unsigned sSize = samples.size();

	    Point<Number> mean = Point<Number>(vector_t<Number>::Zero(dim));

	    //computes the sum of all sample points
	    for (unsigned i=0; i < sSize; ++i){
	         mean += samples[i];
	    }
	    //computes the arithmetic mean of the sample points
	    mean = mean.rawCoordinates()*( ((Number) 1)/sSize);

	    std::vector<Point<Number>> tSamples = samples;

	    //computes the set of translated sample points
	    for (unsigned i=0; i < sSize; ++i){
	        tSamples[i] -= mean;
	    }

	    matrix_t<Number> sMatrix = matrix_t<Number>::Zero(dim, sSize);

	    //builds the sampling matrix
	    for (unsigned i=0; i < sSize; ++i){
	        sMatrix.col(i) = tSamples[i].rawCoordinates();
	    }

	    //computes the sample covariance matrix (represents the distribution of the samples)
	    matrix_t<Number> covMatrix;
		if(sSize > 1) {
			covMatrix = ( ((Number) 1)/(sSize-1))*sMatrix*sMatrix.transpose();
		} else {
			covMatrix = sMatrix*sMatrix.transpose();
		}


	    //conversion from the covariance matrix to double in order for JacobiSVD to work
	    Eigen::MatrixXd doubleCovMatrix = convert<Number,double>(covMatrix);

	    //computes the singular value decomposition of the sample covariance matrix (only U component from U*Sigma*V^T needed)
	    Eigen::JacobiSVD<Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic>, Eigen::NoQRPreconditioner> svd = Eigen::JacobiSVD<Eigen::MatrixXd, Eigen::NoQRPreconditioner>(doubleCovMatrix, Eigen::ComputeThinU);
	    Eigen::MatrixXd u = svd.matrixU();

	    //conversion back to Number
	    matrix_t<Number> numberU = convert<double,Number>(u);

	    //computes halfspaces with the help of U in two steps

	    //first step: compute the maximum/minimum product of transposed translated samples (tSamples) with the corresponding column of U

	    vector_t<Number> max = vector_t<Number>(dim);
	    vector_t<Number> min = vector_t<Number>(dim);
	    for (unsigned i=0; i < dim; ++i){
	        max(i) = tSamples[0].rawCoordinates().dot(numberU.col(i));
	        min(i) = tSamples[0].rawCoordinates().dot(numberU.col(i));
	        for (unsigned j=0; j < sSize; ++j){
	            Number tmp = tSamples[j].rawCoordinates().dot(numberU.col(i));
	            if ( tmp > max(i)){
	                max(i) = tmp;
	            }
	            if ( tmp < min(i)){
	                min(i) = tmp;
	            }
	        }
	    }

	    //second step: create Halfspaces with the given normals and maximum/minimum products
	    res = std::vector<Halfspace<Number>>(2*dim);
	    for (unsigned i=0; i < dim; ++i){
	        res[2*i] = Halfspace<Number>(numberU.col(i), max(i) + numberU.col(i).dot(mean.rawCoordinates()));
	        res[2*i+1] = Halfspace<Number>(-numberU.col(i), -min(i) - numberU.col(i).dot(mean.rawCoordinates()));
	    }
	}
    //returns halfspaces
    return res;
}

} //namespace
