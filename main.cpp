#include "misc_functions.h"

int main(int argc, char* argv[])
{
    if(argc < 2) { // command line options management
        show_usage(argv[0]);
        return -1;
    }
    string efilename;
    string ffilename;
    for (int i = 1; i < argc; ++i) {
        string arg = argv[i];
        if ((arg == "-h") || (arg == "--help")) {
            show_usage(argv[0]);
            return -1;
        } else if ((arg == "-f") || (arg == "--filename")) {
            if (i + 1 <= argc) { // Make sure we aren't at the end of argv!
                efilename = argv[++i]; // Increment 'i' so we don't get the argument as the next argv[i].
                ffilename = argv[++i]; 
                } else { // Uh-oh, there was no argument to the destination option.
                cerr << "--filename option requires one argument." << endl;
                return -1;
            }
        }
    }
    Index* index_incoming = new Index();
    Index* index_outgoing = new Index();
    ptr* pp;
    const clock_t begin_time = clock();
    pp = parser(efilename.c_str(), ffilename.c_str(), index_incoming, index_outgoing); // parse input file <filename> and create Graph representation
    pp->in->emptyBUF();
    pp->out->emptyBUF();
    delete pp->in;
    delete pp->out;
    delete pp;
    cout << "Time elapsed:  " << float(clock() - begin_time) / CLOCKS_PER_SEC << "  seconds" << endl;
    return 0;
}
