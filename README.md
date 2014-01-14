AGILEPack
=========

Algorithms for Generalized Inference, Learning, and Extraction Package, By Luke de Oliveira.

####Dependencies
- `Eigen` matrix library (header only) for fast matrix operations.
- compiler support for C++11.
- `yaml-cpp` for `YAML` parsing, but a version is made locally by default.

####Summary

This is a work in progress, with the ultimate goal of a versatile Deep Learning library in C++. Support will be provided for interfacing with `*.root` files and providing complete training and testing specifications within the `YAML` specification and serialization file.


####Installation

The package can be downloaded using
```
git clone https://github.com/lukedeo/AGILEPack.git
```
and is relatively basic to install. A standard installation can run with 

```
make
```
whick builds a local `yaml-cpp` library called `libYAMLCPP_INTERNAL.so`, which is then linked to the AGILEPack excecutable. 

If you know **for sure** that `yaml-cpp` has been installed in a standard linker search path such as `/usr/lib` or `/usr/local/lib` (or you've hacked it into the linker search), then you can install just the AGILEPack software linking to your pre-existing library using

```
make basic
```

If this gives you something on the order of 

```ld: linker something_error exit status 1```, 

then simply build the local `yaml-cpp` shared library using the simply 

```
make
```








####Algorithms and General Features

- [x] Backpropagation for arbitrary layer structure.
- [ ] Support for:
  - [x] Linear layers with SSE loss.
  - [x] Sigmoidal layers with SSE loss.
  - [x] Softmax layers with Cross Entropy loss.
  - [x] Rectified linear unit layers.
  - [x] Autoencoder pre-training (stacked, denoising, etc.)
  - [ ] Restricted Boltzmann Machine pre-training.
  - [ ] Dropout/DropConnect layers.
  - [ ] Inverted Deep Network Encoding. 
- [x] Dynamic changes to layers (additions, deletions).
- [ ] Interface with the CERN `ROOT` framework.
- [x] Complete serialization of network structure with `YAML` file.
- [ ] More to come...





