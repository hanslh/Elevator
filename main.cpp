#include <iostream>
#include <fstream>
#include "elevator.h"
#include "building.h"
using namespace std;

#define UP_DIRECTION 1
#define DOWN_DIRECTION 0

bool loadElevator(int currentFloor, int *direction, elevator *Elevator, building *Building);    // Support function for loading passengers into elevator at current floor
int getNextFloor(int currentFloor, int direction);

int main(void)
{    
    int currentFloor;           // Current location of elevator
    int nextFloor;              // Next elevator floor
    int direction;              // Current direction of elevator
    int timeIncrement;          // Time increment for a given elevator run
    
    // Load input data from file
    building Building;
    if(Building.loadData("sample.in") != 0){
        cout << "Error opening file" << endl;
        cout << "Press any key to exit" << endl;
        std::cin.get();
        return 1;
    }

    // Open output file
    ofstream outputFile;
    outputFile.open("test.txt");
    // Check that file has opened
    if(!outputFile){
        cout << "Could not open ouput file" << endl;
        cout << "Press any key to exit"  << endl;
        std::cin.get();
        return 1;
    }
    
    // Init elevator -> Set start floor
    elevator Elevator(Building.getStartFloor());
    cout << "The elevator has at start " << Elevator.getPassengerCount() << " passengers" << endl;
    
	// Run loop while there still are passengers in the building and elevator is not empty
    while(!Elevator.isEmpty() || Building.totalPassengersWaiting()){
        
        currentFloor = Elevator.getCurrentFloor();                          // get current floor of elevator
        
        //debug
        cout << endl << endl; 
        cout << "Current floor is: " << Elevator.getCurrentFloor() << endl;
        cout << "The elevator has " << Elevator.getPassengerCount() << " passengers" << endl;
        cout << "Elapsed time is: " << Elevator.getElapsedTime() << endl << endl;
        // 0->0 left:25 arrived:100
        
        // ********* OFFLOADING *********
        if(!Elevator.isEmpty()){                                            // If elevator has passengers
            cout << "Offloading passenger" << endl;
            int departureFloor;
            int departureTime;
            int arrivalTime;
            
            while(Elevator.offloadCurrentFloorPassenger(&departureFloor, &departureTime, &arrivalTime)){    // Offload any passenger that has current floor as destination
                // Print to output file
                outputFile << departureFloor << "->" << currentFloor << " left:" << departureTime << " arrived:" << arrivalTime << endl;
                //cout << departureFloor << "->" << currentFloor << " left:" << departureTime << " arrived:" << arrivalTime << endl;
            }
        }
        // ********** ONLOADING **********
        if(Elevator.isEmpty()){                                             // if elevator is empty
            cout << "Elevator is empty" << endl;
            
            if(!Building.isEmpty(currentFloor)){                                // If current floor has passengers
                cout << "current floor has passengers" << endl << endl;
                
                while(loadElevator(currentFloor,&direction,&Elevator,&Building));          // First passenger in line decides direction. Load additional passengers going in the same direction
                cout << "Done loading passengers at floor " << currentFloor << endl;
                direction = Elevator.getDirection();                                // Get new direction
                nextFloor = getNextFloor(currentFloor,direction);                   // Calculate next floor in new direction
                cout << "Next floor is: " << nextFloor << endl << endl;
            
            } else {                                                            // If current floor is empty
                cout << "Current floor is empty" << endl;
                nextFloor = Building.getLongestWaitFloor(currentFloor);             // Next floor is floor that has waited the longest
                
                if(nextFloor > currentFloor){
                    Elevator.setDirection(UP_DIRECTION);
                } else {
                    Elevator.setDirection(DOWN_DIRECTION);
                }
                cout << "Next floor is: " << nextFloor << endl << endl;
            }
        
        } else if(Elevator.isFull()){                                       // If elevator is full
            cout << "Elevator is full" << endl;
            nextFloor = getNextFloor(currentFloor,direction);                   // Continue to next floor in same direction
            cout << "Next floor is: " << nextFloor << endl << endl;
            
        } else {                                                            // If elevator is not full and not empty
            cout << "Elevator is neither empty or full" << endl;
            while(loadElevator(currentFloor,&direction,&Elevator,&Building));          // Load passengers from current floor going in the same direction as the elevator
            
            nextFloor = getNextFloor(currentFloor,direction);                          // Continue to next floor in same direction
            cout << "Next floor is: " << nextFloor << endl << endl;
            
        }
        
        if(Elevator.hasElevatorStopped()){                                   // If elevator had a stop on this run
            cout << "Reset floor wait time for floor: " << currentFloor << endl;
            Building.resetFloorWaitTime(currentFloor);                          // Reset wait time counter for current floor
        }
        // Run elevator to next floor
        cout << "Run elevator" << endl;
        timeIncrement = Elevator.run(nextFloor);
        
        // Increment wait time counter for all floors
        cout << "Time increment is: " << timeIncrement << "s" << endl;
        Building.incrementFloorWaitTime(timeIncrement);
        
    }
    cout << "The elevator is empty and the building has no more waiting passengers" << endl;
    outputFile.close();  
	return 0;
}

// Loads elevator with a passenger going the same direction as elevator. Return false if elevator is full or no more passengers
// going the same direction are available at current floor. If elevator is empty first passenger decides direction
bool loadElevator(int loadingFloor,int *direction,elevator *Elevator,building *Building){
    if(!Elevator->isFull()){

        if(Elevator->isEmpty()){
            cout << "Load first passenger" << endl;
            int destination = Building->getPassengerFront(loadingFloor);
            Elevator->loadPassenger(destination);   // load first passenger in line. This passenger also decides direction
            *direction = Elevator->getDirection();
            
            if(*direction == 1)
                cout << "New elevator direction is UP" << endl;
            else
                cout << "New elevator direction is DOWN" << endl;
            
            cout << "The elevator has " << Elevator->getPassengerCount() << " passengers" << endl << endl;
            return true;
        }
    
        if(*direction == 1){                                       // If elevator is going up and has free space
            int destination = Building->getPassengerGoingUp(loadingFloor);
            
            if(!(destination < 0)){                                         // and while there still are passengers at current floor going up
                cout << "Load passenger going UP" << endl;
                Elevator->loadPassenger(destination);                        // load passenger going up
                cout << "The elevator has " << Elevator->getPassengerCount() << " passengers" << endl << endl;
                return true;
            } else
                return false;                                                      // no more passengers on floor going up
          
        } else {                                                                // elevator is going down
            int destination = Building->getPassengerGoingDown(loadingFloor);   
            
            if(!(destination < 0)){                                         // and while there still are passengers at current floor going up
                cout << "Load passenger going DOWN" << endl;
                Elevator->loadPassenger(destination);                        // load passenger going down
                cout << "The elevator has " << Elevator->getPassengerCount() << " passengers" << endl << endl;
                return true;
            } else
                return false;                                                      // no more passengers on floor going down
        }
    }
    return false;       // Elevator is full
}

// Increment/decrement floor by one dependant of direction
int getNextFloor(int currentFloor, int direction){
    int nextFloor;

    if(direction == UP_DIRECTION){
        nextFloor = currentFloor + 1;
        return nextFloor;
    } else {
        nextFloor = currentFloor - 1;       // Hvorfor virker ikke currentFloor-- ??
        return nextFloor;
    }
        
}