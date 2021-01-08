#ifndef STACK_H
#define STACK_H

#include <iostream>
#include <cstdlib>
#include <inttypes.h>
#include "queue.h"

using namespace std;

class Stack {
    private:
        //struct node *ttop;
        uint32_t ttop;
        uint32_t* array;
        uint32_t size;
        bool* onstack;
    public:
        Stack(uint32_t sz) 
        {
            ttop = 0;
            size = sz >> 5;
            array = new uint32_t[size];
            onstack = new bool[sz]();
        }
        void push(uint32_t); // to insert an element
        void pop();  // to delete an element
        uint32_t top(); // returns top item 
        void empty();
        ~Stack();
        bool isonstack(uint32_t);
        //void show(); // to show the stack
};

#endif 
