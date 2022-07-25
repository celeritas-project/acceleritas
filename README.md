# Acceleritas Geant!

With this incantation we hope to use the magic of GPU accelerators and Geant4's
new tasking model to offload EM showers from Geant4 to Celeritas to accelerate
HEP simulations.

# Installation

## Requirements:

- CUDA
- Geant4 (Version 11.0) (with GEANT4_USE_GDML=ON)
- VecGeom (Version 1.1.18) (with DBACKEND="Scalar" and CUDA="ON")
- ROOT (Version 6.XX.X)

## Configuring and building acceleritas 

An example CMake command looks like with the CUDA Capability 7.0
(assuming that celeritas required products are installed at ${INSTALL_DIR}:
```sh
cmake ${SRC_DIR}/acceleritas \ 
      -D CELERITAS_USE_CUDA=ON \
      -D CELERITAS_USE_Geant4=ON  \
      -D CELERITAS_USE_VecGeom=ON \
      -D CELERITAS_USE_ROOT=ON \
      -D CMAKE_CUDA_ARCHITECTURES=70 \
      -D CMAKE_BUILD_TYPE="RelWithDebInfo" \
      -D CMAKE_CXX_FLAGS="-Wall -Wextra -pedantic -Werror" \
      -D CMAKE_PREFIX_PATH="${INSTALL_DIR};${XERCES_C_DIR}" \
      -D Geant4_DIR=${GEANT4_BUILD_DIR}
make -j
```

## Running an application (example: acc01 with 4 threads)
```sh
ln -s ${SRC_DIR}/acceleritas/app/acc01/input input
./app/acc01/acc01 -m input/run.mac -t 4
```
