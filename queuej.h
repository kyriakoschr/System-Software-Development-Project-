#ifndef QUEUEJ_H
#define QUEUEJ_H

#include <iostream>
#include <cstdlib>
#include <inttypes.h>
#include <unistd.h>
#define WL 350
#define QU true
#define AD false

using namespace std;

class job{
    private:

    public:
        bool qORa;
        uint32_t source;
        uint32_t target;
        uint32_t version;
        job(bool type,uint32_t sourc,uint32_t targ,uint32_t ver){version=ver;qORa=type;source=sourc;target=targ;};
};

class JQueue{
    private:
        job** array;
        uint32_t front;
        uint32_t rear;
        uint32_t size;
        uint32_t counter;
    public:
        uint32_t* results;
        JQueue(uint32_t);
        void push(job*);
        job** pop(int&,int&);
        void print();
        void reset();
        ~JQueue();
        /*void display();
        int is_empty();
        void emptyQ();*/
};

#endif // QUEUEJ_H
