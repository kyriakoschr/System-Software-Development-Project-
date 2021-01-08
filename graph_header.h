#ifndef HEAD_H
#define HEAD_H

#include <stdlib.h>
#include <iostream>
#include <stdint.h>
#include <string.h>
#include <fstream>
#include <ctime>

#define OK_SUCCESS 1
#define UNUSED 4294967293
#define ERROR 4294967293
#define N 32
#define BUFSIZE 400

using namespace std;

//extern int UsedNodes;
//extern int SizeOfBuffer;
//extern int N;
//extern uint32_t DESTRcntr;
//extern uint32_t CONSTRcntr ;


class List_node {
    private:
        uint32_t* neighbors;
        uint32_t* edgeProperty;
        uint32_t nextListNode;
    public:
        uint32_t counter;
        void setCNTR(uint32_t newcntr){counter=newcntr;};
        List_node();
        ~List_node();
        uint32_t getCNTR(){return counter;};
        uint32_t getNLN(){return nextListNode;};
        void setNLN(uint32_t newnln){nextListNode=newnln;};
        void setneighbor(uint32_t,uint32_t,uint32_t);
        uint32_t getneighbor(uint32_t target){return neighbors[target];};
        uint32_t getversion(uint32_t target){return edgeProperty[target];};
};

class Buffer {
    private:
        List_node** BufferNodes;
        Buffer* getListNode();
        uint32_t UsedNodes;
        uint32_t SizeOfBuffer ; // self-explanatory and size of Index, since they grow together
    public:
        uint32_t allocNewNode();
        Buffer();
        Buffer(Buffer&);
        //Buffer(uint32_t);
        ~Buffer();
        int insertedge(uint32_t,uint32_t,uint32_t);
        uint32_t getSOB(){return SizeOfBuffer;};
        void setSOB(){SizeOfBuffer*=2;};
        uint32_t* getNeighbors(uint32_t&,uint32_t);
        uint32_t* getEdgePr(uint32_t&,uint32_t);
        List_node** getBUF(){return BufferNodes;};
        void reallocBUF();
};

class Index {
    private:
    public:
        uint32_t sizeofindex;
        Buffer* buffer;// for testing purposes, remember to set private
        uint32_t* IndexCells; // for testing purposes, remember to set private
        int insert_node(uint32_t id1, uint32_t id2,uint32_t version=0);
        uint32_t getListhead(uint32_t pos){
            //cout << "GET LIST HEAD" << endl;
            //cout << pos <<" "<<IndexCells[pos]<< endl;
            return IndexCells[pos];};
        void realloc_buffer();
        uint32_t* getNeighbors(uint32_t&,uint32_t);
        uint32_t* getEdgePr(uint32_t&,uint32_t);
        uint32_t getSOI(){return sizeofindex;};
        void setSOI(){buffer->setSOB();};
        void emptyBUF(){delete buffer;};
        Index();
        Index(Index&);
        ~Index();
};

typedef struct ptr{
    Index* in;
    Index* out;
} ptr;

#endif // HEAD_H
