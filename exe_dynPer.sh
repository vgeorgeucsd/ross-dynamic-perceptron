#~/bin/bash

cd /home/vivek/research/dynamic_perceptron/working_ross/ROSS/build
cmake ../ -DROSS_BUILD_MODELS=ON
make
cd /home/vivek/research/dynamic_perceptron/working_ross/ROSS/build/models/ross-dynamic-perceptron
./dynPer --extramem=100000
