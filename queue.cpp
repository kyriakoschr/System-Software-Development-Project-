#include "queue.h"
#include "graph_header.h"

Queue::Queue(uint32_t sz){
/*Queue::Queue(){
    rear = NULL;
    front = NULL;*/
    //
    array = new uint32_t[sz];
    size = sz;
    rear = 0;
    front = 0;
}

void Queue::push(uint32_t data){
    /*node *temp = new node;
    temp->info = data;
    temp->next = NULL;
    if(front == NULL){
        front = temp;
    }else{
        rear->next = temp;
    }
    rear = temp;*/
    //
    if(rear + 1 > size) {
        uint32_t* array2 = new uint32_t[size * 2];
        memcpy(array2,array,sizeof(uint32_t)*size);/*
        for(uint32_t i = 0; i < size; i++) {
            array2[i] = array[i];
        }*/
        delete[] array;
        array = array2;
        size *= 2;
    }
    array[rear] = data;
    rear++;
}

uint32_t Queue::pop(){
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
    if(front + 1 > rear) {
        return UNUSED;
    }
    uint32_t temp = array[front];
    array[front] = UNUSED;
    front++;
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
}*/

int Queue::is_empty()
{
    if(front == rear){
        return 1;
    }else{
        return 0;
    }
}

void Queue::emptyQ(){
    /*while(is_empty()==0)
    {
        uint32_t temp=pop();
    }
    delete front;*/
    for(uint32_t i = 0; i < size; i++) {
        array[i] = UNUSED;
    }
    //delete[] array;
}

Queue::~Queue() {
    delete[] array;
}
