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
"bottom + pt ~ ip3d_pb + ip3d_pu + ip3d_pc + mass"
``` 
Is simply "predict `bottom` and `pt` as functions of everything on the right hand side". Note that formulas aren't space sensitive -- the formulas `bott om    ~ pt+eta +     ip3d_pb` and `bottom ~ pt + eta + ip3d_pb` are the exact same thing.

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

####`--help, -h`       
`Display this help message and exit.`

If you forget what the flags do, invoke this to get a reminder. No matter what other stuff you pass to AGILEPackTrainer, this message will be displayed instead if you pass `--help`.

####`--file, -f`
`Pass at least one file to add to a smart_chain for training.`
Here, we pass the ROOT files for training. If youre using *more than one file*, makes sure the tree names are the same across files, since we make a chain over the files. Passed as either `--file=file1.root file2.root ...`, `--file file1.root file2.root ...`, or `-ffile1.root file2.root ...`.
####`--tree, -t           Name of the TTree to extract from the ROOT file.
####`--save, -s           Name of file to save the YAML neural network file to. Something like my-model.yaml
####`--learning           Pass a learning rate.
####`--momentum           Pass a momentum value.
####`--regularize         Pass an l2 norm regularizer.
####`--batch              Mini-batch size.
####`--config, -c         Pass a configuration file containing branch names and numeric types of interest. Look at READMEPLZ for the format.
####`--struct             Pass the structure of the neural network. For example, one
                     could specify --struct=13 24 23 15 3 [other flags]. If not passed, a heuristic will choose one for you.
####`--quiet, -q          Make the output quiet. If not passed, the training procedure will tell you things, show you progress bars, etc.
####`--shuffle            If passed, AGILEPack will re-shuffle the data you pass before training. This is HIGHLY recommended.
####`--regression         By default, AGILEPack assumes classification. If you are performing regression, pass this flag.
####`--start              Start index for training. (Default = 0)
####`--end                End index for training. (Default, whole tree)
####`--uepochs            Number of passes over the Trees for Unsupervised Pretraining(Default = 5)
####`--sepochs            Number of passes over the Trees for Supervised Training(Default = 10)
####`--prog               Output progress files every (n) epochs. If n isn't passed,
                     uses default. (Default = 1)
####`--formula, -F        Specify Model Formula. Has to be of the form --formula="signal~var1+var2"






