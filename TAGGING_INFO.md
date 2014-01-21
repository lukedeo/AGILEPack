Working on Jet Flavor Tagging with Yale ATLAS
=========

This serves as a tutorial for new members/contributors to flavor tagging in the Yale ATLAS Group, and as a reference for existing contributors. The entire tagging package is split into two parts--the algorithm and AI framework, and the plotting and verification framework. 
- [AGILEPack](https://github.com/lukedeo/AGILEPack) will be the future AI framework for tagging within Yale ATLAS. It was designed and written by Luke de Oliveira `<luke.deoliveira@yale.edu>` to be a generic deep learning framework in `C++` with an interface to `ROOT` (this part is currently in development).
- [GAIA](https://github.com/lukedeo/GAIA) is the current framework for tagging, and was also designed and written by Luke de Oliveira `<luke.deoliveira@yale.edu>`. This is being phased out in the next few weeks in favor of AGILEPack, but produced a extremely high-performance *b*/*c* tagger which is included in `rel19`. 
- [tagging-performance](https://github.com/dguest/tagging-performance) is the current performance plotting framework designed and written by Dan Guest `<dguest@cern.ch>`.

##AGILEPack

###Dependencies

The only dependencies are a `C++11` compliant compiler, and the [`Eigen`](http://eigen.tuxfamily.org/) matrix library. If you're a Mac person, and you use [Homebrew](http://brew.sh) (which if you don't, you *ought* to), then you can install `GCC 4.8` using:

```
user@computer$ brew tap homebrew/versions
user@computer$ brew install gcc48
```
You can then install `Eigen` using:

```
user@computer$ brew install eigen
```

If you use Linux, you can use your package manager (or build from source for you ambitious folks) to get and build `Eigen` and a `C++11` compliant compiler of your choice. 

If you use Windows, I have no idea what you do, but I'm pretty sure others do (I'm sorry bout that, chaps).

###Installation

To get the package, you can use the standard `git` way which is
```
user@computer$ git clone https://github.com/lukedeo/AGILEPack.git
```

If you can't use `git` to pull the source code from a web hosted repository, you can simply use

```
user@computer$ wget https://github.com/lukedeo/AGILEPack/archive/master.zip
user@computer$ unzip master && mv AGILEPack-master AGILEPack
```

To install (assuming `Eigen` is installed), simply type

```
user@computer$ cd AGILEPack && cd SOURCECODE
user@computer$ make
```

This by default installs a local version of `yaml-cpp`. If you think you have it, or want other options for building this, please see the AGILEPack repository [`README.md`](https://github.com/lukedeo/AGILEPack/blob/master/README.md).

###Usage

Usage is changing rapidly. I try and keep up, and I can't. And I'm developing most of it.



