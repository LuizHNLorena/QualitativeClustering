# Qualitative Clustering via Integer Linear Programming

This repository presents the C++ implementation of the ILP models described in the article _"Qualitative data clustering: a new Integer Linear Programming model"_. 

## Table of contents
* [General info](#general-info)
* [Installation](#install)
* [Setup](#setup)

## General info

Two header libraries are avaiable:

* [ILP.h](ILP.h): creates and runs the ILP models on the [IBM's CPLEX Optimizer™](http://www.ibm.com/software/commerce/optimization/cplex-optimizer/)

* [LPCreator.h](LPCreator.h): creates the ILP models on [CPLEX LP format](https://www.ibm.com/support/knowledgecenter/SSSA5P_12.5.0/ilog.odms.cplex.help/CPLEX/FileFormats/topics/LP.html), so it can be run on any compatible solver.

## Installation
------------

### Compiling [ILP.h](ILP.h)

[ILP.h](ILP.h) depends on [IBM's CPLEX Optimizer™](http://www.ibm.com/software/commerce/optimization/cplex-optimizer/) 12>. 

* **Compiling on LINUX/MAC:** You can follow the rules defined in [Setting up CPLEX on GNU/Linux](https://www.ibm.com/support/knowledgecenter/SSSA5P_12.7.1/ilog.odms.cplex.help/CPLEX/GettingStarted/topics/set_up/GNU_Linux.html)
to compile it on Linux/Mac. Let's suppose you are on Mac and `<CPLEX_DIR>` is the directory where CPLEX is installed on your system, the following command will compile a `main.cpp` file that uses `ILP.h`: 

```
g++ main.cpp -o main -I/<CPLEX_DIR>/cplex/include 
                     -I/<CPLEX_DIR>/concert/include
                     -DIL_STD 
                     -L/<CPLEX_DIR>/cplex/lib/x86-64_osx/static_pic/ 
                     -L/<CPLEX_DIR>/concert/lib/x86-64_osx/static_pic/ 
                     -lilocplex -lconcert -lcplex -lm -lpthread -std=c++11
```

* **Compiling on WINDOWS:** The rules defined in [Setting up CPLEX on Windows](https://www.ibm.com/support/knowledgecenter/SSSA5P_12.7.1/ilog.odms.cplex.help/CPLEX/GettingStarted/topics/set_up/Windows.html) can be used to compile it on Windows environment.

### Compiling [LPCreator.h](LPCreator.h)

[LPCreator.h](LPCreator.h) is a single header-only library. It only depends on the [C++ standard library](http://en.cppreference.com/w/cpp/header), you don't need any 3rd party dependencies. To compile a `main.cpp` file that uses `LPCreator.h`:

```
g++ main.cpp -o main -std=c++11
```

[ILP.h](ILP.h)  Example usage 
-------------

[ILP.h](ILP.h) provides the class 

```c++
ILP(std::string dataset)
```
where 
 - `dataset` - name of the input dataset file
 
`ILP` class has a method `ILP::execute` that receives as parameter the type of ILP model you wish to run:
 - `0` - Runs the complete model (GM) 
 - `1` - Runs model GM1
 - `2` - Runs model GM2
 - `3` - Runs model GM3
 
 The following code excerpt shows how to run the complete model (GM) on the dataset [1-Lenses.txt](data/1-Lenses.txt)

```c++
#include "ILP.h"

using namespace std;
using namespace qualitativeclustering;

int main()
{
    ILP ilp("data/1-Lenses.txt");
    cout << ilp.execute(0);
}
```

The output will be
```
Objective = 72
Constraints = 6072
Time Solve = 0.446455
Groups = 0 1 0 1 2 2 3 3 0 1 0 1 2 2 3 3 0 1 0 1 2 2 3 3
```

[LPCreator.h](LPCreator.h) Example usage
-------------

[LPCreator.h](LPCreator.h) provides the method to create ILP models in LP format

```cpp
void createLP(std::string dataset, std::string LPfilename, int modelType)
```
where 
 - `dataset` - name of the input dataset file
 - `LPfilename` - name of the output LP file
 - `modeltype` - type of model to create:
   - `0` - Create the complete model (GM)
   - `1` - Create GM1 model
   - `2` - Create GM2 model
   - `3` - Create GM3 model

The following code excerpt shows how to create an LP file that represents the complete model (GM) for the dataset [1-Lenses.txt](data/1-Lenses.txt):

```c++
#include "LPCreator.h"

using namespace std;
using namespace qualitativeclustering;

int main()
{
    createLP("data/1-Lenses.txt", "1-Lenses-GM.lp", 0, 0);
}
```
The output will be a file in [CPLEX LP format](https://www.ibm.com/support/knowledgecenter/SSSA5P_12.5.0/ilog.odms.cplex.help/CPLEX/FileFormats/topics/LP.html) that can be used on any compatible solver. 
