#ifndef CC_H
#define CC_H

#include <inttypes.h>
#include "graph_header.h"
#include "misc_functions.h"
#include "LinkedList.h"

class CComponent {
    public:
        uint32_t id;
        uint32_t nodes_count;
        uint32_t* nodes_ids;
        uint32_t sz;
        void add(uint32_t);
        CComponent(uint32_t,uint32_t);
        ~CComponent();
};

class CC {
    public:
        uint32_t sz;
        CComponent** comps;
        LinkedList* hasht;
        uint32_t hashsz;
        uint32_t comp_count;
        uint32_t uisz;
        uint32_t uicntr;
        uint32_t* id_belongs_to_comps;
        uint32_t** updateIndex;
        uint32_t upcount;
        void rebuild();
        void join(uint32_t, uint32_t);
        uint32_t qcheck(uint32_t, uint32_t,Index*,Index*,uint32_t version);
        void update(uint32_t, uint32_t);
        void add(CComponent*);
        void set(uint32_t,uint32_t);
        int findNodeCCid(uint32_t node_id); // returns ID of the CC that belongs to
        CC(uint32_t);
        ~CC();
};

void estimateCC(Index&,Index&, CC*);

#endif // CC_H
