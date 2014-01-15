Working on Jet Flavor Tagging with AGILEPack, GAIA, and tagging-performance
=========

This serves as a tutorial for new members/contributors to flavor tagging in the Yale ATLAS Group, and as a reference for existing contributors. The entire tagging package is split into two parts--the algorithm and AI framework, and the plotting and verification framework. 
- AGILEPack will be the future AI framework for tagging within Yale ATLAS. It was designed and written by Luke de Oliveira `<luke.deoliveira@yale.edu>` to be a generic deep learning framework in `C++` with an interface to `ROOT` (this part is currently in development).
- GAIA is the current framework for tagging, and was also designed and written by Luke de Oliveira `<luke.deoliveira@yale.edu>`. This is being phased out in the next few weeks in favor of AGILEPack, but produced a extremely high-performance *b*/*c* tagger which is included in `rel19`. 
- tagging-performance is the current performance plotting framework designed and written by Dan Guest `<dguest@cern.ch>`.

####AGILEPack

###Dependencies

The only dependencies are a `C++11` compliant compiler, and the [`Eigen`](http://eigen.tuxfamily.org/) matrix library. If you're a Mac person, and you use [Homebrew](http://brew.sh) (which if you don't, you *need* to), then you can install `GCC 4.8` using:

```
user@computer$ brew tap homebrew/versions
user@computer$ brew install gcc48
```
You can then install `Eigen` using:

```
user@computer$ brew install eigen
```

If you use Linux, you can use your package manager (or build from source for you ambitious folks) to get and build `Eigen` and a `C++11` compliant compiler of your choice. 

If you use Windows, I have no idea what you do, but I'm sure others do (I'm sorry).

