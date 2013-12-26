AGILEPack
=========

Algorithms for Generalized Inference, Learning, and Extraction Package, By Luke de Oliveira.

####Dependencies
`Eigen` matrix library for fast network operations, `yaml-cpp` for `YAML` parsing, compiler support for C++11.

####Summary

This is a work in progress, with the ultimate goal of a versatile Deep Learning library in C++. Support will be provided for interfacing with `*.root` files and providing complete training and testing specifications within the `YAML` specification and serialization file.


####Algorithms and General Features

- [x] Backpropagation for arbitrary layer structure.
- [ ] Support for:
  - [x] Linear layers with SSE loss.
  - [x] Sigmoidal layers with SSE loss.
  - [x] Softmax layers with Cross Entropy loss.
  - [ ] Rectified linear unit layers.
  - [ ] Autoencoder pre-training (stacked, denoising, etc.)
  - [ ] Restricted Boltzmann Machine pre-training.
  - [ ] Dropout/DropConnect layers.
  - [ ] Inverted Deep Network Encoding. 
- [x] Dynamic changes to layers (additions, deletions).
- [ ] Interface with the CERN `ROOT` framework.
- [x] Complete serialization of network structure with `YAML` file.
- [ ] More to come...





