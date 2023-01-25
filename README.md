# ross-dynamic-perceptron

# Installation

This model can be built by ROSS by sym-linking it into the ROSS/models directory and building with `-DROSS_BUILD_MODELS=ON`

``` shell
git clone https://github.com/ROSS-org/ROSS
cd ROSS/models
ln -s ../../ross-dynamic-perceptron ./
cd ../
mkdir build
cd build
cmake ../ -DROSS_BUILD_MODELS=ON
make
```
