#include "hyreach_utils.h"

//#define FLOWPIPE_VERBOSE

namespace hypro
{
    
    /*
    * This class describes a part of a flowpipe which has been generated by a specific location of an hybrid automaton.
    */     
    class FlowpipeSegment
    {
      private:
		location* generatingLocation;   // Location responsible for the existence of this flowpipeSegment
		transition* previousTransition =0; // Transition that has been taken in order to enter the location which generated this flowpipeSegment
		
	  public:
             
        unsigned int startTimestep;     // Number of the timestep responsible for the first set in this flowpipe
             
        matrix_t<double> sets;     // Every column describes a set of the flowpipe
                                   // public to be accessible directly by the algorithms filling the matrix with values
        /*
        * Constructor: initializes the object representing an empty segment of a flowpipe
        */
        FlowpipeSegment(location* generatingLocation, unsigned int startTimestep, unsigned int numberOfDirections, unsigned int timeHorizon)
        {
            this->generatingLocation = generatingLocation;
            this->startTimestep = startTimestep;
            this->sets.resize(numberOfDirections, timeHorizon);
        }    
         
        FlowpipeSegment(location* generatingLocation, transition* generatingTransition, unsigned int startTimestep, unsigned int numberOfDirections, unsigned int timeHorizon): FlowpipeSegment(generatingLocation, startTimestep, numberOfDirections, timeHorizon)
        {
            this->previousTransition = generatingTransition;
        } 
         
        /*
        * Adds the complete evaluation of a set at the specified position (column) in the sets matrix of this object
        */ 
        void addSetAtPosition(double* set_descriptor, unsigned int position)
        {
             if(position < (unsigned int)sets.rows())
             {
                         for(int i=0; i<sets.rows(); i++)
                         {
                              sets(i,position) = set_descriptor[i];        
                         }
             }
        }
        
        /*
        * Getter for the location which generated this FlowpipeSegment
        */
        location* getLocation()
        {
            return generatingLocation;
        }
        
        unsigned int size()
        {
            return (unsigned int) sets.size();
        }
         
        unsigned int  getLastTimestep()
        {
            return startTimestep + ((unsigned int) sets.size());
        }
    };
}
