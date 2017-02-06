#ifndef QUEUE_H
#define QUEUE_H

class queue
{
public:
    queue();
    ~queue();
    bool appendItem(int value);                 // Add new item at end of queue
    bool insertItem(int value);                 // Insert new item at front of queue
    int valueFrontItem(void) const;             // Return value of item at front of queue. If empty return -1
    int getFrontItem(void);                     // Return item value at front of queue and delete item. If empty, return -1
    int valueEndItem(void) const;               // Return value of item at end of queue. If emtpy, return -1
    int getEndItem(void);                       // Return item value at end of queue and delete item. If empty, return -1
    int valueOfIndex(int index) const;          // Return value of index in queue
    bool hasItem(int value) const;              // Return true if value is found. If not found, else return false
    bool getItem(int value);                    // Return true if value is found and remove item from queue  
    bool hasItemBiggerThan(int value) const;
    int getItemBiggerThan(int value);           // First item in queue with value bigger than. Returns item value and removes item from queue
    bool hasItemSmallerThan(int value) const;
    int getItemSmallerThan(int value);          // First item in queue with value smaller than. Returns item value and removes item from queue
    void makeEmpty(void);                       // Delete all queue entries 
    bool isEmpty(void) const;
    bool isFull(void) const;
    int getLength(void) const;
private:
    struct node{
        int itemValue;
        node *next;
        node *previous;
    };
    node *start;
    node *end;
    int length;
};

#endif // QUEUE_H
