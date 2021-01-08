#include "graph_header.h"
#include "scc_header.h"
#include "cc_header.h"

void tarjan(uint32_t source, uint32_t& i, Index& index, SCC& scc, uint32_t* explored)
{
    //cout << "tarjan run" << endl;
    uint32_t* lowlink = new uint32_t[index.getSOI()];
    uint32_t* caller = new uint32_t[index.getSOI()];
    uint32_t* S = new uint32_t[index.getSOI()];
    for(int j = 0; j < index.getSOI(); j++) {
        caller[j] = UNUSED;
        lowlink[j] = UNUSED;
        S[j] = UNUSED;
    }
    Stack stack(index.getSOI());
    stack.push(source);
    uint32_t last = source;
    explored[source] = 0;
    lowlink[source] = i;
    i++;
    uint32_t backtrack = source;
    uint32_t child = 0; ///////////////////////////////////
    while(true) {
        //cout << "1 last: " << last << " low last: " << lowlink[last] << " child: " << child << " low child: " << lowlink[child] << " i: " << i << endl;
        uint32_t* pinakas = NULL;
        if(S[last] == UNUSED) {
            uint32_t off = index.getListhead(last);
            if(off != UNUSED) {
                uint32_t si;
                pinakas = index.getNeighbors(si, off);
                S[last] = si;
            } else {
                S[last] = 0;
            }
        }
        if(explored[last] < S[last]) {
            uint32_t off = index.getListhead(last);
            if(off != UNUSED) {
                uint32_t si;
                pinakas = index.getNeighbors(si, off);
                S[last] = si;
            } else {
                S[last] = 0;
            }
            if(S[last] > 1) {
                backtrack = last;
            }
            child = pinakas[explored[last]];
            //cout << "ch: " << child << endl;
            explored[last]++;
            delete[] pinakas;
            pinakas = NULL;
            if(explored[child] == UNUSED) {
                caller[child] = last;
                explored[child] = 0;
                lowlink[child] = i;
                //cout << "start: " << child << " " << lowlink[child] << endl;
                i++;
                stack.push(child);
                last = child;
            } else if(stack.isonstack(child)) {
               //cout << last << " " << lowlink[last] << '\t' << child << " " << lowlink[child] << endl;
              // cout << "elif min" << endl;
                lowlink[last] = min(lowlink[last], lowlink[child]);
            }
        } else {
            if(lowlink[last] == i || S[last] == 0) { // i - 1
                //cout << "stack oper" << endl;
                uint32_t top = stack.top();
                stack.pop();
                while(top != backtrack) {
                    last = top;
                    /*if(caller[top] != UNUSED) {
                      lowlink[top] = lowlink[caller[top]];
                      }*/
                    top = stack.top();
                    /*if(top == UNUSED) {
                        break;
                    }*/
                    stack.pop();
                    //cout << "WHILEEEEEEEEEEEEEEEEEEE top:" << top << " backtrack: " << backtrack << endl;
                }
                stack.push(top);
            }
            uint32_t newlast = caller[last];
            delete[] pinakas;
            pinakas = NULL;
            if(newlast != UNUSED) {
                //cout << "nl " << newlast << " " << lowlink[newlast] << '\t' << " l " << last << " " << lowlink[last] << endl;
                lowlink[newlast] = min(lowlink[newlast], lowlink[last]);
                /*if(lowlink[last] < i) { // i - 1
                    //i--;
                    i = lowlink[newlast]; // CHANGED
                    cout << " i becomes " << i << endl;
                }*/
                //cout << "newlast if" << endl;
                last = newlast;
            } else {
                delete[] caller;
                stack.empty();
                /*for(int j = 0; j < index.getSOI(); j++) {
                    if(lowlink[j] != UNUSED) {
                        uint32_t pos = scc.findSCCpos(lowlink[j]);
                        if(pos == UNUSED) {
                            Component* comp = new Component(lowlink[j], index.getSOI());
                            pos = scc.add(comp);
                            scc.set(lowlink[j], pos, j);
                            //cout << "Node \t" << j << " \tlowlink \t"<< lowlink[j] <<endl;
                        } else {
                            scc.set(lowlink[j], pos, j);
                            //cout << "Node \t" << j << " \tlowlink \t"<< lowlink[j] <<endl;
                        }
                    }
                }*/
                uint32_t max = 0;
                uint32_t min = index.getSOI();
                for(int j = 0; j < index.getSOI(); j++) { // finds min/max
                    if(lowlink[j] != UNUSED) {
                        if(max < lowlink[j]) {
                            max = lowlink[j];
                        }
                        if(min > lowlink[j]) {
                            min = lowlink[j];
                        }
                    }
                }
                uint32_t* lcount = new uint32_t[max - min + 1](); // counts how many nodes gonna be in the component in order to save memory
                for(int j = 0; j < index.getSOI(); j++) {
                    if(lowlink[j] != UNUSED) {
                        lcount[lowlink[j] - min]++;
                    }
                }
                for(int j = 0; j < index.getSOI(); j++) {
                    if(lowlink[j] != UNUSED) {
                        if(lcount[lowlink[j] - min] > 1) {
                            scc.set(lowlink[j], j, lcount[lowlink[j] - min]);
                        } else {
                            scc.id_belongs_to_comps[j] = UNUSED - 1;
                        }
                    }
                }
                delete[] lowlink;
                delete[] lcount;
                delete[] S;
                break;
            }
        }
        //cout << "2 last: " << last << " low last: " << lowlink[last] << " child: " << child << " low child: " << lowlink[child] << " i: " << i << endl;
    }
}

/*uint32_t SCC::add(Component* co)
{
    for(uint32_t i = 0; i < max; i++) {
        if(comps[i] == NULL) { // finds first empty position in comps and puts co there
            comps[i] = co;
            comp_count++;
            return i;
        }
    }
    cerr << "component array full" << endl; // never gonna happen
    return UNUSED;
}*/

void SCC::set(uint32_t comp_id_low, uint32_t node, uint32_t sz)
{
    //cout << "node " << node << " lowlink " << comp_id_low << endl;
    uint32_t pos = findSCCpos(comp_id_low);
    if(pos == UNUSED) {
        for(uint32_t i = 0; i < max; i++) {
            if(comps[i] == NULL) { // finds first empty position in comps and puts co there
                comps[i] = new Component(comp_id_low, sz, comp_count);
                comp_count++;
                pos = i;
                break;
            }
        }
    }
    comps[pos]->add(node);
    id_belongs_to_comps[node] = comp_id_low;
}

/*void SCC::clean(void)
{
    for(uint32_t i = 0; i < max; i++) {
        if(comps[i] != NULL) {
            if(comps[i]->nodes_count <= 1) {
                delete comps[i];
                comp_count--;
                comps[i] = NULL;
            }
        }
    }
}*/

SCC::SCC(uint32_t size)
{
    comps = new Component*[size];
    for(uint32_t i = 0; i < size; i++) {
        comps[i] = NULL;
    }
    id_belongs_to_comps = new uint32_t[size];
    for(uint32_t j = 0; j < size; j++) {
        id_belongs_to_comps[j] = UNUSED;
    }
    exp_di=NULL;
    comp_count = 0;
    foo_counter=0;
    max = size;
}

SCC::~SCC()
{
    for(uint32_t i = 0; i < max; i++) {
        delete comps[i];
    }
    delete[] comps;
    delete[] id_belongs_to_comps;
    delete[] exp_di;
}

uint32_t SCC::findSCCpos(uint32_t id) {
    for(uint32_t i = 0; i < max; i++) {
        if(comps[i] != NULL) {
            if(comps[i]->id == id) {
                return i;
            }
        }
    }
    return UNUSED;
}

uint32_t SCC::findNodeSCCid(uint32_t node_id)
{
    return id_belongs_to_comps[node_id];
}

Component::Component(uint32_t i, uint32_t size, uint32_t count)
{
    id = i;
    di = count;
    nodes_count = 0;
    nodes_ids = new uint32_t[size];
}

Component::~Component()
{
    delete[] nodes_ids;
}

void Component::add(uint32_t node)
{
    nodes_ids[nodes_count] = node;
    nodes_count++;
}

void tarjan_rec(uint32_t source, Index& index, SCC& scc, uint32_t* lowlink, Stack* stack, uint32_t* explored)
{
    static uint32_t i = 0;
    stack->push(source);
    lowlink[source] = explored[source] = i++;
    uint32_t child, S;
    uint32_t last = source;
    uint32_t* pinakas = NULL;
    uint32_t off = index.getListhead(last);
    if(off != UNUSED) {
        pinakas = index.getNeighbors(S, off);
    } else {
        S = 0;
    }
    for(uint32_t j = 0; j < S; j++) {
        child = pinakas[j];
        if(explored[child] == UNUSED) {
            tarjan_rec(child, index, scc, lowlink, stack, explored);
            lowlink[last] = min(lowlink[last], lowlink[child]);
        } else if(stack->isonstack(child)) {
            lowlink[last] = min(lowlink[last], explored[child]);
        }
    }
    if(lowlink[last] == explored[last]) {
        uint32_t* temp = new uint32_t[index.getSOI() >> 3];
        uint32_t count = 0;
        while(stack->top() != last) {
            temp[count] = stack->top();
            stack->pop();
            count++;
        }
        temp[count] = stack->top();
        count++;
        stack->pop();
        for(uint32_t j = 0; j < count; j++) {
            if(count > 1) {
                scc.set(lowlink[last], temp[j], count);
            } else {
                scc.id_belongs_to_comps[temp[count - 1]] = UNUSED-2;
            }
        }
        delete[] temp;
    }
    delete[] pinakas;
}
