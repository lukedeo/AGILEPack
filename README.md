AGILEPack
=========

Algorithms for Generalized Inference, Learning, and Extraction Package, By Luke de Oliveira.

####Dependencies
- [`Eigen`](http://eigen.tuxfamily.org/) matrix library (header only) for fast matrix operations.
- Compiler support for C++11. If you're not sure and want to cross check your compiler version to see if it's ok, here's some support info about [GCC](http://gcc.gnu.org/projects/cxx0x.html) and [Clang](http://clang.llvm.org/cxx_status.html). Other compilers haven't been tested yet.
- [`yaml-cpp`](https://code.google.com/p/yaml-cpp/ "yaml-cpp Homepage") for [`YAML`](http://www.yaml.org/ "YAML Homepage") parsing, but a version is made locally by default.

####Summary

This is a work in progress, with the ultimate goal of a versatile Deep Learning library in C++. Support will be provided for interfacing with `*.root` files and providing complete training and testing specifications within the `YAML` specification and serialization file.


####Installation

The package can be downloaded using
```
git clone https://github.com/lukedeo/AGILEPack.git
```
and is relatively basic to install. As long as `Eigen` is in a place such that

```c++ 
#include <Eigen/Dense>
```
won't cause your compiler to yell at you, you should be able to build the whole shebang with

```
make
```

This will build a static library called `lib/libAGILEPack.a`, which you can then link against to do sweet things like build Deep Learners.

####Basic Usage

Let's say you have a program called `prog.cxx` that uses AGILEPack with ROOT stuff. Provided that your file takes the form
```c++
#include "Base"
           ^~~~~~ Includes all AGILEPack stuff!
#include <myotherheaders.h>

int main(int argc, char const *argv[])
{
	// Do stuff with AGILEPack!
}
```

You can compile this program with:

```bash
g++ -o prog prog.cxx `/path/to/AGILEPack/agile-config build --root`  
              ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~^
      fixes include paths and links -lAGILEPack
```

which will produce an excecutable called `prog`.

The ordering of these arguments is **very important** on Linux (particularly on SLC)!


####More compiling and linking stuff

Let's say you're using AGILEPack as part of a larger project, and you need to produce a *.o and then then do other things. Stealing the example from the last section, let's say we want to produce `prog.o`. We can do this with

```bash
g++ -c `/path/to/AGILEPack/agile-config compile --root` prog.cxx -o prog.o
```

then link it to other stuff with 

```bash
g++ -o MyExcecutable prog.o [other *.o ...] `/path/to/AGILEPack/agile-config link --root`
```




####Algorithms and General Features

- [x] Backpropagation for arbitrary layer structure.
- [ ] Support for:
  - [x] Linear layers with SSE loss.
  - [x] Sigmoidal layers with SSE loss.
  - [x] Softmax layers with Cross Entropy loss.
  - [x] Rectified linear unit layers.
  - [x] Autoencoder pre-training (stacked, denoising, etc.)
  - [ ] Dropout/DropConnect layers.
- [x] Dynamic changes to layers (additions, deletions).
- [x] Interface with the CERN `ROOT` framework.
- [x] Complete serialization of network structure with `YAML` file.
- [ ] More to come...

####I have qualms with AGILEPack

If there's something wrong with *anything* here, please bother me at `luke.deoliveira@yale.edu`.





