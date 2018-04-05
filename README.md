# Qualitative Clustering via Integer Linear Programming
Qualitative Data Clustering via Integer Linear Programming

[ILP.h](ILP.h)

You can create LP ([CPLEX LP format](https://www.ibm.com/support/knowledgecenter/SSSA5P_12.5.0/ilog.odms.cplex.help/CPLEX/FileFormats/topics/LP.html)) 

The following 

Installation
------------

[ILP.h](ILP.h) 

[LPCreator.h](LPCreator.h) only depends on [C++ standard library](http://en.cppreference.com/w/cpp/header), you don't need any 3rd party dependencies.


Example usage
-------------

```c++
#include "ILP.h"

using namespace std;
using namespace qualitativeclustering;

int main()
{
    // Run ILP for GM model
    ILP ilp("data/1-Lenses.txt");
    s = ilp.execute(0);
    cout << s;
}
```

The output will be
```
Objective = 72
Constraints = 6072
Time Solve = 0.446455
Groups = 0 1 0 1 2 2 3 3 0 1 0 1 2 2 3 3 0 1 0 1 2 2 3 3
```




Example usage
-------------

[LPCreator.h](LPCreator.h) is a single header-only library.  

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
