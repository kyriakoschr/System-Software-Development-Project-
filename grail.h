#ifndef GRAIL_H
#define GRAIL_H

#include "graph_header.h"
#include "misc_functions.h"

#define NO -1
#define MAYBE 1
#define YES 2

typedef struct ranks{ //array of min_rank, rank
    public:
        uint32_t* min_rank;
        uint32_t* rank;
}ranks;

void grail_rec(uint32_t source, uint32_t &rcounter, Index* grailIndex, uint32_t* explored, ranks* rank);

ranks* buildGrailIndex(Index* grailIndex, SCC& components, Index* indin, Index* indout);

int isReachableGrailIndex(ranks* granks, SCC* scc, uint32_t source_node, uint32_t target_node);

int destroyGrailIndex(Index* grailIndex, ranks* granks);

#endif
