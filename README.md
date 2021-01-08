System Software Development Project, Part I, II, III

Ευγένιος Τσίγκανος, sdi1300184
Δημήτρης Φλουρής, sdi1300222
Κυριάκος Χριστοδούλου, sdi1300211

Makefile included -- to compile run <make> and it produces the executable <exec>
To include a data file run with option <-f> or <--filename> such as: 
<./exec -f ./datasets/tiny/tinyGraph.txt datasets/tiny/tinyWorkload_FINAL.txt>,
first file argument containing the Graph edges and the second the Queries(Q) and Additions(A).
Alternatively, run <make runT>, <make runS>, or <make runM> 
to compile and run with the pre-specified arguments corresponding to Tiny, Small and Medium Graphs.
If other graphs are provided, their location must be specified at the appropiate Makefile arguments, and then run <make runCustom>.


implementation also can be found @gitlab, along with complete commit history.
