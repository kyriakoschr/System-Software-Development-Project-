#include "queuej.h"
#include "graph_header.h"

JQueue::JQueue(uint32_t sz){
/*Queue::Queue(){
    rear = NULL;
    front = NULL;*/
    //
    array = new job*[sz];
    results = new uint32_t[sz]();
    for(uint32_t i=0;i<sz;i++){
        array[i]=NULL;
    }
    size = sz;
    rear = -1;
    counter= 0;
    front = 0;
}

void JQueue::reset(){
    for(uint32_t i=0;i<size;i++){
        results[i]=0;
    }
    rear = -1;
    counter= 0;
    front = 0;
}

JQueue::~JQueue(){
    for(uint32_t i=0;i<size;i++)
            delete array[i];
    delete[] array;
    delete[] results;
}

void JQueue::push(job* data){
    /*node *temp = new node;
    temp->info = data;
    temp->next = NULL;
    if(front == NULL){
        front = temp;
    }else{
        rear->next = temp;
    }
    rear = temp;*/

    job** array2;
    uint32_t* results2;
    if(counter==size){
        array2=new job*[2*size];
        results2=new uint32_t[2*size]();
        memcpy(array2,array,sizeof(job*)*(counter));
        memcpy(results2,results,size*sizeof(uint32_t));
        for(uint32_t i=size;i<size*2;i++){
            array2[i]=NULL;
            results2[i]=0;
        }
        front=0;
        rear=size-1;
        size=size*2;
        delete[] array;
        delete[] results;
        results=results2;
        array=array2;
    }
    rear=(rear + 1) % size;
    array[rear]=data;
    counter++;
    //cout<<rear<<" "<<array[rear]<<" "<<counter<<endl;
}

void JQueue::print(){
    uint32_t i=0;
    while(results[i]!=0){
        if(results[i]==UNUSED+2)
            cout<<-1<<endl;
        else
            cout<<results[i]<<endl;
        i++;
    }
}

job** JQueue::pop(int& cntr,int& startpos){
    /*node *temp; //= new node;
    uint32_t result;
    if(front == NULL){
        result = -1;
    }else{
        temp = front;
        front = front->next;
        result = temp->info;
        delete temp;
    }
    return result;*/
    if(counter==0){
        cntr=0;
        startpos=0;
        return NULL;
    }
    else if(counter<WL){
        cntr=counter;
    }
    else{
        cntr=WL;
    }
    startpos=front;
    //cout<<counter<<" "<<cntr<<" "<<startpos<<endl;
    job** temp=new job*[cntr];
    for(uint32_t i=0;i<cntr;i++){
        temp[i]=array[front+i];
        array[front+i]=NULL;
        counter--;
    }
    front=(front + cntr) % size;
    return temp;
}

/*void Queue::display(){
    node *p = new node;
    p = front;
    if(front == NULL){
        cout<<"\nNothing to Display\n";
    }else{
        while(p!=NULL){
            cout<<endl<<p->info;
            p = p->next;
        }
    }
}

int Queue::is_empty()
{
    if(front == rear){
        return 1;
    }else{
        return 0;
    }
}

void Queue::emptyQ(){
   *//*while(is_empty()==0)
    {
        uint32_t temp=pop();
    }
    delete front;*//*
    for(uint32_t i=0;i<size;i++)
        delete array[i];
    delete[] array;

}*/
