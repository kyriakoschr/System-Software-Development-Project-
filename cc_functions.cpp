#include "cc_header.h"
#include "misc_functions.h"

using namespace std;

void estimateCC(Index& indexIN,Index& indexOUT, CC* ConComp){
    uint32_t maxind;
    if(indexIN.getSOI()>indexOUT.getSOI()){
        maxind=indexIN.getSOI();
    } else {
        maxind=indexOUT.getSOI();
    }
    int* explored = new int[maxind](); // int arrays initialized with 0, becomes 1 if and when it is explored by bbfs
    uint32_t counter=0;
    for(uint32_t i=0;i<maxind;i++){
        if(explored[i]==1)
            continue;
        if((indexIN.getListhead(i) == UNUSED) && (indexOUT.getListhead(i) == UNUSED)) {
            continue;
        }
        CComponent* newcc=new CComponent(10, counter);
        ConComp->add(newcc);
        uint32_t* pinakas; // contains the returned neighbors of the node
        uint32_t s = 0; //gets the number of neighbors
        Queue qtemp(indexOUT.getSOI());
        qtemp.push(i);
        explored[i] = 1;
        newcc->add(i);
        ConComp->set(i,counter);
        uint32_t node;
        while(qtemp.is_empty()!=1){
            node=qtemp.pop();
            if(node<=indexIN.getSOI()){
                uint32_t off = indexIN.getListhead(node);
                if(off != UNUSED) {
                    pinakas = indexIN.getNeighbors(s,off);
                    for(uint32_t j = 0; j < s; j++) {  // for each neighbor
                        if(explored[pinakas[j]] == 0) {  //if it hasn't been checked for the current flow
                            explored[pinakas[j]] = 1; //check it
                            qtemp.push(pinakas[j]);
                            newcc->add(pinakas[j]);
                            ConComp->set(pinakas[j],counter);
                        }
                    }
                    delete[] pinakas;
                }
            }
            if(node<=indexOUT.getSOI()){
                uint32_t off = indexOUT.getListhead(node);
                if(off != UNUSED) {
                    pinakas = indexOUT.getNeighbors(s,off);
                    for(uint32_t j = 0; j < s; j++) {  // for each neighbor
                        if(explored[pinakas[j]] == 0) {  //if it hasn't been checked for the current flow
                            explored[pinakas[j]] = 1; //check it
                            qtemp.push(pinakas[j]);
                            newcc->add(pinakas[j]);
                            ConComp->set(pinakas[j],counter);
                        }
                    }
                    delete[] pinakas;
                }
            }
        }
        counter++;
    }
    /*for(uint32_t i = 0; i < ConComp->upcount; i++) {
      ConComp->updateIndex[i][0] = UNUSED;
      ConComp->updateIndex[i][1] = UNUSED;
      }
      ConComp->upcount = 0;*/
    delete[] explored;
}

void CC::set(uint32_t ID,uint32_t value){
    id_belongs_to_comps[ID]=value;
}

void CC::add(CComponent* newobj){
    if(comp_count==sz){
        CComponent** temp=new CComponent*[sz*2];
        memcpy(temp,comps,sizeof(CComponent*)*sz);
        delete[] comps;
        comps=temp;
        sz*=2;
    }
    comps[comp_count]=newobj;
    comp_count++;
    //cout<<comp_count<<" is comps co"<<endl;
}

void CComponent::add(uint32_t newobj){
    if(nodes_count == sz) {
        uint32_t* temp = new uint32_t[sz * 2];
        memcpy(temp, nodes_ids, sizeof(uint32_t) * sz);
        delete[] nodes_ids;
        nodes_ids = temp;
        sz *=2;
    }
    nodes_ids[nodes_count]=newobj;
    nodes_count++;
}

CC::CC(uint32_t size){
    comps=new CComponent*[size];
    hasht=NULL;
    hashsz=0;
    uicntr=0;
    uisz=size;
    comp_count=0;
    id_belongs_to_comps=new uint32_t[size];
    sz = size;
    updateIndex = new uint32_t*[size];
    for(uint32_t i = 0; i < size; i++) {
        comps[i]=NULL;
        updateIndex[i] = new uint32_t[2];
        updateIndex[i][0]=UNUSED;
        updateIndex[i][1]=UNUSED;
        id_belongs_to_comps[i] = UNUSED;
    }
    upcount = 0;
}

CC::~CC(){
    for(uint32_t i = 0; i < sz; i++) {
        delete comps[i];
    }
    delete[] comps;
    for(uint32_t i = 0; i < uisz; i++) {
        delete[] updateIndex[i];
    }
    delete[] updateIndex;
    delete[] id_belongs_to_comps;
    delete[] hasht;
        //delete hasht;
    /*for(uint32_t i = 0; i < hashsz; i++) {
        delete[] hasht[i];
    }*/
}

void CC::update(uint32_t id1, uint32_t id2) {
    if(hashsz==0){
        hashsz=comp_count;
        hasht=new LinkedList[comp_count];
    }
    while(id1>=sz||id2>=sz){ //in case id1 doesnt exist AND is bigger than the current index duplicate enough times
        uint32_t* temparr=new uint32_t[sz*2];
        memcpy(temparr,id_belongs_to_comps,sizeof(uint32_t)*sz);
        for(uint32_t i=sz;i<sz*2;i++){
            temparr[i]=UNUSED;
        }
        delete[] id_belongs_to_comps;
        id_belongs_to_comps=temparr;
        sz*=2;
    }

    if((id_belongs_to_comps[id1] == id_belongs_to_comps[id2]) && (id_belongs_to_comps[id1] == UNUSED)) { //in case both are not in a ConComp create one
        CComponent* newcc=new CComponent(100,comp_count);
        add(newcc);
        while(comp_count-1>=hashsz){
            LinkedList* newht=new LinkedList[hashsz*2];
            //memcpy(newht,hasht,sizeof(LinkedList)*hashsz);
            for(uint32_t i=0;i<hashsz;i++){
                newht[i].head=hasht[i].head;
                newht[i].length=hashsz;
                hasht[i].head=NULL;
            }
            delete[] hasht;
            hasht=newht;
            hashsz *= 2;
        }
        newcc->add(id1);
        id_belongs_to_comps[id1] = newcc->id;
        newcc->add(id2);
        id_belongs_to_comps[id2] = newcc->id;
    } else if((id_belongs_to_comps[id1] == UNUSED) && (id_belongs_to_comps[id2] != UNUSED)) {//if only one is in a CC add the other one in the same CC
        id_belongs_to_comps[id1]=id_belongs_to_comps[id2];
        comps[id_belongs_to_comps[id1]]->add(id1);
    } else if((id_belongs_to_comps[id1] != UNUSED) && (id_belongs_to_comps[id2] == UNUSED)) {
        id_belongs_to_comps[id2]=id_belongs_to_comps[id1];
        comps[id_belongs_to_comps[id1]]->add(id2);
    } else if(id_belongs_to_comps[id1] != id_belongs_to_comps[id2]) { //in case they are in different CCs
        Item* temp=hasht[id_belongs_to_comps[id1]].getHead();
        /*cout << temp->key << endl;
        return;*/
        //cout<<hasht[id_belongs_to_comps[id1]].getLength()<<endl;
        int flag=0;
        while(temp!=NULL){
            //cout <<"if1 " << updateIndex[temp->key][0]<< " " <<id_belongs_to_comps[id2]<< endl;
            //cout << "if2 " << updateIndex[temp->key][1] << " " << id_belongs_to_comps[id2] << endl;
            if((updateIndex[temp->key][0]==id_belongs_to_comps[id2])||(updateIndex[temp->key][1]==id_belongs_to_comps[id2])){
                flag=1;
                break;
            }
            else{
                temp=temp->next;
            }
        }
        if(flag==0){
            if(uisz==uicntr){
                uint32_t** tempupdate=new uint32_t*[uisz*2];
                memcpy(tempupdate,updateIndex,sizeof(uint32_t*)*uisz);
                for(uint32_t i=uisz;i<uisz*2;i++){
                    updateIndex[i] = new uint32_t[2];
                    updateIndex[i][0]=UNUSED;
                    updateIndex[i][1]=UNUSED;
                }
                delete[] updateIndex;
                updateIndex=tempupdate;
                uisz*=2;
            }
            updateIndex[uicntr][0] = id_belongs_to_comps[id1];
            updateIndex[uicntr][1] = id_belongs_to_comps[id2];
            Item* temp1 = new Item;
            Item* temp2 = new Item;
            temp1->key = uicntr;
            temp2->key = uicntr;
            temp1->next = NULL;
            temp2->next = NULL;
            uicntr++;
            hasht[id_belongs_to_comps[id1]].insertItem(temp1);
            hasht[id_belongs_to_comps[id2]].insertItem(temp2);
            //cout << updateIndex[upcount][0] << " " << updateIndex[upcount][0] << endl;
        }
    } else if(id_belongs_to_comps[id1] == id_belongs_to_comps[id2]) {
        return;
    }
}

CComponent::CComponent(uint32_t size,uint32_t ID)
{
    sz = size;
    id=ID;
    nodes_count=0;
    nodes_ids=new uint32_t[sz];
    for(uint32_t i=0;i<sz;i++)
        nodes_ids[i]=UNUSED;
}

CComponent::~CComponent()
{
    delete[] nodes_ids;
}

void CC::rebuild(void)
{
    for(uint32_t i = 0; i < uisz; i++) {
        if(updateIndex[i][0] != UNUSED) {
            uint32_t uii1=updateIndex[i][1];
            uint32_t uii0=updateIndex[i][0];
            //cout<<i<<" "<<comp_count<<endl;
            //cout << "upd " << updateIndex[i][0] << endl;
            //cout << "upd " << updateIndex[i][1] << endl;
            //thelw ta updateindex lines pou exoyn mesa to updateIndex[i][1] na ginoyn updateIndex[i][0]

            Item* temp=hasht[uii1].getHead();
            while(temp!=NULL){
                if(temp->key == i) {
                    temp=temp->next;
                    continue;
                }
                if(updateIndex[temp->key][0]==uii1) {
                    updateIndex[temp->key][0]=uii0;
                    //cout << "sadf " << updateIndex[temp->key][0] << endl;
                }
                if(updateIndex[temp->key][1]==uii1) {
                    updateIndex[temp->key][1]=uii0;
                    //cout << "sadf1 " << updateIndex[temp->key][1] << endl;
                }
                temp=temp->next;
            }
            //cout << "joining " << updateIndex[i][0] << " " << updateIndex[i][1] << endl;
            join(updateIndex[i][0], updateIndex[i][1]);
            updateIndex[i][0] = UNUSED; // no more use for it so setting it to UNUSED
            updateIndex[i][1] = UNUSED;
        } // else break?
    }
    delete[] hasht; // deleting and re-initializing hash
    hasht = new LinkedList[hashsz];
}

void CC::join(uint32_t id1, uint32_t id2)
{
    if(comps[id1] == comps[id2]) {
        return;
    }
    /*if((comps[id1] == NULL) || comps[id2] == NULL) {
        cout << "FUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUCK by " << id1 << " " << id2 << endl;
        exit(1);
        return;
    }*/
    //copying nodes from id1 to id2
    //cout<<comps[id1]->id<< endl;
    //cout <<comps[id2]->id<<endl;
    //cout<<"mpike"<<endl;

    /*uint32_t* temp = new uint32_t[comps[id1]->nodes_count + comps[id2]->nodes_count];
    memcpy(temp, comps[id1]->nodes_ids, comps[id1]->nodes_count * sizeof(uint32_t));
    for(uint32_t i = 0; i < comps[id2]->nodes_count; i++) {
        temp[comps[id1]->nodes_count + i] = comps[id2]->nodes_ids[i];
    }*/

    //cout << comps[id1]->nodes_count << endl;
    //cout << comps[id2]->nodes_count << endl;
    //memcpy(temp+(comps[id1]->nodes_count*sizeof(uint32_t)), comps[id2]->nodes_ids, comps[id2]->nodes_count * sizeof(uint32_t));

    /*delete[] comps[id1]->nodes_ids;
    comps[id1]->nodes_ids = temp;
    comps[id1]->nodes_count += comps[id2]->nodes_count;*/

    /*for(uint32_t j = 0; j < comps[id2]->nodes_count; j++) { // updating id_belongs to match changes
        id_belongs_to_comps[comps[id2]->nodes_ids[j]] = id1;
        comps[id1]->add(comps[id2]->nodes_ids[j]);
    }*/
    for(uint32_t i = 0; i < sz; i++) {
        if(id_belongs_to_comps[i] == id2) {
            id_belongs_to_comps[i] = id1;
        }
    }
    //delete comps[id2];
    //comps[id2] = NULL;
}

uint32_t CC::qcheck(uint32_t id1, uint32_t id2,Index* indin,Index* indout,uint32_t version)
{
    if(id_belongs_to_comps[id1] == id_belongs_to_comps[id2]) {
        return bbfs(indin,indout,id1,id2,version); // if the CC are the same OR the CC are connected in the update index hash
    } else if (hasht[id_belongs_to_comps[id1]].getItem(id_belongs_to_comps[id2]) != NULL) {
        return bbfs(indin,indout,id1,id2,version);
    }
    else {
        return -1;
    }
}
