#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <string>
#include <stdlib.h>
#include "building.h"
#include "queue.h"
using namespace std;

#define EMPTY -1;
#define FILE_OK 0
#define FILE_NOT_FOUND 1
#define FILE_SYNTAX_ERROR 2

building::building(){
    start = NULL;
    end = NULL;
    numberOfActiveFloors = 0;
    numberOfPassengers = 0;
}

building::~building()
{
        
}

int building::loadData(string inputFileName){
    int returncode = FILE_OK;
    // Open input file
    ifstream inputFile(inputFileName.c_str());
    
    if(!inputFile){
        return FILE_NOT_FOUND;
    } else {
        cout << "Reading file..." << endl;
        
        string inputString;
        int lineCounter = 0;
        int passengerCounter = 0;
        // Count number of active floors
        while(inputFile.good() && returncode != FILE_SYNTAX_ERROR){                            // While no error flags are set
            getline(inputFile,inputString);
            
            if(lineCounter == 0){                           // Read start floor from first line
                istringstream ss(inputString);

                int startFloor;
                char temp, delim;

                if((ss >> temp >> delim >> startFloor) && delim == ':'){
                    elevatorInitialFloor = startFloor;                          // Get intitial elevator floor
                    lineCounter++;
                    cout << "Intitial Floor: " << startFloor << endl;
                } else {
                    returncode = FILE_SYNTAX_ERROR;
                    cout << "Syntax error" << endl;
                }

            } else {

                istringstream lineSS(inputString);
                int floorNo;
                char delim;
                string passengerList;
                
                if((lineSS >> floorNo >> delim >> passengerList) && delim == ':'){          // Each line is a new floor
                    floorStruct *temp = new floorStruct;
                    
                    if(end == NULL){
                        start = end = temp;
                    } else {
                        end->next = temp;
                        end = temp;
                    }
                    
                    temp->next = NULL;
                    temp->floor = floorNo;
                    numberOfActiveFloors++;
                    
                    cout << "Floor: " << floorNo << " Passenger List: " << passengerList << endl;
                    
                    istringstream line(passengerList);
                    string destinationString;
                    temp->passengerQueue = new queue;
                    
                    while(getline(line, destinationString, ',')){
                        temp->passengerQueue->appendItem(atoi(destinationString.c_str()));
                        passengerCounter++;
                    }
                    lineCounter++;
                }
            }
        }
        
        numberOfActiveFloors = lineCounter - 1;
        numberOfPassengers = passengerCounter;
        
        cout << endl;
        cout << "Number of active floors: " << numberOfActiveFloors << endl;
        cout << "Number of waiting passengers: " << numberOfPassengers << endl;
        
    }
    
    inputFile.close();
    return returncode;
}


int building::getStartFloor(void) const{
    return elevatorInitialFloor;
}

int building::getLongestWaitFloor(int currentFloor) const{
    // Select next floor based on the following criterias:
    cout << endl << "Getting the floor with the longest wait" << endl;
    queue *longestWaitFloor = new queue;
    floorStruct *temp = start;
    int maxWaitTime = 0;
    
    // Find floor(s) with longest wait that is not empty
    while(temp != NULL){
        if(!temp->passengerQueue->isEmpty()){
            cout << "Wait time of floor " << temp->floor << " is " << temp->timeSinceElevatorVisit << "s. " << endl;
            
            if(temp->timeSinceElevatorVisit > maxWaitTime){             
                maxWaitTime = temp->timeSinceElevatorVisit;
                cout << "Max wait time for current floor is " << maxWaitTime << endl;
                longestWaitFloor->makeEmpty();
                cout << "Queue erased" << endl;
                longestWaitFloor->appendItem(temp->floor);
                cout << "Floor " << temp->floor << " appended to queue" << endl;
                cout << "Front item of queue is " << longestWaitFloor->valueFrontItem() << endl;

                //longestWaitFloor->appendItem(temp->floor);

                //cout << "Floor " << longestWaitFloor->valueFrontItem() << " has so far waited the longest" << endl;     // Hva i fuck er galt med denne queue'en? Programmet crasher her
            } else if(temp->timeSinceElevatorVisit == maxWaitTime){
                longestWaitFloor->appendItem(temp->floor);
                cout << "Floor " << longestWaitFloor->valueEndItem() << " added to longestWaitFloor queue" << endl;
            }
        }
        temp = temp->next;
    }
    cout << "Number of floors that has the longest waiting time is: " << longestWaitFloor->getLength() << endl;
    
    if(longestWaitFloor->getLength() == 1){                                 // If one single floor has waited the longest
        cout << "Only one floor that has waited the longest: ";
        int nextFloor = longestWaitFloor->getFrontItem(); 
        cout << nextFloor << endl;
        delete longestWaitFloor;
        return nextFloor;
    
    } else {                                                                // If two or more floors has same wait time, find closest floor
        cout << "More than one floor has the longest wait time" << endl;
        // Only two floors can have the same distance to curent floor
        int minDistanceA = 999;                                             // Initiate minDistance to a sufficient large value (bigger than probable min distance)
        int minDistanceB = 999;
        int floorIndexA;
        int floorIndexB;
                
        // Check if there are two floors with the same distance to current floor
        for(int i = 0; i < longestWaitFloor->getLength(); i++){
            int distance;
            
            if(longestWaitFloor->valueOfIndex(i) > currentFloor){
                distance = longestWaitFloor->valueOfIndex(i) - currentFloor;
            } else
                distance = currentFloor - longestWaitFloor->valueOfIndex(i);
            
            cout << "Floor " << longestWaitFloor->valueOfIndex(i) << " has a distance of " << distance << " from current floor" << endl;
            if(distance < minDistanceA){
                minDistanceA = distance;
                floorIndexA = i;
            } else if(distance == minDistanceA){
                minDistanceB = distance;
                floorIndexB = i;
            }
        }
        
        if(minDistanceA < minDistanceB){                                    // One floor is closer than the other
            int nextFloor = longestWaitFloor->valueOfIndex(floorIndexA);
            delete longestWaitFloor;
            return nextFloor;
        } else {                                                            // Both floors has the same distance
            
            // Select the lowest floor
            if(longestWaitFloor->valueOfIndex(floorIndexA) < longestWaitFloor->valueOfIndex(floorIndexB)){
                int nextFloor = longestWaitFloor->valueOfIndex(floorIndexA);
                delete longestWaitFloor;
                return nextFloor;
            } else {
                int nextFloor = longestWaitFloor->valueOfIndex(floorIndexB);
                delete longestWaitFloor;
                return nextFloor;
            }
        }
        
    }
    
}

int building::getPassengerGoingDown(int floorNo){
    floorStruct *temp = start;

    while(temp != NULL){
        
        if(temp->floor == floorNo){
            int destination = temp->passengerQueue->getItemSmallerThan(floorNo);
            
            if(destination >= 0){
                numberOfPassengers--;
                return destination;
            
            } else
                return -1;
        } else
            temp = temp->next;
    }
    return -1;                                                  // Floor not found
}

int building::getPassengerGoingUp(int floorNo){
    floorStruct *temp = start;
    
    while(temp != NULL){
        
        if(temp->floor == floorNo){
            int destination = temp->passengerQueue->getItemBiggerThan(floorNo);
            
            if(destination >= 0){
                numberOfPassengers--;
                return destination;
            
            } else
                return -1;
        } else
            temp = temp->next;
    }
    return -1;                                                  // Floor not found
}

int building::getPassengerFront(int floorNo){
    floorStruct *temp;
    temp = start;
    while(temp->next != NULL && temp->floor != floorNo){
        temp = temp->next;
    }
    
    if(temp->floor == floorNo){
        int destination = temp->passengerQueue->getFrontItem();
        
        if(destination >= 0){       // if valid destination
            numberOfPassengers--;
            return destination;
        } else
            return -1;
    } else
        return -1;                                                  // Floor not found
}

bool building::isEmpty(int floorNo) const{
    floorStruct *temp;
    temp = start;
    
    while(temp->next != NULL && temp->floor != floorNo){
        temp = temp->next;
    }
    
    if(temp->floor == floorNo)
        return temp->passengerQueue->isEmpty();
    else
        return true;                                        // Floor not found
}

int building::totalPassengersWaiting(void) const{
    return numberOfPassengers;
}

void building::incrementFloorWaitTime(int time){
    floorStruct *temp = start;
    while(temp != NULL){
        temp->timeSinceElevatorVisit = temp->timeSinceElevatorVisit + time;
        temp = temp->next;
    }
}

void building::resetFloorWaitTime(int floorNo){
    floorStruct *temp = start;
    while(temp->next != NULL && temp->floor != floorNo){
        temp = temp->next;
    }
    
    if(temp->floor == floorNo)
        temp->timeSinceElevatorVisit = 0;
}

int building::getFloorWaitTime(int floorNo) const{
    floorStruct *temp = start;
    while(temp->next != NULL && temp->floor != floorNo){
        temp = temp->next;
    }
    
    if(temp->floor == floorNo)
        return temp->timeSinceElevatorVisit;
    else
        return -1;
}
