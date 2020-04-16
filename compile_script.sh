#~/bin/bash

# fileName=igraph_test
fileName=igraph_test_erdos_renyi_game

# to compile we need
gcc $fileName.c -I/home/vivek/research/dynamic_perceptron/livermore/new_template_model/igraph/include -L/home/vivek/research/dynamic_perceptron/livermore/new_template_model/igraph/src/.libs -ligraph -o $fileName

# to run the executable
# the next line does not seem to be needed
# LD_PRELOAD=/home/vivek/research/dynamic_perceptron/livermore/new_template_model/igraph/src/.libs/libigraph.so ./$fileName
./$fileName
