/*
 * @file    OrthogonalPolyhedron.h
 * @author  Sebastian Junges
 * @author	Stefan Schupp <stefan.schupp@cs.rwth-aachen.de>
 * @author  Benedikt Seidl
 */

#pragma once

#include "datastructures/Point.h"
#include "datastructures/Vertex.h"
#include "datastructures/VertexContainer.h"
#include "../Box/Box.h"
#include "NeighborhoodContainer.h"
#include "Grid.h"

#include <iostream>
#include <string>
#include <map>
#include <list>
#include <vector>
#include <fstream>

#include <carl/core/Variable.h>

namespace hypro {

/**
 * @brief      Enum holding the type identification of the different representations of orthogonal polyhedra.
 */
enum ORTHO_TYPE { VERTEX, NEIGHBORHOOD, EXTREME_VERTEX };

/**
 * @brief      Class for orthogonal polyhedra.
 *
 * @tparam     Number     The used number type.
 * @tparam     Converter  The used converter.
 * @tparam     Type       The internal representation type.
 */
template <typename Number, typename Converter, ORTHO_TYPE Type = ORTHO_TYPE::VERTEX>
class OrthogonalPolyhedronT {
  private:
	// VertexContainer<Number> mVertices; // the container of all vertices
	Grid<Number> mGrid;				   // the grid the polyhedron is defined on
	mutable BoxT<Number,Converter> mBoundaryBox;  // the cached boundary box (mutable to allow performance optimization)
	mutable bool mBoxUpToDate = false;

  public:
	/***********************************************************************
	 * Constructors
	 ***********************************************************************/

	OrthogonalPolyhedronT();
	OrthogonalPolyhedronT( const Vertex<Number>& _vertex );
	OrthogonalPolyhedronT( const VertexContainer<Number>& vertices );
	OrthogonalPolyhedronT( const std::set<Vertex<Number>>& points );
	OrthogonalPolyhedronT( const std::vector<Vertex<Number>>& points );
	OrthogonalPolyhedronT( const OrthogonalPolyhedronT<Number, Converter, Type>& copy );
	OrthogonalPolyhedronT( const OrthogonalPolyhedronT<Number, Converter, Type>&& move );

	/***********************************************************************
	 * Getter, Setter
	 ***********************************************************************/

	std::vector<carl::Variable> variables() const;
	std::vector<Vertex<Number>> vertices() const;
	BoxT<Number,Converter> boundaryBox() const;
	const Grid<Number>& grid() const;
	void addVertex( const Vertex<Number>& _vertex );
	void addVertices( const std::vector<Vertex<Number>>& _vertices );

	bool empty() const;
	std::size_t size() const;
	std::size_t dimension() const;
	Number supremum() const;
	bool isVertex( const Point<Number>& _point ) const;
	bool isOnIEdge( const Point<Number>& _point, unsigned i ) const;
	bool isInternal( const Point<Number>& _point ) const;
	bool isExternal( const Point<Number>& _point ) const;
	bool color( const Point<Number>& _point ) const;

	std::vector<Point<Number>> iNeighborhood( const Point<Number>& _point, unsigned i ) const;
	std::vector<Point<Number>> iNegNeighborhood( const Point<Number>& _point, unsigned i ) const;
	std::vector<Point<Number>> neighborhood( const Point<Number>& _point ) const;

	std::vector<Point<Number>> iSlice( unsigned i, Number pos ) const;
	OrthogonalPolyhedronT<Number, Converter, Type> iProjection( unsigned i ) const;

	/***********************************************************************
	 * Geometric Object functions
	 ***********************************************************************/
	OrthogonalPolyhedronT<Number, Converter, Type> linearTransformation( const matrix_t<Number>& A,
															 const vector_t<Number>& b ) const;
	OrthogonalPolyhedronT<Number, Converter, Type> minkowskiSum( const OrthogonalPolyhedronT<Number, Converter, Type>& rhs ) const;
	OrthogonalPolyhedronT<Number, Converter, Type> intersect( const OrthogonalPolyhedronT<Number, Converter, Type>& rhs ) const;
	OrthogonalPolyhedronT<Number, Converter, Type> hull() const;
	bool contains( const Point<Number>& point ) const;
	bool contains( const OrthogonalPolyhedronT<Number, Converter, Type>& _other ) const;
	OrthogonalPolyhedronT<Number, Converter, Type> unite( const OrthogonalPolyhedronT<Number, Converter, Type>& rhs ) const;

	/***********************************************************************
	 * Auxiliary functions
	 ***********************************************************************/
	std::vector<std::vector<Point<Number>>> preparePlot( unsigned _xDim = 0, unsigned _yDim = 1 ) const;

	/***********************************************************************
	 * Operators
	 ***********************************************************************/

	OrthogonalPolyhedronT<Number, Converter, Type>& operator=( const OrthogonalPolyhedronT<Number, Converter, Type>& _in ) = default;
	OrthogonalPolyhedronT<Number, Converter, Type>& operator=( OrthogonalPolyhedronT<Number, Converter, Type>&& ) = default;

	friend bool operator==( const OrthogonalPolyhedronT<Number, Converter, Type>& op1,
							const OrthogonalPolyhedronT<Number, Converter, Type>& op2 ) {
		return op1.mGrid == op2.mGrid;
	}

	friend bool operator!=( const OrthogonalPolyhedronT<Number, Converter, Type>& op1,
							const OrthogonalPolyhedronT<Number, Converter, Type>& op2 ) {
		return op1.mGrid != op2.mGrid;
	}

	friend std::ostream& operator<<( std::ostream& ostr, const OrthogonalPolyhedronT<Number, Converter, Type>& p ) {
		ostr << "(";
		for ( const auto& vertex : p.mGrid.vertices() ) ostr << vertex << ", ";
		ostr << ")";
		return ostr;
	}

  private:
	void updateBoundaryBox() const;
};

}  // namespace

#include "OrthogonalPolyhedron.tpp"
