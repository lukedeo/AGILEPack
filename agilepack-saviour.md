AGILEPack 
=========
##A guide to installing it, using it, and not dying from frustration.

First, a little background -- AGILEPack was the codebase for my senior thesis, and while I tried my best to write good code, I have never and will never purport to be a software engineer. Thus, you should not always assume that an error is without a doubt your fault -- it very well could be, but it could also be a bug. This is also an open invitation -- feel free to edit then tell me / open an issue.


First, I will walk through installation, then configuration, then actually running the darn thing and how to "get plots".

###Installation


AGILEPack does not have a ton of dependencies, but those which it has are mandatory. 

- [`Eigen`](http://eigen.tuxfamily.org/) matrix library (header only) for fast matrix operations.
- Compiler support for C++11. If you're not sure and want to cross check your compiler version to see if it's ok, here's some support info about [GCC](http://gcc.gnu.org/projects/cxx0x.html) and [Clang](http://clang.llvm.org/cxx_status.html). Other compilers haven't been tested yet.
- ROOT. I'm not an expert in it, I know it can be a pain. I have minimal dependencies on ROOT -- entirely for data reading.


The package can be downloaded using
```bash
git clone https://github.com/lukedeo/AGILEPack.git
```

and is relatively basic to install. As long as `Eigen` is in a place such that

```c++
#include <Eigen/Dense>
```


Won't cause your compiler to yell at you, you should be able to build the entire project with `make -j`. Afterwards, I recommend you add the path you built AGILEPack in to your `PATH`. If you don't know what that means, you should paste

```bash
echo "export PATH+=:$(pwd)" >> ~/.bashrc
```

or 

```bash
echo "export PATH+=:$(pwd)" >> ~/.bash_profile
```
depending on what you use.


###File format and Formulas

The API for AGILEPack was designed to be as versatile as possible, while still allowing a power user to tweak the package to their own desires. However, there are a few formatting requirements which are NOT negotiable. I will assume that the user will be using the command line interface rather than the API. 

Care must be taken to ensure that the training ntuples are *flat*. That is, every entry in the TTree *must* be a jet, rather than an event, meaning the values will be *numeric* not vectors or D3PD type things. If we are doing classification, we need a branch that indicates whether or not a jet is a signal or backgroud jet. A value of 1 indicates signal, and 0 indicates background.

I'll use the following b-tagging example to illustrate usage -- suppose these are the branch names and associated types in a file called `training.root`, and are within a `TTree` called `physics.

*IMPORTANT* -- AGAIN, IF YOU ARE DOING CLASSIFICATION, YOU NEED A BRANCH THAT IS A 1 IF A JET IS SIGNAL, 0 OTHERWISE.


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



In this case, `bottom` is our signal variable -- we are trying to tag b-jets after all! We now need to learn how to specify a formula to AGILEPack.

We don't know how to use the command line interface yet, but it's important to document the formula syntax. Input and output variables and discriminants are specified using a *model formula*. This is a fancy way of saying that variable inclusion and exclusion can be 
run by a formula parser that constructs what the neural network sees. The following characters are used to specify a formula:

| Character | Meaning |
|-----------|---------|
| `~` | Means "is a function of". This splits the left and right hand sides of what the neural network is estimating.|
| `+` | Means "Include the next variable in the neural network".|
| `-` | Means "Don't include the next variable in the neural network".|
| `*` | Standard glob. Means "Include everything not on the left hand side of the formula".|
| `|` | Means "weight by this variable."|

In our case (with the branches mentioned in the previous section), the formula 
```
"bottom ~ *"
``` 
would mean "predict `bottom` using all other variables in the tree as inputs". The formula 
```
"bottom ~ * - eta"
``` 
would mean "predict `bottom` using all other variables *except* `eta` as inputs". The formula 
```
"bottom + pt ~ ip3d_pb + ip3d_pu + ip3d_pc + mass | jetwt"
``` 
Is simply "predict `bottom` and `pt` as functions of everything on the right hand side, and weight by jetwt". Note that formulas aren't space sensitive -- the formulas `bott om    ~ pt+eta +     ip3d_pb` and `bottom ~ pt + eta + ip3d_pb` are the exact same thing.

A note about weights -- they often have wildly varying orders of magnitude, and should not be used on a first attempt.

###Command Line Interface

Now we will examine how to train a neural network using AGILEPack. After you invoke `make`, you should have an executable called `AGILEPackTrainer`. You should type `AGILEPackTrainer --help` at your terminal and see the following.


```
luke@hep [~/AGILEPack]$ AGILEPackTrainer --help
usage: AGILEPackTrainer [-h] [options]

A simple interface for studies using AGILEPack.

    --help, -h           Display this help message and exit.
    --file, -f           Pass at least one file to add to a smart_chain for training.
    --tree, -t           Name of the TTree to extract from the ROOT file.
    --save, -s           Name of file to save the YAML neural network file to. Something like my-model.yaml
    --learning           Pass a learning rate.
    --momentum           Pass a momentum value.
    --regularize         Pass an l2 norm regularizer.
    --batch              Mini-batch size.
    --config, -c         Pass a configuration file containing branch names and numeric types of interest. Look at READMEPLZ for the format.
    --struct             Pass the structure of the neural network. For example, one
                         could specify --struct=13 24 23 15 3 [other flags]. If not passed, a heuristic will choose one for you.
    --quiet, -q          Make the output quiet. If not passed, the training procedure will tell you things, show you progress bars, etc.
    --shuffle            If passed, AGILEPack will re-shuffle the data you pass before training. This is HIGHLY recommended.
    --regression         By default, AGILEPack assumes classification. If you are performing regression, pass this flag.
    --start              Start index for training. (Default = 0)
    --end                End index for training. (Default, whole tree)
    --uepochs            Number of passes over the Trees for Unsupervised Pretraining(Default = 5)
    --sepochs            Number of passes over the Trees for Supervised Training(Default = 10)
    --prog               Output progress files every (n) epochs. If n isn't passed,
                         uses default. (Default = 1)
    --formula, -F        Specify Model Formula. Has to be of the form --formula="signal~var1+var2"
```
Let's look further!

###Training Parameters

Now, we will go through each parameter and give an explanation.

##`--help, -h`       
`Display this help message and exit.`

If you forget what the flags do, invoke this to get a reminder. No matter what other stuff you pass to AGILEPackTrainer, this message will be displayed instead if you pass `--help`.

##`--file, -f`
`Pass at least one file to add to a smart_chain for training.`

Here, we pass the ROOT files for training. If youre using *more than one file*, makes sure the tree names are the same across files, since we make a chain over the files. Passed as either `--file=file1.root file2.root ...`, `--file file1.root file2.root ...`, or `-ffile1.root file2.root ...`.
##`--tree, -t`
`Name of the TTree to extract from the ROOT file.` 

This should simply be the name of the TTree you plan to train on
##`--save, -s`
`Name of file to save the YAML neural network file to. Something like my-model.yaml`

This is the name of the file that AGILEPack will save YAML files to. YAML is the markup language I chose to perform serialization of networks.

##`--quiet, -q`
`Make the output quiet. If not passed, the training procedure will tell you things, show you progress bars, etc.`

If you submit in a batch system, DEFINITELY pass the `--quiet` flag, as the amount of output will be nasty to deal with in the output files. It's nice to see if you're running interactively.

##`--shuffle`
`If passed, AGILEPack will re-shuffle the data you pass before training. This is HIGHLY recommended.`


Many times, people have seperate signal and background ntuples. In this instance, if the tree names are the same, you can invoke the program as
```
luke@hep [~/AGILEPack]$ AGILEPackTrainer --file=signalfile.root backgroundfile.root --tree=physics --shuffle
```
and AGILEPack will combine and randomize the training file for you.

##`--regression`
`By default, AGILEPack assumes classification. If you are performing regression, pass this flag.`

This is straightforward -- most times, you will be performing classification, so you can mostly ignore this.
##`--start, --end`             

`Start index for training. (Default = 0) End index for training. (Default, whole tree)`

This is only relevant if you aren't shuffling -- it allows you to skip over certain entries if you want to do validation, etc. For all intents and purposes, this can be ignored.

##`--uepochs`
`Number of passes over the Trees for Unsupervised Pretraining (Default = 5)`

For a dataset with `n` training samples, the unsupervised step will pass over `uepochs * n` randomly sampled data points, training the autoencoders.

##`--sepochs`

` Number of passes over the Trees for Supervised Training (Default = 10)`

For a dataset with `n` training samples, the unsupervised step will pass over `sepochs * n` randomly sampled data points, providing supervised fine tuning.

##`--prog`

`Output progress files every (n) epochs. If n isn't passed, uses default. (Default = 1)`

This helps with "backup" files, in case a job crashes, or you want to track training progress. If you pass 0, then progress files will not be outputted.

##`--formula, -F`

`Specify Model Formula. Has to be of the form --formula="signal~var1+var2"`

Consult the aforementioned guide to constructing the model formula.

##`--config, -c`
`Pass a configuration file containing branch names and numeric types of interest.`

The config file is *required*, and provides specifications for data types, preselection, and the branches to be pulled from the tree. If you use a formula with a glob, then all the variables listed here will be included. It needs to be in YAML format -- perhaps saved as `config.yaml`. Here is an example.

```yaml
branches:
  pt: double
  eta: double
  nSingleTracks: int
  nTracks: int
  nTracksAtVtx: int
  nVTX: int
  SV1: double
  SV0: double
  ip3d_pb: double
  ip3d_pu: double
  ip3d_pc: double
  bottom: int
constraints:
  pt: [200, 1500]
  abs(eta): [0, 2.5]
```

##`--learning`
`Pass a learning rate.`

This is the learning rate that the network will use to move along the gradient direction. This is relatively problem dependent, but a reasonable choice is usually between 0.01 and 0.0005. The standard I have used is 0.001 with datasets on the order of 400,000 to 1 million and above entries, with larger learning rates on the order of 0.1 used on datasets on the order of 60,000 entries. For datasets with less than 200,000 entries, deep learning will be extremely sensitive to parameters and is not necessarily optimal -- this should be reserved for dataset on the order of 1 million entries or greater.

##`--momentum`
`Pass a momentum value`

This is a parameter that controls the lagging change in the gradient -- it makes updates "sticky". The usual value is between 0.7 and 0.95. Momentum makes the update at time `t`, `u(t)`, equal to 

```
u(t) = gamma * u(t - 1) + (1 - gamma) * gradient(t)
```

That is, if the gradient changes `A` units in one direction for the first jet, and `-B` units the next jet, the actual gradient update will be 

```
(1 - gamma) * A units
```

for the the first jet, and 

```
gamma * ((1 - gamma) * A units) + (1 - gamma) * (-B) units
```

for the second, and so on... 

This acts as a smoothing agent on the spiky gradient updates.

##`--regularize`
`Pass an l2 norm regularizer.`

This is the neural network equivalent of a ridge regularizer -- it forces the magnitude of weights in the network to be small to avoid overfitting. This parameter is HIGHLY sensitive, and usually should be no more that 1E-4.

##`--batch`
`Mini-batch size.`

Dictates how many jets the network should consider before making a gradient update. Usually, 1 is used for HEP since we have a lot of noise and discontinuity -- I wouldn't change it unless you have a strong reason to.


##`--struct`
`Pass the structure of the neural network. For example, one could specify --struct=13 24 23 15 3 [other flags]. If not passed, a heuristic will choose one for you.`

Generally, we want a network with at least 3 hidden layers. In addition, the first later after the inputs should have more nodes than inputs.

###Putting it all together.

Lets see how a run of AGILEPack could look! Lets suppose we have the same branches that we mentioned before, in a file called `training.root`, and in a tree called `physics`. In addition, lets suppose we have a file called `config.yaml` that looks like this:

```yaml
branches:
  bottom: int
  nTracks: int
  nVTX: int
  ip3d_pb: double
  ip3d_pu: double
  ip3d_pc: double
  mass: float
  significance3d: float
  pt: float
  eta: float
constraints:
  pt: [200, 1500]
  abs(eta): [0, 2.5]
```

Now, how would we train this? We could call:

```bash

luke@hep [~/AGILEPack]$ AGILEPackTrainer --file=training.root --tree=physics --config=config.yaml --shuffle --learning=0.001 --momentum=0.9 --regularize=0.000001 --sepochs=15 --save=agilepack-model.yaml

```

All this worked? Awesome! In order to get the posteriors from the network, I use Python and [AGILEPy](http://lukedeo.github.io/AGILEPy/), but you can use C++. Follow the example at the end [here](http://lukedeo.github.io/AGILEPack) to do that. We can add more here with demand.






