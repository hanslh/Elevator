#include <iostream>
#include "elevator.h"

elevator::elevator(int startFloor)
{
    elapsedTime = 0;
    currentFloor = startFloor;
    passengerCount = 0;
    // create elevator slots. destinationFloor -99 means empty slot
    for(int i = 0; i < CAPACITY; i++){
        passengerSlot[i] = new passenger;
        passengerSlot[i]->destinationFloor = EMPTY_SLOT;
    }
}

elevator::~elevator()
{
    for(int i = 0; i < CAPACITY; i++){
        delete passengerSlot[i];
    }
}

bool elevator::isEmpty(void) const{
    if(passengerCount == 0)
        return true;
    else
        return false;
}

bool elevator::isFull(void) const{
    if(passengerCount == CAPACITY)
        return true;
    else
        return false;
}


int elevator::getCurrentFloor(void) const{
    return currentFloor;
}

int elevator::getDirection(void) const{
    return direction;
}

void elevator::setDirection(int directionValue){
    if(directionValue == 1)
        direction = 1;
    else
        direction = 0;
}

int elevator::getElapsedTime(void) const{
    return elapsedTime;
}

bool elevator::loadPassenger(int destination){
    if(!isFull() && (destination != currentFloor)){ // If available space and destination of passenger is not current floor
        if(isEmpty()){                              // If elevator is empty, first passenger defines direction of elevator
            if(destination > currentFloor)
                direction = UP_DIRECTION;
            else
                direction = DOWN_DIRECTION;
        }
        for(int i = 0; i < CAPACITY; i++){          // Find an empty slot for new passenger
            if(passengerSlot[i]->destinationFloor == EMPTY_SLOT){
                passengerSlot[i]->destinationFloor = destination;
                passengerSlot[i]->departureFloor = currentFloor;
                passengerSlot[i]->departureTime = elapsedTime + STOP_TIME;      // Passengers entering elevator is last and get STOP_TIME added to departuretime  
                passengerCount++;
                std::cout << "Passenger with destination " << destination << " loaded into slot " << i << " of elevator" << std::endl;
                break;
            }
        }
        setElevatorStopFlag();                      // Indicate that the elevator has stopped (used for incrementation of elapsed time)
        return true;
    }
    return false;                                   // Elevator is full or destination is the same as current floor
}

bool elevator::offloadCurrentFloorPassenger(int* departureFloor, int* departureTime, int* arrivalTime){
     for(int i = 0; i < CAPACITY; i++){
        if(passengerSlot[i]->destinationFloor == currentFloor){                  // If match for current floor
            *departureFloor = passengerSlot[i]->departureFloor;                 // Get passenger info
            *departureTime = passengerSlot[i]->departureTime;
            passengerSlot[i]->destinationFloor = EMPTY_SLOT;                    // and set slot to empty
            passengerCount--;
            *arrivalTime = elapsedTime;
            setElevatorStopFlag();                                              // Indicates that the elevator has stopped at a floor. Used by run-function
            return true;                                                        // Passenger with current floor as destination found
        }
    }
    return false;                                                               // No passenger with current floor as destination
}

bool elevator::hasElevatorStopped(void){
    if(elevatorStopFlag)
        return true;
    else
        return false;
}

int elevator::run(int nextFloor){
    int timeIncrement = 0;
    
    if(elevatorStopFlag){

        timeIncrement = STOP_TIME;
        clearElevatorStopFlag();

    }
    
    if(direction == UP_DIRECTION){     

        timeIncrement = timeIncrement + ((nextFloor - currentFloor)*TRANSITION_TIME);   // Calculate time increment for this elevator run
        elapsedTime = elapsedTime + timeIncrement;                                      // Calculate total elapsed time
        
    } else {
                  
        timeIncrement = timeIncrement + ((currentFloor - nextFloor)*TRANSITION_TIME);   // Calculate time increment for this elevator run
        elapsedTime = elapsedTime + timeIncrement;                                      // Calculate total elapsed time

    }
        
    currentFloor = nextFloor;
    return timeIncrement;
}

void elevator::setElevatorStopFlag(){
    elevatorStopFlag = true;
}

void elevator::clearElevatorStopFlag(){
    elevatorStopFlag = false;
}

int elevator::getPassengerCount(void) const{
    return passengerCount;
}