#include "misc_functions.h"
#define WTHR 4

using namespace std;

int bfs(uint32_t& cntr, int& flag,uint32_t& level1,Queue& qin,int* exploredIN,int* exploredOUT,Index& index,uint32_t version){
    //cout<<"vers in bbfs "<<version <<endl;
    uint32_t node;
    uint32_t level2=0;
    uint32_t* pinakas; // contains the returned neighbors of the node
    uint32_t* versions; // contains the returned versions of the node
    uint32_t s = 0; //gets the number of neighbors
    uint32_t v = 0; //gets the number of neighbors
    flag = (flag+1)%2;
    for(uint32_t j = 0; j < level1; j++) {
        if(qin.is_empty() == 1) {
            delete[] exploredIN;
            delete[] exploredOUT;
            return -1;
        }
        node = qin.pop();
        uint32_t off = index.getListhead(node);
        if(off == UNUSED) {
            continue;
        }
        pinakas = index.getNeighbors(s,off);
        versions= index.getEdgePr(v,off);
        //cout<<s<<" s v "<<v<<endl;
        for(uint32_t i = 0; i < s; i++) {  // for each neighbor
            //cout<<versions[i]<<" "<<version<<endl;
            if(versions[i]<=version){
                if(exploredIN[pinakas[i]] == 0) {  //if it hasn't been checked for the current flow
                    if(exploredOUT[pinakas[i]] == 1){//if it has been checked from the other flow too it means the result has been found
                        flag = 3;
                        break;
                    }
                    exploredIN[pinakas[i]] = 1; //check it
                    qin.push(pinakas[i]);
                    level2++;
                }
            }
            else continue;
        }
        delete[] pinakas;
        delete[] versions;
        if(flag == 3) {
            break;
        }
    }
    cntr++;
    level1 = level2;
    if((level1==0)&&(flag!=3)){
        delete[] exploredIN;
        delete[] exploredOUT;
        return -1;
    }
    return 0;
}

uint32_t bbfs(Index *indin,Index *indout,uint32_t source,uint32_t target,uint32_t version){ // stands for bisexual breast first search
    Queue qin(indin->getSOI() >> 3);
    Queue qout(indout->getSOI() >> 3);
    qin.push(target);
    qout.push(source);
    uint32_t nextlastout;
    uint32_t nextlastin;
    int* exploredIN = new int[indin->getSOI()](); // int arrays initialized with 0, becomes 1 if and when it is explored by bbfs
    int* exploredOUT = new int[indout->getSOI()]();
    uint32_t incntr=0;
    uint32_t outcntr=0;
    uint32_t node;
    uint32_t lastIn=target; // initilization of source and target
    uint32_t lastOut=source;
    uint32_t in_level1 = 1;
    uint32_t in_level2 = 0;
    uint32_t out_level1 = 1;
    uint32_t out_level2 = 0;
    exploredIN[target] = 1;
    exploredOUT[source] = 1;
    if((indin->getListhead(target) == UNUSED) || (indout->getListhead(source) == UNUSED)) {
        qin.emptyQ();
        qout.emptyQ();
        delete[] exploredIN;
        delete[] exploredOUT;
        return -1;
    }
    int flag = 0; // if flag = 1 control flow -> incoming, if flag = 0 -> outgoing. if flag = 3 result is found!
    while (flag!=3) {
        if(flag == 1) { // incoming
            if(bfs(incntr, flag, in_level1, qin, exploredIN, exploredOUT, *indin,version) == -1) {
                qin.emptyQ();
                qout.emptyQ();
                return -1;
            }
        } else { // outgoing
            if(bfs(outcntr, flag, out_level1, qout,exploredOUT , exploredIN, *indout,version) == -1) {
                qin.emptyQ();
                qout.emptyQ();
                return -1;
            }
        }
    }
    qin.emptyQ();
    qout.emptyQ();
    delete[] exploredIN;
    delete[] exploredOUT;
    return incntr + outcntr;
}

Index* reallocIndex(Index* index)
{
    Index* newind=new Index(*index);
    delete index;
    return newind;
}

ptr* parser(const char* eFile, const char* fFile, Index* index_incoming, Index* index_outgoing)
{
    string str;
    ifstream infile;
    int flag_in = 1;
    int flag_out = 1;
    uint32_t id1;
    uint32_t id2;
    Index* temp;
    char praksi;
    infile.open(eFile);
    while(!(infile.eof())) {
        getline(infile, str); // Saves the line from file in str
        if(str == "S") { // if <S> character is met, end file reading
            break;
        }
        sscanf(str.c_str(),"%"SCNd32" \t""%"SCNd32, &id1, &id2);
        flag_in = index_incoming->insert_node(id2, id1); // incoming
        // if insert returns -1 it meant that it needs more space and then call it again with the same arguments
        // using atoll() instead of atoi() because of the input size
        flag_out = index_outgoing->insert_node(id1, id2); // outgoing
        while(flag_in < 0) {
            if(flag_in==-1){
                temp=reallocIndex(index_incoming);
                index_incoming=temp;
            }
            else if((flag_in==-2)||(flag_in==-4)){
                index_incoming->realloc_buffer();
            }
            else if(flag_in==-3){
                //cout<<"Shouldn't get in here"<<endl;
            }
            flag_in = index_incoming->insert_node(id2, id1);
        }
        while(flag_out < 0) {
            if(flag_out==-1){
                temp=reallocIndex(index_outgoing);
                index_outgoing=temp;
            }
            else if((flag_out==-2)||(flag_out==-4)){
                index_outgoing->realloc_buffer();
            }
            else if(flag_out==-3){
                cout<<"Shouldn't get in here"<<endl;
            }
            flag_out= index_outgoing->insert_node(id1, id2);
        }
    }
    infile.close();
    uint32_t maxind;
    if(index_incoming->getSOI()>index_outgoing->getSOI()){
        maxind=index_incoming->getSOI();
    } else {
        maxind=index_outgoing->getSOI();
    }
    infile.open(fFile);
    int type;
    SCC* scc;
    Index* grailIndex=NULL;
    ranks* rankIndex=NULL;
    getline(infile, str); // Saves the line from file in str
    if (str == "STATIC") {
        type = 1;
        grailIndex=new Index();
        scc = new SCC(index_outgoing->getSOI());
        uint32_t k = 0;
        uint32_t* explored = new uint32_t[index_outgoing->getSOI()];
        for(uint32_t j = 0; j < index_outgoing->getSOI(); j++) {
            explored[j] = UNUSED;
        }
        uint32_t* lowlink = new uint32_t[index_outgoing->getSOI()];
        Stack* stack = new Stack(index_outgoing->getSOI());
        for(uint32_t i = 0; i < index_outgoing->getSOI(); i++) {
            if(index_outgoing->IndexCells[i] != UNUSED) {
                if(scc->id_belongs_to_comps[i] == UNUSED) {
                    //tarjan(i, k, *index_outgoing, *scc, explored); // iterative WARNING: SLOW
                    tarjan_rec(i, *index_outgoing, *scc, lowlink, stack, explored); // recursive WARNING: FAST
                }
            }
        }
        rankIndex=buildGrailIndex(grailIndex, *scc, index_incoming, index_outgoing);
        delete stack;
        delete[] lowlink;
        delete[] explored;
    } else if (str == "DYNAMIC") {
        type = 2;
    } else {
        type = 3;
    }
    CC* coco = NULL;
    uint32_t queries = 0;
    if(type == 2 || type == 3) {
        coco = new CC(maxind);
        estimateCC(*index_incoming, *index_outgoing, coco);
    }
    uint32_t version=0;
    bool prevjob;
    bool firstprevjob=true;
    JobScheduler myjs(WTHR,index_incoming,index_outgoing,type,scc,coco,rankIndex);
    while(!(infile.eof())) {
        getline(infile, str); // Saves the line from file in str
        if(str == "S") { // if <S> character is met, end file reading
            break;
        } else if (type == 1) { // STATIC
            sscanf(str.c_str(),"%c"" \t%"SCNd32" \t""%"SCNd32, &praksi, &id1, &id2);
            if (praksi == 'Q') {
                job* newjob=new job(QU,id1,id2,version);
                myjs.submit_job(newjob);
                /*uint32_t res = bbfs(index_incoming, index_outgoing, id1, id2);
                if(res == UNUSED + 2) {
                    cout << "-1" << endl;
                } else {
                    cout << res << endl;
                }*/
            } else if (praksi=='F'){
                myjs.execute_all_jobs();
                myjs.wait_all_tasks_finish();
                //continue;
            } else {
                //cerr << " WRONG INPUT AT PARSER FOR RESULT " << endl;
                continue;
            }
        } else if (type == 2 || type == 3) { // DYNAMIC or unspecified
        //cout<<str<<endl;
            sscanf(str.c_str(),"%c"" \t%"SCNd32" \t""%"SCNd32, &praksi, &id1, &id2);
            if (praksi == 'A'){
                //cout<<"ADDITION"<<endl;
                if(firstprevjob==true){
                    firstprevjob=false;
                    prevjob=AD;
                }
                else if(prevjob==QU){
                    version++;
                    //cout<<"v++ "<<version<<endl;
                }
                flag_in = index_incoming->insert_node(id2, id1, version);
                flag_out = index_outgoing->insert_node(id1, id2, version);
                while(flag_in < 0) { //doubling both because they are parallel
                    if(flag_in==-1){
                        temp=reallocIndex(index_incoming);
                        index_incoming=temp;
                    }
                    else if((flag_in==-2)||(flag_in==-4)){
                        index_incoming->realloc_buffer();
                    }
                    else if(flag_in==-3){
                        cout<<"Shouldn't get in here"<<endl;
                    }
                    flag_in = index_incoming->insert_node(id2, id1, version);
                }
                while(flag_out < 0) {
                    if(flag_out==-1){
                        temp=reallocIndex(index_outgoing);
                        index_outgoing=temp;
                    }
                    else if((flag_in==-2)||(flag_in==-4)){
                        index_outgoing->realloc_buffer();
                    }
                    else if(flag_in==-3){
                        cout<<"Shouldn't get in here"<<endl;
                    }
                    flag_out= index_outgoing->insert_node(id1, id2,version);
                }
                // before update check for same comps cause update deosnt
                if((coco->id_belongs_to_comps[id1] != coco->id_belongs_to_comps[id2]) && ( coco->id_belongs_to_comps[id1] != UNUSED)&& ( coco->id_belongs_to_comps[id2] != UNUSED)) {
                    coco->update(id1, id2);
                    coco->upcount++;
                }
                //cout << (double)coco->upcount / (double)queries << endl;
                if(((double)coco->upcount / (double)queries) > 0.02) {
                    //cout << "rebuilding.." << endl;
                    coco->rebuild();
                    coco->upcount = 0;
                    queries = 0;
                }
                prevjob=AD;
                //cout<<"additions"<<endl;
            } else if (praksi == 'Q'){
                //cout<<"v in q "<<version<<endl;
                job* newjob=new job(QU,id1,id2,version);
                myjs.submit_job(newjob);
                queries++;
                /*if(coco->qcheck(id1, id2) == true) {
                    uint32_t res = bbfs(index_incoming, index_outgoing, id1, id2);
                    if(res == UNUSED + 2) {
                        //cout << "-1" << endl;
                    } else {
                        //cout << res << endl;
                    }
                } else {
                    //cout << "-1" << endl;
                }*/
                prevjob=QU;
            } else if (praksi=='F'){
                //continue;
                myjs.execute_all_jobs();
                myjs.wait_all_tasks_finish();
                //cout<<"kokos"<<endl;
                firstprevjob=true;
            } else {
                cerr << " WRONG INPUT AT PARSER FOR RESULT " << endl;
            }
        }
    }
    if(type == 1) {
        delete scc;
        int res=destroyGrailIndex(grailIndex, rankIndex);
    } else if(type == 2 || type == 3) {
        delete coco;
    }
    /*uint32_t max = 0;
    for(uint32_t j = 0; j < index_outgoing->getSOI(); j++) { // finds max
        if(scc->id_belongs_to_comps[j] != UNUSED) {
            if(max < scc->id_belongs_to_comps[j]) {
                max = scc->id_belongs_to_comps[j];

            }
        }
    }

    uint32_t foo=max+1; //counter for id_belongs to comps
    for(uint32_t j = 0; j < index_outgoing->getSOI(); j++) {
        if (scc->id_belongs_to_comps[j]==UNUSED){
            scc->id_belongs_to_comps[j]=foo;
            foo++;
            //cout<<"ID_BELONGS["<<j<<"]="<<scc->id_belongs_to_comps[j]<<endl;
        }
    }

    scc->foo_counter=foo;
    scc->exp_di=new struct pin_di [foo];
    for (uint32_t j=0; j<foo; j++) {
        scc->exp_di[j].di=UNUSED;
        scc->exp_di[j].node=UNUSED;
    }

    uint32_t boo=0; //counter for exp_id.di
    for (uint32_t i=0;i<index_outgoing->getSOI(); i++) {
        scc->exp_di[scc->id_belongs_to_comps[i]].di=boo;
        scc->exp_di[scc->id_belongs_to_comps[i]].node=i;
        boo++;
    }
//---------------------
    Index* grailIndex=new Index();
    while(grailIndex->getSOI()<boo) {
        temp=reallocIndex(grailIndex);
        grailIndex=temp;
    }
	ranks *rankIndex;
    rankIndex=buildGrailIndex(*grailIndex, *scc, index_incoming, index_outgoing);
//grail ---------^^^^^^^*/
    ptr* pp = new ptr;
    pp->in = index_incoming;
    pp->out = index_outgoing;
    return pp;
}

uint32_t st_bbfs(ranks* rankIndex,Index *indin, Index *indout, SCC* scc, uint32_t source, uint32_t target,uint32_t version){ // stands for bisexual breast first search
		Queue qin(indin->getSOI() >> 3);
		Queue qout(indout->getSOI() >> 3);
		qin.push(target);
		qout.push(source);
		uint32_t nextlastout;
		uint32_t nextlastin;
		int* exploredIN = new int[indin->getSOI()](); // int arrays initialized with 0, becomes 1 if and when it is explored by bbfs
		int* exploredOUT = new int[indout->getSOI()]();
		uint32_t incntr=0;
		uint32_t outcntr=0;
		uint32_t node;
		uint32_t lastIn=target; // initilization of source and target
		uint32_t lastOut=source;
		uint32_t in_level1 = 1;
		uint32_t in_level2 = 0;
		uint32_t out_level1 = 1;
		uint32_t out_level2 = 0;
		exploredIN[target] = 1;
		exploredOUT[source] = 1;
		if((indin->getListhead(target) == UNUSED) || (indout->getListhead(source) == UNUSED)) {
			qin.emptyQ();
			qout.emptyQ();
			delete[] exploredIN;
			delete[] exploredOUT;
			return -1;
		}
		int flag = 0; // if flag = 1 control flow -> incoming, if flag = 0 -> outgoing. if flag = 3 result is found!
		while (flag!=3) {

			if(flag == 1) { // incoming
				if(bfs(incntr, flag, in_level1, qin, exploredIN, exploredOUT, *indin, version) == -1) {
					qin.emptyQ();
					qout.emptyQ();
					return -1;
				}
			} else { // outgoing
				if(bfs(outcntr, flag, out_level1, qout,exploredOUT , exploredIN, *indout,version) == -1) {
					qin.emptyQ();
					qout.emptyQ();
					return -1;
				}
			}
			isReachableGrailIndex(rankIndex, scc, qin.seehead(), qout.seehead());
		}
		qin.emptyQ();
		qout.emptyQ();
		delete[] exploredIN;
		delete[] exploredOUT;
		return incntr + outcntr;
}


void show_usage(string name)
{
    cerr << "Usage: " << name << " <option(s)> SOURCES"
        << "Options:\n"
        << "\t-h,--help\t\tShow this help message\n"
        << "\t-f,--filename FILENAME\tSpecify the filename path"
        << endl;
}


/*uint32_t bbfs(Index *indin,Index *indout,uint32_t source,uint32_t target){ // stands for bisexual breast first search
    Queue qin;
    Queue qout;
    qin.push(target);
    qout.push(source);
    uint32_t nextlastout;
    uint32_t nextlastin;
    int* exploredIN = new int[indin->getSOI()](); // int arrays initialized with 0, becomes 1 if and when it is explored by bbfs
    int* exploredOUT = new int[indout->getSOI()]();
    uint32_t incntr=0;
    uint32_t outcntr=0;
    uint32_t node;
    uint32_t lastIn=target; // initilization of source and target
    uint32_t lastOut=source;
    uint32_t in_level1 = 1;
    uint32_t in_level2 = 0;
    uint32_t out_level1 = 1;
    uint32_t out_level2 = 0;
    exploredIN[target] = 1;
    exploredOUT[source] = 1;
    if((indin->getListhead(target) == UNUSED) || (indout->getListhead(source) == UNUSED)) {
        qin.emptyQ();
        qout.emptyQ();
        delete[] exploredIN;
        delete[] exploredOUT;
        return -1;
    }
    int flag = 0; // if flag = 1 control flow -> incoming, if flag = 0 -> outgoing. if flag = 3 result is found!
    while (flag!=3) {
        if(flag == 1) { // incoming
            uint32_t* pinakas; // contains the returned neighbors of the node
            uint32_t s = 0; //gets the number of neighbors
            flag = 0;
            for(uint32_t j = 0; j < in_level1; j++) {
                if(qin.is_empty() == 1) {
                    qout.emptyQ();
                    delete[] exploredIN;
                    delete[] exploredOUT;
                    return -1;
                }
                node = qin.pop();
                uint32_t off = indin->getListhead(node);
                if(off == UNUSED) {
                    continue;
                }
                pinakas = indin->getNeighbors(s,off);
                for(uint32_t i = 0; i < s; i++) {  // for each neighbor
                    if(exploredIN[pinakas[i]] == 0) {  //if it hasn't been checked for the current flow
                        if(exploredOUT[pinakas[i]] == 1){//if it has been checked from the other flow too it means the result has been found
                            flag = 3;
                            break;
                        }
                        exploredIN[pinakas[i]] = 1; //check it
                        qin.push(pinakas[i]);
                        in_level2++;
                    }
                }
                delete[] pinakas;
                if(flag == 3) {
                    break;
                }
            }
            incntr++;
            in_level1 = in_level2;
            in_level2 = 0;
            if((in_level1==0)&&(flag!=3)){
                qin.emptyQ();
                qout.emptyQ();
                delete[] exploredIN;
                delete[] exploredOUT;
                return -1;
            }
        } else { // outgoing
            uint32_t* pinakas_out;
            uint32_t s = 0;
            flag = 1;
            for(uint32_t j = 0; j < out_level1; j++) {
                if(qout.is_empty() == 1) {
                    qin.emptyQ();
                    delete[] exploredIN;
                    delete[] exploredOUT;
                    return -1;
                }
                node = qout.pop();
                uint32_t off = indout->getListhead(node);
                if(off == UNUSED) {
                    continue;
                }
                pinakas_out = indout->getNeighbors(s,off);
                for(uint32_t i = 0; i < s; i++){
                    if(exploredOUT[pinakas_out[i]] == 0) {
                        if(exploredIN[pinakas_out[i]] == 1){
                            flag = 3;
                            break;
                        }
                        exploredOUT[pinakas_out[i]] = 1;
                        qout.push(pinakas_out[i]);
                        out_level2++;
                    }
                }
                delete[] pinakas_out;
                if(flag == 3) {
                    break;
                }
            }
            outcntr++;
            out_level1 = out_level2;
            out_level2 = 0;
            if((out_level1==0)&&(flag!=3)){
                qin.emptyQ();
                qout.emptyQ();
                delete[] exploredIN;
                delete[] exploredOUT;
                return -1;
            }
        }
    }
    qin.emptyQ();
    qout.emptyQ();
    delete[] exploredIN;
    delete[] exploredOUT;
    return incntr + outcntr;
}*/
