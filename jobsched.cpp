#include "jobsched.h"

using namespace std;

JobScheduler::JobScheduler(int thr,Index* in,Index*out,int typ,SCC* scc,CC* concomps,ranks* rankIndex){
    execution_threads=thr;
    que=new JQueue(QSZ);
    type=typ;
    granks=rankIndex;
    sccom=scc;
    ccom=concomps;
    tids=new pthread_t[execution_threads];
    if(pthread_mutex_init(&mtx,0)!=0)
        cerr<<"error in creating mtx"<<endl;
	if(pthread_cond_init(&non_empty,0)!=0)
        cerr<<"error in creating condition variable"<<endl;
    indin=in;
    indout=out;
}

JobScheduler::~JobScheduler(){
    delete que;
    delete[] tids;
    if(pthread_mutex_destroy(&mtx)!=0)
        cerr<<"error in destroying mtx"<<endl;
    if(pthread_cond_destroy(&non_empty)!=0)
        cerr<<"error in destroying condition variable"<<endl;
}

void JobScheduler::execute_all_jobs(){
	for(int i=0;i<execution_threads;i++)
		if(pthread_create(&tids[i],0,execute,this)!=0)
            cerr<<"error in pthread create"<<endl;
}

void JobScheduler::wait_all_tasks_finish(){
	for(int i=0;i<execution_threads;i++)
		if(pthread_join(tids[i],0)!=0)
            cerr<<"error in pthread join"<<endl;
    que->print();
    que->reset();
}
/*
void JobScheduler::resetq(){
    delete que;
    que=new JQueue(QSZ);
}
*/
void JobScheduler::submit_job(job* newjob){
    if(pthread_mutex_lock(&mtx)!=0)
        cerr<<"error in locking"<<endl;
    que->push(newjob);
    //cout<<"PHSHED"<<endl;
    if(pthread_mutex_unlock(&mtx)!=0)
        cerr<<"error in unlocking"<<endl;
}

void* execute(void* myptr){
    JobScheduler* ptr=(JobScheduler*)myptr;
    job** myjobs;
    while(1){
        if(pthread_mutex_lock(&(ptr->mtx))!=0)
            cerr<<"error in locking"<<endl;
        int counter=0;
        int startpos=0;
        myjobs=ptr->que->pop(counter,startpos);
        if(counter==0){
            if(pthread_mutex_unlock(&(ptr->mtx))!=0)
                cerr<<"error in unlocking"<<endl;
            break;
        }
        if(pthread_mutex_unlock(&(ptr->mtx))!=0)
            cerr<<"error in unlocking"<<endl;
        for(uint32_t i=0;i<counter;i++){
            //cout<<pthread_self()<<" "<<i<<" "<<counter<<" "<<endl;
            if(ptr->type==STA){
                uint32_t res=st_bbfs(ptr->granks,ptr->indin,ptr->indout,ptr->sccom ,myjobs[i]->source,myjobs[i]->target,0);
                ptr->que->results[i+startpos]=res;
            }
            else if((ptr->type==DYN2)||(ptr->type==DYN3)){
                uint32_t res=ptr->ccom->qcheck(myjobs[i]->source,myjobs[i]->target,ptr->indin,ptr->indout,myjobs[i]->version);
                ptr->que->results[i+startpos]=res;
            }
            delete myjobs[i];
        }
        delete[] myjobs;
    }
    return NULL;
}
