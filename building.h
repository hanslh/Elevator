#ifndef BUILDING_H
#define BUILDING_H

#include "queue.h"

class building
{
public:
    building();
    ~building();
    // Loads passenger data from file
    int loadData(std::string inputFileName);
    // Return elevator initial floor pulled from input file
    int getStartFloor(void) const;
    // Return new start floor for elevator based on floor wait time and proximity to elevator
    int getLongestWaitFloor(int currentFloor) const;
   
    // return destination floor of first passenger in line going down and removes passenger from line. If no such passenger exist, return -99
    int getPassengerGoingDown(int floorNo);
    // return destination floor of first passenger in line going up and removes passenger from line. If no such passenger exist, return -99
    int getPassengerGoingUp(int floorNo);
    // return destination floor of passenger at front of line and removes passenger from line. In floor is empty, return -99
    int getPassengerFront(int floorNo);
    // return true if floor is empty
    bool isEmpty(int floorNo) const;
    // return total passengers in building still waiting for elevator
    int totalPassengersWaiting(void) const;

    
    // Increment floor wait time with time[s] for every active floor
    void incrementFloorWaitTime(int time);
    // Resets floor wait time for floor
    void resetFloorWaitTime(int floorNo);
    
    int getFloorWaitTime(int floorNo) const;
    
private:
    int elevatorInitialFloor;
    int numberOfActiveFloors;
    int numberOfPassengers;
    
    struct floorStruct{
        int floor;
        int timeSinceElevatorVisit;
        queue *passengerQueue;
        floorStruct *next;
    };
    
    floorStruct *start;
    floorStruct *end;
};

#endif // BUILDING_H
