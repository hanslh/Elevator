#include <iostream>
#include "queue.h"
using namespace std;

queue::queue()
{
    start = NULL;
    end = NULL;
    length = 0;
}

queue::~queue()
{
    makeEmpty();
}

// Add item to end of queue
bool queue::appendItem(int value){
    if(!isFull()){
        node *temp = new node;
        if(end == NULL){        // If queue is empty
            start = temp;
            end = temp;
            temp->itemValue = value;
            temp->previous = NULL;
            temp->next = NULL;
            length++;
            return true;
        } else {
            end->next = temp;
            temp->itemValue = value;
            temp->previous = end;
            temp->next = NULL;
            end = temp;
            length++;
            return true;
        }
    } else
        return false;
}

bool queue::insertItem(int value){
    if(!isFull()){
        node *temp = new node;
        temp->next = start;
        temp->previous = NULL;
        temp->itemValue = value;
        start->previous = temp;
        start = temp;
        length++;
        return true;
    } else
        return false;
}

int queue::valueFrontItem(void) const{
    if(!isEmpty()){
        cout << "Getting value from front item" << endl;
        int frontItem = start->itemValue;
        cout << "Front item is " << frontItem << endl;
        return frontItem;
    }
    return -1;
}

int queue::getFrontItem(void){
    if(!isEmpty()){
        int value;
        node *temp = start;
        
        start = temp->next;
        value = temp->itemValue;
        
        if(start == NULL){      // If only remaining item in queue
            
            end = NULL;
            delete temp;
            length--;
            return value;
        
        } else {
            
            start->previous = NULL;
            delete temp;
            length--;
            return value;
        
        }
    } else
        return -1;
}

int queue::valueEndItem(void) const{
    return end->itemValue;
}

int queue::getEndItem(void){
    if(!isEmpty()){
        if(length == 1){
            return getFrontItem();
        } else {
            int value;
            node *temp = end;
            
            end = end->previous;
            end->next = NULL;
            value = temp->itemValue;
            
            delete temp;
            length--;
            return value;
        }
    } else
        return -1;      // Queue is empty
}

int queue::valueOfIndex(int index) const{
    if(index >= 0 && index < length){
        node *temp = start;

        for(int i = 0; i <= index; i++){
            if(i == index)
                return temp->itemValue;
            else
                temp = temp->next;
        }

    } else
        return -1;                              // Invalid index
}
    
bool queue::hasItem(int value) const{
    node *temp = start;
    
    while(temp->next != NULL && temp->itemValue != value){
        temp = temp->next;
    }
    
    if(temp->itemValue == value){
        return true;                    // Item found
    } else 
        return false;                   // Item not found
}

bool queue::getItem(int value){
    if(start != NULL){
        node *temp = start;
        
        if(start->itemValue == value){              // If match on first item in queue
            
            start = start->next;
            if(start == NULL)                       // If only item in queue
                end = NULL;
            else
                start->previous = NULL;
            
            delete temp;
            length--;
            return true;
        
        } else {
        
            while(temp->next != NULL && temp->itemValue != value){  // Increment until match or end of queue
                temp = temp->next;
            }
            
            if(temp->itemValue == value){
                if(temp->next == NULL){         // if match on last element in queue
                    temp->previous->next = NULL;
                    end = temp->previous;
                    delete temp;
                    length--;
                    return true;
                } else {
                    temp->previous->next = temp->next;
                    temp->next->previous = temp->previous;
                    delete temp;
                    length--;
                    return true;
                }
            } else                              // End of queue
                return false;                   // Item not found
        }
    }
    return false;                               // Empty queue
}

bool queue::hasItemBiggerThan(int value) const{
    node *temp = start;
    
    while(temp->next != NULL && temp->itemValue <= value){
        temp = temp->next;
    }
    
    if(temp->itemValue > value)
        return true;                    // Item found
    else
        return false;                   // Item not found 
}
    
int queue::getItemBiggerThan(int value){
    
    if(start != NULL){
        int returnValue;
        node *temp = start;
        
        if(start->itemValue > value){              // If match on first item in queue
            start = start->next;
            
            if(start == NULL)                       // If only item in queue
                end = NULL;
            else
                start->previous = NULL;
            returnValue = temp->itemValue;
            delete temp;
            length--;
            return returnValue;

        } else {
            
            while(temp->next != NULL && temp->itemValue <= value){  // Increment until match or end of queue
                temp = temp->next;
            }
            
            if(temp->itemValue > value){
                if(temp->next == NULL){         // if match on last element in queue
                    
                    temp->previous->next = NULL;
                    end = temp->previous;
                    returnValue = temp->itemValue;
                    delete temp;
                    length--;
                    return returnValue;
                    
                } else {
                    
                    temp->previous->next = temp->next;
                    temp->next->previous = temp->previous;
                    returnValue = temp->itemValue;
                    delete temp;
                    length--;
                    return returnValue;
                    
                }
            } else                              // End of queue
                return -1;                      // Item not found
        }
    }
    return -1;                               // Empty queue
}

bool queue::hasItemSmallerThan(int value) const{
    node *temp = start;
    
    while(temp->next != NULL && temp->itemValue >= value){
        temp = temp->next;
    }
    
    if(temp->itemValue < value)
        return true;                    // Item found
    else
        return false;                   // Item not found 
}
    
int queue::getItemSmallerThan(int value){
    if(start != NULL){
        int returnValue;
        node *temp = start;
        
        if(start->itemValue < value){              // If match on first item in queue
            start = start->next;
            
            if(start == NULL)                       // If only item in queue
                end = NULL;
            else
                start->previous = NULL;
            
            returnValue = temp->itemValue;
            delete temp;
            length--;
            return returnValue;
            
        } else {
            
            while(temp->next != NULL && temp->itemValue >= value){  // Increment until match or end of queue
                temp = temp->next;
            }
            
            if(temp->itemValue < value){
            
                if(temp->next == NULL){         // if match on last element in queue
                    
                    temp->previous->next = NULL;
                    end = temp->previous;
                    returnValue = temp->itemValue;
                    delete temp;
                    length--;
                    return returnValue;
                
                } else {
                    
                    temp->previous->next = temp->next;
                    temp->next->previous = temp->previous;
                    returnValue = temp->itemValue;
                    delete temp;
                    length--;
                    return returnValue;
                    
                }
            } else                              // End of queue
                return -1;                      // Item not found
        }
    }
    return -1;                                  // Empty queue
}

void queue::makeEmpty(void){
    node *temp;
    while(start != NULL){
        temp = start;
        start = start->next;
        delete temp;
    }
    length = 0;
}

bool queue::isEmpty(void) const{
    if(!length)
        return true;
    else
        return false;
}

bool queue::isFull(void) const{
    try{
        node *temp = new node;
        delete temp;
        return false;
    }
    catch(bad_alloc exception){
        return true;
    }
}

int queue::getLength(void) const{
    return length;
}