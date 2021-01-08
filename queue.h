#ifndef QUEUE_H
#define QUEUE_H

#include <iostream>
#include <cstdlib>
#include <inttypes.h>

using namespace std;

struct node {
    uint32_t info;
    struct node *next;
};

class Queue{
    private:
        /*node *rear;
        node *front;*/
        uint32_t* array;
        uint32_t front;
        uint32_t rear;
        uint32_t size;
    public:

        //Queue();
        Queue(uint32_t sz);
        void push(uint32_t data);
        uint32_t pop();
        void display();
        int is_empty();
        void emptyQ();
        uint32_t seehead(){return array[front];};
        ~Queue();
};

#endif // QUEUE_H
