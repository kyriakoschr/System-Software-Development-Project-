#include "grail.h"

void grail_rec(uint32_t source, uint32_t &rcounter, Index* grailIndex, uint32_t* explored, ranks* grank) {

	uint32_t S=0;
	uint32_t child, j;
	uint32_t last=source;
	uint32_t* pinakas=NULL;
	uint32_t off=grailIndex->getListhead(last);
	if (off!= UNUSED) {
		pinakas=grailIndex->getNeighbors(S, off);
		random_shuffle(&pinakas[0], &pinakas[S]);
	} else {
		explored[last]=1;
		rcounter++;
		grank->rank[last]=rcounter;
		grank->min_rank[last]=rcounter;
	}

	for (j=0;j<S; j++) {
		child=pinakas[j];
		if(explored[child]==UNUSED) {
			grail_rec(child, rcounter, grailIndex, explored, grank);
		}
	}

	if ((j==S)&&(S!=0)) {
		explored[last]=1;
		rcounter++;
		grank->rank[last]=rcounter;
		if (S==1)
			grank->min_rank[last]=grank->min_rank[child];
		else {
			for (uint32_t i=0; i<S; i++){
				grank->min_rank[last]=min(grank->rank[pinakas[i]], grank->rank[last]);
			}
		}
	}
	delete[] pinakas;
}

ranks* buildGrailIndex(Index* grailIndex, SCC& components, Index* indin, Index* indout) {
	uint32_t ucounter=0;
	uint32_t max = 0;
    for(uint32_t j = 0; j < indout->getSOI(); j++) { // finds max
        if(components.id_belongs_to_comps[j] != UNUSED) {
			if(components.id_belongs_to_comps[j] == UNUSED-2) {
				ucounter++;
			} else {
				if(max < components.id_belongs_to_comps[j]) {
					max = components.id_belongs_to_comps[j];
				}
            }
        }
    }
    uint32_t temp_count=max+1;
    for(uint32_t j = 0; j <indout->getSOI(); j++) {
        if (components.id_belongs_to_comps[j]==(UNUSED-2)){
            components.id_belongs_to_comps[j]=temp_count;
            temp_count++;
        }
	}
    components.foo_counter=(temp_count+components.comp_count)-1;

    components.exp_di=new pin_di [components.foo_counter+1];
    for (uint32_t j=0; j<=components.foo_counter; j++) {
        components.exp_di[j].di=UNUSED;
        components.exp_di[j].node=UNUSED;
    }


    uint32_t di_counter=0; //counter for exp_id.di

    for (uint32_t i=0;i<indout->getSOI(); i++) {
        if (components.id_belongs_to_comps[i]!=UNUSED) {
            if(components.exp_di[components.id_belongs_to_comps[i]].di==UNUSED) {
                components.exp_di[components.id_belongs_to_comps[i]].di=di_counter;
                components.exp_di[components.id_belongs_to_comps[i]].node=i;
                di_counter++;
            }
        }
	}

    int in_flag=1;
	uint32_t off;
	uint32_t apo, pros, S;

	for (uint32_t i=0; i<indout->getSOI();i++) {
		uint32_t* pin_scc_i= new uint32_t [components.foo_counter]();
		uint32_t* pinakas=NULL;
		if(components.id_belongs_to_comps[i]==UNUSED){
			delete[] pin_scc_i;
			continue;
		}
		apo=components.exp_di[components.id_belongs_to_comps[i]].di;
		off=indout->getListhead(i);
		if (off!= UNUSED) {
			pinakas=indout->getNeighbors(S, off);
		} else {
			delete[] pin_scc_i;
			continue;
		}
		for (uint32_t j=0; j<S; j++) {
			pros=components.exp_di[components.id_belongs_to_comps[pinakas[j]]].di;
			if ((pin_scc_i[pros]==0) && (apo!=pros)){
				in_flag=grailIndex->insert_node(apo, pros);
				while(in_flag!=OK_SUCCESS) {
					if (in_flag==-1) {
						Index* temp;
						temp=reallocIndex(grailIndex);
						grailIndex=temp;
					}
					else if((in_flag==-2)||(in_flag==-4)) {
						grailIndex->realloc_buffer();
					}
					else if(in_flag==-3){
						cout<<"Shouldn't get in here"<<endl;
					}
					in_flag=grailIndex->insert_node(apo,pros);
				}
				pin_scc_i[j]=1;
			} else {
				continue;
			}
		}
		delete[] pinakas;
		delete[] pin_scc_i;
	}

	ranks* grank=new ranks;
	grank->min_rank=new uint32_t[di_counter];
	grank->rank=new uint32_t[di_counter];
	uint32_t* explored = new uint32_t[di_counter];

    for(uint32_t i = 0; i < di_counter; i++) {
        explored[i] = UNUSED;
		grank->min_rank[i]=UNUSED;
		grank->rank[i]=UNUSED;
	}
	uint32_t* shuffle_pin=new uint32_t [di_counter];
	uint32_t shuffle_counter=0;
    for (uint32_t i=0;i<components.foo_counter+1;i++){
        if(components.exp_di[i].di!=UNUSED){
            shuffle_pin[shuffle_counter]=components.exp_di[i].di;
			shuffle_counter++;
		}
    }

    random_shuffle(&shuffle_pin[0], &shuffle_pin[di_counter]);
    uint32_t gcounter=0;
	for (uint32_t i=0; i<di_counter; i++){
		if (shuffle_pin[i]!=UNUSED) {
			if (explored[shuffle_pin[i]]==UNUSED){
				grail_rec(shuffle_pin[i], gcounter, grailIndex, explored, grank);
			}
		}
	}

	delete[] shuffle_pin;
	delete[] explored;
	return grank;
}


int isReachableGrailIndex(ranks* granks, SCC* scc, uint32_t source_node, uint32_t target_node) {
	if ((scc->id_belongs_to_comps[source_node]!=UNUSED)&&(scc->id_belongs_to_comps[target_node]!=UNUSED)) {
		uint32_t max_val=granks->rank[scc->exp_di[scc->id_belongs_to_comps[source_node]].di];
		uint32_t min_val=granks->min_rank[scc->exp_di[scc->id_belongs_to_comps[source_node]].di];
		uint32_t rtarget_val=granks->rank[scc->exp_di[scc->id_belongs_to_comps[target_node]].di];
		uint32_t mtarget_val=granks->min_rank[scc->exp_di[scc->id_belongs_to_comps[target_node]].di];
		if ((min_val<=mtarget_val)&&(max_val>=mtarget_val)) {
			if ((min_val<=rtarget_val)&&(max_val>=rtarget_val)) {
				return MAYBE;
			} else {
				return NO;
			}
		} else {
			return NO;
		}
	} else {
		return NO;
	}
}

int destroyGrailIndex(Index* grailIndex, ranks* granks){
	delete grailIndex;
	delete[] granks->min_rank;
	delete[] granks->rank;
	delete granks;
	return OK_SUCCESS;
}

