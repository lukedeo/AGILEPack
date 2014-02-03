Using the AGILEPack API (For HEP)
===========

The API for AGILEPack was designed to be as versatile as possible, while still allowing a power user to tweak the package to their own desires. This tutorial will show a very simple example using a fake ROOT file (which doesnt actually exist).

Suppose we want to make a *b*-tagger. Say we have an `TFile` called `training.root` over which we'd like to train a neural network. Let's also say that a *flat, numeric* `ntuple` lives inside a `TTree` called `Physics`. (**NOTE:** the `ntuple(s)` must be flat).

##Dealing with the data

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

This is great, but doesn't help us train a neural network. Enter, the `agile::dataframe`. Unfortunately, it's necessary for neural network training to have access to data stored in a structure that provides random pattern access. Thus, we can use the the `agile::dataframe` as a sort of temporary storage. Let's dump some entries from our `TTree` stored in `btag_reader` into a `dataframe`.

```
agile::dataframe D = btag_reader.get_dataframe(1000);
//                                ~~~~~~~~~~~~~~^
//                            Dumps 1000 entries

agile::dataframe D = btag_reader.get_dataframe(500, 12);
//                                ~~~~~~~~~~~~~~^    ^~~~~~~~
//                            Dumps 500 entries...   ...after skipping the first 12 entries

agile::dataframe D = btag_reader.get_dataframe();
//                                ~~~~~~~~~~~~^
//                            Dump ALL the things
```

Ah shoot, there's another ROOT file called `training_2.root` with a `TTree` called `more_physics` that I *also* want in this `dataframe`. Fear not!

```
agile::root::tree_reader another_reader;                   // declare a tree_reader instance
another_reader.add_file("training_2.root", "more_physics") // Load the file and TTree

// ...set all the branches for this one. (not included for brevity)

// we already have our `agile::dataframe` named `D` from before.
D.append(std::move(another_reader.get_dataframe(1000)));
//       ^~~~~~~~~^
//     Optional, but avoids copy

```

So, we can call `append()` to squash dataframes together (some checking of bounds and such happens in the background).

##Syntax for formulas

We don't know how to use the neural network training portion of the API yet, but it's important to document the formula syntax. Input and output variables and discriminants are specified using a *model formula*. This is a fancy way of saying that variable inclusion and exclusion can run by a formula parser that constructs what the neural network sees. The following characters are used to specify a formula:

| Character | Meaning |
|-----------|---------|
| `~` | Means "is a function of". This splits the left and right hand sides of what the neural network is estimating.|
| `+` | Means "Include the next variable in the neural network".|
| `-` | Means "Don't include the next variable in the neural network".|
| `*` | Standard glob. Means "Include everything not on the left hand side of the formula".|

In our case (with the branches mentioned in the previous section), the formula 
```
"bottom ~ *"
``` 
would mean "predict `bottom` using all other variables as inputs". The formula 
```
"bottom ~ * - eta"
``` 
would mean "predict `bottom` using all other variables *except* `eta` as inputs". The formula 
```
"bottom + pt ~ ip3d_pb + ip3d_pu + ip3d_pc + mass"
``` 
Is simply "predict `bottom` and `pt` as functions of everything on the right hand side". Note that formulas aren't space sensitive -- the formulas `bott om    ~ pt+eta +     ip3d_pb` and `bottom ~ pt + eta + ip3d_pb` are the exact same thing.


##Training a neural network.

```
agile::neural_net my_net;


```









