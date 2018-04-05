# Qualitative Clustering via Integer Linear Programming

This repository presents the C++ implementation of the preprocessing techniques presented in the article _"A new preprocessing technique for ILP qualitative data clustering"_. Two header libraries are avaiable:

* [ILP.h](ILP.h): creates the ILP models and run on [IBM's CPLEX Optimizer™](http://www.ibm.com/software/commerce/optimization/cplex-optimizer/)

* [LPCreator.h](LPCreator.h): creates the ILP models on [CPLEX LP format](https://www.ibm.com/support/knowledgecenter/SSSA5P_12.5.0/ilog.odms.cplex.help/CPLEX/FileFormats/topics/LP.html), so it can be run on any compatible solver.

Installation
------------

[ILP.h](ILP.h) depends on [IBM's CPLEX Optimizer™](http://www.ibm.com/software/commerce/optimization/cplex-optimizer/) 12>. You can follow the rules defined in [Setting up CPLEX on GNU/Linux](https://www.ibm.com/support/knowledgecenter/SSSA5P_12.7.1/ilog.odms.cplex.help/CPLEX/GettingStarted/topics/set_up/GNU_Linux.html)
to compile it on Linux or [Setting up CPLEX on Windows](https://www.ibm.com/support/knowledgecenter/SSSA5P_12.7.1/ilog.odms.cplex.help/CPLEX/GettingStarted/topics/set_up/Windows.html) to compile it on Windows environment.

[LPCreator.h](LPCreator.h) is a single header-only library. Only depends on [C++ standard library](http://en.cppreference.com/w/cpp/header), you don't need any 3rd party dependencies.

[ILP.h](ILP.h)  Example usage 
-------------

`ILP` class has a method `execute` that receives as parameter the type of preprocessing technique you wish to run:
 - `0` - Runs the complete model (GM) without any preprocessing technique
 - `1` - Runs the technique (GM1)
 - `2` - Runs the technique (GM2)
 - `3` - Runs the technique (GM3)
 
 The following code excerpt shows how to run the complete model (GM) on the dataset `1-Lenses.txt`

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

```c++
#include "LPCreator.h"

using namespace std;
using namespace qualitativeclustering;

int main()
{
    // Create GM model for dataset "1-Lenses.txt" in LP format
    createLP("data/1-Lenses.txt", "1-Lenses-GM.lp", 0);
}
```
The output will be a file in [CPLEX LP format](https://www.ibm.com/support/knowledgecenter/SSSA5P_12.5.0/ilog.odms.cplex.help/CPLEX/FileFormats/topics/LP.html) that can be used on any compatible solver. 
