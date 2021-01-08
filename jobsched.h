#ifndef JSC_H
#define JSC_H

#include "queuej.h"
#include "graph_header.h"
#include "misc_functions.h"

#define QSZ 10
#define STA 1
#define DYN2 2
#define DYN3 3

using namespace std;

class CC;
struct ranks;
class SCC;

class JobScheduler{
    private:
        int execution_threads; // number of execution threads
        pthread_t* tids; // execution threads
    public:
        Index* indin;
        Index* indout;
        ranks* granks;
        int type;
        CC* ccom;
        SCC* sccom;
        JQueue* que; // a queue that holds submitted jobs / tasks
        JobScheduler(int,Index*,Index*,int,SCC*,CC*,ranks*);
        ~JobScheduler();
        pthread_mutex_t mtx ;
        pthread_cond_t non_empty;
        void submit_job(job*);
        void execute_all_jobs();
        void wait_all_tasks_finish(); //waits all submitted tasks to finish
        //void resetq();
        // mutex, condition variable, ...
};

void* execute(void*);

#endif // JSC_H
