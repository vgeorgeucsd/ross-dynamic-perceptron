#~/bin/bash
NUMHIDNODES=10

#ROSSMODELDIR=/home/vivek/research/dynamic_perceptron/working_ross/ROSS/build/models/ross-dynamic-perceptron
#MODELDIR=/home/vivek/research/dynamic_perceptron/working_ross/ross-dynamic-perceptron
#MSFTDIR=/home/vivek/research/dynamic_perceptron/ucsd_ceni/ucsd_ceni_msft/scripts
#ROSSBUILDDIR=/home/vivek/research/dynamic_perceptron/working_ross/ROSS/build
#OUTPUTDIR=/home/vivek/research/dynamic_perceptron/working_ross/ross-dynamic-perceptron/temp_outputs

ROSSMODELDIR=/g/g20/george39/ROSS/build/models/ross-dynamic-perceptron
MODELDIR=/p/lustre1/george39/ross-dynamic-perceptron
MSFTDIR=/g/g20/george39/ucsd_ceni_msft/scripts
ROSSBUILDDIR=/g/g20/george39/ROSS/build
#OUTPUTDIR=/g/g20/george39/ross-dynamic-perceptron/temp_outputs
OUTPUTDIR=/p/lustre1/george39/simulations/ROSS/z_sample_outputs_${NUMHIDNODES}

TOTALNODES=$((${NUMHIDNODES} + 784 + 4))

# for debug
#TOTALNODES=3
#NUMHIDNODES=3

cd ${MSFTDIR}
sed "s/NUMBEROFHIDDENNODES/$NUMHIDNODES/g" create_ross_inputs.py > create_ross_inputs_${NUMHIDNODES}.py

# conda activate vivek_ucsd_ceni
python create_ross_inputs_${NUMHIDNODES}.py
#mv vertex_info_source_${NUMHIDNODES}.out ${MODELDIR}
#mv edge_info_source_${NUMHIDNODES}.out ${MODELDIR}
rm create_ross_inputs_${NUMHIDNODES}.py

cd ${MODELDIR}

mv vertex_info_source_${NUMHIDNODES}.out vertex_info_source_${TOTALNODES}.out
mv edge_info_source_${NUMHIDNODES}.out edge_info_source_${TOTALNODES}.out
sed "s/TOTALNUMBEROFNODES/$TOTALNODES/g" network_parameters.h > network_parameters_${TOTALNODES}.h
sed "s/TOTALNUMBEROFNODES/$TOTALNODES/g" dynPer.c > dynPer_${TOTALNODES}.c
sed "s/TOTALNUMBEROFNODES/$TOTALNODES/g" dynPer.h > dynPer_${TOTALNODES}.h
sed "s/TOTALNUMBEROFNODES/$TOTALNODES/g" generate_graph.h > generate_graph_${TOTALNODES}.h
sed "s/TOTALNUMBEROFNODES/$TOTALNODES/g" generate_stimulus.h > generate_stimulus_${TOTALNODES}.h
sed "s/TOTALNUMBEROFNODES/$TOTALNODES/g" CMakeLists.txt.var > CMakeLists.txt

cd ${OUTPUTDIR}
rm *

cd ${ROSSBUILDDIR}
cmake ../ -DROSS_BUILD_MODELS=ON
make
cd ${ROSSMODELDIR}
./dynPer --extramem=1000000 --synch=1 --clock-rate=2100000000
# mpirun -np 1 ./dynPer --synch=4 --nkp=1
#mpirun -np 3 ./dynPer --synch=3 --extramem=10000
#./dynPer --synch=1
#srun -n 12 ./dynPer --synch=3 --extramem=1000000 --clock-rate=2100000000

cd ${MODELDIR}
rm vertex_info_source_${TOTALNODES}.out
rm edge_info_source_${TOTALNODES}.out
#rm dynPer_${TOTALNODES}.h
#rm dynPer_${TOTALNODES}.c
#rm network_parameters_${TOTALNODES}.c
#rm generate_graph_${TOTALNODES}.c
