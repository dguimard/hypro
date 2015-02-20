/** 
 * A vertex is a point with a color assigned. The color determines is needed by
 * some representations e.g. to determine if the vertex is part of the state set.
 * @file Vertex.h
 * 
 * @author Stefan Schupp <stefan.schupp@cs.rwth-aachen.de>
 * @author Sebastian Junges
 * 
 * @since	2011-03-24
 * @version 2014-01-17
 */

#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <list>

#include "Point.h"

namespace hypro {
	template<typename Number>
    class Vertex;
	template<typename Number>
    using vVec = typename std::vector<Vertex<Number>>;
    template<typename Number>
	using vVecIt = typename std::vector<Vertex<Number>>::iterator;
    template<typename Number>
	using vList = typename std::list<Vertex<Number>>;
    template<typename Number>
	using vListIt = typename std::list<Vertex<Number>>::iterator;
    template<typename Number>
	using vSet = typename std::set<Vertex<Number>>;
    template<typename Number>
	using vSetIt = typename std::set<Vertex<Number>>::iterator;
    
    template<class Number>
    class Vertex
    {
        private:
            Point<Number> mPoint;
            bool mColor;

        public:

            /**
             *
             * @param dimension
             * @param color
             */
            Vertex(bool color = false) : mPoint()
            {
                mColor = color;
            }

            /**
             *
             * @param coordinates
             * @param color
             */
            Vertex(const typename Point<Number>::coordinateMap& coordinates, bool color = false) : mPoint(coordinates)
            {
                mColor = color;
            }

            /**
             *
             * @param p
             * @param color
             */
            Vertex(const Point<Number>& p, bool color = false) : mPoint(p)
            {
                mColor = color;
            }

            /**
             * 
             * @param coordinates
             * @param color
             */
            Vertex(std::initializer_list<Number> coordinates, bool color = false) : mPoint(coordinates)
            {
                mColor = color;
            }
            
            /**
             * 
             * @param v
             */
            Vertex(const Vertex<Number>& v) : mPoint(v.mPoint), mColor(v.mColor)
            {
            }

            /**
             *
             * @return the color of the vertex.
             */
            bool color() const
            {
                return mColor;
            }

            /**
             *
             * @param c the new value for this vertex' color.
             */
            void setColor(bool c = true) 
            {
                mColor = c;
            }

            /**
             * 
             * @return the point of the vertex
             */
            const Point<Number>& point() const
            {
                return mPoint;
            }

            /**
             * 
             * @return the point of the vertex
             */
            Point<Number>& rPoint()
            {
                return mPoint;
            }

            /**
             * 
             * @param p the new value for this vertex' point
             */
            void setPoint(const Point<Number>& p)
            {
                mPoint = p;
            }

            /**
             * Inverts the color.
             */
            void invertColor() 
            {
                mColor = !mColor;
            }
            
            /*******************************************************************
             * Point shortcuts
             *******************************************************************/
            
            /**
             * @see Point::coordinate
             */
            Number coordinate(const carl::Variable& _var) const
            {
                return mPoint.coordinate(_var);
            }
			
			Number coordinate(unsigned _dim) const {
				return mPoint.coordinate(_dim);
			}

            /**
             * @see Point::dimension
             */
            unsigned dimension() const 
            {
                return mPoint.dimension();
            }
            
            /**
             * @see Point::variables
             */
            std::vector<carl::Variable> variables() const
            {
                return mPoint.variables();
            }
            
            /**
             * @see Point::hasDimension
             */
            bool hasDimension(const carl::Variable& _i) const
            {
                return mPoint.hasDimension(_i);
            }
            
            /**
             * @see Point::hasDimensions
             */
            bool hasDimensions(const std::vector<carl::Variable>& _variables) const
            {
                return mPoint.hasDimensions(_variables);
            }
            
            /*******************************************************************
             * Operators
             *******************************************************************/

            /**
             *
             * @param v1
             * @param v2
             * @return true, if they are equal.
             */
            friend bool operator==(const Vertex<Number> & _v1, const Vertex<Number> & _v2)
            {
                if(_v1.mColor != _v2.mColor) return false;
                if(_v1.mPoint != _v2.mPoint) return false;
                return true;
            }

            /**
             *
             * @param v1
             * @param v2
             * @return true, if they are not equal.
             */
            friend bool operator!=(const Vertex<Number> & _v1, const Vertex<Number> & _v2)
            {
                return !(_v1 == _v2);
            }

            /**
             * Comparison operator for the map.
             * Compares the points of both vertices. If both are equal,
             * the color is compared while true is greater than false
             *
             * @param v1
             * @param v2
             * @return
             */
            friend bool operator<(const Vertex<Number> & _v1, const Vertex<Number> & _v2)
            {
                if (_v1.mPoint < _v2.mPoint) return true;
                if (_v1.mPoint > _v2.mPoint) return false;
                if (!_v1.mColor && _v2.mColor) return true;
                return false;
            }

            /**
             * Comparison operator for the map.
             * Compares the points of both vertices. If both are equal,
             * the color is compared while true is greater than false
             *
             * @param v1
             * @param v2
             * @return
             */
            friend bool operator>(const Vertex<Number> & _v1, const Vertex<Number> & _v2)
            {
                return _v2 < _v1;
            }

            /**
             *
             * @param i
             * @return
             */
            Number& operator[] (const carl::Variable& _i)
            {
                return mPoint[_i];
            }

            Number at(const carl::Variable& _i) const
            {
                return mPoint.at(_i);
            }

            friend std::ostream& operator<<(std::ostream& ostr, const Vertex& v) 
            {
                ostr << v.mPoint << " [" << v.mColor << "]";
                return ostr;
            }

    };
}