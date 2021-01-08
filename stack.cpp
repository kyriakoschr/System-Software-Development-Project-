#include "stack.h"
#include "graph_header.h"

// PUSH Operation
void Stack::push(uint32_t value)
{
    //cout << "pushing things in stack val: " << value << " \tttop: " << ttop << endl;
    /*struct node *ptr;
    ptr = new node;
    ptr->info = value;
    ptr->next = NULL;
    if(ttop != NULL) {
        ptr->next=ttop;
    }
    ttop = ptr;*/
    if(ttop + 1 > size) {
        uint32_t* array2 = new uint32_t[size * 2];/*
        for(uint32_t i = 0; i < size; i++) {
            array2[i] = array[i];
        }*/
        memcpy(array2,array,sizeof(uint32_t)*size);
        delete[] array;
        array = array2;
        size *= 2;
    }
    array[ttop] = value;
    ttop++;
    onstack[value] = true;
}

// POP Operation
void Stack::pop()
{
    //cout << "***********************poping things in stack ttop: " << ttop  - 1<< endl;

    /*struct node *temp;
    if(ttop == NULL) {
        cout<<"nThe stack is empty!!!";
    }
    temp = ttop;
    ttop = ttop->next;
    delete temp;*/
    if(ttop - 1 > 0) {
        onstack[array[ttop - 1]] = false;
        array[ttop - 1] = UNUSED;
        ttop--;
    } else {
        //cout << "Stack is empty." << endl;
        //exit(0);
    }
}

uint32_t Stack::top()
{
    //return ttop->info;
    //cout << "returning: " << array[ttop - 1] << " ttop: " << ttop - 1<< endl;
    return array[ttop - 1];
}

void Stack::empty()
{
    /*while(ttop != NULL) {
        pop();
    }*/
    delete[] array;
    delete[] onstack;
}

Stack::~Stack()
{
    /*while(ttop != NULL) {
        pop();
    }*/
    delete[] array;
    delete[] onstack;
}

bool Stack::isonstack(uint32_t node)
{
    if(onstack[node] == true) {
        return true;
    } else {
        return false;
    }
}
/*
// Show stack
void stack::show()
{
    struct node *ptr1=ttop;
    cout<<"nThe stack isn";
    while(ptr1!=NULL)
    {
        cout<<ptr1->info<<" ->";
        ptr1=ptr1->next;
    }
    cout<<"NULLn";
}
*/
