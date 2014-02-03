Using the AGILEPack API (For HEP)
===========

The API for AGILEPack was designed to be as versatile as possible, while still allowing a power user to tweak the package to their own desires. This tutorial will show a very simple example using a fake ROOT file (which doesnt actually exist).

Suppose we want to make a *b*-tagger. Say we have an `TFile` called `training.root` over which we'd like to train a neural network. Let's also say that a *flat, numeric* `ntuple` lives inside a `TTree` called `Physics`. (**NOTE:** the `ntuple(s)` must be flat).

For the sake of this tutorial, suppose that the `ntuple` has the following branches:

| Branch Name | (C/C++) Numeric Type |
|-------------|--------------------------|
| `bottom`| `int` |
| `nTracks`| `int` |
| `nVTX`| `int` |
| `ip3d_pb`| `double` |
| `ip3d_pu`| `double` |
| `ip3d_pc`| `double` |
| `mass`| `float` |
| `significance3d`| `float` |
| `pt`| `float` |
| `eta`| `float` |

For the estimation of the posterior probability that a given jet is a bottom jet, we need that that the `bottom` branch is a `1` if a jet is a bottom, and a `0` otherwise.

Now, let's see how the AGILEPack API helps us load this into a usable format.

```
agile::root::tree_reader btag_reader;            // declare a tree_reader instance
btag_reader.add_file("training.root", "Physics") // Load the file and TTree

// Set all the branches. Notice the passing of types.
btag_reader.set_branch("bottom", agile::root::integer);
btag_reader.set_branch("nTracks", agile::root::integer);
btag_reader.set_branch("nVTX", agile::root::integer);
btag_reader.set_branch("ip3d_pb", agile::root::double_precision);
btag_reader.set_branch("ip3d_pu", agile::root::double_precision);
btag_reader.set_branch("ip3d_pc", agile::root::double_precision);
btag_reader.set_branch("mass", agile::root::single_precision);
btag_reader.set_branch("significance3d", agile::root::single_precision);
btag_reader.set_branch("pt", agile::root::single_precision);
btag_reader.set_branch("eta", agile::root::single_precision);
//                     ^~~~^  ^~~~~~~~~~~~~~~~~~~~~~~~~~~~^  
//                  branch name       the numeric type
```

What happens now? Well, suppose we want to get the value of `pt` at the 4th entry. We can call:

```
btag_reader(4, "pt")
```

This is great, but doesn't help us train a neural network. 