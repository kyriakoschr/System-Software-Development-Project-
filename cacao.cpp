#ifndef FUNC_G
#define FUNC_G

#include "graph_header.h"

int Index::insert_node(uint32_t id1, uint32_t id2,uint32_t version)
{
	/* ID 1 */
	uint32_t cap=getSOI(); // cap stands for capacity of index
    if (id1>=cap) { //index position not exist
        return -1;//realloc index
    } else {
        if (IndexCells[id1]==UNUSED) { // first allocation for node id1
            uint32_t temp=buffer->allocNewNode();
            if (temp==ERROR){
                return -2;//realloc buffer
            } else {
                IndexCells[id1]=temp;
                if(buffer->insertedge(IndexCells[id1],id2,version) == OK_SUCCESS) {
                    return OK_SUCCESS;
                } else {
                    return -3;
                }
            }
        } else { // already alocated
            if(buffer->insertedge(IndexCells[id1],id2,version)==OK_SUCCESS) {
                    return OK_SUCCESS;
            } else {
                return -4;
            }
        }
    }
}

Index::Index(){
    buffer= new Buffer();
    sizeofindex=BUFSIZE;
    IndexCells =new uint32_t[sizeofindex];
    for(int i = 0; i < sizeofindex; i++) {
        IndexCells[i] = UNUSED;
    }
}

void Index::realloc_buffer(){
    buffer->reallocBUF();
}

Index::Index(Index& source){
    IndexCells=new uint32_t[source.getSOI()*2];
    sizeofindex=source.sizeofindex*2;
    memcpy(IndexCells,source.IndexCells,sizeof(uint32_t)*source.getSOI());
    buffer = source.buffer;
    for(uint32_t i = source.getSOI(); i <getSOI(); i++){
        IndexCells[i] = UNUSED;
    }
}

Index::~Index(){
    delete[] IndexCells;
}

uint32_t* Index::getNeighbors(uint32_t& counter,uint32_t offset)
{
    return buffer->getNeighbors(counter,offset);
}

uint32_t* Index::getEdgePr(uint32_t& counter,uint32_t offset)
{
    return buffer->getEdgePr(counter,offset);
}

#endif
