### To run with custom graph change ONLY the below two lines:
CustomGraph="<specify path to data>"
CustomGraphWorkload="<specify path to Workload>"
###

#CC=g++
CC=clang++

CFLAGS=-Wall -Wextra -g3 -pthread

.PHONY: clean distclean runT runS runM swpclean no_duplicatesM no_duplicatesS no_duplicatesT no_duplicatesCustom runCustom no_duplicatesL runLD runLS

exec: main.o LinkedList.o LinkedList.h queuej.o queuej.h jobsched.o jobsched.h graph_functions.o misc_functions.o misc_functions.h cacao.o queue.o graph_header.h queue.h stack.h stack.o cc_functions.o cc_header.h scc_functions.o scc_header.h grail.o grail.h
	$(CC) $(CFLAGS) -o exec main.o LinkedList.o jobsched.o queuej.o graph_functions.o misc_functions.o cacao.o queue.o stack.o cc_functions.o scc_functions.o grail.o

main.o: main.cpp
	$(CC) -c -g3 main.cpp

LinkedList.o: LinkedList.cpp
	$(CC) -c -g3 LinkedList.cpp

queuej.o: queuej.cpp
	$(CC) -c -g3 queuej.cpp

jobsched.o: jobsched.cpp
	$(CC) -c -g3 jobsched.cpp

graph_functions.o: graph_functions.cpp
	$(CC) -c -g3 graph_functions.cpp

cacao.o: cacao.cpp
	$(CC) -c -g3 cacao.cpp

misc_functions.o: misc_functions.cpp
	$(CC) -c -g3 misc_functions.cpp

cc_functions.o: cc_functions.cpp
	$(CC) -c -g3 cc_functions.cpp

scc_functions.o: scc_functions.cpp
	$(CC) -c -g3 scc_functions.cpp

queue.o: queue.cpp
	$(CC) -c -g3 queue.cpp

stack.o: stack.cpp
	$(CC) -c -g3 stack.cpp

grail.o: grail.cpp
	$(CC) -c -g3 grail.cpp

no_duplicatesT:
	uniq -u ./datasets/tiny/tinyGraph.txt no_duplicates_tiny.txt

no_duplicatesS:
	uniq -u ./datasets/small/smallGraph.txt no_duplicates_small.txt

no_duplicatesM:
	uniq -u ./datasets/medium/mediumGraph.txt no_duplicates_medium.txt

no_duplicatesL:
	uniq -u ./datasets/large/large.txt no_duplicates_large.txt

no_duplicatesCustom:
	uniq -u $(CustomGraph) no_duplicates_custom.txt

runT: exec no_duplicatesT
	./exec -f no_duplicates_tiny.txt ./datasets/tiny/tinyWorkload_FINAL.txt

runS: exec no_duplicatesS
	./exec -f no_duplicates_small.txt ./datasets/small/smallWorkload_FINAL.txt

runM: exec no_duplicatesM
	./exec -f no_duplicates_medium.txt ./datasets/medium/mediumWorkload_FINAL.txt

runLD: exec no_duplicatesL
	./exec -f no_duplicates_large.txt ./datasets/large/dynamic/largeWorkload_6000_20.txt

runLS: exec no_duplicatesL
	./exec -f no_duplicates_large.txt ./datasets/large/static/largeWorkload_48000_40.txt

runCustom: exec no_duplicatesCustom
	./exec -f no_duplicates_custom $(CustomGraphWorkload)

clean:
	rm -f ./*.o

swpclean:
	rm -f ./*.swp

distclean: clean swpclean
	rm -f ./exec
