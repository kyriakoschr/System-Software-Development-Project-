
#include "graph_header.h"

List_node::List_node(){
    neighbors = new uint32_t[N];
    edgeProperty = new uint32_t[N];
    for(uint32_t i=0;i<N;i++) {
        neighbors[i]=UNUSED;
        edgeProperty[i]=UNUSED;
    }
    nextListNode = UNUSED;
    counter = 0;
}

List_node::~List_node(){
    delete[] neighbors;
    delete[] edgeProperty;
}

Buffer::Buffer(){
    SizeOfBuffer=BUFSIZE;
    BufferNodes = new List_node*[SizeOfBuffer];
    for(uint32_t i=0;i<SizeOfBuffer;i++)
        BufferNodes[i]=NULL;
    UsedNodes = 0;
}

Buffer::~Buffer(){
    for(uint32_t i=0;i<SizeOfBuffer;i++)
        delete BufferNodes[i];
    delete[] BufferNodes;
}

void Buffer::reallocBUF(){ // doubles the buffer array of list nodes
    List_node** array2=new List_node*[SizeOfBuffer*2];
    /*for(uint32_t i = 0; i < SizeOfBuffer; i++){
    List_node* newarray = new List_node[SizeOfBuffer*2]; //creates a new array
    for(uint32_t i = 0; i < SizeOfBuffer; i++){
        uint32_t tempcntr=BufferNodes[i].getCNTR();
        for(uint32_t j = 0; j < tempcntr; j++){
            newarray[i].setneighbor(j,BufferNodes[i].getneighbor(j));
            BufferNodes[i].setneighbor(j,UNUSED);
        }
        newarray[i].setCNTR(tempcntr); //set the counter as the old one
        newarray[i].setNLN(BufferNodes[i].getNLN());
    }*/
    memcpy(array2,BufferNodes,sizeof(List_node*)*SizeOfBuffer);
    for(uint32_t i=SizeOfBuffer;i<SizeOfBuffer*2;i++)
        array2[i]=NULL;
    //memcpy(newarray,BufferNodes,sizeof(List_node)*SizeOfBuffer);
    delete[] BufferNodes;
    BufferNodes=array2;
    SizeOfBuffer*=2;
}

uint32_t Buffer::allocNewNode(){
    if(UsedNodes==SizeOfBuffer)
        return ERROR;
    else{
        BufferNodes[UsedNodes]=new List_node();
        UsedNodes++;
    }
    return UsedNodes-1;
}

void List_node::setneighbor(uint32_t position,uint32_t neighbor,uint32_t version){
    neighbors[position]=neighbor;
    edgeProperty[position]=version;
    //cout<<version<<endl;
    counter++;
}

int Buffer::insertedge(uint32_t offset,uint32_t edge,uint32_t version){
    uint32_t tempoffset=offset;
    while(BufferNodes[tempoffset]->getNLN()!=UNUSED) {
        tempoffset=BufferNodes[tempoffset]->getNLN();
    }
    if(BufferNodes[tempoffset]->getCNTR()==N) {
        uint32_t newoffset=allocNewNode();
        if(newoffset==ERROR)
            return-1;
//        BufferNodes[newoffset]=new List_node();
        BufferNodes[tempoffset]->setNLN(newoffset);
        tempoffset=newoffset;
    }
    BufferNodes[tempoffset]->setneighbor(BufferNodes[tempoffset]->getCNTR(),edge,version);
    return OK_SUCCESS;
}

uint32_t* Buffer::getEdgePr(uint32_t &counter_rtn,uint32_t offset){
    uint32_t sizeofarray = N;
    uint32_t* pinakas = new uint32_t[sizeofarray];
    for(uint32_t i = 0;i < N; i++) {
        pinakas[i]=UNUSED;
    }
    uint32_t* reallocptr;
    uint32_t temp=offset;
    uint32_t pinakascounter=0;
    do{
        for(int i=0;i<BufferNodes[temp]->getCNTR();i++){
            if(pinakascounter >= sizeofarray){
                reallocptr=new uint32_t[sizeofarray*2];
                for(uint32_t i = sizeofarray; i < sizeofarray*2;i++) {
                    reallocptr[i]=UNUSED;
                }
                memcpy(reallocptr,pinakas,sizeofarray*sizeof(uint32_t));
                sizeofarray*=2;
                delete[] pinakas;
                pinakas=reallocptr;
                reallocptr=NULL;
            }
            pinakas[pinakascounter]=BufferNodes[temp]->getversion(i);
            pinakascounter++;
        }
        temp=BufferNodes[temp]->getNLN();
    }while(temp!=UNUSED);
    counter_rtn=pinakascounter;
    return pinakas;
}


uint32_t* Buffer::getNeighbors(uint32_t &counter_rtn,uint32_t offset){
    uint32_t sizeofarray = N;
    uint32_t* pinakas = new uint32_t[sizeofarray];
    for(uint32_t i = 0;i < N; i++) {
        pinakas[i]=UNUSED;
    }
    uint32_t* reallocptr;
    uint32_t temp=offset;
    uint32_t pinakascounter=0;
    do{
        for(int i=0;i<BufferNodes[temp]->getCNTR();i++){
            if(pinakascounter >= sizeofarray){
                reallocptr=new uint32_t[sizeofarray*2];
                for(uint32_t i = sizeofarray; i < sizeofarray*2;i++) {
                    reallocptr[i]=UNUSED;
                }
                memcpy(reallocptr,pinakas,sizeofarray*sizeof(uint32_t));
                sizeofarray*=2;
                delete[] pinakas;
                pinakas=reallocptr;
                reallocptr=NULL;
            }
            pinakas[pinakascounter]=BufferNodes[temp]->getneighbor(i);
            pinakascounter++;
        }
        temp=BufferNodes[temp]->getNLN();
    }while(temp!=UNUSED);
    counter_rtn=pinakascounter;
    return pinakas;
}
