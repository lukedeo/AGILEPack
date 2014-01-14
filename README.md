AGILEPack
=========

Algorithms for Generalized Inference, Learning, and Extraction Package, By Luke de Oliveira.

####Dependencies
- `Eigen` matrix library (header only) for fast matrix operations.
- Compiler support for C++11.
- `yaml-cpp` for [`YAML`](http://www.yaml.org/ "YAML Homepage") parsing, but a version is made locally by default.

####Summary

This is a work in progress, with the ultimate goal of a versatile Deep Learning library in C++. Support will be provided for interfacing with `*.root` files and providing complete training and testing specifications within the `YAML` specification and serialization file.


####Installation

The package can be downloaded using
```
git clone https://github.com/lukedeo/AGILEPack.git
```
and is relatively basic to install. I *highly* recommend letting `make` do it's thing and build my slight hack of the `yaml-cpp` library, since I use `std::unique_ptr<T>` and `std::move` functionality, which definitely ain't a bad thang. There's also a stripped `boost` mod shipped with this, since `yaml-cpp` uses it. A standard installation *including* `yaml-cpp` can built with 

```
make
```
which builds a local `yaml-cpp` library called `libYAMLCPP_INTERNAL.so`, which is then linked to the AGILEPack excecutable. 

If you know **for sure** that `yaml-cpp` has been installed in a standard linker search path such as `/usr/lib` or `/usr/local/lib` (or you've hacked it into the linker search), then you can install just the AGILEPack software linking to your pre-existing library using

```
make basic
```

If this gives you something on the order of 

```ld: linker some_cryptic_error exit status 1```, 

then simply build the local `yaml-cpp` shared library using simply

```
make
```

If you are using this in a performance sensitive application, compile with

```
make production
```

which will compile with `-O2` optimization, and the `Eigen` flag `-DEIGEN_NO_DEBUG`, which turns off some expensive dimension checks. 











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

####I have qualms with AGILEPack

If there's something wrong with *anything* here, please bother me at `luke.deoliveira@yale.edu`.





