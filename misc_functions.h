#ifndef MF_HEAD
#define MF_HEAD

#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <inttypes.h>
#include <string.h>
#include "graph_header.h"
#include "jobsched.h"
#include <algorithm>
#include "queue.h"
#include "scc_header.h"
#include "cc_header.h"
#include "grail.h"

using namespace std;

uint32_t bbfs(Index* indin,Index* indout,uint32_t source,uint32_t target,uint32_t version);

int bfs(uint32_t&,int& ,uint32_t& ,Queue& ,int* ,int* ,Index& ,uint32_t version );

Index* reallocIndex(Index* index);

ptr* parser(const char* eFile, const char* fFile, Index* index_incoming, Index* index_outgoing);

uint32_t st_bbfs(ranks* rankIndex,Index *indin,Index *indout,SCC* sccom,uint32_t source,uint32_t target, uint32_t version);

void show_usage(string name);

#endif //MF_HEAD
