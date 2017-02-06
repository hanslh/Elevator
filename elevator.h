#ifndef ELEVATOR_H
#define ELEVATOR_H

#define TRANSITION_TIME 5
#define STOP_TIME 25
#define CAPACITY 4
#define EMPTY_SLOT -99
#define UP_DIRECTION 1
#define DOWN_DIRECTION 0

class elevator
{
public:
    elevator(int startFloor);
    ~elevator();
    // returns true if elevator is empty
    bool isEmpty(void) const;
    // returns true if elevator is full
    bool isFull(void) const;
    // returns current elevator current floor
    int getCurrentFloor(void) const;
    // Direction 1=up, 0=down
    int getDirection(void) const;
    // 0 = down, 1 = up
    void setDirection(int directionValue);
    // returns time of floor departure. Stop time must be subtracted for correct destination time
    int getElapsedTime(void) const;
    // load passenger if free space. Also updates direction automatically when empty and first passenger enters
    bool loadPassenger(int destination);
    // Returns false if passenger with current floor as destination does not exist. Else returns true and removes passenger from elevator
    bool offloadCurrentFloorPassenger(int *departureFloor, int *departureTime, int *arrivalTime);
    // Return true if elevator has stopped at current floor 
    bool hasElevatorStopped(void);
    // Updates current floor to the value of nextFloor and return time increment for the elevator run
    int run(int nextFloor);
    
    int getPassengerCount(void) const;
private:
    int passengerCount;
    int direction;
    int currentFloor;
    int elapsedTime;
    int endDestinationFloor;
    bool elevatorStopFlag;
    
    // Set elevator stop flag. If set, run-function will increment elapsed time by STOP_TIME seconds
    void setElevatorStopFlag(void);
    // Clears elevator stop flag.
    void clearElevatorStopFlag(void);
    
    struct passenger{
        int destinationFloor;
        int departureFloor;
        int departureTime;
    };
    // array of pointers to passanger struct
    passenger *passengerSlot[CAPACITY];
};

#endif // ELEVATOR_H
