/*
 *  This file contains general helper methods.
 *  Some of these methods correspond to standard Matlab functionality
 *  Author: Norman Hansen
 */

#include "hyreach_utils.h"    
//#include "../../datastructures/hybridAutomata/util.h"
    
//#define HELPER_METHODS_VERBOSE

    /*
    * This templated method can be used to print arrays of arbitrary type
    */
    template<typename Type>
    void printArray(Type* array, unsigned int size)
    {
         for( unsigned int i=0; i< size; i++)
         {
             std::cout << array[i] << ", ";
         }
    }

    /*
	* Prints the argument on the standard output
	*/
	void printDirectionList(std::vector<matrix_t<double>> list)
	{
		for(auto iterator1 = list.begin(); iterator1 != list.end(); ++iterator1)
		{
			matrix_t<double> actual = *iterator1;
            std::cout << actual.transpose() << "\n\n";
		}
	}
	
	/*
	* Prints the argument on the standard output
	*/
	void print(std::vector<double> list)
	{
         for(unsigned int i=0; i<list.size(); i++)
         {
             std::cout << list[i] << '\n';
         }
    }

    /*
	* Checks wether the list contains the specified direction
	*/
	bool contains(std::vector<matrix_t<double>> list, matrix_t<double> direction, unsigned int length)
	{
		#ifdef HELPER_METHODS_VERBOSE
		       std::cout << "contains: check wether a list contains a specific direction ...  ";
		#endif

		for(auto iterator1 = list.begin(); iterator1 != list.end(); ++iterator1)
		{
			bool equal = true;
			matrix_t<double> a = *iterator1;

			for(unsigned int i=0; i<length; i++)
			{
				equal &= a(i) == direction(i);
			}

			if(equal)
			{
				#ifdef HELPER_METHODS_VERBOSE
				       std::cout << "list contains \n" << a <<"\n";
				#endif

				return true;
			}
		}

		#ifdef HELPER_METHODS_VERBOSE
		       std::cout << "no.\n";
		#endif

		return false;		
	}
	
	/*
	* Returns the index of the position of direction in directions if it is contained.
	* In case direction is not in directions, this method returns -1
	*/
	int contains(std::vector<matrix_t<double>>* directions, matrix_t<double>* direction)
	{
		#ifdef HELPER_METHODS_VERBOSE
		       std::string method = "contains(...) ";
		#endif

        unsigned int length = direction->rows();
		for(unsigned int i=0; i< directions->size(); i++)
		{
			matrix_t<double> temp = directions->at(i);

            bool equal = true;
			for(unsigned int j=0; j<length; j++)
			{
				equal &= temp(j) == (*direction)(j);
			}

			if(equal)
			{
				#ifdef HELPER_METHODS_VERBOSE
				       std::cout << method << "list contains" << BL << temp << BL;
				#endif

				return i;
			}
		}

		#ifdef HELPER_METHODS_VERBOSE
		       std::cout << method << "no suitable match found for " << BL;
		       std::cout << *direction << BL;
		#endif

		return -1;		
	}
	
	int contains(std::vector<matrix_t<double>>* directions, matrix_t<double> direction)
	{
        return contains(directions, &direction);
    }
	
	
	int* computeDirectionMapping(std::vector<matrix_t<double>>* directions)
	{
          int* result = new int[directions->size()];
          #ifdef HELPER_METHODS_VERBOSE
              std::string method = "computeDirectionMapping(...): ";
              std::cout << method << "directions: " << BL;
              std:: cout << *directions << BL;
              
              std:: cout << "for every direction: " << BL;
          #endif
          
          for(unsigned int i=0; i<directions->size(); i++)
          {
              #ifdef HELPER_METHODS_VERBOSE
                  std::cout << method << "directions contains: " << BL;
                  std:: cout << -directions->at(i) << BL;
              #endif
              result[i] = contains(directions, -(directions->at(i)));
              #ifdef HELPER_METHODS_VERBOSE
                  std:: cout << "at position " << result[i] << BL;
              #endif
          }
          
           return result;
    }

    /*
    * Returns true if the list of integers "list" contains the value "item"
    */
    bool listContains(std::vector<int>* list, int item)
    {
         for(auto iterator = list->begin(); iterator != list->end(); ++iterator)
         {
                  if( *iterator == item )
                  {
                      return true;
                  }
         }
         return false;
    }

    /*
    * Converts a matrix to a list of directions
    */
/*    std::vector<matrix_t<double>>* Matrix2DirectionList(matrix_t<double> m)
    {
        std::vector<matrix_t<double>>* result = new std::vector<matrix_t<double>>(m.rows());
        
        for(unsigned int i=0; i<m.rows(); i++)
        {
            
        }
        
        return result;
    } */

    /**
	*  Converts a list of directions into a matrix containing directions
	*/
	matrix_t<double> directionList2Matrix(std::vector<matrix_t<double>>* list)
	{
		// get list size
		unsigned int counter = 0;
		unsigned int matrixlength = 0;
		unsigned int temp = 0;
		for (auto iterator = (*list).begin(); iterator != (*list).end(); ++iterator)
		{
			counter++;
			temp = (*iterator).size();
			
			if(matrixlength != 0 && temp != matrixlength)
			{
                std::cout << "Cannot convert a list of matrices with different length into a single matrix!";
                matrix_t<double> null(0, 0);
                return null;           
            }
            else
            {
                matrixlength = temp;
            }
		}

		matrix_t<double> result(counter, matrixlength);

		counter = 0;
		for (auto iterator = (*list).begin(); iterator != (*list).end(); ++iterator)
		{
			for(unsigned int i=0; i<matrixlength; i++)
			{
                 result(counter, i) = (*iterator)(i);
            }
             
			counter++;
		}
		return result;
	}
	
	/*
	* Converts a forward list of matrices into an array of matrices
	*/
	matrix_t<double>* list2Array(std::vector<matrix_t<double>>* list)
	{
		// get list size
		unsigned int counter = 0;
		for (auto iterator = (*list).begin(); iterator != (*list).end(); ++iterator)
		{
			counter++;
		}

		matrix_t<double> *result[counter];

		counter = 0;
		for (auto iterator = (*list).begin(); iterator != (*list).end(); ++iterator)
		{
			result[counter] = &(*iterator);
			counter++;
		}

		return *result;
	}
	
	/*
	* Converts a nx1 matrix into an array of doubles
	*/
	double* matrix2Array(matrix_t<double> m)
	{
            double* result = new double[m.rows()];
            
            for(int i=0; i<m.rows(); i++)
            {
                result[i] = m(i,0).toDouble();
            }
            
            return result;
    }
    
    /*
	* Converts a vector into an array of doubles
	*/
	double* vector2Array(vector_t<double> v)
	{
            double* result = new double[v.size()];
            
            for(int i=0; i<v.size(); i++)
            {
                result[i] = v(i).toDouble();
            }
            
            return result;
    }
	
	/*
	 *  returns the dimensionality (number of rows) of the matrix from the first initial location
	 */
	double getDimensionality(HybridAutomaton<double>* model)
	{
           return ((*((*model).initialLocations().begin()))->activityMat()).rows();
    }

    /*
    * adds a zero column to the matrix parameter
    */
    matrix_t<double> addZeroColumn(matrix_t<double> m)
    {
    
        //matrix_t<double> result = matrix_t<double>::Zero(m.rows(), m.cols()+1);
        int newcols = m.cols()+1;
        matrix_t<double> result(m.rows(), m.cols()+1);
        #ifdef HELPER_METHODS_VERBOSE
            std::cout<< "empty result: " << result;
            std::cout << "result rows: " << result.rows() << BL;
            std::cout << "result cols: " << result.cols() << BL;
            
            std::cout << "m rows: " << m.rows() << BL;
            std::cout << "m cols: " << m.cols() << BL;
        #endif
        
        // copy m into result
        for( unsigned int i=0; i<m.rows(); i++)
        {
             for( unsigned int j=0; j<m.cols(); j++)
             {
                  //std::cout << "i: " << i << " j: " << j << BL;
                  result(i,j) = m(i,j);
                  std::cout << "result( " << i << ", " << j << ") = " << result(i,j) << BL;
             }
             
             std::cout << "i: " << i << " j: " << newcols-1 << BL;
             result(i,newcols-1) = 0;
        }
        
        #ifdef HELPER_METHODS_VERBOSE
            std::cout<< "complete result: " << result;
        #endif
        return result;
        
    
    /*
        int newcols = m.cols()+1;
        matrix_t<double> result(m.rows(), newcols);
    
        #ifdef HELPER_METHODS_VERBOSE
            std::cout<< "empty result: " << result;
            std::cout << "result rows: " << result.rows() << BL;
            std::cout << "result cols: " << result.cols() << BL;
            
            std::cout << "m rows: " << m.rows() << BL;
            std::cout << "m cols: " << m.cols() << BL;
        #endif
        
        for( unsigned int j=0; j<m.cols(); j++)
        {
             for( unsigned int i=0; i<m.rows(); i++)
             {
                  result(i,j) = m(i,j);
                  std::cout << "result( " << i << ", " << j << ") = " << result(i,j) << BL;
             }
        }
        
        for( unsigned int i=0; i<m.rows(); i++)
        {
            result(i,newcols-1) = 0;
            std::cout << "result( " << i << ", " << newcols-1 << ") = " << result(i,newcols-1) << BL;
        }
         */    
    }
