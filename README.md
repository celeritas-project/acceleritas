# Acceleritas Geant!

With this incantation we hope to use the magic of GPU accelerators and Geant4's
new tasking model to offload EM showers from Geant4 to Celeritas to accelerate
HEP simulations.

# Installation

## Requirements:

- Geant4 (Version 10.7)
- VecGeom (Version 1.8)
- CUDA

## Configuring and building acceleritas

An example CMake command looks like with the CUDA Capability 7.0:
```sh
cmake ${SRC_DIR}/acceleritas  
      -D CELERITAS_USE_CUDA=ON \
      -D CELERITAS_USE_MPI=OFF \
      -D CELERITAS_USE_Geant4=ON  \
      -D CELERITAS_USE_VecGeom=ON \
      -D CMAKE_CUDA_FLAGS="-arch=sm_70" \
      -D CMAKE_BUILD_TYPE="RelWithDebInfo" \
      -D CMAKE_CXX_FLAGS="-Wall -Wextra -pedantic -Werror" \
      -D CMAKE_PREFIX_PATH="${VECGEOM_DIR};${XERCES_C_DIR}" \
      -D Geant4_DIR=${GEANT4_BUILD_DIR}
make
```

## Running an application (example: g4hpc)
```sh
ln -s ${SRC_DIR}/acceleritas/app/g4hpc/input input
./app/g4hpc/g4hpc input/run.mac
```
