/*
 * The class which represents a polytope.
 *
 * @file   Polytope.h
 * @author Stefan Schupp <stefan.schupp@cs.rwth-aachen.de>
 *
 * @since       2014-03-20
 * @version     2015-01-21
 */

#pragma once
#ifdef USE_PPL

#include <map>
#include <cassert>
#include <string>
#include <sstream>
#include <gmp.h>
CLANG_WARNING_DISABLE("-Wunused-local-typedef")
#include <ppl.hh>
CLANG_WARNING_RESET
#include <carl/core/Variable.h>

#include "util.h"
#include "../GeometricObject.h"
#include "../../datastructures/Point.h"
#include "../../datastructures/Facet.h"
#include "../../datastructures/Ridge.h"
#include "../../datastructures/Halfspace.h"
#include "../../util/convexHull.h"


using namespace Parma_Polyhedra_Library;
using namespace Parma_Polyhedra_Library::IO_Operators;

namespace hypro
{
	template<typename Number>
	class Polytope : public GeometricObject<Number, Polytope<Number>>
	{
	private:
		C_Polyhedron mPolyhedron;
		mutable std::vector<Point<Number>> mPoints;
		mutable bool mPointsUpToDate;
		polytope::Fan<Number> mFan;

	public:
		Polytope();
		Polytope(const Polytope& orig);
		Polytope(unsigned dimension);
		Polytope(const Point<Number>& point);
		Polytope(const typename std::vector<Point<Number>>& points);
		Polytope(const typename std::vector<vector_t<Number>>& points);
		Polytope(const matrix_t<Number>& A, const vector_t<Number>& b);
		Polytope(const matrix_t<Number>& A);
		Polytope(const C_Polyhedron& _rawPoly);

		virtual ~Polytope();

		/**
		 * Getters and Setters
		 */

		/**
		 * Returns true, if the generatorset is empty.
		 * @return
		 */
		bool empty() const;

		/**
		 * Adds a point to the polytope. Note that if the point lies inside the
		 * current polytope, this has no effect.
		 * @param point
		 */
		void addPoint(const Point<Number>& point);


		void updatePoints() const;
		void setPointsUpToDate(bool _val = true){
			mPointsUpToDate = _val;
		}

		/**initConvexHull
		 * Returns the set of points which form the polytope.
		 * @return Pointset.
		 */
		const std::vector<Point<Number>>& vertices() const;

		/*
		 *
		 */
		std::vector<Point<Number>>& rVertices();

		/*
		 * returns the fan of the Polytope
		 */
		const polytope::Fan<Number>& fan();

		/*
		 * returns a reference to the fan of the Polytope
		 */
		polytope::Fan<Number>& rFan();

		/*
		 * sets the fan of the Polytope
		 */
		void setFan(const polytope::Fan<Number>& _fan);

		void calculateFan();

		/**
		 * Prints the polytopes' generators obtained from the PPL to stdout.
		 */
		void print() const;

		/**
		 * Writes the polytope to a file in a matlab format.
		 * @param _filename The filename of the output file.
		 */
		void writeToFile(std::string _filename) const;

		/*
		 *
		 */
		friend std::ostream& operator<<(std::ostream& lhs, const Polytope<Number>& rhs)
		{
			lhs << "[";
			for(auto& generator : rhs.rawPolyhedron().generators())
			{
				// lhs << generator;
				generator.print();
			}
			lhs << "]" << std::endl;
			return lhs;
		}

		/**
		 * PPL related functions
		 */
		const C_Polyhedron& rawPolyhedron() const;
		C_Polyhedron& rRawPolyhedron();

		/**
		 * Geometric Object interface
		 */
		std::size_t dimension() const;
		Polytope<Number> linearTransformation(const matrix_t<Number>& A) const;
		Polytope<Number> affineTransformation(const matrix_t<Number>& A, const vector_t<Number>& b) const;
		Polytope<Number> project(const std::vector<unsigned>& dimensions) const;
		Polytope<Number> minkowskiSum(const Polytope<Number>& rhs) const;
		// implemented according to Komei Fukuda 2004
		Polytope<Number> altMinkowskiSum(Polytope<Number>& rhs);
		Polytope<Number> intersect(const Polytope<Number>& rhs) const;
		Polytope<Number> intersectHalfspace(const Halfspace<Number>& rhs) const;
		Polytope<Number> intersectHalfspaces(const matrix_t<Number>& _mat, const vector_t<Number>& _vec) const;
		std::pair<bool, Polytope<Number>> satisfiesHalfspace(const Halfspace<Number>& rhs) const;
		std::pair<bool, Polytope<Number>> satisfiesHalfspaces(const matrix_t<Number>& _mat, const vector_t<Number>& _vec) const;
		Polytope<Number> hull() const;
		bool contains(const Point<Number>& point) const;
		bool contains(const Polytope<Number>& poly) const;
		Polytope<Number> unite(const Polytope<Number>& rhs) const;
	  //  static std::vector<Facet<Number>> convexHull(const std::vector<Point<Number>> points);


		/**
		 * Polytope related
		 */
		Number supremum () const;


		//Polytope<Number>& operator= (const Polytope<Number>& rhs);

		/**
		 * Auxiliary functions
		 */
		int computeMaxVDegree();
		Point<Number> computeMaxPoint();
		Point<Number> computeInitVertex(Polytope<Number> _secondPoly);
		Point<Number> localSearch(Point<Number>& _vertex,  Point<Number>& _sinkMaximizerTarget);

	};
} // namespace

#include "Polytope.tpp"

#endif