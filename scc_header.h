#ifndef SCC_H
#define SCC_H

#include "stack.h"

typedef struct pin_di {
        uint32_t di;
        uint32_t node;
}pin_di;

class Component { //strongly connecting component
    public:
        uint32_t id; // lowlink num
        uint32_t di; // arthimena ta id
        uint32_t nodes_count;// num of nodes
        uint32_t* nodes_ids; //array of nodes
        void add(uint32_t);
        Component(uint32_t id, uint32_t size, uint32_t count);
        ~Component();
};

class CompCursor;

class SCC {
    public:
        Component** comps; //array of components
        uint32_t comp_count; //posa component exei to scc
        uint32_t foo_counter;//counter gia to exp_di
        uint32_t max; //size of scc
        uint32_t* id_belongs_to_comps; //id_belong_to_comps(g)=lowlink(g)
        pin_di* exp_di; //explored array with di
        uint32_t findNodeSCCid(uint32_t node_id); // returns ID of the SCC that the node belongs to
        uint32_t findSCCpos(uint32_t id);
        //uint32_t add(Component*);
        void set(uint32_t, uint32_t, uint32_t);
        int iterateSCC(CompCursor* cursor);
        void clean(void); // deletes any empty components
        SCC(uint32_t size);
        ~SCC();
};

class CompCursor {
    public:
        Component* comp_ptr;
        bool next_SCC(SCC* scc);
        CompCursor(void);
        ~CompCursor();
};

SCC* estimateSCC(Index* index); // contains tarjan
int estimateShortestPathSCC(SCC* scc, Index* index, uint32_t source, uint32_t target); // BBFS between source and target of the same SCC -- important to develop ONLY nodes that belong to the same SCC, otherwise returns -1

void tarjan(uint32_t source, uint32_t& i, Index& index, SCC& scc, uint32_t*);
void tarjan_rec(uint32_t source, Index& index, SCC& scc, uint32_t* , Stack*, uint32_t*);

#endif // SCC_H
